// Tier 1 for packet p1-03 (defmodel part): the parts of the model root that run without other
// packets' code. Expected values are read off the decompiles (cited per check), not the port.
// DB_STEP_DATA / Cls_1401e7b10 functions are exercised on zeroed storage, the way the keyword
// packets use the model until the member classes of other packets land.
// ctest-labels: p1-03 step
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>

using namespace def::model;

namespace {

template <class T>
struct Zeroed {
    alignas(16) std::byte buf[sizeof(T)];
    Zeroed() { std::memset(buf, 0, sizeof buf); }
    T& get() { return *reinterpret_cast<T*>(buf); }
};

} // namespace

// ---- DB_WRITE_VAR (1401bae10 -> 1401baf30) ----------------------------------------------------

TEST_CASE(db_write_var_ctor_values)
{
    auto* mem = static_cast<DB_WRITE_VAR*>(std::malloc(sizeof(DB_WRITE_VAR)));
    std::memset(static_cast<void*>(mem), 0xcd, sizeof(DB_WRITE_VAR));
    DB_WRITE_VAR* w = new (mem) DB_WRITE_VAR();
    CHECK_EQ(w->u008, 1);           // 1401c5720(1)
    CHECK_EQ(w->u009, 1);
    CHECK_EQ(w->version, 0x44c);    // 1401baf30
    CHECK(std::strcmp(w->date, "06/30/14") == 0);
    CHECK_EQ(w->dimension, 3);      // 1401bae10
    CHECK_EQ(w->u0e0, 0x10);
    CHECK(w->u050 == nullptr && w->u058 == nullptr && w->u060 == nullptr);
    for (int i = 0; i < 21; ++i) {
        CHECK_EQ(w->u074[i], 0);
    }
    CHECK_EQ(w->u0c8, 0);
    CHECK_EQ(w->u0cc, 0);
    CHECK_EQ(w->u0d0, 0);
    CHECK_EQ(w->u130, 0);
    CHECK_EQ(w->u13c, 0);
    CHECK(w->u0e8 == 0 && w->u0f0 == 0 && w->u0f8 == 0);
    // Fields neither ctor nor init writes keep their old bytes (0x4c..0x4f is padding, u0c4 is
    // the last of the 21 zeroed ints).
    std::uint32_t pad;
    std::memcpy(&pad, reinterpret_cast<std::byte*>(w) + 0x4c, 4);
    CHECK_EQ(pad, 0xcdcdcdcdu);

    std::int32_t two = 2;
    w->set_dimension(&two); // 1401c5700
    CHECK_EQ(w->dimension, 2);

    w->u050 = std::malloc(8);
    w->u060 = std::malloc(8);
    db_write_var_free_buffers(*w); // 1401bb320
    CHECK(w->u050 == nullptr && w->u060 == nullptr);
    w->~DB_WRITE_VAR();
    std::free(mem);
}

// ---- ACTION_DB_STEP --------------------------------------------------------------------------

TEST_CASE(action_db_step_ctor_clears_text)
{
    Zeroed<ACTION_DB_STEP> z;
    std::memset(z.buf, 'x', sizeof z.buf);
    ACTION_DB_STEP* a = new (z.buf) ACTION_DB_STEP(); // 1401d8380: byte 0 = 0
    CHECK_EQ(a->text[0], '\0');
    CHECK_EQ(a->text[1], 'x');
    a->~ACTION_DB_STEP();
}

// ---- Cls_1401dbfe0 (1401dbfe0, 1401dc030, 1401dc220, 1401dc1c0) ------------------------------

