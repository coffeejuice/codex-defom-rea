// MDT packet V: the cached 3D viewer box (0x140c82d50 + 0x1411f5040(view, out, false)) of the
// objects of corpus 42_vdie's cogging operation (Id 4) after the KEY load 0x1400569d0.
// Expected values are hand calculations from the corpus KEY files (spec_V section 0: per component
// min / max of the float-rounded coordinates of every node a face references, first 3 entries of
// each DIEGEO face since e[0] == abs(e[3]) on the first face):
//   object 1 "Workpiece" (Objects/Object00001.KEY, geometry only after 0x140c7ce40; 18 nodes, all
//     referenced): x = +-556.27976216 -> float +-556.279785 (bits 0x440b11e8), y = z = +-202.16 ->
//     float +-202.160004 (0x434a28f6); (double)max - (double)min printed with %.7E = 1.1125596E+003,
//     4.0432001E+002.
//   object 2 "TOP_80MN_V_Die_D370_D550" (Objects/Object00007.KEY, 1013 nodes, all referenced):
//     x +-3.25E+002, y +-3.0E+002 (exact floats), z from 6.7186130278E+001 (float 67.1861267, bits
//     0x42865f4c) to 4.0718613028E+002 (float 407.186127, 0x43cb97d3); differences 650, 600, 340.
//   object 3 "BOTTOM_80MN_V_Die_D370_D550" (Objects/Object00008.KEY): the same with z mirrored,
//     -4.0718613028E+002 .. -6.7186130278E+001.
// The objects are type 5 (not rigid) without a mesh, so 0x1411ee800 shows the geometry (flag 3)
// and the box is the geometry rep's.
// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include <cstdint>
#include <cstddef>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <process.h>

#include <defdbio/db_query.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/layout/state_var_node.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/db_directory_receipt.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <def_pre/menu56.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "m2m/engine.hpp"
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/doe_property_table.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "moproj2mst.h"
#include "../../src/engine/engine_internal.hpp"
#include "../../src/engine/cogging_save.hpp"
#include "../../src/engine/gui_model_access.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

xml::NodePtr parseDisplayOptions(const std::string& text)
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(doc, text, &error, &line, &column));
    return doc.root;
}

void enableCrossSectionGeometry(engine::GuiModel& model)
{
    engine::readViewerDisplayOptions(model, 0,
        *parseDisplayOptions("<SfDispOpt><DispFlagList ObjectOn='1' CrossSectionGeoOn='1'/></SfDispOpt>"));
    const auto options = engine::viewerDisplayOptionSnapshot(model, 0);
    REQUIRE(options.has_value());
    REQUIRE(options->flags[1] == 1);
    REQUIRE(options->flags[43] == 1);
}

// The first internal access seam supports one synchronous void/bool result.
// Pointer/reference results cannot turn it into an unchecked retained borrow.
struct StepVoidResult { void operator()(def::model::DB_STEP_DATA*) const; };
struct StepBoolResult { bool operator()(def::model::DB_STEP_DATA*) const; };
struct StepPointerResult { def::model::DB_STEP_DATA* operator()(def::model::DB_STEP_DATA*) const; };
struct StepReferenceResult { def::model::DB_STEP_DATA& operator()(def::model::DB_STEP_DATA*) const; };
struct StepBoolReferenceResult { bool& operator()(def::model::DB_STEP_DATA*) const; };
template<class F> concept CanUseStepAccess = requires(engine::GuiModel& model, F callback) {
    engine::GuiModelAccess::withStep(model, callback);
};
static_assert(CanUseStepAccess<StepVoidResult> && CanUseStepAccess<StepBoolResult>);
static_assert(!CanUseStepAccess<StepPointerResult> && !CanUseStepAccess<StepReferenceResult>);
static_assert(!CanUseStepAccess<StepBoolReferenceResult>);
static_assert(sizeof(def::model::DB_STEP_DATA) == 0x3408);
static_assert(std::is_same_v<decltype(engine::GuiModel::step), std::unique_ptr<def::model::DB_STEP_DATA>>);
static_assert(!std::is_polymorphic_v<engine::GuiModel>);
static_assert(M2M_API_VERSION == 1);
static_assert(std::is_same_v<decltype(&m2m_api_version), int(*)()>);
static_assert(std::is_same_v<decltype(&m2m_set_codepage), int(*)(unsigned)>);
static_assert(std::is_same_v<decltype(&m2m_set_gui_unit), int(*)(unsigned)>);
static_assert(std::is_same_v<decltype(&m2m_set_deform_dir_w), int(*)(const wchar_t*)>);
static_assert(std::is_same_v<decltype(&m2m_convert_w),
    int(*)(const wchar_t*, const wchar_t*, unsigned, wchar_t*, std::size_t)>);
static_assert(std::is_same_v<decltype(&m2m_convert_utf8),
    int(*)(const char*, const char*, unsigned, char*, std::size_t)>);

using GeometryVariables = def::model::CowList<def::model::StateVarBase*>;

struct GeometryGetterDispatch {
    std::int32_t count, key;
    const void* dimensions;
    const void* before;
    const void* after;
    std::int32_t beforeIndexDimension, afterIndexDimension;
    std::int32_t beforeMar0, afterMar0, beforeArray0, afterArray0;
    std::int32_t beforeNodeTimerCount, afterNodeTimerCount;
    std::int32_t beforeStepVersion, afterStepVersion;
};

struct RestoreGeometryTimers {
    std::array<def::model::StepTimer*, 5> actual{
        &def::model::g_timer_read_elem_integer_info, &def::model::g_timer_read_record,
        &def::model::g_timer_read_arrays, &def::model::g_timer_read_obj_state_variable,
        &def::model::g_timer_read_obj_node_state_variable};
    std::array<std::unique_ptr<def::model::StepTimer>, 5> saved;
    RestoreGeometryTimers()
    {
        for (std::size_t i = 0; i < actual.size(); ++i)
            saved[i] = std::make_unique<def::model::StepTimer>(*actual[i]);
    }
    ~RestoreGeometryTimers()
    {
        for (std::size_t i = 0; i < actual.size(); ++i) *actual[i] = *saved[i];
    }
    void checkUnchanged(bool includeNode = true, bool includeArrays = true) const
    {
        for (std::size_t i = 0; i < actual.size(); ++i) {
            if ((!includeNode && i == 4) || (!includeArrays && i == 2)) continue;
            CHECK(std::memcmp(actual[i], saved[i].get(), sizeof(def::model::StepTimer)) == 0);
        }
    }
};

void checkGeometryArraysTimerAppend(const def::model::StepTimer& before, int invocations)
{
    REQUIRE(before.count >= 0);
    const auto& timer = def::model::g_timer_read_arrays;
    auto expectedCount = before.count;
    for (int i = 0; i < invocations && expectedCount < 100; ++i) ++expectedCount;
    CHECK(timer.count == expectedCount);
    CHECK(std::memcmp(timer.name, before.name, sizeof timer.name) == 0); CHECK(timer.flag == before.flag);
    CHECK(std::memcmp(timer.pad_539d, before.pad_539d, sizeof timer.pad_539d) == 0);
    for (std::int32_t i = 0; i < 100; ++i) {
        if (before.count <= i && i < expectedCount) {
            CHECK(std::string(timer.labels[i]) == "Assign the variable to the object data");
            CHECK(timer.subs[i] == nullptr);
            const std::size_t written = std::strlen(timer.labels[i]) + 1;
            CHECK(std::memcmp(timer.labels[i] + written, before.labels[i] + written,
                sizeof timer.labels[i] - written) == 0);
        } else {
            CHECK(std::memcmp(timer.labels[i], before.labels[i], sizeof timer.labels[i]) == 0);
            CHECK(timer.times[i] == before.times[i]); CHECK(timer.subs[i] == before.subs[i]);
        }
    }
}

struct RestoreGeometryIndexTables {
    std::int32_t dimension = def::model::g_index_tables_dim;
    std::array<std::int32_t, 53> mar{};
    std::array<std::int32_t, 129> arrays{};
    RestoreGeometryIndexTables()
    {
        for (std::size_t i = 0; i < mar.size(); ++i) mar[i] = def::model::g_mar_index[i];
        for (std::size_t i = 0; i < arrays.size(); ++i) arrays[i] = def::model::g_a_index[i];
    }
    ~RestoreGeometryIndexTables()
    {
        for (std::size_t i = 0; i < mar.size(); ++i) def::model::g_mar_index[i] = mar[i];
        for (std::size_t i = 0; i < arrays.size(); ++i) def::model::g_a_index[i] = arrays[i];
        def::model::g_index_tables_dim = dimension;
    }
};

struct GeometryGetterTrace {
    def::model::Cls_14017ef20* nodes;
    std::vector<GeometryGetterDispatch> dispatches;
    const def::model::DB_STEP_DATA* step = nullptr;
    std::function<void(std::size_t)> beforeCoordinateDispatch = {};
};

GeometryGetterTrace* geometryGetterTrace = nullptr;

std::uint64_t forwardGeometryGetter(std::uint64_t address, const std::uint64_t* args, int nargs)
{
    namespace ext = def::model::llobj::ext;
    std::array<std::uint64_t, 4> forwarded{};
    for (int i = 0; i < nargs && i < 4; ++i) forwarded[static_cast<std::size_t>(i)] = args[i];
    auto* trace = geometryGetterTrace;
    const bool record = trace && address == 0x140329e60 && nargs == 4 &&
        forwarded[0] == ext::u(trace->nodes);
    const auto* variables = record ? reinterpret_cast<const GeometryVariables*>(trace->nodes) : nullptr;
    const void* before = variables ? variables->body() : nullptr;
    const auto beforeIndexDimension = def::model::g_index_tables_dim;
    const auto beforeMar0 = def::model::g_mar_index[0];
    const auto beforeArray0 = def::model::g_a_index[0];
    const auto beforeNodeTimerCount = def::model::g_timer_read_obj_node_state_variable.count;
    const auto beforeStepVersion = trace && trace->step ? trace->step->step_version : 0;
    if (record && trace->beforeCoordinateDispatch)
        trace->beforeCoordinateDispatch(trace->dispatches.size());
    // Execute the real default dispatcher, including its mutable COW lookups.
    // Restore the hook even if a forwarded call throws.
    struct RestoreHook {
        ext::Hook hook = ext::g_hook;
        RestoreHook() { ext::g_hook = nullptr; }
        ~RestoreHook() { ext::g_hook = hook; }
    } restore;
    const auto result = ext::call(address, forwarded[0], forwarded[1], forwarded[2], forwarded[3]);
    if (record) trace->dispatches.push_back({static_cast<std::int32_t>(forwarded[1]),
        static_cast<std::int32_t>(forwarded[3]), reinterpret_cast<const void*>(forwarded[2]),
        before, variables->body(), beforeIndexDimension, def::model::g_index_tables_dim,
        beforeMar0, def::model::g_mar_index[0], beforeArray0, def::model::g_a_index[0],
        beforeNodeTimerCount, def::model::g_timer_read_obj_node_state_variable.count,
        beforeStepVersion, trace->step ? trace->step->step_version : 0});
    return result;
}

struct ScopedGeometryGetterTrace {
    explicit ScopedGeometryGetterTrace(GeometryGetterTrace& trace)
    {
        REQUIRE(def::model::llobj::ext::g_hook == nullptr);
        REQUIRE(geometryGetterTrace == nullptr);
        geometryGetterTrace = &trace;
        def::model::llobj::ext::g_hook = forwardGeometryGetter;
    }
    ~ScopedGeometryGetterTrace()
    {
        def::model::llobj::ext::g_hook = nullptr;
        geometryGetterTrace = nullptr;
    }
};

void checkGeometryGetterArguments(const GeometryGetterTrace& trace, const void* dimensions = nullptr)
{
    for (const auto& dispatch : trace.dispatches) {
        CHECK(dispatch.count == 4); CHECK(dispatch.key == 0);
        CHECK(dispatch.dimensions == dimensions);
    }
}

Project* openCorpus(Project& p, Ctx& ctx, const char* caseDir, const char* moproj)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return nullptr;
    const fs::path f = fs::path(corpus) / caseDir / moproj;
    const qt::String file{f.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    return &p;
}

std::uint32_t bits(float f)
{
    std::uint32_t u;
    std::memcpy(&u, &f, 4);
    return u;
}

float fromBits(std::uint32_t u)
{
    float f;
    std::memcpy(&f, &u, 4);
    return f;
}

def::model::OBJ_GEO2D* geometryLine(engine::GuiModel& m, double halfExtent, int objectNumber = 1)
{
    auto* object = m.objectWrap(objectNumber, true, false);
    def::model::geo_kw::ensure_geo_list(*object);
    if (def::model::geo_kw::geometry_count(*object) == 0)
        def::model::geo_kw::geo_list_append_default(def::model::geo_kw::geo_list(*object));
    auto* geometry = def::model::geo_kw::geometry(*object, 0);
    REQUIRE(geometry != nullptr);
    auto* curve = geometry->add_edge_curve();
    REQUIRE(curve != nullptr);
    curve->curve_type = 1;
    curve->n_points = 2;
    double* points = curve->allocate_points();
    REQUIRE(points != nullptr);
    points[0] = -halfExtent; points[1] = 0.0; points[2] = 0.0;
    points[3] = halfExtent; points[4] = 0.0; points[5] = 0.0;
    engine::viewerSync(m, object, true);
    return curve;
}

void populateGeometryMeshQuad(def::model::OBJ_MESH2D& mesh)
{
    auto* nodes = reinterpret_cast<def::model::Cls_14017ef20*>(mesh.s008);
    auto* elements = reinterpret_cast<def::model::Cls_140182880*>(mesh.s148);
    nodes->u008 = static_cast<double*>(std::calloc(8, sizeof(double)));
    elements->u008 = static_cast<std::int32_t*>(std::calloc(4, sizeof(std::int32_t)));
    REQUIRE(nodes->u008 != nullptr);
    REQUIRE(elements->u008 != nullptr);
    nodes->n = 4;
    elements->n = 1;
    elements->m = 4;
    const double xy[8] = {0.0, 0.0, 2.0, 0.0, 2.0, 1.0, 0.0, 1.0};
    const std::int32_t conn[4] = {1, 2, 3, 4}; // original geometry-mesh connectivity is one-based
    std::memcpy(nodes->u008, xy, sizeof xy);
    std::memcpy(elements->u008, conn, sizeof conn);
}

def::model::OBJ_MESH2D& geometryQuad(engine::GuiModel& m, int objectNumber = 1)
{
    geometryLine(m, 2.0, objectNumber);
    auto* geometry = def::model::geo_kw::geometry(*m.object(objectNumber), 0);
    geometry->mesh = new def::model::OBJ_MESH2D();
    populateGeometryMeshQuad(*geometry->mesh);
    return *geometry->mesh;
}

struct RestoreGeometryMeshFields {
    def::model::Cls_14017ef20& nodes;
    def::model::Cls_140182880& elements;
    double* coordinates;
    std::int32_t* connectivity;
    void* nodeOwner;
    def::model::LLOBJ_DATA* elementOwner;
    std::int32_t nodeDimension, elementDimension, width;
    explicit RestoreGeometryMeshFields(def::model::OBJ_MESH2D& mesh)
        : nodes(*reinterpret_cast<def::model::Cls_14017ef20*>(mesh.s008)),
          elements(*reinterpret_cast<def::model::Cls_140182880*>(mesh.s148)),
          coordinates(nodes.u008), connectivity(elements.u008), nodeOwner(nodes.u130),
          elementOwner(elements.u0f0), nodeDimension(nodes.u138),
          elementDimension(elements.u0f8), width(elements.m) {}
    ~RestoreGeometryMeshFields()
    {
        nodes.u008 = coordinates; elements.u008 = connectivity;
        nodes.u130 = nodeOwner; elements.u0f0 = elementOwner;
        nodes.u138 = nodeDimension; elements.u0f8 = elementDimension; elements.m = width;
    }
};

void checkGeometryScene(engine::GuiModel& m, float maximumY)
{
    bool valid = false;
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(valid); CHECK(mn[0] == -2.0f && mx[0] == 2.0f);
    CHECK(mn[1] == 0.0f && mx[1] == maximumY);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
}

bool sceneBoxIsUnsupported(engine::GuiModel& m)
{
    bool valid = false;
    float mn[3] = {}, mx[3] = {};
    try {
        engine::viewerSceneBox(m, valid, mn, mx);
    } catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED;
    }
    return false;
}

// 0x140766200: (double)max.c - (double)min.c, one SUBSD of the widened floats; printed with the
// legacy CRT formatting (3-digit exponent)
std::string extent(const float mn[3], const float mx[3], int c)
{
    char buf[64];
    def::rt::crt::snprintf(buf, sizeof buf, "%.7E", static_cast<double>(mx[c]) - static_cast<double>(mn[c]));
    return buf;
}

}  // namespace

TEST_CASE(vdie42_cogging_viewer_boxes)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    Operation* op = operationById(p, 4);
    REQUIRE(op != nullptr);
    engine::FileEffects fx;
    engine::GuiModel* m = engine::loadFromKey(p, *op, true, fx);
    REQUIRE(m != nullptr);

    float mn[3] = {}, mx[3] = {};
    // object 1: the billet
    REQUIRE(engine::viewerBox(*m, 1, mn, mx));
    CHECK_EQ(bits(mn[0]), 0xc40b11e8u);
    CHECK_EQ(bits(mx[0]), 0x440b11e8u);
    CHECK_EQ(bits(mn[1]), 0xc34a28f6u);
    CHECK_EQ(bits(mx[1]), 0x434a28f6u);
    CHECK_EQ(bits(mn[2]), 0xc34a28f6u);
    CHECK_EQ(bits(mx[2]), 0x434a28f6u);
    CHECK_EQ(extent(mn, mx, 0), std::string("1.1125596E+003"));
    CHECK_EQ(extent(mn, mx, 1), std::string("4.0432001E+002"));
    CHECK_EQ(extent(mn, mx, 2), std::string("4.0432001E+002"));

    // object 2: top die
    REQUIRE(engine::viewerBox(*m, 2, mn, mx));
    CHECK(mn[0] == -325.0f && mx[0] == 325.0f);
    CHECK(mn[1] == -300.0f && mx[1] == 300.0f);
    CHECK_EQ(bits(mn[2]), 0x42865f4cu);
    CHECK_EQ(bits(mx[2]), 0x43cb97d3u);
    CHECK_EQ(extent(mn, mx, 0), std::string("6.5000000E+002"));
    CHECK_EQ(extent(mn, mx, 1), std::string("6.0000000E+002"));
    CHECK_EQ(extent(mn, mx, 2), std::string("3.4000000E+002"));

    // object 3: bottom die
    REQUIRE(engine::viewerBox(*m, 3, mn, mx));
    CHECK(mn[0] == -325.0f && mx[0] == 325.0f);
    CHECK(mn[1] == -300.0f && mx[1] == 300.0f);
    CHECK_EQ(bits(mn[2]), 0xc3cb97d3u);
    CHECK_EQ(bits(mx[2]), 0xc2865f4cu);
    CHECK_EQ(extent(mn, mx, 2), std::string("3.4000000E+002"));

    // no object 0 view (0x140c82d50 returns null for no < 1)
    CHECK(!engine::viewerBox(*m, 0, mn, mx));
}

TEST_CASE(multiblow_primary_die_command_changes_only_pdie_and_does_not_recreate_step)
{
    engine::GuiModel model(nullptr, 3);
    auto* const step = model.step.get();
    REQUIRE(step);
    auto& control = step->sim_ctrl;
    control.pdie1 = 71;
    control.nstep = 123;
    control.dttype.u58 = 7;
    control.tnow3 = 19;
    std::array<unsigned char, sizeof(def::model::DB_STEP_DATA)> expected{};
    // Snapshot the entire expected step with only the target field changed.
    control.pdie1 = 37;
    std::memcpy(expected.data(), step, expected.size());
    control.pdie1 = 71;
    model.setMultiBlowPrimaryDie(37);
    CHECK(model.step.get() == step);
    CHECK(std::memcmp(expected.data(), step, expected.size()) == 0);
    CHECK(!model.viewer);

    engine::GuiModel absent(nullptr, 3);
    absent.step.reset();
    absent.setMultiBlowPrimaryDie(99);
    CHECK(!absent.step);
    CHECK(!absent.viewer);
}

