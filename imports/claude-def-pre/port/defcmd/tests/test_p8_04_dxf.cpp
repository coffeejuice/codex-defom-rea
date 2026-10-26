// ctest-labels: p8-04
// Tier-1 tests of the DXF reader (packet p8-04).
//
//  * golden_segments: FUN_140422ab0 (parse, entity handlers, segments, OCS, size pass, G) on every
//    DXF file the GFREAD DXF oracle cases read, against tests/commands/GFREAD/data/
//    dxf_segments.golden (written by tests/commands/GFREAD/models/dxf_golden.py from the reference
//    model dxf_model.py; `dxf_golden.py oracle` checks that model against the cached oracle
//    OUT.KEY of all 27 cases). Bit-identical counts, segment tables and G. The install DXFs are
//    used (as copies) only when DEFORM is installed.
//  * entry_*: FUN_1404093a0 with the calls into p8-01 / p8-03 / p8-09 through the seam hooks.
//  * helpers: string compare / trim / code lookup, the class.
#define _CRT_SECURE_NO_WARNINGS
#include <testkit/check.hpp>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <process.h>

#include <defcmd/dxf.hpp>
#include <defcmd/globals_p8_04.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>

#include "../src/dxf/dxf_internal.hpp"
#include "../src/dxf/ext.hpp"

namespace fs = std::filesystem;
namespace dxf = def::cmd::dxf;
namespace ext = def::cmd::dxf::ext;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO2D;

