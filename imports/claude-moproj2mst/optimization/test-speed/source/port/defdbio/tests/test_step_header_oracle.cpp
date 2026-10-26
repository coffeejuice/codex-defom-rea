// ctest-labels: p4-07 oracle
// Oracle check of the step-header statistics of packet p4-07 (14020cae0, 14020d320, 140211ef0 and
// the kernels under them) against DB steps that DEF_PRE's GENDB wrote.
//
// Cases: the cached runs "p4-07/gendb/<KEY>" (KFREAD <KEY> / GENDB NEW.DB, made by
// tests/data/p4_07/make_cases.py, cache %USERPROFILE%\.deform-oracle or $DEFORM_ORACLE_CACHE).
// For every meshed object of every step the test rebuilds the object's state-variable arrays
// from the arrays GENDB wrote into the same step (GENDB copies the model's arrays before it writes
// the header, docs/re/dbmap/stepdriver.md §6), runs the ported functions through the header slot
// table and compares each computed header word with DEF_PRE's bit for bit.
//
// Compared: velocity, displacement, temperature, relative density, effective strain, damage,
// user node / element variables, the codes 1600 (2D), 2600, 2800 and 5000, all stress kinds
// (effective, mean, both principal values) and the stress and strain component blocks.
// Not compared: copies of model fields (FORCE, STROKE, name, counts; nothing is computed), the
// strain-rate items (EPS / EFEPS are not written to the DB), volume (p4-03) and surface area
// (needs the surface polygons GENDB builds in p4-05), and items whose input array the DB lacks.
//
// Without the cache the test passes with a note. P407_ORACLE_VERBOSE=1 prints each mismatch.
#define _CRT_SECURE_NO_WARNINGS
#include <bit>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <memory>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include <deformdb/database.hpp>
#include <deformdb/model.hpp>
#include <defdbio/gendb/step_header.hpp>
#include <defmodel/hdrstats/ext_p4_07.hpp>
#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace p407 = def::model::p407;

namespace {

bool verbose() { return std::getenv("P407_ORACLE_VERBOSE") != nullptr; }

fs::path cache_dir()
{
    if (const char* c = std::getenv("DEFORM_ORACLE_CACHE")) return c;
    if (const char* h = std::getenv("USERPROFILE")) return fs::path(h) / ".deform-oracle";
    return {};
}

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

std::string json_string(const std::string& j, const char* key)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat);
    if (p == std::string::npos) return {};
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        if (j[p] == '\\' && p + 1 < j.size()) ++p;
        out += j[p++];
    }
    return out;
}

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// Zeroed, aligned raw storage for a model object (the functions under test read raw fields only).
struct Raw {
    explicit Raw(std::size_t n) : p(static_cast<std::byte*>(::operator new(n, std::align_val_t{16})))
    {
        std::memset(p, 0, n);
    }
    ~Raw() { ::operator delete(p, std::align_val_t{16}); }
    Raw(const Raw&) = delete;
    Raw& operator=(const Raw&) = delete;
    std::byte* p;
};

// One object rebuilt from a DB step.
struct Fake {
    Raw model{sizeof(DB_STEP_DATA)};
    Raw obj{sizeof(LLOBJ_DATA)};
    std::map<int, std::vector<double>> arrays; // by state-variable id
    std::set<int> present;                     // ids whose array came from the DB
    std::vector<double> efsts;                 // target of the kind-0 stress copy
    std::int32_t user_node_w = 0;
    std::int32_t user_elem_w = 0;
    int dim = 3;

    DB_STEP_DATA* m() { return reinterpret_cast<DB_STEP_DATA*>(model.p); }
    LLOBJ_DATA* o() { return reinterpret_cast<LLOBJ_DATA*>(obj.p); }
    void* nset() { return obj.p + 0x8a0; }
    void* eset() { return obj.p + 0x9e0; }
    std::int32_t& width(int id) { return at<std::int32_t>(model.p, 0x15f8 + 4 * static_cast<std::size_t>(id)); }
    double* arr(int id)
    {
        auto it = arrays.find(id);
        return it == arrays.end() || it->second.empty() ? nullptr : it->second.data();
    }
};