TEST_CASE(viewer_sync_uses_tail_number_and_keeps_null_object_shortcut_before_allocation)
{
    engine::GuiModel model(nullptr, 3);
    auto* const originalStep = model.step.get();
    REQUIRE(originalStep);
    CHECK(!model.viewer);
    engine::viewerSync(model, nullptr, true);
    CHECK(!model.viewer); CHECK(model.step.get() == originalStep);

    auto* const first = model.objectWrap(1, true, false); REQUIRE(first);
    REQUIRE(model.objectWrap(4, true, false));
    CHECK(model.objectCount() == 4);
    CHECK(def::pre::step_count_objects(model.step.get()) == 2);
    CHECK_EQ(model.lastObjectNumber(), 4);
    REQUIRE(model.object(2)); REQUIRE(model.object(3));
    CHECK(*engine::at<std::int32_t>(model.object(2), 0x0c) == 7);
    CHECK(*engine::at<std::int32_t>(model.object(3), 0x0c) == 7);
    engine::viewerClear(model);
    CHECK(engine::viewerItemCount(model) == 0);
    engine::viewerSync(model, first, false);
    CHECK(engine::viewerItemCount(model) == 4);
    CHECK(model.step.get() == originalStep); CHECK(model.objectCount() == 4);
    CHECK(def::pre::step_count_objects(model.step.get()) == 2);
    CHECK(*engine::at<std::int32_t>(model.object(2), 0x0c) == 7);
    CHECK(*engine::at<std::int32_t>(model.object(3), 0x0c) == 7);

    engine::GuiModel absent(nullptr, 3);
    absent.step.reset();
    CHECK_EQ(absent.lastObjectNumber(), 0);
    absent.initializeMultiBlowLoadControl();
    CHECK(!absent.step);
    engine::viewerSync(absent, nullptr, false);
    CHECK(!absent.step); CHECK(!absent.viewer);
}

TEST_CASE(viewer_rebuild_preserves_normal_flags_and_missing_step_shortcut_before_clear)
{
    engine::GuiModel model(nullptr, 2);
    auto* const first = model.objectWrap(1, true, false); REQUIRE(first);
    auto* const second = model.objectWrap(2, true, false); REQUIRE(second);
    engine::viewerSync(model, first, true);
    engine::viewerSync(model, second, true);
    auto* const originalStep = model.step.get();
    const auto originalViewer = model.viewer;
    const auto firstSettings = engine::viewerDisplayOptionSnapshot(model, 0);
    const auto secondSettings = engine::viewerDisplayOptionSnapshot(model, 1);
    REQUIRE(firstSettings && secondSettings);
    const auto checkSettings = [&] {
        for (const int index : {0, 1}) {
            const auto actual = engine::viewerDisplayOptionSnapshot(model, index);
            REQUIRE(actual);
            const auto& before = index == 0 ? *firstSettings : *secondSettings;
            CHECK(actual->mode == before.mode && actual->flags == before.flags);
            CHECK(actual->cachedMesh == before.cachedMesh && actual->cachedGeometry == before.cachedGeometry);
        }
    };
    for (const bool reset : {false, true}) {
        for (const bool contacts : {false, true}) {
            engine::viewerRebuild(model, reset, contacts);
            CHECK(model.step.get() == originalStep && model.viewer == originalViewer);
            CHECK(model.object(1) == first && model.object(2) == second);
            CHECK(model.objectCount() == 2 && engine::viewerItemCount(model) == 2);
            checkSettings();
        }
    }
    {
        // Keep the actual objects alive while exercising the legacy nullable field.
        struct RestoreStep {
            engine::GuiModel& model;
            std::unique_ptr<def::model::DB_STEP_DATA> step;
            ~RestoreStep() { model.step = std::move(step); }
        } held{model, std::move(model.step)};
        for (const bool reset : {false, true}) {
            for (const bool contacts : {false, true}) {
                engine::viewerRebuild(model, reset, contacts);
                CHECK(!model.step && model.viewer == originalViewer);
                CHECK(engine::viewerItemCount(model) == 2);
                checkSettings();
            }
        }
    }
    CHECK(model.step.get() == originalStep);
    engine::GuiModel absent(nullptr, 2);
    absent.step.reset();
    CHECK_EQ(absent.lastObjectNumber(), 0);
    absent.initializeMultiBlowLoadControl();
    CHECK(!absent.step);
    for (const bool reset : {false, true}) {
        for (const bool contacts : {false, true}) {
            engine::viewerRebuild(absent, reset, contacts);
            CHECK(!absent.step && !absent.viewer);
        }
    }
}

TEST_CASE(empty_3d_item_rect_falls_back_to_the_model_box)
{
    // 0x140c8e0f0 converts the empty rect to a valid zero-sized box, but
    // its radius fails the ordered >1e-6 gate. The returned model box is
    // still invalid. Test both a bound empty geometry and an unbound item0.
    engine::GuiModel m(nullptr, 3);
    auto* object = m.objectWrap(1, true, true);
    REQUIRE(object != nullptr);
    def::model::geo_kw::ensure_geo_list(*object);
    if (def::model::geo_kw::geometry_count(*object) == 0)
        def::model::geo_kw::geo_list_append_default(def::model::geo_kw::geo_list(*object));
    engine::viewerSync(m, object, true);
    bool valid = true;
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(!valid);
    def::model::geo_kw::geo_list_clear(def::model::geo_kw::geo_list(*object));
    engine::viewerSync(m, object, true);
    valid = true;
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(!valid);
    CHECK(bits(mn[0]) == 0u && bits(mx[0]) == 0u);
}

TEST_CASE(scene_rect_radius_uses_float_length_and_an_ordered_threshold)
{
    // Fixed float inputs around 1e-6. Original Qt multiplies/adds floats,
    // widens the sum, then sqrt returns double. 0x358637bd remains below
    // the double threshold after that arithmetic; its next float exceeds it.
    const std::uint32_t halfExtents[] = {0x358637bcu, 0x358637bdu, 0x358637beu,
                                         0x7f800000u, 0x7fc00000u};
    const bool selected[] = {false, false, true, false, false};
    for (int i = 0; i < 5; ++i) {
        engine::GuiModel m(nullptr, 3);
        const float half = fromBits(halfExtents[i]);
        geometryLine(m, static_cast<double>(half));
        bool valid = false;
        float mn[3] = {}, mx[3] = {};
        REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
        CHECK(valid == selected[i]);
        if (selected[i]) {
            CHECK(bits(mn[0]) == (halfExtents[i] | 0x80000000u));
            CHECK(bits(mx[0]) == halfExtents[i]);
            CHECK(mn[1] == 0.0f && mx[1] == 0.0f && mn[2] == 0.0f && mx[2] == 0.0f);
        }
    }
}

TEST_CASE(geometry_item_rect_refresh_and_missing_source_keep_original_state_order)
{
    engine::GuiModel m(nullptr, 3);
    auto* curve = geometryLine(m, 2.0);
    auto* object = m.object(1);
    bool valid = false;
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(valid && mn[0] == -2.0f && mx[0] == 2.0f);
    curve->points[0] = -4.0;
    curve->points[3] = 4.0;
    engine::viewerRefresh(m);  // vt+0x48 rereads the bound geometry
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(valid && mn[0] == -4.0f && mx[0] == 4.0f);
    def::model::geo_kw::geo_list_clear(def::model::geo_kw::geo_list(*object));
    engine::viewerRefresh(m);  // 0x14126cae0 returns null: cached rect is unchanged
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(valid && mn[0] == -4.0f && mx[0] == 4.0f);
    engine::viewerSync(m, object, true);  // rebind drops the item and creates empty item0
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(!valid);
}

TEST_CASE(display_refresh_scene_refreshes_only_shown_main_secondary_and_third_caches)
{
    engine::GuiModel main(nullptr, 3);
    engine::GuiModel third(nullptr, 3);
    auto* thirdCurve = geometryLine(third, 4.0);
    Project project;
    ProjectScope scope(project);
    auto* mainCurve = geometryLine(main, 2.0);
    engine::displayRefreshScene(project, &main);
    REQUIRE(project.display);

    auto* secondary = engine::displaySecond(*project.display);
    REQUIRE(secondary);
    auto* secondaryCurve = geometryLine(*secondary, 3.0);
    auto& models = engine::displayModels(*project.display);
    models.third.reset(&third);

    const auto checkX = [](engine::GuiModel& model, float low, float high) {
        bool valid = false;
        float mn[3] = {}, mx[3] = {};
        REQUIRE(engine::viewerSceneBox(model, valid, mn, mx));
        CHECK(valid && mn[0] == low && mx[0] == high);
    };
    const auto moveCurve = [](def::model::OBJ_GEO2D* curve, double halfExtent) {
        REQUIRE(curve);
        curve->points[0] = -halfExtent;
        curve->points[3] = halfExtent;
    };

    // Hidden secondary/third caches remain stale while the actual caller fits main.
    moveCurve(mainCurve, 5.0); moveCurve(secondaryCurve, 6.0); moveCurve(thirdCurve, 7.0);
    checkX(main, -2.0f, 2.0f); checkX(*secondary, -3.0f, 3.0f); checkX(third, -4.0f, 4.0f);
    engine::displayRefreshScene(project, &main);
    checkX(main, -5.0f, 5.0f); checkX(*secondary, -3.0f, 3.0f); checkX(third, -4.0f, 4.0f);

    // Enabling only secondary refreshes its cache, but the hidden third remains stale.
    models.secondShown = true;
    moveCurve(secondaryCurve, 8.0);
    engine::displayRefreshScene(project, &main);
    checkX(*secondary, -8.0f, 8.0f); checkX(third, -4.0f, 4.0f);

    // Both shown secondary and third are refreshed during the same caller visit.
    models.thirdShown = true;
    moveCurve(secondaryCurve, 10.0); moveCurve(thirdCurve, 11.0);
    engine::displayRefreshScene(project, &main);
    checkX(*secondary, -10.0f, 10.0f); checkX(third, -11.0f, 11.0f);

    // Normal teardown clears the canonical secondary guard and its owner.
    engine::displayDropSecond(*project.display);
    CHECK(!models.second.get() && !models.secondOwned);
    // A shown third remains refreshable with the naturally empty secondary slot.
    moveCurve(thirdCurve, 9.0);
    engine::displayRefreshScene(project, &main);
    checkX(third, -9.0f, 9.0f);
}

TEST_CASE(empty_and_populated_geometry_mesh_rectangles)
{
    engine::GuiModel m(nullptr, 3);
    geometryLine(m, 2.0);
    auto* object = m.object(1);
    auto* geometry = def::model::geo_kw::geometry(*object, 0);
    geometry->mesh = new def::model::OBJ_MESH2D();
    engine::viewerRefresh(m);  // 0x1408ee940 skips mesh with zero counts
    bool valid = false;
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(valid && mn[0] == -2.0f && mx[0] == 2.0f);
    populateGeometryMeshQuad(*geometry->mesh);
    engine::viewerRefresh(m);
    REQUIRE(engine::viewerSceneBox(m, valid, mn, mx));
    CHECK(valid);
    CHECK(mn[0] == -2.0f && mx[0] == 2.0f);
    CHECK(mn[1] == 0.0f && mx[1] == 1.0f);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
}

TEST_CASE(cross_section_geometry_refresh_unions_every_known_item_box)
{
    engine::GuiModel m(nullptr, 3);
    geometryLine(m, 2.0);
    auto* object = m.object(1);
    REQUIRE(object != nullptr);
    auto* list = def::model::geo_kw::geo_list(*object);
    REQUIRE(list != nullptr);
    def::model::geo_kw::geo_list_append_default(list);
    REQUIRE(def::model::geo_kw::geometry_count(*object) == 2);
    auto* secondGeometry = def::model::geo_kw::geometry(*object, 1);
    REQUIRE(secondGeometry != nullptr);
    auto* secondCurve = secondGeometry->add_edge_curve();
    REQUIRE(secondCurve != nullptr);
    secondCurve->curve_type = 1;
    secondCurve->n_points = 2;
    double* points = secondCurve->allocate_points();
    REQUIRE(points != nullptr);
    points[0] = 3.0; points[1] = -1.0; points[2] = 0.0;
    points[3] = 5.0; points[4] = 1.0; points[5] = 0.0;
    engine::viewerSync(m, object, true);
    REQUIRE(engine::viewerItemCount(m) == 1);
    enableCrossSectionGeometry(m);

    engine::viewerRefresh(m);
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerBox(m, 1, mn, mx));
    CHECK(mn[0] == -2.0f && mx[0] == 5.0f);
    CHECK(mn[1] == -1.0f && mx[1] == 1.0f);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
}

TEST_CASE(cross_section_geometry_refresh_unions_qualified_mesh_item_box)
{
    engine::GuiModel m(nullptr, 3);
    auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    REQUIRE(saved.nodes.u130 == nullptr);
    REQUIRE(saved.elements.u0f0 == nullptr);
    enableCrossSectionGeometry(m);

    engine::viewerRefresh(m);
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerBox(m, 1, mn, mx));
    CHECK(mn[0] == -2.0f && mx[0] == 2.0f);
    CHECK(mn[1] == 0.0f && mx[1] == 1.0f);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
    CHECK(saved.nodes.u008 == saved.coordinates);
    CHECK(saved.elements.u008 == saved.connectivity);
    CHECK(saved.nodes.u130 == nullptr && saved.elements.u0f0 == nullptr);
}

TEST_CASE(cross_section_geometry_refresh_keeps_unqualified_mesh_refused)
{
    engine::GuiModel m(nullptr, 3);
    auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    auto* object = m.object(1);
    REQUIRE(object != nullptr);
    saved.nodes.u130 = object; // not the actual model step; reject before getter dispatch
    enableCrossSectionGeometry(m);

    GeometryGetterTrace trace{&saved.nodes, {}};
    bool refused = false;
    {
        ScopedGeometryGetterTrace scope(trace);
        try {
            engine::viewerRefresh(m);
        } catch (const Error& error) {
            refused = error.code == M2M_E_UNSUPPORTED &&
                std::string(error.what()).find("geometry-list item boxes (flag 0x2b") != std::string::npos;
        }
    }
    CHECK(refused);
    CHECK(trace.dispatches.empty());
    CHECK(saved.nodes.u008 == saved.coordinates);
    CHECK(saved.elements.u008 == saved.connectivity);
    CHECK(saved.nodes.u130 == object);
}

TEST_CASE(cross_section_geometry_refresh_validates_all_items_before_any_box_union)
{
    engine::GuiModel m(nullptr, 3);
    geometryLine(m, 2.0); // first item has a known, nonempty rectangle
    auto* object = m.object(1);
    REQUIRE(object != nullptr);
    auto* list = def::model::geo_kw::geo_list(*object);
    REQUIRE(list != nullptr);
    def::model::geo_kw::geo_list_append_default(list);
    auto* secondGeometry = def::model::geo_kw::geometry(*object, 1);
    REQUIRE(secondGeometry != nullptr);
    secondGeometry->mesh = new def::model::OBJ_MESH2D();
    populateGeometryMeshQuad(*secondGeometry->mesh);
    engine::viewerSync(m, object, true);

    auto& mesh = *secondGeometry->mesh;
    RestoreGeometryMeshFields saved(mesh);
    saved.nodes.u130 = object; // later item is unqualified and cannot be refreshed
    enableCrossSectionGeometry(m);

    GeometryGetterTrace trace{&saved.nodes, {}};
    bool refused = false;
    {
        ScopedGeometryGetterTrace scope(trace);
        try {
            engine::viewerRefresh(m);
        } catch (const Error& error) {
            refused = error.code == M2M_E_UNSUPPORTED &&
                std::string(error.what()).find("geometry-list item boxes (flag 0x2b") != std::string::npos;
        }
    }
    CHECK(refused);
    CHECK(trace.dispatches.empty());
    float mn[3] = {}, mx[3] = {};
    REQUIRE(engine::viewerBox(m, 1, mn, mx));
    // The first edge item was refreshed, but the native list aggregator is a
    // second pass. Candidate refusal before that pass leaves the view box empty.
    CHECK(mn[0] == 0.0f && mx[0] == 0.0f);
    CHECK(mn[1] == 0.0f && mx[1] == 0.0f);
    CHECK(mn[2] == 0.0f && mx[2] == 0.0f);
    CHECK(saved.nodes.u008 == saved.coordinates);
    CHECK(saved.elements.u008 == saved.connectivity);
    CHECK(saved.nodes.u130 == object);
}

TEST_CASE(populated_geometry_mesh_rejects_unqualified_arrays_and_dimensions)
{
    for (int mode = 0; mode < 5; ++mode) {
        engine::GuiModel m(nullptr, 3);
        geometryLine(m, 2.0);
        auto* object = m.object(1);
        auto* geometry = def::model::geo_kw::geometry(*object, 0);
        geometry->mesh = new def::model::OBJ_MESH2D();
        populateGeometryMeshQuad(*geometry->mesh);
        auto* nodes = reinterpret_cast<def::model::Cls_14017ef20*>(geometry->mesh->s008);
        auto* elements = reinterpret_cast<def::model::Cls_140182880*>(geometry->mesh->s148);
        double* savedCoords = nodes->u008;
        switch (mode) {
        case 0: nodes->u008 = nullptr; break;
        case 1: nodes->u138 = 3; break; // 3D owner/layout is not the 2D builder contract
        case 2: elements->m = -1; break;
        case 3: elements->u008[2] = 5; break; // invalid one-based node index
        case 4: nodes->u008[0] = std::numeric_limits<double>::quiet_NaN(); break;
        }
        engine::viewerRefresh(m);
        const bool refused = sceneBoxIsUnsupported(m);
        nodes->u008 = savedCoords; // keep OBJ_MESH2D's free_arrays owner intact for every case
        CHECK(refused);
    }
}

