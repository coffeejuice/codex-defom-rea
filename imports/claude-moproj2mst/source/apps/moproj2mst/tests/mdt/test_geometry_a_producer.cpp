// CP143: actual high-record query/fresh A allocation/empty-region cache prefix.
// ctest-labels: m2m-mdt mdt-viewer
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <limits>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>
#include <vector>
#include <process.h>
#include <string>
#include <defdbio/db_query.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/layout/state_var_base.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/db_directory_receipt.hpp>
#include "m2m/engine.hpp"
#include "../../src/engine/engine_internal.hpp"
#include "../../src/engine/geometry_temp_prefix.hpp"

using namespace m2m;
namespace fs = std::filesystem;

namespace m2m::engine::detail {
using GeometrySizingLookupObserver = void (*)(const void*, std::int64_t, bool, const def::model::StateVarBase*);
extern GeometrySizingLookupObserver geometrySizingLookupObserverForTest;
using GeometryRecipientAppendObserver = void (*)(std::size_t, std::size_t, std::int32_t);
extern GeometryRecipientAppendObserver geometryRecipientAppendObserverForTest;
}

namespace {
struct RestoreGlobals {
    std::int32_t dimension = def::model::g_index_tables_dim;
    std::array<std::int32_t, 53> mar{};
    std::array<std::int32_t, 129> arrays{};
    std::unique_ptr<def::model::StepTimer> node =
        std::make_unique<def::model::StepTimer>(def::model::g_timer_read_obj_node_state_variable);
    std::unique_ptr<def::model::StepTimer> timer =
        std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    RestoreGlobals()
    {
        for (std::size_t i = 0; i < mar.size(); ++i) mar[i] = def::model::g_mar_index[i];
        for (std::size_t i = 0; i < arrays.size(); ++i) arrays[i] = def::model::g_a_index[i];
    }
    ~RestoreGlobals()
    {
        for (std::size_t i = 0; i < mar.size(); ++i) def::model::g_mar_index[i] = mar[i];
        for (std::size_t i = 0; i < arrays.size(); ++i) def::model::g_a_index[i] = arrays[i];
        def::model::g_index_tables_dim = dimension;
        def::model::g_timer_read_obj_node_state_variable = *node;
        def::model::g_timer_read_arrays = *timer;
    }
};

struct Database {
    Project project;
    std::unique_ptr<ProjectScope> scope;
    fs::path directory;
    explicit Database(const char* tag)
    {
        directory = m2m_test::scratchRoot() / ("a_producer_" + std::to_string(_getpid()) + "_" + tag);
        fs::create_directories(directory);
        const auto file = directory / "X.DB";
        fs::copy_file(m2m_test::configuredPath("M2M_DB_FIXTURE") / "COG_P1_B1.DB", file,
            fs::copy_options::overwrite_existing);
        scope = std::make_unique<ProjectScope>(project);
        auto* db = engine::projectDb(project);
        REQUIRE(def::dbio::load_db_info(*db->reader, file.string().c_str(), false) == 0);
        const auto receipt = def::model::directory_receipts::current(db, *db->reader);
        REQUIRE(receipt.has_value()); REQUIRE(receipt->elementCount == std::size_t{32});
    }
    ~Database() { std::error_code error; fs::remove_all(directory, error); }
    engine::DbHandle* handle() { return project.dataDb.get(); }
    def::model::DbDirRecord& record() { return *static_cast<def::model::DbDirRecord*>(handle()->reader->uad8); }
};

struct Candidate {
    engine::GuiModel model;
    def::model::Cls_14017ef20* nodes = nullptr;
    def::model::Cls_140182880* elements = nullptr;
    std::int32_t* ownedConnectivity = nullptr;
    explicit Candidate(engine::DbHandle* db, std::int32_t width = 4, std::int32_t metadata = 2)
        : model(db, 3)
    {
        auto* object = model.objectWrap(1, true, false); REQUIRE(object);
        def::model::geo_kw::ensure_geo_list(*object);
        if (def::model::geo_kw::geometry_count(*object) == 0)
            def::model::geo_kw::geo_list_append_default(def::model::geo_kw::geo_list(*object));
        auto* geometry = def::model::geo_kw::geometry(*object, 0); REQUIRE(geometry);
        auto* curve = geometry->add_edge_curve(); REQUIRE(curve);
        curve->curve_type = 1; curve->n_points = 2;
        auto* points = curve->allocate_points(); REQUIRE(points);
        const double line[6] = {-2.0, 0.0, 0.0, 2.0, 0.0, 0.0}; std::memcpy(points, line, sizeof line);
        engine::viewerSync(model, object, true);
        geometry->mesh = new def::model::OBJ_MESH2D();
        nodes = reinterpret_cast<def::model::Cls_14017ef20*>(geometry->mesh->s008);
        elements = reinterpret_cast<def::model::Cls_140182880*>(geometry->mesh->s148);
        nodes->u008 = static_cast<double*>(std::calloc(8, sizeof(double)));
        elements->u008 = static_cast<std::int32_t*>(std::calloc(4, sizeof(std::int32_t)));
        REQUIRE(nodes->u008); REQUIRE(elements->u008);
        ownedConnectivity = elements->u008;
        const double xy[8] = {0.0, 0.0, 2.0, 0.0, 2.0, 1.0, 0.0, 1.0};
        const std::int32_t connectivity[4] = {1, 2, 3, 4};
        std::memcpy(nodes->u008, xy, sizeof xy); std::memcpy(elements->u008, connectivity, sizeof connectivity);
        nodes->n = 4; elements->n = 1; elements->m = width; elements->u0f8 = metadata;
        nodes->u130 = model.step.get();
        elements->u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(model.step.get());
        auto& step = *model.step;
        step.db = db->reader.get(); step.u740 = 0; step.step_version = 6000;
        step.u1b58 = 0; step.u1b59 = 0; step.u26c4[11] = 0;
        step.u1b68 = 12; step.u1b6c = 20; step.u1b74 = 777; step.u2b24 = 1; step.u2b28[11] = 0;
        REQUIRE(step.u1b78 == nullptr); REQUIRE(step.u1b88 == nullptr);
        def::model::g_timer_read_obj_node_state_variable.count = 7;
    }
    ~Candidate()
    {
        if (nodes) nodes->u130 = nullptr;
        if (elements) { elements->u0f0 = nullptr; elements->u008 = ownedConnectivity; }
        model.db = nullptr; model.step->db = nullptr;
    }
};

struct Trace {
    def::model::Cls_14017ef20* nodes;
    std::size_t count = 0;
    std::vector<std::int32_t> nodeCounts;
    std::function<void(std::size_t)> before = {};
    std::function<void(std::uint64_t, const std::uint64_t*, int)> onCall = {};
};
Trace* active = nullptr;
std::uint64_t forward(std::uint64_t address, const std::uint64_t* args, int nargs)
{
    namespace ext = def::model::llobj::ext;
    std::array<std::uint64_t, 4> copied{};
    for (int i = 0; i < nargs && i < 4; ++i) copied[static_cast<std::size_t>(i)] = args[i];
    const bool record = active && address == 0x140329e60 && nargs == 4 && copied[0] == ext::u(active->nodes);
    const auto nodeCount = def::model::g_timer_read_obj_node_state_variable.count;
    if (record && active->before) active->before(active->count);
    if (active && active->onCall) active->onCall(address, copied.data(), nargs);
    struct RestoreHook {
        ext::Hook hook = ext::g_hook;
        RestoreHook() { ext::g_hook = nullptr; }
        ~RestoreHook() { ext::g_hook = hook; }
    } restore;
    const auto result = ext::call(address, copied[0], copied[1], copied[2], copied[3]);
    if (record) { ++active->count; active->nodeCounts.push_back(nodeCount); }
    return result;
}
struct ScopedTrace {
    explicit ScopedTrace(Trace& trace)
    {
        REQUIRE(active == nullptr); REQUIRE(def::model::llobj::ext::g_hook == nullptr);
        active = &trace; def::model::llobj::ext::g_hook = forward;
    }
    ~ScopedTrace() { active = nullptr; def::model::llobj::ext::g_hook = nullptr; }
};

struct SizingObservation {
    const void* nodes;
    std::vector<std::int64_t> before, after;
    std::vector<const def::model::StateVarBase*> receivers;
    std::function<void(std::size_t, bool)> adversarialMutation = {};
    const void* secondNodes = nullptr;
    const void* thirdNodes = nullptr;
    std::vector<const void*> afterNodes;
    std::function<void(const void*, std::size_t, bool)> adversarialByNodes = {};
    std::vector<const void*> additionalNodes;
};
SizingObservation* sizingObservation = nullptr;
void observeSizingLookup(const void* nodes, std::int64_t ordinal, bool before,
                        const def::model::StateVarBase* receiver)
{
    if (!sizingObservation) return;
    if (sizingObservation->nodes != nodes && sizingObservation->secondNodes != nodes && sizingObservation->thirdNodes != nodes &&
        std::find(sizingObservation->additionalNodes.begin(), sizingObservation->additionalNodes.end(), nodes) == sizingObservation->additionalNodes.end()) return;
    if (before) sizingObservation->before.push_back(ordinal);
    else {
        sizingObservation->after.push_back(ordinal); sizingObservation->receivers.push_back(receiver);
        sizingObservation->afterNodes.push_back(nodes);
    }
    if (sizingObservation->adversarialMutation)
        sizingObservation->adversarialMutation(static_cast<std::size_t>(ordinal), before);
    if (sizingObservation->adversarialByNodes)
        sizingObservation->adversarialByNodes(nodes, static_cast<std::size_t>(ordinal), before);
}
struct ScopedSizingObservation {
    explicit ScopedSizingObservation(SizingObservation& observation)
    {
        REQUIRE(sizingObservation == nullptr);
        REQUIRE(engine::detail::geometrySizingLookupObserverForTest == nullptr);
        sizingObservation = &observation;
        engine::detail::geometrySizingLookupObserverForTest = observeSizingLookup;
    }
    ~ScopedSizingObservation()
    {
        engine::detail::geometrySizingLookupObserverForTest = nullptr;
        sizingObservation = nullptr;
    }
};

struct AppendObservation {
    std::vector<std::size_t> counts, capacities;
    std::vector<std::int32_t> ordinals;
    std::function<void(std::size_t, std::int32_t)> adversarialMutation;
};
AppendObservation* appendObservation = nullptr;
void observeAppend(std::size_t count, std::size_t capacity, std::int32_t ordinal)
{
    if (!appendObservation) return;
    appendObservation->counts.push_back(count); appendObservation->capacities.push_back(capacity);
    appendObservation->ordinals.push_back(ordinal);
    if (appendObservation->adversarialMutation) appendObservation->adversarialMutation(count, ordinal);
}
struct ScopedAppendObservation {
    explicit ScopedAppendObservation(AppendObservation& observation)
    {
        REQUIRE(appendObservation == nullptr); REQUIRE(engine::detail::geometryRecipientAppendObserverForTest == nullptr);
        appendObservation = &observation; engine::detail::geometryRecipientAppendObserverForTest = observeAppend;
    }
    ~ScopedAppendObservation()
    {
        engine::detail::geometryRecipientAppendObserverForTest = nullptr; appendObservation = nullptr;
    }
};

void refresh(engine::GuiModel& model)
{
    try { engine::viewerRefresh(model); }
    catch (const Error& error) { if (error.code != M2M_E_UNSUPPORTED) throw; }
}
bool unknownScene(engine::GuiModel& model)
{
    bool valid = false; float mn[3] = {}, mx[3] = {};
    try { engine::viewerSceneBox(model, valid, mn, mx); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
void checkScene(engine::GuiModel& model, float height = 1.0f)
{
    bool valid = false; float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(model, valid, mn, mx)); CHECK(valid);
    CHECK(mn[0] == -2.0f && mx[0] == 2.0f); CHECK(mn[1] == 0.0f && mx[1] == height);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
}
def::model::LLOBJ_DATA* freshMain(Candidate& candidate, std::int32_t id = 2)
{
    auto& step = *candidate.model.step;
    auto* object = def::model::llobj_ctor::get_or_create(&step, id); REQUIRE(object);
    REQUIRE(object->owner == &step); REQUIRE(def::model::llobj_not_kind7(object));
    object->u87c[0] = 1; object->u87c[1] = 1;
    *engine::at<std::int32_t>(object, 0x3984) = 1;
    *engine::at<std::uint8_t>(object, 0x3610) = 0;
    step.u15f8[0] = 3; step.u1dc0[151] = 3;
    const std::int32_t dimension = 3; def::model::select_index_tables(&dimension);
    auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
    REQUIRE(nodes->u008 == nullptr); REQUIRE(nodes->n == 0);
    for (const auto* pointer : nodes->u0f0) REQUIRE(pointer == nullptr);
    for (const auto* pointer : nodes->u010) REQUIRE(pointer == nullptr);
    nodes->u13c = 9; // Observable native reset marker, not a numeric data seed.
    return object;
}
void zeroOrdinaryWidths(def::model::DB_STEP_DATA& step)
{
    for (const auto& entry : def::model::Cls_14017ef20::kRegs)
        if (entry.ctor == 0x14032a990) step.u15f8[entry.id] = 0;
}
} // namespace

TEST_CASE(geometry_high_empty_region_publishes_exact_zero_A_then_parks_before_assignment)
{
    Database database("producer");
    for (const std::int32_t version : {6000, 11000})
    for (const std::int32_t maximum : {0, 1, 4})
    for (const std::int32_t width : {0, 4})
    for (const std::int32_t metadata : {2, 3}) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle(), width, metadata); auto& step = *candidate.model.step;
        step.u2b24 = maximum;
        auto& arrays = def::model::g_timer_read_arrays; arrays.count = -1; // Genuine unresolved caller guard.
        auto beforeArrays = std::make_unique<def::model::StepTimer>(arrays);
        auto* const identity = candidate.model.step.get(); auto* const db = step.db;
        auto* const coordinates = candidate.nodes->u008; auto* const connectivity = candidate.elements->u008;
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(step.u1b78 != nullptr);
        auto* const produced = static_cast<double*>(step.u1b78);
        for (std::int32_t i = 0; i < maximum + 2; ++i) CHECK(produced[i] == 0.0);
        CHECK(step.step_version == version); CHECK(step.u26c4[11] == 1);
        CHECK(step.u1b74 == 777); CHECK(step.u1b68 == 12); CHECK(step.u1b6c == 20);
        CHECK(step.u1b88 == nullptr); CHECK(step.db == db); CHECK(candidate.model.step.get() == identity);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(std::memcmp(&arrays, beforeArrays.get(), sizeof arrays) == 0);
        CHECK(unknownScene(candidate.model));
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(step.u1b78 == produced);
        step.u26c4[11] = 0; // Declared raw mutation must not turn the parked frame into a resume.
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(step.u1b78 == produced); CHECK(unknownScene(candidate.model));
        bool refusedMutation = false;
        try { candidate.model.objectWrap(2, true, false); }
        catch (const Error& error) { refusedMutation = error.code == M2M_E_UNSUPPORTED; }
        CHECK(refusedMutation); CHECK(step.u1378 == 1);
        CHECK(candidate.nodes->u008 == coordinates); CHECK(candidate.elements->u008 == connectivity);
        CHECK(candidate.nodes->u008[5] == 1.0); CHECK(candidate.elements->u008[2] == 3);
        CHECK(std::memcmp(&arrays, beforeArrays.get(), sizeof arrays) == 0);
        step.u26c4[11] = 1;
    }
}

