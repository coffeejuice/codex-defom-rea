// Tier 1 for packet p4-04: GENDB checks 1 (simulation control) and 5 (inter-object data).
//
// Models are built on zeroed storage (no ctors; the checks only read fields) and the fields each
// case needs are set by hand. stdout is redirected to a per-process temp file around each call,
// since the checks print there (__iob_func() + 0x30). Expected texts and flags are worked out by
// hand from the decompile (deform-rea-work/out/p404/p404.c) and the listing; the strings are the
// binary's (checked byte for byte against DEF_PRE_64.exe by tools in the packet notes).
// ctest-labels: p4-04 gendb
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/gendb/checks_p4_04.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "../src/gendb/p4_04/ext_p4_04.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <io.h>
#include <process.h>

using namespace def::model;
using namespace def::dbio::gendb;

namespace {

template <class T>
struct Zeroed {
    alignas(16) std::byte buf[sizeof(T)];
    Zeroed() { std::memset(buf, 0, sizeof buf); }
    T& get() { return *reinterpret_cast<T*>(buf); }
};

struct Window {
    alignas(16) std::byte buf[0x920];
    Window() { std::memset(buf, 0, sizeof buf); }
};

// Runs f with stdout going to a temp file and returns what it printed.
template <class F>
std::string capture(F f)
{
    const auto path = std::filesystem::temp_directory_path() /
                      ("p4_04_out_" + std::to_string(_getpid()) + ".txt");
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    FILE* tmp = std::fopen(path.string().c_str(), "w");
    _dup2(_fileno(tmp), _fileno(stdout));
    f();
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(tmp);
    std::ifstream in(path, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    in.close();
    std::filesystem::remove(path);
    std::string s = ss.str();
    std::string r; // text mode wrote \r\n
    for (char c : s)
        if (c != '\r') r += c;
    return r;
}

struct Model {
    std::unique_ptr<Zeroed<DB_STEP_DATA>> z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    std::vector<std::unique_ptr<Zeroed<LLOBJ_DATA>>> objs;
    std::vector<std::unique_ptr<Zeroed<Cls_1401f46e0>>> recs;
    std::vector<std::unique_ptr<Zeroed<Material>>> mats;
    std::vector<std::unique_ptr<Window>> wins;

    explicit Model(std::int32_t dim)
    {
        DB_STEP_DATA& st = s();
        st.dimension = dim;
        st.sim_ctrl.owner = &st;
        st.inter_obj.owner = &st;
        st.objects.owner = &st;
        // a valid default simulation control: no message from check 1
        Cls_1401e7b10& c = st.sim_ctrl;
        c.stype = 1;
        c.trans[1] = 1;
        c.pdie1 = 1;
        c.nstart = 1;
        c.nstep = 10;
        c.stpinc = 1;
        c.dsmax = 0.1;
        c.dttype.u58 = 1;
        c.dtsub = 1.0;
        c.dplen = 0.5;
        c.itrmth = 1;
        c.solmtd = 4;
        c.solmtt = 1;
        c.stnout[0] = 1;
        c.stnout[4] = 1;
    }
    DB_STEP_DATA& s() { return z->get(); }
    Cls_1401e7b10& sim() { return s().sim_ctrl; }

    LLOBJ_DATA& add_object(std::int32_t number, std::int32_t type)
    {
        objs.push_back(std::make_unique<Zeroed<LLOBJ_DATA>>());
        LLOBJ_DATA& o = objs.back()->get();
        o.owner = &s();
        field<std::int32_t>(&o, 8) = number;
        field<std::int32_t>(&o, 0xc) = type;
        if (s().objects.head == nullptr) s().objects.head = &o;
        else s().objects.tail->next = &o;
        s().objects.tail = &o;
        s().objects.count = s().objects.count + 1;
        s().u1378 = s().u1378 + 1;
        s().u074[2] = s().u074[2] + 1; // +0x7c, the GENDB object count
        return o;
    }
    Cls_1401f46e0& add_relation(std::int32_t a, std::int32_t b)
    {
        recs.push_back(std::make_unique<Zeroed<Cls_1401f46e0>>());
        Cls_1401f46e0& r = recs.back()->get();
        r.owner = &s();
        r.u168 = a;
        r.u16c = b;
        r.u170 = 1;
        r.u000.u000 = 1;
        r.u000.u010 = -1; // no friction curve check unless a case sets a type
        r.u000.u014 = -1;
        Cls_1401fa020& l = s().inter_obj;
        if (l.head == nullptr) l.head = &r;
        else l.tail->next = &r;
        l.tail = &r;
        l.count = l.count + 1;
        return r;
    }
    void* add_window(Cls_1401f46e0& r)
    {
        wins.push_back(std::make_unique<Window>());
        void* w = wins.back()->buf;
        field<DB_STEP_DATA*>(w, 0x888) = &s();
        void* last = nullptr;
        for (void* p = r.u188.head; p != nullptr; p = field<void*>(p, 0x840)) last = p;
        if (last == nullptr) r.u188.head = static_cast<LLWINDEF_DATA*>(w);
        else field<void*>(last, 0x840) = w;
        r.u188.count = r.u188.count + 1;
        return w;
    }
    Material& add_material(std::int32_t number)
    {
        mats.push_back(std::make_unique<Zeroed<Material>>());
        Material& m = mats.back()->get();
        m.number = number;
        MaterialList& l = s().materials;
        if (l.head == nullptr) l.head = &m;
        else l.tail->next = &m;
        l.tail = &m;
        l.count = l.count + 1;
        return m;
    }
};

} // namespace

// ---- small functions --------------------------------------------------------------------------

TEST_CASE(method_codes_per_dimension)
{
    std::int32_t d = 2;
    method_codes_select(&d); // 1401c7d70
    const std::int32_t e2[14] = {0, 2, 1, 0, 2, 3, 4, 5, 6, 0, 3, 2, 4, 5};
    CHECK(std::memcmp(&g_method_codes, e2, sizeof e2) == 0);
    d = 3;
    method_codes_select(&d); // 1401c7e10
    const std::int32_t e3[14] = {0, 2, 1, 0, 2, 1, 4, 5, 6, 0, 1, 2, 4, 5};
    CHECK(std::memcmp(&g_method_codes, e3, sizeof e3) == 0);
    d = 7;
    method_codes_select(&d);
    CHECK(std::memcmp(&g_method_codes, e3, sizeof e3) == 0);
    static_assert(sizeof(MethodCodes) == 14 * 4);
}

TEST_CASE(sim_type_names)
{
    Model m(3);
    const char* want[8] = {"Undefined", "Lagrangian incremental", "Discrete", "Steady-state machining",
                           "Steady-state extrusion", "Steady-state rolling", "Ring rolling", "ALE extrusion"};
    for (int i = 0; i < 8; ++i) {
        m.sim().stype = i;
        CHECK(std::strcmp(sim_type_name(m.sim()), want[i]) == 0);
    }
}

TEST_CASE(step_size_defined)
{
    Model m(3);
    Cls_1401e7b10& c = m.sim();
    c.dsmax = 0.0;
    c.dtmax1 = 0.0;
    CHECK(!sim_ctrl_step_size_defined(c));
    c.dtmax1 = 1e-9;
    CHECK(sim_ctrl_step_size_defined(c));
    c.dtmax1 = -1.0;
    c.dsmax = -2.0;
    CHECK(!sim_ctrl_step_size_defined(c));
    // DTTYPE as a table: defined unless every value is zero (14012fe90)
    double vals[2] = {0.0, 0.0};
    c.dttype.ndim = 1;
    c.dttype.n[0] = 2;
    c.dttype.ncomp = 1;
    c.dttype.values = vals;
    CHECK(!sim_ctrl_step_size_defined(c));
    vals[1] = 3.0;
    CHECK(sim_ctrl_step_size_defined(c));
    c.dttype.values = nullptr;
    c.dttype.ndim = 0;
}

TEST_CASE(dynary_zero_tests)
{
    DYNARY* t = &(new Zeroed<DYNARY>)->get();
    CHECK(dynary_values_zero(t));
    CHECK(dynary_all_zero(t));
    t->value = 2.0;
    CHECK(!dynary_values_zero(t));
    CHECK(!dynary_all_zero(t));
    double ax[3] = {0.0, 0.0, 1.0};
    double v[3] = {0.0, 0.0, 0.0};
    t->ndim = 1;
    t->n[0] = 3;
    t->ncomp = 1;
    t->axis[0] = ax;
    t->values = v;
    CHECK(dynary_values_zero(t)); // only the values
    CHECK(!dynary_all_zero(t));   // abscissae too
    ax[2] = 0.0;
    CHECK(dynary_all_zero(t));
    v[1] = -1.0;
    CHECK(!dynary_values_zero(t));
    CHECK(!dynary_all_zero(t));
}

TEST_CASE(window_validity)
{
    Model m2(2);
    Window w;
    field<DB_STEP_DATA*>(w.buf, 0x888) = &m2.s();
    double xy[24] = {};
    field<double*>(w.buf, 0x180) = xy;
    CHECK(!window_valid(w.buf)); // 2D, no points
    field<std::int32_t>(w.buf, 0x178) = 2;
    CHECK(!window_valid(w.buf)); // 2D, all four coordinates zero
    xy[3] = 1.0;
    CHECK(window_valid(w.buf));
    xy[3] = 0.0;
    xy[4] = 1.0; // beyond 2 * 2 values: not looked at
    CHECK(!window_valid(w.buf));

    Model m3(3);
    field<DB_STEP_DATA*>(w.buf, 0x888) = &m3.s();
    field<std::int32_t>(w.buf, 0x210) = 0;
    CHECK(!window_valid(w.buf)); // kind 0
    field<std::int32_t>(w.buf, 0x210) = 2;
    CHECK(window_valid(w.buf)); // kind 2: no point test
    field<std::int32_t>(w.buf, 0x210) = 1;
    CHECK(!window_valid(w.buf)); // box needs 8 points
    field<std::int32_t>(w.buf, 0x178) = 8;
    xy[4] = 0.0;
    CHECK(!window_valid(w.buf)); // 8 points, all zero
    xy[23] = 5.0;
    CHECK(window_valid(w.buf));
}

TEST_CASE(object_material_ids_distinct_and_capped)
{
    Model m(3);
    LLOBJ_DATA& o = m.add_object(1, 4);
    std::int32_t n = -1;
    std::int32_t out[4] = {-1, -1, -1, -1};
    std::int32_t cap = 4;
    CHECK(!object_material_ids(&o, &n, out, &cap, &m.s())); // no mesh
    CHECK_EQ(n, 0);
    field<std::int32_t>(&o, 0x87c) = 1;
    CHECK(!object_material_ids(&o, &n, out, &cap, &m.s())); // no array
    std::int32_t ids[7] = {3, 3, 9, 1, 9, 3, 2};
    field<std::int32_t*>(&o, 0x9f8) = ids;
    field<std::int32_t>(&o, 0x884) = 7;
    CHECK(object_material_ids(&o, &n, out, &cap, &m.s()));
    CHECK_EQ(n, 4);
    CHECK(out[0] == 3 && out[1] == 9 && out[2] == 1 && out[3] == 2);
    cap = 2;
    out[2] = -7;
    CHECK(object_material_ids(&o, &n, out, &cap, &m.s()));
    CHECK_EQ(n, 4); // the count is not capped, the copy is
    CHECK(out[0] == 3 && out[1] == 9 && out[2] == -7);
    CHECK_EQ(object_u3980(&o), 0);
}

TEST_CASE(object_queries)
{
    Model m(3);
    LLOBJ_DATA& a = m.add_object(1, 4);
    LLOBJ_DATA& b = m.add_object(2, 7); // skipped by the 140160f10 test
    LLOBJ_DATA& c = m.add_object(3, 3);
    field<std::int32_t>(&b, 0x21c8) = 5;
    CHECK_EQ(count_elasto_plastic(m.s()), 1);
    CHECK(!any_movement_type_5(m.s()));
    field<std::int32_t>(&c, 0x21c8) = 5;
    CHECK(any_movement_type_5(m.s()));
    std::int32_t t = 7;
    CHECK_EQ(count_objects_of_type(m.s(), &t), 0);
    t = 3;
    CHECK_EQ(count_objects_of_type(m.s(), &t), 1);
    CHECK(any_type_4(m.s()));
    field<std::int32_t>(&a, 0xc) = 7;
    CHECK(any_type_4(m.s()) == false);
    field<std::int32_t>(&b, 0xc) = 4; // 1401c5500 does not skip type 7, but b is 4 now
    CHECK(any_type_4(m.s()));
    field<std::int32_t>(&a, 0xc) = 4;
    field<std::int32_t>(&b, 0xc) = 7;

    // mesh kind (+0x890) and mesh type (+0x2154) need a mesh (+0x87c)
    field<std::int32_t>(&a, 0x890) = 4;
    field<std::int32_t>(&a, 0x2154) = 2;
    CHECK(!any_mesh_kind(m.s(), false));
    std::int32_t k = 2;
    CHECK(!any_elasto_plastic_mesh_type(m.s(), &k));
    field<std::int32_t>(&a, 0x87c) = 1;
    CHECK(any_mesh_kind(m.s(), false));
    CHECK(!any_mesh_kind(m.s(), true));
    CHECK(any_elasto_plastic_mesh_type(m.s(), &k));
    k = 1;
    CHECK(!any_elasto_plastic_mesh_type(m.s(), &k));

    // BCC codes 7/8, element-major with s.u15f8[18] columns; 3D only
    std::int32_t bcc[6] = {1, 2, 3, 0, 0, 8};
    field<std::int32_t*>(&a, 0x990) = bcc;
    field<std::int32_t>(&a, 0x880) = 2;
    m.s().u15f8[18] = 2;
    CHECK(!any_rotational_symmetry_bcc(m.s())); // only bcc[0..3] are read
    m.s().u15f8[18] = 3;
    CHECK(any_rotational_symmetry_bcc(m.s()));
    m.s().dimension = 2;
    CHECK(!any_rotational_symmetry_bcc(m.s()));
    m.s().dimension = 3;

    // movement (stub of 1401792d0, case 1: +0x224c set)
    CHECK(!any_force_movement(m.s()));
    CHECK(!movement_type_4_defined(&a));
    field<std::int32_t>(&a, 0x21c8) = 4;
    field<double>(&a, 0x2478) = 1.0;
    field<double>(&a, 0x2480) = 1.0;
    CHECK(movement_type_4_defined(&a));
    CHECK(any_mechanical_press(m.s()));
    field<std::int32_t>(&c, 0x21c8) = 0;
    CHECK(!any_force_movement(m.s())); // movement type 0 needs DTTYPE mode 1 in 1401792d0
    m.sim().dttype.u58 = 1;
    field<std::int32_t>(&c, 0x2780) = 1;
    CHECK(any_force_movement(m.s()));

    // torque: rotation type 0 (+0x2788) with a rotation speed (+0x27c8, stub of 140179840)
    CHECK(!any_torque_movement(m.s()));
    field<double>(&c, 0x27c8) = 2.0;
    CHECK(any_torque_movement(m.s()));
}

TEST_CASE(primary_die_queries)
{
    Model m(3);
    LLOBJ_DATA& a = m.add_object(1, 0);
    LLOBJ_DATA& b = m.add_object(2, 0);
    std::int32_t f = -1;
    m.sim().pdie1 = 3; // beyond the object count
    CHECK(!primary_die_moves(m.s()));
    CHECK(!primary_die_follows(m.s(), &f));
    CHECK_EQ(f, 0);
    m.sim().pdie1 = 1;
    CHECK(!primary_die_moves(m.s()));
    field<std::int32_t>(&a, 0x21c8) = 1;
    field<std::int32_t>(&a, 0x224c) = 1;
    CHECK(primary_die_moves(m.s()));
    field<std::int32_t>(&a, 0x21c8) = 0;
    field<std::int32_t>(&a, 0x224c) = 0;
    // follows object 2 (+0x22c0 is a list position), which moves, with ratio +0x22c8
    field<std::int32_t>(&a, 0x22c0) = 2;
    field<std::int32_t>(&b, 0x21c8) = 1;
    field<std::int32_t>(&b, 0x224c) = 1;
    CHECK(!primary_die_follows(m.s(), &f)); // ratio 0
    CHECK_EQ(f, 2);
    field<double>(&a, 0x22c8) = 0.5;
    CHECK(primary_die_follows(m.s(), &f));
    field<std::int32_t>(&b, 0xc) = 7;
    f = -1;
    CHECK(!primary_die_follows(m.s(), &f));
    CHECK_EQ(f, 0);
}

TEST_CASE(inter_obj_and_rigid)
{
    Model m(3);
    LLOBJ_DATA& a = m.add_object(1, 0);
    std::uint8_t r1 = 9, r2 = 9;
    field<std::int32_t>(&a, 0x21c8) = 1;
    field<std::int32_t>(&a, 0x224c) = 1;
    inter_obj_and_rigid_movement(m.s(), &r1, &r2);
    CHECK(r1 == 0 && r2 == 0); // empty inter-object list: objects not looked at
    Cls_1401f46e0& rec = m.add_relation(1, 1);
    rec.u170 = 0;
    inter_obj_and_rigid_movement(m.s(), &r1, &r2);
    CHECK(r1 == 0 && r2 == 1);
    rec.u170 = 3;
    field<std::int32_t>(&a, 0xc) = 1;
    inter_obj_and_rigid_movement(m.s(), &r1, &r2);
    CHECK(r1 == 1 && r2 == 0);
}

// ---- check 1 -----------------------------------------------------------------------------------

TEST_CASE(sim_ctrl_clean_model_prints_type_only)
{
    Model m(3);
    std::int32_t err = 0, warn = 0;
    const std::string out = capture([&] { sim_ctrl_check(m.sim(), &err, &warn); });
    CHECK_EQ(out, std::string("INFO   : Simulation Type : Lagrangian incremental\n"));
    CHECK(err == 0 && warn == 0);
}

TEST_CASE(sim_ctrl_deformation_messages)
{
    Model m(3);
    Cls_1401e7b10& c = m.sim();
    c.pdie1 = 0;
    c.nstart = 0;
    c.nstep = 0;
    c.stpinc = 0;
    c.dsmax = 1.0;
    c.dtmax1 = 2.0;
    c.dtsub = 0.5;
    c.dplen = 0.0;
    c.stnout[0] = 0;
    c.stnout[4] = 0;
    c.stype = 2;
    std::int32_t err = 0, warn = 0;
    const std::string out = capture([&] { sim_ctrl_check(c, &err, &warn); });
    CHECK_EQ(out, std::string("INFO   : Simulation Type : Discrete\n"
                              "WARNING: PDIE(0) not defined; Object 1 is assumed\n"
                              "ERROR  : NSTART must not be 0\n"
                              "ERROR  : NSTEP must be > 0\n"
                              "ERROR  : STPINC must be > 0\n"
                              "WARNING: DSMAX and DTMAX cannot be defined together. DSMAX will be used.\n"
                              "WARNING: DTSUB for sub-stepping =       0.5, not 1.0.\n"
                              "         This can increase the time for running a\n"
                              "         simulation\n"
                              "WARNING: DPLEN  = 0.00, auto-time stepping not\n"
                              "         activated\n"
                              "WARNING: STNOUT not specified.  Total strain will be stored.\n"));
    CHECK(err == 1 && warn == 1);
    CHECK_EQ(c.pdie1, 1);
    CHECK(c.dtmax1 == 0.0);
    CHECK_EQ(c.stnout[4], 1);

    // the same in 2D: no DTSUB/DPLEN messages; DPLEN >= 1 only in 3D
    Model m2(2);
    m2.sim().dtsub = 0.5;
    m2.sim().dplen = 1.0;
    err = warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check(m2.sim(), &err, &warn); }),
             std::string("INFO   : Simulation Type : Lagrangian incremental\n"));
    m.sim().nstart = 1;
    m.sim().nstep = 10;
    m.sim().stpinc = 1;
    m.sim().dtsub = 1.0;
    m.sim().dplen = 1.0;
    err = warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check(m.sim(), &err, &warn); }),
             std::string("INFO   : Simulation Type : Discrete\n"
                         "WARNING: DPLEN >= 1.00, can cause severe distortion\n"
                         "         of the mesh during the simulation\n"));
    CHECK(err == 0 && warn == 0); // the DPLEN warnings set no flag
}