TEST_CASE(db_info_ctor_values)
{
    auto* mem = static_cast<Cls_1401dbfe0*>(std::malloc(sizeof(Cls_1401dbfe0)));
    std::memset(static_cast<void*>(mem), 0x5a, sizeof(Cls_1401dbfe0));
    Cls_1401dbfe0* d = new (mem) Cls_1401dbfe0();
    CHECK_EQ(d->u000, 0);
    CHECK_EQ(d->u004, 0);
    CHECK_EQ(d->u008, 1);
    CHECK_EQ(d->u00c[0].a, 0);
    CHECK_EQ(d->u00c[99].b, 0);
    CHECK(d->u330[0] == nullptr && d->u330[99] == nullptr);
    CHECK_EQ(d->u650[0], '\0');
    CHECK_EQ(d->u650[1], 0x5a); // only byte 0 written
    CHECK_EQ(d->ua50[0], '\0');
    CHECK_EQ(d->uaa1[0], '\0');
    CHECK_EQ(d->uaa9[0], '\0');
    CHECK_EQ(d->uab1[0], '\0');
    CHECK_EQ(d->uabc, 0);
    CHECK_EQ(d->uac8, 0);
    CHECK_EQ(d->uacc, 0);
    CHECK_EQ(d->uad0, 0);
    CHECK_EQ(d->uae0, -1);
    CHECK(d->uad8 == nullptr && d->uae8 == nullptr);
    CHECK_EQ(d->uaf0[0], 0u);
    CHECK_EQ(d->uaf0[399], 0u);
    CHECK_EQ(d->u1900, 0);
    CHECK(d->u1908 == nullptr);
    CHECK_EQ(d->u1910, 0);
    d->~Cls_1401dbfe0();
    std::free(mem);
}

TEST_CASE(db_info_reset_frees_only_with_entries)
{
    Cls_1401dbfe0 d;
    // uac8 == 0: the arrays stay, the 100 buffers are freed anyway (1401dc220).
    d.u330[3] = std::malloc(16);
    d.u1900 = 7;
    db_info_reset(d);
    CHECK(d.u330[3] == nullptr);
    CHECK_EQ(d.u1900, 7);

    d.uac8 = 2;
    d.uacc = 1;
    d.uad8 = new DbDirRecord[2];
    auto* e = new DbInfoEntryE0[3];
    e[1].u20 = std::malloc(32); // freed by the element dtor 1401d91d0
    d.uae8 = e;
    d.u1900 = 1;
    db_info_reset(d);
    CHECK_EQ(d.uac8, 0);
    CHECK_EQ(d.uacc, 0);
    CHECK(d.uad8 == nullptr && d.uae8 == nullptr);
    CHECK_EQ(d.u1900, 0);
    d.u1908 = std::malloc(4); // freed by the dtor
}

TEST_CASE(dir_record_ctor_zeroes)
{
    alignas(8) std::byte buf[sizeof(DbDirRecord)];
    std::memset(buf, 0xff, sizeof buf);
    auto* r = new (buf) DbDirRecord(); // 1401d9080: 10 ints = 0
    const auto* w = reinterpret_cast<const std::int32_t*>(buf);
    for (int i = 0; i < 10; ++i) {
        CHECK_EQ(w[i], 0);
    }
    r->~DbDirRecord();
}

// ---- index tables (1401c6e60 .. 1401c7d40) ---------------------------------------------------

TEST_CASE(index_tables_per_dimension)
{
    reset_globals_p1_03();
    std::int32_t two = 2;
    select_index_tables(&two);
    CHECK_EQ(g_index_tables_dim, 2);
    CHECK_EQ(g_mar_index[4], 6);      // 1401c6e60: DAT_1406e820c = 6
    CHECK_EQ(g_mar_index[12], 4);     // DAT_1406e822c = 4
    CHECK_EQ(g_mar_index[52], 0x83);  // DAT_1406e82cc
    CHECK_EQ(g_a_index[101], 0xb9);   // 1401c72a0: DAT_1406e8464 = 0xb9
    CHECK_EQ(g_a_index[128], 0x118);  // DAT_1406e84d0

    // Already 2D: nothing is refilled.
    g_mar_index[4] = -7;
    select_index_tables(&two);
    CHECK_EQ(g_mar_index[4], -7);

    std::int32_t other = 5; // anything but 2 selects 3D (1401c7d40)
    select_index_tables(&other);
    CHECK_EQ(g_index_tables_dim, 3);
    CHECK_EQ(g_mar_index[4], 4);      // 1401c7080: DAT_1406e820c = 4
    CHECK_EQ(g_mar_index[52], 0x7f);  // DAT_1406e82cc = 0x7f
    CHECK_EQ(g_a_index[6], 6);        // 1401c77c0 stores 0x...ec before 0x...e8; both land
    CHECK_EQ(g_a_index[7], 7);
    CHECK_EQ(g_a_index[101], 0xba);   // DAT_1406e8464 = 0xba
    CHECK_EQ(g_a_index[128], 0x118);
    reset_globals_p1_03();
}