TEST_CASE(geometry_high_A_producer_keeps_other_high_profiles_at_their_existing_guard)
{
    Database database("guards"); database.record().version = 11000;
    for (int mode = 0; mode < 9; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        switch (mode) {
        case 0: step.u1b68 = 0; break;
        case 1: step.u1b6c = 0; break;
        case 2: step.u2b24 = -1; break;
        case 3: step.u2b24 = std::numeric_limits<std::int32_t>::max(); break;
        case 4: step.u2b24 = std::numeric_limits<std::int32_t>::max() - 1; break;
        case 5: step.u2b28[11] = 1; break;
        case 6: step.u1b78 = def::model::bc::calloc_doubles(2); REQUIRE(step.u1b78); break;
        case 7: step.u740 = 32; break;
        case 8: candidate.model.db = nullptr; break;
        }
        const auto* previous = step.u1b78;
        auto& arrays = def::model::g_timer_read_arrays; arrays.count = -1; // Keep restoration at the producer boundary.
        auto beforeArrays = std::make_unique<def::model::StepTimer>(arrays);
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); CHECK(step.u1b78 == previous); CHECK(step.u26c4[11] == 0);
        CHECK(unknownScene(candidate.model)); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(std::memcmp(&arrays, beforeArrays.get(), sizeof arrays) == 0);
        if (mode == 6) { std::free(step.u1b78); step.u1b78 = nullptr; }
        step.u1b68 = 12; step.u1b6c = 20; step.u2b24 = 1; step.u2b28[11] = 0; step.u740 = 0;
        candidate.model.db = database.handle();
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 2u); REQUIRE(step.u1b78 != nullptr); CHECK(step.u26c4[11] == 1);
        CHECK(step.step_version == 11000); CHECK(unknownScene(candidate.model));
        CHECK(std::memcmp(&arrays, beforeArrays.get(), sizeof arrays) == 0);
    }
}

TEST_CASE(geometry_cached_high_owner_keeps_its_existing_two_getter_path_without_A_producer)
{
    Database database("cached"); database.record().version = 11000;
    RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
    step.u26c4[11] = 1; def::model::g_timer_read_arrays.count = 0;
    Trace trace{candidate.nodes};
    { ScopedTrace scope(trace); refresh(candidate.model); }
    REQUIRE(trace.count == 2u); CHECK(step.u1b78 == nullptr); CHECK(step.step_version == 6000);
    CHECK(def::model::g_timer_read_obj_node_state_variable.count == 7);
    CHECK(def::model::g_timer_read_arrays.count == 0);
    bool valid = false; float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(candidate.model, valid, mn, mx)); CHECK(valid); CHECK(mx[1] == 1.0f);
}

TEST_CASE(geometry_high_producer_retires_its_exact_frame_then_runs_only_cached_getters)
{
    Database database("retire");
    for (const std::int32_t version : {std::int32_t{6000}, std::int32_t{10999}, std::int32_t{11000},
        std::numeric_limits<std::int32_t>::max()})
    for (const std::int32_t width : {0, 4})
    for (const std::int32_t metadata : {2, 3})
    for (const bool populated : {false, true}) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle(), width, metadata); auto& step = *candidate.model.step;
        if (!populated) candidate.elements->u008 = nullptr;
        auto& arrays = def::model::g_timer_read_arrays;
        arrays.count = version == 11000 && width == 0 && metadata == 3 ? 100 : 0;
        const auto initialArrays = arrays.count;
        auto* const identity = candidate.model.step.get();
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        const std::size_t rounds = populated ? 2u : 1u;
        REQUIRE(trace.count == rounds); REQUIRE(step.u1b78 != nullptr);
        CHECK(trace.nodeCounts[0] == 7);
        if (populated) CHECK(trace.nodeCounts[1] == 3);
        auto* const produced = static_cast<double*>(step.u1b78);
        CHECK(produced[0] == 0.0 && produced[1] == 0.0 && produced[2] == 0.0);
        CHECK(step.u26c4[11] == 1); CHECK(step.step_version == version); CHECK(step.u1b74 == 777);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
        CHECK(arrays.count == (initialArrays < 100 ? initialArrays + 1 : initialArrays));
        CHECK(std::string(def::model::g_timer_read_obj_node_state_variable.labels[2]) == "Stop");
        checkScene(candidate.model, populated ? 1.0f : 0.0f);
        CHECK(candidate.model.objectWrap(1, false, false) != nullptr); // Exact frame was retired.
        auto beforeArrays = std::make_unique<def::model::StepTimer>(arrays);
        database.record().version = 1; // A cached refresh must not query this changed record.
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 2u * rounds);
        for (std::size_t i = rounds; i < trace.nodeCounts.size(); ++i) CHECK(trace.nodeCounts[i] == 3);
        CHECK(step.u1b78 == produced); CHECK(step.step_version == version); CHECK(step.u26c4[11] == 1);
        CHECK(candidate.model.step.get() == identity);
        CHECK(std::memcmp(&arrays, beforeArrays.get(), sizeof arrays) == 0);
        checkScene(candidate.model, populated ? 1.0f : 0.0f);
    }
}

TEST_CASE(geometry_high_nonzero_main_and_assignment_failures_keep_producer_frame_parked)
{
    Database database("assignment"); database.record().version = 11000;
    for (int mode = 0; mode < 4; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        engine::GuiModel foreign(nullptr, 3);
        auto* main = def::model::llobj_ctor::get_or_create(&step, 2); REQUIRE(main);
        REQUIRE(main->owner == &step); REQUIRE(def::model::llobj_not_kind7(main));
        struct RestoreMain {
            def::model::LLOBJ_DATA& object;
            def::model::DB_STEP_DATA* owner;
            std::int32_t gate;
            ~RestoreMain() { object.owner = owner; object.u87c[0] = gate; }
        } restoreMain{*main, main->owner, main->u87c[0]};
        *engine::at<std::uint8_t>(main, 0x3610) = 0;
        const std::int32_t dimension = 3; def::model::select_index_tables(&dimension);
        double* mainCoordinates = nullptr;
        if (mode == 0) {
            step.u15f8[0] = 3; step.u1dc0[151] = 3;
            main->u87c[0] = 1; main->u87c[1] = 1;
            def::model::bc::alloc_node_arrays(*main, step.u15f8);
            mainCoordinates = *engine::at<double*>(main, 0x8a8); REQUIRE(mainCoordinates != nullptr);
            mainCoordinates[0] = 11.0; mainCoordinates[1] = 12.0; mainCoordinates[2] = 13.0;
        } else if (mode == 1) main->owner = foreign.step.get();
        else if (mode == 2) def::model::g_mar_index[27] = 0x8c;
        else { def::model::g_a_index[27] = 11; step.u15f8[0x1a] = 1; }
        auto& arrays = def::model::g_timer_read_arrays; arrays.count = 0;
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(step.u1b78 != nullptr); CHECK(step.u26c4[11] == 1);
        CHECK(arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        if (mainCoordinates) CHECK(mainCoordinates[0] == 11.0 && mainCoordinates[1] == 12.0 && mainCoordinates[2] == 13.0);
        CHECK(unknownScene(candidate.model)); auto* const produced = step.u1b78;
        main->owner = &step; main->u87c[0] = 0;
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(step.u1b78 == produced); CHECK(unknownScene(candidate.model));
        CHECK(arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
    }
}

TEST_CASE(geometry_high_cached_builder_rechecks_after_COW_on_initial_and_fresh_refreshes)
{
    Database database("cached_guard"); database.record().version = 11000;
    for (const bool freshCached : {false, true})
    for (int mode = 0; mode < 3; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        engine::GuiModel foreign(nullptr, 3); def::model::g_timer_read_arrays.count = 0;
        if (freshCached) { refresh(candidate.model); checkScene(candidate.model); }
        Trace trace{candidate.nodes}; bool sawFirstStop = false;
        trace.before = [&](std::size_t ordinal) {
            if (ordinal != 1) return;
            sawFirstStop = def::model::g_timer_read_obj_node_state_variable.count == 3 &&
                def::model::g_timer_read_arrays.count == 1;
            if (mode == 0) step.u26c4[11] = 0;
            else if (mode == 1) step.step_version = 5999;
            else candidate.elements->u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(foreign.step.get());
        };
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 2u); CHECK(sawFirstStop); CHECK(unknownScene(candidate.model));
        auto* const produced = step.u1b78; REQUIRE(produced != nullptr);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
        step.u26c4[11] = 1; step.step_version = 11000;
        candidate.elements->u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step); trace.before = {};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 4u); CHECK(step.u1b78 == produced); checkScene(candidate.model);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
    }
}