Fake* g_fake = nullptr;          // the object under test (per-function check)
std::vector<Fake*> g_fakes;      // all objects of the step (writer check)

// The fake a hook argument belongs to (the hooks get a set, an object or a model pointer).
Fake* fake_of(const void* p)
{
    if (g_fake != nullptr) return g_fake;
    const auto* b = static_cast<const std::byte*>(p);
    for (Fake* f : g_fakes) {
        if ((b >= f->obj.p && b < f->obj.p + sizeof(LLOBJ_DATA)) ||
            (b >= f->model.p && b < f->model.p + sizeof(DB_STEP_DATA)))
            return f;
    }
    std::printf("fake_of: unknown pointer\n");
    std::abort();
}

void install_hooks()
{
    using namespace p407::ext;
    fun_14017f020 = [](void* s) -> std::int32_t* { return &fake_of(s)->width(0); };
    fun_140182980 = [](void* s) -> std::int32_t* { return &fake_of(s)->width(0); };
    fun_14017ffe0 = [](void*, std::int32_t, std::int32_t*, std::int32_t) {};
    fun_1401836c0 = [](void*, std::int32_t, std::int32_t*, std::int32_t, std::int32_t) {};
    fun_1401d48d0 = [](DB_STEP_DATA*, const std::int32_t*) {};
    fun_1401840d0 = [](void* s) { return fake_of(s)->arr(0x2d); };
    fun_140184050 = [](void* s) { return fake_of(s)->arr(0x2c); };
    fun_140184150 = [](void* s, const bool*) { return fake_of(s)->arr(0x2e); };
    fun_140184a30 = [](void* s) { return fake_of(s)->arr(0x3c); };
    fun_1401815b0 = [](void* s) { return fake_of(s)->arr(0x11); };
    fun_1401812a0 = [](void* s) { return fake_of(s)->arr(0x1c); };
    fun_1401cbbc0 = [](DB_STEP_DATA* m) { return fake_of(m)->user_node_w; };
    fun_1401cbc80 = [](DB_STEP_DATA* m) { return fake_of(m)->user_elem_w; };
    fun_140158ce0 = [](const LLOBJ_DATA* o) { return fake_of(o)->dim; };
    fun_140160f10 = [](const LLOBJ_DATA*) { return true; };
    fun_140160c20 = [](LLOBJ_DATA*) { return false; };
    fun_140163540 = [](LLOBJ_DATA*, std::int32_t) { return 0.0; };
    fun_140324000 = []() { return 2; };
    fun_1401808d0 = [](void* s) { return fake_of(s)->arr(0x00); };
    fun_140257150 = [](std::int32_t n) -> double* {
        return n == 0 ? nullptr : static_cast<double*>(std::calloc(static_cast<std::size_t>(n), 8));
    };
}

struct Stats {
    int steps = 0, objects = 0, words = 0, nonzero = 0, bad = 0;
    std::map<std::string, int> nonzero_by_item; // coverage: nonzero DEF_PRE words per item
};

void compare(Stats& st, const std::string& where, const char* what, double mine, double ref)
{
    ++st.words;
    if (ref != 0.0) {
        ++st.nonzero;
        ++st.nonzero_by_item[what];
    }
    if (std::bit_cast<std::uint64_t>(mine) != std::bit_cast<std::uint64_t>(ref)) {
        ++st.bad;
        if (verbose() || st.bad <= 20)
            std::printf("  MISMATCH %s %s: port %.17g  DEF_PRE %.17g\n", where.c_str(), what, mine, ref);
    }
}

// Width of each object's STNCMP record in the input KEY ("STNCMP obj n width"): the model's
// W[0x28] when the header is written. The DB region's table width can be larger (GENDB counts
// STNOUT5 = 1 there), so the DB alone does not give it.
std::map<int, int> stncmp_widths(const std::string& key_path)
{
    std::map<int, int> w;
    std::ifstream in(key_path);
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("STNCMP", 0) != 0) continue;
        int obj = 0, n = 0, width = 0;
        if (std::sscanf(line.c_str() + 6, "%d %d %d", &obj, &n, &width) == 3 && n > 0) w[obj] = width;
    }
    return w;
}