namespace {

fs::path repo()
{
    // port/defcmd/tests/<this file> -> repository root
    return fs::path(__FILE__).parent_path().parent_path().parent_path().parent_path();
}

std::string tmp_path(const std::string& base)
{
    return (fs::temp_directory_path() / (std::to_string(_getpid()) + "_p804_" + base)).string();
}

std::uint64_t bits(double v)
{
    std::uint64_t u;
    std::memcpy(&u, &v, 8);
    return u;
}
double from_hex(const std::string& h)
{
    // little-endian byte string as struct.pack('<d').hex() writes it
    unsigned char b[8];
    for (int i = 0; i < 8; ++i) b[i] = static_cast<unsigned char>(std::stoul(h.substr(2 * i, 2), nullptr, 16));
    double v;
    std::memcpy(&v, b, 8);
    return v;
}

// Stand-in for p8-03's FUN_140463374 (rational B-spline sampling, DXF_READER.md section 6.11),
// operation for operation as dxf_model.spline_eval, with p8-03's argument meaning: ctrl, knots,
// weights, K (= control count here), M (degree), out, nsamp, *nout (1-based write index, +1 per
// point, -1 at the end), v0, v1.
void spline_hook(double* ctrl, double* knots, double* w, std::int32_t* nc_p, std::int32_t* deg_p, double* out,
                 std::int32_t* nsamp_p, std::int32_t* idx_p, double* t0_p, double* tn_p)
{
    const int nc = *nc_p, p = *deg_p, nsamp = *nsamp_p;
    const int n = p + 1, nk = p + 1 + nc;
    const double t0 = *t0_p, tn = *tn_p;
    auto U = [&](int i) { return i >= 1 && i <= nk ? knots[i - 1] : std::numeric_limits<double>::quiet_NaN(); };
    for (int j = 0; j < nsamp; ++j) {
        const double t = static_cast<double>(j) * ((tn - t0) / static_cast<double>(nsamp - 1)) + t0;
        for (int i = p + 1; i <= nk; ++i) {
            if (!(U(i) <= t && t <= U(i + 1))) continue;
            std::vector<double> B(static_cast<std::size_t>((n + 2) * (n + 2)), 0.0);
            auto b = [&](int r, int c) -> double& { return B[static_cast<std::size_t>(r * (n + 2) + c)]; };
            b(n, 1) = 1.0;
            for (int k = 1; k <= p; ++k) {
                for (int m = 1; m <= k + 1; ++m) {
                    const int jj = m - 1;
                    const double d1 = U(i + k - jj) - U(i - jj);
                    const double A = d1 != 0.0 ? ((t - U(i - jj)) / d1) * b(p - jj + 1, k) : 0.0;
                    const double d2 = U(i + k + 1 - jj) - U(i + 1 - jj);
                    double Bt = 0.0;
                    if (d2 != 0.0) {
                        const int rr = p - jj + 2;
                        Bt = rr > n ? 0.0 : ((U(k + 1 + i - jj) - t) / d2) * b(rr, k);
                    }
                    b(n - jj, k + 1) = A + Bt;
                }
            }
            double o[3] = {0.0, 0.0, 0.0}, W[3] = {0.0, 0.0, 0.0};
            int row = 0;
            for (int cp = i - p; cp <= i; ++cp) {
                ++row;
                const double wt = w[cp - 1];
                const double bb = b(row, n);
                for (int d = 0; d < 3; ++d) {
                    o[d] = wt * ctrl[(cp - 1) * 3 + d] * bb + o[d];
                    W[d] = wt * bb + W[d];
                }
            }
            double* dst = out + (*idx_p - 1) * 3;
            for (int d = 0; d < 3; ++d) dst[d] = o[d] / W[d];
            *idx_p += 1;
            break;
        }
    }
    *idx_p -= 1;
}

std::int64_t ulps(double a, double b)
{
    const std::int64_t x = static_cast<std::int64_t>(bits(a)), y = static_cast<std::int64_t>(bits(b));
    if ((x < 0) != (y < 0)) return std::numeric_limits<std::int64_t>::max();
    return x > y ? x - y : y - x;
}
int g_host_trig_diffs = 0;

struct GoldenRead {
    std::string name, kind, base;
    double g0 = 0, g = 0;
    int rc = 0, npts = 0, nseg = 0;
    std::vector<std::vector<double>> segs;
};

std::vector<GoldenRead> load_golden()
{
    std::vector<GoldenRead> v;
    std::ifstream in(repo() / "tests" / "commands" / "GFREAD" / "data" / "dxf_segments.golden");
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.rfind("read ", 0) == 0) {
            std::istringstream s(line);
            GoldenRead r;
            std::string kw, g0, g;
            s >> kw >> r.name >> r.kind >> r.base >> kw >> g0 >> kw >> r.rc >> kw >> r.npts >> kw >> r.nseg >> kw >> g;
            r.g0 = from_hex(g0);
            r.g = from_hex(g);
            v.push_back(r);
        } else if (line.rfind("  ", 0) == 0 && !v.empty()) {
            std::istringstream s(line);
            std::vector<double> seg;
            std::string h;
            while (s >> h) seg.push_back(from_hex(h));
            v.back().segs.push_back(seg);
        }
    }
    return v;
}

fs::path source_of(const GoldenRead& r)
{
    if (r.kind == "data") return repo() / "tests" / "commands" / "GFREAD" / "data" / r.base;
    return fs::path("C:/Program Files/SFTC/DEFORM/v11.0.2/Tutorials") / r.base;
}

// a copy of a test file under a per-process temp name
std::string copy_of(const fs::path& src, const std::string& base)
{
    const std::string dst = tmp_path(base);
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
    return dst;
}

