// CP141: real same-step geometry reader with a nonzero main-node width-mismatch return.
// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <limits>
#include <memory>
#include <vector>

#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <defmodel/step/db_step.hpp>
#include "m2m/engine.hpp"
#include "../../src/engine/engine_internal.hpp"

using namespace m2m;

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

struct Fixture {
    engine::GuiModel model{nullptr, 3};
    def::model::LLOBJ_DATA* main = nullptr;
    def::model::Cls_14017ef20* nodes = nullptr;
    def::model::Cls_140182880* elements = nullptr;
    double* mainCoordinates = nullptr;

    Fixture(std::int32_t width = 4, std::int32_t metadata = 2, std::int32_t version = 11000)
    {
        auto* object = model.objectWrap(1, true, false); REQUIRE(object);
        def::model::geo_kw::ensure_geo_list(*object);
        if (def::model::geo_kw::geometry_count(*object) == 0)
            def::model::geo_kw::geo_list_append_default(def::model::geo_kw::geo_list(*object));
        auto* geometry = def::model::geo_kw::geometry(*object, 0); REQUIRE(geometry);
        auto* curve = geometry->add_edge_curve(); REQUIRE(curve);
        curve->curve_type = 1; curve->n_points = 2;
        auto* points = curve->allocate_points(); REQUIRE(points);
        const double line[6] = {-2.0, 0.0, 0.0, 2.0, 0.0, 0.0};
        std::memcpy(points, line, sizeof line);
        engine::viewerSync(model, object, true);
        geometry->mesh = new def::model::OBJ_MESH2D();
        nodes = reinterpret_cast<def::model::Cls_14017ef20*>(geometry->mesh->s008);
        elements = reinterpret_cast<def::model::Cls_140182880*>(geometry->mesh->s148);
        nodes->u008 = static_cast<double*>(std::calloc(8, sizeof(double)));
        elements->u008 = static_cast<std::int32_t*>(std::calloc(4, sizeof(std::int32_t)));
        REQUIRE(nodes->u008); REQUIRE(elements->u008);
        nodes->n = 4; elements->n = 1; elements->m = width; elements->u0f8 = metadata;
        const double xy[8] = {0.0, 0.0, 2.0, 0.0, 2.0, 1.0, 0.0, 1.0};
        const std::int32_t connectivity[4] = {1, 2, 3, 4};
        std::memcpy(nodes->u008, xy, sizeof xy);
        std::memcpy(elements->u008, connectivity, sizeof connectivity);
        auto& step = *model.step;
        nodes->u130 = &step; elements->u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        REQUIRE(step.db == nullptr); REQUIRE(step.u1b78 == nullptr); REQUIRE(step.u1b88 == nullptr);
        step.u1b58 = 0; step.u26c4[0x0b] = 0; step.step_version = version; step.u15f8[0] = 3;
        const std::int32_t dimension = 3;
        def::model::select_index_tables(&dimension);
        step.u1dc0[151] = 2;
        main = def::model::llobj_ctor::get_or_create(&step, 2); REQUIRE(main);
        REQUIRE(main->owner == &step); REQUIRE(def::model::llobj_not_kind7(main));
        main->u87c[0] = 1; main->u87c[1] = 1;
        *engine::at<std::uint8_t>(main, 0x3610) = 0;
        *engine::at<std::int32_t>(main, 0x3984) = 1;
        def::model::bc::alloc_node_arrays(*main, step.u15f8);
        mainCoordinates = *engine::at<double*>(main, 0x8a8); REQUIRE(mainCoordinates);
        mainCoordinates[0] = 11.0; mainCoordinates[1] = 12.0; mainCoordinates[2] = 13.0;
        REQUIRE(engine::viewerItemCount(model) == 1);
        def::model::g_timer_read_obj_node_state_variable.count = 7;
        def::model::g_timer_read_arrays.count = 0;
    }
    ~Fixture()
    {
        // Repair declared test mutations before the real owning model destroys its objects.
        if (nodes) nodes->u130 = nullptr;
        if (elements) elements->u0f0 = nullptr;
        if (main) {
            main->owner = model.step.get();
            if (mainCoordinates) *engine::at<double*>(main, 0x8a8) = mainCoordinates;
        }
    }
    void checkUnchanged() const
    {
        CHECK(main->owner == model.step.get()); CHECK(main->u87c[0] == 1); CHECK(main->u87c[1] == 1);
        CHECK(*engine::at<double*>(main, 0x8a8) == mainCoordinates);
        CHECK(mainCoordinates[0] == 11.0 && mainCoordinates[1] == 12.0 && mainCoordinates[2] == 13.0);
        CHECK(nodes->u130 == model.step.get());
        CHECK(elements->u0f0 == reinterpret_cast<def::model::LLOBJ_DATA*>(model.step.get()));
        CHECK(nodes->n == 4); CHECK(nodes->u008[5] == 1.0); CHECK(elements->u008[2] == 3);
        CHECK(model.step->u1b78 == nullptr); CHECK(model.step->u1b88 == nullptr);
        CHECK(model.step->db == nullptr); CHECK(model.step->u26c4[0x0b] == 0);
        CHECK(engine::viewerItemCount(model) == 1);
    }
};

