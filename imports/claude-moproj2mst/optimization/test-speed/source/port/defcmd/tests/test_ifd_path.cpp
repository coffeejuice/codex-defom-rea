// ctest-labels: p67-2 path
// Interference positioning, "arbitrary path" branch (OBJPOS_interference.md sections 4.1, 8).
// Tables are built by hand on real objects (DB_STEP_DATA(3), llobj_ctor::get_or_create); the
// object move 14021d0b0 (p67-1) and the kernel switch 1401a0b00 are replaced through test hooks.
// Expected doubles are computed in the test with the listing's operation order.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/ifd/ifd_internal.hpp>
#include <defcmd/ifd/path.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

#include <fcntl.h>
#include <io.h>
#include <process.h>

using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;
namespace ifd = def::cmd::ifd;
namespace path = def::cmd::ifd::path;

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}

DYNARY* table_of(LLOBJ_DATA* o) { return &at<DYNARY>(o, 0x2160 + 0x448); }

// path table of type `type` with points (t_k, p_k), ncomp comps stored per point
void set_path(LLOBJ_DATA* o, int type, const std::vector<double>& t, const std::vector<double>& p)
{
    DYNARY* d = table_of(o);
    path::dynary_reshape(d, type, static_cast<std::int32_t>(t.size()));
    for (std::size_t k = 0; k < t.size(); ++k) d->axis[0][k] = t[k];
    for (std::size_t k = 0; k < p.size(); ++k) d->values[k] = p[k];
    at<std::int32_t>(o, 0x21c8) = 6;      // movement type (mv+0x68)
    at<std::int32_t>(o, 0x2160 + 0x4c0) = 0;
}

struct Fixture {
    DB_STEP_DATA* m;
    Fixture() : m(new DB_STEP_DATA(3)) {}  // the real model ctor (p1-03); leaked like the other tests
    LLOBJ_DATA* obj(int no) { return def::model::llobj_ctor::get_or_create(m, no); }
};

double* pos(LLOBJ_DATA* o) { return &at<double>(o, 0x2160 + 0x630); }

// translate hook: record and move the path position (what 14021d0b0 does to mv+0x630 is p67-1's)
struct Moves {
    std::vector<std::pair<LLOBJ_DATA*, std::vector<double>>> calls;
};
Moves g_moves;
void record_translate(LLOBJ_DATA* o, double* v, std::int32_t* a, char* b)
{
    CHECK_EQ(*a, 0);
    CHECK_EQ(static_cast<int>(*b), 1);
    g_moves.calls.push_back({o, {v[0], v[1], v[2]}});
    double* p = pos(o);
    p[0] += v[0];
    p[1] += v[1];
    p[2] += v[2];
}

// kernel hook: scripted results per call; records direction and the mover's position
struct KCall {
    LLOBJ_DATA* x;
    double dir[3];
    double at[3];
    char flag;
};
std::vector<KCall> g_kcalls;
std::vector<std::pair<bool, double>> g_kscript;
bool scripted_kernel(DB_STEP_DATA*, LLOBJ_DATA* x, LLOBJ_DATA*, double* dir, double* tol, double* d,
                     double*, char flag)
{
    CHECK_BITS(*tol, 0.25);
    KCall c{x, {dir[0], dir[1], dir[2]}, {pos(x)[0], pos(x)[1], pos(x)[2]}, flag};
    g_kcalls.push_back(c);
    const std::size_t i = g_kcalls.size() - 1;
    if (i >= g_kscript.size()) return false;
    *d = g_kscript[i].second;
    return g_kscript[i].first;
}

void reset_hooks()
{
    g_moves.calls.clear();
    g_kcalls.clear();
    g_kscript.clear();
    ifd::ext::g_translate_hook = record_translate;
    path::g_kernel_hook = scripted_kernel;
    path::g_copy_hook = nullptr;
}

