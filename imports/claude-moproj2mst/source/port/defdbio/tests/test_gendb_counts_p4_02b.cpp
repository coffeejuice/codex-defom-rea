// Tier 1 for packet p4-02b (GENDB counts frame 1401bc230 and its counters). Models are built on
// zeroed storage (as the p1-03 tests do); calls into other packets go through the ext table with
// test stand-ins. Expected values are worked out by hand from the decompiles (shown per check).
// ctest-labels: p4-02b gendb
#include <defdbio/gendb/counts_ext.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

using namespace def::dbio::gendb;
using def::model::field;

namespace {

template <class T>
T* zeroed()
{
    void* p = std::calloc(1, sizeof(T));
    return static_cast<T*>(p);
}

struct Model {
    DB_STEP_DATA* m = zeroed<DB_STEP_DATA>();
    std::vector<LLOBJ_DATA*> objs;
    std::vector<Material*> mats;

    explicit Model(std::int32_t dim)
    {
        m->dimension = dim;
        m->materials.owner = m;
        m->phase_pairs.owner = m;
        m->objects.owner = m;
        m->inter_obj.owner = m;
        m->sim_ctrl.owner = m;
    }
    LLOBJ_DATA* add_object(std::int32_t number, std::int32_t kind)
    {
        auto* o = zeroed<LLOBJ_DATA>();
        field<std::int32_t>(o, 8) = number;
        field<std::int32_t>(o, 0xc) = kind;
        o->owner = m;
        if (m->objects.head == nullptr) {
            m->objects.head = o;
        } else {
            m->objects.tail->next = o;
        }
        m->objects.tail = o;
        m->objects.count = m->objects.count + 1;
        m->u1378 = m->u1378 + 1;
        objs.push_back(o);
        return o;
    }
    Material* add_material(std::int32_t number)
    {
        auto* mat = zeroed<Material>();
        mat->number = number;
        if (m->materials.head == nullptr) {
            m->materials.head = mat;
        } else {
            m->materials.tail->next = mat;
        }
        m->materials.tail = mat;
        m->materials.count = m->materials.count + 1;
        mats.push_back(mat);
        return mat;
    }
};

void set_mesh(LLOBJ_DATA* o, std::int32_t nodes, std::int32_t elems, std::int32_t width, std::int32_t* groups)
{
    field<std::int32_t>(o, 0x8a0 + 0x12c) = nodes;
    field<std::int32_t>(o, 0x9e0 + 0xe8) = elems;
    field<std::int32_t>(o, 0x9e0 + 0xec) = width;
    field<std::int32_t*>(o, 0x9f8) = groups;
}

int g_calls_134120 = 0;
std::int32_t g_w2090 = -1, g_w21c0 = -1;
bool g_beb30 = false;

void install_stubs()
{
    reset_gendb_ext();
    GendbExt& x = gendb_ext();
    x.fun_1401f3d90 = [](def::model::Cls_1401e7b10*) { return false; };
    x.fun_1401f3f80 = [](def::model::Cls_1401e7b10* s) { s->smode = 7; };
    x.fun_1401d2090 = [](DB_STEP_DATA*, std::int32_t w) { g_w2090 = w; };
    x.fun_1401d21c0 = [](DB_STEP_DATA*, std::int32_t w) { g_w21c0 = w; };
    x.fun_1401beb30 = [](DB_STEP_DATA*, std::int32_t*, std::int32_t*, std::int32_t*, std::int32_t*,
                         std::int32_t*) { g_beb30 = true; };
    x.fun_140134120 = [](DYNARY*) { ++g_calls_134120; };
}

} // namespace

// ---- DYNARY lengths ---------------------------------------------------------------------------

TEST_CASE(dynary_lengths)
{
    install_stubs();
    DYNARY t{};
    t.n[0] = 2;
    t.n[1] = 3;
    t.n[2] = 0;
    t.ncomp = 4;
    CHECK_EQ(dynary_length_3d_comp(&t), 29); // 2 + 3 + 0 + 2 * 3 * 1 * 4
    DYNARY a{};
    a.ndim = 1;
    a.n[0] = 3;
    a.ncomp = 2;
    CHECK_EQ(dynary_length_plus10(&a), 20); // 140130230: 3 * 3 = 9, odd -> 10; + 10
    a.ndim = 2;
    a.n[1] = 2;
    a.ncomp = 1;
    CHECK_EQ(dynary_length_plus10(&a), 21); // 140130160: 3 + 2 + 3 * 2 * 1 = 11; + 10
    a.ndim = 0;
    CHECK_EQ(dynary_length_plus10(&a), 10);
    std::int32_t n = 1, len = 5;
    a.ndim = 1;
    a.n[0] = 2;
    a.ncomp = 2;
    dynary_count_plus10(&a, &n, &len); // 2 * 3 = 6 even; + 10
    CHECK_EQ(n, 2);
    CHECK_EQ(len, 21);
}