TEST_CASE(geometry_high_late_builder_refusal_does_not_reinstate_the_retired_frame)
{
    Database database("layout"); database.record().version = 11000;
    for (int mode = 0; mode < 3; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle());
        def::model::g_timer_read_arrays.count = 0;
        if (mode == 0) candidate.nodes->u138 = 3;
        else if (mode == 1) candidate.nodes->u008[0] = std::numeric_limits<double>::quiet_NaN();
        else candidate.elements->u008[2] = 5;
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 2u); CHECK(unknownScene(candidate.model));
        CHECK(candidate.model.objectWrap(1, false, false) != nullptr);
        auto* const produced = candidate.model.step->u1b78; REQUIRE(produced != nullptr);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
        candidate.nodes->u138 = 2; candidate.nodes->u008[0] = 0.0; candidate.elements->u008[2] = 3;
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 4u); checkScene(candidate.model);
        CHECK(candidate.model.step->u1b78 == produced); CHECK(def::model::g_timer_read_arrays.count == 1);
    }
}

TEST_CASE(geometry_high_fresh_main_publishes_owned_coordinates_then_stops_before_sizing)
{
    Database database("main_publish");
    for (const std::int32_t version : {6000, 11000})
    for (const std::int32_t maximum : {1, 4})
    for (const std::int32_t width : {0, 4})
    for (const std::int32_t metadata : {2, 3}) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle(), width, metadata); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        step.u15f8[0x12] = -1; // Genuine guard keeps this cohort at coordinate publication.
        step.u2b24 = maximum; def::model::g_timer_read_arrays.count = 0;
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(step.u1b78 != nullptr); REQUIRE(nodes->u008 != nullptr);
        auto* const source = static_cast<double*>(step.u1b78); auto* const destination = nodes->u008;
        CHECK(destination != source);
        CHECK(destination[0] == 0.0 && destination[1] == 0.0 && destination[2] == 0.0);
        CHECK(nodes->n == 0); CHECK(nodes->u13c == 0);
        for (const auto* pointer : nodes->u0f0) CHECK(pointer == nullptr);
        for (const auto* pointer : nodes->u010) CHECK(pointer == nullptr);
        CHECK(object->u87c[0] == 1 && object->u87c[1] == 1);
        CHECK(object->owner == &step); CHECK(step.step_version == version); CHECK(step.u26c4[11] == 1);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
        object->u87c[0] = 0; // Repair cannot resume a partially allocated native call.
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(nodes->u008 == destination); CHECK(step.u1b78 == source);
        CHECK(nodes->n == 0); CHECK(unknownScene(candidate.model));
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
    }
}

TEST_CASE(geometry_high_main_publication_rejects_unqualified_storage_before_reset)
{
    Database database("main_guards"); database.record().version = 11000;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    for (int mode = 0; mode < 12; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        auto& variables = *reinterpret_cast<Variables*>(nodes);
        auto* variable = variables.items().front(); REQUIRE(variable);
        std::unique_ptr<Variables> collectionLease;
        std::unique_ptr<def::model::CowVector<double>> valueLease;
        engine::GuiModel foreign(nullptr, 3); double* foreignSlot = nullptr;
        struct RestoreBinding {
            def::model::StateVarBase& variable;
            void* slot;
            def::model::LLOBJ_DATA& object;
            def::model::DB_STEP_DATA* owner;
            ~RestoreBinding() { variable.u30 = slot; object.owner = owner; }
        } restoreBinding{*variable, variable->u30, *object, object->owner};
        switch (mode) {
        case 0: step.u2b24 = 0; break; // Actual source allocation is only two doubles.
        case 1:
            def::model::bc::alloc_node_arrays(*object, step.u15f8);
            REQUIRE(nodes->u008); nodes->u008[0] = 11.0; nodes->u008[1] = 12.0; nodes->u008[2] = 13.0;
            nodes->u13c = 9; break;
        case 2: object->u87c[1] = 2; break;
        case 3: *engine::at<std::int32_t>(object, 0x3984) = 2; break;
        case 4: step.u1dc0[151] = 2; break;
        case 5: def::model::g_a_index[12] = 11; break;
        case 6: def::model::g_a_index[70] = -1; break;
        case 7: variable->values.push_back(42.0); break;
        case 8: collectionLease = std::make_unique<Variables>(variables); break;
        case 9: valueLease = std::make_unique<def::model::CowVector<double>>(variable->values); break;
        case 10: variable->u30 = &foreignSlot; break;
        case 11: object->owner = foreign.step.get(); break;
        }
        auto* const priorDestination = nodes->u008; const auto* const priorBody = variables.body();
        def::model::g_timer_read_arrays.count = 0; Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(step.u1b78 != nullptr);
        CHECK(nodes->u008 == priorDestination); CHECK(nodes->u13c == 9); CHECK(variables.body() == priorBody);
        if (priorDestination) CHECK(priorDestination[0] == 11.0 && priorDestination[1] == 12.0 && priorDestination[2] == 13.0);
        if (mode == 7) CHECK(variable->values.size() == 1u);
        CHECK(foreignSlot == nullptr); CHECK(step.u26c4[11] == 1);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_high_main_clear_callback_changes_preserve_reset_before_publication_guard)
{
    Database database("main_reset"); database.record().version = 11000;
    for (int mode = 0; mode < 4; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        engine::GuiModel foreign(nullptr, 3);
        struct RestoreOwner {
            def::model::LLOBJ_DATA& object; def::model::DB_STEP_DATA* owner;
            ~RestoreOwner() { object.owner = owner; }
        } restoreOwner{*object, object->owner};
        def::model::g_timer_read_arrays.count = 0; bool sawReset = false;
        Trace trace{candidate.nodes};
        trace.onCall = [&](std::uint64_t address, const std::uint64_t* args, int nargs) {
            if (address != 0x140329dc0 || nargs < 1 || args[0] != def::model::llobj::ext::u(nodes)) return;
            sawReset = nodes->u13c == 0 && step.u1b78 != nullptr && step.u26c4[11] == 1;
            if (mode == 0) object->u87c[1] = 2;
            else if (mode == 1) step.u15f8[0] = 2;
            else if (mode == 2) step.u2b24 = 0;
            else object->owner = foreign.step.get();
        };
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); CHECK(sawReset); REQUIRE(step.u1b78 != nullptr);
        CHECK(nodes->u008 == nullptr); CHECK(nodes->n == 0); CHECK(nodes->u13c == 0);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2); CHECK(unknownScene(candidate.model));
        object->owner = &step; object->u87c[1] = 1; step.u15f8[0] = 3; step.u2b24 = 1; trace.onCall = {};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(nodes->u008 == nullptr); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_high_main_BC_attempts_publish_all_six_then_count_without_variable_sizing)
{
    Database database("main_bc");
    const std::int32_t patterns[4][6] = {{0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 1, 1},
        {0, 1, 2, 3, 4, 5}, {5, 4, 3, 2, 1, 0}};
    for (const std::int32_t version : {6000, 11000})
    for (const std::int32_t maximum : {1, 4})
    for (const auto& counts : patterns) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        for (std::size_t i = 0; i < 6; ++i) step.u15f8[0x12 + i] = counts[i];
        step.u15f8[1] = 1; // Full sizing stays guarded; the exact scalar profile may commit dimensions before allocation.
        step.u2b24 = maximum; def::model::g_timer_read_arrays.count = 0;
        Trace trace{candidate.nodes}; std::size_t sizingCalls = 0;
        trace.onCall = [&](std::uint64_t address, const std::uint64_t* args, int nargs) {
            if (address == 0x140329e60 && nargs > 0 && args[0] == def::model::llobj::ext::u(nodes)) ++sizingCalls;
        };
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(nodes->u008 != nullptr); REQUIRE(step.u1b78 != nullptr);
        auto* const primary = nodes->u008; auto* const source = step.u1b78;
        std::int32_t* pointers[6];
        for (std::size_t i = 0; i < 6; ++i) {
            pointers[i] = nodes->u0f0[i];
            if (counts[i] == 0) CHECK(pointers[i] == nullptr);
            else {
                REQUIRE(pointers[i] != nullptr);
                for (std::int32_t j = 0; j < counts[i]; ++j) CHECK(pointers[i][j] == 0);
            }
            CHECK(step.u15f8[0x12 + i] == counts[i]);
        }
        CHECK(nodes->u0f0[6] == nullptr); CHECK(nodes->n == 1); CHECK(nodes->u13c == 0);
        CHECK(nodes->u128 == 300); CHECK(nodes->u138 == 3); CHECK(sizingCalls == 0u);
        using Variables = def::model::CowList<def::model::StateVarBase*>;
        for (const auto* variable : reinterpret_cast<const Variables*>(nodes)->items()) {
            CHECK(variable->values.size() == 0u); CHECK(variable->dim0(false) == 0);
        }
        for (const auto* pointer : nodes->u010) CHECK(pointer == nullptr);
        CHECK(step.u26c4[11] == 1); CHECK(step.step_version == version);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2); CHECK(unknownScene(candidate.model));
        object->u87c[0] = 0;
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(nodes->u008 == primary); CHECK(step.u1b78 == source);
        for (std::size_t i = 0; i < 6; ++i) CHECK(nodes->u0f0[i] == pointers[i]);
        CHECK(nodes->n == 1); CHECK(sizingCalls == 0u); CHECK(unknownScene(candidate.model));
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
    }
}

TEST_CASE(geometry_high_negative_BC_count_preserves_coordinate_only_prefix_before_any_attempt)
{
    Database database("bc_count_guard"); database.record().version = 11000;
    for (const std::size_t negativeIndex : {std::size_t{0}, std::size_t{5}}) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        for (std::size_t i = 0; i < 6; ++i) step.u15f8[0x12 + i] = static_cast<std::int32_t>(i + 1);
        step.u15f8[0x12 + negativeIndex] = -1; def::model::g_timer_read_arrays.count = 0;
        Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(nodes->u008 != nullptr); REQUIRE(step.u1b78 != nullptr);
        auto* const primary = nodes->u008;
        CHECK(nodes->n == 0); CHECK(nodes->u13c == 0);
        for (const auto* pointer : nodes->u0f0) CHECK(pointer == nullptr);
        for (const auto* pointer : nodes->u010) CHECK(pointer == nullptr);
        CHECK(step.u26c4[11] == 1); CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2); CHECK(unknownScene(candidate.model));
        step.u15f8[0x12 + negativeIndex] = 1;
        { ScopedTrace scope(trace); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(nodes->u008 == primary); CHECK(nodes->n == 0);
        for (const auto* pointer : nodes->u0f0) CHECK(pointer == nullptr);
        CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_main_zero_size_dispatch_uses_actual_ordered_receivers_without_values_allocation)
{
    Database database("zero_sizing");
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    for (const std::int32_t version : {6000, 11000})
    for (const bool positiveBc : {false, true}) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        zeroOrdinaryWidths(step);
        for (std::size_t i = 0; i < 6; ++i) step.u15f8[0x12 + i] = positiveBc ? static_cast<std::int32_t>(i % 3) : 0;
        step.u15f8[0x1e] = 7; step.u15f8[0x1f] = 8; // Post widths are not read for key-1.
        const auto& variables = *reinterpret_cast<const Variables*>(nodes);
        REQUIRE(variables.size() == 28u);
        const auto* const initialBody = variables.body();
        std::array<const def::model::StateVarBase*, 28> receivers{};
        std::array<const void*, 28> valueBodies{};
        std::int32_t current[28][3], previous[28][3]; std::size_t ordinal = 0;
        for (auto* variable : variables.items()) {
            variable->cur[1] = static_cast<std::int32_t>(ordinal + 2);
            variable->cur[2] = static_cast<std::int32_t>(ordinal % 3 + 1);
            receivers[ordinal] = variable; valueBodies[ordinal] = variable->values.body();
            for (std::size_t dim = 0; dim < 3; ++dim) {
                current[ordinal][dim] = variable->cur[dim]; previous[ordinal][dim] = variable->prev[dim];
            }
            ++ordinal;
        }
        SizingObservation observation{nodes}; Trace trace{candidate.nodes};
        def::model::g_timer_read_arrays.count = 0;
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 2u); REQUIRE(observation.before.size() == 28u); REQUIRE(observation.after.size() == 28u);
        REQUIRE(observation.receivers.size() == 28u); CHECK(variables.body() == initialBody);
        ordinal = 0;
        for (const auto* variable : variables.items()) {
            CHECK(observation.before[ordinal] == static_cast<std::int64_t>(ordinal));
            CHECK(observation.after[ordinal] == static_cast<std::int64_t>(ordinal));
            CHECK(variable == receivers[ordinal]); CHECK(observation.receivers[ordinal] == receivers[ordinal]);
            const bool normal = def::model::Cls_14017ef20::kRegs[ordinal].ctor == 0x14032a990;
            for (std::size_t dim = 0; dim < 3; ++dim) {
                CHECK(variable->cur[dim] == (normal && dim < 2 ? (dim == 0 ? 1 : 0) : current[ordinal][dim]));
                CHECK(variable->prev[dim] == (normal && dim < 2 ? current[ordinal][dim] : previous[ordinal][dim]));
            }
            CHECK(variable->values.body() == valueBodies[ordinal]); CHECK(variable->values.size() == 0u);
            ++ordinal;
        }
        for (const auto* pointer : nodes->u010) CHECK(pointer == nullptr);
        REQUIRE(nodes->u008 != nullptr); CHECK(nodes->n == 1);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3); checkScene(candidate.model);
        auto* const primary = nodes->u008; auto* const source = step.u1b78;
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 4u); CHECK(observation.after.size() == 28u);
        CHECK(nodes->u008 == primary); CHECK(step.u1b78 == source); checkScene(candidate.model);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
    }
}

