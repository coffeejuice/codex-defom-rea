// Tier 1 for packet p4-03 (GENDB object / material / inter-material checks).
//
// The objects are zeroed storage of the binary's sizes (DB_STEP_DATA 0x3408, LLOBJ_DATA 0x3f28,
// Material 0x2218) with the members the checks read set by offset; calls into packets that have not
// landed go through the ext hook. Expected texts and values are worked out by hand from the
// decompile (out/p403/p403_all.c) and the disassembly; the arithmetic is shown next to each value.
// ctest-labels: p4-03 objcheck
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <io.h>
#include <process.h>

#include "../src/objcheck/ext.hpp"

using namespace def::dbio::objcheck;

namespace {

struct Mem {
    std::vector<std::uint64_t> v;
    explicit Mem(std::size_t bytes) : v((bytes + 7) / 8, 0) {}
    char* p() { return reinterpret_cast<char*>(v.data()); }
};

template <class T>
T& at(void* p, std::int64_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}

// Captures what the checks print on stdout.
struct Capture {
    std::filesystem::path path;
    int saved = -1;
    FILE* f = nullptr;
    Capture()
    {
        path = std::filesystem::temp_directory_path() /
               ("p403_" + std::to_string(::_getpid()) + "_" + std::to_string(std::rand()) + ".txt");
        std::fflush(stdout);
        saved = ::_dup(::_fileno(stdout));
        f = std::fopen(path.string().c_str(), "wb");
        ::_dup2(::_fileno(f), ::_fileno(stdout));
    }
    std::string done()
    {
        std::fflush(stdout);
        ::_dup2(saved, ::_fileno(stdout));
        ::_close(saved);
        std::fclose(f);
        std::ifstream in(path, std::ios::binary);
        std::stringstream ss;
        ss << in.rdbuf();
        in.close();
        std::error_code ec;
        std::filesystem::remove(path, ec);
        return ss.str();
    }
};

// Hook results by address; anything else runs the default body.
struct HookTable {
    std::vector<std::pair<std::uint64_t, std::uint64_t>> ret;
    std::vector<std::uint64_t> calls;
};
HookTable* g_tab = nullptr;

bool hook(std::uint64_t addr, const std::uint64_t*, int, std::uint64_t* r)
{
    g_tab->calls.push_back(addr);
    for (auto& [a, v] : g_tab->ret) {
        if (a == addr) {
            *r = v;
            return true;
        }
    }
    return false;
}

struct HookGuard {
    HookTable t;
    HookGuard()
    {
        g_tab = &t;
        ext::g_hook = &hook;
    }
    ~HookGuard()
    {
        ext::g_hook = nullptr;
        g_tab = nullptr;
    }
    void set(std::uint64_t a, std::uint64_t v) { t.ret.emplace_back(a, v); }
};

constexpr double kPiF = 3.141590118408203; // DAT_140bb14e0

} // namespace

// ---- Fortran kernels ---------------------------------------------------------------------------------

TEST_CASE(heron_right_triangle)
{
    // 3-4-5 triangle: s = 6, area = sqrt(6*3*2*1) = 6 exactly.
    const double p[9] = {0, 0, 0, 3, 0, 0, 0, 4, 0};
    double a = -1;
    triangle_area(p, &a);
    CHECK_BITS(a, 6.0);
    // degenerate (collinear): product <= 0 -> 0
    const double q[9] = {0, 0, 0, 1, 0, 0, 2, 0, 0};
    triangle_area(q, &a);
    CHECK_BITS(a, 0.0);
}