TEST_CASE(sim_ctrl_heat_transfer_messages)
{
    Model m(2);
    Cls_1401e7b10& c = m.sim();
    c.trans[1] = 0;       // no deformation
    c.dttype.u58 = 0;     // stroke per step
    c.trans[4] = 1;       // grain model without heat transfer
    c.htmthd = 2;         // ENVMPR / ENVMPT needed
    c.stnout[4] = 0;      // restored to 1 without a message
    c.stype = 1;
    std::int32_t err = 0, warn = 0;
    const std::string out = capture([&] { sim_ctrl_check(c, &err, &warn); });
    CHECK_EQ(out, std::string("INFO   : Simulation Type : Lagrangian incremental\n"
                              "ERROR  : If stroke per step is used, the primary die (1) must have assigned "
                              "translational movement.\n"
                              "ERROR  : DTMAX not defined for Heat Transfer\n"
                              "ERROR  : Grain Model requires Heat Transfer to be on\n"
                              "ERROR  : Undefined Keyword ENVMPR\n"
                              "ERROR  : Undefined Keyword ENVMPT\n"));
    CHECK(err == 1 && warn == 0);
    CHECK_EQ(c.stnout[4], 1);

    // DTTYPE mode 1 by stroke (var 2) is refused for heat transfer
    c.dttype.u58 = 1;
    c.dttype.var[0] = 2;
    c.trans[4] = 0;
    c.htmthd = 0;
    err = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check(c, &err, &warn); }),
             std::string("INFO   : Simulation Type : Lagrangian incremental\n"
                         "ERROR  : DTMAX cannot be defined as function of stroke for Heat Transfer\n"));
    CHECK_EQ(err, 1);
}