TEST_CASE(geometry_main_sizing_nonzero_width_retains_BC_and_guards_before_allocation)
{
    Database database("sizing_width_guard"); database.record().version = 11000;
    for (const std::int32_t width : {-1, 1}) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        zeroOrdinaryWidths(step); step.u15f8[1] = width; step.u15f8[0x12] = 2;
        using Variables = def::model::CowList<def::model::StateVarBase*>;
        const auto& variables = *reinterpret_cast<const Variables*>(nodes);
        auto* first = variables.items().front(); first->cur[1] = 7; first->prev[1] = -9;
        const auto* valueBody = first->values.body();
        def::model::g_timer_read_arrays.count = 0;
        SizingObservation observation{nodes}; Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u);
        if (width == 1) {
            REQUIRE(observation.before.size() == 1u); REQUIRE(observation.after.size() == 1u);
            CHECK(observation.before[0] == 0); CHECK(observation.after[0] == 0); CHECK(observation.receivers[0] == first);
            CHECK(first->cur[0] == 1 && first->prev[0] == 0); CHECK(first->cur[1] == 1 && first->prev[1] == 7);
        } else {
            CHECK(observation.before.empty()); CHECK(observation.after.empty());
            CHECK(first->cur[0] == 0); CHECK(first->cur[1] == 7 && first->prev[1] == -9);
        }
        CHECK(first->values.body() == valueBody); CHECK(first->values.size() == 0u); CHECK(nodes->u010[0] == nullptr);
        CHECK(nodes->n == 1); REQUIRE(nodes->u0f0[0] != nullptr);
        CHECK(nodes->u0f0[0][0] == 0 && nodes->u0f0[0][1] == 0);
        for (const auto* variable : variables.items()) if (variable != first) CHECK(variable->cur[0] == 0);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_main_lookup_adversarial_changes_retain_completed_leaves_and_real_detach)
{
    Database database("sizing_partial"); database.record().version = 11000;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    for (int mode = 0; mode < 5; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        auto& variables = *reinterpret_cast<Variables*>(nodes); const auto* const initialBody = variables.body();
        REQUIRE(variables.size() == 28u);
        std::array<def::model::StateVarBase*, 28> receivers{}; std::size_t ordinal = 0;
        for (auto* variable : variables.items()) receivers[ordinal++] = variable;
        std::unique_ptr<Variables> adversarialLease; engine::GuiModel foreign(nullptr, 3); double* foreignSlot = nullptr;
        struct RestoreBinding {
            def::model::LLOBJ_DATA& object; def::model::DB_STEP_DATA* owner;
            def::model::StateVarBase& variable; void* slot;
            ~RestoreBinding() { object.owner = owner; variable.u30 = slot; }
        } restoreBinding{*object, object->owner, *receivers[5], receivers[5]->u30};
        SizingObservation observation{nodes}; Trace trace{candidate.nodes};
        observation.adversarialMutation = [&](std::size_t index, bool before) {
            if (index != 5 || !before) return;
            if (mode == 0) step.u15f8[receivers[5]->id] = 1;
            else if (mode == 1) object->owner = foreign.step.get();
            else if (mode == 2) adversarialLease = std::make_unique<Variables>(variables);
            else if (mode == 3) receivers[5]->u30 = &foreignSlot;
            else step.u2b24 = 0;
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.before.size() == 6u); REQUIRE(observation.after.size() == 6u);
        for (std::size_t i = 0; i < receivers.size(); ++i) {
            CHECK(receivers[i]->cur[0] == (i < 5 ? 1 : 0));
            CHECK(receivers[i]->values.size() == 0u);
        }
        if (mode == 2) { CHECK(variables.body() != initialBody); CHECK(variables.use_count() == 1); }
        else CHECK(variables.body() == initialBody);
        CHECK(foreignSlot == nullptr); CHECK(nodes->n == 1); CHECK(unknownScene(candidate.model));
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        object->owner = &step; receivers[5]->u30 = restoreBinding.slot;
        step.u2b24 = 1; zeroOrdinaryWidths(step); observation.adversarialMutation = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 6u); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_main_zero_sizing_copies_actual_source_once_into_owned_destination)
{
    Database database("sizing_copy_guard"); database.record().version = 11000;
    RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
    auto* object = freshMain(candidate);
    auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
    zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
    const double expectedSource[3] = {1.25, -2.5, 7.75};
    SizingObservation observation{nodes}; Trace trace{candidate.nodes}; bool injectedOwnedSentinel = false;
    observation.adversarialMutation = [&](std::size_t ordinal, bool before) {
        if (ordinal != 27 || before) return;
        if (!nodes->u008 || !step.u1b78) return;
        std::memcpy(step.u1b78, expectedSource, sizeof expectedSource);
        injectedOwnedSentinel = true;
        nodes->u008[0] = 11.0; nodes->u008[1] = 12.0; nodes->u008[2] = 13.0;
    };
    { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
    REQUIRE(trace.count == 2u); REQUIRE(observation.after.size() == 28u); CHECK(injectedOwnedSentinel);
    CHECK(nodes->u008[0] == 1.25 && nodes->u008[1] == -2.5 && nodes->u008[2] == 7.75);
    CHECK(std::memcmp(nodes->u008, expectedSource, sizeof expectedSource) == 0);
    const auto* source = static_cast<const double*>(step.u1b78); REQUIRE(source);
    CHECK(source[0] == 1.25 && source[1] == -2.5 && source[2] == 7.75);
    CHECK(std::memcmp(source, expectedSource, sizeof expectedSource) == 0);
    CHECK(std::memcmp(nodes->u008, source, sizeof expectedSource) == 0);
    CHECK(def::model::g_timer_read_arrays.count == 1);
    CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3); checkScene(candidate.model);
    nodes->u008[0] = 21.0; nodes->u008[1] = 22.0; nodes->u008[2] = 23.0;
    { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
    CHECK(trace.count == 4u); CHECK(observation.after.size() == 28u);
    CHECK(nodes->u008[0] == 21.0 && nodes->u008[1] == 22.0 && nodes->u008[2] == 23.0);
    CHECK(def::model::g_timer_read_arrays.count == 1);
    CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3); checkScene(candidate.model);
}

TEST_CASE(geometry_main_equal_copy_retains_committed_bytes_before_later_object_guards)
{
    Database database("copy_later_guard"); database.record().version = 11000;
    for (const bool invalidLaterMar : {false, true}) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        auto* later = def::model::llobj_ctor::get_or_create(&step, 3); REQUIRE(later);
        REQUIRE(later->owner == &step); REQUIRE(def::model::llobj_not_kind7(later));
        *engine::at<std::uint8_t>(later, 0x3610) = 0;
        later->u87c[0] = invalidLaterMar ? 0 : 1;
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        const auto originalMar = def::model::g_mar_index[27];
        SizingObservation observation{nodes}; Trace trace{candidate.nodes};
        // Adversarial mutation occurs after this recipient's actual MAR load.
        observation.adversarialMutation = [&](std::size_t ordinal, bool before) {
            if (ordinal != 27 || before || !nodes->u008) return;
            nodes->u008[0] = 11.0; nodes->u008[1] = 12.0; nodes->u008[2] = 13.0;
            if (invalidLaterMar) def::model::g_mar_index[27] = 0x8c;
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 28u); REQUIRE(nodes->u008);
        CHECK(nodes->u008[0] == 0.0 && nodes->u008[1] == 0.0 && nodes->u008[2] == 0.0);
        auto* const primary = nodes->u008; auto* const source = step.u1b78;
        CHECK(engine::at<def::model::Cls_14017ef20>(later, 0x8a0)->u008 == nullptr);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2); CHECK(unknownScene(candidate.model));
        def::model::g_mar_index[27] = originalMar; later->u87c[0] = 0;
        observation.adversarialMutation = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 28u);
        CHECK(nodes->u008 == primary); CHECK(step.u1b78 == source); CHECK(unknownScene(candidate.model));
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
    }
}

TEST_CASE(geometry_general_fresh_recipients_copy_independent_rows_and_complete_once)
{
    Database database("general_recipients");
    static_assert(sizeof(std::array<double, 3>) == 24);
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    for (const std::int32_t version : {6000, 11000})
    for (const bool populated : {false, true})
    for (const std::size_t recipientCount : {std::size_t{0}, std::size_t{1}, std::size_t{2}, std::size_t{3}, std::size_t{4}, std::size_t{8}}) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        if (!populated) candidate.elements->u008 = nullptr;
        std::vector<def::model::Cls_14017ef20*> recipients;
        std::vector<std::array<double, 3>> rows(recipientCount);
        std::vector<const void*> bodies(recipientCount);
        std::vector<std::array<const def::model::StateVarBase*, 28>> receivers(recipientCount);
        SizingObservation observation{nullptr}; AppendObservation appends; Trace trace{candidate.nodes};
        for (std::size_t r = 0; r < recipientCount; ++r) {
            auto* object = freshMain(candidate, static_cast<std::int32_t>(r + 2));
            auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
            recipients.push_back(nodes); observation.additionalNodes.push_back(nodes);
            rows[r] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
            const auto& variables = *reinterpret_cast<const Variables*>(nodes);
            bodies[r] = variables.body(); std::size_t ordinal = 0;
            for (const auto* variable : variables.items()) receivers[r][ordinal++] = variable;
            REQUIRE(ordinal == 28u);
        }
        zeroOrdinaryWidths(step); const bool positiveBc = recipientCount != 2;
        for (std::size_t k = 0; k < 6; ++k) step.u15f8[0x12 + k] = positiveBc ? static_cast<std::int32_t>(k % 3) : 0;
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (ordinal != 27 || before || !step.u1b78) return;
            for (std::size_t r = 0; r < recipients.size(); ++r) if (actual == recipients[r]) {
                if (!recipients[r]->u008) return;
                // Declared adversarial rows distinguish copies, receipt relocation and replay from zero-fill.
                std::memcpy(step.u1b78, rows[r].data(), sizeof rows[r]);
                recipients[r]->u008[0] = 31.0; recipients[r]->u008[1] = 32.0; recipients[r]->u008[2] = 33.0;
            }
        };
        def::model::g_timer_read_arrays.count = 0;
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
        const std::size_t rounds = populated ? 2u : 1u;
        CHECK(trace.count == rounds); REQUIRE(observation.after.size() == recipientCount * 28u);
        REQUIRE(observation.before.size() == recipientCount * 28u); REQUIRE(observation.afterNodes.size() == recipientCount * 28u);
        REQUIRE(observation.receivers.size() == recipientCount * 28u); REQUIRE(appends.counts.size() == recipientCount);
        REQUIRE(step.u1b78); std::size_t growths = 0;
        for (std::size_t r = 0; r < recipientCount; ++r) {
            auto* nodes = recipients[r]; const auto& variables = *reinterpret_cast<const Variables*>(nodes);
            CHECK(appends.counts[r] == r); CHECK(appends.ordinals[r] == static_cast<std::int32_t>(r + 1));
            CHECK(appends.capacities[r] >= r); if (appends.capacities[r] == r) ++growths;
            REQUIRE(nodes->u008); CHECK(nodes->u008 != step.u1b78); CHECK(nodes->n == 1); CHECK(nodes->u13c == 0);
            CHECK(nodes->u0f0[6] == nullptr); CHECK(variables.body() == bodies[r]);
            CHECK(std::memcmp(nodes->u008, rows[r].data(), sizeof rows[r]) == 0);
            for (std::size_t prior = 0; prior < r; ++prior) {
                CHECK(nodes->u008 != recipients[prior]->u008); CHECK(bodies[r] != bodies[prior]);
            }
            for (std::size_t i = 0; i < 28; ++i) {
                const auto index = r * 28 + i;
                CHECK(observation.before[index] == static_cast<std::int64_t>(i));
                CHECK(observation.after[index] == static_cast<std::int64_t>(i)); CHECK(observation.afterNodes[index] == nodes);
                CHECK(observation.receivers[index] == receivers[r][i]);
                const bool normal = def::model::Cls_14017ef20::kRegs[i].ctor == 0x14032a990;
                CHECK(receivers[r][i]->cur[0] == (normal ? 1 : 0)); CHECK(receivers[r][i]->values.size() == 0u); CHECK(nodes->u010[i] == nullptr);
            }
            for (std::size_t k = 0; k < 6; ++k) {
                const auto count = step.u15f8[0x12 + k];
                if (count == 0) CHECK(nodes->u0f0[k] == nullptr);
                else {
                    REQUIRE(nodes->u0f0[k]);
                    for (std::size_t prior = 0; prior < r; ++prior) CHECK(nodes->u0f0[k] != recipients[prior]->u0f0[k]);
                    for (std::int32_t i = 0; i < count; ++i) CHECK(nodes->u0f0[k][i] == 0);
                }
            }
        }
        if (recipientCount == 8) CHECK(growths >= 2u); // Observe actual metadata capacity changes; no native ownership moves.
        if (recipientCount != 0) CHECK(std::memcmp(step.u1b78, rows.back().data(), sizeof rows.back()) == 0);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
        checkScene(candidate.model, populated ? 1.0f : 0.0f);
        std::vector<const double*> primary; for (const auto* nodes : recipients) primary.push_back(nodes->u008);
        auto* const source = step.u1b78;
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
        CHECK(trace.count == 2u * rounds); CHECK(observation.after.size() == recipientCount * 28u); CHECK(appends.counts.size() == recipientCount);
        CHECK(step.u1b78 == source);
        for (std::size_t r = 0; r < recipientCount; ++r) {
            CHECK(recipients[r]->u008 == primary[r]); CHECK(std::memcmp(primary[r], rows[r].data(), sizeof rows[r]) == 0);
        }
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
    }
}