TEST_CASE(cross_and_normal)
{
    const double a[3] = {2, 0, 0};
    const double b[3] = {0, 3, 0};
    double c[3];
    double len;
    cross_normalize(a, b, c, &len);   // (0, 0, 6), |c| = 6, normalised (0, 0, 1)
    CHECK_BITS(len, 6.0);
    CHECK_BITS(c[0], 0.0);
    CHECK_BITS(c[1], 0.0);
    CHECK_BITS(c[2], 1.0);
    const double z[3] = {0, 0, 0};
    cross_normalize(a, z, c, &len);   // |c| = 0 <= 1e-16: len set to 0, c left as computed (0)
    CHECK_BITS(len, 0.0);
    const double tri[9] = {0, 0, 0, 0, 1, 0, 1, 0, 0};
    double n[3];
    triangle_normal(tri, n);          // (0,1,0) x (1,0,0) = (0, 0, -1)
    CHECK_BITS(n[2], -1.0);
}

TEST_CASE(tetrahedron_volume)
{
    // Right tetrahedron, outward facets (stride 3). Only the slanted face does not pass through
    // the origin: p1 = (1,0,0), n = (1,1,1)/sqrt(3), area = sqrt(3)/2 -> V = 1/6.
    const double xyz[12] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
    const std::int32_t fac[12] = {1, 3, 2, 1, 2, 4, 1, 4, 3, 2, 3, 4};
    std::int32_t np = 4, st = 3, nf = 4;
    double v = -1;
    surface_volume(xyz, &np, fac, &st, &nf, &v);
    CHECK(std::fabs(v - 1.0 / 6.0) < 1e-15);
}

TEST_CASE(cube_volume_quads)
{
    // Unit cube from 6 outward quads (stride 4, node 1 != node 4): each split into 4 triangles.
    const double xyz[24] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1};
    const std::int32_t fac[24] = {1, 4, 3, 2, 5, 6, 7, 8, 1, 2, 6, 5, 2, 3, 7, 6, 3, 4, 8, 7, 4, 1, 5, 8};
    std::int32_t np = 8, st = 4, nf = 6;
    double v = -1;
    surface_volume(xyz, &np, fac, &st, &nf, &v);
    CHECK(std::fabs(v - 1.0) < 1e-14);
    // Same facets read as triangles (node 1 == node 4 is not the case, so force stride 3 by
    // repeating: a quad with node 4 == node 1 is one triangle).
    const std::int32_t tri[8] = {1, 2, 3, 1, 1, 3, 4, 1};
    std::int32_t n2 = 2;
    surface_volume(xyz, &np, tri, &st, &n2, &v);   // bottom face z = 0: p1 . n = 0
    CHECK_BITS(v, 0.0);
}

TEST_CASE(quad_revolution)
{
    // Unit square r 1..2, z 0..1 (counter-clockwise): area 1, volume 2*pi*1.5 with pi = 3.14159f.
    const double p[8] = {1, 0, 2, 0, 2, 1, 1, 1};
    double v = 0, a = 0;
    quad_area_volume_1(p, &v, &a);
    CHECK(std::fabs(a - 1.0) < 1e-15);
    CHECK(std::fabs(v - 2.0 * kPiF * 1.5) < 1e-13);
    const double xy[8] = {1, 0, 2, 0, 2, 1, 1, 1};
    const std::int32_t el[4] = {1, 2, 3, 4};
    std::int32_t np = 4, ne = 1, ierr = 0;
    double vv = -1, aa = -1;
    quad_area_volume(xy, el, &np, &ne, &vv, &aa, &ierr);
    CHECK_EQ(ierr, 1);
    CHECK_BITS(vv, v);
    CHECK_BITS(aa, a);
    // numnp < 4: nothing written
    np = 3;
    vv = -7;
    quad_area_volume(xy, el, &np, &ne, &vv, &aa, &ierr);
    CHECK_BITS(vv, -7.0);
}

// ---- helpers -------------------------------------------------------------------------------------------