// ---- array size table (1401d09c0, 1401d64f0 without materials) -------------------------------

TEST_CASE(array_sizes_dim2_defaults)
{
    reset_globals_p1_03();
    auto z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    DB_STEP_DATA& s = z->get();
    s.dimension = 2;
    step_setup_array_sizes(s, 2);
    const std::int32_t* t = s.u15f8;
    CHECK_EQ(t[0], 2);
    CHECK_EQ(t[6], 2);
    CHECK_EQ(t[7], 1);    // untouched entries stay 1
    CHECK_EQ(t[37], 4);   // 0x168c
    CHECK_EQ(t[62], 2);   // 0x16f0
    CHECK_EQ(t[46], 1);   // 0x16b0 only written in 3D
    CHECK_EQ(t[24], 5);   // has_array(0x18) is always true
    CHECK_EQ(t[38], 4);   // dim << 1
    CHECK_EQ(t[39], 4);
    // stnout all 0: sum 0 -> 1; extra 0 -> t[40] = 0 + 1 * 2 * 2
    CHECK_EQ(t[40], 4);
    CHECK_EQ(t[57], 4);
    CHECK_EQ(t[47], 2);
    // 0x31: trans[2] == 0 and no materials -> false -> 0
    CHECK_EQ(t[49], 0);
    CHECK_EQ(t[52], 0);
    CHECK_EQ(t[53], 0);   // 0x35: trans[4] == 0
    CHECK_EQ(t[29], 0);   // 0x1d: trans[4] == 0
    CHECK_EQ(t[54], 0);   // 0x36: no materials
    CHECK_EQ(t[55], 0);   // 0x37: no materials
    CHECK_EQ(t[56], 2);
    CHECK_EQ(t[58], 2);
    CHECK_EQ(t[59], 2);
    CHECK_EQ(t[17], 0);   // u3398
    CHECK_EQ(t[60], 0);   // u33a8
    // elmnod[0..2] == 0 -> 0x1668, 0x1664, 0x1660 = 0
    CHECK_EQ(t[28], 0);
    CHECK_EQ(t[27], 0);
    CHECK_EQ(t[26], 0);
    CHECK_EQ(g_index_tables_dim, 2); // has_array selected the 2D tables
    reset_globals_p1_03();
}

TEST_CASE(array_sizes_dim3_and_controls)
{
    reset_globals_p1_03();
    auto z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    DB_STEP_DATA& s = z->get();
    s.dimension = 3;
    s.sim_ctrl.stnout[0] = 2; // 0x86c..: sum 2+3 = 5
    s.sim_ctrl.stnout[3] = 3;
    s.sim_ctrl.stnout[5] = 7; // 0x880
    s.sim_ctrl.stnout[6] = 1; // 0x884: extra = 8
    s.sim_ctrl.trans[2] = 1;  // 0x85c != 0: has_array(0x31) true
    s.sim_ctrl.elmnod[0] = 3; // 0x888 == 3 -> t[28] = 0
    s.sim_ctrl.elmnod[1] = 1; // 0x88c == 1 -> t[27] kept (1)
    s.sim_ctrl.elmnod[2] = 2; // 0x890 == 2 -> t[26] kept (6)
    // n_materials stays 0: with materials, 0x36/0x37 walk the list (p1-08 code)
    s.n_phase_on = 9;
    s.u3398 = 11;
    s.u33a8 = 12;
    step_setup_array_sizes(s, 3);
    const std::int32_t* t = s.u15f8;
    CHECK_EQ(t[0], 3);
    CHECK_EQ(t[37], 0);
    CHECK_EQ(t[62], 3);
    CHECK_EQ(t[40], 8 + 5 * 3 * 2); // 1401d09c0: local_1c + local_18 * dim * 2
    CHECK_EQ(t[49], 0); // = n_materials
    CHECK_EQ(t[50], 0);
    CHECK_EQ(t[51], 9);
    CHECK_EQ(t[52], 9);
    CHECK_EQ(t[59], 3);
    // 0x2e in 3D with no materials -> false -> t[46] = 0
    CHECK_EQ(t[46], 0);
    CHECK_EQ(t[17], 11);
    CHECK_EQ(t[60], 12);
    CHECK_EQ(t[28], 0);
    CHECK_EQ(t[27], 1);
    CHECK_EQ(t[26], 6);
    CHECK_EQ(g_index_tables_dim, 3);
    reset_globals_p1_03();
}

