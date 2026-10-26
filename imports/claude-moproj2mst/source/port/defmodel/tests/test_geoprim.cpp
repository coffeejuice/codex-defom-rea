// Tier 1: _DEF_GEOPRIM ctor, reset, revolve/extrude setup records, profile and int-list arrays,
// the section vector (packet p9-02).
// ctest-labels: p9-02 geoprim
//
// Golden values come from the listing: the ctor stores of 1402b7ad0 with the constants read from
// the image (DAT_140541420 = 0x3f3a36e2eb1c432d, DAT_140541428 = 0x405e000000000000,
// DAT_140541418 = 0x3fd3333333333333, DAT_140541410 = 0x4014ee978d4fdf3b, DAT_140522fc8 =
// 0xbff0000000000000, DAT_14051f1e8 = 0.001), the record inits 1402e3880 / 1402e2c00 and the
// argument routing of 1402b8980 / 1402b8520. The OBJ_MESH2D accessors 1403de680 / 1403de6e0 /
// 140220440 / 140220460 (other packets) are recorders here.
#include <defmodel/geoprim/def_geoprim.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

using namespace def::model;
using namespace def::model::geoprim;

namespace {

std::vector<std::string> g_log;
double g_xy[64];
std::int32_t g_conn[64];
std::vector<std::string> g_deleted;

void install_hooks()
{
    Ext& e = ext();
    e.mesh_alloc_nodes = [](OBJ_MESH2D*, const std::int32_t* nn) { g_log.push_back("nodes " + std::to_string(*nn)); };
    e.mesh_alloc_elems = [](OBJ_MESH2D*, const std::int32_t* ne, const std::int32_t* nper) {
        g_log.push_back("elems " + std::to_string(*ne) + "x" + std::to_string(*nper));
    };
    e.mesh_coords = [](OBJ_MESH2D*) -> double* { g_log.push_back("coords"); return g_xy; };
    e.mesh_conn = [](OBJ_MESH2D*) -> std::int32_t* { g_log.push_back("conn"); return g_conn; };
    e.obj_delete = [](LLOBJ_DATA*) { g_deleted.push_back("obj"); };
    e.model_delete = [](DB_STEP_DATA*) { g_deleted.push_back("model"); };
}

std::uint64_t bits(double d) { return std::bit_cast<std::uint64_t>(d); }

} // namespace

TEST_CASE(ctor_defaults)
{
    install_hooks();
    DEF_GEOPRIM* p = new DEF_GEOPRIM();
    CHECK_EQ(std::string(p->key_name), std::string("DEF_GEOPRIM.KEY"));
    CHECK_EQ(p->u1b30[0], '\0');
    CHECK_EQ(p->kind, 0);
    CHECK_EQ(p->mode_geo, 0);
    CHECK_EQ(p->mode_mesh, 0);
    CHECK(p->extrude == nullptr && p->revolve == nullptr && p->u2338 == nullptr);
    CHECK_EQ(p->sections, 0x12);
    CHECK_BITS(p->u2388, 1.0);
    CHECK_BITS(p->u2418, 1.0);
    CHECK_EQ(bits(p->u2448), 0xbff0000000000000ull);
    CHECK_EQ(bits(p->u2450), 0xbff0000000000000ull);
    CHECK_EQ(p->u24bc, 3);
    CHECK_EQ(bits(p->u24c0), 0x405e000000000000ull);
    CHECK_EQ(bits(p->u24d0), 0x405e000000000000ull);
    CHECK_EQ(bits(p->u1718), 0x3f3a36e2eb1c432dull);
    CHECK_BITS(p->u2500, 0.001);
    CHECK_EQ(bits(p->u2508), 0x3fd3333333333333ull);
    CHECK_EQ(bits(p->u2510), 0x4014ee978d4fdf3bull);
    CHECK_EQ(p->profile.n, 0);
    CHECK(p->profile.pts == nullptr);
    CHECK_EQ(p->int_lists.n, 0);
    CHECK(p->h24e0 != nullptr && p->h24e0->refs == 1 && p->h24e0->data == nullptr);
    CHECK(p->h24f0 != nullptr && p->h24f0->refs == 1);
    CHECK_EQ(p->v2490.size(), std::size_t{0});
    CHECK(p->l2518.empty());
    CHECK(p->ctx == nullptr && p->obj == nullptr && p->model == nullptr);
    delete p;   // 1402f0620
}