TEST_CASE(sim_ctrl_follow_object_message)
{
    Model m(3);
    LLOBJ_DATA& a = m.add_object(1, 0);
    m.add_object(2, 0);
    m.sim().dttype.u58 = 0;
    field<std::int32_t>(&a, 0x22c0) = 2; // follows object 2, which does not move
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check(m.sim(), &err, &warn); }),
             std::string("INFO   : Simulation Type : Lagrangian incremental\n"
                         "ERROR  : Primary die's follow object movement control not defined!\n"));
    CHECK_EQ(err, 1);
}

TEST_CASE(sim_ctrl_functions_and_time_steps)
{
    Model m(3);
    Cls_1401e7b10& c = m.sim();
    c.trans[0] = 1;
    c.trans[3] = 1;
    c.envtmp.ndim = 1; // a table with no points: 14012ff20 gives 0
    c.envtmp.ncomp = 1;
    c.cnvcof.ndim = 1;
    c.cnvcof.ncomp = 1;
    c.envatm.ndim = 2;
    c.envatm.ncomp = 1;
    c.envatm.n[0] = 2; // 2 * 0 = 0
    c.acvcof.ndim = 1;
    c.acvcof.ncomp = 1;
    c.acvcof.n[0] = 1; // valid
    c.dtpmax1 = 1.0;
    c.dtpmax2_5[0] = 1.0; // the maximum (DTPMAX 3) missing
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check(c, &err, &warn); }),
             std::string("INFO   : Simulation Type : Lagrangian incremental\n"
                         "ERROR  : Invalid Function Definition for Environment Temp.\n"
                         "ERROR  : Invalid Function Definition for Convection Coef.\n"
                         "ERROR  : Invalid Function Definition for Environment Carbon Potential\n"
                         "ERROR  : DTPMAX must have min/max time step defined\n"));
    CHECK(err == 1 && warn == 0);

    // a mechanical press with stroke steps and no maximum time step
    c.envtmp.ndim = c.cnvcof.ndim = c.envatm.ndim = 0;
    c.dtpmax1 = 0.0;
    c.dttype.u58 = 0;
    LLOBJ_DATA& a = m.add_object(1, 0);
    field<std::int32_t>(&a, 0x21c8) = 4;
    field<double>(&a, 0x2478) = 1.0;
    field<double>(&a, 0x2480) = 1.0;
    err = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check(c, &err, &warn); }),
             std::string("INFO   : Simulation Type : Lagrangian incremental\n"
                         "WARNING: Maximum time step should be specified\n"
                         "         to avoid a possible huge time step\n"
                         "         after the mechanical press reverse its direction.\n"));
    CHECK(err == 0 && warn == 0); // that warning sets no flag
}