// A step's objects and per-object real rows: 3D through deformdb's StepModel (region tables),
// 2D from the address tables at the start of MAR (DBMADR: IIAD1 = MAR[1 .. I1], IIAD2 next, with
// I1 = MAR[2] - MAR[1], I2 = MAR[3] - MAR[2]; starts then widths) and deformdb's 2D region names.
struct Obj {
    int number = 0;
    std::int32_t first_node = 0, node_count = 0, first_elem = 0, elem_count = 0;
};
struct Rows {
    std::vector<double> v;
    int width = 0;
};
class View {
public:
    virtual ~View() = default;
    virtual int dim() const = 0;
    virtual std::int32_t isim(std::size_t k) const = 0;
    virtual Rows rows(const Obj& o, const char* region, bool elem) const = 0;
    std::vector<Obj> objects;
};

class View3D : public View {
public:
    explicit View3D(const deformdb::StepData& sd) : sm_(sd), sim_(sd.mar_region("IISIM"))
    {
        for (const auto& oi : sm_.objects()) {
            if (oi.status != 1) continue;
            objects.push_back({oi.number, oi.first_node, oi.node_count, oi.first_elem, oi.elem_count});
            infos_[oi.number] = oi;
        }
    }
    bool supported() const { return sm_.supported(); }
    int dim() const override { return 3; }
    std::int32_t isim(std::size_t k) const override { return k < sim_.size() ? sim_[k] : 0; }
    Rows rows(const Obj& o, const char* region, bool elem) const override
    {
        const auto& oi = infos_.at(o.number);
        auto fv = elem ? sm_.elem_real(oi, region) : sm_.node_real(oi, region);
        Rows out;
        if (!fv) return out;
        for (std::size_t r = 0; r < fv.rows; ++r)
            for (int c = 0; c < fv.width; ++c) out.v.push_back(fv.at(r, c));
        out.width = fv.width;
        return out;
    }

private:
    deformdb::StepModel sm_;
    std::span<const std::int32_t> sim_;
    std::map<int, deformdb::ObjectInfo> infos_;
};

class View2D : public View {
public:
    explicit View2D(const deformdb::StepData& sd) : sd_(sd)
    {
        const auto& mar = sd.mar;
        if (mar.size() < 3) return;
        const std::int32_t i1 = mar[1] - mar[0], i2 = mar[2] - mar[1];
        if (i1 < 140 || i2 < 280 || static_cast<std::size_t>(i1) + static_cast<std::size_t>(i2) > mar.size()) return;
        tm_ = std::span<const std::int32_t>(mar).subspan(0, static_cast<std::size_t>(i1));
        ta_ = std::span<const std::int32_t>(mar).subspan(static_cast<std::size_t>(i1), static_cast<std::size_t>(i2));
        mnames_ = deformdb::mar_region_names(2, sd.entry.version);
        anames_ = deformdb::a_region_names(2, sd.entry.version);
        const int is = index_of(mnames_, "IISIM"), io = index_of(mnames_, "IIOBJ");
        if (is == 0 || io == 0) return;
        sim_start_ = tm_[static_cast<std::size_t>(is - 1)];
        const int nobj = sd.header.empty() ? 0 : static_cast<int>(sd.header[0]);
        const std::int32_t os = tm_[static_cast<std::size_t>(io - 1)];
        const std::int32_t ow = tm_[static_cast<std::size_t>(70 + io - 1)];
        for (int k = 0; k < nobj && ow >= 8; ++k) {
            const std::size_t b = static_cast<std::size_t>(os - 1 + k * ow);
            if (b + static_cast<std::size_t>(ow) > mar.size()) break;
            const std::int32_t* r = mar.data() + b;
            if (r[0] != 1 || r[4] <= 0 || r[6] <= 0) continue;
            objects.push_back({k + 1, r[4], r[5] - r[4] + 1, r[6], r[7] - r[6] + 1});
        }
        ok_ = true;
    }
    bool supported() const { return ok_; }
    int dim() const override { return 2; }
    std::int32_t isim(std::size_t k) const override
    {
        const std::size_t b = static_cast<std::size_t>(sim_start_ - 1) + k;
        return sim_start_ > 0 && b < sd_.mar.size() ? sd_.mar[b] : 0;
    }
    Rows rows(const Obj& o, const char* region, bool elem) const override
    {
        Rows out;
        const int i = index_of(anames_, region);
        if (i == 0) return out;
        const std::int32_t start = ta_[static_cast<std::size_t>(i - 1)];
        const std::int32_t w = ta_[static_cast<std::size_t>(140 + i - 1)];
        if (start <= 0 || w <= 0) return out;
        const std::int32_t first = elem ? o.first_elem : o.first_node;
        const std::int32_t n = elem ? o.elem_count : o.node_count;
        const std::size_t b = static_cast<std::size_t>(start - 1) +
                              static_cast<std::size_t>(first - 1) * static_cast<std::size_t>(w);
        const std::size_t len = static_cast<std::size_t>(n) * static_cast<std::size_t>(w);
        if (b + len > sd_.a.size()) return out;
        out.v.assign(sd_.a.begin() + static_cast<std::ptrdiff_t>(b),
                     sd_.a.begin() + static_cast<std::ptrdiff_t>(b + len));
        out.width = w;
        return out;
    }

private:
    static int index_of(std::span<const char* const> names, const char* region)
    {
        for (std::size_t k = 0; k < names.size(); ++k)
            if (names[k] != nullptr && std::strcmp(names[k], region) == 0) return static_cast<int>(k + 1);
        return 0;
    }
    const deformdb::StepData& sd_;
    std::span<const std::int32_t> tm_, ta_;
    std::span<const char* const> mnames_, anames_;
    std::int32_t sim_start_ = 0;
    bool ok_ = false;
};