// stdout capture into a per-process temp file
struct Capture {
    std::string file;
    int saved = -1;
    Capture()
    {
        file = "ifd_path_out_" + std::to_string(_getpid()) + ".txt";
        std::fflush(stdout);
        saved = _dup(_fileno(stdout));
        std::FILE* f = std::fopen(file.c_str(), "w");
        _dup2(_fileno(f), _fileno(stdout));
        std::fclose(f);
    }
    std::string done()
    {
        std::fflush(stdout);
        _dup2(saved, _fileno(stdout));
        _close(saved);
        std::string s;
        if (std::FILE* f = std::fopen(file.c_str(), "rb")) {
            char buf[512];
            std::size_t n;
            while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) {
                for (std::size_t i = 0; i < n; ++i)
                    if (buf[i] != '') s.push_back(buf[i]);  // stdout is in text mode
            }
            std::fclose(f);
        }
        std::remove(file.c_str());
        return s;
    }
};

const double kNaN = std::numeric_limits<double>::quiet_NaN();

} // namespace

TEST_CASE(table_all_zero_14012fd80)
{
    DYNARY t;
    t.ndim = 0;
    t.value = 0.0;
    CHECK(path::table_all_zero(&t));
    t.value = -0.0;
    CHECK(path::table_all_zero(&t));
    t.value = kNaN;
    CHECK(!path::table_all_zero(&t));
    path::dynary_reshape(&t, 0x7a, 2);  // ndim 1, ncomp 4, zeros
    CHECK(path::table_all_zero(&t));
    t.values[7] = 1e-300;
    CHECK(!path::table_all_zero(&t));
    t.values[7] = 0.0;
    t.axis[0][1] = 2.0;
    CHECK(!path::table_all_zero(&t));
}

TEST_CASE(movement_on_path_14021e470)
{
    Fixture f;
    LLOBJ_DATA* o = f.obj(1);
    void* mv = &o->s2160;
    set_path(o, 0x7a, {0.0, 1.0}, {0, 0, 0, 0, 1, 0, 0, 0});
    CHECK(path::movement_on_path(mv));
    CHECK(path::movement_position(mv) == pos(o));
    at<std::int32_t>(mv, 0x68) = 4;
    CHECK(!path::movement_on_path(mv));
    CHECK(path::movement_position(mv) == &at<double>(mv, 0x170));
    at<std::int32_t>(mv, 0x68) = 6;
    set_path(o, 0x7a, {0.0, 0.0}, {0, 0, 0, 0, 0, 0, 0, 0});
    CHECK(!path::movement_on_path(mv));
}

TEST_CASE(interval_140132a60)
{
    DYNARY t;
    path::dynary_reshape(&t, 0x79, 3);
    t.axis[0][0] = 0.0;
    t.axis[0][1] = 1.0;
    t.axis[0][2] = 3.0;
    double fr = -7.0;
    CHECK_EQ(path::path_interval(&t, 2.0, &fr), 1);
    CHECK_BITS(fr, (2.0 - 1.0) / (3.0 - 1.0));
    CHECK_EQ(path::path_interval(&t, 1.0, &fr), 0);  // first interval that contains it
    CHECK_BITS(fr, 1.0);
    CHECK_EQ(path::path_interval(&t, -1.0, &fr), 0);  // below: extrapolate the first
    CHECK_BITS(fr, -1.0);
    CHECK_EQ(path::path_interval(&t, 4.0, &fr), 1);  // above: extrapolate the last
    CHECK_BITS(fr, 1.5);
    fr = -7.0;
    CHECK_EQ(path::path_interval(&t, kNaN, &fr), -1);  // NaN: loop finds nothing
    CHECK_BITS(fr, 0.0);
}