// ---- materials --------------------------------------------------------------------------------

TEST_CASE(material_marking_and_renumbering)
{
    install_stubs();
    Model md(3);
    Material* m5 = md.add_material(5);
    Material* m2 = md.add_material(2);
    Material* m9 = md.add_material(9);
    MaterialList* l = &md.m->materials;
    CHECK_EQ(material_list_max_number(l), 9);

    std::int32_t comp[2] = {5, 9};
    m2->mstmtr_count = 2;
    m2->mstmtr_list = comp;
    field<std::int32_t>(m2, 4) = 3;
    field<std::int32_t>(m2, 8) = 0x11;
    CHECK_EQ(material_list_mark_mixtures(l), 1);
    CHECK_EQ(field<std::int32_t>(m5, 4), 1); // used flag copied as 0/1
    CHECK_EQ(field<std::int32_t>(m9, 8), 0x11);

    field<std::int32_t>(m2, 4) = 0; // only 5 and 9 used
    CHECK_EQ(material_list_renumber(l), 2);
    CHECK_EQ(field<std::int32_t>(m5, 4), 1);
    CHECK_EQ(field<std::int32_t>(m9, 4), 2);
    CHECK_EQ(field<std::int32_t>(m2, 4), 0);

    material_list_clear_flags(l);
    CHECK_EQ(field<std::int32_t>(m9, 4), 0);
    CHECK_EQ(field<std::int32_t>(m9, 8), 0);
    m2->mstmtr_list = nullptr;
}

TEST_CASE(material_function_counts_tables)
{
    install_stubs();
    g_calls_134120 = 0;
    auto* mat = zeroed<Material>();
    std::int32_t n = 0, len = 0, err = 0;
    material_function_counts(mat, &n, &len, &err);
    CHECK_EQ(n, 3);   // 0x2160, 0x17f0, 0x1878: one function each
    CHECK_EQ(len, 30); // ndim 0 -> 0 + 10 each

    auto* fs = reinterpret_cast<char*>(mat) + 0x50;
    field<std::int32_t>(fs, 0) = 0xd;
    auto* t598 = reinterpret_cast<DYNARY*>(fs + 0x598);
    t598->n[0] = 2;
    t598->n[1] = 2;
    t598->n[2] = 2;
    t598->ncomp = 3;
    auto* young = reinterpret_cast<DYNARY*>(reinterpret_cast<char*>(mat) + 0x670);
    young->ndim = 1;
    young->n[0] = 4;
    auto* heatcp = reinterpret_cast<DYNARY*>(reinterpret_cast<char*>(mat) + 0x858);
    heatcp->ndim = 2;
    heatcp->n[0] = 2;
    heatcp->n[1] = 3;
    heatcp->ncomp = 1;
    n = 0;
    len = 0;
    material_function_counts(mat, &n, &len, &err);
    CHECK_EQ(n, 6);
    // 30 (0x598: 6 + 8 * 3) + 8 (YOUNG 2 * 4) + 11 (HEATCP 2 + 3 + 6) + 30
    CHECK_EQ(len, 79);
    CHECK_EQ(g_calls_134120, 1);
    std::free(mat);
}