struct Trace {
    def::model::Cls_14017ef20* nodes;
    std::vector<std::int32_t> nodeCounts;
    std::function<void(std::size_t)> before = {};
};
Trace* activeTrace = nullptr;

std::uint64_t forward(std::uint64_t address, const std::uint64_t* args, int nargs)
{
    namespace ext = def::model::llobj::ext;
    std::array<std::uint64_t, 4> copied{};
    for (int i = 0; i < nargs && i < 4; ++i) copied[static_cast<std::size_t>(i)] = args[i];
    auto* trace = activeTrace;
    const bool record = trace && address == 0x140329e60 && nargs == 4 && copied[0] == ext::u(trace->nodes);
    const auto count = def::model::g_timer_read_obj_node_state_variable.count;
    if (record && trace->before) trace->before(trace->nodeCounts.size());
    struct RestoreHook {
        ext::Hook hook = ext::g_hook;
        RestoreHook() { ext::g_hook = nullptr; }
        ~RestoreHook() { ext::g_hook = hook; }
    } restore;
    const auto result = ext::call(address, copied[0], copied[1], copied[2], copied[3]);
    if (record) trace->nodeCounts.push_back(count);
    return result;
}

struct ScopedTrace {
    explicit ScopedTrace(Trace& trace)
    {
        REQUIRE(activeTrace == nullptr); REQUIRE(def::model::llobj::ext::g_hook == nullptr);
        activeTrace = &trace; def::model::llobj::ext::g_hook = forward;
    }
    ~ScopedTrace() { def::model::llobj::ext::g_hook = nullptr; activeTrace = nullptr; }
};

void checkScene(engine::GuiModel& model)
{
    bool valid = false; float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(model, valid, mn, mx)); CHECK(valid);
    CHECK(mn[0] == -2.0f && mx[0] == 2.0f); CHECK(mn[1] == 0.0f && mx[1] == 1.0f);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
}

bool unknownScene(engine::GuiModel& model)
{
    bool valid = false; float mn[3] = {}, mx[3] = {};
    try { engine::viewerSceneBox(model, valid, mn, mx); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}

} // namespace