TEST_CASE(path_point_and_time_1401350e0_1401351c0)
{
    DYNARY t;
    path::dynary_reshape(&t, 0x7a, 3);
    const double tt[3] = {0.0, 0.3, 1.0};
    const double p[12] = {1.0, 2.0, 3.0, 9.0, 1.1, 2.7, -3.3, 9.0, 0.1, 0.2, 0.3, 9.0};
    for (int k = 0; k < 3; ++k) t.axis[0][k] = tt[k];
    for (int k = 0; k < 12; ++k) t.values[k] = p[k];
    double out[3] = {-1, -1, -1};
    std::int32_t k = 1;
    CHECK(path::path_point(&t, &k, out));
    CHECK_BITS(out[0], 1.1);
    CHECK_BITS(out[2], -3.3);
    const double time = 0.7;
    CHECK(path::path_at_time(&t, &time, out));
    const double f = (0.7 - 0.3) / (1.0 - 0.3);
    for (int c = 0; c < 3; ++c) CHECK_BITS(out[c], p[4 + c] + f * (p[8 + c] - p[4 + c]));
    // 2D (type 0x79, ncomp 2): two components, the third is untouched
    DYNARY u;
    path::dynary_reshape(&u, 0x79, 2);
    u.axis[0][1] = 2.0;
    u.values[2] = 4.0;
    u.values[3] = -2.0;
    out[2] = 77.0;
    const double t1 = 0.5;
    CHECK(path::path_at_time(&u, &t1, out));
    CHECK_BITS(out[0], 1.0);
    CHECK_BITS(out[1], -0.5);
    CHECK_BITS(out[2], 77.0);
    // not a time path
    u.var[0] = 0x1a;
    CHECK(!path::path_at_time(&u, &t1, out));
    CHECK(!path::path_point(&u, &k, out));
}

TEST_CASE(speed_to_time_140134de0)
{
    DYNARY t;
    path::dynary_reshape(&t, 0x7a, 4);
    t.var[0] = 0x1a;
    const double v[4] = {2.0, 0.0, 4.0, 1.0};
    const double p[16] = {0, 0, 0, 0, 3, 4, 0, 0, 3, 4, 0, 0, 3, 4, 12, 0};
    for (int k = 0; k < 4; ++k) t.axis[0][k] = v[k];
    for (int k = 0; k < 16; ++k) t.values[k] = p[k];
    CHECK(path::path_speed_to_time(&t));
    CHECK_EQ(t.var[0], 1);
    CHECK_BITS(t.axis[0][0], 0.0);
    CHECK_BITS(t.axis[0][1], 2.5);      // 5 / 2
    CHECK_BITS(t.axis[0][2], 2.5);      // speed 0: same time
    CHECK_BITS(t.axis[0][3], 2.5 + 12.0 / 4.0);
    CHECK(!path::path_speed_to_time(&t));  // var[0] is 1 now
    // 2D speed path (ncomp 2): sqrt((0 + dx*dx) + dy*dy)
    DYNARY u;
    path::dynary_reshape(&u, 0x79, 2);
    u.var[0] = 0x1a;
    u.axis[0][0] = 0.3;
    u.values[2] = 0.1;
    u.values[3] = 0.2;
    CHECK(path::path_speed_to_time(&u));
    CHECK_BITS(u.axis[0][1], 0.0 + std::sqrt((0.0 + 0.1 * 0.1) + 0.2 * 0.2) / 0.3);
}

TEST_CASE(transform_140135bc0)
{
    DYNARY t;
    path::dynary_reshape(&t, 0x7b, 2);
    CHECK_EQ(t.ncomp, 3);
    t.axis[0][1] = 5.0;
    const double p[6] = {1.0, 2.0, 7.0, -1.5, 0.25, 8.0};
    for (int k = 0; k < 6; ++k) t.values[k] = p[k];
    const double m[9] = {0.5, -0.25, 3.0, 0.125, 2.0, -1.0, 0.0, 0.0, 1.0};
    CHECK(path::path_transform(&t, m));
    CHECK_EQ(t.type, 0x7a);
    CHECK_EQ(t.ncomp, 4);
    CHECK_EQ(t.n[0], 2);
    CHECK_BITS(t.axis[0][1], 5.0);
    for (int k = 0; k < 2; ++k) {
        const double x = p[3 * k], y = p[3 * k + 1];
        CHECK_BITS(t.values[4 * k + 0], m[0] * x + m[1] * y + m[2] * 1.0);
        CHECK_BITS(t.values[4 * k + 1], m[3] * x + m[4] * y + m[5] * 1.0);
        CHECK_BITS(t.values[4 * k + 2], m[6] * x + m[7] * y + m[8] * 1.0);
        CHECK_BITS(t.values[4 * k + 3], p[3 * k + 2]);
    }
    CHECK(!path::path_transform(&t, m));  // type 0x7a now
}