TEST_CASE(geometry_second_recipient_partial_lookup_preserves_first_and_rechecks_live_table)
{
    Database database("two_partial"); database.record().version = 11000;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const double row[3] = {1.25, -2.5, 7.75};
    for (int mode = 0; mode < 3; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* first = freshMain(candidate); auto* second = freshMain(candidate, 3);
        auto* spare = def::model::llobj_ctor::get_or_create(&step, 4); REQUIRE(spare);
        auto* nodes0 = engine::at<def::model::Cls_14017ef20>(first, 0x8a0);
        auto* nodes1 = engine::at<def::model::Cls_14017ef20>(second, 0x8a0);
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        engine::GuiModel foreign(nullptr, 3);
        struct RestoreOwnersAndLinks {
            def::model::DB_STEP_DATA& step; def::model::LLOBJ_DATA* head;
            def::model::LLOBJ_DATA* first; def::model::LLOBJ_DATA* second; def::model::LLOBJ_DATA* spare;
            ~RestoreOwnersAndLinks() { restore(); }
            void restore() {
                head->next = first; first->next = second; second->next = spare; spare->next = nullptr;
                second->owner = &step;
            }
        } restoreBindings{step, step.objects.head, first, second, spare};
        SizingObservation observation{nodes0}; observation.secondNodes = nodes1; Trace trace{candidate.nodes};
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (actual == nodes0 && ordinal == 27 && !before && step.u1b78) std::memcpy(step.u1b78, row, sizeof row);
            if (actual != nodes1 || ordinal != 5 || !before) return;
            if (nodes1->u008) { nodes1->u008[0] = 31.0; nodes1->u008[1] = 32.0; nodes1->u008[2] = 33.0; }
            if (mode == 0) step.u15f8[1] = 1;
            else if (mode == 1) second->owner = foreign.step.get();
            else {
                // Keep the second at ordinal3, but replace the first's actual ordinal2 identity.
                restoreBindings.head->next = spare; spare->next = second; second->next = first; first->next = nullptr;
            }
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        restoreBindings.restore(); zeroOrdinaryWidths(step);
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 34u); REQUIRE(nodes0->u008); REQUIRE(nodes1->u008);
        CHECK(std::memcmp(nodes0->u008, row, sizeof row) == 0);
        CHECK(nodes1->u008[0] == 31.0 && nodes1->u008[1] == 32.0 && nodes1->u008[2] == 33.0);
        CHECK(nodes0->u008 != nodes1->u008); CHECK(nodes0->n == 1); CHECK(nodes1->n == 1);
        std::size_t ordinal = 0;
        for (const auto* variable : reinterpret_cast<const Variables*>(nodes1)->items()) {
            CHECK(variable->cur[0] == (ordinal < 5 ? 1 : 0)); ++ordinal;
        }
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
        observation.adversarialByNodes = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 34u);
        CHECK(std::memcmp(nodes0->u008, row, sizeof row) == 0);
        CHECK(nodes1->u008[0] == 31.0 && nodes1->u008[1] == 32.0 && nodes1->u008[2] == 33.0);
        CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_two_recipient_copies_remain_before_unqualified_third_or_later_MAR_guard)
{
    Database database("two_later_guard"); database.record().version = 11000;
    const double rows[2][3] = {{1.25, -2.5, 7.75}, {4.5, 5.25, -6.75}};
    for (const bool invalidLaterMar : {false, true}) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* first = freshMain(candidate); auto* second = freshMain(candidate, 3); auto* third = freshMain(candidate, 4);
        if (invalidLaterMar) third->u87c[0] = 0;
        else third->u87c[1] = 0; // Explicit unqualified n0 guard; a fresh n1 third is now supported.
        auto* nodes0 = engine::at<def::model::Cls_14017ef20>(first, 0x8a0);
        auto* nodes1 = engine::at<def::model::Cls_14017ef20>(second, 0x8a0);
        auto* nodes2 = engine::at<def::model::Cls_14017ef20>(third, 0x8a0);
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        const auto originalMar = def::model::g_mar_index[27];
        SizingObservation observation{nodes0}; observation.secondNodes = nodes1; Trace trace{candidate.nodes};
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (ordinal != 27 || before || !step.u1b78) return;
            const std::size_t r = actual == nodes0 ? 0 : 1;
            std::memcpy(step.u1b78, rows[r], sizeof rows[r]);
            if (actual == nodes1 && invalidLaterMar) def::model::g_mar_index[27] = 0x8c;
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 56u); REQUIRE(nodes0->u008); REQUIRE(nodes1->u008);
        CHECK(std::memcmp(nodes0->u008, rows[0], sizeof rows[0]) == 0);
        CHECK(std::memcmp(nodes1->u008, rows[1], sizeof rows[1]) == 0);
        CHECK(nodes2->u008 == nullptr); CHECK(nodes2->n == 0); CHECK(nodes2->u13c == 9);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
        def::model::g_mar_index[27] = originalMar; third->u87c[0] = 0; observation.adversarialByNodes = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 56u); CHECK(nodes2->u008 == nullptr);
        CHECK(std::memcmp(nodes0->u008, rows[0], sizeof rows[0]) == 0);
        CHECK(std::memcmp(nodes1->u008, rows[1], sizeof rows[1]) == 0); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_second_recipient_fresh_admission_guards_preserve_first_copy)
{
    Database database("two_admission"); database.record().version = 11000;
    const double row[3] = {1.25, -2.5, 7.75};
    for (int mode = 0; mode < 3; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* first = freshMain(candidate); auto* second = freshMain(candidate, 3);
        auto* nodes0 = engine::at<def::model::Cls_14017ef20>(first, 0x8a0);
        auto* nodes1 = engine::at<def::model::Cls_14017ef20>(second, 0x8a0);
        engine::GuiModel foreign(nullptr, 3);
        struct RestoreOwner {
            def::model::LLOBJ_DATA& object; def::model::DB_STEP_DATA* owner;
            ~RestoreOwner() { object.owner = owner; }
        } restoreOwner{*second, second->owner};
        if (mode == 0) { nodes1->u008 = def::model::bc::calloc_doubles(3); REQUIRE(nodes1->u008); nodes1->u008[0] = 42.0; }
        else if (mode == 1) *engine::at<std::int32_t>(second, 0x3984) = 2;
        else second->owner = foreign.step.get();
        auto* const untouchedPrimary = nodes1->u008;
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        SizingObservation observation{nodes0}; observation.secondNodes = nodes1; Trace trace{candidate.nodes};
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (actual == nodes0 && ordinal == 27 && !before && step.u1b78) std::memcpy(step.u1b78, row, sizeof row);
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 28u); REQUIRE(nodes0->u008);
        CHECK(std::memcmp(nodes0->u008, row, sizeof row) == 0);
        CHECK(nodes1->u008 == untouchedPrimary); CHECK(nodes1->n == 0); CHECK(nodes1->u13c == 9);
        if (mode == 0) CHECK(nodes1->u008[0] == 42.0);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_third_recipient_partial_lookup_rechecks_both_prior_live_ordinals)
{
    Database database("three_partial"); database.record().version = 11000;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const double rows[2][3] = {{1.25, -2.5, 7.75}, {4.5, 5.25, -6.75}};
    for (int mode = 0; mode < 4; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* first = freshMain(candidate); auto* second = freshMain(candidate, 3); auto* third = freshMain(candidate, 4);
        auto* spare = def::model::llobj_ctor::get_or_create(&step, 5); REQUIRE(spare);
        auto* nodes0 = engine::at<def::model::Cls_14017ef20>(first, 0x8a0);
        auto* nodes1 = engine::at<def::model::Cls_14017ef20>(second, 0x8a0);
        auto* nodes2 = engine::at<def::model::Cls_14017ef20>(third, 0x8a0);
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        engine::GuiModel foreign(nullptr, 3);
        struct RestoreOwnersAndLinks {
            def::model::DB_STEP_DATA& step; def::model::LLOBJ_DATA* head;
            def::model::LLOBJ_DATA* first; def::model::LLOBJ_DATA* second;
            def::model::LLOBJ_DATA* third; def::model::LLOBJ_DATA* spare;
            ~RestoreOwnersAndLinks() { restore(); }
            void restore() {
                head->next = first; first->next = second; second->next = third; third->next = spare; spare->next = nullptr;
                third->owner = &step;
            }
        } restoreBindings{step, step.objects.head, first, second, third, spare};
        SizingObservation observation{nodes0}; observation.secondNodes = nodes1; observation.thirdNodes = nodes2;
        Trace trace{candidate.nodes};
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (actual != nodes2 && ordinal == 27 && !before && step.u1b78) {
                const std::size_t r = actual == nodes0 ? 0 : 1;
                std::memcpy(step.u1b78, rows[r], sizeof rows[r]);
            }
            if (actual != nodes2 || ordinal != 5 || !before) return;
            if (nodes2->u008) { nodes2->u008[0] = 31.0; nodes2->u008[1] = 32.0; nodes2->u008[2] = 33.0; }
            if (mode == 0) step.u15f8[1] = 1;
            else if (mode == 1) third->owner = foreign.step.get();
            else if (mode == 2) {
                // Swap first/spare in the live table; second ordinal3 and third ordinal4 remain valid.
                restoreBindings.head->next = spare; spare->next = second; third->next = first; first->next = nullptr;
            } else {
                // Swap second/spare; first ordinal2 and third ordinal4 remain valid.
                first->next = spare; spare->next = third; third->next = second; second->next = nullptr;
            }
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        restoreBindings.restore(); zeroOrdinaryWidths(step);
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 62u);
        REQUIRE(nodes0->u008); REQUIRE(nodes1->u008); REQUIRE(nodes2->u008);
        CHECK(std::memcmp(nodes0->u008, rows[0], sizeof rows[0]) == 0);
        CHECK(std::memcmp(nodes1->u008, rows[1], sizeof rows[1]) == 0);
        CHECK(nodes2->u008[0] == 31.0 && nodes2->u008[1] == 32.0 && nodes2->u008[2] == 33.0);
        CHECK(nodes0->u008 != nodes2->u008); CHECK(nodes1->u008 != nodes2->u008); CHECK(nodes2->n == 1);
        std::size_t ordinal = 0;
        for (const auto* variable : reinterpret_cast<const Variables*>(nodes2)->items()) {
            CHECK(variable->cur[0] == (ordinal < 5 ? 1 : 0)); ++ordinal;
        }
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
        observation.adversarialByNodes = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 62u);
        CHECK(std::memcmp(nodes0->u008, rows[0], sizeof rows[0]) == 0);
        CHECK(std::memcmp(nodes1->u008, rows[1], sizeof rows[1]) == 0);
        CHECK(nodes2->u008[0] == 31.0 && nodes2->u008[1] == 32.0 && nodes2->u008[2] == 33.0);
        CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_three_recipient_copies_remain_before_unqualified_fourth_or_later_MAR_guard)
{
    Database database("three_later_guard"); database.record().version = 11000;
    const double rows[3][3] = {{1.25, -2.5, 7.75}, {4.5, 5.25, -6.75}, {-7.5, 8.25, 9.75}};
    for (const bool invalidLaterMar : {false, true}) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* first = freshMain(candidate); auto* second = freshMain(candidate, 3); auto* third = freshMain(candidate, 4);
        auto* fourth = freshMain(candidate, 5);
        if (invalidLaterMar) fourth->u87c[0] = 0;
        else fourth->u87c[1] = 0; // Explicit unqualified n0; a fresh n1 fourth is supported.
        auto* nodes0 = engine::at<def::model::Cls_14017ef20>(first, 0x8a0);
        auto* nodes1 = engine::at<def::model::Cls_14017ef20>(second, 0x8a0);
        auto* nodes2 = engine::at<def::model::Cls_14017ef20>(third, 0x8a0);
        auto* nodes3 = engine::at<def::model::Cls_14017ef20>(fourth, 0x8a0);
        const def::model::Cls_14017ef20* recipients[3] = {nodes0, nodes1, nodes2};
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        const auto originalMar = def::model::g_mar_index[27];
        SizingObservation observation{nodes0}; observation.secondNodes = nodes1; observation.thirdNodes = nodes2;
        Trace trace{candidate.nodes};
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (ordinal != 27 || before || !step.u1b78) return;
            const std::size_t r = actual == nodes0 ? 0 : actual == nodes1 ? 1 : 2;
            std::memcpy(step.u1b78, rows[r], sizeof rows[r]);
            if (actual == nodes2 && invalidLaterMar) def::model::g_mar_index[27] = 0x8c;
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 84u);
        for (std::size_t r = 0; r < 3; ++r) {
            REQUIRE(recipients[r]->u008); CHECK(std::memcmp(recipients[r]->u008, rows[r], sizeof rows[r]) == 0);
        }
        CHECK(nodes3->u008 == nullptr); CHECK(nodes3->n == 0); CHECK(nodes3->u13c == 9);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
        def::model::g_mar_index[27] = originalMar; fourth->u87c[0] = 0; observation.adversarialByNodes = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 84u); CHECK(nodes3->u008 == nullptr);
        for (std::size_t r = 0; r < 3; ++r) CHECK(std::memcmp(recipients[r]->u008, rows[r], sizeof rows[r]) == 0);
        CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_third_recipient_fresh_admission_guards_preserve_two_prior_copies)
{
    Database database("three_admission"); database.record().version = 11000;
    const double rows[2][3] = {{1.25, -2.5, 7.75}, {4.5, 5.25, -6.75}};
    for (int mode = 0; mode < 3; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* first = freshMain(candidate); auto* second = freshMain(candidate, 3); auto* third = freshMain(candidate, 4);
        auto* nodes0 = engine::at<def::model::Cls_14017ef20>(first, 0x8a0);
        auto* nodes1 = engine::at<def::model::Cls_14017ef20>(second, 0x8a0);
        auto* nodes2 = engine::at<def::model::Cls_14017ef20>(third, 0x8a0);
        engine::GuiModel foreign(nullptr, 3);
        struct RestoreOwner {
            def::model::LLOBJ_DATA& object; def::model::DB_STEP_DATA* owner;
            ~RestoreOwner() { object.owner = owner; }
        } restoreOwner{*third, third->owner};
        if (mode == 0) { nodes2->u008 = def::model::bc::calloc_doubles(3); REQUIRE(nodes2->u008); nodes2->u008[0] = 42.0; }
        else if (mode == 1) *engine::at<std::int32_t>(third, 0x3984) = 2;
        else third->owner = foreign.step.get();
        auto* const untouchedPrimary = nodes2->u008;
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        SizingObservation observation{nodes0}; observation.secondNodes = nodes1; observation.thirdNodes = nodes2;
        Trace trace{candidate.nodes};
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (ordinal != 27 || before || !step.u1b78) return;
            const std::size_t r = actual == nodes0 ? 0 : 1;
            std::memcpy(step.u1b78, rows[r], sizeof rows[r]);
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 56u); REQUIRE(nodes0->u008); REQUIRE(nodes1->u008);
        CHECK(std::memcmp(nodes0->u008, rows[0], sizeof rows[0]) == 0);
        CHECK(std::memcmp(nodes1->u008, rows[1], sizeof rows[1]) == 0);
        CHECK(nodes2->u008 == untouchedPrimary); CHECK(nodes2->n == 0); CHECK(nodes2->u13c == 9);
        if (mode == 0) CHECK(nodes2->u008[0] == 42.0);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_general_loop_observes_genuine_owned_list_growth_in_current_bound)
{
    Database database("general_current_bound"); database.record().version = 11000;
    RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
    std::vector<def::model::Cls_14017ef20*> nodes; nodes.reserve(8);
    SizingObservation observation{nullptr}; observation.additionalNodes.reserve(8);
    for (std::int32_t id : {2, 3}) {
        auto* object = freshMain(candidate, id); auto* receiver = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        nodes.push_back(receiver); observation.additionalNodes.push_back(receiver);
    }
    REQUIRE(step.u1378 == 3); zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
    bool grewActualList = false; AppendObservation appends; Trace trace{candidate.nodes};
    observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
        if (ordinal != 27 || before || !step.u1b78) return;
        if (actual == nodes[0] && !grewActualList) {
            // Actual get-or-create appends real owned objects and updates u1378; no count fabrication.
            for (std::int32_t id = 4; id <= 9; ++id) {
                auto* object = freshMain(candidate, id); auto* receiver = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
                nodes.push_back(receiver); observation.additionalNodes.push_back(receiver);
            }
            grewActualList = true;
        }
        for (std::size_t r = 0; r < nodes.size(); ++r) if (actual == nodes[r]) {
            const double row[3] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
            std::memcpy(step.u1b78, row, sizeof row);
        }
    };
    { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
    CHECK(grewActualList); CHECK(step.u1378 == 9); REQUIRE(nodes.size() == 8u);
    REQUIRE(trace.count == 2u); REQUIRE(observation.after.size() == 224u); REQUIRE(appends.counts.size() == 8u);
    for (std::size_t r = 0; r < nodes.size(); ++r) {
        const double row[3] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
        REQUIRE(nodes[r]->u008); CHECK(nodes[r]->n == 1); CHECK(std::memcmp(nodes[r]->u008, row, sizeof row) == 0);
        CHECK(appends.counts[r] == r); CHECK(appends.ordinals[r] == static_cast<std::int32_t>(r + 1));
    }
    CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
    checkScene(candidate.model);
    { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
    CHECK(trace.count == 4u); CHECK(observation.after.size() == 224u); CHECK(appends.counts.size() == 8u);
}

TEST_CASE(geometry_general_metadata_resource_guard_retains_prior_copies_before_new_reset)
{
    Database database("general_metadata_guard"); database.record().version = 11000;
    for (const bool lengthFailure : {false, true}) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        std::vector<def::model::Cls_14017ef20*> nodes; SizingObservation observation{nullptr};
        for (std::int32_t id = 2; id <= 9; ++id) {
            auto* object = freshMain(candidate, id); auto* receiver = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
            nodes.push_back(receiver); observation.additionalNodes.push_back(receiver);
        }
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (ordinal != 27 || before || !step.u1b78) return;
            for (std::size_t r = 0; r < nodes.size(); ++r) if (actual == nodes[r]) {
                const double row[3] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
                std::memcpy(step.u1b78, row, sizeof row);
            }
        };
        AppendObservation appends; bool injectedMetadataFailure = false; std::size_t refusedCount = nodes.size(); Trace trace{candidate.nodes};
        appends.adversarialMutation = [&](std::size_t completed, std::int32_t) {
            if (completed < 4 || appends.capacities.back() != completed) return;
            refusedCount = completed;
            injectedMetadataFailure = true; // Reconstruction bookkeeping fault, not an original/native OOM witness.
            if (lengthFailure) throw std::length_error("injected receipt length guard");
            throw std::bad_alloc();
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
        CHECK(injectedMetadataFailure); REQUIRE(refusedCount >= 4u && refusedCount < nodes.size()); REQUIRE(trace.count == 1u);
        REQUIRE(appends.counts.size() == refusedCount + 1); CHECK(appends.counts.back() == refusedCount);
        CHECK(appends.capacities.back() == refusedCount); CHECK(appends.ordinals.back() == static_cast<std::int32_t>(refusedCount + 1));
        REQUIRE(observation.after.size() == refusedCount * 28u);
        for (std::size_t r = 0; r < nodes.size(); ++r) {
            if (r < refusedCount) {
                const double row[3] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
                REQUIRE(nodes[r]->u008); CHECK(std::memcmp(nodes[r]->u008, row, sizeof row) == 0); CHECK(nodes[r]->n == 1);
            } else {
                CHECK(nodes[r]->u008 == nullptr); CHECK(nodes[r]->n == 0); CHECK(nodes[r]->u13c == 9);
                for (const auto* pointer : nodes[r]->u0f0) CHECK(pointer == nullptr);
            }
        }
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model)); appends.adversarialMutation = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(appends.counts.size() == refusedCount + 1); CHECK(observation.after.size() == refusedCount * 28u);
        CHECK(nodes[refusedCount]->u008 == nullptr); CHECK(nodes[refusedCount]->u13c == 9); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_general_late_lookup_preserves_all_prior_rows_and_live_identity_guards)
{
    Database database("general_late_partial"); database.record().version = 11000;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    for (int mode = 0; mode < 4; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        std::vector<def::model::LLOBJ_DATA*> objects;
        std::vector<def::model::Cls_14017ef20*> nodes; SizingObservation observation{nullptr};
        for (std::int32_t id = 2; id <= 9; ++id) {
            auto* object = freshMain(candidate, id); objects.push_back(object);
            auto* receiver = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
            nodes.push_back(receiver); observation.additionalNodes.push_back(receiver);
        }
        auto* spare = def::model::llobj_ctor::get_or_create(&step, 10); REQUIRE(spare);
        std::vector<def::model::LLOBJ_DATA*> chain{step.objects.head};
        for (auto* object : objects) chain.push_back(object); chain.push_back(spare);
        engine::GuiModel foreign(nullptr, 3);
        struct RestoreOwnersAndLinks {
            def::model::DB_STEP_DATA& step; const std::vector<def::model::LLOBJ_DATA*>& chain;
            def::model::LLOBJ_DATA* current;
            ~RestoreOwnersAndLinks() { restore(); }
            void restore() {
                step.objects.head = chain.front(); step.objects.tail = chain.back();
                for (std::size_t i = 0; i < chain.size(); ++i) chain[i]->next = i + 1 < chain.size() ? chain[i + 1] : nullptr;
                current->owner = &step;
            }
        } restoreBindings{step, chain, objects[5]};
        zeroOrdinaryWidths(step); def::model::g_timer_read_arrays.count = 0;
        observation.adversarialByNodes = [&](const void* actual, std::size_t ordinal, bool before) {
            if (ordinal == 27 && !before && step.u1b78) {
                for (std::size_t r = 0; r < nodes.size(); ++r) if (actual == nodes[r]) {
                    const double row[3] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
                    std::memcpy(step.u1b78, row, sizeof row);
                }
            }
            if (actual != nodes[5] || ordinal != 5 || !before) return;
            if (nodes[5]->u008) { nodes[5]->u008[0] = 31.0; nodes[5]->u008[1] = 32.0; nodes[5]->u008[2] = 33.0; }
            if (mode == 0) step.u15f8[1] = 1;
            else if (mode == 1) objects[5]->owner = foreign.step.get();
            else {
                auto changedOrder = chain; std::swap(changedOrder[mode == 2 ? 1 : 5], changedOrder.back());
                for (std::size_t i = 0; i < changedOrder.size(); ++i)
                    changedOrder[i]->next = i + 1 < changedOrder.size() ? changedOrder[i + 1] : nullptr;
            }
        };
        AppendObservation appends; Trace trace{candidate.nodes};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
        restoreBindings.restore(); zeroOrdinaryWidths(step);
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 146u); REQUIRE(appends.counts.size() == 6u);
        for (std::size_t r = 0; r < 5; ++r) {
            const double row[3] = {static_cast<double>(r) + 1.25, -static_cast<double>(r) - 2.5, static_cast<double>(r) + 7.75};
            REQUIRE(nodes[r]->u008); CHECK(std::memcmp(nodes[r]->u008, row, sizeof row) == 0);
        }
        REQUIRE(nodes[5]->u008); CHECK(nodes[5]->n == 1);
        CHECK(nodes[5]->u008[0] == 31.0 && nodes[5]->u008[1] == 32.0 && nodes[5]->u008[2] == 33.0);
        std::size_t ordinal = 0;
        for (const auto* variable : reinterpret_cast<const Variables*>(nodes[5])->items()) {
            CHECK(variable->cur[0] == (ordinal < 5 ? 1 : 0)); ++ordinal;
        }
        CHECK(nodes[6]->u008 == nullptr); CHECK(nodes[6]->u13c == 9); CHECK(nodes[7]->u008 == nullptr);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model)); observation.adversarialByNodes = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); ScopedAppendObservation admission(appends); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 146u); CHECK(appends.counts.size() == 6u);
        CHECK(nodes[5]->u008[0] == 31.0); CHECK(nodes[6]->u008 == nullptr); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_first_nonempty_leaf_commits_ordered_dimensions_then_parks_before_resize)
{
    Database database("scalar_dimensions");
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    for (const std::int32_t version : {6000, 11000}) {
        RestoreGlobals restore; database.record().version = version;
        Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate); auto* later = freshMain(candidate, 3);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        auto* laterNodes = engine::at<def::model::Cls_14017ef20>(later, 0x8a0);
        const auto& variables = *reinterpret_cast<const Variables*>(nodes);
        auto* first = variables.items().front(); first->cur[1] = 7; first->prev[1] = -9; first->prev[2] = 8;
        zeroOrdinaryWidths(step); step.u15f8[1] = 1; step.u15f8[0x12] = 2;
        const auto* valueBody = first->values.body(); const auto* registryBody = variables.body();
        std::int32_t beforeCur[28][3] = {}, beforePrev[28][3] = {}; bool sawBeforeSetters = false;
        SizingObservation observation{nodes}; Trace trace{candidate.nodes};
        observation.adversarialMutation = [&](std::size_t ordinal, bool before) {
            if (ordinal != 0 || before) return;
            sawBeforeSetters = true; std::size_t i = 0;
            for (const auto* variable : variables.items()) {
                for (std::size_t d = 0; d < 3; ++d) { beforeCur[i][d] = variable->cur[d]; beforePrev[i][d] = variable->prev[d]; }
                ++i;
            }
            if (nodes->u008) { nodes->u008[0] = 11.0; nodes->u008[1] = 12.0; nodes->u008[2] = 13.0; }
        };
        def::model::g_timer_read_arrays.count = 0;
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(sawBeforeSetters); REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 1u);
        CHECK(observation.before[0] == 0); CHECK(observation.after[0] == 0); CHECK(observation.receivers[0] == first);
        CHECK(beforeCur[0][0] == 0 && beforeCur[0][1] == 7 && beforeCur[0][2] == 1);
        std::size_t i = 0;
        for (const auto* variable : variables.items()) {
            for (std::size_t d = 0; d < 3; ++d) {
                CHECK(variable->cur[d] == (i == 0 && d < 2 ? 1 : beforeCur[i][d]));
                CHECK(variable->prev[d] == (i == 0 && d < 2 ? beforeCur[i][d] : beforePrev[i][d]));
            }
            CHECK(variable->values.size() == 0u); CHECK(nodes->u010[i] == nullptr); ++i;
        }
        CHECK(variables.body() == registryBody); CHECK(first->values.body() == valueBody);
        CHECK(valueBody->first == nullptr && valueBody->last == nullptr && valueBody->end == nullptr);
        REQUIRE(nodes->u008); CHECK(nodes->u008[0] == 11.0 && nodes->u008[1] == 12.0 && nodes->u008[2] == 13.0);
        CHECK(nodes->n == 1); REQUIRE(nodes->u0f0[0]); CHECK(nodes->u0f0[0][0] == 0 && nodes->u0f0[0][1] == 0);
        CHECK(laterNodes->u008 == nullptr); CHECK(laterNodes->n == 0); CHECK(laterNodes->u13c == 9);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model)); step.u15f8[1] = 0; observation.adversarialMutation = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 1u);
        CHECK(first->cur[0] == 1 && first->cur[1] == 1 && first->prev[1] == 7);
        CHECK(first->values.body() == valueBody); CHECK(valueBody->first == nullptr); CHECK(nodes->u010[0] == nullptr);
        CHECK(nodes->u008[0] == 11.0 && nodes->u008[1] == 12.0 && nodes->u008[2] == 13.0);
        CHECK(laterNodes->u008 == nullptr); CHECK(unknownScene(candidate.model));
    }
}