TEST_CASE(solver_checks)
{
    Model m(3);
    Cls_1401e7b10& c = m.sim();
    LLOBJ_DATA& ep = m.add_object(1, 4); // elasto-plastic, sliding (movement 5), meshed tets
    LLOBJ_DATA& el = m.add_object(2, 3); // elastic
    LLOBJ_DATA& rg = m.add_object(3, 0); // rigid, moving (movement 1, +0x224c)
    field<std::int32_t>(&ep, 0x21c8) = 5;
    field<std::int32_t>(&ep, 0x87c) = 1;
    field<std::int32_t>(&ep, 0x2154) = 1;
    field<std::int32_t>(&ep, 0x3980) = 5;
    std::int32_t ids[3] = {5, 5, 6};
    field<std::int32_t*>(&ep, 0x9f8) = ids;
    field<std::int32_t>(&ep, 0x884) = 3;
    field<std::int32_t>(&el, 0x21c8) = 1;
    field<std::int32_t>(&el, 0x224c) = 1;
    field<std::int32_t>(&rg, 0x21c8) = 1;
    field<std::int32_t>(&rg, 0x224c) = 1;
    m.add_relation(1, 3);
    m.add_material(5).masden.value = 7.8e-9;
    m.add_material(6); // no mass density
    c.itrmth = 0; // unset
    c.solmtd = 2; // CG
    c.solmtt = 6; // explicit
    c.cvgerr[0] = 0.01;
    c.cvgerr[1] = 0.01;
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check_solver(c, &err, &warn); }),
             std::string("ERROR  : Iteration method for deformation analysis has not been set.\n"
                         "ERROR  : Conjugate-gradient solver should not be used with sliding die movement control.\n"
                         "WARNING: CVGERR for velocity is too large for reliable simulation result for elastic "
                         "objects.  A value of 0.001 or less is recommended.\n"
                         "ERROR  : Explicit solver is recommended for objects with Mass Density defined in the "
                         "material.\n"
                         "WARNING: TET(STD) is recommended.\n"));
    CHECK(err == 1 && warn == 1);

    // Newton-Raphson with CG and plastic/elastic/porous objects; brick advice (printed twice)
    c.itrmth = 1;
    c.solmtt = 1;
    c.cvgerr[0] = 0.001;
    c.trans[0] = 1;
    field<std::int32_t>(&ep, 0x21c8) = 1;
    field<std::int32_t>(&ep, 0x224c) = 1;
    field<std::int32_t>(&ep, 0x2154) = 2;
    err = warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check_solver(c, &err, &warn); }),
             std::string("WARNING: Direct method is recommended with CG solver and plastic/elastic/porous object(s).\n"
                         "WARNING: Brick(STD) is recommended.\n"
                         "WARNING: Brick(STD) is recommended.\n"));
    CHECK(err == 0 && warn == 1);

    // direct iteration with an elasto-plastic object; explicit without one; CG + force/torque
    c.itrmth = 2;
    c.solmtd = 6;
    c.trans[0] = 0;
    err = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check_solver(c, &err, &warn); }),
             std::string("ERROR  : Direct iteration is not recommended for elasto-plastic simulations. Please select "
                         "Newton-Raphson.\n"
                         "ERROR  : Explicit solver is recommended for objects with Mass Density defined in the "
                         "material.\n"));
    field<std::int32_t>(&ep, 0xc) = 1;
    c.itrmth = 1;
    CHECK_EQ(capture([&] { sim_ctrl_check_solver(c, &err, &warn); }),
             std::string("ERROR  : Explicit solver is recommended for Elasto-plastic objects only.\n"));
    c.solmtd = 2;
    c.dttype.u58 = 1;
    field<std::int32_t>(&rg, 0x21c8) = 0; // force control
    field<std::int32_t>(&rg, 0x2780) = 1;
    field<double>(&el, 0x27c8) = 1.0;     // torque control
    field<std::int32_t>(&ep, 0x890) = 4;  // brick mesh with a BCC code 7
    std::int32_t bcc[1] = {7};
    field<std::int32_t*>(&ep, 0x990) = bcc;
    field<std::int32_t>(&ep, 0x880) = 1;
    m.s().u15f8[18] = 1;
    c.cvgerr[1] = 0.1;
    err = warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check_solver(c, &err, &warn); }),
             std::string("ERROR  : Conjugate-gradient solver is not recommended for force movement control.\n"
                         "ERROR  : Conjugate-gradient solver is not recommended for torque movement control.\n"
                         "ERROR_DEF: Conjugate-gradient solver is not recommended for brick mesh with rotational "
                         "symmetry BCC.\n"
                         "WARNING: CVGERR for force is too large for reliable simulation result for elastic objects.  "
                         "A value of 0.01 or less is recommended.\n"
                         "WARNING: Direct method is recommended with CG solver and plastic/elastic/porous object(s).\n"));
    CHECK(err == 1 && warn == 1);
    c.stype = 6; // ring rolling: no force/torque advice
    err = warn = 0;
    const std::string ring = capture([&] { sim_ctrl_check_solver(c, &err, &warn); });
    CHECK(ring.find("force movement") == std::string::npos);
    CHECK(ring.find("torque movement") == std::string::npos);
    CHECK(ring.find("ERROR_DEF") != std::string::npos);
}