TEST_CASE(geometry_packed_xy_element_metadata_preserves_bounds_owners_and_getter_order)
{
    for (int profile = 0; profile < 3; ++profile)
    for (const std::int32_t elementMetadata : {2, 3})
    for (const std::int32_t width : {3, 4}) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        REQUIRE(saved.nodes.u138 == 2); REQUIRE(step.db == nullptr);
        saved.elements.u0f8 = elementMetadata; saved.elements.m = width;
        if (profile != 0) {
            saved.nodes.u130 = &step;
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        }
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = profile == 1 ? 1 : 0;
        const auto* currentStep = model.step.get();
        const auto cache = step.u26c4[0x0b];
        std::array<double, 8> coordinates;
        std::array<std::int32_t, 4> connectivity;
        std::memcpy(coordinates.data(), saved.coordinates, sizeof coordinates);
        std::memcpy(connectivity.data(), saved.connectivity, sizeof connectivity);
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 2u);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        CHECK(trace.dispatches[1].before == variables.body()); CHECK(trace.dispatches[1].after == variables.body());
        if (profile == 2) {
            CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
            CHECK(nodeTimer.count == 3); CHECK(arraysTimer.count == 2);
            CHECK(std::string(nodeTimer.labels[2]) == "Stop");
        } else { CHECK(nodeTimer.count == 7); CHECK(arraysTimer.count == 0); }
        checkGeometryScene(model, 1.0f);
        CHECK(model.step.get() == currentStep); CHECK(step.step_version == 11000);
        CHECK(step.u26c4[0x0b] == cache); CHECK(step.db == nullptr);
        CHECK(saved.nodes.u138 == 2); CHECK(saved.elements.u0f8 == elementMetadata);
        CHECK(saved.elements.m == width);
        CHECK(saved.nodes.u130 == (profile == 0 ? nullptr : &step));
        CHECK(saved.elements.u0f0 == (profile == 0 ? nullptr : reinterpret_cast<def::model::LLOBJ_DATA*>(&step)));
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        CHECK(std::memcmp(saved.coordinates, coordinates.data(), sizeof coordinates) == 0);
        CHECK(std::memcmp(saved.connectivity, connectivity.data(), sizeof connectivity) == 0);
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_element_metadata_three_keeps_later_layout_and_data_guards)
{
    for (const bool actualNullOwner : {false, true})
    for (int mode = 0; mode < 8; ++mode) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        saved.elements.u0f8 = 3;
        if (actualNullOwner) {
            saved.nodes.u130 = &step;
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        }
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = 0;
        switch (mode) {
        case 0: saved.nodes.u138 = 3; break;
        case 1: saved.elements.u0f8 = 0; break;
        case 2: saved.elements.u0f8 = -1; break;
        case 3: saved.elements.m = -1; break;
        case 4: saved.elements.u008[2] = 5; break;
        case 5: saved.nodes.u008[0] = std::numeric_limits<double>::quiet_NaN(); break;
        case 6: saved.nodes.u008[0] = std::numeric_limits<double>::infinity(); break;
        case 7: saved.nodes.u008[0] = std::numeric_limits<double>::max(); break;
        }
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 2u);
        CHECK(sceneBoxIsUnsupported(model));
        CHECK(nodeTimer.count == (actualNullOwner ? 3 : 7));
        CHECK(arraysTimer.count == (actualNullOwner ? 2 : 0));
        if (actualNullOwner) CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
        CHECK(step.step_version == 11000); CHECK(step.u26c4[0x0b] == 0);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        saved.nodes.u138 = 2; saved.elements.u0f8 = 3; saved.elements.m = 4;
        saved.nodes.u008[0] = 0.0; saved.elements.u008[2] = 3;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 4u); checkGeometryScene(model, 1.0f);
        CHECK(arraysTimer.count == (actualNullOwner ? 4 : 0));
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_native_vertex_triple_product_guard_follows_completed_getters)
{
    for (const bool actualNullOwner : {false, true})
    for (const std::int32_t elementMetadata : {2, 3}) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        struct RestoreCount {
            def::model::Cls_14017ef20& nodes;
            std::int32_t count;
            ~RestoreCount() { nodes.n = count; }
        } restoreCount{saved.nodes, saved.nodes.n};
        saved.elements.u0f8 = elementMetadata;
        if (actualNullOwner) {
            saved.nodes.u130 = &step;
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        }
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = 0;
        const auto largeCount = std::numeric_limits<std::int32_t>::max() / 3 + 1;
        REQUIRE(largeCount <= std::numeric_limits<std::int32_t>::max() / 2);
        saved.nodes.n = largeCount; // Actual small storage proves refusal before coordinate traversal.
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 2u); CHECK(sceneBoxIsUnsupported(model));
        CHECK(saved.nodes.n == largeCount);
        CHECK(nodeTimer.count == (actualNullOwner ? 3 : 7));
        CHECK(arraysTimer.count == (actualNullOwner ? 2 : 0));
        if (actualNullOwner) CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        CHECK(saved.coordinates[6] == 0.0 && saved.coordinates[7] == 1.0);
        saved.nodes.n = restoreCount.count;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 4u); checkGeometryScene(model, 1.0f);
        CHECK(arraysTimer.count == (actualNullOwner ? 4 : 0));
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_zero_width_populated_refreshes_coordinates_without_connectivity_entries)
{
    for (int profile = 0; profile < 3; ++profile)
    for (const std::int32_t elementMetadata : {2, 3}) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        REQUIRE(step.db == nullptr); REQUIRE(saved.nodes.u138 == 2);
        if (profile != 0) {
            saved.nodes.u130 = &step;
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        }
        saved.elements.u0f8 = elementMetadata; saved.elements.m = 0;
        saved.elements.u008[2] = 0; // Allocated, nonnull connectivity is not consumed at width0.
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = profile == 1 ? 1 : 0;
        const auto* currentStep = model.step.get(); const auto cache = step.u26c4[0x0b];
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 2u); checkGeometryScene(model, 1.0f);
        CHECK(saved.elements.u008[2] == 0);
        saved.elements.m = 4;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 4u); CHECK(sceneBoxIsUnsupported(model));
        saved.elements.u008[2] = 3; saved.nodes.u008[5] = 2.0;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 6u); checkGeometryScene(model, 2.0f);
        saved.elements.m = 0; saved.elements.u008[2] = std::numeric_limits<std::int32_t>::min();
        saved.nodes.u008[5] = 3.0;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 8u); checkGeometryScene(model, 3.0f);
        CHECK(saved.elements.u008[2] == std::numeric_limits<std::int32_t>::min());
        if (profile == 2) {
            for (std::size_t i = 1; i < trace.dispatches.size(); i += 2)
                CHECK(trace.dispatches[i].beforeNodeTimerCount == 3);
            CHECK(nodeTimer.count == 3); CHECK(arraysTimer.count == 8);
            CHECK(std::string(nodeTimer.labels[2]) == "Stop");
        } else { CHECK(nodeTimer.count == 7); CHECK(arraysTimer.count == 0); }
        CHECK(model.step.get() == currentStep); CHECK(step.step_version == 11000);
        CHECK(step.u26c4[0x0b] == cache); CHECK(step.db == nullptr);
        CHECK(saved.nodes.u138 == 2); CHECK(saved.elements.u0f8 == elementMetadata);
        CHECK(saved.nodes.u130 == (profile == 0 ? nullptr : &step));
        CHECK(saved.elements.u0f0 == (profile == 0 ? nullptr : reinterpret_cast<def::model::LLOBJ_DATA*>(&step)));
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        CHECK(saved.nodes.n == 4); CHECK(saved.elements.n == 1); CHECK(saved.elements.m == 0);
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_zero_width_keeps_layout_numeric_and_triple_count_guards_after_getters)
{
    for (const bool actualNullOwner : {false, true})
    for (int mode = 0; mode < 7; ++mode) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        struct RestoreCount {
            def::model::Cls_14017ef20& nodes;
            std::int32_t count;
            ~RestoreCount() { nodes.n = count; }
        } restoreCount{saved.nodes, saved.nodes.n};
        if (actualNullOwner) {
            saved.nodes.u130 = &step;
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        }
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = 0;
        saved.elements.u0f8 = 3; saved.elements.m = 0;
        switch (mode) {
        case 0: saved.nodes.u138 = 3; break;
        case 1: saved.elements.u0f8 = 0; break;
        case 2: saved.elements.m = -1; break;
        case 3: saved.nodes.u008[0] = std::numeric_limits<double>::quiet_NaN(); break;
        case 4: saved.nodes.u008[0] = std::numeric_limits<double>::infinity(); break;
        case 5: saved.nodes.u008[0] = std::numeric_limits<double>::max(); break;
        case 6: saved.nodes.n = std::numeric_limits<std::int32_t>::max() / 3 + 1; break;
        }
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 2u); CHECK(sceneBoxIsUnsupported(model));
        CHECK(nodeTimer.count == (actualNullOwner ? 3 : 7));
        CHECK(arraysTimer.count == (actualNullOwner ? 2 : 0));
        if (actualNullOwner) CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
        CHECK(step.step_version == 11000); CHECK(step.u26c4[0x0b] == 0);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        saved.nodes.n = restoreCount.count; saved.nodes.u138 = 2;
        saved.elements.u0f8 = 3; saved.elements.m = 0; saved.nodes.u008[0] = 0.0;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 4u); checkGeometryScene(model, 1.0f);
        CHECK(arraysTimer.count == (actualNullOwner ? 4 : 0));
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_zero_width_null_connectivity_still_skips_before_builder)
{
    for (int profile = 0; profile < 3; ++profile)
    for (const std::int32_t elementMetadata : {2, 3}) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        if (profile != 0) {
            saved.nodes.u130 = &step;
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        }
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = profile == 1 ? 1 : 0;
        saved.elements.u008 = nullptr; saved.elements.m = 0; saved.elements.u0f8 = elementMetadata;
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 1u); checkGeometryScene(model, 0.0f);
        CHECK(saved.elements.u008 == nullptr);
        CHECK(nodeTimer.count == (profile == 2 ? 3 : 7));
        CHECK(arraysTimer.count == (profile == 2 ? 1 : 0));
        saved.elements.u008 = saved.connectivity;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 3u); checkGeometryScene(model, 1.0f);
        CHECK(nodeTimer.count == (profile == 2 ? 3 : 7));
        CHECK(arraysTimer.count == (profile == 2 ? 3 : 0));
        if (profile == 2) CHECK(trace.dispatches[2].beforeNodeTimerCount == 3);
        CHECK(saved.elements.m == 0); CHECK(saved.elements.u0f8 == elementMetadata);
        CHECK(saved.nodes.u138 == 2); CHECK(step.step_version == 11000);
        CHECK(step.u26c4[0x0b] == (profile == 1 ? 1 : 0));
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(ownerless_geometry_connectivity_null_skips_mesh_and_preserves_edge_bounds)
{
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    REQUIRE(saved.nodes.u130 == nullptr); REQUIRE(saved.elements.u0f0 == nullptr);
    saved.elements.u008 = nullptr;
    engine::viewerRefresh(m);
    checkGeometryScene(m, 0.0f);
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == nullptr);
    CHECK(saved.nodes.u130 == nullptr); CHECK(saved.elements.u0f0 == nullptr);
    CHECK(saved.nodes.n == 4); CHECK(saved.elements.n == 1);
}

TEST_CASE(geometry_false_readiness_skips_builder_values_width_and_dimension_checks)
{
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    saved.elements.u008 = nullptr; saved.elements.m = 0;
    saved.nodes.u138 = 3; saved.elements.u0f8 = 3;
    saved.nodes.u008[0] = std::numeric_limits<double>::quiet_NaN();
    engine::viewerRefresh(m);
    checkGeometryScene(m, 0.0f);
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == nullptr);
}

TEST_CASE(geometry_connectivity_skip_keeps_missing_coordinates_and_nonnull_owners_guarded)
{
    for (int mode = 0; mode != 3; ++mode) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh);
        saved.elements.u008 = nullptr;
        if (mode == 0) saved.nodes.u008 = nullptr;
        // Opaque unqualified owner identities must never be dereferenced here.
        if (mode == 1) saved.nodes.u130 = m.object(1);
        if (mode == 2) saved.elements.u0f0 = m.object(1);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        CHECK(trace.dispatches.empty());
        CHECK(sceneBoxIsUnsupported(m));
        CHECK(saved.elements.u008 == nullptr);
    }
}

TEST_CASE(geometry_readiness_rejects_custom_null_id_zero_and_noncanonical_registry)
{
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    struct UnexpectedNodeVariable final : def::model::StateVarNode {
        int calls = 0;
        UnexpectedNodeVariable(std::int32_t id, void* slot) : StateVarNode(id, slot) {}
        void vslot2_140301690(std::int32_t, const std::int32_t*, std::int32_t) override { ++calls; }
    };
    struct RestoreRegistry {
        Variables& variables;
        def::model::StateVarBase* original;
        std::int32_t id;
        bool appended = false;
        ~RestoreRegistry()
        {
            variables.mutable_at(0) = original; original->id = id;
            if (appended) variables.mutable_items().pop_back();
        }
    };
    for (int mode = 0; mode != 4; ++mode) for (const bool populated : {false, true}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh);
        if (!populated) saved.elements.u008 = nullptr;
        auto& variables = *reinterpret_cast<Variables*>(&saved.nodes);
        REQUIRE(variables.size() == 28u);
        auto* original = variables.at(0); REQUIRE(original);
        UnexpectedNodeVariable unknown(original->id, &saved.nodes.u010[0]);
        RestoreRegistry restore{variables, original, original->id};
        if (mode == 0) original->id = 0;
        if (mode == 1) variables.mutable_at(0) = nullptr;
        if (mode == 2) variables.mutable_at(0) = &unknown;
        if (mode == 3) { variables.push_back(nullptr); restore.appended = true; }
        Variables lease(variables); const auto* oldBody = lease.body();
        REQUIRE(variables.use_count() == 2);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        CHECK(trace.dispatches.empty()); CHECK(variables.body() == oldBody);
        CHECK(lease.body() == oldBody); CHECK(variables.use_count() == 2); CHECK(lease.use_count() == 2);
        CHECK(sceneBoxIsUnsupported(m)); CHECK(unknown.calls == 0);
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
    }
}

TEST_CASE(geometry_false_readiness_preserves_coordinate_getter_collection_detach)
{
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    struct Snapshot {
        def::model::StateVarBase* variable;
        std::int32_t id;
        std::array<std::int32_t, 3> current, previous;
        const void* valuesBody;
    };
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh); saved.elements.u008 = nullptr;
    auto& variables = *reinterpret_cast<Variables*>(&saved.nodes);
    REQUIRE(variables.size() == 28u);
    std::array<Snapshot, 28> before{};
    for (std::size_t i = 0; i < before.size(); ++i) {
        auto* v = variables.at(i); REQUIRE(v);
        before[i] = {v, v->id, {v->cur[0], v->cur[1], v->cur[2]},
            {v->prev[0], v->prev[1], v->prev[2]}, v->values.body()};
    }
    // This handle owns list storage only; it does not delete StateVar pointees.
    Variables lease(variables); const auto* oldBody = lease.body();
    REQUIRE(variables.use_count() == 2); REQUIRE(variables.body() == oldBody);
    GeometryGetterTrace trace{&saved.nodes, {}};
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
    REQUIRE(trace.dispatches.size() == 1u); checkGeometryGetterArguments(trace);
    CHECK(trace.dispatches[0].before == oldBody);
    CHECK(trace.dispatches[0].after == variables.body());
    checkGeometryScene(m, 0.0f);
    CHECK(variables.body() != oldBody); CHECK(lease.body() == oldBody);
    CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1);
    for (std::size_t i = 0; i < before.size(); ++i) {
        auto* v = variables.at(i); REQUIRE(v);
        CHECK(v == before[i].variable); CHECK(lease.at(i) == v); CHECK(v->id == before[i].id);
        CHECK(v->values.body() == before[i].valuesBody);
        for (std::size_t k = 0; k < 3; ++k) {
            CHECK(v->cur[k] == before[i].current[k]); CHECK(v->prev[k] == before[i].previous[k]);
        }
    }
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == nullptr);
}

TEST_CASE(geometry_populated_ownerless_getters_detach_shared_collection_then_reuse_it)
{
    struct Snapshot {
        def::model::StateVarBase* variable;
        std::int32_t id;
        std::array<std::int32_t, 3> current, previous;
        const void* valuesBody;
    };
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    REQUIRE(saved.nodes.u130 == nullptr); REQUIRE(saved.elements.u0f0 == nullptr);
    auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
    REQUIRE(variables.size() == 28u);
    std::array<Snapshot, 28> before{};
    for (std::size_t i = 0; i < before.size(); ++i) {
        auto* v = variables.at(i); REQUIRE(v);
        before[i] = {v, v->id, {v->cur[0], v->cur[1], v->cur[2]},
            {v->prev[0], v->prev[1], v->prev[2]}, v->values.body()};
    }
    GeometryVariables lease(variables); const auto* oldBody = lease.body();
    REQUIRE(variables.use_count() == 2);
    GeometryGetterTrace trace{&saved.nodes, {}};
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
    REQUIRE(trace.dispatches.size() == 2u); checkGeometryGetterArguments(trace);
    CHECK(trace.dispatches[0].before == oldBody);
    CHECK(trace.dispatches[0].after != oldBody);
    CHECK(trace.dispatches[0].after == variables.body());
    CHECK(trace.dispatches[1].before == variables.body());
    CHECK(trace.dispatches[1].after == variables.body());
    CHECK(lease.body() == oldBody); CHECK(lease.use_count() == 1); CHECK(variables.use_count() == 1);
    for (std::size_t i = 0; i < before.size(); ++i) {
        auto* v = variables.at(i); REQUIRE(v);
        CHECK(v == before[i].variable); CHECK(lease.at(i) == v); CHECK(v->id == before[i].id);
        CHECK(v->values.body() == before[i].valuesBody);
        for (std::size_t k = 0; k < 3; ++k) {
            CHECK(v->cur[k] == before[i].current[k]); CHECK(v->prev[k] == before[i].previous[k]);
        }
    }
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
    CHECK(saved.nodes.u130 == nullptr); CHECK(saved.elements.u0f0 == nullptr);
    checkGeometryScene(m, 1.0f);
}

TEST_CASE(geometry_populated_ownerless_unique_collection_keeps_body_across_getter_rounds)
{
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
    REQUIRE(variables.use_count() == 1);
    const auto* oldBody = variables.body();
    GeometryGetterTrace trace{&saved.nodes, {}};
    for (int refresh = 0; refresh != 2; ++refresh) {
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        CHECK(trace.dispatches.size() == static_cast<std::size_t>(2 * (refresh + 1)));
        CHECK(variables.body() == oldBody); CHECK(variables.use_count() == 1);
        checkGeometryScene(m, 1.0f);
    }
    checkGeometryGetterArguments(trace);
    for (const auto& dispatch : trace.dispatches) {
        CHECK(dispatch.before == oldBody); CHECK(dispatch.after == oldBody);
    }
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
    CHECK(saved.nodes.u130 == nullptr); CHECK(saved.elements.u0f0 == nullptr);
}

TEST_CASE(geometry_mesh_populated_skip_populated_refresh_rebuilds_each_contribution)
{
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh);
    engine::viewerRefresh(m); checkGeometryScene(m, 1.0f);
    saved.elements.u008 = nullptr;
    engine::viewerRefresh(m); checkGeometryScene(m, 0.0f);
    saved.elements.u008 = saved.connectivity;
    engine::viewerRefresh(m); checkGeometryScene(m, 1.0f);
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
    CHECK(saved.nodes.u130 == nullptr); CHECK(saved.elements.u0f0 == nullptr);
}

TEST_CASE(geometry_actual_owner_fast_path_uses_actual_dimensions_and_preserves_cow)
{
    for (const std::uint8_t flag : {std::uint8_t{1}, std::uint8_t{0x80}}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        // Explicit observed fixture binding, never a production default/write.
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = flag;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        struct Snapshot {
            def::model::StateVarBase* variable;
            std::int32_t id;
            std::array<std::int32_t, 3> current, previous;
            const void* valuesBody;
        };
        REQUIRE(variables.size() == 28u);
        std::array<Snapshot, 28> before{};
        for (std::size_t i = 0; i < before.size(); ++i) {
            auto* v = variables.at(i); REQUIRE(v);
            before[i] = {v, v->id, {v->cur[0], v->cur[1], v->cur[2]},
                {v->prev[0], v->prev[1], v->prev[2]}, v->values.body()};
        }
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        REQUIRE(variables.use_count() == 2);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 2u);
        checkGeometryGetterArguments(trace, m.step->u15f8);
        CHECK(trace.dispatches[0].before == oldBody);
        CHECK(trace.dispatches[0].after != oldBody);
        const auto* uniqueBody = variables.body();
        CHECK(trace.dispatches[0].after == uniqueBody);
        CHECK(trace.dispatches[1].before == uniqueBody); CHECK(trace.dispatches[1].after == uniqueBody);
        CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
        checkGeometryScene(m, 1.0f);
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 4u);
        checkGeometryGetterArguments(trace, m.step->u15f8);
        for (std::size_t i = 2; i < 4; ++i) {
            CHECK(trace.dispatches[i].before == uniqueBody); CHECK(trace.dispatches[i].after == uniqueBody);
        }
        for (std::size_t i = 0; i < before.size(); ++i) {
            auto* v = variables.at(i); REQUIRE(v);
            CHECK(v == before[i].variable); CHECK(lease.at(i) == v); CHECK(v->id == before[i].id);
            CHECK(v->values.body() == before[i].valuesBody);
            for (std::size_t k = 0; k < 3; ++k) {
                CHECK(v->cur[k] == before[i].current[k]); CHECK(v->prev[k] == before[i].previous[k]);
            }
        }
        CHECK(variables.body() == uniqueBody); CHECK(variables.use_count() == 1);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        CHECK(saved.nodes.u130 == m.step.get()); CHECK(saved.elements.u0f0 == nullptr);
        CHECK(m.step->u1b58 == flag); checkGeometryScene(m, 1.0f);
    }
}