TEST_CASE(revolve_setup)
{
    install_hooks();
    g_log.clear();
    DEF_GEOPRIM p;
    const double start = 15.0, ang = 90.0;
    const double dir[3] = {0.0, 0.0, 2.0};   // copied raw (140239800 is a copy)
    const double pt[3] = {1.0, 2.0, 3.0};
    const std::int32_t sec = 6, npts = 3, nn = 2, ne = 1, nlist = 2;
    const double prof[6] = {0.5, 1.0, 2.0, 1.0, 2.0, 3.0};
    const double xy[4] = {7.0, 8.0, 9.0, 10.0};
    const std::int32_t conn[4] = {1, 2, 2, 1};
    const std::int32_t list[2] = {11, 12};
    p.setup_revolve(1, 0, &start, &ang, dir, pt, &sec, &npts, prof, &nn, xy, &ne, conn, &nlist, list);
    CHECK_EQ(p.kind, 2);
    CHECK_EQ(p.mode_geo, 1);
    CHECK_EQ(p.mode_mesh, 0);
    REQUIRE(p.revolve != nullptr);
    CHECK(p.extrude == nullptr);
    CHECK_BITS(p.revolve->start, 15.0);
    CHECK_BITS(p.revolve->angle, 90.0);
    CHECK_BITS(p.revolve->axis_dir[2], 2.0);
    CHECK_BITS(p.revolve->axis_pt[0], 1.0);
    CHECK_BITS(p.revolve->axis_pt[2], 3.0);
    CHECK_EQ(p.revolve->sections, 6);
    CHECK_EQ(p.revolve->axis_kind, 0);
    CHECK_BITS(p.angle, 90.0);     // 1402dd0d0
    CHECK_EQ(p.sections, 6);
    CHECK_EQ(p.profile.n, 3);      // 1-based: slot 0 unused
    CHECK_BITS(p.profile.pts[2], 0.5);
    CHECK_BITS(p.profile.pts[3], 1.0);
    CHECK_BITS(p.profile.pts[6], 2.0);
    CHECK_BITS(p.profile.pts[7], 3.0);
    CHECK_EQ(p.int_lists.n, 1);
    CHECK_EQ(p.int_lists.items[0].n, 2);
    CHECK_EQ(p.int_lists.items[0].data[1], 12);
    const std::vector<std::string> want = {"nodes 2", "elems 1x4", "coords", "conn"};
    CHECK(g_log == want);
    CHECK_BITS(g_xy[3], 10.0);
    CHECK_EQ(g_conn[3], 1);

    // second setup: the record is reused and re-initialised (1402e3880)
    RevolveRec* const r = p.revolve;
    const std::int32_t zero = 0;
    p.setup_revolve(0, 2, &start, &ang, dir, pt, &sec, &zero, prof, &zero, xy, &zero, conn, &zero, list);
    CHECK(p.revolve == r);
    CHECK_EQ(p.mode_mesh, 2);
    CHECK_EQ(p.profile.n, 0);
    CHECK(p.profile.pts != nullptr);   // n + 1 = 1 > 0: one slot is allocated
    CHECK_EQ(p.int_lists.items[0].n, 0);
    r->init();
    CHECK_EQ(r->sections, 0x24);
    CHECK_BITS(r->axis_dir[2], 1.0);
    CHECK_BITS(r->axis_pt[2], 0.0);
}