TEST_CASE(path_curve_14021ee40_with_matrix)
{
    Fixture f;
    LLOBJ_DATA* o = f.obj(1);
    void* mv = &o->s2160;
    set_path(o, 0x7b, {1.0, 1.0}, {0, 0, 0, 3, 4, 2});
    at<std::int32_t>(mv, 0x448 + 0x10) = 0x1a;  // speed table (+0x458)
    at<std::int32_t>(mv, 0x4c0) = 2;
    // matrix: m[0]=+4e0 m[3]=+4e8 m[6]=+4f0 m[1]=+4f8 m[4]=+500 m[7]=+508 m[2]=+4c8 m[5]=+4d0 m[8]=+4d8
    const double vals[9] = {2.0, 0.0, 0.0, 0.0, 2.0, 0.0, 10.0, 20.0, 1.0};
    const std::size_t offs[9] = {0x4e0, 0x4e8, 0x4f0, 0x4f8, 0x500, 0x508, 0x4c8, 0x4d0, 0x4d8};
    for (int i = 0; i < 9; ++i) at<double>(mv, offs[i]) = vals[i];
    double m[9];
    CHECK(path::movement_path_matrix(mv, m));
    CHECK_BITS(m[0], 2.0);
    CHECK_BITS(m[2], 10.0);
    CHECK_BITS(m[5], 20.0);
    DYNARY c;
    path::movement_path_curve(mv, &c);
    CHECK_EQ(c.type, 0x7a);
    CHECK_BITS(c.axis[0][1], 5.0);  // len 5 at speed 1 (time conversion before the transform)
    CHECK_BITS(c.values[4], 2.0 * 3.0 + 0.0 * 4.0 + 10.0);
    CHECK_BITS(c.values[5], 0.0 * 3.0 + 2.0 * 4.0 + 20.0);
    CHECK_BITS(c.values[7], 2.0);
    // the movement's own table is untouched
    CHECK_EQ(table_of(o)->type, 0x7b);
}

TEST_CASE(place_on_path_1402007d0)
{
    reset_hooks();
    Fixture f;
    LLOBJ_DATA* o = f.obj(1);
    set_path(o, 0x7a, {0.0, 2.0}, {0, 0, 0, 0, 4.0, -2.0, 1.0, 0});
    double* p = pos(o);
    p[0] = 0.5;
    p[1] = 0.25;
    p[2] = -1.0;
    const double t = 0.5;
    CHECK(path::place_on_path(o, &t));
    REQUIRE(g_moves.calls.size() == 1u);
    const double f4 = (0.5 - 0.0) / (2.0 - 0.0);
    const double P[3] = {0.0 + f4 * (4.0 - 0.0), 0.0 + f4 * (-2.0 - 0.0), 0.0 + f4 * (1.0 - 0.0)};
    CHECK_BITS(g_moves.calls[0].second[0], P[0] - 0.5);
    CHECK_BITS(g_moves.calls[0].second[1], P[1] - 0.25);
    CHECK_BITS(g_moves.calls[0].second[2], P[2] - -1.0);
    // STROKE = current path position (3D model: three components)
    CHECK_BITS(at<double>(o, 0x22d0), p[0]);
    CHECK_BITS(at<double>(o, 0x22d8), p[1]);
    CHECK_BITS(at<double>(o, 0x22e0), p[2]);
    // already there: no move, STROKE still written
    at<double>(o, 0x22d0) = 99.0;
    CHECK(!path::place_on_path(o, &t));
    CHECK_EQ(g_moves.calls.size(), 1u);
    CHECK_BITS(at<double>(o, 0x22d0), p[0]);
    // not type 6
    at<std::int32_t>(o, 0x21c8) = 4;
    CHECK(!path::place_on_path(o, &t));
}