TEST_CASE(geometry_first_nonempty_lookup_adversarial_changes_refuse_before_setters)
{
    Database database("scalar_lookup_guard"); database.record().version = 11000;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    using Values = def::model::CowVector<double>;
    for (int mode = 0; mode < 5; ++mode) {
        RestoreGlobals restore; Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate); auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        const auto& variables = *reinterpret_cast<const Variables*>(nodes);
        auto* first = variables.items().front(); first->cur[1] = 7; first->prev[1] = -9;
        zeroOrdinaryWidths(step); step.u15f8[1] = 1; def::model::g_timer_read_arrays.count = 0;
        std::unique_ptr<Values> adversarialValueLease; engine::GuiModel foreign(nullptr, 3); double* foreignSlot = nullptr;
        struct RestoreBinding {
            def::model::LLOBJ_DATA& object; def::model::DB_STEP_DATA* owner;
            def::model::StateVarBase& variable; void* slot;
            ~RestoreBinding() { restore(); }
            void restore() { object.owner = owner; variable.u30 = slot; }
        } restoreBinding{*object, object->owner, *first, first->u30};
        SizingObservation observation{nodes}; Trace trace{candidate.nodes};
        observation.adversarialMutation = [&](std::size_t ordinal, bool before) {
            if (ordinal != 0 || before) return;
            if (mode == 0) step.u15f8[1] = 2;
            else if (mode == 1) object->owner = foreign.step.get();
            else if (mode == 2) first->u30 = &foreignSlot;
            else if (mode == 3) first->cur[2] = 2;
            else adversarialValueLease = std::make_unique<Values>(first->values);
        };
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        restoreBinding.restore(); adversarialValueLease.reset();
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 1u);
        CHECK(first->cur[0] == 0); CHECK(first->cur[1] == 7 && first->prev[1] == -9);
        CHECK(first->values.size() == 0u); CHECK(first->values.body()->first == nullptr);
        CHECK(nodes->u010[0] == nullptr); CHECK(foreignSlot == nullptr); CHECK(nodes->n == 1);
        CHECK(def::model::g_timer_read_arrays.count == 1); CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(unknownScene(candidate.model)); step.u15f8[1] = 1; first->cur[2] = 1; observation.adversarialMutation = {};
        { ScopedTrace scope(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u); CHECK(observation.after.size() == 1u);
        CHECK(first->cur[0] == 0 && first->cur[1] == 7 && first->prev[1] == -9); CHECK(unknownScene(candidate.model));
    }
}