TEST_CASE(geometry_actual_owner_fast_path_connectivity_skip_uses_one_getter_round)
{
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
    saved.nodes.u130 = m.step.get(); m.step->u1b58 = 1; saved.elements.u008 = nullptr;
    auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
    GeometryVariables lease(variables); const auto* oldBody = lease.body();
    GeometryGetterTrace trace{&saved.nodes, {}};
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
    REQUIRE(trace.dispatches.size() == 1u); checkGeometryGetterArguments(trace, m.step->u15f8);
    CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
    CHECK(trace.dispatches[0].after == variables.body());
    CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1);
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == nullptr);
    CHECK(saved.nodes.u130 == m.step.get()); CHECK(saved.elements.u0f0 == nullptr);
    CHECK(m.step->u1b58 == 1); checkGeometryScene(m, 0.0f);
}

TEST_CASE(geometry_actual_owner_getter_admission_keeps_unqualified_profiles_guarded)
{
    RestoreGeometryIndexTables restoreTables;
    for (int mode = 0; mode != 10; ++mode) for (const bool populated : {false, true}) {
        RestoreGeometryTimers restoreTimers;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        engine::GuiModel other(nullptr, 3);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step); REQUIRE(other.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 1;
        if (!populated) saved.elements.u008 = nullptr;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        auto* first = variables.at(0); REQUIRE(first);
        struct RestoreId {
            def::model::StateVarBase* variable;
            std::int32_t id;
            ~RestoreId() { variable->id = id; }
        } restoreId{first, first->id};
        if (mode == 0) { m.step->u1b58 = 0; m.step->step_version = 6000; }
        if (mode == 1) saved.nodes.u008 = nullptr;
        if (mode == 2) saved.elements.u0f0 = m.object(1); // opaque, never read as a step
        if (mode == 3) saved.nodes.u130 = other.step.get(); // valid but not this view's step
        if (mode == 4) first->id = 0;
        if (mode == 5) saved.nodes.u130 = m.object(1); // opaque N owner must not be read
        if (mode >= 6 && mode <= 8)
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        if (mode == 6) { m.step->u1b58 = 0; m.step->u1b59 = 0; m.step->step_version = 6000; }
        if (mode == 7) saved.nodes.u008 = nullptr;
        if (mode == 8) first->id = 0;
        if (mode == 9)
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(other.step.get());
        const bool completedNullReader = mode == 0 || mode == 6;
        if (completedNullReader) {
            REQUIRE(m.step->db == nullptr); REQUIRE(m.step->u26c4[0x0b] == 0);
            auto* object = m.object(1); REQUIRE(object);
            REQUIRE(object->owner == m.step.get()); REQUIRE(object->u87c[0] == 0);
            REQUIRE(def::model::g_timer_read_arrays.count >= 0);
        }
        GeometryVariables lease(variables);
        const auto* oldBody = variables.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        if (mode == 0 || mode == 6) {
            REQUIRE(trace.dispatches.size() == (populated ? 2u : 1u));
            checkGeometryGetterArguments(trace, m.step->u15f8);
            CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
            CHECK(trace.dispatches[0].after == variables.body());
            CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1);
            CHECK(def::model::g_index_tables_dim == 3);
        } else {
            CHECK(trace.dispatches.empty()); CHECK(variables.body() == oldBody);
            CHECK(variables.use_count() == 2); CHECK(lease.use_count() == 2);
        }
        CHECK(lease.body() == oldBody);
        CHECK(m.step->u1b58 == (mode == 0 || mode == 6 ? 0 : 1));
        CHECK(m.step->u1b59 == (mode == 6 ? 0 : 1));
        CHECK(m.step->step_version == (mode == 0 || mode == 6 ? 6000 : 0));
        // Both qualified null-companion getter rounds now complete. Foreign
        //and opaque owner profiles remain guarded before unsafe reads.
        if (completedNullReader) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
    }
}

TEST_CASE(geometry_populated_unqualified_refusal_recovers_after_source_restoration)
{
    RestoreGeometryIndexTables restoreTables;
    for (int mode = 0; mode != 4; ++mode) {
        RestoreGeometryTimers restoreTimers;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        engine::GuiModel other(nullptr, 3);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step); REQUIRE(other.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 1;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        auto* first = variables.at(0); REQUIRE(first);
        struct RestoreId {
            def::model::StateVarBase* variable;
            std::int32_t id;
            ~RestoreId() { variable->id = id; }
        } restoreId{first, first->id};
        GeometryGetterTrace initial{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(initial); engine::viewerRefresh(m); }
        REQUIRE(initial.dispatches.size() == 2u);
        checkGeometryGetterArguments(initial, m.step->u15f8); checkGeometryScene(m, 1.0f);
        const auto* body = variables.body();
        const auto version = m.step->step_version;
        if (mode == 0) {
            m.step->u1b58 = 0; m.step->step_version = 6000;
            REQUIRE(m.step->db == nullptr); REQUIRE(m.step->u26c4[0x0b] == 0);
            auto* object = m.object(1); REQUIRE(object);
            REQUIRE(object->owner == m.step.get()); REQUIRE(object->u87c[0] == 0);
            REQUIRE(def::model::g_timer_read_arrays.count >= 0);
        }
        if (mode == 1) saved.elements.u0f0 = m.object(1);
        if (mode == 2) saved.nodes.u130 = other.step.get();
        if (mode == 3) first->id = 0;
        auto beforeArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
        GeometryGetterTrace refused{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(refused); engine::viewerRefresh(m); }
        if (mode == 0) {
            // This high/null profile now returns through both GUI getter rounds.
            REQUIRE(refused.dispatches.size() == 2u); checkGeometryGetterArguments(refused, m.step->u15f8);
            CHECK(def::model::g_index_tables_dim == 3);
            CHECK(refused.dispatches[1].beforeNodeTimerCount == 3);
            CHECK(def::model::g_timer_read_obj_node_state_variable.count == 3);
            checkGeometryArraysTimerAppend(*beforeArrays, 2);
            checkGeometryScene(m, 1.0f);
        } else {
            CHECK(refused.dispatches.empty()); CHECK(sceneBoxIsUnsupported(m));
            restoreTimers.checkUnchanged();
        }
        CHECK(variables.body() == body);
        CHECK(m.step->step_version == (mode == 0 ? 6000 : version));
        m.step->u1b58 = 1; saved.elements.u0f0 = nullptr;
        saved.nodes.u130 = m.step.get(); first->id = restoreId.id; m.step->step_version = version;
        GeometryGetterTrace restored{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(restored); engine::viewerRefresh(m); }
        REQUIRE(restored.dispatches.size() == 2u);
        checkGeometryGetterArguments(restored, m.step->u15f8); checkGeometryScene(m, 1.0f);
        CHECK(variables.body() == body); CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == saved.connectivity); CHECK(saved.nodes.u130 == m.step.get());
        CHECK(saved.elements.u0f0 == nullptr); CHECK(m.step->u1b58 == 1);
        CHECK(m.step->step_version == version);
    }
}

TEST_CASE(geometry_empty_count_skips_unqualified_getter_profiles_before_registry_and_owner_reads)
{
    for (const bool emptyNodes : {false, true}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh);
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        auto* first = variables.at(0); REQUIRE(first);
        struct RestoreId {
            def::model::StateVarBase* variable;
            std::int32_t id;
            ~RestoreId() { variable->id = id; }
        } restoreId{first, first->id};
        first->id = 0;
        saved.nodes.u130 = m.object(1); saved.elements.u0f0 = m.object(1);
        if (emptyNodes) saved.nodes.n = 0; else saved.elements.n = 0;
        GeometryVariables lease(variables); const auto* body = variables.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        CHECK(trace.dispatches.empty()); CHECK(variables.body() == body);
        CHECK(variables.use_count() == 2); CHECK(lease.body() == body);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        CHECK(saved.nodes.u130 == m.object(1)); CHECK(saved.elements.u0f0 == m.object(1));
        checkGeometryScene(m, 0.0f);
    }
}

TEST_CASE(geometry_actual_element_owner_all_byte_values_preserve_getter_cow_and_bounds)
{
    for (const std::uint8_t flag : {std::uint8_t{0}, std::uint8_t{1}, std::uint8_t{0x80}})
    for (const bool actualNodeOwner : {false, true})
    for (const bool populated : {false, true}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        saved.nodes.u130 = actualNodeOwner ? m.step.get() : nullptr;
        m.step->u1b58 = actualNodeOwner ? 1 : 0; m.step->u1b59 = flag;
        if (!populated) saved.elements.u008 = nullptr;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        REQUIRE(variables.use_count() == 2);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == (populated ? 2u : 1u));
        checkGeometryGetterArguments(trace, actualNodeOwner ? m.step->u15f8 : nullptr);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        const auto* uniqueBody = variables.body();
        CHECK(trace.dispatches[0].after == uniqueBody);
        if (populated) {
            CHECK(trace.dispatches[1].before == uniqueBody); CHECK(trace.dispatches[1].after == uniqueBody);
        }
        CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
        for (std::size_t i = 0; i < variables.size(); ++i) {
            auto* variable = variables.at(i); REQUIRE(variable);
            CHECK(variable == lease.at(i));
            CHECK(variable->id == def::model::Cls_14017ef20::kRegs[i].id);
        }
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
        CHECK(saved.nodes.u130 == (actualNodeOwner ? m.step.get() : nullptr));
        CHECK(static_cast<const void*>(saved.elements.u0f0) == m.step.get());
        CHECK(m.step->u1b58 == (actualNodeOwner ? 1 : 0)); CHECK(m.step->u1b59 == flag);
        checkGeometryScene(m, populated ? 1.0f : 0.0f);
    }
}

TEST_CASE(geometry_actual_element_owner_restoration_recovers_qualified_populated_result)
{
    for (const bool opaque : {false, true}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        engine::GuiModel other(nullptr, 3);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step); REQUIRE(other.step);
        m.step->u1b58 = 0; m.step->u1b59 = 0; // no node owner, qualified E gate for zero
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        GeometryGetterTrace initial{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(initial); engine::viewerRefresh(m); }
        REQUIRE(initial.dispatches.size() == 2u); checkGeometryGetterArguments(initial);
        checkGeometryScene(m, 1.0f);
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        const auto* body = variables.body();
        saved.elements.u0f0 = opaque ? m.object(1)
            : reinterpret_cast<def::model::LLOBJ_DATA*>(other.step.get());
        GeometryGetterTrace refused{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(refused); engine::viewerRefresh(m); }
        CHECK(refused.dispatches.empty()); CHECK(variables.body() == body);
        CHECK(sceneBoxIsUnsupported(m));
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        GeometryGetterTrace restored{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(restored); engine::viewerRefresh(m); }
        REQUIRE(restored.dispatches.size() == 2u); checkGeometryGetterArguments(restored);
        checkGeometryScene(m, 1.0f);
        CHECK(variables.body() == body); CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == saved.connectivity); CHECK(saved.nodes.u130 == nullptr);
        CHECK(static_cast<const void*>(saved.elements.u0f0) == m.step.get());
        CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0);
    }
}

TEST_CASE(geometry_actual_node_zero_flag_signed_version_gate_preserves_getter_cow)
{
    RestoreGeometryIndexTables restoreTables;
    const std::array<std::int32_t, 7> versions{
        std::numeric_limits<std::int32_t>::min(), -1, 0, 5999, 6000, 6001,
        std::numeric_limits<std::int32_t>::max()};
    for (const auto version : versions) for (const bool actualElementOwner : {false, true})
    for (const bool populated : {false, true}) {
        RestoreGeometryTimers restoreTimers;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = version;
        m.step->u1b59 = 0;
        if (actualElementOwner)
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        if (!populated) saved.elements.u008 = nullptr;
        if (version >= 6000) {
            REQUIRE(m.step->db == nullptr); REQUIRE(m.step->u26c4[0x0b] == 0);
            auto* object = m.object(1); REQUIRE(object);
            REQUIRE(object->owner == m.step.get()); REQUIRE(object->u87c[0] == 0);
            REQUIRE(def::model::g_timer_read_arrays.count >= 0);
        }
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        struct Snapshot {
            std::int32_t id;
            std::array<std::int32_t, 3> current, previous;
            const void* valuesBody;
        };
        REQUIRE(variables.size() == 28u);
        std::array<Snapshot, 28> before{};
        for (std::size_t i = 0; i < before.size(); ++i) {
            auto* variable = variables.at(i); REQUIRE(variable);
            before[i] = {variable->id, {variable->cur[0], variable->cur[1], variable->cur[2]},
                {variable->prev[0], variable->prev[1], variable->prev[2]}, variable->values.body()};
        }
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        REQUIRE(variables.use_count() == 2);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        if (version < 6000) {
            REQUIRE(trace.dispatches.size() == (populated ? 2u : 1u));
            checkGeometryGetterArguments(trace, m.step->u15f8);
            CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
            CHECK(trace.dispatches[0].after == variables.body());
            if (populated) {
                CHECK(trace.dispatches[1].before == variables.body());
                CHECK(trace.dispatches[1].after == variables.body());
            }
            CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1);
            checkGeometryScene(m, populated ? 1.0f : 0.0f);
        } else {
            REQUIRE(trace.dispatches.size() == (populated ? 2u : 1u));
            checkGeometryGetterArguments(trace, m.step->u15f8);
            CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
            CHECK(trace.dispatches[0].after == variables.body());
            CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1);
            CHECK(def::model::g_index_tables_dim == 3);
            if (populated) {
                CHECK(trace.dispatches[1].before == variables.body());
                CHECK(trace.dispatches[1].after == variables.body());
                CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
            }
            checkGeometryScene(m, populated ? 1.0f : 0.0f);
        }
        CHECK(lease.body() == oldBody);
        for (std::size_t i = 0; i < before.size(); ++i) {
            auto* variable = variables.at(i); REQUIRE(variable);
            CHECK(variable == lease.at(i)); CHECK(variable->id == before[i].id);
            CHECK(variable->values.body() == before[i].valuesBody);
            for (std::size_t k = 0; k < 3; ++k) {
                CHECK(variable->cur[k] == before[i].current[k]);
                CHECK(variable->prev[k] == before[i].previous[k]);
            }
        }
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
        CHECK(saved.nodes.u130 == m.step.get());
        CHECK(static_cast<const void*>(saved.elements.u0f0) == (actualElementOwner ? m.step.get() : nullptr));
        CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0); CHECK(m.step->step_version == version);
    }
}

TEST_CASE(geometry_actual_node_nonzero_flag_bypasses_version_rejection)
{
    for (const auto flag : {std::uint8_t{1}, std::uint8_t{0x80}, std::uint8_t{0xff}})
    for (const auto version : {std::int32_t{6000}, std::numeric_limits<std::int32_t>::max()}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = flag; m.step->step_version = version;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        m.step->u1b59 = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        const auto* body = variables.body(); REQUIRE(variables.use_count() == 1);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 2u); checkGeometryGetterArguments(trace, m.step->u15f8);
        CHECK(variables.body() == body); checkGeometryScene(m, 1.0f);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        CHECK(saved.nodes.u130 == m.step.get()); CHECK(static_cast<const void*>(saved.elements.u0f0) == m.step.get());
        CHECK(m.step->u1b58 == flag); CHECK(m.step->u1b59 == 0); CHECK(m.step->step_version == version);
    }
}

TEST_CASE(geometry_actual_node_old_version_keeps_missing_coordinate_and_id_zero_guards)
{
    for (const bool missingCoordinates : {false, true}) {
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 0;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        m.step->u1b59 = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        auto* first = variables.at(0); REQUIRE(first);
        struct RestoreId {
            def::model::StateVarBase* variable;
            std::int32_t id;
            ~RestoreId() { variable->id = id; }
        } restoreId{first, first->id};
        if (missingCoordinates) saved.nodes.u008 = nullptr; else first->id = 0;
        GeometryVariables lease(variables); const auto* body = variables.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        CHECK(trace.dispatches.empty()); CHECK(variables.body() == body);
        CHECK(lease.body() == body); CHECK(variables.use_count() == 2); CHECK(lease.use_count() == 2);
        CHECK(sceneBoxIsUnsupported(m)); CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0);
        CHECK(m.step->step_version == 0); CHECK(saved.elements.u008 == saved.connectivity);
    }
}

TEST_CASE(geometry_high_version_prefix_commits_cow_then_actual_tables_before_cache_decision)
{
    // Final slots decoded from native GUI140820320/140820a60, with complete
    // body/value hashes retained in resume69-geometry-global-tables.md.
    const std::array<std::int32_t, 53> nativeMar{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 17, 18,
        19, 20, 21, 22, 69, 24, 25, 57, 70, 71, 72, 73, 74, 75, 76, 77,
        78, 79, 80, 81, 84, 85, 87, 88, 89, 90, 91, 92, 131, 132, 133, 134,
        135, 136, 94, 95, 127};
    const std::array<std::int32_t, 129> nativeArrays{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16,
        17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
        34, 35, 37, 38, 40, 41, 42, 43, 44, 45, 46, 60, 48, 49, 50, 51,
        52, 53, 54, 55, 47, 56, 57, 58, 59, 61, 139, 140, 141, 142, 143, 144,
        145, 146, 147, 148, 149, 150, 151, 152, 153, 155, 156, 157, 158, 160, 161, 162,
        163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 177, 178, 180,
        181, 182, 183, 184, 185, 186, 200, 188, 189, 190, 191, 192, 193, 194, 195, 266,
        267, 268, 269, 270, 271, 274, 275, 276, 277, 278, 187, 196, 197, 198, 199, 201,
        280};
    for (const auto initialDimension : {0, 2, 3}) for (const bool cached : {false, true})
    for (const bool populated : {false, true}) {
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 6000;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        m.step->u1b59 = 0; m.step->u26c4[0x0b] = cached ? 1 : 0;
        if (!populated) saved.elements.u008 = nullptr;
        REQUIRE(m.step->u1b88 == nullptr); REQUIRE(m.step->u1b78 == nullptr);
        def::model::g_index_tables_dim = initialDimension;
        for (std::size_t i = 0; i < nativeMar.size(); ++i)
            def::model::g_mar_index[i] = -701 - static_cast<std::int32_t>(i);
        for (std::size_t i = 0; i < nativeArrays.size(); ++i)
            def::model::g_a_index[i] = -901 - static_cast<std::int32_t>(i);
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        REQUIRE(variables.use_count() == 2);
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        const bool knownCached = cached && initialDimension != 3;
        const bool knownNullReturn = !cached && initialDimension != 3 &&
            def::model::g_timer_read_arrays.count >= 0;
        const std::size_t rounds = (knownCached || knownNullReturn) && populated ? 2u : 1u;
        REQUIRE(trace.dispatches.size() == rounds); checkGeometryGetterArguments(trace, m.step->u15f8);
        const auto& dispatch = trace.dispatches[0];
        CHECK(dispatch.before == oldBody); CHECK(dispatch.after != oldBody);
        CHECK(dispatch.after == variables.body());
        CHECK(dispatch.beforeIndexDimension == initialDimension);
        CHECK(dispatch.afterIndexDimension == initialDimension);
        CHECK(dispatch.beforeMar0 == -701); CHECK(dispatch.afterMar0 == -701);
        CHECK(dispatch.beforeArray0 == -901); CHECK(dispatch.afterArray0 == -901);
        CHECK(def::model::g_index_tables_dim == 3);
        for (std::size_t i = 0; i < nativeMar.size(); ++i)
            CHECK(def::model::g_mar_index[i] == (initialDimension == 3
                ? -701 - static_cast<std::int32_t>(i) : nativeMar[i]));
        for (std::size_t i = 0; i < nativeArrays.size(); ++i)
            CHECK(def::model::g_a_index[i] == (initialDimension == 3
                ? -901 - static_cast<std::int32_t>(i) : nativeArrays[i]));
        if (knownCached || knownNullReturn) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        const auto* uniqueBody = variables.body();
        CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
        // A fresh public refresh is another bounded invocation, not resume of
        // an invented native reader frame. Current3 must preserve table edits.
        def::model::g_mar_index[4] = 123456; def::model::g_a_index[17] = -456;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 2u * rounds); checkGeometryGetterArguments(trace, m.step->u15f8);
        for (std::size_t i = rounds; i < trace.dispatches.size(); ++i) {
            CHECK(trace.dispatches[i].before == uniqueBody); CHECK(trace.dispatches[i].after == uniqueBody);
            CHECK(trace.dispatches[i].beforeIndexDimension == 3); CHECK(trace.dispatches[i].afterIndexDimension == 3);
        }
        CHECK(def::model::g_mar_index[4] == 123456); CHECK(def::model::g_a_index[17] == -456);
        if (knownCached || knownNullReturn) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
        CHECK(saved.nodes.u130 == m.step.get()); CHECK(static_cast<const void*>(saved.elements.u0f0) == m.step.get());
        CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0); CHECK(m.step->step_version == 6000);
        CHECK(m.step->u26c4[0x0b] == (cached ? 1 : 0));
        CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
    }
}