// The model fields the header code reads, from IISIM: dimension, GEOTYP, STNOUT 1..7, ELMNOD.
void fill_model(Fake& f, const View& sm)
{
    const int dim = sm.dim();
    const bool d3 = dim == 3;
    auto isim = [&](std::size_t k) { return sm.isim(k); };
    f.dim = dim;
    at<std::int32_t>(f.model.p, 0x1c) = dim;
    at<std::int32_t>(f.model.p, 0x850) = d3 ? 0 : isim(2);
    for (int k = 0; k < 7; ++k)
        at<std::int32_t>(f.model.p, 0x86c + 4 * static_cast<std::size_t>(k)) = isim((d3 ? 50 : 52) + static_cast<std::size_t>(k));
    for (int k = 0; k < 10; ++k)
        at<std::int32_t>(f.model.p, 0x888 + 4 * static_cast<std::size_t>(k)) = isim((d3 ? 57 : 59) + static_cast<std::size_t>(k));
    f.o()->owner = f.m();
}

// Object `oi` with its state-variable arrays taken from the step's arrays.
void build_fake(Fake& f, const View& sm, const Obj& oi, const std::map<int, int>& key_stncmp)
{
    const int dim = sm.dim();
    const bool d3 = dim == 3;
    const bool g3 = !d3 && sm.isim(2) == 3;
    fill_model(f, sm);
    LLOBJ_DATA* o = f.o();
    o->u87c[0] = 1;
    o->u87c[1] = oi.node_count;
    o->u87c[2] = oi.elem_count;
    at<std::int32_t>(f.nset(), 300) = oi.node_count;
    at<std::int32_t>(f.eset(), 0xe8) = oi.elem_count;

    // node arrays: id -> region, model width
    struct Src {
        int id;
        const char* region;
        bool elem;
        std::size_t slot; // data slot in the set, 0 = through a hook
        std::int32_t w;
    };
    const std::int32_t vw = g3 ? 3 : dim;
    const Src src[] = {
        {0x01, "LLDRZ", false, 0x10, dim}, {0x02, "LLURZ", false, 0x18, vw},
        {0x08, "LLTMP", false, 0x40, 1},   {0x0d, "LLATM", false, 0x68, 1},
        {0x0f, "LLVLT", false, 0x78, 1},   {0x11, "LLND1", false, 0, 0},
        {0x26, "LLSTS", true, 0x20, g3 ? 6 : 2 * dim},
        {0x28, "LLTSR", true, 0x30, 0},    {0x2b, "LLTEP", true, 0x48, 1},
        {0x2c, "LLRDT", true, 0, 1},       {0x2d, "LLDMG", true, 0, 1},
        {0x2e, "LLMTA", true, 0, 1},       {0x38, "LLHDN", true, 0xb0, 2},
        {0x3c, "LLET1", true, 0, 0},
    };
    for (const Src& s : src) {
        Rows v = sm.rows(oi, s.region, s.elem);
        if (v.v.empty()) continue;
        f.arrays[s.id] = std::move(v.v);
        f.present.insert(s.id);
        f.width(s.id) = s.w > 0 ? s.w : v.width;
        if (s.id == 0x11) f.user_node_w = v.width;
        if (s.id == 0x3c) f.user_elem_w = v.width;
        if (s.slot != 0) at<double*>(s.elem ? f.eset() : f.nset(), s.slot) = f.arr(s.id);
    }
    if (f.arr(0x26) == nullptr) { // the stress getter allocates the array in the original
        const std::int32_t w = g3 ? 6 : 2 * dim;
        f.arrays[0x26].assign(static_cast<std::size_t>(oi.elem_count) * static_cast<std::size_t>(w), 0.0);
        f.width(0x26) = w;
        at<double*>(f.eset(), 0x20) = f.arr(0x26);
    }
    if (auto it = key_stncmp.find(oi.number); it != key_stncmp.end() && f.arr(0x28) != nullptr)
        f.width(0x28) = it->second;
    f.width(0x29) = 1;
    f.efsts.assign(static_cast<std::size_t>(oi.elem_count) + 1, 0.0);
    at<double*>(f.eset(), 0x38) = f.efsts.data();
}

