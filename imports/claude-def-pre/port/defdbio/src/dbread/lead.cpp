// DBREAD: read one step of a .DB into the model (_DB_STEP_DATA::ReadRecordFromDB and the step
// lookup in front of it). Listing out/dbread/lead/L.txt (deform-rea-work), packet p-dbread.
// The subtree readers (helpers W1-W4) are declared in defdbio/dbread/*.hpp.
#include <defdbio/dbread/dbread.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/dbread/w2.hpp>
#include <defdbio/dbread/w3.hpp>
#include <defdbio/dbread/w4.hpp>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/pre_steps.hpp>
#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/ftn/unit_system.hpp>
#include <defrt/ftn/units.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::dbio::dbread {

namespace {

using model::DB_STEP_DATA;
using i32 = std::int32_t;

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// step directory entry k (0-based) of the DB object: 0x28 bytes at +0xad8
std::byte* dir_entry(void* db, i32 k) { return at<std::byte*>(db, 0xad8) + static_cast<std::size_t>(k) * 0x28; }

bool model_is_2d(const DB_STEP_DATA* m) { return at<i32>(const_cast<DB_STEP_DATA*>(m), 0x1c) == 2; } // 1400015e0
bool model_is_3d(const DB_STEP_DATA* m) { return at<i32>(const_cast<DB_STEP_DATA*>(m), 0x1c) == 3; } // 1400015f0

} // namespace

// @src 1401cc120  +0x1b60 = v (DB version of the file)
void fun_1401cc120(DB_STEP_DATA* m, i32 v) { at<i32>(m, 0x1b60) = v; }

// @src 1401d83b0  byte +0x1b5b
bool fun_1401d83b0(DB_STEP_DATA* m) { return at<std::uint8_t>(m, 0x1b5b) != 0; }

// @src 1401d8f80  byte +0x1b5a = *v
void fun_1401d8f80(DB_STEP_DATA* m, const char* v) { at<char>(m, 0x1b5a) = *v; }

// @src 1401d5f50  2D: copy six doubles of the object +0x8b8 (+0x2318..+0x2340) into the model
// (+0x8d0, +0x8d8, +0x8e8, +0x8f0, +0x900, +0x908)
void fun_1401d5f50(DB_STEP_DATA* m)
{
    void* o = gendb::ext::f1401d6ea0(m, at<i32>(m, 0x8b8)); // 14017aaf0 on +0x1380
    if (o == nullptr) return;
    at<double>(m, 0x8d0) = at<double>(o, 0x2318);
    at<double>(m, 0x8d8) = at<double>(o, 0x2320);
    at<double>(m, 0x8e8) = at<double>(o, 0x2328);
    at<double>(m, 0x8f0) = at<double>(o, 0x2330);
    at<double>(m, 0x900) = at<double>(o, 0x2338);
    at<double>(m, 0x908) = at<double>(o, 0x2340);
}

// @src 1401cd020  per object: STROKE (+0x2300..) and the second triple (+0x22e8..) from the step
// header doubles (+0x1b80) at the slots 1406e85d0.. of the object's block
i32 fun_1401cd020(DB_STEP_DATA* m, void* db, i32 step)
{
    i32 st = step;
    i32 dim = model::llobj::int_at_1c(m); // 140158e90
    model::p407::load_hdr_slots(&dim);    // 1401c7f80
    i32 base = 0, stride = 0;
    header_layout(static_cast<model::Cls_1401dbfe0*>(db), &st, &base, &stride);
    const i32 nobj = at<i32>(at<std::byte*>(db, 0xae8) + static_cast<std::size_t>(st) * 0xe0, 0);
    using model::p407::hdr_slot;
    for (i32 i = 0; i < nobj; i = i + 1) {
        void* o = model::step_object_at(*m, i + 1);
        if (o == nullptr) continue;
        double* h = at<double*>(m, 0x1b80) + (base + stride * i);
        at<double>(o, 0x2300) = h[hdr_slot(0x1406e85d0)];
        at<double>(o, 0x2308) = h[hdr_slot(0x1406e85d4)];
        if (dim == 3) at<double>(o, 0x2310) = h[hdr_slot(0x1406e85d8)];
        at<double>(o, 0x22e8) = h[hdr_slot(0x1406e85dc)];
        at<double>(o, 0x22f0) = h[hdr_slot(0x1406e85e0)];
        if (dim == 3) at<double>(o, 0x22f8) = h[hdr_slot(0x1406e85e4)];
    }
    return 0;
}