TEST_CASE(segment_search_14019ffc0)
{
    reset_hooks();
    Fixture f;
    LLOBJ_DATA* x = f.obj(1);
    LLOBJ_DATA* b = f.obj(2);
    // a zero-length segment (skipped), then (10,0,0), then (0,10,0)
    set_path(x, 0x7a, {0.0, 0.5, 1.0, 2.0}, {0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 10, 10, 0, 0});
    g_kscript = {{true, 15.0}, {true, 4.0}};  // first contact beyond the segment, second inside
    double tol = 0.25, d = -1.0, disp[3] = {}, tout = -1.0;
    CHECK(path::fun_14019ffc0(f.m, x, b, &tol, &d, disp, &tout));
    REQUIRE(g_kcalls.size() == 2u);
    CHECK_BITS(g_kcalls[0].dir[0], 1.0);
    CHECK_BITS(g_kcalls[0].dir[1], 0.0);
    CHECK_BITS(g_kcalls[0].at[0], 0.0);
    CHECK_EQ(static_cast<int>(g_kcalls[0].flag), 0);
    CHECK_BITS(g_kcalls[1].dir[1], 1.0);
    CHECK_BITS(g_kcalls[1].at[0], 10.0);
    CHECK_BITS(g_kcalls[1].at[1], 0.0);
    CHECK_BITS(tout, 1.0 + (4.0 / 10.0) * (2.0 - 1.0));
    CHECK_BITS(pos(x)[1], 0.0 + ((tout - 1.0) / (2.0 - 1.0)) * (10.0 - 0.0));
    // no contact: false, tout untouched
    g_kcalls.clear();
    g_kscript = {{false, 0.0}, {true, 11.0}};
    tout = -1.0;
    CHECK(!path::fun_14019ffc0(f.m, x, b, &tol, &d, disp, &tout));
    CHECK_EQ(g_kcalls.size(), 2u);
    CHECK_BITS(tout, -1.0);
    // not on a path
    at<std::int32_t>(x, 0x21c8) = 0;
    CHECK(!path::fun_14019ffc0(f.m, x, b, &tol, &d, disp, &tout));
}

TEST_CASE(all_on_path_14019ec30)
{
    reset_hooks();
    Fixture f;
    LLOBJ_DATA* a = f.obj(1);
    LLOBJ_DATA* c2 = f.obj(2);
    LLOBJ_DATA* c3 = f.obj(3);
    set_path(a, 0x7a, {0.0, 1.0}, {0, 0, 0, 0, 1, 0, 0, 0});
    set_path(c2, 0x7a, {0.0, 1.0}, {0, 0, 0, 0, 0, 1, 0, 0});
    at<std::int32_t>(c3, 0xc) = 7;  // empty object: skipped
    std::int32_t list[3] = {-2, 3, 9};  // 9: no such object, skipped
    std::int32_t n = 3;
    CHECK(ifd::fun_14019ec30(f.m, a, &n, list));
    at<std::int32_t>(c3, 0xc) = 0;  // a real object without a path
    CHECK(!ifd::fun_14019ec30(f.m, a, &n, list));
    n = 1;
    CHECK(ifd::fun_14019ec30(f.m, a, &n, list));
    at<std::int32_t>(a, 0x21c8) = 1;
    CHECK(!ifd::fun_14019ec30(f.m, a, &n, list));
}

namespace {
std::vector<std::pair<LLOBJ_DATA*, const LLOBJ_DATA*>> g_copies;
void record_copy(LLOBJ_DATA* d, const LLOBJ_DATA* s) { g_copies.push_back({d, s}); }
} // namespace