void check_object(Stats& st, const std::string& where, const View& sm, const Obj& oi, const double* blk,
                  const std::map<int, int>& key_stncmp)
{
    const bool d3 = sm.dim() == 3;
    Fake f;
    build_fake(f, sm, oi, key_stncmp);
    g_fake = &f;
    LLOBJ_DATA* o = f.o();
    auto slot = [](std::uint64_t a) { return p407::hdr_slot(a); };
    auto mm = [&](const char* what, std::int32_t code, std::int32_t comp, std::uint64_t lo, std::uint64_t hi,
                  int need_id) {
        if (f.present.count(need_id) == 0) return;
        double vmin = 0.0, vmax = 0.0;
        const bool init = true;
        p407::obj_min_max(o, &code, &vmin, &vmax, &comp, &init);
        compare(st, where, (std::string(what) + " min").c_str(), vmin, blk[slot(lo)]);
        compare(st, where, (std::string(what) + " max").c_str(), vmax, blk[slot(hi)]);
    };

    if (!d3) mm("1600", 0x640, 0, 0x1406e8608, 0x1406e860c, 0x2e);
    mm("velocity", 700, 0, 0x1406e8540, 0x1406e8544, 0x02);
    mm("displacement", 800, 0, 0x1406e8548, 0x1406e854c, 0x01);
    mm("temperature", 600, 0, 0x1406e8550, 0x1406e8554, 0x08);
    mm("density", 200, 0, 0x1406e8560, 0x1406e8564, 0x2c);
    if (f.present.count(0x26) != 0) {
        const std::int32_t code = 500, comp = 0;
        const bool init = true;
        double a = 0.0, b = 0.0, scratch = 0.0;
        std::int32_t kind = 0;
        p407::obj_stress_min_max(o, &code, &kind, &a, &b, &comp, &init);
        compare(st, where, "stress eff min", a, blk[slot(0x1406e8568)]);
        compare(st, where, "stress eff max", b, blk[slot(0x1406e856c)]);
        kind = 1;
        p407::obj_stress_min_max(o, &code, &kind, &a, &b, &comp, &init);
        compare(st, where, "stress mean min", a, blk[slot(0x1406e861c)]);
        compare(st, where, "stress mean max", b, blk[slot(0x1406e8620)]);
        kind = 4;
        p407::obj_stress_min_max(o, &code, &kind, &a, &scratch, &comp, &init);
        compare(st, where, "stress p-min min", a, blk[slot(0x1406e8614)]);
        kind = 2;
        p407::obj_stress_min_max(o, &code, &kind, &scratch, &b, &comp, &init);
        compare(st, where, "stress p-max max", b, blk[slot(0x1406e8618)]);
        double out[12] = {};
        std::int32_t c500 = 500;
        p407::obj_component_min_max(o, &c500, out);
        const int nc = f.width(0x26) < 7 ? f.width(0x26) : 6;
        for (int k = 0; k < 2 * nc; ++k) compare(st, where, "stress comp", out[k], blk[slot(0x1406e8680) + k]);
    }
    if (f.present.count(0x2b) != 0 && at<std::int32_t>(f.model.p, 0x88c) != 1 && at<std::int32_t>(f.model.p, 0x88c) != 2)
        mm("eff strain", 300, 0, 0x1406e8570, 0x1406e8574, 0x2b);
    if (f.present.count(0x28) != 0) {
        double out[12] = {};
        std::int32_t c300 = 300;
        p407::obj_component_min_max(o, &c300, out);
        const int nc = f.width(0x28) < 7 ? f.width(0x28) : 6;
        for (int k = 0; k < 2 * nc; ++k) compare(st, where, "strain comp", out[k], blk[slot(0x1406e867c) + k]);
    }
    if (f.user_node_w > 0) {
        mm("user node 1", 900, 1, 0x1406e8580, 0x1406e8584, 0x11);
        mm("user node 2", 900, 2, 0x1406e8588, 0x1406e858c, 0x11);
    }
    if (f.user_elem_w > 0) {
        mm("user elem 1", 1000, 1, 0x1406e8590, 0x1406e8594, 0x3c);
        mm("user elem 2", 1000, 2, 0x1406e8598, 0x1406e859c, 0x3c);
    }
    if (at<std::int32_t>(f.model.p, 0x888) != 1 && at<std::int32_t>(f.model.p, 0x888) != 2)
        mm("damage", 100, 0, 0x1406e85a0, 0x1406e85a4, 0x2d);
    mm("2600", 0xa28, 0, 0x1406e85a8, 0x1406e85ac, 0x0d);
    mm("2800 c1", 0xaf0, 1, 0x1406e85b0, 0x1406e85b4, 0x38);
    mm("2800 c2", 0xaf0, 2, 0x1406e85b8, 0x1406e85bc, 0x38);
    mm("5000", 5000, 0, 0x1406e85c0, 0x1406e85c4, 0x0f);
    g_fake = nullptr;
}