template <class T>
T& field(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

// ---- seam hooks for the entry tests ----------------------------------------------------------
struct Calls {
    int snapshot = 0, conv = 0, iges_read = 0;
    int conv_rc = 0;
    int conv_nseg_out = -1;  // -1: leave n_records
    std::int32_t conv_kind = 0, conv_mode = 0;
    std::int32_t ctx_npts = 0, ctx_nseg = 0;
    double ctx_tol = 0;
    std::vector<double> ctx_table;
    double* ctx_table_ptr = nullptr;
    // 140409af0
    const char* af0_file = reinterpret_cast<const char*>(1);
    const char* af0_prefix = reinterpret_cast<const char*>(1);
    LLOBJ_DATA* af0_obj = nullptr;
    double af0_tol = 0, af0_iges20 = 0;
    std::int32_t af0_p6 = 0, af0_p8 = 0, af0_mode = 0;
    char af0_p7 = 9;
};
Calls g_calls;

void install_hooks()
{
    g_calls = Calls{};
    ext::g_140197bc0 = [](LLOBJ_DATA*) -> std::int64_t {
        ++g_calls.snapshot;
        return 0;
    };
    ext::g_14019a650 = [](LLOBJ_DATA* o, std::int32_t v) { field<std::int32_t>(o, 0xbc8) = v; };
    ext::g_1402566b0 = [](ext::IgesHelper* h) { return field<std::int32_t>(h->raw, 0x1c); };
    ext::g_1404098d0 = [](LLOBJ_DATA* o, double v) { field<double>(o, 0x3a48) = v; };
    ext::g_1404098f0 = [](LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x3a50); };
    ext::g_140409930 = [](LLOBJ_DATA* o) { return field<double>(o, 0x3a58); };
    ext::g_14040cde0 = [](OBJ_GEO2D* c, double* d) { field<double*>(c, 0x18) = d; };
    ext::g_14040ce40 = [](ext::IgesHelper* h) { field<std::int32_t>(h->raw, 0x18) = 1; };
    ext::g_14040ce60 = [](ext::IgesHelper* h, std::int32_t v) { field<std::int32_t>(h->raw, 0x1c) = v; };
    ext::g_140198270 = [](LLOBJ_DATA* o) {
        def::model::geo_kw::ensure_geo_list(*o);
        def::model::GEO2D_AND_MESH2D* g = def::model::geo_kw::geometry(*o, 0);
        if (g == nullptr) g = def::model::geo_kw::geo_list_append_default(def::model::geo_kw::geo_list(*o));
        g->add_edge_curve();
    };
    ext::g_14040c220 = [](char*, void* ctx, std::int32_t kind, std::int32_t mode) -> int {
        auto* c = static_cast<dxf::ConvContext*>(ctx);
        ++g_calls.conv;
        g_calls.conv_kind = kind;
        g_calls.conv_mode = mode;
        g_calls.ctx_npts = c->n_points;
        g_calls.ctx_nseg = c->n_records;
        g_calls.ctx_tol = c->tol;
        g_calls.ctx_table_ptr = c->table;
        g_calls.ctx_table.assign(c->table, c->table + 6 * c->n_records);
        if (g_calls.conv_nseg_out >= 0) c->n_records = g_calls.conv_nseg_out;
        return g_calls.conv_rc;
    };
    ext::g_140409af0 = [](def::cmd::iges::Uic2CObjectIgesFile* h, const char* f, const char* p, LLOBJ_DATA* o, double* tol, std::int32_t p6,
                          char p7, std::int32_t p8, std::int32_t mode) -> int {
        ++g_calls.iges_read;
        g_calls.af0_file = f;
        g_calls.af0_prefix = p;
        g_calls.af0_obj = o;
        g_calls.af0_tol = *tol;
        g_calls.af0_iges20 = field<double>(h, 0x20);
        g_calls.af0_p6 = p6;
        g_calls.af0_p7 = p7;
        g_calls.af0_p8 = p8;
        g_calls.af0_mode = mode;
        return 77;
    };
    ext::g_140463374 = spline_hook;
    // ctor / dtor of the stack Uic2CObjectIgesFile: zeroed storage (p8-03's class need not exist)
    ext::g_1404099d0 = [](ext::IgesHelper* h) { std::memset(h->raw, 0, sizeof h->raw); };
    ext::g_140409a90 = [](ext::IgesHelper*) {};
    ext::g_140409330 = [](def::cmd::Uic2CObjectDxfFile*) {};
}

