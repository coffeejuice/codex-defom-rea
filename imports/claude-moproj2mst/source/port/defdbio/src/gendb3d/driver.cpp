// gendb3d/driver.cpp: the 3D object-array writer of GENDB, FUN_14017b370 (packet p4-05).
// Order per object and the computed offsets: docs/re/dbmap/objarrays.md section 1 (branch
// claude/project-thread-bz5k5e).
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/gendb3d/obj3d.hpp>

#include <cstdio>
#include <cstdlib>

#include <defdbio/step/db_info_read.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_obj3d.hpp"

namespace def::dbio::g3d {

namespace {
// FUN_14047903c() + 0x30: stdout of the VS2010 CRT
std::FILE* out_stream() { return stdout; }
} // namespace

// @src 140174b40  debug checkpoint after each writer: the body is a bare ret in this build
static void debug_checkpoint(LLOBJ_DATA*, std::int32_t*, double*, const std::int32_t*, const std::int32_t*,
                             const char*)
{
}

// @src 14017b230  CHK_VOL.INI: "%lf" volume, "%d" object, "%d" compensation flag (mode "rt", str
//                 14052048c); messages to stdout
void read_chk_vol_ini(void* /*unused*/, double* volume, std::uint32_t* object, std::uint32_t* flag)
{
    *object = 0;
    *volume = 0.0;
    if (!file_exists("CHK_VOL.INI")) { // 1401b8560
        rt::crt::fprintf(out_stream(), "CHK_VOL.INI doesn't exist\n");
        return;
    }
    std::FILE* f = std::fopen("CHK_VOL.INI", "rt"); // 14047b934
    if (f == nullptr) return;
    std::fscanf(f, "%lf", volume);
    std::fscanf(f, "%d", object);
    std::fscanf(f, "%d", flag);
    std::fclose(f);
    rt::crt::fprintf(out_stream(), "CHK_VOL.INI exists (Object %d)\n", *object);
    rt::crt::fprintf(out_stream(), "Target volume will be set as %lf\n", *volume);
    rt::crt::fprintf(out_stream(), "Volume compensation flag is %d\n", *flag);
}

// @src 14017b370  3D objects into MAR/A (see the header). mar_tab / a_tab indices used directly:
//                 MAR 3 IIOBJ (width [73]), 4 IINOD, 5 IIMTR, 6 IISIM, 7 IIBCD, 13 IIDOD, 14 IIBDY,
//                 15 IIBEL, 16 IIBND, 23 IIFLD; A 5 LLOBJ (width [145]), 6 LLBND (width [146]),
//                 8 LLGRY, 11 LLCRZ, 14 LLFLD; [149] LLWIN width
void write_objects_3d(model::Cls_14017a6c0* objs, std::int32_t* mar, double* a, const std::int32_t* mar_tab,
                      const std::int32_t* a_tab, std::int32_t* iifnc, double* llfnc, double* llwin,
                      double* llwrz)
{
    DB_STEP_DATA* const m = objs->owner;
    std::int32_t* iiobj_row = nullptr;
    double* llobj_row = nullptr;
    LLOBJ_DATA* obj = nullptr;
    const std::int32_t nobj = ext::f14019a280(objs);
    std::int32_t* const iiobj = mar + (mar_tab[3] - 1);
    const std::int32_t a_obj = a_tab[5];
    double* const llbnd = a + (a_tab[6] - 1);
    double* const llgry = a + (a_tab[8] - 1);
    std::int32_t* const iidod = mar + (mar_tab[13] - 1);
    std::int32_t* const iibnd = mar + (mar_tab[16] - 1);
    std::int32_t* const iibdy = mar + (mar_tab[14] - 1);
    std::int32_t* const iibel = mar + (mar_tab[15] - 1);
    const std::int32_t iiobj_w = mar_tab[73];
    const std::int32_t llobj_w = a_tab[145];
    std::int32_t llbnd_w = a_tab[146];
    const std::int32_t iinod = mar_tab[4];
    const std::int32_t win_w = a_tab[149];
    const std::int32_t fold_n0 = fold_node_offset(m);
    fold_face_offset(m);
    std::int32_t zero = 0;
    set_fold_node_offset(m, &zero);
    std::int32_t zero2 = 0;
    set_fold_face_offset(m, &zero2);
    (void)fold_n0;
    if (rt::dimension_is_3(m)) {
        double volume = 0.0;
        std::uint32_t vobj = 0;
        std::uint32_t vflag = 0;
        read_chk_vol_ini(objs, &volume, &vobj, &vflag);
        if (0 < static_cast<std::int32_t>(vobj) && static_cast<std::int32_t>(vobj) <= nobj) {
            obj = ext::f14017aaf0(objs, static_cast<std::int32_t>(vobj));
            if (obj != nullptr && fld<double>(obj, 0x2090) != 0.0) { // DAT_14051bed8 (NaN: not equal)
                rt::crt::fprintf(out_stream(), "Target volume of object %d : %lf -> %lf\n",
                                 fld<std::uint32_t>(obj, 8), fld<double>(obj, 0x2090), volume);
                fld<double>(obj, 0x2090) = volume;
            }
        }
    }
    std::int32_t nmat = ext::f14029c410(&m->materials);
    std::int32_t* mat_pos = nullptr;
    if (nmat != 0) {
        mat_pos = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(nmat)) * 4));
        material_positions(&m->materials, mat_pos, &nmat);
    }
    std::int32_t conn_next = iinod; // local_140
    std::int32_t conn = iinod;      // local_150
    char meshed = 0;
    std::int32_t rec_last = 0;
    for (std::int32_t k = 0; k < nobj; k = k + 1) {
        obj = objs->at(k + 1); // 14017aa80
        if (fld<std::int32_t>(obj, 0xc) == 7) continue;
        iiobj_row = iiobj + iiobj_w * k;
        llobj_row = a + (a_obj - 1) + llobj_w * k;
        meshed = 1;
        if (fld<std::int32_t>(obj, 0xc) == 0 &&
            (ext::f1401f3d90(&m->sim_ctrl) || fld<std::int32_t>(obj, 0x880) < 4 || fld<std::int32_t>(obj, 0x884) < 1)) {
            // geometry-only record (type 1)
            std::int32_t type = 1;
            std::int32_t rec = record_counter(m);
            const std::int32_t row = rec * llbnd_w;
            rec = rec + 1;
            set_record_counter(m, &rec);
            write_boundary_info(obj, &type, llbnd + row, llgry, iidod, iifnc, llfnc, iibdy, &rec);
            meshed = 0;
        }
        if (meshed != 0) {
            std::int32_t node_off = node_offset(m);
            std::int32_t elem_off = elem_offset(m);
            const std::int32_t npe = reinterpret_cast<model::Cls_140182880*>(at(obj, 0x9e0))->width(); // 140185530
            conn = conn_next;
            conn_next = conn_next + fld<std::int32_t>(obj, 0x884) * npe;
            std::int32_t face_nodes = 4; // local_c0 (not read further)
            if (npe == 4) face_nodes = 3;
            (void)face_nodes;
            write_node_info(obj, mar, a, mar_tab, a_tab, &node_off);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteNodeInfoToDB");
            write_elem_int_info(obj, mar, mar_tab, &conn, &elem_off, &node_off);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteElemIntegerInfoToDB");

            std::int32_t* surf = &fld<std::int32_t>(obj, 0xbe0); // surface 0: +0 nodes, +8 list, +0x10 faces
            const std::int32_t bnd0 = bnd_node_offset(m);
            std::int32_t* dst = iibnd + bnd0;
            for (std::int32_t i = 0; i < surf[0]; i = i + 1) {
                dst[i] = fld<std::int32_t*>(surf, 8)[i] + node_off; // IIBND
            }
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after pBND");
            std::int32_t face_off = face_offset(m);
            for (std::int32_t i = 0; i < surf[4] * 4; i = i + 1) {
                iibdy[(face_off << 2) + i] = fld<std::int32_t*>(surf, 0x18)[i] + node_off; // IIBDY
            }
            dst = iibel + face_off;
            for (std::int32_t i = 0; i < surf[4]; i = i + 1) {
                dst[i] = fld<std::int32_t*>(surf, 0x20)[i] + elem_off; // IIBEL
            }
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after IIBEL");

            std::int32_t fn_cap = bc_function_capacity(obj);
            auto* fn_used = static_cast<std::int32_t*>(
                std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(fn_cap)) * 4));
            for (std::int32_t i = 0; i < fn_cap; i = i + 1) fn_used[i] = 0;
            std::int32_t sym_cap = bc_function_capacity(obj);
            auto* sym_used = static_cast<std::int32_t*>(
                std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(sym_cap)) * 4));
            for (std::int32_t i = 0; i < sym_cap; i = i + 1) sym_used[i] = 0;
            std::uint32_t face_kinds = 0;
            write_edge_face_info(obj, mar, a, mar_tab, a_tab, &face_off, &surf[4], fn_used, &face_kinds);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteEdgeFaceInfoToDB");
            write_elem_info(obj, mar, a, mar_tab, a_tab, &elem_off, &node_off);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteElemInfoToDB");
            std::uint32_t fn_kinds = 0;
            std::int32_t sym_any = 0;
            write_node_int_info(obj, mar, mar_tab, &node_off, fn_used, sym_used, &fn_cap, &sym_cap,
                                &fn_kinds, &sym_any);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteNodeIntegerInfoToDB");
            write_bc_functions(obj, mar, mar_tab, &node_off, &face_off, &surf[4], iifnc, llfnc, fn_used,
                               sym_used, &fn_kinds, &sym_any, &face_kinds);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteBdryCndFuncToDB");
            std::free(fn_used);
            std::free(sym_used);

            std::int32_t type = 3;
            if (fld<std::int32_t>(obj, 0xc) == 0 && object_has_geometry(obj)) {
                type = 2;
            }
            std::int32_t rec = record_counter(m);
            const std::int32_t row = rec * llbnd_w;
            rec = rec + 1;
            set_record_counter(m, &rec);
            write_boundary_info(obj, &type, llbnd + row, llgry, iidod, iifnc, llfnc, iibdy, &rec);
            debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteBdryInfoToDB");
        }
        rec_last = record_counter(m);
        write_general_info(obj, iiobj_row, llobj_row, (conn - iinod) + 1, rec_last, 1, iifnc, llfnc, mat_pos,
                           &meshed);
        debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after WriteGeneralInfoToDB");
        if (fld<std::int32_t>(obj, 0xc) != 0 && fold_flag(m) != 0) {
            const std::int32_t fn0 = fold_node_offset(m);
            const std::int32_t ff0 = fold_face_offset(m);
            std::int32_t nfn = 0;
            std::int32_t nff = 0;
            double* llfld = a + (a_tab[14] - 1);
            std::int32_t* iifld = mar + (mar_tab[23] - 1);
            write_fold_info(obj, iiobj_row, llfld + fn0 * 3, iifld + (ff0 << 2), &nfn, &nff);
            std::int32_t v = fn0 + nfn;
            set_fold_node_offset(m, &v);
            std::int32_t v2 = ff0 + nff;
            set_fold_face_offset(m, &v2);
        }
        write_obj_1ea0(at(obj, 0x1ea0), iiobj_row, llobj_row, iifnc, llfnc, m, &fld<std::int32_t>(obj, 8));
        char f_bc = 1;   // local_101
        char f_win = 1;  // local_102
        char f_heat = 1; // local_104
        char f_103 = 1;  // local_103
        if (fld<std::int32_t>(obj, 0xc) == 0 && ext::f1401f3d90(&m->sim_ctrl)) {
            f_bc = 0;
            f_win = 0;
        }
        else if (fld<std::int32_t>(obj, 0xc) == 0 && fld<std::int32_t>(obj, 0x884) < 1) {
            f_bc = 0;
            f_win = 0;
        }
        else {
            f_bc = 1;
            f_win = 1;
        }
        if (ext::f1401f3c30(&m->sim_ctrl) == 0 || fld<std::int32_t>(obj, 0x884) < 1) {
            f_heat = 0;
        }
        else {
            f_heat = 1;
        }
        write_obj_2d10(at(obj, 0x2d10), iiobj_row, llobj_row, iifnc, llfnc, llwin, win_w, llwrz, &f_bc, m,
                       &fld<std::int32_t>(obj, 8));
        write_obj_2e68(at(obj, 0x2e68), iiobj_row, llobj_row, iifnc, llfnc, llwin, win_w, llwrz, &f_win,
                       &f_heat, &f_103, m, &fld<std::int32_t>(obj, 8));
        debug_checkpoint(obj, mar, a, mar_tab, a_tab, "after BdryCnd.WriteToDB");
        write_general_info2(obj, iiobj_row, llobj_row, (conn - iinod) + 1, rec_last, 1, iifnc, llfnc, mat_pos,
                            &meshed);
    }
    std::uint32_t pdie = static_cast<std::uint32_t>(fld<std::int32_t>(m, 0x8b8));
    obj = ext::f14017aaf0(objs, static_cast<std::int32_t>(pdie));
    if (obj == nullptr) {
        if (ext::f1401f3c30(&m->sim_ctrl) != 0) {
            rt::crt::fprintf(out_stream(), "Invalid Pricipal Die Defined; PDIE = %d\n", pdie);
            std::int32_t c1 = 0xe;
            std::int32_t c3 = 0;
            ext::f14001cd90(&c1, reinterpret_cast<const void*>(0x14052274c), &pdie, &c3);
        }
    }
    else {
        std::int32_t* iisim = mar + (mar_tab[6] - 1);
        iisim[7] = fld<std::int32_t>(obj, 0x3588); // PDIE record (byte offset 0x1c)
    }
    convert_contact_codes(objs, a + (a_tab[11] - 1), mar + (mar_tab[7] - 1), llbnd, &llbnd_w);
    if (nmat != 0) {
        std::int32_t* iimtr = mar + (mar_tab[5] - 1);
        const std::int32_t ne = elem_offset(m);
        for (std::int32_t i = 0; i < ne; i = i + 1) {
            const std::int32_t v = iimtr[i];
            if (0 < v) iimtr[i] = mat_pos[v - 1];
        }
    }
    if (mat_pos != nullptr) std::free(mat_pos);
}

} // namespace def::dbio::g3d