TEST_CASE(material_part_counts)
{
    install_stubs();
    // hardness 14028a080 form 2: p[0x12] * p[9] * p[10] + p[9] + p[10]
    std::int32_t h[0x20] = {};
    h[0] = 1;
    h[3] = 2;
    h[9] = 2;
    h[10] = 3;
    h[0x12] = 4;
    std::int32_t n = 0, len = 0;
    hardness_counts(h, &n, &len);
    CHECK_EQ(n, 1);
    CHECK_EQ(len, 29);
    // diffusion 140289a40 form 2: table at +0x80, n0 * (ncomp + 1) -> even
    std::vector<std::int32_t> d(0x60, 0);
    d[0] = 2;
    d[0x20 + 7] = 3;
    d[0x20 + 0x10] = 2;
    n = 0;
    len = 0;
    diffusion_counts(d.data(), &n, &len);
    CHECK_EQ(n, 1);
    CHECK_EQ(len, 10); // 9 -> 10
    // diffusion form 4: counted with length 0
    d[0] = 4;
    diffusion_counts(d.data(), &n, &len);
    CHECK_EQ(n, 2);
    CHECK_EQ(len, 10);
    // creep 140289d80 forms 7/8: three functions, the counter of the last is not added twice
    std::vector<std::int32_t> c(0x120, 0);
    c[0] = 7;
    n = 0;
    len = 0;
    creep_counts(c.data(), &n, &len);
    CHECK_EQ(n, 3);
    CHECK_EQ(len, 0);
    // misc 14028c240 kind 0xc: fixed 9
    std::vector<std::int32_t> p(0x60, 0);
    p[0] = 0xc;
    n = 0;
    len = 0;
    material_misc_counts(p.data(), &n, &len);
    CHECK_EQ(n, 1);
    CHECK_EQ(len, 9);
    // grain mode 1: nine 1D tables n0 * (ncomp + 1)
    std::vector<char> g(0x800, 0);
    field<std::int32_t>(g.data(), 0x770) = 1;
    field<std::int32_t>(g.data(), 0x2c) = 2;
    field<std::int32_t>(g.data(), 0x50) = 1;
    field<std::int32_t>(g.data(), 0x3ec) = 3;
    field<std::int32_t>(g.data(), 0x410) = 2;
    CHECK_EQ(grain_table_length(g.data()), 13); // 2 * 2 + 3 * 3
    // grain record 14028a650: 0x25 + tables (all 0) when +0x1e0 == 0
    std::vector<char> r(0x200, 0);
    CHECK_EQ(grain_record_length(r.data()), 0x25);
}

// ---- phase pairs ------------------------------------------------------------------------------

TEST_CASE(phase_pairs)
{
    install_stubs();
    Model md(2);
    Material* a = md.add_material(1);
    md.add_material(2);
    a->mstmtr_flag = 1; // mixture
    PhasePairList* l = &md.m->phase_pairs;
    auto add = [&](std::int32_t from, std::int32_t to, std::int32_t ph) {
        // the ctor 140333f60 is not on the base: zeroed storage from operator new, which the
        // list's delete (14033a990) releases
        auto* p = static_cast<PhasePair*>(::operator new(sizeof(PhasePair)));
        std::memset(static_cast<void*>(p), 0, sizeof(PhasePair));
        p->from = from;
        p->to = to;
        p->phastf = ph;
        p->next = nullptr;
        if (l->head == nullptr) {
            l->head = p;
        } else {
            l->tail->next = p;
        }
        l->tail = p;
        l->count = l->count + 1;
        return p;
    };
    add(1, 2, 1); // mixture with PHASTF: dropped
    add(2, 3, 0); // material 3 missing: dropped
    PhasePair* keep = add(2, 2, 1);
    add(0, 2, 1); // dropped
    std::int32_t nrel = -1, err = 0;
    relation_count(md.m, &nrel, &err);
    CHECK_EQ(nrel, 1);
    CHECK_EQ(l->count, 1);
    CHECK(l->head == keep && l->tail == keep);

    field<std::int32_t>(keep, 0x14) = 0;
    field<std::int32_t>(keep, 0x94) = 2;
    field<std::int32_t>(keep, 0x9c) = 3;
    CHECK_EQ(phase_pair_kinetics_length(keep), 11); // 2 * 1 * 3 * 1 + 5 + 0
    field<std::int32_t>(keep, 0x14) = 6;
    field<std::int32_t>(keep, 0x2e4) = 4;
    CHECK_EQ(phase_pair_kinetics_length(keep), 12);
    field<std::int32_t>(keep, 0x14) = 0;
    auto* t = reinterpret_cast<DYNARY*>(reinterpret_cast<char*>(keep) + 0x478);
    t->ndim = 1;
    t->n[0] = 2;
    std::int32_t n = 0, len = 0;
    phase_pair_counts(keep, &n, &len);
    CHECK_EQ(n, 2);
    CHECK_EQ(len, 15); // 11 + 2 * 2
    t->ndim = 0;
}

// ---- simulation control -----------------------------------------------------------------------