const GoldenRead* golden_of(const std::vector<GoldenRead>& gs, const std::string& name)
{
    for (const auto& r : gs)
        if (r.name == name) return &r;
    return nullptr;
}

LLOBJ_DATA* new_object()
{
    auto* m = new def::model::DB_STEP_DATA(2);  // leaked: the test checks the reader, not the model dtor
    return def::model::llobj_ctor::get_or_create(m, 1);
}

} // namespace

TEST_CASE(golden_segments)
{
    install_hooks();
    const std::vector<GoldenRead> gs = load_golden();
    REQUIRE(gs.size() >= 27);
    int done = 0, skipped = 0;
    for (const GoldenRead& r : gs) {
        const fs::path src = source_of(r);
        if (!fs::exists(src)) {
            ++skipped;
            continue;
        }
        const std::string file = copy_of(src, r.base);
        dxf::reset_globals();
        dxf::fun_140428f70(r.g0);
        std::int32_t npts = -7, nseg = -7;
        double* data = reinterpret_cast<double*>(1);
        const std::int32_t rc = dxf::fun_140422ab0(file.c_str(), &npts, &nseg, &data);
        std::error_code ec;
        fs::remove(file, ec);
        const std::string what = r.name + " " + r.base;
        bool ok = rc == r.rc && npts == r.npts && nseg == r.nseg && bits(dxf::fun_1404099c0()) == bits(r.g);
        if (ok && rc == 0) {
            ok = data != nullptr && static_cast<int>(r.segs.size()) == nseg;
            for (int i = 0; ok && i < nseg; ++i)
                for (int k = 0; k < 6; ++k) {
                    const double want = r.segs[static_cast<std::size_t>(i)][static_cast<std::size_t>(k)];
                    const double got = data[i * 6 + k];
                    if (bits(got) == bits(want)) continue;
                    // ARC/CIRCLE start point cx + r*cos(s), cy + r*sin(s): the port calls the
                    // binary's own sin/cos (defrt libm 140bb3000/140bb3490, verified bit-exact);
                    // the Python model uses the host's, which can differ in the last bits.
                    if (data[i * 6] == 2.0 && (k == 1 || k == 2) && ulps(got, want) <= 4) {
                        ++g_host_trig_diffs;
                        continue;
                    }
                    {
                        std::printf("  %s: segment %d value %d: %.17g vs golden %.17g\n", what.c_str(), i + 1, k,
                                    data[i * 6 + k], r.segs[static_cast<std::size_t>(i)][static_cast<std::size_t>(k)]);
                        ok = false;
                    }
                }
        }
        if (!ok)
            std::printf("  %s: rc %d npts %d nseg %d G %.17g; golden rc %d npts %d nseg %d G %.17g\n", what.c_str(), rc, npts,
                        nseg, dxf::fun_1404099c0(), r.rc, r.npts, r.nseg, r.g);
        CHECK(ok);
        if (rc == 0) dxf::fun_140422d90(data);
        ++done;
    }
    std::printf("  golden reads compared: %d, skipped (no DEFORM install): %d; arc start values within 4 ulp "
                "of the host-trig model: %d\n", done, skipped, g_host_trig_diffs);
    CHECK(done >= 22);
}

TEST_CASE(read_error_codes)
{
    std::int32_t a = 1, b = 1;
    double* d = reinterpret_cast<double*>(1);
    CHECK_EQ(dxf::fun_140422ab0(nullptr, &a, &b, &d), -100);
    CHECK(a == 0 && b == 0 && d == nullptr);
    CHECK_EQ(dxf::fun_140422ab0("", &a, &b, &d), -200);
    const double g = dxf::fun_1404099c0();
    CHECK_EQ(dxf::fun_140422ab0(tmp_path("missing.dxf").c_str(), &a, &b, &d), -300);
    CHECK_BITS(dxf::fun_1404099c0(), g);  // no open: G unchanged
}