TEST_CASE(solver_unset_methods)
{
    Model m(2);
    Cls_1401e7b10& c = m.sim();
    c.itrmth = 0;
    c.solmtd = 0;
    c.solmtt = 0;
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { sim_ctrl_check_solver(c, &err, &warn); }),
             std::string("ERROR  : Iteration method for deformation analysis has not been set.\n"
                         "ERROR  : Solution method for deformation analysis has not been set.\n"
                         "ERROR  : Solution method for temperature analysis has not been set.\n"));
    CHECK(err == 1 && warn == 0);
    CHECK_EQ(g_method_codes.c14, 3); // 2D codes selected from the owner's dimension
}

// ---- check 5 -----------------------------------------------------------------------------------

TEST_CASE(inter_object_no_relation)
{
    Model m(3);
    m.add_object(1, 0);
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }), std::string());
    CHECK(warn == 0); // one object
    m.add_object(2, 1);
    Cls_1401f46e0& r = m.add_relation(1, 2);
    r.u170 = 0; // not defined
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }),
             std::string("WARNING: No Inter-Object Relation Defined\n"));
    CHECK(err == 0 && warn == 1);
}

TEST_CASE(inter_object_relation_messages)
{
    Model m(3);
    m.add_object(1, 0);
    m.add_object(2, 1);
    m.add_object(3, 7); // type 7: its relations are not checked
    Cls_1401f46e0& r = m.add_relation(1, 2);
    Cls_1401f46e0& skip = m.add_relation(2, 3);
    skip.u000.u010 = 0;
    r.u000.u010 = 0; // FRCFAC as a curve (zero)
    m.sim().trans[0] = 1;
    m.sim().htmthd = 1;
    void* w = m.add_window(r); // 3D window of kind 0: not valid
    (void)w;
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }),
             std::string("WARNING: FRCFAC between Objects (     1      2) is ZERO\n"
                         "WARNING: IHTCOF between Objects (     1      2) is ZERO\n"
                         "WARNING: INTRST between Objects (     1      2) is ZERO\n"
                         "Error: FRCWIN between Objects (     1      2) is not valid\n"));
    CHECK(err == 1 && warn == 1);

    // relation (2, 1) is not looked up: only a <= b pairs
    r.u168 = 2;
    r.u16c = 1;
    err = warn = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }),
             std::string("WARNING: No Inter-Object Relation Defined\n"));
}