TEST_CASE(edge_index)
{
    // two boundaries with 4 and 3 edges (records 0x30 bytes, count at +0)
    Mem bnd(0x60);
    at<std::int32_t>(bnd.p(), 0) = 4;
    at<std::int32_t>(bnd.p(), 0x30) = 3;
    std::int32_t nb = 2;
    // first = 1, next = 1 - 1 + 4 = 4: edges 1..3 -> edge - 1; then first = 4, next = 6: edges 4,5
    // -> edge - 1 + 2; edge 6 and beyond -> -1
    std::int32_t e = 1;
    CHECK_EQ(boundary_edge_index(&e, &nb, bnd.p()), 0);
    e = 3;
    CHECK_EQ(boundary_edge_index(&e, &nb, bnd.p()), 2);
    e = 4;
    CHECK_EQ(boundary_edge_index(&e, &nb, bnd.p()), 5);
    e = 6;
    CHECK_EQ(boundary_edge_index(&e, &nb, bnd.p()), -1);
}

TEST_CASE(dynary_zero_tests)
{
    Mem t(0x78);
    CHECK(dynary_all_zero(t.p()));       // ndim 0, scalar 0
    CHECK(dynary_values_zero(t.p()));
    at<double>(t.p(), 8) = 2.0;
    CHECK(!dynary_all_zero(t.p()));
    // 1 axis of 2 points (0, 5), values (0, 0): all_zero false (axis), values_zero true
    at<std::int32_t>(t.p(), 0) = 0x10;   // type
    at<std::int32_t>(t.p(), 4) = 1;      // ndim
    at<std::int32_t>(t.p(), 0x1c) = 2;   // n[0]
    at<std::int32_t>(t.p(), 0x40) = 1;   // ncomp
    double ax[2] = {0, 5};
    double val[2] = {0, 0};
    at<double*>(t.p(), 0x28) = ax;
    at<double*>(t.p(), 0x48) = val;
    CHECK(!dynary_all_zero(t.p()));
    CHECK(dynary_values_zero(t.p()));
    val[1] = 1e-300;
    CHECK(!dynary_values_zero(t.p()));
}

TEST_CASE(plane_list)
{
    using def::model::linked::Cls_140257340List;
    Cls_140257340List l;
    l.add(3);
    auto* b = l.add(7);
    l.add(2);
    CHECK_EQ(plane_list_max_id(&l), 7);   // max(count 3, ids 3, 7, 2)
    CHECK(plane_direction_zero(b));
    b->direction[1] = 2e-6;
    CHECK(!plane_direction_zero(b));
    CHECK(plane_list_remove(&l, b));
    CHECK_EQ(l.count, 2);
    CHECK_EQ(l.head->next->id, 2);
    CHECK(l.tail->id == 2);
    def::model::linked::Cls_140257340 x;
    CHECK(!plane_list_remove(&l, &x));   // not in the list
}

TEST_CASE(stroke_vector)
{
    // movement block (Cls_14018b220): direction at +0x50 = (0, 3, 4), owner at +0xa80 (3D step)
    Mem step(0x3408);
    at<std::int32_t>(step.p(), 0x1c) = 3;
    Mem blk(0xa88);
    at<double>(blk.p(), 0x50) = 0;
    at<double>(blk.p(), 0x58) = 3;
    at<double>(blk.p(), 0x60) = 4;
    at<void*>(blk.p(), 0xa80) = step.p();
    double out[3];
    press_stroke_vector(blk.p(), 2.0, 12.0, out);   // unit (0, .6, .8) * (12 - 2)
    CHECK(std::fabs(out[1] - 6.0) < 1e-14 && std::fabs(out[2] - 8.0) < 1e-14 && out[0] == 0.0);
    press_stroke_vector(blk.p(), 2.0, 1e-7, out);   // |stroke| <= 1e-6: zero
    CHECK(out[0] == 0.0 && out[1] == 0.0 && out[2] == 0.0);
    const double st[3] = {0, 6, 8};
    // stroke . unit(dir): direction (0,3,4) -> (0,.6,.8); 6*.6 + 8*.8 = 10
    CHECK(std::fabs(press_stroke_projection(blk.p(), st, reinterpret_cast<const double*>(blk.p() + 0x50)) - 10.0) < 1e-14);
}