// @src 1401cc420  _DB_STEP_DATA::ReadRecordFromDB(db, index): read step `index` (directory
// position; -1 = the current one) of db (null = the model's DB, +0x3390)
i32 fun_1401cc420(DB_STEP_DATA* m, void* db, i32 index)
{
    if (at<std::byte*>(db, 0xad8) == nullptr) return 2;
    // @src 1401cc420 (1401cc46b..1401cc4ff) omitted: licence (step types 2 / 3 against licence
    // flags DAT_1406dcb6c / DAT_1406dcb70; the port reads every step type)
    model::step_clear_u2f89(*m);                      // 1401d7ef0
    model::g_timer_read_record.start("Start");       // 140199dc0
    void* d = db != nullptr ? db : static_cast<void*>(m->db);
    i32 k = index;
    if (index == -1) k = m->u740;
    if (k < 0 || at<i32>(d, 0xac8) <= k) return 2;
    model::g_timer_read_record.checkpoint("FreeData", nullptr);
    model::step_free_contents(*m);                   // 1401cb440
    if (at<i32>(dir_entry(d, k), 0x1c) == 2) {
        i32 two = 2;
        m->set_dimension(&two);                      // vtable slot 0
    } else {
        i32 three = 3;
        m->set_dimension(&three);
    }
    m->u740 = k;
    m->u744 = at<i32>(dir_entry(d, m->u740), 4);
    fun_1401cc120(m, at<i32>(d, 0xabc));
    set_step_version(m, at<i32>(dir_entry(d, m->u740), 0x20));
    model::g_timer_read_record.checkpoint("ReadStepHeader", nullptr);
    i32 rc = read_step_header(m, static_cast<model::Cls_1401dbfe0*>(d));
    if (rc != 0) {
        m->u1378 = 0;
        return rc;
    }
    if (at<i32>(dir_entry(d, m->u740), 0x24) == 0) return 2;
    model::g_timer_read_record.checkpoint("ReadArrays", &model::g_timer_read_arrays);
    rc = read_arrays(m, static_cast<model::Cls_1401dbfe0*>(d));
    model::g_timer_read_record.checkpoint("Update progressbar", nullptr);
    if (rc != 0) {
        m->u1378 = 0;
        return rc;
    }
    if (fun_1401d83b0(m)) return 0;
    model::g_timer_read_record.checkpoint("getUnitSystem", nullptr);
    rt::ftn::unit_system();                          // 140235c10 (result unused)
    model::g_timer_read_record.checkpoint("ReadSimCtrl", nullptr);
    read_sim_ctrl(m);
    rt::ftn::set_unit_system(at<i32>(m, 0x84c));     // 140235c30
    model::g_timer_read_record.checkpoint("ReadMatlInfo", nullptr);
    read_materials(m);
    model::g_timer_read_record.checkpoint("ReadInterMatlInfo", nullptr);
    read_inter_matl_info(m);
    model::g_timer_read_record.checkpoint("ReadObjInfo", nullptr);
    read_obj_info(m);
    model::g_timer_read_record.checkpoint("ReadInterObjInfo", nullptr);
    read_inter_obj_info(m);
    model::step_material_state_sizes(*m);            // 1401d0320
    model::g_timer_read_record.checkpoint("ReadArrays", &model::g_timer_read_obj_state_variable);
    read_arrays(m);
    if (model_is_2d(m)) fun_1401d5f50(m);
    if (model_is_3d(m)) {
        model::g_timer_read_record.checkpoint("ReadObjFoldInfoFromDB", nullptr);
        read_obj_fold_info(m);
    }
    model::g_timer_read_record.checkpoint("ProcessStepHeader", nullptr);
    rc = fun_1401cd020(m, d, m->u740);
    if (rc != 0) {
        m->u1378 = 0;
        return rc;
    }
    model::g_timer_read_record.checkpoint("free DB arrays", nullptr);
    for (std::size_t off : {std::size_t{0x1b80}, std::size_t{0x1b88}, std::size_t{0x1b78}, std::size_t{0x2240}}) {
        void*& p = at<void*>(m, off);
        if (p != nullptr) std::free(p);
        p = nullptr;
    }
    model::g_timer_read_record.checkpoint("ReadDynamicArrayFromDB", nullptr);
    if (10999 < gendb::stored_version(*m)) read_dynamic_arrays(m);   // 1401cc180
    model::g_timer_read_record.checkpoint("genGeoSymPlane", nullptr);
    if (model_is_3d(m)) gendb::objects_surface_normals(m, 0); // 1401d63c0
    char report = 1;
    model::g_timer_read_record.stop(&report);        // 140199eb0
    char flag;
    if (!model::names_up_to_date(m) || !gendb::step_fun_1401d2bb0(m)) flag = 1;
    else flag = 0;
    fun_1401d8f80(m, &flag);
    return 0;
}

// @src 1401cc2a0  DBREAD by step number: find `stepno` in the directory of db, read it, and when
// `fold` is set run 1401dd4d0(db, &index, arg5)
i32 fun_1401cc2a0(DB_STEP_DATA* m, void* db, i32 stepno, char fold, char arg5)
{
    if (step_links_db(m)) m->db = static_cast<model::Cls_1401dbfe0*>(db);
    i32 k = 0;
    while (true) {
        if (at<i32>(db, 0xac8) <= k) {
            // 1401cc2f0: __iob_func() + 0x30 = stdout
            rt::crt::fprintf(stdout, "Step %d not found in Database %s.\n", stepno,
                             reinterpret_cast<const char*>(static_cast<std::byte*>(db) + 0x650));
            return 1;
        }
        if (at<i32>(dir_entry(db, k), 4) == stepno) break;
        k = k + 1;
    }
    const i32 rc = fun_1401cc420(m, db, k);
    if (rc != 0) return rc;
    if (fold == 0) return 0;
    read_header_cache(static_cast<model::Cls_1401dbfe0*>(db), &k, arg5 != 0);
    return rc;
}


// part of @src 1401cc2a0  hook-typed entry for p5-01's dispatcher (defcmd/dispatch.hpp)
int db_read_step(DB_STEP_DATA* m, model::Cls_1401dbfe0* db, i32 stepno, std::uint8_t a, std::uint8_t b)
{
    return fun_1401cc2a0(m, db, stepno, static_cast<char>(a), static_cast<char>(b));
}

} // namespace def::dbio::dbread