TEST_CASE(geometry_high_version_cached_reader_returns_normally_with_null_mar_and_array_bodies)
{
    for (const auto version : {std::int32_t{6000}, std::int32_t{11000},
        std::numeric_limits<std::int32_t>::max()})
    for (const auto cached : {std::int32_t{1}, std::int32_t{-1}})
    for (const bool actualElementOwner : {false, true}) for (const bool populated : {false, true}) {
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = version;
        m.step->u1b59 = 0; m.step->u26c4[0x0b] = cached;
        m.step->u1b90[3] = cached > 0 ? std::numeric_limits<std::int32_t>::min()
            : std::numeric_limits<std::int32_t>::max();
        const auto marStart = m.step->u1b90[3];
        if (actualElementOwner)
            saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        if (!populated) saved.elements.u008 = nullptr;
        REQUIRE(m.step->u1b88 == nullptr); REQUIRE(m.step->u1b78 == nullptr);
        def::model::g_index_tables_dim = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == (populated ? 2u : 1u));
        checkGeometryGetterArguments(trace, m.step->u15f8);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        CHECK(trace.dispatches[0].after == variables.body());
        CHECK(trace.dispatches[0].beforeIndexDimension == 0); CHECK(trace.dispatches[0].afterIndexDimension == 0);
        if (populated) {
            CHECK(trace.dispatches[1].before == variables.body()); CHECK(trace.dispatches[1].after == variables.body());
            CHECK(trace.dispatches[1].beforeIndexDimension == 3); CHECK(trace.dispatches[1].afterIndexDimension == 3);
        }
        CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
        CHECK(def::model::g_index_tables_dim == 3); checkGeometryScene(m, populated ? 1.0f : 0.0f);
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
        CHECK(saved.nodes.u130 == m.step.get());
        CHECK(static_cast<const void*>(saved.elements.u0f0) == (actualElementOwner ? m.step.get() : nullptr));
        CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0); CHECK(m.step->step_version == version);
        CHECK(m.step->u26c4[0x0b] == cached); CHECK(m.step->u1b90[3] == marStart);
        CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
    }
}

TEST_CASE(geometry_high_version_cached_reader_bounds_and_miss_keep_real_prefix_guarded)
{
    for (int mode = 0; mode != 7; ++mode) {
        RestoreGeometryTimers restoreTimers;
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 11000;
        m.step->u26c4[0x0b] = mode == 6 ? 0 : 1;
        def::model::g_index_tables_dim = 0;
        def::model::select_index_tables_3d();
        if (mode == 0) def::model::g_mar_index[3] = -1;
        if (mode == 1) def::model::g_mar_index[3] = 140;
        if (mode == 2) def::model::g_mar_index[3] = std::numeric_limits<std::int32_t>::max();
        if (mode == 3) def::model::g_a_index[128] = 11;
        if (mode == 4) def::model::g_a_index[128] = 0;
        if (mode == 5) def::model::g_a_index[128] = -1;
        if (mode == 6) {
            REQUIRE(m.step->db == nullptr); REQUIRE(m.object(1)->owner == m.step.get());
            REQUIRE(m.object(1)->u87c[0] == 0); REQUIRE(def::model::g_timer_read_arrays.count >= 0);
        }
        const auto marIndex = def::model::g_mar_index[3];
        const auto idLimit = def::model::g_a_index[128];
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == (mode == 6 ? 2u : 1u));
        checkGeometryGetterArguments(trace, m.step->u15f8);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        CHECK(trace.dispatches[0].after == variables.body()); CHECK(variables.use_count() == 1);
        CHECK(lease.body() == oldBody); CHECK(lease.use_count() == 1);
        CHECK(def::model::g_index_tables_dim == 3); CHECK(def::model::g_mar_index[3] == marIndex);
        CHECK(def::model::g_a_index[128] == idLimit);
        if (mode == 6) checkGeometryScene(m, 1.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        CHECK(m.step->u26c4[0x0b] == (mode == 6 ? 0 : 1));
        CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
    }
}

TEST_CASE(geometry_high_version_cached_reader_uses_valid_altered_actual_header_indices)
{
    for (const auto marIndex : {0, 139}) {
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 11000;
        m.step->u26c4[0x0b] = -1; m.step->u1b90[marIndex] = std::numeric_limits<std::int32_t>::min();
        def::model::g_index_tables_dim = 0;
        def::model::select_index_tables_3d();
        def::model::g_mar_index[3] = marIndex; def::model::g_a_index[128] = 12;
        def::model::g_mar_index[4] = -99; def::model::g_a_index[17] = -98;
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 2u); checkGeometryGetterArguments(trace, m.step->u15f8);
        checkGeometryScene(m, 1.0f); CHECK(def::model::g_index_tables_dim == 3);
        CHECK(def::model::g_mar_index[3] == marIndex); CHECK(def::model::g_a_index[128] == 12);
        CHECK(def::model::g_mar_index[4] == -99); CHECK(def::model::g_a_index[17] == -98);
        CHECK(m.step->u1b90[marIndex] == std::numeric_limits<std::int32_t>::min());
        CHECK(m.step->u26c4[0x0b] == -1); CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
    }
}

TEST_CASE(geometry_high_version_cached_miss_cached_transition_preserves_each_actual_result)
{
    RestoreGeometryIndexTables restoreTables;
    engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
    RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
    saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 11000;
    saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
    m.step->u1b59 = 0; def::model::g_index_tables_dim = 0;
    auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
    const auto* body = variables.body(); REQUIRE(variables.use_count() == 1);
    GeometryGetterTrace trace{&saved.nodes, {}};
    m.step->u26c4[0x0b] = 1;
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
    REQUIRE(trace.dispatches.size() == 2u); checkGeometryScene(m, 1.0f);
    m.step->u26c4[0x0b] = 0;
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
    REQUIRE(trace.dispatches.size() == 4u); checkGeometryScene(m, 1.0f);
    m.step->u26c4[0x0b] = -1;
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
    REQUIRE(trace.dispatches.size() == 6u); checkGeometryGetterArguments(trace, m.step->u15f8);
    checkGeometryScene(m, 1.0f); CHECK(variables.body() == body); CHECK(variables.use_count() == 1);
    CHECK(m.step->u26c4[0x0b] == -1); CHECK(m.step->u1b58 == 0); CHECK(m.step->step_version == 11000);
    CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
    CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
}

TEST_CASE(geometry_cache_miss_records_actual_node_timer_prefix_before_db_loader_guard)
{
    for (const bool opaqueDb : {false, true}) for (const bool populated : {false, true}) {
        RestoreGeometryTimers restoreTimers;
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        struct RestoreDb {
            def::model::DB_STEP_DATA& step;
            def::model::Cls_1401dbfe0* db;
            ~RestoreDb() { step.db = db; }
        } restoreDb{*m.step, m.step->db};
        if (opaqueDb) m.step->db = reinterpret_cast<def::model::Cls_1401dbfe0*>(m.object(1));
        else m.step->db = nullptr;
        const auto* db = m.step->db;
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 11000;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        m.step->u1b59 = 0; m.step->u26c4[0x0b] = 0;
        if (!populated) saved.elements.u008 = nullptr;
        const auto marLength = m.step->u1b68, arrayLength = m.step->u1b6c;
        const auto bodyCount = m.step->u2b24, storedLength = m.step->u2b28[0x0b];
        REQUIRE(m.step->u1b88 == nullptr); REQUIRE(m.step->u1b78 == nullptr);
        def::model::g_index_tables_dim = 0;
        auto& timer = def::model::g_timer_read_obj_node_state_variable;
        int marker = 0;
        timer.count = 7;
        for (std::size_t i = 0; i < 100; ++i) {
            std::memset(timer.labels[i], 'Z', sizeof timer.labels[i]);
            std::snprintf(timer.labels[i], sizeof timer.labels[i], "kept-%zu", i);
            timer.times[i] = static_cast<std::int32_t>(900 + i); timer.subs[i] = &marker;
        }
        auto before = std::make_unique<def::model::StepTimer>(timer);
        auto beforeArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
        REQUIRE(beforeArrays->count >= 0);
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        const std::size_t rounds = !opaqueDb && populated ? 2u : 1u;
        auto checkTimerPrefix = [&](int invocations) {
            const std::size_t checkpoints = opaqueDb ? 2u : 3u;
            CHECK(timer.count == static_cast<std::int32_t>(checkpoints));
            CHECK(std::string(timer.labels[0]) == "Start");
            CHECK(std::string(timer.labels[1]) == "ReadSingleAArrayFromDB");
            CHECK(timer.subs[0] == nullptr); CHECK(timer.subs[1] == nullptr);
            if (!opaqueDb) { CHECK(std::string(timer.labels[2]) == "Stop"); CHECK(timer.subs[2] == nullptr); }
            CHECK(std::memcmp(timer.name, before->name, sizeof timer.name) == 0);
            CHECK(timer.flag == before->flag);
            CHECK(std::memcmp(timer.pad_539d, before->pad_539d, sizeof timer.pad_539d) == 0);
            for (std::size_t i = 0; i < checkpoints; ++i) {
                const std::size_t written = std::strlen(timer.labels[i]) + 1;
                CHECK(std::memcmp(timer.labels[i] + written, before->labels[i] + written,
                    sizeof timer.labels[i] - written) == 0);
            }
            for (std::size_t i = checkpoints; i < 100; ++i) {
                CHECK(std::memcmp(timer.labels[i], before->labels[i], sizeof timer.labels[i]) == 0);
                CHECK(timer.times[i] == before->times[i]); CHECK(timer.subs[i] == before->subs[i]);
            }
            // Reached timestamps are opaque actual clock samples, not fixed or
            // monotonic test inputs. The existing checkpoint owns those writes.
            restoreTimers.checkUnchanged(false, opaqueDb);
            if (!opaqueDb) checkGeometryArraysTimerAppend(*beforeArrays, invocations);
        };
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == rounds); checkGeometryGetterArguments(trace, m.step->u15f8);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        CHECK(trace.dispatches[0].after == variables.body());
        CHECK(trace.dispatches[0].beforeIndexDimension == 0); CHECK(trace.dispatches[0].afterIndexDimension == 0);
        CHECK(trace.dispatches[0].beforeNodeTimerCount == 7); CHECK(trace.dispatches[0].afterNodeTimerCount == 7);
        CHECK(def::model::g_index_tables_dim == 3); checkTimerPrefix(static_cast<int>(rounds));
        if (rounds == 2u) {
            CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
            CHECK(trace.dispatches[1].before == variables.body()); CHECK(trace.dispatches[1].after == variables.body());
        }
        if (!opaqueDb) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        const auto* uniqueBody = variables.body();
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 2u * rounds);
        for (std::size_t i = rounds; i < trace.dispatches.size(); ++i) {
            CHECK(trace.dispatches[i].before == uniqueBody); CHECK(trace.dispatches[i].after == uniqueBody);
            CHECK(trace.dispatches[i].beforeNodeTimerCount == (opaqueDb ? 2 : 3));
            CHECK(trace.dispatches[i].afterNodeTimerCount == (opaqueDb ? 2 : 3));
            CHECK(trace.dispatches[i].beforeIndexDimension == 3); CHECK(trace.dispatches[i].afterIndexDimension == 3);
        }
        checkTimerPrefix(static_cast<int>(2u * rounds));
        if (!opaqueDb) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
        CHECK(m.step->db == db); CHECK(m.step->step_version == 11000); CHECK(m.step->u26c4[0x0b] == 0);
        CHECK(m.step->u1b68 == marLength); CHECK(m.step->u1b6c == arrayLength);
        CHECK(m.step->u2b24 == bodyCount); CHECK(m.step->u2b28[0x0b] == storedLength);
        CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
        CHECK(saved.nodes.u130 == m.step.get()); CHECK(static_cast<const void*>(saved.elements.u0f0) == m.step.get());
        CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0);
    }
}

TEST_CASE(geometry_nonmiss_and_unqualified_paths_do_not_mutate_reader_timers)
{
    for (int mode = 0; mode != 11; ++mode) {
        RestoreGeometryTimers restoreTimers;
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = 11000;
        m.step->u26c4[0x0b] = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        auto* first = variables.at(0); REQUIRE(first);
        struct RestoreId {
            def::model::StateVarBase* variable;
            std::int32_t id;
            ~RestoreId() { variable->id = id; }
        } restoreId{first, first->id};
        def::model::g_index_tables_dim = 0; def::model::select_index_tables_3d();
        if (mode == 0) m.step->u26c4[0x0b] = 1;
        if (mode == 1) m.step->u26c4[0x0b] = -1;
        if (mode == 2) def::model::g_mar_index[3] = -1;
        if (mode == 3) def::model::g_a_index[128] = 11;
        if (mode == 4) saved.nodes.u130 = m.object(1);
        if (mode == 5) saved.elements.u0f0 = m.object(1);
        if (mode == 6) first->id = 0;
        if (mode == 7) saved.nodes.u008 = nullptr;
        if (mode == 8) saved.nodes.n = 0;
        if (mode == 9) m.step->step_version = 5999;
        if (mode == 10) m.step->u1b58 = 1;
        GeometryGetterTrace trace{&saved.nodes, {}};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        restoreTimers.checkUnchanged();
        if (mode == 0 || mode == 1 || mode == 9 || mode == 10) {
            REQUIRE(trace.dispatches.size() == 2u); checkGeometryScene(m, 1.0f);
        } else if (mode == 8) {
            CHECK(trace.dispatches.empty()); checkGeometryScene(m, 0.0f);
        } else {
            CHECK(sceneBoxIsUnsupported(m));
            CHECK(trace.dispatches.size() == (mode == 2 || mode == 3 ? 1u : 0u));
        }
    }
}

TEST_CASE(geometry_null_companion_completes_exact_readiness_and_builder_getter_rounds)
{
    for (const auto currentVersion : {std::int32_t{6000}, std::int32_t{10999}, std::int32_t{11000},
        std::numeric_limits<std::int32_t>::max()})
    for (const auto initialCount : {std::int32_t{-1}, std::int32_t{0}, std::int32_t{98}, std::int32_t{99},
        std::int32_t{100}, std::int32_t{101}, std::numeric_limits<std::int32_t>::max()})
    for (const bool populated : {false, true}) {
        RestoreGeometryTimers restoreTimers;
        RestoreGeometryIndexTables restoreTables;
        engine::GuiModel m(nullptr, 3); auto& mesh = geometryQuad(m);
        RestoreGeometryMeshFields saved(mesh); REQUIRE(m.step); REQUIRE(m.step->db == nullptr);
        saved.nodes.u130 = m.step.get(); m.step->u1b58 = 0; m.step->step_version = currentVersion;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(m.step.get());
        m.step->u1b59 = 0; m.step->u26c4[0x0b] = 0;
        if (!populated) saved.elements.u008 = nullptr;
        const auto version = m.step->step_version, objectCount = m.step->u1378;
        const auto marLength = m.step->u1b68, arrayLength = m.step->u1b6c;
        const auto bodyCount = m.step->u2b24, storedLength = m.step->u2b28[0x0b];
        REQUIRE(m.step->u1b88 == nullptr); REQUIRE(m.step->u1b78 == nullptr);
        auto& arraysTimer = def::model::g_timer_read_arrays;
        arraysTimer.count = initialCount;
        int marker = 0;
        for (std::size_t i = 0; i < 100; ++i) {
            std::memset(arraysTimer.labels[i], 'Y', sizeof arraysTimer.labels[i]);
            std::snprintf(arraysTimer.labels[i], sizeof arraysTimer.labels[i], "arrays-kept-%zu", i);
            arraysTimer.times[i] = static_cast<std::int32_t>(1200 + i); arraysTimer.subs[i] = &marker;
        }
        auto before = std::make_unique<def::model::StepTimer>(arraysTimer);
        def::model::g_timer_read_obj_node_state_variable.count = 7;
        def::model::g_index_tables_dim = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        GeometryGetterTrace trace{&saved.nodes, {}};
        const std::size_t rounds = populated && initialCount >= 0 ? 2u : 1u;
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == rounds); checkGeometryGetterArguments(trace, m.step->u15f8);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        CHECK(trace.dispatches[0].after == variables.body());
        CHECK(trace.dispatches[0].beforeIndexDimension == 0); CHECK(trace.dispatches[0].afterIndexDimension == 0);
        CHECK(trace.dispatches[0].beforeNodeTimerCount == 7); CHECK(trace.dispatches[0].afterNodeTimerCount == 7);
        CHECK(def::model::g_index_tables_dim == 3);
        const auto checkpoints = initialCount < 0 ? 2 : 3;
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == checkpoints);
        if (initialCount >= 0) {
            CHECK(std::string(def::model::g_timer_read_obj_node_state_variable.labels[2]) == "Stop");
            CHECK(def::model::g_timer_read_obj_node_state_variable.subs[2] == nullptr);
        }
        if (initialCount < 0) CHECK(std::memcmp(&arraysTimer, before.get(), sizeof arraysTimer) == 0);
        else checkGeometryArraysTimerAppend(*before, static_cast<int>(rounds));
        if (rounds == 2u) {
            CHECK(trace.dispatches[1].beforeNodeTimerCount == 3);
            CHECK(trace.dispatches[1].before == variables.body()); CHECK(trace.dispatches[1].after == variables.body());
        }
        if (initialCount >= 0) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        restoreTimers.checkUnchanged(false, false);
        auto afterFirst = std::make_unique<def::model::StepTimer>(arraysTimer);
        const auto* uniqueBody = variables.body();
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(m); }
        REQUIRE(trace.dispatches.size() == 2u * rounds);
        for (std::size_t i = rounds; i < trace.dispatches.size(); ++i) {
            CHECK(trace.dispatches[i].before == uniqueBody); CHECK(trace.dispatches[i].after == uniqueBody);
            CHECK(trace.dispatches[i].beforeIndexDimension == 3); CHECK(trace.dispatches[i].afterIndexDimension == 3);
            CHECK(trace.dispatches[i].beforeNodeTimerCount == checkpoints);
            CHECK(trace.dispatches[i].afterNodeTimerCount == checkpoints);
        }
        CHECK(def::model::g_timer_read_obj_node_state_variable.count == checkpoints);
        if (initialCount < 0) CHECK(std::memcmp(&arraysTimer, before.get(), sizeof arraysTimer) == 0);
        else {
            checkGeometryArraysTimerAppend(*before, static_cast<int>(2u * rounds));
            checkGeometryArraysTimerAppend(*afterFirst, static_cast<int>(rounds));
        }
        if (initialCount >= 0) checkGeometryScene(m, populated ? 1.0f : 0.0f);
        else CHECK(sceneBoxIsUnsupported(m));
        restoreTimers.checkUnchanged(false, false);
        CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
        CHECK(m.step->db == nullptr); CHECK(m.step->step_version == version); CHECK(m.step->u1378 == objectCount);
        CHECK(m.step->u1b68 == marLength); CHECK(m.step->u1b6c == arrayLength);
        CHECK(m.step->u2b24 == bodyCount); CHECK(m.step->u2b28[0x0b] == storedLength);
        CHECK(m.step->u26c4[0x0b] == 0); CHECK(m.step->u1b58 == 0); CHECK(m.step->u1b59 == 0);
        CHECK(m.step->u1b88 == nullptr); CHECK(m.step->u1b78 == nullptr);
        CHECK(saved.nodes.u008 == saved.coordinates);
        CHECK(saved.elements.u008 == (populated ? saved.connectivity : nullptr));
        CHECK(saved.nodes.u130 == m.step.get()); CHECK(static_cast<const void*>(saved.elements.u0f0) == m.step.get());
    }
}