TEST_CASE(inter_object_relation)
{
    Mem step(0x3408);
    Mem r1(0x398), r2(0x398);
    // relation 1: objects (1, 2), kind 2; relation 2: (3, 4), kind 2
    at<std::int32_t>(r1.p(), 0x168) = 1;
    at<std::int32_t>(r1.p(), 0x16c) = 2;
    at<std::int32_t>(r1.p(), 0x170) = 2;
    at<void*>(r1.p(), 0x390) = r2.p();
    at<std::int32_t>(r2.p(), 0x168) = 3;
    at<std::int32_t>(r2.p(), 0x16c) = 4;
    at<std::int32_t>(r2.p(), 0x170) = 2;
    at<void*>(step.p(), 0x13c8) = r1.p();
    at<std::int32_t>(step.p(), 0x13c8 + 0x10) = 2;
    auto* s = reinterpret_cast<DB_STEP_DATA*>(step.p());
    CHECK_EQ(step_inter_object_relation(s, 1, 2), 0);        // first is (a, b), kind 2
    CHECK_EQ(step_inter_object_relation(s, 2, 1), -0xca);    // (b, a) needs kind 1
    CHECK_EQ(step_inter_object_relation(s, 4, 3), -0xca);
    CHECK_EQ(step_inter_object_relation(s, 1, 5), -0xc9);    // none
}

// ---- material and inter-material checks ----------------------------------------------------------------

TEST_CASE(material_zero_warnings)
{
    Mem step(0x3408);
    Mem mat(0x2218);
    at<std::int32_t>(mat.p(), 0) = 1;        // number
    at<std::int32_t>(mat.p(), 4) = 1;        // active (1401b0620)
    at<std::int32_t>(mat.p(), 8) = 0x10;     // elastic data
    at<double>(mat.p(), 0x6e8 + 8) = 0.3;    // POISON scalar
    at<void*>(mat.p(), 0x2208) = step.p();
    // list at DB_STEP_DATA+0x1328: head, tail, count
    at<void*>(step.p(), 0x1328) = mat.p();
    at<void*>(step.p(), 0x1330) = mat.p();
    at<std::int32_t>(step.p(), 0x1338) = 1;
    std::int32_t err = 0, warn = 0;
    Capture cap;
    check_material_data(reinterpret_cast<DB_STEP_DATA*>(step.p()), nullptr, &err, &warn);
    std::string s = cap.done();
    CHECK_EQ(s, std::string("INFO   : Checking material properties\n"
                            "WARNING: YOUNG of Material      1 is ZERO\n"
                            "WARNING: EXPAND of Material      1 is ZERO\n"
                            "WARNING: EXPAND(Ref. T) of Material      1 is ZERO\n"));
    CHECK_EQ(err, 0);
    CHECK_EQ(warn, 1);
}

TEST_CASE(inter_material_none)
{
    Mem step(0x3408);
    at<void*>(step.p(), 0x1358 + 0x18) = step.p();   // list owner
    at<std::int32_t>(step.p(), 0x85c) = 1;           // phases in the model
    std::int32_t err = 0, warn = 0;
    Capture cap;
    check_inter_material_data(reinterpret_cast<DB_STEP_DATA*>(step.p()), nullptr, &err, &warn);
    CHECK_EQ(cap.done(), std::string("INFO   : Checking inter-material data\n"
                                     "WARNING: No Inter-Material Relation Defined\n"));
    CHECK_EQ(warn, 1);
}

// ---- object checks ------------------------------------------------------------------------------------------