TEST_CASE(entry_mode300_success)
{
    install_hooks();
    const std::vector<GoldenRead> gs = load_golden();
    const GoldenRead* r = golden_of(gs, "gfread_dxf_lwpoly_bulge");
    REQUIRE(r != nullptr);
    const std::string file = copy_of(source_of(*r), r->base);
    dxf::reset_globals();
    LLOBJ_DATA* obj = new_object();
    REQUIRE(obj != nullptr);
    CHECK_EQ(field<std::int32_t>(obj, 0x3a50), 0);  // T8-3: the ctor's 0 (14015f0b0), no setter
    g_calls.conv_nseg_out = 3;                       // the chaining stage writes back 3 segments
    def::cmd::Uic2CObjectDxfFile d(nullptr);
    const int rc = dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300);
    CHECK_EQ(rc, 0);
    CHECK_EQ(g_calls.snapshot, 1);
    CHECK_EQ(g_calls.conv, 1);
    CHECK_EQ(g_calls.conv_kind, 2);
    CHECK_EQ(g_calls.conv_mode, 700);
    CHECK_EQ(g_calls.ctx_npts, r->npts);
    CHECK_EQ(g_calls.ctx_nseg, r->nseg);
    CHECK_BITS(g_calls.ctx_tol, r->g);
    REQUIRE(static_cast<int>(g_calls.ctx_table.size()) == 6 * r->nseg);
    for (int i = 0; i < r->nseg; ++i)
        for (int k = 0; k < 6; ++k)
            CHECK_BITS(g_calls.ctx_table[static_cast<std::size_t>(i * 6 + k)], r->segs[static_cast<std::size_t>(i)][static_cast<std::size_t>(k)]);
    // tolerance: G of this read, stored in obj+0x3a48 and dxf+0x20
    CHECK_BITS(field<double>(obj, 0x3a48), r->g);
    CHECK_BITS(d.u20, r->g);
    // curve 0: type 2, the written-back count, the segment array itself
    OBJ_GEO2D* c = def::model::geo_kw::curve(*obj, 0);
    REQUIRE(c != nullptr);
    CHECK_EQ(c->curve_type, 2);
    CHECK_EQ(c->n_points, 3);
    CHECK(c->points == g_calls.ctx_table_ptr);
    CHECK_EQ(def::model::geo_kw::curve_count(*obj), 1);
    // one boundary
    CHECK_EQ(field<std::int32_t>(obj, 0xbc8), 1);
    CHECK_EQ(d.u1c, 1);
    // the work arrays are released, the context's table pointer cleared
    CHECK(dxf::conv_rw() == nullptr);
    CHECK(dxf::conv_iw() == nullptr);
    CHECK(dxf::conv_ctx().table == nullptr);
    std::error_code ec;
    fs::remove(file, ec);
}

TEST_CASE(entry_mode300_several_loops)
{
    install_hooks();
    const std::vector<GoldenRead> gs = load_golden();
    const GoldenRead* r = golden_of(gs, "gfread_dxf_two_loops");
    REQUIRE(r != nullptr);
    const std::string file = copy_of(source_of(*r), r->base);
    dxf::reset_globals();
    LLOBJ_DATA* obj = new_object();
    g_calls.conv_rc = 200;
    def::cmd::Uic2CObjectDxfFile d;
    CHECK_EQ(dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300), 0);
    // code 200: boundary count = the segment count passed IN (listing 14040bee6: [rsp+0x30])
    CHECK_EQ(field<std::int32_t>(obj, 0xbc8), r->nseg);
    CHECK_EQ(d.u1c, r->nseg);
    OBJ_GEO2D* c = def::model::geo_kw::curve(*obj, 0);
    REQUIRE(c != nullptr);
    CHECK_EQ(c->n_points, r->nseg);
    std::error_code ec;
    fs::remove(file, ec);
}