// The header writer itself (1401e2180) on all objects of the step: every statistic the per-object
// check covers must land in DEF_PRE's slot, and the global words it computes must match.
void check_writer(Stats& st, const std::string& where, const View& sm, const std::vector<double>& ref,
                  const std::map<int, int>& key_stncmp)
{
    const int dim = sm.dim();
    const bool d3 = dim == 3;
    std::int32_t nobj = static_cast<std::int32_t>(ref[0]);
    std::vector<std::unique_ptr<Fake>> fakes;
    std::vector<const Obj*> meshed(static_cast<std::size_t>(nobj), nullptr);
    for (const Obj& oi : sm.objects)
        if (oi.number >= 1 && oi.number <= nobj && oi.node_count > 0 && oi.elem_count > 0)
            meshed[static_cast<std::size_t>(oi.number - 1)] = &oi;
    for (std::int32_t k = 0; k < nobj; ++k) {
        auto f = std::make_unique<Fake>();
        if (meshed[static_cast<std::size_t>(k)] != nullptr) {
            build_fake(*f, sm, *meshed[static_cast<std::size_t>(k)], key_stncmp);
        } else {
            fill_model(*f, sm);
        }
        if (k > 0) fakes.back()->o()->next = f->o();
        fakes.push_back(std::move(f));
    }
    g_fakes.clear();
    for (auto& f : fakes) g_fakes.push_back(f.get());

    Raw list(sizeof(def::model::Cls_14017a6c0));
    auto* objects = reinterpret_cast<def::model::Cls_14017a6c0*>(list.p);
    objects->head = nobj > 0 ? fakes.front()->o() : nullptr;
    objects->count = nobj;
    Raw dbraw(sizeof(def::model::Cls_1401dbfe0));
    auto* db = reinterpret_cast<def::model::Cls_1401dbfe0*>(dbraw.p);
    db->uae0 = -1; // a fresh DB object (1401dc030), as in GENDB
    Raw simraw(sizeof(def::model::Cls_1401e7b10));
    for (int k = 0; k < 10; ++k)
        at<std::int32_t>(simraw.p, 0x138 + 4 * static_cast<std::size_t>(k)) = sm.isim((d3 ? 57 : 59) + static_cast<std::size_t>(k));
    std::int32_t fold = d3 ? static_cast<std::int32_t>(ref[8]) : 0;
    std::int32_t d = dim;
    std::int32_t version = 11000;
    std::int32_t hsize = 0;
    std::vector<double> h(100 + 400 * static_cast<std::size_t>(nobj), -1.0);
    def::dbio::gendb::fill_step_header(db, nullptr, &hsize, h.data(), 0,
                                       reinterpret_cast<def::model::Cls_1401e7b10*>(simraw.p), &nobj, &fold,
                                       objects, &d, &version);
    g_fakes.clear();

    auto slot = [](std::uint64_t a) { return static_cast<std::size_t>(p407::hdr_slot(a)); };
    compare(st, where, "w hsize", static_cast<double>(hsize), static_cast<double>(ref.size()));
    compare(st, where, "w H0", h[0], ref[0]);
    compare(st, where, "w platform", h[slot(0x1406e8524)], ref[slot(0x1406e8524)]);
    compare(st, where, "w H9", h[9], ref[9]);
    compare(st, where, "w H12", h[12], ref[12]);
    if (d3) compare(st, where, "w fold", h[8], ref[8]);
    for (std::int32_t k = 0; k < nobj; ++k) {
        const Obj* oi = meshed[static_cast<std::size_t>(k)];
        if (oi == nullptr) continue;
        Fake& f = *fakes[static_cast<std::size_t>(k)];
        const double* mine = h.data() + 100 + 400 * static_cast<std::size_t>(k);
        const double* blk = ref.data() + 100 + 400 * static_cast<std::size_t>(k);
        const std::string w = where + " obj " + std::to_string(k + 1);
        auto pair = [&](const char* what, std::uint64_t lo, std::uint64_t hi, int need_id) {
            if (f.present.count(need_id) == 0) return;
            compare(st, w, (std::string("w ") + what + " min").c_str(), mine[slot(lo)], blk[slot(lo)]);
            compare(st, w, (std::string("w ") + what + " max").c_str(), mine[slot(hi)], blk[slot(hi)]);
        };
        compare(st, w, "w nodes", mine[slot(0x1406e8538)], blk[slot(0x1406e8538)]);
        compare(st, w, "w elements", mine[slot(0x1406e853c)], blk[slot(0x1406e853c)]);
        if (!d3) {
            pair("1600", 0x1406e8608, 0x1406e860c, 0x2e);
            compare(st, w, "w 2D slot 0 (area over FORCE x)", mine[0], blk[0]);
        }
        pair("velocity", 0x1406e8540, 0x1406e8544, 0x02);
        pair("displacement", 0x1406e8548, 0x1406e854c, 0x01);
        pair("temperature", 0x1406e8550, 0x1406e8554, 0x08);
        pair("density", 0x1406e8560, 0x1406e8564, 0x2c);
        if (f.present.count(0x26) != 0) {
            pair("stress eff", 0x1406e8568, 0x1406e856c, 0x26);
            pair("stress mean", 0x1406e861c, 0x1406e8620, 0x26);
            compare(st, w, "w stress p-min", mine[slot(0x1406e8614)], blk[slot(0x1406e8614)]);
            compare(st, w, "w stress p-max", mine[slot(0x1406e8618)], blk[slot(0x1406e8618)]);
            const std::size_t nc = static_cast<std::size_t>(f.width(0x26) < 7 ? f.width(0x26) : 6);
            for (std::size_t c = 0; c < 2 * nc; ++c)
                compare(st, w, "w stress comp", mine[slot(0x1406e8680) + c], blk[slot(0x1406e8680) + c]);
        }
        if (at<std::int32_t>(f.model.p, 0x88c) != 1 && at<std::int32_t>(f.model.p, 0x88c) != 2)
            pair("eff strain", 0x1406e8570, 0x1406e8574, 0x2b);
        if (f.present.count(0x28) != 0) {
            const std::size_t nc = static_cast<std::size_t>(f.width(0x28) < 7 ? f.width(0x28) : 6);
            for (std::size_t c = 0; c < 2 * nc; ++c)
                compare(st, w, "w strain comp", mine[slot(0x1406e867c) + c], blk[slot(0x1406e867c) + c]);
        }
        if (f.user_node_w > 0) {
            pair("user node 1", 0x1406e8580, 0x1406e8584, 0x11);
            pair("user node 2", 0x1406e8588, 0x1406e858c, 0x11);
        }
        if (f.user_elem_w > 0) {
            pair("user elem 1", 0x1406e8590, 0x1406e8594, 0x3c);
            pair("user elem 2", 0x1406e8598, 0x1406e859c, 0x3c);
        }
        if (at<std::int32_t>(f.model.p, 0x888) != 1 && at<std::int32_t>(f.model.p, 0x888) != 2)
            pair("damage", 0x1406e85a0, 0x1406e85a4, 0x2d);
        pair("2600", 0x1406e85a8, 0x1406e85ac, 0x0d);
        pair("2800 c1", 0x1406e85b0, 0x1406e85b4, 0x38);
        pair("2800 c2", 0x1406e85b8, 0x1406e85bc, 0x38);
        pair("5000", 0x1406e85c0, 0x1406e85c4, 0x0f);
    }
}

} // namespace