TEST_CASE(object_list_all_rigid)
{
    HookGuard h;
    h.set(0x140167ae0, 1);   // only-deformation mode: the rest of 140167e90 is skipped
    Mem step(0x3408);
    Mem obj(0x3f28);
    at<std::int32_t>(step.p(), 0x1c) = 3;
    at<std::int32_t>(step.p(), 0x750 + 0x108) = 1;   // deformation on
    at<void*>(step.p(), 0x1380) = obj.p();            // object list head, tail, count, owner
    at<void*>(step.p(), 0x1388) = obj.p();
    at<std::int32_t>(step.p(), 0x1390) = 1;
    at<void*>(step.p(), 0x1398) = step.p();
    at<std::int32_t>(obj.p(), 8) = 3;                 // object number, type 0 (rigid)
    at<void*>(obj.p(), 0x3a98) = step.p();
    at<void*>(obj.p(), 0x2160 + 0xa80) = step.p();    // movement block owner
    std::int32_t err = 0, warn = 0;
    Capture cap;
    check_object_data(reinterpret_cast<DB_STEP_DATA*>(step.p()), nullptr, &err, &warn);
    CHECK_EQ(cap.done(), std::string("INFO   : Checking object data\n"
                                     "ERROR  : The rigid object    3 has no \n"
                                     "         geometry and deformation is turned on\n"
                                     "WARNING: MOVCTL Not Defined for Any Object\n"
                                     "ERROR  : Invalid Principal Die Defined; PDIE(1) =   0\n"
                                     "ERROR  : All objects rigid and deformation on\n"));
    CHECK_EQ(err, 1);
    CHECK_EQ(warn, 0);
}

TEST_CASE(object_2d_trgvol_nbcd)
{
    HookGuard h;
    h.set(0x140165170, 0);   // no edge arrays
    h.set(0x140136a10, 0);   // no boundary functions
    h.set(0x140195b60, 0);
    h.set(0x140195b90, 0);
    h.set(0x140186da0, 0);
    h.set(0x1401fb270, 0);   // material number stays 0
    h.set(0x14017ffe0, 0);
    h.set(0x1401836c0, 0);
    Mem step(0x3408);
    Mem obj(0x3f28);
    char* s = step.p();
    char* o = obj.p();
    at<std::int32_t>(s, 0x1c) = 2;           // 2D
    at<std::int32_t>(s, 0x850) = 1;          // axisymmetric
    at<std::int32_t>(s, 0x750 + 0x108) = 1;  // deformation on
    at<std::int32_t>(s, 0x1640) = 2;
    at<std::int32_t>(s, 0x1644) = 2;
    at<std::int32_t>(o, 8) = 1;
    at<std::int32_t>(o, 0xc) = 1;            // plastic
    at<void*>(o, 0x3a98) = s;
    at<std::int32_t>(o, 0x87c) = 1;
    at<std::int32_t>(o, 0xbd8) = 1;
    at<std::int32_t>(o, 0x880) = 4;
    at<std::int32_t>(o, 0x884) = 1;
    double xy[8] = {1, 0, 2, 0, 2, 1, 1, 1};
    std::int32_t el[4] = {1, 2, 3, 4};
    double temp[4] = {20, 20, 20, 20};
    double rho[1] = {1.0};
    std::int32_t bcc[8] = {0, 0, -5, 0, 0, 0, 0, 0};   // node 2, dof 1: object 5 (missing)
    at<double*>(o, 0x8a8) = xy;
    at<std::int32_t*>(o, 0x9e8) = el;
    at<double*>(o, 0x8e0) = temp;
    at<double*>(o, 0x9e0 + 0x50) = rho;
    at<std::int32_t*>(o, 0x990) = bcc;
    at<double>(o, 0x1ea8) = 1.0;            // AVGSTR > LMTSTR
    at<double>(o, 0x1ea0) = 0.5;            // PENVOL 0 -> warning
    at<std::int32_t>(o, 0x2098) = 0;        // TRGVOL set
    at<double>(o, 0x2090) = 5.0;            // target volume
    at<std::uint8_t>(o, 0x3f20) = 1;
    at<std::int32_t>(o, 0x2d84) = 1;        // mesh density ok
    std::int32_t nrig = 0, err = 0, warn = 0;
    char mv = '*', p1 = '*', p2 = '*', m1 = '*', m2 = '*';
    Capture cap;
    check_object(reinterpret_cast<LLOBJ_DATA*>(o), &nrig, &mv, &p1, &p2, &m1, &m2, &err, &warn);
    // volume = 2*pi*1.5 = 9.42; |9.42 - 5| > 0.942 and the 0x3f20 flag is set -> error
    CHECK_EQ(cap.done(), std::string("WARNING: PENVOL < 10 for Object   1\n"
                                     "ERROR  : Volume compensation activated for Obj   1\n"
                                     "         but target volume is completely\n"
                                     "         different\n"
                                     "WARNING: Interference depth of Obj   1 is not set.\n"
                                     "         Excessive mesh penetration may occour\n"
                                     "Object 1 Node 2 has wrong NBCD Code(-5). Initialized.\n"));
    CHECK_EQ(err, 1);
    CHECK_EQ(warn, 1);
    CHECK_EQ(bcc[2], 0);
    CHECK_EQ(nrig, 0);
    CHECK_EQ(mv, '*');
    CHECK(std::fabs(obj_volume(reinterpret_cast<LLOBJ_DATA*>(o), 1) - 2.0 * kPiF * 1.5) < 1e-13);
    at<std::int32_t>(s, 0x850) = 2;   // plane strain: the area
    CHECK(std::fabs(obj_volume(reinterpret_cast<LLOBJ_DATA*>(o), 1) - 1.0) < 1e-15);
    at<std::int32_t>(s, 0x850) = 4;   // plane stress: 0
    CHECK_BITS(obj_volume(reinterpret_cast<LLOBJ_DATA*>(o), 1), 0.0);
}