TEST_CASE(entry_mode300_conversion_error)
{
    install_hooks();
    const std::vector<GoldenRead> gs = load_golden();
    const GoldenRead* r = golden_of(gs, "gfread_dxf_lines_open");
    REQUIRE(r != nullptr);
    const std::string file = copy_of(source_of(*r), r->base);
    dxf::reset_globals();
    LLOBJ_DATA* obj = new_object();
    g_calls.conv_rc = -10;
    const std::int32_t bc_before = field<std::int32_t>(obj, 0xbc8);
    def::cmd::Uic2CObjectDxfFile d;
    CHECK_EQ(dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300), -10);
    CHECK_EQ(d.u1c, 0);
    CHECK_EQ(field<std::int32_t>(obj, 0xbc8), bc_before);  // not touched on a conversion error
    CHECK(dxf::conv_rw() == nullptr);
    std::error_code ec;
    fs::remove(file, ec);
}

TEST_CASE(entry_mode300_early_results)
{
    install_hooks();
    def::cmd::Uic2CObjectDxfFile d;
    LLOBJ_DATA* obj = new_object();
    CHECK_EQ(dxf::fun_1404093a0(&d, "x.dxf", "", nullptr, 2, 300), -1);
    CHECK_EQ(dxf::fun_1404093a0(&d, nullptr, "", obj, 2, 300), -2);
    CHECK_EQ(dxf::fun_1404093a0(&d, "", "", obj, 2, 300), -2);
    CHECK_EQ(dxf::fun_1404093a0(&d, tmp_path("none.dxf").c_str(), "", obj, 2, 300), -3);
    CHECK_EQ(g_calls.snapshot, 0);
    const std::vector<GoldenRead> gs = load_golden();
    // no segments: -500 after the snapshot; G = 0; no conversion
    {
        const GoldenRead* r = golden_of(gs, "gfread_dxf_empty");
        REQUIRE(r != nullptr);
        const std::string file = copy_of(source_of(*r), r->base);
        dxf::reset_globals();
        CHECK_EQ(dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300), -500);
        CHECK_EQ(g_calls.snapshot, 1);
        CHECK_EQ(g_calls.conv, 0);
        CHECK_BITS(dxf::fun_1404099c0(), 0.0);
        std::error_code ec;
        fs::remove(file, ec);
    }
    // binary DXF: -10000
    {
        const GoldenRead* r = golden_of(gs, "gfread_dxf_binary");
        REQUIRE(r != nullptr);
        const std::string file = copy_of(source_of(*r), r->base);
        CHECK_EQ(dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300), -10000);
        std::error_code ec;
        fs::remove(file, ec);
    }
    // only an arc of radius 0: 0 segments after the -500 check -> 14040ba80 returns -1
    {
        const GoldenRead* r = golden_of(gs, "gfread_dxf_arc_r0");
        REQUIRE(r != nullptr);
        const std::string file = copy_of(source_of(*r), r->base);
        g_calls.conv = 0;
        CHECK_EQ(dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300), -1);
        CHECK_EQ(g_calls.conv, 0);
        CHECK(dxf::conv_rw() == nullptr);
        std::error_code ec;
        fs::remove(file, ec);
    }
}

TEST_CASE(entry_user_tolerance)
{
    // obj+0x3a50 != 0 takes obj+0x3a58 instead of G (no setter exists in the program, T8-3)
    install_hooks();
    const std::vector<GoldenRead> gs = load_golden();
    const GoldenRead* r = golden_of(gs, "gfread_dxf_lines_gap");
    REQUIRE(r != nullptr);
    const std::string file = copy_of(source_of(*r), r->base);
    LLOBJ_DATA* obj = new_object();
    field<std::int32_t>(obj, 0x3a50) = 1;
    field<double>(obj, 0x3a58) = 0.125;
    def::cmd::Uic2CObjectDxfFile d;
    CHECK_EQ(dxf::fun_1404093a0(&d, file.c_str(), "", obj, 2, 300), 0);
    CHECK_BITS(g_calls.ctx_tol, 0.125);
    CHECK_BITS(d.u20, 0.125);
    CHECK_BITS(field<double>(obj, 0x3a48), 0.125);
    CHECK_BITS(dxf::fun_1404099c0(), r->g);  // G is still set by the read
    std::error_code ec;
    fs::remove(file, ec);
}