TEST_CASE(array_sizes_dim2_axisymmetric_3)
{
    // geotyp == 3 in 2D overrides several sizes with 3 and 6 (1401d09c0 end).
    reset_globals_p1_03();
    auto z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    DB_STEP_DATA& s = z->get();
    s.dimension = 2;
    s.sim_ctrl.geotyp = 3;
    s.sim_ctrl.stnout[1] = 4; // sum 4
    s.sim_ctrl.stnout[5] = 2; // extra 2
    s.sim_ctrl.elmnod[2] = 1; // keep t[26]
    step_setup_array_sizes(s, 2);
    const std::int32_t* t = s.u15f8;
    CHECK_EQ(t[1], 2);
    CHECK_EQ(t[2], 3);
    CHECK_EQ(t[4], 3);
    CHECK_EQ(t[18], 3);
    CHECK_EQ(t[38], 6);
    CHECK_EQ(t[26], 6);
    CHECK_EQ(t[40], 4 * 6 + 2);
    CHECK_EQ(t[57], 6);
    reset_globals_p1_03();
}

TEST_CASE(has_array_default_uses_table)
{
    reset_globals_p1_03();
    auto z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    DB_STEP_DATA& s = z->get();
    s.dimension = 3;
    s.u15f8[10] = 1;
    s.u15f8[11] = 0;
    s.u15f8[12] = -1;
    CHECK(step_has_array(s, 10, nullptr));
    CHECK(!step_has_array(s, 11, nullptr));
    CHECK(!step_has_array(s, 12, nullptr));
    CHECK(step_has_array(s, 0x18, nullptr));
    s.dimension = 2;
    CHECK(step_has_array(s, 0x2e, nullptr)); // 2D: always true
    reset_globals_p1_03();
}

TEST_CASE(fun_1401d70c0_kinds)
{
    auto z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    DB_STEP_DATA& s = z->get();
    for (std::int32_t k : {-1, 1, 4, 5}) {
        CHECK_EQ(step_fun_1401d70c0(s, &k), 0x10);
    }
    for (std::int32_t k : {0, 3, 6, 100, -2}) {
        CHECK_EQ(step_fun_1401d70c0(s, &k), 0);
    }
    std::int32_t two = 2; // u1378 == 0 objects: 1401d71b0 returns 0, m stays 0
    CHECK_EQ(step_fun_1401d70c0(s, &two), 0);
}