TEST_CASE(inter_object_friction_kinds)
{
    Model m(3);
    m.add_object(1, 0);
    m.add_object(2, 1);
    Cls_1401f46e0& r = m.add_relation(1, 2);
    Cls_1401fb020& f = r.u000;
    double* k = reinterpret_cast<double*>(f.u108); // +0x108 ..
    f.u010 = 6;  // needs +0x108 and +0x110
    k[0] = 1.0;
    f.u000 = 4;  // second definition
    f.u005 = 1;  // by user routine, number +0xc not set
    std::int32_t err = 0, warn = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }),
             std::string("WARNING: FRCFAC between Objects (     1      2) is ZERO\n"
                         "WARNING: FRCFAC User Routine no. between Objects (     1      2) is not assigned\n"));
    CHECK(err == 0 && warn == 1);
    k[1] = 2.0;
    f.u00c = 3;
    warn = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }), std::string());
    CHECK_EQ(warn, 0);
    f.u010 = 7; // +0x118
    f.u005 = 0;
    f.u014 = 8; // +0x158
    k[10] = 1.0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }),
             std::string("WARNING: FRCFAC between Objects (     1      2) is ZERO\n"));
    f.u010 = 9; // types above 8: nothing
    f.u004 = 1;
    f.u008 = 2;
    warn = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }), std::string());
    m.sim().trans[1] = 0; // friction only for deformation
    f.u008 = 0;
    CHECK_EQ(capture([&] { inter_obj_check(m.s().inter_obj, &err, &warn); }), std::string());
    CHECK_EQ(warn, 0);
}