TEST_CASE(extrude_setup)
{
    install_hooks();
    DEF_GEOPRIM p;
    const double len = 5.0;
    const std::int32_t cnt = 2, npts = 1, zero = 0, i23cc = 7;
    const double prof[2] = {4.0, 5.0};
    const double xy[2] = {0, 0};
    const std::int32_t conn[4] = {0, 0, 0, 0};
    const char b8 = 1, b9 = 0;
    const double d2388 = 2.5, a = 3.5, b = 4.5, c = 6.0, d = 7.0;
    p.setup_extrude(1, 0, &len, &cnt, &npts, prof, &zero, xy, &zero, conn, &zero, conn, &b8, &b9, &i23cc,
                    &d2388, &a, &b, &c, &d, nullptr);
    CHECK_EQ(p.kind, 1);
    REQUIRE(p.extrude != nullptr);
    CHECK_BITS(p.extrude->length, 5.0);
    CHECK_EQ(p.extrude->count, 2);
    CHECK_BITS(p.extrude->dir[0], 0.0);   // default (0, 0, 1) when dir is null
    CHECK_BITS(p.extrude->dir[2], 1.0);
    CHECK_BITS(p.u2360, 5.0);
    CHECK_BITS(p.u2368, 0.0);
    CHECK_EQ(p.sections, 2);
    CHECK_EQ(p.u23c8, '\1');
    CHECK_EQ(p.u23cc, 7);
    CHECK_EQ(p.extrude->u830, 7);
    CHECK_BITS(p.u2388, 2.5);
    CHECK_EQ(p.v2490.size(), std::size_t{1});
    CHECK_BITS(p.v2490[0], 3.5);
    CHECK_BITS(p.v2498[0], 4.5);
    CHECK_BITS(p.u2448, 6.0);
    CHECK_BITS(p.u2450, 7.0);
    CHECK_EQ(p.extrude->sections.size(), std::size_t{100});
    CHECK(p.extrude->sections[99] == nullptr);
    CHECK_EQ(p.profile.n, 1);
    CHECK_BITS(p.profile.pts[3], 5.0);

    // second setup: record reused, element 0 overwritten (no push), dir taken
    ExtrudeRec* const r = p.extrude;
    const double dir[3] = {1.0, 0.0, 0.0};
    const double a2 = 9.0;
    p.setup_extrude(0, 2, &len, &cnt, &npts, prof, &zero, xy, &zero, conn, &zero, conn, &b8, &b9, &i23cc,
                    &d2388, &a2, &b, &c, &d, dir);
    CHECK(p.extrude == r);
    CHECK_EQ(p.v2490.size(), std::size_t{1});
    CHECK_BITS(p.v2490[0], 9.0);
    CHECK_BITS(p.extrude->dir[0], 1.0);
    CHECK_BITS(p.extrude->dir[2], 0.0);
    CHECK_EQ(p.extrude->sections.size(), std::size_t{100});
}

TEST_CASE(reset_deletes_owned)
{
    install_hooks();
    g_deleted.clear();
    DEF_GEOPRIM p;
    p.revolve = new RevolveRec();
    p.u2340 = reinterpret_cast<Rec2340*>(p.revolve);   // aliasing: deleted once, as u2340
    p.extrude = new ExtrudeRec();
    p.extrude->sections.mutable_at(3) = new RevolveRec();
    p.owns_obj = 1;
    p.obj = reinterpret_cast<LLOBJ_DATA*>(&p);
    p.owns_model = 0;
    p.model = reinterpret_cast<DB_STEP_DATA*>(&p);
    p.kind = 2;
    p.mode_geo = 1;
    p.l2518.push_back(1.0);
    p.reset();
    CHECK_EQ(p.kind, 0);
    CHECK_EQ(p.mode_geo, 0);
    CHECK(g_deleted == std::vector<std::string>{"obj"});
    CHECK(p.l2518.empty());
    // the pointers are left as they were (1402b81a0 does not clear them)
    p.revolve = nullptr;
    p.u2340 = nullptr;
    p.extrude = nullptr;
    p.owns_obj = 0;
}

TEST_CASE(arrays)
{
    ProfileArray a;
    a.init();
    const std::int32_t m1 = -1;
    a.resize(&m1);
    CHECK_EQ(a.n, -1);
    CHECK(a.pts == nullptr);
    a.clear();

    IntArrayList l;
    l.init();
    const std::int32_t two = 2, k0 = 0, k3 = 3, k2 = 2, n = 1, v = 42;
    l.resize(&two);
    l.assign(&k0, &n, &v);   // ignored: k < 1
    l.assign(&k3, &n, &v);   // ignored: k > n
    CHECK_EQ(l.items[0].n, 0);
    CHECK_EQ(l.items[1].n, 0);
    l.assign(&k2, &n, &v);
    CHECK_EQ(l.items[1].n, 1);
    CHECK_EQ(l.items[1].data[0], 42);
    l.clear();
    CHECK_EQ(l.n, 0);
    CHECK(l.items == nullptr);
}

TEST_CASE(section_vector_growth)
{
    // 1402f0ba0: capacity size + 1 + size / 2 when full: 1, 2, 4, 7, 11
    CowVector<RevolveRec*> v;
    std::vector<std::size_t> caps;
    for (int i = 0; i < 11; ++i) {
        v.push_back(nullptr);
        const auto* b = v.body();
        const std::size_t cap = static_cast<std::size_t>(b->end - b->first);
        if (caps.empty() || caps.back() != cap) caps.push_back(cap);
    }
    CHECK(caps == (std::vector<std::size_t>{1, 2, 4, 7, 11}));
    // shared body: push_back detaches first
    CowVector<RevolveRec*> w(v);
    w.push_back(nullptr);
    CHECK_EQ(v.size(), std::size_t{11});
    CHECK_EQ(w.size(), std::size_t{12});
}

int main() { return testkit::run_all(); }