TEST_CASE(step_free_strings_and_setters)
{
    auto z = std::make_unique<Zeroed<DB_STEP_DATA>>();
    DB_STEP_DATA& s = z->get();
    s.u3398 = 2;
    s.u33a0 = static_cast<char**>(std::calloc(2, sizeof(char*)));
    s.u33a0[1] = static_cast<char*>(std::malloc(5));
    step_free_strings_33a0(s); // 1401cb910
    CHECK(s.u33a0 == nullptr);
    CHECK_EQ(s.u3398, 2);      // the count is not reset
    s.u33a8 = 1;
    s.u33b0 = static_cast<char**>(std::calloc(1, sizeof(char*)));
    s.u33b0[0] = static_cast<char*>(std::malloc(5));
    step_free_strings_33b0(s); // 1401cb660
    CHECK(s.u33b0 == nullptr);

    s.u2f88 = 1;
    s.u2f89 = 1;
    step_clear_u2f89(s);
    CHECK_EQ(s.u2f89, 0);
    CHECK_EQ(s.u2f88, 1);
    step_clear_u2f88(s);
    CHECK_EQ(s.u2f88, 0);
    s.companion = &s;
    s.use_companion = 1;
    step_clear_partner(s);
    CHECK(s.companion == nullptr);
    CHECK_EQ(s.use_companion, 0);
    s.u33f8 = 5;
    s.u3400 = 6;
    step_clear_u33f8(s);
    CHECK(s.u33f8 == 0 && s.u3400 == 0);
    s.u1b48 = 1;
    s.u1b4c = 1;
    s.u1b50 = 1;
    step_clear_u1b48(s);
    CHECK(s.u1b48 == 0 && s.u1b4c == 0 && s.u1b50 == 0);
    s.u1b90[139] = 1;
    s.u1dc0[279] = 1;
    step_clear_address_tables(s);
    CHECK(s.u1b90[139] == 0 && s.u1dc0[279] == 0);
    s.u2260[0] = s.u2494[139] = s.u2490 = s.u26c4[279] = s.u2b28[279] = s.u2b24 = 1;
    step_clear_gendb_tables(s);
    CHECK(s.u2260[0] == 0 && s.u2494[139] == 0 && s.u2490 == 0);
    CHECK(s.u26c4[279] == 0 && s.u2b28[279] == 0 && s.u2b24 == 0);

    materials_set_owner(s.materials, &s);
    inter_matl_set_owner(s.phase_pairs, &s);
    objects_set_owner(s.objects2, &s);
    inter_obj_set_owner(s.inter_obj, &s);
    CHECK(s.materials.owner == &s && s.phase_pairs.owner == &s);
    CHECK(s.objects2.owner == &s && s.inter_obj.owner == &s);
}

// ---- simulation control parts (1401e7e90, 1401e8490, 1401e8fb0, 1401ee510) -------------------

TEST_CASE(sim_ctrl_names_and_counters)
{
    auto z = std::make_unique<Zeroed<Cls_1401e7b10>>();
    std::memset(z->buf, 0x77, sizeof z->buf);
    Cls_1401e7b10& c = z->get();
    sim_ctrl_init_names(c);
    CHECK(std::strcmp(c.title, "SIMULATION") == 0);
    CHECK(std::strcmp(c.oprnam, "OPERATION 1") == 0);
    CHECK(std::strcmp(c.simnam, "SIMULATION 1") == 0);
    CHECK_EQ(c.cursim[0], 1);
    CHECK_EQ(c.cursim[1], 0);
    CHECK_EQ(c.meshno, 1);
    CHECK_EQ(c.stype, 1);
    CHECK_EQ(c.unit, 0x77777777); // 0xfc is not written by init
    CHECK_EQ(c.geotyp, 1);
    CHECK_EQ(c.trans[0], 0);
    CHECK_EQ(c.trans[1], 1);
    CHECK_EQ(c.htmthd, 0);
    CHECK_EQ(c.stnout[4], 1);
    CHECK_EQ(c.stnout[6], 0);
    CHECK_EQ(c.elmnod[9], 0);

    sim_ctrl_init_u160(c);
    CHECK_EQ(c.nstart, -1);
    CHECK_EQ(c.stpinc, 10);
    CHECK_EQ(c.nstep, 1);

    sim_ctrl_init_u170(c);
    CHECK_BITS(c.emax, 0.0);
    CHECK_BITS(c.lmax[2], 0.0);
    CHECK_EQ(c.mdsobj[2], 0);
    CHECK_BITS(c.odmax, 0.0);
}

TEST_CASE(sim_ctrl_dttype_mode)
{
    auto z = std::make_unique<Zeroed<Cls_1401e7b10>>();
    Cls_1401e7b10& c = z->get();
    c.dttype.u58 = 9;
    c.dsmax = 0.0;
    sim_ctrl_update_u240(c);
    CHECK_EQ(c.dttype.u58, 1); // dsmax <= 0
    c.dsmax = 1e-300;
    sim_ctrl_update_u240(c);
    CHECK_EQ(c.dttype.u58, 0);
    c.dsmax = -2.0;
    c.dttype.ndim = 1; // a table: mode untouched
    sim_ctrl_update_u240(c);
    CHECK_EQ(c.dttype.u58, 0);
}

int main() { return testkit::run_all(); }