TEST_CASE(step_header_statistics_match_gendb)
{
    const fs::path cache = cache_dir();
    if (cache.empty() || !fs::exists(cache / "entries")) {
        std::printf("note: no oracle cache, skipped\n");
        return;
    }
    install_hooks();
    Stats st;
    int cases = 0;
    for (const auto& shard : fs::directory_iterator(cache / "entries")) {
        if (!shard.is_directory()) continue;
        for (const auto& e : fs::directory_iterator(shard.path())) {
            const fs::path rj = e.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = slurp(rj);
            const std::string label = json_string(j, "label");
            if (label.rfind("p4-07/gendb/", 0) != 0) continue;
            const fs::path dbp = e.path() / "files" / "NEW.DB";
            if (!fs::exists(dbp)) continue;
            ++cases;
            const std::map<int, int> key_stncmp = stncmp_widths(json_string(j, "source"));
            auto db = deformdb::Database::open(dbp);
            for (const auto& ent : db->entries()) {
                if (ent.mar_size <= 0 || ent.a_size <= 0) continue;
                deformdb::StepData sd = db->read_step(ent);
                if (sd.header.size() < 100) continue;
                std::unique_ptr<View> v;
                if (ent.dim == 3) {
                    auto v3 = std::make_unique<View3D>(sd);
                    if (!v3->supported()) continue;
                    v = std::move(v3);
                } else {
                    auto v2 = std::make_unique<View2D>(sd);
                    if (!v2->supported()) continue;
                    v = std::move(v2);
                }
                ++st.steps;
                std::int32_t dim = ent.dim;
                p407::load_hdr_slots(&dim);
                for (const Obj& oi : v->objects) {
                    if (oi.node_count <= 0 || oi.elem_count <= 0) continue;
                    const std::size_t off = 100 + 400 * static_cast<std::size_t>(oi.number - 1);
                    if (off + 400 > sd.header.size()) continue;
                    ++st.objects;
                    check_object(st, label.substr(12) + " step " + std::to_string(ent.step) + " obj " +
                                         std::to_string(oi.number),
                                 *v, oi, sd.header.data() + off, key_stncmp);
                }
                if (static_cast<std::size_t>(100 + 400 * static_cast<std::int64_t>(sd.header[0])) == sd.header.size())
                    check_writer(st, label.substr(12) + " step " + std::to_string(ent.step), *v, sd.header, key_stncmp);
            }
        }
    }
    std::printf("p4-07 oracle: %d cases, %d steps, %d objects, %d words compared (%d nonzero), %d mismatches\n",
                cases, st.steps, st.objects, st.words, st.nonzero, st.bad);
    for (const auto& [item, n] : st.nonzero_by_item) std::printf("  %-20s %d nonzero\n", item.c_str(), n);
    p407::reset_ext_p4_07();
    p407::reset_globals_p4_07();
    CHECK_EQ(st.bad, 0);
}

int main() { return testkit::run_all(); }