namespace {
void runGeometryTempChild(const std::string& scenario)
{
    using namespace engine::detail;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    using Values = def::model::CowVector<double>;
    const bool valueMode = scenario.substr(0, 5) == "value";
    const auto selected = valueMode ? scenario.substr(5) : scenario;
    const auto mode = valueMode ? def::rt::HeapPrefixMode::UnpublishedScalar : def::rt::HeapPrefixMode::RawTemporaryOnly;
    const bool parked = selected == "parked";
    const bool success = selected == "success6000" || selected == "success11000";
    int mutation = -1;
    if (selected.size() == 6 && selected.substr(0, 5) == "guard" && selected[5] >= '0' && selected[5] <= '4')
        mutation = selected[5] - '0';
    REQUIRE(parked || success || mutation >= 0);
    Database database(("temp_" + scenario).c_str());
    database.record().version = selected == "success6000" ? 6000 : 11000;
    RestoreGlobals restore;
    std::shared_ptr<GeometryTempPrefixBinding> retained;
    std::uintptr_t returned = 0, buffer = 0;
    {
        Candidate candidate(database.handle()); auto& step = *candidate.model.step;
        auto* object = freshMain(candidate); auto* later = freshMain(candidate, 3);
        auto* nodes = engine::at<def::model::Cls_14017ef20>(object, 0x8a0);
        auto* laterNodes = engine::at<def::model::Cls_14017ef20>(later, 0x8a0);
        const auto& variables = *reinterpret_cast<const Variables*>(nodes);
        auto* first = variables.items().front(); first->cur[1] = 7; first->prev[1] = -9; first->prev[2] = 8;
        zeroOrdinaryWidths(step); step.u15f8[1] = 1; step.u15f8[0x12] = 2;
        const auto* const body = first->values.body(); const auto* const registry = variables.body();
        engine::GuiModel foreign(nullptr, 3); double* foreignSlot = nullptr;
        struct RestoreBinding {
            def::model::LLOBJ_DATA& object; def::model::DB_STEP_DATA* owner;
            def::model::StateVarBase& variable; void* slot;
            ~RestoreBinding() { object.owner = owner; variable.u30 = slot; }
        } restoreBinding{*object, object->owner, *first, first->u30};
        std::unique_ptr<Values> sharedBody;
        if (!parked) {
            auto prepared = prepareGeometryTempPrefix(candidate.model, mode);
            REQUIRE(prepared.status == GeometryTempPrepareStatus::Prepared); REQUIRE(prepared.binding);
            retained = prepared.binding;
            CHECK(prepareGeometryTempPrefix(candidate.model, mode).status == GeometryTempPrepareStatus::AlreadyPrepared);
            const auto otherMode = valueMode ? def::rt::HeapPrefixMode::RawTemporaryOnly : def::rt::HeapPrefixMode::UnpublishedScalar;
            CHECK(prepareGeometryTempPrefix(candidate.model, otherMode).status == GeometryTempPrepareStatus::ModeMismatch);
            CHECK(retained->report().mode == mode);
            CHECK(retained->report().temporary.heapAllocCalls == 0u);
        }
        Trace trace{candidate.nodes}; SizingObservation observation{nodes};
        std::int32_t beforeCur[28][3]{}, beforePrev[28][3]{};
        observation.adversarialMutation = [&](std::size_t ordinal, bool before) {
            if (ordinal != 0 || before) return;
            std::size_t i = 0;
            for (const auto* variable : variables.items()) {
                for (std::size_t d = 0; d < 3; ++d) { beforeCur[i][d] = variable->cur[d]; beforePrev[i][d] = variable->prev[d]; }
                ++i;
            }
            if (nodes->u008) { nodes->u008[0] = 11.0; nodes->u008[1] = 12.0; nodes->u008[2] = 13.0; }
            if (mutation == 0) step.u15f8[1] = 2;
            else if (mutation == 1) object->owner = foreign.step.get();
            else if (mutation == 2) first->u30 = &foreignSlot;
            else if (mutation == 3) first->cur[2] = 2;
            else if (mutation == 4) sharedBody = std::make_unique<Values>(first->values);
        };
        def::model::g_timer_read_arrays.count = 0;
        { ScopedTrace tracing(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        REQUIRE(trace.count == 1u); REQUIRE(observation.after.size() == 1u);
        CHECK(observation.after[0] == 0 && observation.receivers[0] == first);
        CHECK(first->values.body() == body && body->first == nullptr && body->last == nullptr && body->end == nullptr);
        CHECK(variables.body() == registry && nodes->u010[0] == nullptr && foreignSlot == nullptr);
        CHECK(nodes->n == 1 && nodes->u0f0[0] != nullptr && nodes->u0f0[6] == nullptr);
        REQUIRE(nodes->u008); CHECK(nodes->u008[0] == 11.0 && nodes->u008[1] == 12.0 && nodes->u008[2] == 13.0);
        CHECK(laterNodes->u008 == nullptr && unknownScene(candidate.model));
        CHECK(def::model::g_timer_read_arrays.count == 1);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        if (parked) {
            CHECK(prepareGeometryTempPrefix(candidate.model, mode).status == GeometryTempPrepareStatus::ParkedModel);
            CHECK(def::rt::ModuleHeapRole::prepareCurrentModule()->producerReport().heapCreateCalls == 0u);
        } else {
            const auto report = retained->report();
            REQUIRE(report.producer.origin == def::rt::HeapRoleOrigin::ActualWin32);
            CHECK(report.producer.heapCreateCalls == 1u && report.producer.slotStores == 1u);
            CHECK(report.producer.heapFlags == 0u && report.producer.initialBytes == 4096u && report.producer.maximumBytes == 0u);
            CHECK(report.producer.compatibilityCalls == 1u && report.producer.producerResult == 1);
            if (success) {
                REQUIRE(report.sealed); REQUIRE(report.temporary.stage == def::rt::Temp0Stage::RawNonNullPending);
                REQUIRE(report.temporary.returnToken != 0u); returned = report.temporary.returnToken;
                CHECK(report.modelAlive && report.modelToken == reinterpret_cast<std::uintptr_t>(&candidate.model));
                CHECK(report.objectToken == reinterpret_cast<std::uintptr_t>(object));
                CHECK(report.receiverToken == reinterpret_cast<std::uintptr_t>(first));
                CHECK(report.bodyToken == reinterpret_cast<std::uintptr_t>(body));
                CHECK(report.registryToken == reinterpret_cast<std::uintptr_t>(registry));
                CHECK(report.slotToken == reinterpret_cast<std::uintptr_t>(first->u30));
                CHECK(report.frameToken != 0u && report.ordinal == 1);
                CHECK(report.temporary.heapToken == report.producer.heapToken);
                CHECK(report.temporary.requestedBytes == 0u && report.temporary.kernelBytes == 1u && report.temporary.flags == 0u);
                CHECK(report.temporary.heapSlotReads == 1u && report.temporary.heapAllocCalls == 1u && report.temporary.effectCount == 4u);
                CHECK(!report.temporary.mallocReturned && !report.temporary.operatorNewReturned && !report.temporary.zeroCopyEntered);
                CHECK(!report.temporary.valueAllocationEntered && !report.temporary.temporaryFreeEntered && !report.temporary.wholeLeafReturned);
                CHECK(report.preTempDataObservedNull == valueMode);
                if (valueMode) {
                    const auto& value = report.scalar;
                    REQUIRE(value.stage == def::rt::UnpublishedScalarStage::BeforeBodyPublication);
                    REQUIRE(value.bufferToken != 0u); buffer = value.bufferToken;
                    CHECK(value.origin == def::rt::HeapRoleOrigin::ActualWin32);
                    CHECK(value.bufferToken != returned && value.temporaryToken == returned);
                    CHECK(value.roleToken == report.temporary.roleToken && value.heapToken == report.producer.heapToken);
                    CHECK(value.started && value.currentOwnerProved && value.tempNormalTailReturned && value.tempZeroCopyReturned);
                    CHECK(value.requestedBytes == 8u && value.kernelBytes == 8u && value.flags == 0u);
                    CHECK(value.heapSlotReads == 1u && value.heapAllocCalls == 1u);
                    CHECK(report.temporary.heapAllocCalls + value.heapAllocCalls == 2u);
                    CHECK(value.valueRawReturnKnown && value.valueRawReturnNonNull && value.valueNormalTailReturned);
                    CHECK(value.leftEmptyCopyReturned && value.fillCommitted && value.initializedBits == 0u);
                    CHECK(value.rightEmptyCopyReturned && value.candidateOldNullFreeReturned);
                    REQUIRE(value.effectCount == 16u);
                    CHECK(value.effects[0] == def::rt::UnpublishedScalarEffect::TempMallocTail);
                    CHECK(value.effects[3] == def::rt::UnpublishedScalarEffect::TempZeroCopy);
                    CHECK(value.effects[4] == def::rt::UnpublishedScalarEffect::CurrentOwnerProved);
                    CHECK(value.effects[5] == def::rt::UnpublishedScalarEffect::ValueHeapSlotRead);
                    CHECK(value.effects[7] == def::rt::UnpublishedScalarEffect::ValueHeapAllocEntered);
                    CHECK(value.effects[8] == def::rt::UnpublishedScalarEffect::ValueHeapAllocReturned);
                    CHECK(value.effects[11] == def::rt::UnpublishedScalarEffect::LeftEmptyCopy);
                    CHECK(value.effects[12] == def::rt::UnpublishedScalarEffect::Fill);
                    CHECK(value.effects[13] == def::rt::UnpublishedScalarEffect::RightEmptyCopy);
                    CHECK(value.effects[14] == def::rt::UnpublishedScalarEffect::OldNullFreePrefix);
                    CHECK(value.effects[15] == def::rt::UnpublishedScalarEffect::BeforeBodyPublication);
                    CHECK(!value.bodyPublished && !value.slotPublished && !value.heapFreeEntered && !value.nativeFreeObserved);
                    CHECK(!value.temporaryReleased && !value.bufferReleased && !value.vectorReturned && !value.wholeLeafReturned);
                } else CHECK(report.scalar.stage == def::rt::UnpublishedScalarStage::NotEnabled);
            } else CHECK(!report.sealed && report.temporary.heapAllocCalls == 0u && report.temporary.returnToken == 0u);
        }
        if (mutation >= 0) CHECK(first->cur[0] == 0 && first->cur[1] == 7 && first->prev[1] == -9);
        else {
            CHECK(first->cur[0] == 1 && first->cur[1] == 1 && first->prev[0] == beforeCur[0][0]);
            CHECK(first->prev[1] == 7 && first->cur[2] == 1 && first->prev[2] == 8);
            std::size_t i = 0;
            for (const auto* variable : variables.items()) {
                if (i != 0) for (std::size_t d = 0; d < 3; ++d) {
                    CHECK(variable->cur[d] == beforeCur[i][d]); CHECK(variable->prev[d] == beforePrev[i][d]);
                }
                ++i;
            }
        }
        object->owner = restoreBinding.owner; first->u30 = restoreBinding.slot; sharedBody.reset(); first->cur[2] = 1;
        CHECK(first->values.use_count() == 1);
        step.u15f8[1] = 0; observation.adversarialMutation = {};
        { ScopedTrace tracing(trace); ScopedSizingObservation sizing(observation); refresh(candidate.model); }
        CHECK(trace.count == 1u && observation.after.size() == 1u);
        CHECK(prepareGeometryTempPrefix(candidate.model, def::rt::HeapPrefixMode::UnpublishedScalar).status == GeometryTempPrepareStatus::ParkedModel);
        CHECK(nodes->u010[0] == nullptr && body->first == nullptr && laterNodes->u008 == nullptr && unknownScene(candidate.model));
        if (retained) {
            std::weak_ptr<GeometryTempPrefixBinding> weak = retained; retained.reset();
            retained = weak.lock(); REQUIRE(retained);
            CHECK(retained == preparedGeometryTempPrefix(candidate.model));
            CHECK(retained->report().temporary.returnToken == returned);
            CHECK(retained->report().temporary.heapAllocCalls == (success ? 1u : 0u));
            CHECK(retained->report().scalar.bufferToken == buffer);
            CHECK(retained->report().scalar.heapAllocCalls == (success && valueMode ? 1u : 0u));
        }
    }
    if (retained) {
        std::weak_ptr<GeometryTempPrefixBinding> afterModel = retained;
        retained.reset(); retained = afterModel.lock(); REQUIRE(retained);
        const auto history = retained->report();
        CHECK(!history.modelAlive); CHECK(history.temporary.returnToken == returned);
        CHECK(history.temporary.heapAllocCalls == (success ? 1u : 0u));
        CHECK(!history.temporary.temporaryFreeEntered && !history.temporary.heapDestroyEntered);
        CHECK(history.scalar.bufferToken == buffer);
        CHECK(history.scalar.heapAllocCalls == (success && valueMode ? 1u : 0u));
        CHECK(!history.scalar.bodyPublished && !history.scalar.slotPublished && !history.scalar.heapFreeEntered);
        Candidate other(database.handle());
        CHECK(prepareGeometryTempPrefix(other.model).status == GeometryTempPrepareStatus::ForeignModel);
        CHECK(!preparedGeometryTempPrefix(other.model));
        CHECK(retained->report().temporary.returnToken == returned);
    }
}

void launchGeometryTempChild(const char* scenario)
{
    std::array<wchar_t, 32768> path{};
    const auto length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    REQUIRE(length != 0 && length < path.size());
    std::wstring command = L"\"" + std::wstring(path.data(), length) + L"\" --geometry-temp-child ";
    for (const char* p = scenario; *p; ++p) command.push_back(static_cast<wchar_t>(*p));
    STARTUPINFOW startup{}; startup.cb = sizeof(startup); PROCESS_INFORMATION process{};
    REQUIRE(CreateProcessW(path.data(), command.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW,
        nullptr, nullptr, &startup, &process) != FALSE);
    CloseHandle(process.hThread);
    const auto waited = WaitForSingleObject(process.hProcess, 30000);
    DWORD status = 1;
    if (waited != WAIT_OBJECT_0) {
        (void)TerminateProcess(process.hProcess, 0xe1550001u);
        (void)WaitForSingleObject(process.hProcess, 2000);
        testkit::fail(__FILE__, __LINE__, std::string("geometry temp child timeout: ") + scenario);
    } else if (!GetExitCodeProcess(process.hProcess, &status)) {
        testkit::fail(__FILE__, __LINE__, std::string("geometry temp child status unavailable: ") + scenario);
    }
    CloseHandle(process.hProcess);
    if (status != 0) testkit::fail(__FILE__, __LINE__, std::string("geometry temp child failed: ") + scenario + " exit=" + std::to_string(status));
}
} // namespace

TEST_CASE(geometry_temp_prefix_hidden_process_scenarios)
{
    for (const auto* scenario : {"success6000", "success11000", "guard0", "guard1", "guard2", "guard3", "guard4", "parked"})
        launchGeometryTempChild(scenario);
}

TEST_CASE(geometry_unpublished_value_prefix_hidden_process_scenarios)
{
    for (const auto* scenario : {"valuesuccess6000", "valuesuccess11000", "valueguard0", "valueguard1", "valueguard2",
                               "valueguard3", "valueguard4", "valueparked"})
        launchGeometryTempChild(scenario);
}

int main(int argc, char** argv)
{
    // Test-file-local dispatch only. Child mode never runs the parent launcher
    // or testkit::run_all(), which has no filtering API.
    if (argc == 3 && std::strcmp(argv[1], "--geometry-temp-child") == 0) {
        try { runGeometryTempChild(argv[2]); }
        catch (const testkit::Abort&) {}
        catch (const std::exception& error) { testkit::fail(__FILE__, __LINE__, error.what()); }
        return testkit::failures() == 0 ? 0 : 1;
    }
    if (argc != 1) return 2;
    return testkit::run_all();
}