TEST_CASE(geometry_nonzero_main_width_mismatch_completes_two_no_copy_readers)
{
    for (const auto version : {std::int32_t{6000}, std::int32_t{10999}, std::int32_t{11000},
        std::numeric_limits<std::int32_t>::max()})
    for (const std::int32_t width : {0, 4})
    for (const std::int32_t metadata : {2, 3}) {
        RestoreGlobals restore; Fixture fixture(width, metadata, version);
        const auto* currentStep = fixture.model.step.get();
        Trace trace{fixture.nodes, {}};
        { ScopedTrace scope(trace); engine::viewerRefresh(fixture.model); }
        REQUIRE(trace.nodeCounts.size() == 2u); CHECK(trace.nodeCounts[0] == 7); CHECK(trace.nodeCounts[1] == 3);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
        CHECK(def::model::g_timer_read_arrays.count == 2);
        checkScene(fixture.model); fixture.checkUnchanged();
        CHECK(fixture.model.step.get() == currentStep); CHECK(fixture.model.step->step_version == version);
        CHECK(fixture.elements->m == width); CHECK(fixture.elements->u0f8 == metadata);
    }
}

TEST_CASE(geometry_nonzero_main_width_mismatch_prerequisite_refusals_preserve_prefix)
{
    for (const std::int32_t width : {0, 4})
    for (int mode = 0; mode < 8; ++mode) {
        RestoreGlobals restore; Fixture fixture(width); engine::GuiModel foreign(nullptr, 3);
        auto& step = *fixture.model.step;
        switch (mode) {
        case 0: step.u1dc0[151] = step.u15f8[0]; break;
        case 1: *engine::at<double*>(fixture.main, 0x8a8) = nullptr; break;
        case 2: def::model::g_a_index[12] = 11; break;
        case 3: def::model::g_a_index[70] = 280; break;
        case 4: def::model::g_a_index[55] = 11; break;
        case 5: fixture.main->u87c[0] = -1; break;
        case 6: fixture.main->owner = foreign.step.get(); break;
        case 7: def::model::g_a_index[70] = -1; break;
        }
        Trace trace{fixture.nodes, {}};
        { ScopedTrace scope(trace); engine::viewerRefresh(fixture.model); }
        REQUIRE(trace.nodeCounts.size() == 1u); CHECK(unknownScene(fixture.model));
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
        CHECK(def::model::g_timer_read_arrays.count == 1);
        step.u1dc0[151] = 2; *engine::at<double*>(fixture.main, 0x8a8) = fixture.mainCoordinates;
        fixture.main->u87c[0] = 1; fixture.main->owner = &step;
        def::model::g_a_index[12] = 12; def::model::g_a_index[70] = 151; def::model::g_a_index[55] = 58;
        { ScopedTrace scope(trace); engine::viewerRefresh(fixture.model); }
        REQUIRE(trace.nodeCounts.size() == 3u); CHECK(trace.nodeCounts[2] == 3);
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
        CHECK(def::model::g_timer_read_arrays.count == 3);
        checkScene(fixture.model); fixture.checkUnchanged();
    }
}

TEST_CASE(geometry_nonzero_main_width_mismatch_rechecks_after_first_reader_stop)
{
    RestoreGlobals restore; Fixture fixture;
    auto& step = *fixture.model.step;
    Trace trace{fixture.nodes, {}}; bool sawFirstStop = false;
    trace.before = [&](std::size_t ordinal) {
        if (ordinal != 1) return;
        sawFirstStop = def::model::g_timer_read_obj_node_state_variable.count == 3 &&
            def::model::g_timer_read_arrays.count == 1;
        step.u1dc0[151] = step.u15f8[0];
    };
    { ScopedTrace scope(trace); engine::viewerRefresh(fixture.model); }
    REQUIRE(trace.nodeCounts.size() == 2u); CHECK(sawFirstStop); CHECK(unknownScene(fixture.model));
    CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
    CHECK(def::model::g_timer_read_arrays.count == 2);
    step.u1dc0[151] = 2; trace.before = {};
    { ScopedTrace scope(trace); engine::viewerRefresh(fixture.model); }
    REQUIRE(trace.nodeCounts.size() == 4u); CHECK(trace.nodeCounts[3] == 3);
    CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
    CHECK(def::model::g_timer_read_arrays.count == 4);
    checkScene(fixture.model); fixture.checkUnchanged();
}

int main() { return testkit::run_all(); }