TEST_CASE(sim_ctrl_function_counts_basic)
{
    install_stubs();
    Model md(3);
    Cls_1401e7b10& s = md.m->sim_ctrl;
    s.envtmp.ndim = 1;
    s.envtmp.n[0] = 2;
    s.envtmp.ncomp = 1;
    s.timeid.ndim = 1;
    s.timeid.n[0] = 2;
    s.timeid.ncomp = 1;
    s.frqncy_rows = 6;
    s.frqncy_cols = 1;
    s.frqncy.type = 0xb5;
    s.frqncy.ncomp = 6;
    std::int32_t n = 0, len = 0;
    sim_ctrl_counts(&s, &n, &len);
    CHECK_EQ(n, 3);
    CHECK_EQ(len, 24); // ENVTMP 2 values * 2 + TIMEID (2 * 2 + 10) + FRQNCY 6
    CHECK_EQ(sim_ctrl_smode(&s), 7);
}

// ---- objects ----------------------------------------------------------------------------------

TEST_CASE(boundary_node_list_3d)
{
    install_stubs();
    Model md(3);
    LLOBJ_DATA* o = md.add_object(1, 0);
    auto* b = reinterpret_cast<Cls_14017e740*>(&o->sbe0[0][0]);
    std::int32_t faces[8] = {1, 3, 5, 7, 3, 5, 9, 1};
    b->u010 = 2;
    b->u018 = faces;
    b->u028 = md.m;
    std::int32_t nn = 10;
    boundary_node_list(b, &nn);
    CHECK_EQ(b->u000, 5);
    auto* list = static_cast<std::int32_t*>(b->u008);
    CHECK_EQ(list[0], 1);
    CHECK_EQ(list[4], 9);
    void* before = b->u008;
    boundary_node_list(b, &nn); // same count: the array is kept
    CHECK(b->u008 == before);
    b->u018 = nullptr;
}

TEST_CASE(counts_3d_object)
{
    install_stubs();
    Model md(3);
    md.add_material(1);
    LLOBJ_DATA* o = md.add_object(1, 0);
    std::int32_t groups[2] = {1, 1};
    set_mesh(o, 10, 2, 8, groups);
    o->u87c[1] = 10;
    auto* b = reinterpret_cast<Cls_14017e740*>(&o->sbe0[0][0]);
    std::int32_t faces[4] = {1, 3, 5, 7};
    b->u010 = 1;
    b->u018 = faces;
    b->u028 = md.m;
    md.m->sim_ctrl.trans[0] = 1;
    md.m->sim_ctrl.smode = 1;

    std::int32_t err = 0;
    g_beb30 = false;
    counts_frame(md.m, &err);
    CHECK_EQ(err, 0);
    DB_STEP_DATA* m = md.m;
    CHECK_EQ(m->u074[2], 1);  // NOBJ 0x7c
    CHECK_EQ(m->u06c, 10);    // NUMNP
    CHECK_EQ(m->u070, 2);     // NUMEL
    CHECK_EQ(m->u074[11], 16); // 0xa0: 2 elements * 8
    CHECK_EQ(m->u074[16], 16); // 0xb4: width 8 -> 8 per element
    CHECK_EQ(m->u074[3], 1);  // 0x80: one meshed object
    CHECK_EQ(m->u074[0], 11); // 0x74 = NUMNP + 0x80
    CHECK_EQ(m->u074[7], 4);  // 0x90 boundary nodes
    CHECK_EQ(m->u074[8], 1);  // 0x94 boundary faces
    CHECK_EQ(m->u074[1], 1);  // 0x78 materials: material 1 used by the groups
    CHECK_EQ(field<std::int32_t>(md.mats[0], 8), 0x100); // TRANS(1) = 1
    CHECK_EQ(m->u074[18], 0); // 0xbc relations
    CHECK_EQ(m->u0c8, 0);     // material groups
    CHECK_EQ(m->u0d8, 2);
    CHECK_EQ(m->u0dc, 2);
    CHECK_EQ(g_w2090, 2);
    CHECK_EQ(g_w21c0, 2);
    CHECK_EQ(m->u074[5], 3);  // NFN: the used material's three fixed functions
    CHECK_EQ(m->u074[6], 30);
    CHECK_EQ(m->u134, 1);
    CHECK_EQ(m->u138, 1);
    CHECK_EQ(m->u13c, 7);     // SMODE from 1401f3f80
    CHECK_EQ(m->version, 0x44c);
    CHECK(g_beb30);
    CHECK_EQ(m->u020, 1);     // modules available (licence omitted)
    b->u018 = nullptr;
}