// ---- the chain ---------------------------------------------------------------------------------

TEST_CASE(check_chain_runs_checks_in_order)
{
    // checks 2..4 are p4-03's (covered by defcmd.objcheck_oracle); here only their INFO lines
    ext::g_checks_234_hook = [](std::int32_t check, DB_STEP_DATA&, std::int32_t*, std::int32_t*) {
        static const char* const line[] = {"", "", "INFO   : Checking material properties\n",
                                           "INFO   : Checking inter-material data\n", "INFO   : Checking object data\n"};
        def::rt::crt::fprintf(stdout, "%s", line[check]);
    };
    Model m(3);
    m.add_object(1, 0);
    m.add_object(2, 1);
    m.add_relation(1, 2);
    std::int32_t err = 5, warn = 5;
    CHECK_EQ(capture([&] { check_chain(m.s(), &err, &warn); }),
             std::string("INFO   : Checking simulation controls\n"
                         "INFO   : Simulation Type : Lagrangian incremental\n"
                         "INFO   : Checking material properties\n"
                         "INFO   : Checking inter-material data\n"
                         "INFO   : Checking object data\n"
                         "INFO   : Checking inter-object data\n"));
    CHECK(err == 0 && warn == 0);
    m.sim().nstart = 0;
    m.s().inter_obj.head->u170 = 0;
    CHECK_EQ(capture([&] { check_chain(m.s(), &err, &warn); }),
             std::string("INFO   : Checking simulation controls\n"
                         "INFO   : Simulation Type : Lagrangian incremental\n"
                         "ERROR  : NSTART must not be 0\n"
                         "INFO   : Checking material properties\n"
                         "INFO   : Checking inter-material data\n"
                         "INFO   : Checking object data\n"
                         "INFO   : Checking inter-object data\n"
                         "WARNING: No Inter-Object Relation Defined\n"));
    CHECK(err == 1 && warn == 1);
    ext::g_checks_234_hook = nullptr;
}

int main() { return testkit::run_all(); }