TEST_CASE(transformation_flag)
{
    HookGuard h;
    Mem step(0x3408);
    h.set(0x14029bb50, 2);
    CHECK(check_transformation(reinterpret_cast<DB_STEP_DATA*>(step.p())));
    CHECK_EQ(at<std::int32_t>(step.p(), 0x898), 1);
    h.t.ret.clear();
    h.set(0x14029bb50, 5);
    CHECK(check_transformation(reinterpret_cast<DB_STEP_DATA*>(step.p())));
    CHECK_EQ(at<std::int32_t>(step.p(), 0x898), 0);
    at<std::int32_t>(step.p(), 0x864) = 1;
    h.t.ret.clear();
    h.set(0x14029bb50, static_cast<std::uint64_t>(-99));
    CHECK(!check_transformation(reinterpret_cast<DB_STEP_DATA*>(step.p())));
}

TEST_CASE(edge_bc_cleanup)
{
    HookGuard h;
    h.set(0x140165170, 3);
    Mem obj(0x3f28);
    std::int32_t code[4][3] = {{0, 1, 0}, {0, 0, 0}, {5, 0, 0}, {0, 0, 0}};
    std::int32_t fn[4][3] = {{2, 2, 0}, {1, 0, 0}, {7, 7, 7}, {0, 0, 3}};
    const std::int64_t offs[4][2] = {{0xae8, 0xaf0}, {0xb00, 0xb08}, {0xb18, 0xb20}, {0xb30, 0xb38}};
    for (int k = 0; k < 4; ++k) {
        at<std::int32_t*>(obj.p(), offs[k][0]) = code[k];
        at<std::int32_t*>(obj.p(), offs[k][1]) = fn[k];
    }
    std::int32_t a, b, c, d;
    CHECK(obj_clean_edge_bcs(reinterpret_cast<LLOBJ_DATA*>(obj.p()), &a, &b, &c, &d));
    CHECK_EQ(a, 1);   // edge 0 (code 0, fn 2); edge 2 has fn 0
    CHECK_EQ(b, 1);
    CHECK_EQ(c, 2);   // edges 1, 2
    CHECK_EQ(d, 1);
    CHECK_EQ(fn[0][0], 0);
    CHECK_EQ(fn[0][1], 2);
    CHECK_EQ(fn[2][0], 7);
}

int main() { return testkit::run_all(); }