TEST_CASE(counts_frame_errors)
{
    install_stubs();
    Model md(3);
    std::int32_t err = 0;
    counts_frame(md.m, &err); // TRANS(1..4) all 0
    CHECK_EQ(err, 1);
    CHECK_EQ(md.m->u074[2], 0);

    md.m->sim_ctrl.trans[1] = 1;
    md.add_object(101, 0);
    err = 0;
    counts_frame(md.m, &err); // object number 101 > 100
    CHECK_EQ(err, 1);
    CHECK_EQ(max_object_number(md.m), 101);
    md.add_object(102, 7); // kind 7 is skipped
    CHECK_EQ(max_object_number(md.m), 101);
}

TEST_CASE(counts_2d_object)
{
    install_stubs();
    static OBJ_GEO2D* curves[2];
    curves[0] = new OBJ_GEO2D();
    curves[0]->curve_type = 2;
    curves[0]->n_points = 5;
    curves[1] = new OBJ_GEO2D();
    curves[1]->curve_type = 0;
    curves[1]->n_points = 3;
    GendbExt& x = gendb_ext();
    x.fun_140198310 = [](LLOBJ_DATA*) { return 2; };
    x.fun_1401982c0 = [](LLOBJ_DATA*, const std::int32_t* i) { return curves[*i]; };
    x.fun_140160d20 = [](LLOBJ_DATA*) { return true; };
    x.fun_140304e40 = [](LLOBJ_DATA*, std::int32_t) { return false; };

    Model md(2);
    // 1401bdaf0 looks objects up by NUMBER 1..u1378 (14017aaf0), so an object numbered above the
    // object count is not counted; it is numbered 1 here.
    LLOBJ_DATA* o = md.add_object(1, 0);
    set_mesh(o, 10, 4, 4, nullptr);
    field<std::int32_t>(o, 0xbd8) = 2;
    field<std::int32_t>(o, 0xbe0) = 3;
    field<std::int32_t>(o, 0xbe0 + 0x30) = 4;
    md.m->sim_ctrl.trans[1] = 1;

    std::int32_t nobj = 0, numnp = 0, numel = 0, nbcrv = 0, nbseg = 0, ntot = 0, nout = 0,
                 ndof = 0, ndof2 = 0, err = 5;
    object_counts_2d(md.m, &nobj, &numnp, &numel, &nbcrv, &nbseg, &ntot, &nout, &ndof, &ndof2, &err);
    CHECK_EQ(err, 0);
    CHECK_EQ(nobj, 1);
    CHECK_EQ(numnp, 10);
    CHECK_EQ(numel, 4);
    CHECK_EQ(nbcrv, 2);
    CHECK_EQ(nbseg, 7);
    CHECK_EQ(nout, 8);   // both curves have points and are not extensions
    CHECK_EQ(ntot, 12);
    CHECK_EQ(ndof, 2);
    CHECK_EQ(ndof2, 4);
    md.m->sim_ctrl.geotyp = 3;
    object_counts_2d(md.m, &nobj, &numnp, &numel, &nbcrv, &nbseg, &ntot, &nout, &ndof, &ndof2, &err);
    CHECK_EQ(ndof, 3);
    CHECK_EQ(ndof2, 6);
}

// ---- inter-object records ---------------------------------------------------------------------

TEST_CASE(inter_object_counts_one_pair)
{
    install_stubs();
    Model md(3);
    md.add_object(1, 0);
    md.m->u074[2] = 1; // NOBJ (1401f4610)
    auto* r = static_cast<Cls_1401f46e0*>(std::calloc(1, 0x3a0));
    field<std::int32_t>(r, 0x168) = 1;
    field<std::int32_t>(r, 0x16c) = 1;
    field<std::int32_t>(r, 0x170) = 1;
    field<std::int32_t>(r, 0x340) = 1;
    field<DB_STEP_DATA*>(r, 0x398) = md.m;
    auto* t = reinterpret_cast<DYNARY*>(reinterpret_cast<char*>(r) + 0x18);
    t->ndim = 1;
    t->n[0] = 3;
    md.m->inter_obj.head = r;
    md.m->inter_obj.count = 1;
    std::int32_t n = 0, len = 0, nwin = 0, lwin = 0, err = 0;
    inter_object_function_counts(md.m, &n, &len, &nwin, &lwin, &err);
    CHECK_EQ(n, 2);
    CHECK_EQ(len, 10); // 2 * 3 + 4
    CHECK_EQ(md.m->u0e4, 1);
}

int main() { return testkit::run_all(); }