TEST_CASE(branch_14019fb50_success_takes_coupled_minimum)
{
    reset_hooks();
    path::g_copy_hook = record_copy;
    g_copies.clear();
    Fixture f;
    LLOBJ_DATA* a = f.obj(1);
    LLOBJ_DATA* b = f.obj(2);
    LLOBJ_DATA* c = f.obj(3);
    LLOBJ_DATA* e = f.obj(4);
    set_path(a, 0x7a, {0.0, 1.0}, {0, 0, 0, 0, 10, 0, 0, 0});
    set_path(c, 0x7a, {0.0, 2.0}, {0, 0, 0, 0, 0, 10, 0, 0});
    set_path(e, 0x7a, {0.0, 1.0}, {0, 0, 0, 0, 0, 0, 10, 0});
    auto& sc = f.m->sim_ctrl;
    sc.tnow3 = 1;  // selects TNOW 2
    sc.tnow1 = -5.0;
    sc.tnow2 = 0.5;
    CHECK_BITS(ifd::sim_time(&sc), 0.5);
    // A: 0.6 (d 6 of 10 over t 0..1); C: 0.4 (d 2 of 10 over 0..2); E: fails
    g_kscript = {{true, 6.0}, {true, 2.0}, {false, 0.0}};
    std::int32_t list[3] = {3, -4, 9};
    std::int32_t n = 3;
    double dir[3] = {7, 7, 7}, tol = 0.25, dist = 3.0, disp[3] = {1, 2, 3};
    Capture cap;
    const bool ok = ifd::fun_14019fb50(f.m, a, b, dir, &tol, &dist, disp, 1, &n, list);
    const std::string out = cap.done();
    CHECK(ok);
    const double ta = 0.0 + (6.0 / 10.0) * (1.0 - 0.0);
    const double tcc = 0.0 + (2.0 / 10.0) * (2.0 - 0.0);
    CHECK_BITS(sc.tnow2, tcc < ta ? tcc : ta);
    CHECK_BITS(sc.tnow1, -5.0);
    CHECK_BITS(dist, 3.0);   // the caller's distance and displacement are not written
    CHECK_BITS(disp[0], 1.0);
    REQUIRE(g_copies.size() == 1u);  // backup only
    CHECK(g_copies[0].second == a);
    CHECK_EQ(out, std::string("  Arbitrary path has been detected.\n"
                              "  Current TNow is 0.500000.\n"
                              "  Calculated new time = 0.400000\n"));
    // the dispatcher's re-placement at the new time
    g_moves.calls.clear();
    f.m->u1378 = 4;
    ifd::fun_14019ed10(f.m);
    // A from 0.6 to 0.4, E from wherever its failed search left it to 0.4; C already at 0.4
    CHECK_BITS(pos(a)[0], 0.0 + (0.4 / 1.0) * 10.0);
    CHECK_BITS(pos(e)[2], 0.0 + (0.4 / 1.0) * 10.0);
    for (auto& mv : g_moves.calls) CHECK(mv.first != c && mv.first != b);
}

TEST_CASE(branch_14019fb50_failure_restores)
{
    reset_hooks();
    path::g_copy_hook = record_copy;
    g_copies.clear();
    Fixture f;
    LLOBJ_DATA* a = f.obj(1);
    LLOBJ_DATA* b = f.obj(2);
    set_path(a, 0x7a, {0.0, 1.0}, {0, 0, 0, 0, 10, 0, 0, 0});
    auto& sc = f.m->sim_ctrl;
    sc.tnow3 = 0;
    sc.tnow1 = 1.25;
    std::int32_t n = 0;
    double tol = 0.25, dist = 0.0, disp[3] = {};
    Capture cap;
    const bool ok = ifd::fun_14019fb50(f.m, a, b, nullptr, &tol, &dist, disp, 0, &n, nullptr);
    const std::string out = cap.done();
    CHECK(!ok);
    CHECK_BITS(sc.tnow1, 1.25);
    REQUIRE(g_copies.size() == 2u);
    CHECK(g_copies[0].second == a);
    CHECK(g_copies[1].first == a);
    CHECK(g_copies[1].second == g_copies[0].first);
    CHECK_EQ(out, std::string("  Arbitrary path has been detected.\n"
                              "  Current TNow is 1.250000.\n"
                              "  Failed in interference positioning\n"));
}

// The real 1401613a0 backup / restore is not run here: on a constructed object it reaches
// 0x1402f9b60 (OBJ_GEO3D, p1-07 ext call not wired in defmodel's llobj ext layer yet).

TEST_CASE(sim_time_get_set_1401f3ae0)
{
    Fixture f;
    auto& sc = f.m->sim_ctrl;
    sc.tnow3 = 0;
    double t = 2.5;
    ifd::set_sim_time(&sc, &t);
    CHECK_BITS(sc.tnow1, 2.5);
    CHECK_BITS(ifd::sim_time(&sc), 2.5);
    sc.tnow3 = 3;
    t = -1.0;
    ifd::set_sim_time(&sc, &t);
    CHECK_BITS(sc.tnow2, -1.0);
    CHECK_BITS(sc.tnow1, 2.5);
}

int main() { return testkit::run_all(); }
