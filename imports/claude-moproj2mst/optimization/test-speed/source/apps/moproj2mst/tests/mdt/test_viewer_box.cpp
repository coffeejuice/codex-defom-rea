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

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

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

def::model::OBJ_GEO2D* geometryLine(engine::GuiModel& m, double halfExtent)
{
    auto* object = m.objectWrap(1, true, false);
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

TEST_CASE(empty_geometry_mesh_is_known_but_populated_mesh_rect_stays_refused)
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
    auto* nodes = reinterpret_cast<def::model::Cls_14017ef20*>(geometry->mesh->s008);
    auto* elements = reinterpret_cast<def::model::Cls_140182880*>(geometry->mesh->s148);
    nodes->n = 1;
    elements->n = 1;
    engine::viewerRefresh(m);
    bool refused = false;
    try {
        engine::viewerSceneBox(m, valid, mn, mx);
    } catch (const Error& e) {
        refused = e.code == M2M_E_UNSUPPORTED;
    }
    CHECK(refused);
}

int main()
{
    return testkit::run_all();
}