TEST_CASE(geometry_null_companion_keeps_actual_owner_gate_state_and_mar_refusals)
{
    for (int mode = 0; mode < 6; ++mode) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel foreign(nullptr, 2);
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        auto* object = model.object(1); REQUIRE(object);
        struct RestoreMain {
            def::model::LLOBJ_DATA& object;
            def::model::DB_STEP_DATA* owner = object.owner;
            std::int32_t gate = object.u87c[0];
            std::uint8_t flag = *engine::at<std::uint8_t>(&object, 0x3610);
            ~RestoreMain() {
                object.owner = owner; object.u87c[0] = gate;
                *engine::at<std::uint8_t>(&object, 0x3610) = flag;
            }
        } restoreMain{*object};
        saved.nodes.u130 = &step;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        saved.elements.u008 = nullptr;
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = 0;
        REQUIRE(step.db == nullptr); REQUIRE(object->owner == &step); REQUIRE(object->u87c[0] == 0);
        *engine::at<std::uint8_t>(object, 0x3610) = 0;
        def::model::g_index_tables_dim = 0; def::model::select_index_tables_3d();
        if (mode == 1) object->owner = foreign.step.get();
        if (mode == 2) object->u87c[0] = 1;
        if (mode == 3) def::model::g_mar_index[27] = -1;
        if (mode == 4) def::model::g_mar_index[27] = 0x8c;
        if (mode == 5) { def::model::g_a_index[27] = 0x0b; step.u15f8[0x1a] = 1; }
        const auto marIndex = def::model::g_mar_index[27];
        std::array<std::byte, sizeof object->s8a0> mainBefore{};
        std::memcpy(mainBefore.data(), object->s8a0, mainBefore.size());
        std::array<double, 8> coordinatesBefore{};
        std::memcpy(coordinatesBefore.data(), saved.nodes.u008, sizeof coordinatesBefore);
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        for (std::size_t invocation = 0; invocation < 2; ++invocation) {
            { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
            REQUIRE(trace.dispatches.size() == invocation + 1);
            CHECK(nodeTimer.count == (mode == 0 ? 3 : 2));
            CHECK(arraysTimer.count == static_cast<std::int32_t>(invocation + 1));
            if (mode == 0) checkGeometryScene(model, 0.0f);
            else CHECK(sceneBoxIsUnsupported(model));
            CHECK(engine::GuiModelAccess::withStep(model,
                [&step](def::model::DB_STEP_DATA* current) { return current == &step; }));
            CHECK(step.step_version == 11000); CHECK(step.u26c4[0x0b] == 0); CHECK(step.db == nullptr);
            CHECK(step.u1b88 == nullptr); CHECK(step.u1b78 == nullptr);
            CHECK(def::model::g_mar_index[27] == marIndex);
            CHECK(std::memcmp(object->s8a0, mainBefore.data(), mainBefore.size()) == 0);
            CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == nullptr);
            CHECK(std::memcmp(saved.nodes.u008, coordinatesBefore.data(), sizeof coordinatesBefore) == 0);
        }
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_null_companion_builder_round_preserves_prior_effects_and_avoids_third_getter)
{
    for (const bool secondGateRefusal : {false, true}) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        engine::GuiModel model(nullptr, 3); auto& mesh = geometryQuad(model);
        RestoreGeometryMeshFields saved(mesh); auto& step = *model.step;
        auto* object = model.object(1); REQUIRE(object);
        REQUIRE(object->owner == &step); REQUIRE(object->u87c[0] == 0);
        REQUIRE(def::model::llobj_not_kind7(object));
        struct RestoreMain {
            def::model::LLOBJ_DATA& object;
            std::int32_t gate = object.u87c[0];
            std::uint8_t flag = *engine::at<std::uint8_t>(&object, 0x3610);
            ~RestoreMain() { object.u87c[0] = gate; *engine::at<std::uint8_t>(&object, 0x3610) = flag; }
        } restoreMain{*object};
        *engine::at<std::uint8_t>(object, 0x3610) = 0;
        saved.nodes.u130 = &step;
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(&step);
        if (!secondGateRefusal) saved.nodes.u138 = 3; // Layout guard belongs after both getter rounds.
        step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = 0; REQUIRE(step.db == nullptr);
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        nodeTimer.count = 7; arraysTimer.count = 0; def::model::g_index_tables_dim = 0;
        auto& variables = *reinterpret_cast<GeometryVariables*>(&saved.nodes);
        GeometryVariables lease(variables); const auto* oldBody = lease.body();
        bool firstStopSeen = false;
        GeometryGetterTrace trace{&saved.nodes, {}, &step};
        trace.beforeCoordinateDispatch = [&](std::size_t ordinal) {
            if (ordinal != 1) return;
            firstStopSeen = nodeTimer.count == 3 && arraysTimer.count == 1;
            // Declared test-only reentrancy: refuse the second reader after
            //its own COW/arrays checkpoint, preserving the completed first one.
            if (secondGateRefusal) object->u87c[0] = 1;
        };
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 2u); CHECK(firstStopSeen);
        CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
        CHECK(trace.dispatches[1].before == variables.body()); CHECK(trace.dispatches[1].after == variables.body());
        CHECK(nodeTimer.count == (secondGateRefusal ? 2 : 3)); CHECK(arraysTimer.count == 2);
        CHECK(sceneBoxIsUnsupported(model));
        CHECK(engine::GuiModelAccess::withStep(model,
            [&step](def::model::DB_STEP_DATA* current) { return current == &step; }));
        CHECK(step.step_version == 11000); CHECK(step.u26c4[0x0b] == 0);
        CHECK(step.db == nullptr); CHECK(step.u1b88 == nullptr); CHECK(step.u1b78 == nullptr);
        CHECK(saved.nodes.u008 == saved.coordinates); CHECK(saved.elements.u008 == saved.connectivity);
        object->u87c[0] = 0; saved.nodes.u138 = 2; trace.beforeCoordinateDispatch = {};
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(trace.dispatches.size() == 4u); checkGeometryScene(model, 1.0f);
        CHECK(nodeTimer.count == 3); CHECK(arraysTimer.count == 4);
        CHECK(lease.body() == oldBody); CHECK(variables.use_count() == 1);
        restoreTimers.checkUnchanged(false, false);
    }
}

namespace {
namespace directoryReceipts = def::model::directory_receipts;

struct GeometryDirectoryFixture {
    Project project;
    std::unique_ptr<ProjectScope> scope;
    fs::path directory, file;
    explicit GeometryDirectoryFixture(const char* tag)
    {
        directory = m2m_test::scratchRoot() /
            ("geometry_directory_" + std::to_string(_getpid()) + "_" + tag);
        fs::create_directories(directory);
        file = directory / "X.DB";
        const auto input = m2m_test::configuredPath("M2M_DB_FIXTURE") / "COG_P1_B1.DB";
        REQUIRE(fs::is_regular_file(input));
        fs::copy_file(input, file, fs::copy_options::overwrite_existing);
        scope = std::make_unique<ProjectScope>(project);
        auto* handle = engine::projectDb(project); // actual strong owner and unique reader
        REQUIRE(def::dbio::load_db_info(*handle->reader, file.string().c_str(), false) == 0);
        const auto receipt = directoryReceipts::current(handle, *handle->reader);
        REQUIRE(receipt.has_value()); REQUIRE(receipt->elementCount == std::size_t{32});
    }
    ~GeometryDirectoryFixture()
    {
        std::error_code error;
        fs::remove_all(directory, error);
    }
    engine::DbHandle* handle() { return project.dataDb.get(); }
    def::model::Cls_1401dbfe0& reader() { return *handle()->reader; }
    def::model::DbDirRecord* records()
    {
        return static_cast<def::model::DbDirRecord*>(reader().uad8);
    }
};

struct GeometryDirectoryCandidate {
    engine::GuiModel model;
    def::model::OBJ_MESH2D& mesh;
    RestoreGeometryMeshFields saved;
    explicit GeometryDirectoryCandidate(engine::DbHandle* handle, bool populated = true)
        : model(handle, 3), mesh(geometryQuad(model)), saved(mesh)
    {
        saved.nodes.u130 = model.step.get();
        saved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(model.step.get());
        if (!populated) saved.elements.u008 = nullptr;
        model.step->u1b58 = 0; model.step->u1b59 = 0;
        model.step->step_version = 6000; model.step->u26c4[0x0b] = 0;
        model.step->u740 = 0; model.step->db = handle->reader.get();
    }
    ~GeometryDirectoryCandidate()
    {
        // Opaque/expired owner cases use borrowed pointer values only.
        model.db = nullptr;
        model.step->db = nullptr;
    }
};

TEST_CASE(geometry_parked_reaction_callback_projects_controls_before_model_guard)
{
    using namespace engine::simcontrol300;
    struct Io final : Extra3DIo {
        Io() : Extra3DIo(1252) {}
        bool readDoubles(const qt::String&, std::span<double>) override { return false; }
        bool readInts(const qt::String&, std::span<int>) override { return false; }
        bool readSprings(std::vector<SpringRecord>&) override { return false; }
        bool removeIfReadable(const qt::String&) override { return true; }
        void nbcRemoveWarning() override {}
        std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override { return nullptr; }
    } io;
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture directory("reaction_park");
    GeometryDirectoryCandidate candidate(directory.handle());
    auto& model = candidate.model;
    Operation operation; RemeshState remesh;
    operation.project = &directory.project;
    operation.engineModel = std::shared_ptr<engine::GuiModel>(&model, [](engine::GuiModel*) {});
    auto* liveControl = model.simCtl(); REQUIRE(liveControl);
    liveControl->unit = 1; liveControl->nstart = 7; liveControl->nstep = 23;
    liveControl->trans[0] = liveControl->trans[1] = 1;
    Editor300Binding binding{directory.project, operation, model, remesh};
    StagedEditor300 owner(binding);
    MoInitialObservations features;
    features.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
    features.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
    features.startupHeating = {0, 1};
    InitialEditorContext context{generalContextForMo(directory.project, 3, {true, true}),
        {5, {}, MoStepCountContext{&operation}}, NumericLocale::KnownC, true, features};
    owner.constructPageState(binding, io, context);
    auto* session = owner.pageState(); REQUIRE(session);
    auto& page = session->pages.environment; // retain before parking; pageState() is noexcept
    auto& temporary = *session->environmentCurves.curves[3];
    temporary.var[0] = 77; temporary.ndim = 2;
    liveControl->acvcof.var[0] = 66; liveControl->acvcof.ndim = 5;
    session->environmentCurves.currentCurveIndex = 1;

    directory.records()[0].version = 5999;
    model.step->step_version = 11000;
    auto* object = model.object(1); REQUIRE(object);
    object->u87c[0] = 1; // keep the low-version reader continuation parked
    GeometryGetterTrace trace{&candidate.saved.nodes, {}, model.step.get()};
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
    REQUIRE(trace.dispatches.size() == 1u);
    bool caught = false;
    try { (void)owner.reactionRateClicked(binding, 2); }
    catch (const Error& error) {
        caught = error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find("geometry query is parked") != std::string::npos;
    }
    CHECK(caught);
    int parkedUnit = -900;
    CHECK(model.readPropertyHeatingTimeUnit(parkedUnit));
    CHECK_EQ(parkedUnit, 1);
    CHECK_EQ(page.acvcofControls.selection, 2);
    CHECK(!page.acvcofControls.constantInputEnabled);
    CHECK(!page.acvcofControls.constantUnitEnabled);
    CHECK(page.acvcofControls.definitionEnabled);
    REQUIRE(session->environmentCurves.currentCurveIndex);
    CHECK_EQ(*session->environmentCurves.currentCurveIndex, 1);
    CHECK_EQ(liveControl->acvcof.ndim, 5); CHECK_EQ(liveControl->acvcof.var[0], 66);
    CHECK_EQ(temporary.ndim, 2); CHECK_EQ(temporary.var[0], 77);
}

void checkGeometryDirectoryOutcome(GeometryDirectoryCandidate& candidate, std::int32_t expectedVersion,
                                   bool lowRecord = false, bool nodeSkipLoop = true, bool preselectedTables = false)
{
    auto& model = candidate.model;
    auto* const stepIdentity = model.step.get(); REQUIRE(stepIdentity);
    auto& step = *stepIdentity;
    const auto initialVersion = step.step_version;
    const auto marLength = step.u1b68, arrayLength = step.u1b6c;
    const auto bodyCount = step.u2b24, storedLength = step.u2b28[0x0b], objectCount = step.u1378;
    const auto* db = step.db; const auto* coordinates = candidate.saved.nodes.u008;
    const auto* connectivity = candidate.saved.elements.u008;
    const auto* mar = step.u1b88; const auto* arrays = step.u1b78;
    auto beforeArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    std::unique_ptr<def::model::StepTimer> afterFirstArrays;
    std::unique_ptr<def::model::StepTimer> afterFirstNode;
    const bool stopped = lowRecord && beforeArrays->count >= 0 && nodeSkipLoop;
    const bool parked = lowRecord && !stopped;
    const std::size_t rounds = stopped && connectivity ? 2u : 1u;
    auto& timer = def::model::g_timer_read_obj_node_state_variable;
    timer.count = 7;
    if (!preselectedTables) def::model::g_index_tables_dim = 0;
    auto& variables = *reinterpret_cast<GeometryVariables*>(&candidate.saved.nodes);
    GeometryVariables lease(variables); const auto* oldBody = lease.body();
    GeometryGetterTrace trace{&candidate.saved.nodes, {}, &step};
    for (std::size_t invocation = 0; invocation < 2; ++invocation) {
        const auto beforeDispatches = trace.dispatches.size();
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        REQUIRE(model.step.get() == stepIdentity);
        const auto newDispatches = invocation == 0 || !parked ? rounds : 0u;
        REQUIRE(trace.dispatches.size() == beforeDispatches + newDispatches);
        for (std::size_t j = beforeDispatches; j < trace.dispatches.size(); ++j) {
            const auto& call = trace.dispatches[j];
            const auto version = j == 0 ? initialVersion : expectedVersion;
            CHECK(call.beforeStepVersion == version); CHECK(call.afterStepVersion == version);
            CHECK(call.beforeIndexDimension == (j == 0 && !preselectedTables ? 0 : 3));
            CHECK(call.afterIndexDimension == call.beforeIndexDimension);
            CHECK(call.beforeNodeTimerCount == (j == 0 ? 7 : stopped ? 3 : 2));
            CHECK(call.afterNodeTimerCount == call.beforeNodeTimerCount);
            if (j == 0) { CHECK(call.before == oldBody); CHECK(call.after != oldBody); }
            else CHECK(call.before == call.after);
            CHECK(call.after == variables.body());
        }
        CHECK(def::model::g_index_tables_dim == 3); CHECK(timer.count == (stopped ? 3 : 2));
        CHECK(std::string(timer.labels[0]) == "Start");
        CHECK(std::string(timer.labels[1]) == "ReadSingleAArrayFromDB");
        CHECK(timer.subs[0] == nullptr); CHECK(timer.subs[1] == nullptr);
        if (stopped) { CHECK(std::string(timer.labels[2]) == "Stop"); CHECK(timer.subs[2] == nullptr); }
        CHECK(step.step_version == expectedVersion);
        if (stopped) {
            checkGeometryScene(model, connectivity ? 1.0f : 0.0f);
            CHECK(engine::GuiModelAccess::withStep(model,
                [stepIdentity](def::model::DB_STEP_DATA* current) { return current == stepIdentity; }));
        } else {
            CHECK(sceneBoxIsUnsupported(model));
            if (lowRecord) {
                bool invoked = false, refused = false;
                try { engine::GuiModelAccess::withStep(model,
                    [&invoked](def::model::DB_STEP_DATA*) { invoked = true; }); }
                catch (const Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
                CHECK(refused); CHECK(!invoked);
            }
        }
        CHECK(step.db == db); CHECK(step.u1b58 == 0); CHECK(step.u1b59 == 0);
        CHECK(step.u26c4[0x0b] == 0); CHECK(step.u1378 == objectCount);
        CHECK(step.u1b68 == marLength); CHECK(step.u1b6c == arrayLength);
        CHECK(step.u2b24 == bodyCount); CHECK(step.u2b28[0x0b] == storedLength);
        CHECK(step.u1b88 == mar); CHECK(step.u1b78 == arrays);
        CHECK(candidate.saved.nodes.u008 == coordinates); CHECK(candidate.saved.elements.u008 == connectivity);
        if (lowRecord && beforeArrays->count >= 0) checkGeometryArraysTimerAppend(*beforeArrays, 1);
        else CHECK(std::memcmp(&def::model::g_timer_read_arrays, beforeArrays.get(), sizeof *beforeArrays) == 0);
        if (invocation == 0) {
            afterFirstArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
            afterFirstNode = std::make_unique<def::model::StepTimer>(timer);
        }
        else {
            REQUIRE(afterFirstArrays);
            CHECK(std::memcmp(&def::model::g_timer_read_arrays, afterFirstArrays.get(), sizeof *afterFirstArrays) == 0);
            if (lowRecord) {
                REQUIRE(afterFirstNode);
                CHECK(std::memcmp(&timer, afterFirstNode.get(), sizeof timer) == 0);
            }
        }
    }
    CHECK(trace.dispatches[0].beforeStepVersion == initialVersion);
    checkGeometryGetterArguments(trace, step.u15f8);
    CHECK(variables.use_count() == 1); CHECK(lease.use_count() == 1); CHECK(lease.body() == oldBody);
}
} // namespace

TEST_CASE(geometry_nonnull_directory_query_keeps_high_versions_pending_with_actual_owner)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture fixture("versions");
    for (const auto version : {std::int32_t{6000}, std::int32_t{11000},
        std::numeric_limits<std::int32_t>::max()})
    for (const auto index : {0, 31}) for (const bool populated : {false, true}) {
        auto& record = fixture.records()[index];
        record.version = version; record.hsize = 123; record.mar_size = 456; record.a_size = 789;
        GeometryDirectoryCandidate candidate(fixture.handle(), populated);
        candidate.model.step->u740 = index;
        candidate.model.step->step_version = version == 11000 ? 6000 : 11000;
        checkGeometryDirectoryOutcome(candidate, version);
        restoreTimers.checkUnchanged(false);
    }
}

TEST_CASE(geometry_nonnull_low_record_query_returns_actual_model_without_replay)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture fixture("low_versions");
    for (const auto version : {std::int32_t{5999}, std::int32_t{0}, std::int32_t{-1},
        std::numeric_limits<std::int32_t>::min()}) {
        fixture.records()[0].version = version;
        GeometryDirectoryCandidate candidate(fixture.handle());
        candidate.model.step->step_version = 11000;
        checkGeometryDirectoryOutcome(candidate, version, true);
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_low_record_arrays_timer_commits_once_before_return_or_park)
{
    for (const auto initialCount : {std::int32_t{-1}, std::int32_t{0}, std::int32_t{99},
        std::int32_t{100}, std::int32_t{101}, std::numeric_limits<std::int32_t>::max()})
    for (const bool populated : {false, true}) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        GeometryDirectoryFixture fixture("low_timer");
        fixture.records()[0].version = 5999;
        GeometryDirectoryCandidate candidate(fixture.handle(), populated);
        candidate.model.step->step_version = 11000;
        auto& arraysTimer = def::model::g_timer_read_arrays;
        arraysTimer.count = initialCount;
        int marker = 0;
        for (std::size_t i = 0; i < 100; ++i) {
            std::memset(arraysTimer.labels[i], 'Y', sizeof arraysTimer.labels[i]);
            std::snprintf(arraysTimer.labels[i], sizeof arraysTimer.labels[i], "low-arrays-kept-%zu", i);
            arraysTimer.times[i] = static_cast<std::int32_t>(1200 + i);
            arraysTimer.subs[i] = &marker;
        }
        checkGeometryDirectoryOutcome(candidate, 5999, true);
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_completed_low_reader_does_not_repark_at_later_builder_guard)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture fixture("returned_builder");
    fixture.records()[0].version = 5999;
    GeometryDirectoryCandidate candidate(fixture.handle());
    auto& model = candidate.model; auto& step = *model.step;
    step.step_version = 11000;
    candidate.saved.nodes.u138 = 3; // Outside the recovered 2D builder layout, after reader return.
    auto& variables = *reinterpret_cast<GeometryVariables*>(&candidate.saved.nodes);
    GeometryVariables lease(variables); const auto* oldBody = lease.body();
    auto beforeArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    auto& timer = def::model::g_timer_read_obj_node_state_variable;
    timer.count = 7; def::model::g_index_tables_dim = 0;
    GeometryGetterTrace trace{&candidate.saved.nodes, {}, &step};
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
    REQUIRE(trace.dispatches.size() == 2u); // First getter was not replayed; builder has its own round.
    CHECK(trace.dispatches[0].before == oldBody); CHECK(trace.dispatches[0].after != oldBody);
    CHECK(trace.dispatches[1].before == variables.body()); CHECK(trace.dispatches[1].after == variables.body());
    CHECK(step.step_version == 5999); CHECK(timer.count == 3);
    CHECK(std::string(timer.labels[2]) == "Stop"); CHECK(timer.subs[2] == nullptr);
    checkGeometryArraysTimerAppend(*beforeArrays, 1);
    CHECK(sceneBoxIsUnsupported(model));
    CHECK(engine::GuiModelAccess::withStep(model,
        [&step](def::model::DB_STEP_DATA* current) { return current == &step; }));
    auto completedNode = std::make_unique<def::model::StepTimer>(timer);
    auto completedArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
    REQUIRE(trace.dispatches.size() == 4u);
    for (std::size_t i = 2; i < trace.dispatches.size(); ++i) {
        CHECK(trace.dispatches[i].before == variables.body()); CHECK(trace.dispatches[i].after == variables.body());
        CHECK(trace.dispatches[i].beforeStepVersion == 5999); CHECK(trace.dispatches[i].afterStepVersion == 5999);
    }
    CHECK(std::memcmp(&timer, completedNode.get(), sizeof timer) == 0);
    CHECK(std::memcmp(&def::model::g_timer_read_arrays, completedArrays.get(), sizeof *completedArrays) == 0);
    CHECK(sceneBoxIsUnsupported(model)); CHECK(lease.body() == oldBody);
    restoreTimers.checkUnchanged(false, false);
}