TEST_CASE(entry_other_mode_delegates_to_iges)
{
    install_hooks();
    LLOBJ_DATA* obj = new_object();
    def::cmd::Uic2CObjectDxfFile d;
    d.u20 = 0.375;
    CHECK_EQ(dxf::fun_1404093a0(&d, "ignored.dxf", "p", obj, 2, 400), 77);
    CHECK_EQ(g_calls.iges_read, 1);
    CHECK(g_calls.af0_file == nullptr);
    CHECK(g_calls.af0_prefix == nullptr);
    CHECK(g_calls.af0_obj == obj);
    CHECK_BITS(g_calls.af0_tol, 0.375);
    CHECK_BITS(g_calls.af0_iges20, 0.375);  // 140409950 on the IGES object
    CHECK_EQ(g_calls.af0_p6, 2);
    CHECK_EQ(static_cast<int>(g_calls.af0_p7), 0);
    CHECK_EQ(g_calls.af0_p8, 1);
    CHECK_EQ(g_calls.af0_mode, 400);
    CHECK_EQ(g_calls.snapshot, 0);
}

TEST_CASE(dxf_object)
{
    install_hooks();
    def::cmd::Uic2CObjectDxfFile d;
    CHECK_EQ(std::string(d.class_name()), std::string("Uic2CObjectDxfFile"));
    CHECK_EQ(d.u18, 0);
    CHECK_EQ(d.u1c, 0);
    CHECK_BITS(d.u20, 0.0);
    CHECK_EQ(d.u08, 0);
    REQUIRE(d.name != nullptr);
    CHECK_EQ(d.name[0], '\0');
    auto* h = new def::cmd::Uic2CObjectDxfFile("abc");
    CHECK_EQ(std::string(h->name), std::string("abc"));
    def::cmd::Uic2CObject* b = h;
    delete b;  // slot 0 (140409890)
}

TEST_CASE(string_helpers)
{
    CHECK_EQ(dxf::fun_140428650("entities", "ENTITIES"), 0);
    CHECK(dxf::fun_140428650("LINE", "LINES") < 0);
    CHECK_EQ(dxf::fun_140428650(nullptr, "A"), 1);
    CHECK_EQ(dxf::fun_140428650("A", nullptr), -1);
    char s1[] = "   10 \r\n";
    CHECK_EQ(std::string(dxf::fun_140427df0(s1, 1)), std::string("10"));
    char s2[] = "  AB  ";
    CHECK_EQ(std::string(dxf::fun_140427df0(s2, 0)), std::string("  AB"));
    char s3[] = " \r\n";
    CHECK_EQ(std::string(dxf::fun_140427df0(s3, 1)), std::string(""));
    const dxf::CodeEntry tab[2] = {{"10", 0}, {"20", 1}};
    std::int32_t idx = -1, term = -1;
    CHECK_EQ(dxf::fun_140424c80("20", tab, 2, &idx, &term), 1);
    CHECK(idx == 1 && term == 0);
    CHECK_EQ(dxf::fun_140424c80("0", tab, 2, &idx, &term), 0);
    CHECK_EQ(term, 1);
    CHECK_EQ(dxf::fun_140424c80("seqend", tab, 2, &idx, &term), 0);  // entity names terminate too
    CHECK_EQ(term, 1);
    CHECK_EQ(dxf::fun_140424c80("30", tab, 2, &idx, &term), 0);
    CHECK_EQ(term, 0);
    CHECK_EQ(dxf::fun_140409ae0(), 30);
}

int main() { return testkit::run_all(); }