TEST_CASE(geometry_low_record_node_skip_requires_actual_owner_and_ordered_assignment_state)
{
    for (int mode = 0; mode < 19; ++mode) {
        RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
        GeometryDirectoryFixture fixture("node_skip");
        fixture.records()[0].version = 5999;
        engine::GuiModel foreign(nullptr, 2);
        GeometryDirectoryCandidate candidate(fixture.handle());
        auto& model = candidate.model; auto& step = *model.step;
        step.step_version = 11000;
        auto* first = model.object(1); REQUIRE(first);
        // Main-object assignment is independent of the Viewer list. Keep
        //secondary foreign/skip fixtures outside later display consumers.
        auto* second = def::model::llobj_ctor::get_or_create(&step, 2); REQUIRE(second);
        auto* third = def::model::llobj_ctor::get_or_create(&step, 3); REQUIRE(third);
        REQUIRE(engine::viewerItemCount(model) == 1);
        struct RestoreMainObject {
            def::model::LLOBJ_DATA& object;
            def::model::DB_STEP_DATA* owner = object.owner;
            std::int32_t gate = object.u87c[0];
            std::int32_t kind = *engine::at<std::int32_t>(&object, 0x0c);
            std::uint8_t skip = *engine::at<std::uint8_t>(&object, 0x3610);
            ~RestoreMainObject() {
                object.owner = owner; object.u87c[0] = gate;
                *engine::at<std::int32_t>(&object, 0x0c) = kind;
                *engine::at<std::uint8_t>(&object, 0x3610) = skip;
            }
        } restoreFirst{*first}, restoreSecond{*second}, restoreThird{*third};
        for (auto* object : {first, second, third}) {
            REQUIRE(object->owner == &step); REQUIRE(object->u87c[0] == 0);
            *engine::at<std::int32_t>(object, 0x0c) = 5;
            *engine::at<std::uint8_t>(object, 0x3610) = 0;
        }
        REQUIRE(step.u1378 == 3);
        def::model::g_index_tables_dim = 0; def::model::select_index_tables_3d();
        bool stopped = true;
        if (mode == 1) { first->owner = foreign.step.get(); stopped = false; }
        if (mode == 2) { first->u87c[0] = 1; stopped = false; }
        if (mode == 3 || mode == 4) {
            second->owner = foreign.step.get(); second->u87c[0] = 1;
            if (mode == 3) *engine::at<std::int32_t>(second, 0x0c) = 7;
            else *engine::at<std::uint8_t>(second, 0x3610) = 1;
        }
        if (mode == 5) { second->u87c[0] = 1; stopped = false; }
        if (mode == 6) { first->owner = nullptr; stopped = false; }
        if (mode == 7 || mode == 8) {
            def::model::g_a_index[27] = 0x0b;
            step.u15f8[0x1a] = mode == 7 ? 1 : 0;
            stopped = mode == 8;
            if (mode == 8) { // First matching zero wins over the later positive field.
                def::model::g_a_index[32] = 0x0b; step.u15f8[0x1b] = 1;
            }
        }
        if (mode == 9 || mode == 10) {
            def::model::g_a_index[mode == 9 ? 32 : 35] = 0x0b;
            step.u15f8[mode == 9 ? 0x1b : 0x1c] = 1; stopped = false;
        }
        if (mode == 11 || mode == 12) {
            const auto start = def::model::g_mar_index[3], stride = def::model::g_mar_index[27];
            REQUIRE(start >= 0 && start < 0x8c); REQUIRE(stride >= 0 && stride < 0x8c);
            step.u1b90[start] = mode == 11 ? std::numeric_limits<std::int32_t>::min()
                                         : std::numeric_limits<std::int32_t>::max();
            step.u1b90[stride] = mode == 11 ? std::numeric_limits<std::int32_t>::max()
                                          : std::numeric_limits<std::int32_t>::min();
            // No MAR/A allocation or per-object record pointer is consumed.
            REQUIRE(step.u1b88 == nullptr); REQUIRE(step.u1b78 == nullptr);
        }
        if (mode >= 13 && mode <= 16) {
            const std::array<std::int32_t, 4> invalidIndices{
                -1, 0x8c, std::numeric_limits<std::int32_t>::min(),
                std::numeric_limits<std::int32_t>::max()};
            def::model::g_mar_index[27] = invalidIndices[static_cast<std::size_t>(mode - 13)];
            stopped = false;
        }
        if (mode == 17 || mode == 18) {
            const std::int32_t stride = mode == 17 ? 0 : 0x8b;
            def::model::g_mar_index[27] = stride;
            step.u1b90[stride] = mode == 17 ? std::numeric_limits<std::int32_t>::min()
                                          : std::numeric_limits<std::int32_t>::max();
        }
        std::array<std::byte, sizeof first->s8a0> firstRows{};
        std::memcpy(firstRows.data(), first->s8a0, firstRows.size());
        std::array<std::byte, sizeof step.u1b90> marTableBefore{};
        std::memcpy(marTableBefore.data(), step.u1b90, marTableBefore.size());
        const auto strideIndexBefore = def::model::g_mar_index[27];
        const auto* optionalCoordinates = candidate.saved.nodes.u008;
        checkGeometryDirectoryOutcome(candidate, 5999, true, stopped, true);
        CHECK(def::model::g_mar_index[27] == strideIndexBefore);
        CHECK(std::memcmp(step.u1b90, marTableBefore.data(), marTableBefore.size()) == 0);
        CHECK(std::memcmp(first->s8a0, firstRows.data(), firstRows.size()) == 0);
        CHECK(candidate.saved.nodes.u008 == optionalCoordinates);
        CHECK(first->owner == (mode == 1 ? foreign.step.get() : mode == 6 ? nullptr : &step));
        restoreTimers.checkUnchanged(false, false);
    }
}

TEST_CASE(geometry_nonnull_directory_query_bounds_use_actual_extent_not_logical_count)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture fixture("bounds");
    fixture.reader().uac8 = 33;
    for (const auto index : {std::int32_t{-1}, std::int32_t{32},
        std::numeric_limits<std::int32_t>::max()}) {
        GeometryDirectoryCandidate candidate(fixture.handle());
        candidate.model.step->u740 = index;
        checkGeometryDirectoryOutcome(candidate, 6000);
    }
    fixture.reader().uac8 = 1;
    GeometryDirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = 31;
    checkGeometryDirectoryOutcome(candidate, 11000);
    restoreTimers.checkUnchanged(false);
}

TEST_CASE(geometry_nonnull_directory_query_rejects_missing_foreign_expired_and_unobserved_owners)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    for (int mode = 0; mode < 8; ++mode) {
        GeometryDirectoryFixture fixture("owners");
        GeometryDirectoryCandidate candidate(fixture.handle());
        std::shared_ptr<engine::DbHandle> outsideLease;
        struct RestorePointer {
            def::model::Cls_1401dbfe0& reader; void* pointer;
            explicit RestorePointer(def::model::Cls_1401dbfe0& value) : reader(value), pointer(value.uad8) {}
            ~RestorePointer() { reader.uad8 = pointer; }
        };
        std::unique_ptr<RestorePointer> restorePointer;
        switch (mode) {
        case 0: fixture.scope.reset(); break; // missing actual Project scope
        case 1:
            outsideLease = fixture.project.dataDb; fixture.project.dataDb.reset(); break;
        case 2: fixture.project.dataDb.reset(); break; // actual owner/reader expired
        case 3: fixture.project.dataDb = std::make_shared<engine::DbHandle>(); break;
        case 4:
            candidate.model.db = reinterpret_cast<engine::DbHandle*>(candidate.model.object(1)); break;
        case 5:
            candidate.model.step->db = reinterpret_cast<def::model::Cls_1401dbfe0*>(candidate.model.object(1)); break;
        case 6:
            fixture.handle()->reader = std::make_unique<def::model::Cls_1401dbfe0>();
            candidate.model.step->db = fixture.handle()->reader.get(); break;
        case 7:
            restorePointer = std::make_unique<RestorePointer>(fixture.reader());
            fixture.reader().uad8 = nullptr; break;
        }
        checkGeometryDirectoryOutcome(candidate, 6000);
        restoreTimers.checkUnchanged(false);
    }
}

TEST_CASE(geometry_nonnull_directory_query_rebinds_current_receipt_after_reload_and_multipart)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture fixture("replacement");
    const auto first = directoryReceipts::current(fixture.handle(), fixture.reader()); REQUIRE(first);
    GeometryDirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = 31;
    checkGeometryDirectoryOutcome(candidate, 11000);
    REQUIRE(def::dbio::load_db_info(fixture.reader(), fixture.file.string().c_str(), false) == 0);
    const auto second = directoryReceipts::current(fixture.handle(), fixture.reader()); REQUIRE(second);
    CHECK(first->allocationGeneration != second->allocationGeneration); CHECK(!directoryReceipts::isCurrent(*first));
    fixture.records()[31].version = 12000;
    checkGeometryDirectoryOutcome(candidate, 12000);
    fs::copy_file(fixture.file, fixture.directory / "X25.DB", fs::copy_options::overwrite_existing);
    fixture.records()[0].step = 26;
    REQUIRE(def::dbio::query::setup_part_chain(fixture.reader()) == 0);
    const auto third = directoryReceipts::current(fixture.handle(), fixture.reader()); REQUIRE(third);
    CHECK_EQ(third->elementCount, std::size_t{64}); CHECK(!directoryReceipts::isCurrent(*second));
    CHECK(first->ownerGeneration == third->ownerGeneration);
    candidate.model.step->u740 = 63; candidate.model.step->step_version = 6000;
    checkGeometryDirectoryOutcome(candidate, 12000);
    restoreTimers.checkUnchanged(false);
}

TEST_CASE(geometry_nonnull_directory_query_retains_physical_bounds_after_failed_reload)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    GeometryDirectoryFixture fixture("failed_reload");
    const auto original = directoryReceipts::current(fixture.handle(), fixture.reader()); REQUIRE(original);
    {
        std::fstream file(fixture.file, std::ios::binary | std::ios::in | std::ios::out);
        REQUIRE(file.is_open()); std::array<char, 4> word{};
        const std::array<char, 4> count32{0, 0, 0, 0x20}, chunk4{0, 0, 0, 4};
        const std::array<char, 4> count33{0, 0, 0, 0x21}, chunk0{0, 0, 0, 0};
        file.seekg(632); file.read(word.data(), 4); REQUIRE(file.good()); REQUIRE(word == count32);
        file.seekg(1024); file.read(word.data(), 4); REQUIRE(file.good()); REQUIRE(word == chunk4);
        file.seekp(632); file.write(count33.data(), 4); REQUIRE(file.good());
        file.seekp(1024); file.write(chunk0.data(), 4); file.flush(); REQUIRE(file.good());
        file.close(); REQUIRE(!file.fail());
    }
    REQUIRE(def::dbio::load_db_info(fixture.reader(), fixture.file.string().c_str(), false) == 3);
    CHECK_EQ(fixture.reader().uac8, 33); CHECK(directoryReceipts::isCurrent(*original));
    GeometryDirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = 32;
    checkGeometryDirectoryOutcome(candidate, 6000);
    candidate.model.step->u740 = 31;
    checkGeometryDirectoryOutcome(candidate, 11000);
    const auto retained = directoryReceipts::current(fixture.handle(), fixture.reader()); REQUIRE(retained);
    CHECK_EQ(retained->elementCount, std::size_t{32});
    CHECK_EQ(retained->allocationGeneration, original->allocationGeneration);
    restoreTimers.checkUnchanged(false);
}

TEST_CASE(geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass)
{
    RestoreGeometryTimers restoreTimers; RestoreGeometryIndexTables restoreTables;
    const auto previousLoaderSlots = mst::g_loaderSlots;
    struct RestoreLoaderSlots {
        mst::LoaderSlots slots;
        ~RestoreLoaderSlots() { mst::g_loaderSlots = slots; }
    } restoreLoaderSlots{previousLoaderSlots};
    mst::installLoaderSlots(); REQUIRE(mst::g_loaderSlots.setDimension); REQUIRE(mst::g_loaderSlots.showModel);
    GeometryDirectoryFixture fixture("model_barrier");
    Project outgoingDisplayProject; // outlives the model's borrowed display backlink
    GeometryDirectoryCandidate candidate(fixture.handle());
    auto& model = candidate.model; auto& step = *model.step;
    step.u1b58 = 1; // establish both Views before arming the pending-query fixture
    auto& secondMesh = geometryQuad(model, 2);
    RestoreGeometryMeshFields secondSaved(secondMesh);
    secondSaved.nodes.u130 = model.step.get();
    secondSaved.elements.u0f0 = reinterpret_cast<def::model::LLOBJ_DATA*>(model.step.get());
    REQUIRE(engine::viewerItemCount(model) == 2);
    auto* const firstObject = model.object(1); REQUIRE(firstObject);
    struct RestoreNodeGate {
        def::model::LLOBJ_DATA& object;
        std::int32_t gate;
        ~RestoreNodeGate() { object.u87c[0] = gate; }
    } restoreNodeGate{*firstObject, firstObject->u87c[0]};
    firstObject->u87c[0] = 1; // Keep this fixture genuinely incomplete after the arrays checkpoint.
    const auto keyCopy = fixture.directory / "guarded.KEY";
    fs::copy_file(m2m_test::configuredPath("M2M_DB_FIXTURE") / "Objects" / "Object00001.KEY",
        keyCopy, fs::copy_options::overwrite_existing);
    const qt::String keyPath{keyCopy.u16string()};
    {
        RestoreGeometryTimers controlTimers; RestoreGeometryIndexTables controlTables;
        engine::GuiModel control(nullptr, 3);
        REQUIRE(control.readKey(keyPath)); // successful nonpending import still works
        auto* const object = control.object(1); REQUIRE(object);
        CHECK(control.objectAt(1) == object); CHECK(control.objectWrap(1, false, false) == object);
        CHECK(control.simCtl() != nullptr); (void)control.material(1);
        control.step->dimension = 3;
        CHECK_EQ(control.dimension(), 3);
        CHECK_EQ(control.simGeometryTypeIf2D(), 0); // the loaded step is 3D
        *engine::at<std::int32_t>(&control.step->sim_ctrl, 0x100) = 37;
        control.step->dimension = 2;
        CHECK_EQ(control.simGeometryTypeIf2D(), 37);
        control.step->dimension = 3;
        control.setPdie(1); CHECK(control.pdie() == 1); CHECK(control.objectCount() > 0);
        const auto countBeforeDelete = control.objectCount();
        engine::deleteObject(control, 1);
        CHECK(control.objectCount() == countBeforeDelete - 1); CHECK(control.object(1) == nullptr);
        mst::g_loaderSlots.setDimension(control, 2); CHECK(engine::guiModelDim(control) == 2);
        CHECK_EQ(control.dimension(), 2);
        int accessInvocations = 0;
        CHECK(engine::GuiModelAccess::withStep(control, [&](def::model::DB_STEP_DATA* step) {
            ++accessInvocations;
            return step == control.step.get() && step->dimension == 2;
        }));
        CHECK(accessInvocations == 1);
        engine::GuiModel empty(nullptr, 3); int noStep = 0;
        empty.step.reset(); CHECK_EQ(empty.simGeometryTypeIf2D(), 0);
        CHECK_EQ(empty.dimension(), 0);
        CHECK(!engine::readDbEntry(empty, &noStep, false)); CHECK(!engine::readDbStep(empty, &noStep, false));
        CHECK(engine::modelDbReader(empty) == nullptr);
        Project displayProject;
        engine::GuiModel displayed(nullptr, 3);
        engine::displayShow(displayProject, &displayed);
        REQUIRE(displayProject.display); CHECK(engine::displayMain(*displayProject.display) == &displayed);
        CHECK(displayed.display == displayProject.display.get());
        auto& links = engine::displayModels(*displayProject.display);
        REQUIRE(links.named.empty()); REQUIRE(!links.second.get()); REQUIRE(!links.third.get());
        // Null participates in the second/third comparisons before main reset.
        engine::displayShow(displayProject, nullptr);
        CHECK(engine::displayMain(*displayProject.display) == &displayed);
        CHECK(displayed.display == displayProject.display.get());
        engine::GuiModel second(nullptr, 3), third(nullptr, 3);
        links.second.reset(&second); links.third.reset(&third);
        engine::displayShow(displayProject, nullptr); // neither secondary guard now matches null
        CHECK(engine::displayMain(*displayProject.display) == nullptr);
        CHECK(displayed.display == displayProject.display.get()); // source does not clear old backlink
    }
    engine::displayShow(outgoingDisplayProject, &model);
    REQUIRE(outgoingDisplayProject.display);
    const auto outgoingDisplay = outgoingDisplayProject.display;
    REQUIRE(model.simCtl());
    *engine::at<std::int32_t>(&step.sim_ctrl, 0x100) = 173; // source GEOTYP before parking
    fixture.records()[0].version = 5999;
    fixture.records()[0].hsize = 123; fixture.records()[0].mar_size = 456; fixture.records()[0].a_size = 789;
    step.u1b58 = 0; step.step_version = 11000; step.u26c4[0x0b] = 0; step.u740 = 0;

    auto& firstVariables = *reinterpret_cast<GeometryVariables*>(&candidate.saved.nodes);
    auto& secondVariables = *reinterpret_cast<GeometryVariables*>(&secondSaved.nodes);
    GeometryVariables firstLease(firstVariables), secondLease(secondVariables);
    const auto* firstBody = firstVariables.body(); const auto* secondBody = secondVariables.body();
    const auto marLength = step.u1b68, arrayLength = step.u1b6c, objectCount = step.u1378;
    const auto* mar = step.u1b88; const auto* arrays = step.u1b78;
    auto beforeArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    def::model::g_index_tables_dim = 0;
    def::model::g_timer_read_obj_node_state_variable.count = 7;
    GeometryGetterTrace trace{&candidate.saved.nodes, {}, &step};
    { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
    REQUIRE(trace.dispatches.size() == 1u);
    CHECK(trace.dispatches[0].beforeStepVersion == 11000); CHECK(trace.dispatches[0].afterStepVersion == 11000);
    CHECK(trace.dispatches[0].beforeNodeTimerCount == 7); CHECK(trace.dispatches[0].afterNodeTimerCount == 7);
    CHECK(trace.dispatches[0].beforeIndexDimension == 0); CHECK(trace.dispatches[0].afterIndexDimension == 0);
    CHECK(firstVariables.body() != firstBody); CHECK(firstLease.body() == firstBody);
    CHECK(firstVariables.use_count() == 1); CHECK(firstLease.use_count() == 1);
    CHECK(secondVariables.body() == secondBody); CHECK(secondLease.body() == secondBody);
    CHECK(secondVariables.use_count() == 2); CHECK(secondLease.use_count() == 2);
    CHECK(step.step_version == 5999); CHECK(step.u26c4[0x0b] == 0); CHECK(step.u1378 == objectCount);
    CHECK(step.u1b68 == marLength); CHECK(step.u1b6c == arrayLength);
    CHECK(step.u1b88 == mar); CHECK(step.u1b78 == arrays);
    CHECK(def::model::g_index_tables_dim == 3);
    CHECK(def::model::g_timer_read_obj_node_state_variable.count == 2);
    checkGeometryArraysTimerAppend(*beforeArrays, 1);
    CHECK(sceneBoxIsUnsupported(model)); restoreTimers.checkUnchanged(false, false);

    auto refused = [&](auto&& action) {
        bool caught = false;
        try { action(); } catch (const Error& error) { caught = error.code == M2M_E_UNSUPPORTED; }
        CHECK(caught);
    };
    auto pollWithoutEffects = [&] {
        std::array<unsigned char, sizeof(def::model::DB_STEP_DATA)> stepBytes{};
        std::memcpy(stepBytes.data(), model.step.get(), stepBytes.size());
        RestoreGeometryTimers timers;
        RestoreGeometryIndexTables tables;
        const auto viewer = model.viewer;
        const auto* firstCurrent = firstVariables.body(); const auto* secondCurrent = secondVariables.body();
        const auto firstCount = firstVariables.use_count(), secondCount = secondVariables.use_count();
        { ScopedGeometryGetterTrace scope(trace); engine::viewerRefresh(model); }
        CHECK(trace.dispatches.size() == 1u);
        CHECK(model.viewer == viewer); CHECK(sceneBoxIsUnsupported(model));
        CHECK(std::memcmp(stepBytes.data(), model.step.get(), stepBytes.size()) == 0);
        CHECK(firstVariables.body() == firstCurrent); CHECK(secondVariables.body() == secondCurrent);
        CHECK(firstVariables.use_count() == firstCount); CHECK(secondVariables.use_count() == secondCount);
        timers.checkUnchanged();
        CHECK(def::model::g_index_tables_dim == tables.dimension);
        for (std::size_t i = 0; i < tables.mar.size(); ++i) CHECK(def::model::g_mar_index[i] == tables.mar[i]);
        for (std::size_t i = 0; i < tables.arrays.size(); ++i) CHECK(def::model::g_a_index[i] == tables.arrays[i]);
    };
    pollWithoutEffects(); pollWithoutEffects();
    refused([&] { engine::viewerSync(model, firstObject, false); });
    refused([&] { engine::viewerSync(model, nullptr, true); });
    float minimum[3] = {}, maximum[3] = {};
    refused([&] { (void)engine::viewerBox(model, 1, minimum, maximum); });

    // All built-in mutation entries and fresh mutable capabilities must refuse
    // before imports, object creation, picking allocation or sim-control writes.
    model.viewerMirrorPlanes = {7, 11}; // external legacy state cannot retire the barrier
    const auto mirrorStateBeforeEntries = model.viewerMirrorPlanes;
    std::array<unsigned char, sizeof(def::model::DB_STEP_DATA)> beforeEntries{};
    std::memcpy(beforeEntries.data(), model.step.get(), beforeEntries.size());
    std::array<std::int32_t, 16> tableBefore{};
    std::array<double, 6> tableDoublesBefore{}, tableBoxBefore{};
    std::memcpy(tableBefore.data(), model.tableF8, sizeof model.tableF8);
    std::memcpy(tableDoublesBefore.data(), model.tableF8d, sizeof model.tableF8d);
    std::memcpy(tableBoxBefore.data(), model.tableF8box, sizeof model.tableF8box);
    const auto stateBefore = model.state100, pdieBefore = model.pdie();
    const auto viewerBefore = model.viewer;
    const auto* pickingBefore = model.pendingPickingConstruction(); REQUIRE(!pickingBefore);
    RestoreGeometryTimers entryTimers; RestoreGeometryIndexTables entryTables;
    for (const bool reset : {false, true}) {
        for (const bool contacts : {false, true})
            refused([&] { engine::viewerRebuild(model, reset, contacts); });
    }
    const auto& pendingModel = model;
    CHECK_EQ(pendingModel.dimension(), step.dimension);
    CHECK_EQ(engine::guiModelDim(pendingModel), step.dimension);
    int requestedNumber = 99; bool mark5 = true;
    doe::PropertyTableState parkedPropertyTable(model);
    parkedPropertyTable.markPopulationUnrecovered();
    refused([&] { engine::finalizeStepDataPropertyTable(model, parkedPropertyTable); });
    CHECK(parkedPropertyTable.owner() == &model);
    CHECK(parkedPropertyTable.queryCount() == 0 && parkedPropertyTable.queryPrefix().empty());
    refused([&] { (void)pendingModel.simCtl(); });
    refused([&] { model.initializeMultiBlowLoadControl(); });
    refused([&] { model.setMultiBlowPrimaryDie(777); });
    refused([&] { model.setHeatTreatPropertyStepDefinition(0); });
    refused([&] { (void)model.matchesCurrentSimControl(nullptr); });
    engine::simcontrol300::StepCountContext parkedStepContext{5, {},
        engine::simcontrol300::MoStepCountContext{nullptr}};
    engine::simcontrol300::StepCountPage parkedStepPage{101, 102, 103, 104,
        engine::simcontrol300::MoStepCountContext{nullptr}, true};
    refused([&] { (void)model.refillEditorStepCountIfControlMatches(
        nullptr, parkedStepContext, false, parkedStepPage); });
    CHECK_EQ(parkedStepPage.start, 101); CHECK_EQ(parkedStepPage.steps, 102);
    CHECK_EQ(parkedStepPage.increment, 103); CHECK_EQ(parkedStepPage.primaryDie, 104);
    REQUIRE(parkedStepPage.mo); CHECK(parkedStepPage.mo->operation == nullptr);
    CHECK(parkedStepPage.nstartChecked);
    auto heldParkedStep = std::move(model.step);
    refused([&] { (void)model.matchesCurrentSimControl(nullptr); });
    model.step = std::move(heldParkedStep);
    refused([&] { (void)pendingModel.simGeometryTypeIf2D(); });
    refused([&] { (void)pendingModel.lastObjectNumber(); });
    refused([&] { (void)model.applyEditorFrequencyIndexIfControlMatches(nullptr, 0, 2); });
    refused([&] { (void)model.applyEditorFrequencyTextIfControlMatches(nullptr, 0, qt::L("1")); });
    refused([&] { (void)pendingModel.object(1); });
    REQUIRE(pendingModel.objectCount() > 0);
    engine::simcontrol300::SpringComboLabels parkedLabels{
        {qt::L("parked-first")}, {qt::L("parked-second")}, 31, 47};
    refused([&] { (void)pendingModel.copyEditorSpringComboLabels(parkedLabels); });
    CHECK((parkedLabels.first == std::vector<qt::String>{qt::L("parked-first")}));
    CHECK((parkedLabels.second == std::vector<qt::String>{qt::L("parked-second")}));
    CHECK_EQ(parkedLabels.firstCurrentIndex, 31); CHECK_EQ(parkedLabels.secondCurrentIndex, 47);

    engine::simcontrol300::MeshObjectSnapshot parkedMeshingObject{};
    parkedMeshingObject.number = 1;
    parkedMeshingObject.interference = 913.25;
    parkedMeshingObject.sizeRatio = 914.5;
    parkedMeshingObject.tolerance = 915.75;
    parkedMeshingObject.nodeCount = 916.125;
    refused([&] { model.copyEditorMeshingObjectValues(parkedMeshingObject); });
    refused([&] { (void)pendingModel.objectAt(1); });
    refused([&] { (void)pendingModel.material(1); });
    refused([&] { (void)model.objectWrap(requestedNumber, true, true); });
    refused([&] { (void)model.appendObject(); });
    refused([&] { (void)model.readKey(keyPath); });
    refused([&] { (void)model.readMaterial(keyPath, &requestedNumber); });
    refused([&] { (void)model.loadObjectKey(keyPath, &requestedNumber); });
    refused([&] { (void)model.loadObjectKeyGeometry(keyPath, &requestedNumber, &mark5); });
    refused([&] { (void)model.beginPickingConstructionPrefix(); });
    refused([&] { model.setPdie(777); });
    const std::array<int, 3> heatTreatWords{0x108, 0x110, 0x10c};
    std::array<std::int32_t, 3> heatTreatBefore{};
    for (std::size_t i = 0; i < heatTreatWords.size(); ++i)
        std::memcpy(&heatTreatBefore[i], reinterpret_cast<const std::byte*>(&model.step->sim_ctrl) + heatTreatWords[i], sizeof(std::int32_t));
    refused([&] { model.setHeatTreatPropertyDeformation(true); });
    refused([&] { model.setHeatTreatPropertyDiffusion(true); });
    refused([&] { model.setHeatTreatPropertyPhaseTransformation(true); });
    for (std::size_t i = 0; i < heatTreatWords.size(); ++i) {
        std::int32_t after{};
        std::memcpy(&after, reinterpret_cast<const std::byte*>(&model.step->sim_ctrl) + heatTreatWords[i], sizeof(after));
        CHECK_EQ(after, heatTreatBefore[i]);
    }
    int dbIndex = 0; const int dbStep = fixture.records()[dbIndex].step;
    refused([&] { (void)engine::modelDbReader(model); });
    for (const bool resetViewer : {false, true}) {
        refused([&] { (void)engine::readDbEntry(model, &dbIndex, resetViewer); });
        refused([&] { (void)engine::readDbStep(model, &dbStep, resetViewer); });
    }
    REQUIRE(pdieBefore == 1 || pdieBefore == 2);
    for (const int object : {0, 99, pdieBefore, pdieBefore == 1 ? 2 : 1})
        refused([&] { engine::deleteObject(model, object); });
    refused([&] { engine::repickPdie(model); });
    refused([&] { mst::g_loaderSlots.setDimension(model, 2); });
    refused([&] { model.setDimension(2); });
    bool accessCallbackInvoked = false;
    refused([&] {
        (void)engine::GuiModelAccess::withStep(model, [&](def::model::DB_STEP_DATA*) {
            accessCallbackInvoked = true;
            return true;
        });
    });
    CHECK(!accessCallbackInvoked);
    CHECK(std::memcmp(beforeEntries.data(), model.step.get(), beforeEntries.size()) == 0);
    CHECK(std::memcmp(tableBefore.data(), model.tableF8, sizeof model.tableF8) == 0);
    CHECK(std::memcmp(tableDoublesBefore.data(), model.tableF8d, sizeof model.tableF8d) == 0);
    CHECK(std::memcmp(tableBoxBefore.data(), model.tableF8box, sizeof model.tableF8box) == 0);
    CHECK(model.state100 == stateBefore); CHECK(model.pdie() == pdieBefore);
    CHECK(model.viewerMirrorPlanes == mirrorStateBeforeEntries);
    CHECK(model.objectCount() == objectCount); CHECK(engine::viewerItemCount(model) == 2);
    CHECK(model.viewer == viewerBefore); CHECK(model.pendingPickingConstruction() == pickingBefore);
    entryTimers.checkUnchanged();
    CHECK(def::model::g_index_tables_dim == entryTables.dimension);
    for (std::size_t i = 0; i < entryTables.mar.size(); ++i) CHECK(def::model::g_mar_index[i] == entryTables.mar[i]);
    for (std::size_t i = 0; i < entryTables.arrays.size(); ++i) CHECK(def::model::g_a_index[i] == entryTables.arrays[i]);
    pollWithoutEffects();

    // Incoming parked models must refuse before allocating a display or
    // altering existing Display.main/model backlink/camera state.
    auto checkSameCamera = [](const engine::CameraState& before, const engine::CameraState& after) {
        CHECK(before.version == after.version); CHECK(before.zoom == after.zoom);
        CHECK(before.centerX == after.centerX); CHECK(before.centerY == after.centerY);
        for (int i = 0; i < 4; ++i) {
            CHECK(before.quat[i] == after.quat[i]); CHECK(before.lrtb[i] == after.lrtb[i]);
        }
        for (int i = 0; i < 3; ++i) {
            CHECK(before.eye[i] == after.eye[i]); CHECK(before.view[i] == after.view[i]);
            CHECK(before.up[i] == after.up[i]); CHECK(before.origin[i] == after.origin[i]);
        }
    };
    REQUIRE(!fixture.project.display); const auto* modelDisplay = model.display;
    refused([&] { engine::displayShow(fixture.project, &model); });
    refused([&] { engine::displayRefreshScene(fixture.project, &model); });
    refused([&] { mst::g_loaderSlots.showModel(fixture.project, &model); });
    CHECK(!fixture.project.display); CHECK(model.display == modelDisplay);
    Project existingDisplayProject;
    engine::GuiModel displayed(nullptr, 3);
    engine::displayShow(existingDisplayProject, &displayed);
    REQUIRE(existingDisplayProject.display);
    const auto displayOwner = existingDisplayProject.display;
    const auto beforeCamera = engine::displayCamera(existingDisplayProject);
    refused([&] { engine::displayShow(existingDisplayProject, &model); });
    refused([&] { engine::displayRefreshScene(existingDisplayProject, &model); });
    CHECK(existingDisplayProject.display == displayOwner);
    CHECK(engine::displayMain(*displayOwner) == &displayed); CHECK(model.display == modelDisplay);
    CHECK(displayed.display == displayOwner.get());
    const auto afterCamera = engine::displayCamera(existingDisplayProject);
    checkSameCamera(beforeCamera, afterCamera);
    const auto outgoingCamera = engine::displayCamera(outgoingDisplayProject);
    refused([&] { engine::displayShow(outgoingDisplayProject, nullptr); });
    refused([&] { engine::displayRefreshScene(outgoingDisplayProject, nullptr); });
    refused([&] { engine::displayResetView(outgoingDisplayProject); });
    CHECK(outgoingDisplayProject.display == outgoingDisplay);
    CHECK(engine::displayMain(*outgoingDisplay) == &model); CHECK(model.display == outgoingDisplay.get());
    checkSameCamera(outgoingCamera, engine::displayCamera(outgoingDisplayProject));

    // This compound caller constructs legitimately before its main-control query
    // refuses. Keep it after the pure-refusal timer/index-table snapshots above:
    // new-model initialization may change globals; it must not change the parked main.
    auto& outgoingModels = engine::displayModels(*outgoingDisplay);
    REQUIRE(!outgoingModels.second.get()); REQUIRE(!outgoingModels.secondOwned);
    const auto* parkedStep = model.step.get();
    std::vector<unsigned char> parkedObject(sizeof *firstObject);
    std::memcpy(parkedObject.data(), firstObject, parkedObject.size());
    const auto firstCurrent = firstVariables.body(), secondCurrent = secondVariables.body();
    const auto firstCount = firstVariables.use_count(), secondCount = secondVariables.use_count();
    std::int32_t defaultGeotyp = 0;
    {
        engine::GuiModel ordinary(nullptr, step.dimension);
        REQUIRE(ordinary.simCtl());
        defaultGeotyp = *engine::at<std::int32_t>(&ordinary.step->sim_ctrl, 0x100);
    }
    REQUIRE(defaultGeotyp != 173);
    bool secondaryRefused = false;
    try { (void)engine::displaySecond(*outgoingDisplay); }
    catch (const Error& error) {
        secondaryRefused = true;
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        CHECK(std::string(error.what()) == "geometry query is parked before unrecovered reader continuation");
    }
    CHECK(secondaryRefused);
    auto* partialSecond = outgoingModels.second.get(); REQUIRE(partialSecond);
    CHECK(outgoingModels.secondOwned.get() == partialSecond);
    CHECK(partialSecond->display == outgoingDisplay.get());
    CHECK_EQ(partialSecond->dimension(), step.dimension);
    REQUIRE(partialSecond->simCtl());
    CHECK_EQ(*engine::at<std::int32_t>(&partialSecond->step->sim_ctrl, 0x100), defaultGeotyp);
    CHECK(!outgoingModels.secondShown && !outgoingModels.keepSecond);
    engine::GuiModelGuard partialGuard(partialSecond);
    // The still-parked main would throw again if the reuse branch queried it.
    CHECK(engine::displaySecond(*outgoingDisplay) == partialSecond);
    CHECK(outgoingModels.secondOwned.get() == partialSecond);
    refused([&] { (void)model.simCtl(); });
    engine::displayDropSecond(*outgoingDisplay);
    CHECK(!partialGuard.get()); CHECK(!outgoingModels.second.get()); CHECK(!outgoingModels.secondOwned);
    CHECK(model.step.get() == parkedStep);
    CHECK(std::memcmp(beforeEntries.data(), model.step.get(), beforeEntries.size()) == 0);
    CHECK(std::memcmp(parkedObject.data(), firstObject, parkedObject.size()) == 0);
    CHECK(firstVariables.body() == firstCurrent); CHECK(secondVariables.body() == secondCurrent);
    CHECK(firstVariables.use_count() == firstCount); CHECK(secondVariables.use_count() == secondCount);
    CHECK(std::memcmp(tableBefore.data(), model.tableF8, sizeof model.tableF8) == 0);
    CHECK(std::memcmp(tableDoublesBefore.data(), model.tableF8d, sizeof model.tableF8d) == 0);
    CHECK(std::memcmp(tableBoxBefore.data(), model.tableF8box, sizeof model.tableF8box) == 0);
    CHECK(model.state100 == stateBefore); CHECK(model.pdie() == pdieBefore);
    CHECK(model.viewer == viewerBefore); CHECK(model.viewerMirrorPlanes == mirrorStateBeforeEntries);
    CHECK(model.pendingPickingConstruction() == pickingBefore);
    CHECK(engine::displayMain(*outgoingDisplay) == &model); CHECK(model.display == outgoingDisplay.get());
    checkSameCamera(outgoingCamera, engine::displayCamera(outgoingDisplayProject));
    pollWithoutEffects();

    // These external fixture changes cannot start a different cached call or
    // repair/reselect process-global table state on a parked retry.
    step.step_version = 11000; step.u26c4[0x0b] = 1; def::model::g_index_tables_dim = 2;
    pollWithoutEffects();
    auto originalViewer = model.viewer;
    engine::GuiModel foreign(nullptr, 3); geometryLine(foreign, 3.0);
    const auto foreignViewer = foreign.viewer;
    const auto foreignOptions = engine::viewerDisplayOptionSnapshot(foreign, 0);
    REQUIRE(foreignOptions);
    model.viewer = foreignViewer;
    pollWithoutEffects();
    const auto afterForeignOptions = engine::viewerDisplayOptionSnapshot(foreign, 0);
    REQUIRE(afterForeignOptions);
    CHECK(afterForeignOptions->flags == foreignOptions->flags); CHECK(afterForeignOptions->mode == foreignOptions->mode);
    CHECK(engine::viewerItemCount(foreign) == 1); CHECK(foreign.viewer == foreignViewer);
    model.viewer.reset(); pollWithoutEffects(); CHECK(!model.viewer);
    model.viewer = originalViewer;

    // The cursor owns no native DB lease. Even lost provenance cannot erase
    // the model's barrier and fall through to the current low/cached branch.
    std::weak_ptr<engine::DbHandle> dbLifetime = fixture.project.dataDb;
    REQUIRE(fixture.project.dataDb.use_count() == 1);
    fixture.project.dataDb.reset(); CHECK(dbLifetime.expired());
    pollWithoutEffects();
    const auto* stepIdentity = model.step.get();
    refused([&] { engine::viewerClear(model); });
    refused([&] { model.reset(); });
    CHECK(model.step.get() == stepIdentity); CHECK(step.u1378 == objectCount);
    CHECK(engine::viewerItemCount(model) == 2);
    pollWithoutEffects();
    checkGeometryGetterArguments(trace, step.u15f8);
}

int main()
{
    return testkit::run_all();
}
