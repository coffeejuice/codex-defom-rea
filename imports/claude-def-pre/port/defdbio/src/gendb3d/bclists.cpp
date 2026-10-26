// gendb3d/bclists.cpp: GENDB writers of three LLOBJ_DATA sub-objects (packet p4-05):
//   0x1ea0 Cls_1401860a0  three curves and two lists -> LLOBJ words, IIOBJ words, LLFNC functions
//   0x2d10 Cls_140189440  heat-exchange windows -> LLOBJ words, LLWIN rows, LLWRZ coordinates
//   0x2e68 Cls_1401956d0  BC window lists -> LLOBJ words, LLWIN rows, LLWRZ coordinates
// and the LLWINDEF_DATA record writer (140260ea0) with its helpers. Mapping:
// docs/re/dbmap/object.md (branch claude/project-thread-bz5k5e).
//
// Sub-objects are read by the binary's offsets (access.hpp). iiobj is indexed by the byte offset
// / 4 and llobj / rec by the byte offset / 8 of the decompile.
#include "gendb3d/bclists.hpp"

#include <cstdio>
#include <cstring>

#include <defdbio/gendb3d/obj3d.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_bclists.hpp"

namespace def::dbio::g3d {

namespace {

// Constants of the listing (DAT_...): the codes stored in the records.
constexpr double k0 = 0.0;   // DAT_14051bed8
constexpr double k1 = 1.0;   // DAT_140509c90
constexpr double k2 = 2.0;   // DAT_140509fb8
constexpr double k3 = 3.0;   // DAT_14051f1c8
constexpr double k4 = 4.0;   // DAT_14051f248
constexpr double k5 = 5.0;   // DAT_140522f48
constexpr double k6 = 6.0;   // DAT_140522ff0
constexpr double k7 = 7.0;   // DAT_140522f38
constexpr double k8 = 8.0;   // DAT_140522f40
constexpr double k9 = 9.0;   // DAT_140522fe8
constexpr double k10 = 10.0; // DAT_14051bee8
constexpr double k11 = 11.0; // DAT_140522fe0
constexpr double k20 = 20.0; // DAT_14051f1f0

// FUN_14047903c() + 0x30: the CRT's stdout
std::FILE* out_stream() { return stdout; }

// 32-bit NEG (wraps for INT_MIN)
inline std::int32_t neg32(std::int32_t v) { return static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)); }
// FUN_140484ab8: CRT abs (abs(INT_MIN) == INT_MIN)
inline std::int32_t crt_abs(std::int32_t v) { return v < 0 ? neg32(v) : v; }
// 32-bit SHL
inline std::int32_t shl32(std::int32_t v, int n) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v) << n); }

inline model::DYNARY* dyn(void* base, std::ptrdiff_t off) { return reinterpret_cast<model::DYNARY*>(at(base, off)); }
inline model::OBJ_GEO3D* geo(void* base, std::ptrdiff_t off) { return reinterpret_cast<model::OBJ_GEO3D*>(at(base, off)); }

// The binary's repeated sequence: pos = 0, len = <length of the DYNARY>, allocate a function
// (number into *number), then the DYNARY into LLFNC at pos.
inline void store_function(DB_STEP_DATA* m, std::int32_t len, std::int32_t* iifnc, double* llfnc,
                           const std::int32_t* id, double* number, model::DYNARY* f)
{
    std::int32_t pos = 0;
    alloc_function(m, &len, iifnc, id, number, &pos);
    write_dynary_to_db_array(f, llfnc + (pos - 1));
}

// One window list of write_obj_2d10 / write_obj_2e68: every node (filtered by take) into LLWIN
// from the DB_WRITE_VAR cursors; *first / *last (LLOBJ words) = first / last row when rows were
// written; the cursors are stored back.
template <class Count, class Take>
void write_window_list(void* list, Count count, Take take, double* llobj, std::ptrdiff_t off_first,
                       std::ptrdiff_t off_last, std::int32_t* iifnc, double* llfnc, double* llwin,
                       std::int32_t win_w, double* llwrz, DB_STEP_DATA* m,
                       const std::int32_t* obj_no);

} // namespace

// ================================================================================================
// LLWINDEF_DATA record helpers
// ================================================================================================

// @src 140258f10  if (dst) memcpy(dst, r + 0x08, 0x18)
void record_e8_get_08(const void* r, void* dst)
{
    if (dst != nullptr) std::memcpy(dst, at(const_cast<void*>(r), 0x08), 0x18);
}
// @src 140258f90  if (dst) memcpy(dst, r + 0x20, 0x18)
void record_e8_get_20(const void* r, void* dst)
{
    if (dst != nullptr) std::memcpy(dst, at(const_cast<void*>(r), 0x20), 0x18);
}
// @src 140259020  if (dst) memcpy(dst, r + 0x68, 0x18)
void record_e8_get_68(const void* r, void* dst)
{
    if (dst != nullptr) std::memcpy(dst, at(const_cast<void*>(r), 0x68), 0x18);
}
// @src 1402590b0  if (dst) memcpy(dst, r + 0x80, 0x18)
void record_e8_get_80(const void* r, void* dst)
{
    if (dst != nullptr) std::memcpy(dst, at(const_cast<void*>(r), 0x80), 0x18);
}
// @src 140259130  if (dst) memcpy(dst, r + 0x98, 0x18)
void record_e8_get_98(const void* r, void* dst)
{
    if (dst != nullptr) std::memcpy(dst, at(const_cast<void*>(r), 0x98), 0x18);
}

// @src 140258c40  out[6] = type; types 0..2, 3, 4, 5, 7 copy their fields (6 and others: none)
void record_e8_to_values(const void* r, double* out)
{
    out[6] = static_cast<double>(fld<std::int32_t>(r, 4));
    switch (fld<std::int32_t>(r, 4)) {
    case 0:
    case 1:
    case 2:
        record_e8_get_08(r, out);
        record_e8_get_20(r, out + 3);
        out[7] = static_cast<double>(fld<std::int32_t>(r, 0x38));
        out[8] = fld<double>(r, 0x40);
        out[9] = fld<double>(r, 0x48);
        out[10] = fld<double>(r, 0x50);
        out[11] = fld<double>(r, 0x58);
        break;
    case 3:
        record_e8_get_08(r, out);
        record_e8_get_20(r, out + 3);
        out[7] = static_cast<double>(fld<std::int32_t>(r, 0x60));
        break;
    case 4:
        record_e8_get_68(r, out);
        record_e8_get_80(r, out + 3);
        record_e8_get_98(r, out + 9);
        out[7] = static_cast<double>(fld<std::int32_t>(r, 0xb0));
        out[8] = fld<double>(r, 0xb8);
        break;
    case 5:
        record_e8_get_20(r, out + 3);
        out[8] = fld<double>(r, 0xb8);
        break;
    case 7:
        record_e8_get_20(r, out);
        record_e8_get_08(r, out + 3);
        out[7] = fld<double>(r, 0x40);
        out[8] = static_cast<double>(fld<std::int32_t>(r, 0xc8));
        out[9] = static_cast<double>(fld<std::int32_t>(r, 0xcc));
        out[10] = fld<double>(r, 0xd0);
        out[11] = fld<double>(r, 0xd8);
        break;
    }
}

// ================================================================================================
// LLWINDEF_DATA::WriteToDBArrays and helpers
// ================================================================================================

// @src 140262250  LLWINDEF_DATA::storeWindefFric: column set by type (1 or 7; type 7 first maps
//                 the curve kind (+0x24 / +0x30) into +0x18), then friction / its curve and,
//                 for kind 4 (+8), the second set (columns 13.. 3D, 13..17 2D)
void windef_store_fric(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                       const std::int32_t* obj_no)
{
    std::int32_t c_kind = 6;  // local_30
    std::int32_t c_value = 7; // local_28
    std::int32_t c_func = 8;  // local_38
    std::int32_t c_mode = 9;  // local_34
    const std::int32_t type = fld<std::int32_t>(w, 0x174);
    if (type == 1) {
        c_kind = 6;
        c_value = 7;
        c_func = 8;
        c_mode = 9;
    }
    else if (type == 7) {
        c_kind = 0xd;
        c_value = 9;
        c_func = 9;
        c_mode = 8;
        if (fld<std::int32_t>(w, 0x24) == 0) {
            fld<std::int32_t>(w, 0x18) = 0;
        }
        else if (fld<std::int32_t>(w, 0x24) == 1) {
            const std::int32_t v = fld<std::int32_t>(w, 0x30);
            if (v == 1) {
                fld<std::int32_t>(w, 0x18) = 1;
            }
            else if (v == 3) {
                fld<std::int32_t>(w, 0x18) = 3;
            }
            else if (v == 4) {
                fld<std::int32_t>(w, 0x18) = 2;
            }
            else if (v == 0x15) {
                fld<std::int32_t>(w, 0x18) = 4;
            }
        }
        else if (fld<std::int32_t>(w, 0x24) == 3) {
            fld<std::int32_t>(w, 0x18) = 5;
        }
        else {
            fld<std::int32_t>(w, 0x18) = 0;
        }
    }
    else {
        // str 140535e08
        rt::crt::fprintf(out_stream(), "incorrect function call in LLWINDEF_DATA::storeWindefFric()\n");
    }
    const std::int32_t kind = fld<std::int32_t>(w, 8);
    if (kind == 1) {
        rec[c_kind] = k1;
    }
    else if (kind == 2) {
        rec[c_kind] = k2;
    }
    else if (kind == 3) {
        rec[c_kind] = k3;
    }
    else if (kind == 4) {
        rec[c_kind] = k4;
    }
    else {
        rec[c_kind] = 0.0;
    }
    if (fld<char>(w, 0xc) == '\0') {
        rec[10] = 0.0;
        rec[11] = 0.0;
        rec[12] = 0.0;
        rec[c_mode] = static_cast<double>(fld<std::int32_t>(w, 0x18));
        const std::int32_t mode = fld<std::int32_t>(w, 0x18);
        if (mode == 0) {
            rec[c_value] = fld<double>(w, 0x28);
        }
        else if (mode == 5) {
            rec[10] = static_cast<double>(fld<std::int32_t>(w, 0x3c));
            rec[11] = static_cast<double>(fld<std::int32_t>(w, 0x40));
            rec[12] = static_cast<double>(fld<std::int32_t>(w, 0x44));
        }
        else if (mode == 6) {
            rec[10] = fld<double>(w, 0x110);
            rec[11] = fld<double>(w, 0x118);
        }
        else if (mode == 7) {
            rec[10] = fld<double>(w, 0x120);
            rec[11] = fld<double>(w, 0x128);
        }
        else if (mode == 8) {
            rec[10] = fld<double>(w, 0x130);
            rec[11] = fld<double>(w, 0x138);
        }
        if (fld<std::int32_t>(w, 0x24) != 0) {
            store_function(m, ext::f140130290(at(w, 0x20)), iifnc, llfnc, obj_no, rec + c_func,
                           dyn(w, 0x20));
        }
    }
    else {
        rec[c_mode] = static_cast<double>(neg32(fld<std::int32_t>(w, 0x10)));
        rec[c_value] = fld<double>(w, 0x28);
    }
    if (!ext::f14025c830(w)) {
        if (fld<std::int32_t>(w, 8) == 4) {
            if (fld<char>(w, 0xd) == '\0') {
                rec[17] = 0.0;
                rec[18] = 0.0;
                rec[19] = 0.0;
                rec[16] = static_cast<double>(fld<std::int32_t>(w, 0x1c));
                const std::int32_t mode = fld<std::int32_t>(w, 0x1c);
                if (mode == 0) {
                    rec[13] = fld<double>(w, 0xa0);
                }
                else if (mode == 5) {
                    rec[17] = static_cast<double>(fld<std::int32_t>(w, 0xb4));
                    rec[18] = static_cast<double>(fld<std::int32_t>(w, 0xb8));
                    rec[19] = static_cast<double>(fld<std::int32_t>(w, 0xbc));
                }
                else if (mode == 6) {
                    rec[17] = fld<double>(w, 0x140);
                    rec[18] = fld<double>(w, 0x148);
                }
                else if (mode == 7) {
                    rec[17] = fld<double>(w, 0x150);
                    rec[18] = fld<double>(w, 0x158);
                }
                else if (mode == 8) {
                    rec[17] = fld<double>(w, 0x160);
                    rec[18] = fld<double>(w, 0x168);
                }
                if (fld<std::int32_t>(w, 0x9c) != 0) {
                    store_function(m, ext::f140130290(at(w, 0x98)), iifnc, llfnc, obj_no, rec + 13,
                                   dyn(w, 0x98));
                }
            }
            else {
                rec[16] = static_cast<double>(neg32(fld<std::int32_t>(w, 0x14)));
                rec[13] = fld<double>(w, 0xa0);
            }
        }
    }
    else if (fld<std::int32_t>(w, 8) == 4) {
        if (fld<char>(w, 0xd) == '\0') {
            rec[15] = 0.0;
            rec[16] = 0.0;
            rec[17] = 0.0;
            rec[14] = static_cast<double>(fld<std::int32_t>(w, 0x1c));
            const std::int32_t mode = fld<std::int32_t>(w, 0x1c);
            if (mode == 0) {
                rec[13] = fld<double>(w, 0xa0);
            }
            else if (mode == 5) {
                rec[15] = static_cast<double>(fld<std::int32_t>(w, 0xb4));
                rec[16] = static_cast<double>(fld<std::int32_t>(w, 0xb8));
                rec[17] = static_cast<double>(fld<std::int32_t>(w, 0xbc));
            }
            else if (mode == 6) {
                rec[15] = fld<double>(w, 0x140);
                rec[16] = fld<double>(w, 0x148);
            }
            else if (mode == 7) {
                rec[15] = fld<double>(w, 0x150);
                rec[16] = fld<double>(w, 0x158);
            }
            else if (mode == 8) {
                rec[15] = fld<double>(w, 0x160);
                rec[16] = fld<double>(w, 0x168);
            }
            if (fld<std::int32_t>(w, 0x9c) != 0) {
                store_function(m, ext::f140130290(at(w, 0x98)), iifnc, llfnc, obj_no, rec + 13,
                               dyn(w, 0x98));
            }
        }
        else {
            rec[14] = static_cast<double>(neg32(fld<std::int32_t>(w, 0x14)));
            rec[13] = fld<double>(w, 0xa0);
        }
    }
}

// @src 140262c00  +0x74c == 0: rec[6] = 0, rec[7] = +0x750; else rec[6] = 1 and the table at
//                 +0x748 as a function (length 140130200), its number in rec[7]
void windef_store_748(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                      const std::int32_t* obj_no)
{
    if (fld<std::int32_t>(w, 0x74c) == 0) {
        rec[6] = 0.0;
        rec[7] = fld<double>(w, 0x750);
    }
    else {
        rec[6] = k1;
        store_function(m, ext::f140130200(at(w, 0x748)), iifnc, llfnc, obj_no, rec + 7,
                       dyn(w, 0x748));
    }
}

// @src 140262cf0  three value-or-function pairs: +0x380 -> rec[6]/[7] (length 140130200),
//                 +0x3f8 -> rec[8]/[9] (kind 2 also rec[10]/[11]), +0x470 -> rec[12]/[13]
void windef_store_380(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                      const std::int32_t* obj_no)
{
    if (fld<std::int32_t>(w, 900) == 0) {
        rec[6] = 0.0;
        rec[7] = fld<double>(w, 0x388);
    }
    else {
        rec[6] = k1;
        store_function(m, ext::f140130200(at(w, 0x380)), iifnc, llfnc, obj_no, rec + 7,
                       dyn(w, 0x380));
    }
    if (fld<std::int32_t>(w, 0x3fc) == 0) {
        rec[8] = 0.0;
        rec[9] = fld<double>(w, 0x400);
    }
    else {
        if (fld<std::int32_t>(w, 0x3fc) == 1) {
            const std::int32_t v = fld<std::int32_t>(w, 0x408);
            if (v == 1) {
                rec[8] = k1;
            }
            else if (v == 3) {
                rec[8] = k2;
            }
        }
        else if (fld<std::int32_t>(w, 0x3fc) == 2) {
            rec[8] = k3;
            rec[10] = static_cast<double>(fld<std::int32_t>(w, 0x414));
            rec[11] = static_cast<double>(fld<std::int32_t>(w, 0x418));
        }
        store_function(m, ext::f140130290(at(w, 0x3f8)), iifnc, llfnc, obj_no, rec + 9,
                       dyn(w, 0x3f8));
    }
    if (fld<std::int32_t>(w, 0x474) == 0) {
        rec[12] = 0.0;
        rec[13] = fld<double>(w, 0x478);
    }
    else {
        if (fld<std::int32_t>(w, 0x474) == 1) {
            const std::int32_t v = fld<std::int32_t>(w, 0x480);
            if (v == 1) {
                rec[12] = k1;
            }
            else if (v == 3) {
                rec[12] = k2;
            }
        }
        store_function(m, ext::f140130290(at(w, 0x470)), iifnc, llfnc, obj_no, rec + 13,
                       dyn(w, 0x470));
    }
}

// @src 140263040  two value-or-function pairs: +0x5e0 -> rec[6]/[7], +0x658 -> rec[8]/[9]
//                 (kind 2 also rec[10]/[11])
void windef_store_5e0(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                      const std::int32_t* obj_no)
{
    if (fld<std::int32_t>(w, 0x5e4) == 0) {
        rec[6] = 0.0;
        rec[7] = fld<double>(w, 0x5e8);
    }
    else {
        if (fld<std::int32_t>(w, 0x5e4) == 1) {
            const std::int32_t v = fld<std::int32_t>(w, 0x5f0);
            if (v == 1) {
                rec[6] = k1;
            }
            else if (v == 3) {
                rec[6] = k2;
            }
        }
        store_function(m, ext::f140130290(at(w, 0x5e0)), iifnc, llfnc, obj_no, rec + 7,
                       dyn(w, 0x5e0));
    }
    if (fld<std::int32_t>(w, 0x65c) == 0) {
        rec[8] = 0.0;
        rec[9] = fld<double>(w, 0x660);
    }
    else {
        if (fld<std::int32_t>(w, 0x65c) == 1) {
            const std::int32_t v = fld<std::int32_t>(w, 0x668);
            if (v == 1) {
                rec[8] = k1;
            }
            else if (v == 0xc) {
                rec[8] = k2;
            }
        }
        else if (fld<std::int32_t>(w, 0x65c) == 2) {
            rec[8] = k3;
            rec[10] = static_cast<double>(fld<std::int32_t>(w, 0x674));
            rec[11] = static_cast<double>(fld<std::int32_t>(w, 0x678));
        }
        store_function(m, ext::f140130290(at(w, 0x658)), iifnc, llfnc, obj_no, rec + 9,
                       dyn(w, 0x658));
    }
}

// @src 140260ea0  LLWINDEF_DATA::WriteToDBArrays: rec[0] = window type (+0x174); the shape
//                 (+0x210) and its coordinates into llwrz from *pos (types other than 5..7);
//                 the type's own columns; *row += 1. Types 3 and 4 of a 3D model without a
//                 shape return after rec[0] (no row).
void windef_write_to_db_arrays(void* w, double* rec, double* llwrz, std::int32_t* iifnc,
                               double* llfnc, std::int32_t* row, std::int32_t* pos,
                               DB_STEP_DATA* m, const std::int32_t* obj_no, std::int32_t code2d)
{
    bool no_shape_3d = false;
    if (rt::dimension_is_3(m) && fld<std::int32_t>(w, 0x210) == 0) {
        no_shape_3d = true;
    }
    switch (fld<std::int32_t>(w, 0x174) - 1) {
    case 0:
        rec[0] = k1;
        break;
    case 1:
        rec[0] = k2;
        break;
    case 2:
        rec[0] = k3;
        if (no_shape_3d) return;
        break;
    case 3:
        rec[0] = k4;
        if (no_shape_3d) return;
        break;
    case 4:
        rec[0] = k5;
        break;
    case 5:
        rec[0] = k6;
        break;
    case 6:
        rec[0] = k7;
        break;
    case 7:
        rec[0] = k8;
        break;
    case 9:
        rec[0] = k9;
        break;
    default: // jump table entry 8 (type 9) also lands here
        // str 140535d20
        rt::crt::printf("ERROR in LLWINDEF_DATA::WriteToDBArrays(): not implemented windef type\n");
        break;
    }
    if (fld<std::int32_t>(w, 0x174) != 5 && fld<std::int32_t>(w, 0x174) != 6 &&
        fld<std::int32_t>(w, 0x174) != 7) {
        if (!ext::f14025c830(w)) {
            std::int32_t n = 0;     // local_48: LLWRZ rows of the shape
            std::int32_t n4 = 0;    // local_4c
            std::int32_t extra = 0; // local_44
            switch (fld<std::int32_t>(w, 0x210) - 1) {
            case 0:
                rec[14] = k1;
                n = 8;
                break;
            case 1:
                rec[14] = k2;
                n = 3;
                break;
            case 2:
                rec[7] = static_cast<double>(geo(w, 0x220)->node_count());
                rec[8] = static_cast<double>(geo(w, 0x220)->elem_count());
                rec[14] = k3;
                n = geo(w, 0x220)->node_count();
                n4 = geo(w, 0x220)->elem_count();
                n4 = shl32(n4, 2);
                extra = n4 / 3;
                if (extra * 3 < n4) {
                    extra = extra + 1;
                }
                n = n + extra;
                break;
            default:
                rec[14] = 0.0;
                // str 140535d70
                rt::crt::printf(
                    "ERROR in LLWINDEF_DATA::WriteToDBArrays(): not implemented WindefShape type\n");
                break;
            case 10:
            case 11:
            case 12:
                fld<std::int32_t>(w, 0x378) = 1;
                rec[7] = k1;
                rec[14] = k11;
                n = fld<std::int32_t>(w, 0x178);
                break;
            }
            rec[1] = static_cast<double>(*pos);
            rec[2] = static_cast<double>(*pos + -1 + n);
            if (fld<std::int32_t>(w, 0x210) == 3) {
                const std::int32_t nn = geo(w, 0x220)->node_count();
                const double* xyz = ext::f140006410(geo(w, 0x220));
                std::int32_t cnt = nn * 3;
                std::memcpy(llwrz + static_cast<std::int32_t>(rec[1] - k1) * 3, xyz,
                            static_cast<std::size_t>(static_cast<std::int64_t>(cnt) << 3));
                const std::int32_t ne = geo(w, 0x220)->elem_count();
                const std::int32_t* el = ext::f140005510(geo(w, 0x220));
                cnt = shl32(ne, 2);
                for (std::int32_t i = 0; i < cnt; i = i + 1) {
                    // 140261626: (rec[1] + (double)nn) - 1.0, truncated, * 3 + i
                    llwrz[static_cast<std::int32_t>((rec[1] + static_cast<double>(nn)) - k1) * 3 + i] =
                        static_cast<double>(el[i]);
                }
            }
            else if (fld<std::int32_t>(w, 0x210) == 1 || fld<std::int32_t>(w, 0x210) == 2) {
                if (fld<std::int32_t>(w, 0x210) == 2) {
                    // 140261698: row rec[1] + 1 (overwritten by the copy below when it is longer)
                    llwrz[static_cast<std::int32_t>(rec[1] + k1) * 3] = fld<double>(w, 0x1b8);
                    llwrz[static_cast<std::int32_t>(rec[1] + k1) * 3 + 1] = fld<double>(w, 0x1c0);
                }
                std::memcpy(llwrz + static_cast<std::int32_t>(rec[1] - k1) * 3, fld<void*>(w, 0x180),
                            static_cast<std::size_t>(
                                static_cast<std::int64_t>(fld<std::int32_t>(w, 0x178) * 3) << 3));
            }
            else if (fld<std::int32_t>(w, 0x210) == 0xb || fld<std::int32_t>(w, 0x210) == 0xc ||
                     fld<std::int32_t>(w, 0x210) == 0xd) {
                std::memcpy(llwrz + static_cast<std::int32_t>(rec[1] - k1) * 3, fld<void*>(w, 0x180),
                            static_cast<std::size_t>(
                                static_cast<std::int64_t>(fld<std::int32_t>(w, 0x178) * 3) << 3));
            }
            *pos = *pos + n;
            rec[3] = fld<double>(w, 0x350);
            rec[4] = fld<double>(w, 0x358);
            rec[5] = fld<double>(w, 0x360);
            if (fld<std::int32_t>(w, 0x368) == 0) {
                rec[15] = k1;
            }
            else if (fld<std::int32_t>(w, 0x368) == 2) {
                const std::int32_t a = crt_abs(fld<std::int32_t>(w, 0x36c));
                rec[15] = static_cast<double>(neg32(a));
            }
        }
        else {
            rec[1] = static_cast<double>(*pos);
            rec[2] = static_cast<double>(*pos + -1 + fld<std::int32_t>(w, 0x178));
            std::memcpy(llwrz + shl32(static_cast<std::int32_t>(rec[1] - k1), 1), fld<void*>(w, 0x180),
                        static_cast<std::size_t>(
                            static_cast<std::int64_t>(shl32(fld<std::int32_t>(w, 0x178), 1)) << 3));
            *pos = *pos + fld<std::int32_t>(w, 0x178);
            rec[4] = fld<double>(w, 0x350);
            rec[5] = fld<double>(w, 0x358);
            if (fld<std::int32_t>(w, 0x368) == 0) {
                rec[3] = k1;
            }
            else if (fld<std::int32_t>(w, 0x368) == 2) {
                const std::int32_t a = crt_abs(fld<std::int32_t>(w, 0x36c));
                rec[3] = static_cast<double>(neg32(a));
                if (fld<std::int32_t>(w, 0x174) == 1 && fld<std::int32_t>(w, 0x36c) == 0) {
                    rec[3] = static_cast<double>(neg32(code2d));
                }
            }
        }
    }
    switch (fld<std::int32_t>(w, 0x174) - 1) {
    case 0:
        windef_store_fric(w, rec, iifnc, llfnc, m, obj_no);
        break;
    case 1:
        rec[6] = fld<double>(w, 0x370);
        break;
    case 2:
        windef_store_380(w, rec, iifnc, llfnc, m, obj_no);
        if (fld<std::int32_t>(w, 0x4ec) == 0) {
            rec[16] = 0.0;
            rec[17] = fld<double>(w, 0x4f0);
        }
        else {
            if (fld<std::int32_t>(w, 0x4ec) == 1 && fld<std::int32_t>(w, 0x4f8) == 1) {
                rec[16] = k1;
            }
            store_function(m, ext::f140130290(at(w, 0x4e8)), iifnc, llfnc, obj_no, rec + 17,
                           dyn(w, 0x4e8));
        }
        break;
    case 3:
        windef_store_5e0(w, rec, iifnc, llfnc, m, obj_no);
        break;
    case 4:
        rec[1] = static_cast<double>(fld<std::int32_t>(w, 0x838));
        rec[2] = 0.0;
        rec[3] = k1;
        rec[4] = 0.0;
        rec[5] = 0.0;
        windef_store_380(w, rec, iifnc, llfnc, m, obj_no);
        if (fld<std::int32_t>(w, 0x4ec) == 0) {
            rec[14] = 0.0;
            rec[15] = fld<double>(w, 0x4f0);
        }
        else {
            rec[14] = k1;
            store_function(m, ext::f140130200(at(w, 0x4e8)), iifnc, llfnc, obj_no, rec + 15,
                           dyn(w, 0x4e8));
        }
        if (fld<char>(w, 0x560) == '\0') {
            rec[16] = 0.0;
        }
        else if (fld<std::int32_t>(w, 0x56c) == 0) {
            rec[16] = k1;
            rec[17] = fld<double>(w, 0x570);
        }
        else {
            if (fld<std::int32_t>(w, 0x56c) == 1) {
                const std::int32_t v = fld<std::int32_t>(w, 0x578);
                if (v == 1) {
                    rec[16] = k2;
                }
                else if (v == 4) {
                    rec[16] = k3;
                }
            }
            store_function(m, ext::f140130290(at(w, 0x568)), iifnc, llfnc, obj_no, rec + 17,
                           dyn(w, 0x568));
        }
        break;
    case 5:
        rec[1] = static_cast<double>(fld<std::int32_t>(w, 0x838));
        rec[2] = 0.0;
        rec[3] = k1;
        rec[4] = 0.0;
        rec[5] = 0.0;
        windef_store_5e0(w, rec, iifnc, llfnc, m, obj_no);
        if (fld<std::int32_t>(w, 0x6d4) == 0) {
            rec[12] = 0.0;
            rec[13] = fld<double>(w, 0x6d8);
        }
        else {
            rec[12] = k1;
            store_function(m, ext::f140130200(at(w, 0x6d0)), iifnc, llfnc, obj_no, rec + 13,
                           dyn(w, 0x6d0));
        }
        break;
    case 6:
        rec[1] = static_cast<double>(fld<std::int32_t>(w, 0x838));
        rec[2] = 0.0;
        rec[3] = k1;
        rec[4] = 0.0;
        rec[5] = 0.0;
        windef_store_748(w, rec, iifnc, llfnc, m, obj_no);
        windef_store_fric(w, rec, iifnc, llfnc, m, obj_no);
        break;
    case 7:
        windef_store_748(w, rec, iifnc, llfnc, m, obj_no);
        if (fld<std::int32_t>(w, 0x7c4) == 0) {
            rec[8] = 0.0;
            rec[9] = fld<double>(w, 0x7c8);
        }
        else {
            rec[8] = k1;
            store_function(m, ext::f140130200(at(w, 0x7c0)), iifnc, llfnc, obj_no, rec + 9,
                           dyn(w, 0x7c0));
        }
        break;
    case 9:
        break;
    default: // jump table entry 8 (type 9) also lands here
        // str 140535dc0
        rt::crt::printf("ERROR in LLWINDEF_DATA::WriteToDBArrays(): not implemented windef type\n");
        break;
    }
    *row = *row + 1;
}

// ================================================================================================
// Sub-object writers
// ================================================================================================

namespace {

template <class Count, class Take>
void write_window_list(void* list, Count count, Take take, double* llobj, std::ptrdiff_t off_first,
                       std::ptrdiff_t off_last, std::int32_t* iifnc, double* llfnc, double* llwin,
                       std::int32_t win_w, double* llwrz, DB_STEP_DATA* m,
                       const std::int32_t* obj_no)
{
    std::int32_t row = window_row(m);
    std::int32_t pos = window_pos(m);
    const std::int32_t first = row;
    for (std::int32_t k = 0; k < count(); k = k + 1) {
        void* w = ext::f140269900(list, k + 1);
        if (take(w)) {
            windef_write_to_db_arrays(w, llwin + win_w * (row + -1), llwrz, iifnc, llfnc, &row,
                                      &pos, m, obj_no, 0);
        }
    }
    if (0 < row - first) {
        fld<double>(llobj, off_first) = static_cast<double>(first);
        fld<double>(llobj, off_last) = static_cast<double>(row + -1);
    }
    ext::f14019a5c0(m, &row);
    ext::f14019a600(m, &pos);
}

} // namespace

// @src 140187a60  Cls_1401860a0 (obj+0x1ea0) to LLOBJ: the curve at +0x140 (kind code in
//                 word 0x40), the curve at +0xc0 (words 0x43/0x44), the record lists at +0x200
//                 (12-double functions) and +0x218 (20-double functions) with their first / last
//                 function numbers in IIOBJ, and the curve at +0x238 when it has 8 components
void write_obj_1ea0(void* s, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                    double* llfnc, DB_STEP_DATA* m, const std::int32_t* obj_no)
{
    llobj[0] = fld<double>(s, 0x18);
    llobj[1] = fld<double>(s, 0x8);
    llobj[2] = fld<double>(s, 0x0);
    llobj[3] = fld<double>(s, 0x10);
    llobj[0x34] = static_cast<double>(fld<std::int32_t>(s, 0x9c));
    llobj[0x35] = static_cast<double>(fld<std::int32_t>(s, 0x94));
    llobj[0x36] = static_cast<double>(fld<std::int32_t>(s, 0x98));
    llobj[0x42] = static_cast<double>(fld<std::int32_t>(s, 0x78));
    llobj[0x38] = static_cast<double>(fld<std::int32_t>(s, 0xa8));
    llobj[0x39] = static_cast<double>(fld<std::int32_t>(s, 0xa4));
    llobj[0x3a] = fld<double>(s, 0xb0);
    llobj[0x3b] = fld<double>(s, 0xb8);
    const std::int32_t kind = fld<std::int32_t>(s, 0x138);
    if (kind == 0) {
        llobj[0x40] = 0.0;
    }
    else if (kind == 1) {
        llobj[0x40] = k10;
    }
    else if (kind == 2) {
        llobj[0x40] = k20;
    }
    if (fld<std::int32_t>(s, 0x144) == 0) {
        llobj[0x41] = fld<double>(s, 0x148);
    }
    else {
        llobj[0x40] = llobj[0x40] + k1;
        store_function(m, ext::f140130290(at(s, 0x140)), iifnc, llfnc, obj_no, llobj + 0x41,
                       dyn(s, 0x140));
    }
    if (fld<std::int32_t>(s, 0xc4) == 0) {
        llobj[0x43] = 0.0;
        llobj[0x44] = fld<double>(s, 0xc8);
    }
    else {
        if (fld<std::int32_t>(s, 0xd0) == 1) {
            llobj[0x43] = k1;
        }
        else if (fld<std::int32_t>(s, 0xd0) == 0x1c) {
            llobj[0x43] = k2;
        }
        else {
            llobj[0x43] = k3;
        }
        store_function(m, ext::f140130290(at(s, 0xc0)), iifnc, llfnc, obj_no, llobj + 0x44,
                       dyn(s, 0xc0));
    }
    llobj[0x45] = fld<double>(s, 0x58);
    llobj[0x46] = fld<double>(s, 0x60);
    if (fld<std::int32_t>(s, 0x4c) == 3) {
        llobj[0x48] = static_cast<double>(neg32(fld<std::int32_t>(s, 0x50)));
    }
    else {
        llobj[0x48] = static_cast<double>(fld<std::int32_t>(s, 0x4c));
    }
    llobj[0x4a] = static_cast<double>(fld<std::int32_t>(s, 0x68));
    llobj[0x47] = fld<double>(s, 0x70);
    llobj[0x49] = static_cast<double>(fld<std::int32_t>(s, 0x2b0));
    iiobj[0x38 / 4] = fld<std::int32_t>(s, 0xa0);
    if (!rt::dimension_is_2(m)) {
        llobj[0x2a] = static_cast<double>(fld<std::int32_t>(s, 0x28));
        if (fld<std::int32_t>(s, 0x28) == 1) {
            llobj[0x2b] = fld<double>(s, 0x30);
            llobj[0x2c] = fld<double>(s, 0x38);
            llobj[0x2d] = fld<double>(s, 0x40);
        }
        else if (fld<std::int32_t>(s, 0x28) == 2) {
            llobj[0x2b] = static_cast<double>(fld<std::int32_t>(s, 0x48));
            llobj[0x2c] = 0.0;
            llobj[0x2d] = 0.0;
        }
        std::int32_t nv = fld<std::int32_t>(s, 0x1f8);
        if (nv < 0) {
            nv = 0;
            // 140188188: ucomisd, JP/JZ: cleared when != 0.0 or NaN (-0.0 is kept)
            if (fld<double>(s, 0x1f0) != k0) {
                fld<double>(s, 0x1f0) = 0.0;
            }
        }
        llobj[0x2e] = static_cast<double>(nv);
        llobj[0x2f] = fld<double>(s, 0x1f0);
        iiobj[0x28 / 4] = fld<std::int32_t>(s, 0x2b4);
        const std::int32_t n200 = ext::f14019a530(s);
        if (n200 < 1) {
            iiobj[0x4c / 4] = 0;
            iiobj[0x50 / 4] = 0;
        }
        else {
            double number = 0.0;
            for (std::int32_t k = 1; k <= n200; k = k + 1) {
                void* r = ext::f140259b60(at(s, 0x200), k);
                std::int32_t pos = 0;
                std::int32_t len = 0xc;
                alloc_function(m, &len, iifnc, &k, &number, &pos);
                record_e8_to_values(r, llfnc + (pos - 1));
                if (k == 1) {
                    iiobj[0x4c / 4] = static_cast<std::int32_t>(number);
                }
                if (k == n200) {
                    iiobj[0x50 / 4] = static_cast<std::int32_t>(number);
                }
            }
        }
        const std::int32_t n218 = ext::f14019a560(s);
        if (n218 < 1) {
            iiobj[0x54 / 4] = 0;
            iiobj[0x58 / 4] = 0;
        }
        else {
            double number = 0.0;
            for (std::int32_t k = 1; k <= n218; k = k + 1) {
                auto* d = static_cast<model::SfLinkedData*>(ext::f14025a9e0(at(s, 0x218), k));
                std::int32_t pos = 0;
                std::int32_t len = 0x14;
                alloc_function(m, &len, iifnc, &k, &number, &pos);
                d->to_values(llfnc + (pos - 1)); // vtable slot 3 (+0x18)
                if (k == 1) {
                    iiobj[0x54 / 4] = static_cast<std::int32_t>(number);
                }
                if (k == n218) {
                    iiobj[0x58 / 4] = static_cast<std::int32_t>(number);
                }
            }
        }
        llobj[0x51] = static_cast<double>(fld<std::int32_t>(s, 0x7c));
    }
    else {
        iiobj[0x28 / 4] = 0;
        iiobj[0x2c / 4] = 0;
        iiobj[0x30 / 4] = ext::f1401f3c50(at(m, 0x750));
        iiobj[0x34 / 4] = 0;
        llobj[0x11] = static_cast<double>(fld<std::int32_t>(s, 0x80));
        llobj[0x12] = static_cast<double>(fld<std::int32_t>(s, 0x90));
        llobj[0x13] = fld<double>(s, 0x88);
        llobj[0x37] = static_cast<double>(fld<std::int32_t>(s, 0x28));
        if (fld<std::int32_t>(s, 0x28) == 1) {
            llobj[0x32] = fld<double>(s, 0x30);
            llobj[0x33] = fld<double>(s, 0x38);
        }
        else if (fld<std::int32_t>(s, 0x28) == 2) {
            llobj[0x32] = static_cast<double>(fld<std::int32_t>(s, 0x48));
            llobj[0x33] = 0.0;
        }
        llobj[0x53] = static_cast<double>(fld<std::int32_t>(s, 0x7c));
        std::int32_t nv = fld<std::int32_t>(s, 0x1f8);
        if (nv < 0) {
            nv = 0;
            // 140188567: as above
            if (fld<double>(s, 0x1f0) != k0) {
                fld<double>(s, 0x1f0) = 0.0;
            }
        }
        llobj[0x54] = static_cast<double>(nv);
        llobj[0x55] = fld<double>(s, 0x1f0);
        const std::int32_t n200 = ext::f14019a530(s);
        const std::int32_t n56 = ext::f140186d00(s);
        if (n56 < 1 || n56 != n200) {
            iiobj[0x3c / 4] = 0;
            iiobj[0x40 / 4] = 0;
        }
        else {
            double number = 0.0;
            for (std::int32_t k = 1; k <= n200; k = k + 1) {
                void* r = ext::f140259b60(at(s, 0x200), k);
                std::int32_t pos = 0;
                std::int32_t len = 0xc;
                alloc_function(m, &len, iifnc, &k, &number, &pos);
                record_e8_to_values(r, llfnc + (pos - 1));
                if (k == 1) {
                    iiobj[0x3c / 4] = static_cast<std::int32_t>(number);
                }
                if (k == n200) {
                    iiobj[0x40 / 4] = static_cast<std::int32_t>(number);
                }
            }
        }
        const std::int32_t n218 = ext::f14019a560(s);
        if (n218 < 1) {
            iiobj[0x44 / 4] = 0;
            iiobj[0x48 / 4] = 0;
        }
        else {
            double number = 0.0;
            for (std::int32_t k = 1; k <= n218; k = k + 1) {
                auto* d = static_cast<model::SfLinkedData*>(ext::f14025a9e0(at(s, 0x218), k));
                std::int32_t pos = 0;
                std::int32_t len = 0x14;
                alloc_function(m, &len, iifnc, &k, &number, &pos);
                d->to_values(llfnc + (pos - 1)); // vtable slot 3 (+0x18)
                if (k == 1) {
                    iiobj[0x44 / 4] = static_cast<std::int32_t>(number);
                }
                if (k == n218) {
                    iiobj[0x48 / 4] = static_cast<std::int32_t>(number);
                }
            }
        }
    }
    if (fld<std::int32_t>(s, 0x278) == 8) {
        llobj[0x56] = static_cast<double>(fld<std::int32_t>(s, 0x278));
        store_function(m, fld<std::int32_t>(s, 0x278), iifnc, llfnc, obj_no, llobj + 0x57,
                       dyn(s, 0x238));
    }
    else {
        llobj[0x56] = 0.0;
        llobj[0x57] = 0.0;
    }
}

// @src 14018a760  Cls_140189440 (obj+0x2d10) to LLOBJ; with *flag the windows of the list at
//                 +0x40 whose +0x370 > 0 into LLWIN (rows: words 200/0xd0 in 3D, 0x160/0x168 in
//                 2D, byte offsets). iiobj is not used.
void write_obj_2d10(void* s, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                    double* llfnc, double* llwin, std::int32_t win_w, double* llwrz,
                    const char* flag, DB_STEP_DATA* m, const std::int32_t* obj_no)
{
    (void)iiobj;
    fld<double>(llobj, 0xa0) = fld<double>(s, 0xb8);
    fld<double>(llobj, 0xa8) = fld<double>(s, 0xc0);
    fld<double>(llobj, 0xb0) = fld<double>(s, 0xd0);
    fld<double>(llobj, 0xb8) = fld<double>(s, 200);
    // 14018aa5b: comisd, JBE: only windows with +0x370 > 0 (not NaN)
    auto positive = [](void* w) { return k0 < fld<double>(w, 0x370); };
    auto count = [s]() { return ext::f140189580(s); };
    if (!rt::dimension_is_2(m)) {
        fld<double>(llobj, 0xc0) = fld<double>(s, 0x30);
        fld<double>(llobj, 0xe0) = fld<double>(s, 0x88);
        fld<double>(llobj, 0xe8) = static_cast<double>(fld<std::int32_t>(s, 0x90));
        fld<double>(llobj, 0xf8) = fld<double>(s, 0x10);
        fld<double>(llobj, 0x100) = fld<double>(s, 0x18);
        fld<double>(llobj, 0x108) = fld<double>(s, 0x20);
        fld<double>(llobj, 0x110) = fld<double>(s, 0x28);
        fld<double>(llobj, 0x118) = fld<double>(s, 0x38);
        fld<double>(llobj, 0x128) = static_cast<double>(fld<std::int32_t>(s, 0x74));
        fld<double>(llobj, 0x130) = static_cast<double>(fld<std::int32_t>(s, 0x78));
        fld<double>(llobj, 0x138) = static_cast<double>(fld<std::int32_t>(s, 0x7c));
        fld<double>(llobj, 0x1b8) = static_cast<double>(fld<std::int32_t>(s, 0x80));
        fld<double>(llobj, 0xd8) = static_cast<double>(fld<std::int32_t>(s, 0x84));
        fld<double>(llobj, 0x140) = static_cast<double>(fld<std::int32_t>(s, 0xb0));
        fld<double>(llobj, 0x148) = static_cast<double>(fld<std::int32_t>(s, 0xb4));
        fld<double>(llobj, 400) = fld<double>(s, 0xa0);
        fld<double>(llobj, 0x198) = fld<double>(s, 0xa8);
        fld<double>(llobj, 200) = 0.0;
        fld<double>(llobj, 0xd0) = 0.0;
        if (*flag != '\0' && ext::f140189580(s) != 0) {
            write_window_list(at(s, 0x40), count, positive, llobj, 200, 0xd0, iifnc, llfnc, llwin,
                              win_w, llwrz, m, obj_no);
        }
    }
    else {
        fld<double>(llobj, 0xf0) = static_cast<double>(fld<std::int32_t>(s, 0x70));
        fld<double>(llobj, 0x100) = fld<double>(s, 0x10);
        fld<double>(llobj, 0x110) = fld<double>(s, 0x18);
        fld<double>(llobj, 0x118) = fld<double>(s, 0x20);
        fld<double>(llobj, 0x120) = fld<double>(s, 0x28);
        fld<double>(llobj, 0x128) = fld<double>(s, 0x30);
        fld<double>(llobj, 0xf8) = static_cast<double>(fld<std::int32_t>(s, 0x74));
        fld<double>(llobj, 0x108) = fld<double>(s, 0x38);
        fld<double>(llobj, 0x140) = static_cast<double>(fld<std::int32_t>(s, 0xb0));
        fld<double>(llobj, 0x148) = static_cast<double>(fld<std::int32_t>(s, 0xb4));
        fld<double>(llobj, 0x150) = fld<double>(s, 0xa0);
        fld<double>(llobj, 0x158) = fld<double>(s, 0xa8);
        fld<double>(llobj, 200) = 0.0;
        fld<double>(llobj, 0xd0) = 0.0;
        if (*flag != '\0' && ext::f140189580(s) != 0) {
            write_window_list(at(s, 0x40), count, positive, llobj, 0x160, 0x168, iifnc, llfnc,
                              llwin, win_w, llwrz, m, obj_no);
        }
    }
}

// @src 140196840  Cls_1401956d0 (obj+0x2e68) BC window lists to LLWIN; the dimension comes from
//                 the model at s + 0x198. Lists (count, flag, LLOBJ first/last byte offsets):
//                 +0x48 (f3; 2D 0x288/0x290, 3D 0x290/0x298), +0x78 (f1; 0x1e0), +0xa8 (f1;
//                 0x1f0), +0x168 (always; 0x2c0), +0xd8 (f1; 0x258), +0x108 (f1; 0x268), +0x138
//                 (f2, windows with a type (+0x174) only; 0x278)
void write_obj_2e68(void* s, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                    double* llfnc, double* llwin, std::int32_t win_w, double* llwrz,
                    const char* f1, const char* f2, const char* f3, DB_STEP_DATA* m,
                    const std::int32_t* obj_no)
{
    (void)iiobj;
    auto all = [](void*) { return true; };
    std::int32_t n;
    if (rt::dimension_is_2(fld<void*>(s, 0x198))) {
        n = ext::f140195b30(s);
        fld<double>(llobj, 0x288) = 0.0;
        fld<double>(llobj, 0x290) = 0.0;
        if (*f3 != '\0' && 0 < n) {
            write_window_list(at(s, 0x48), [n]() { return n; }, all, llobj, 0x288, 0x290, iifnc,
                              llfnc, llwin, win_w, llwrz, m, obj_no);
        }
    }
    if (rt::dimension_is_3(fld<void*>(s, 0x198))) {
        n = ext::f140195b30(s);
        fld<double>(llobj, 0x290) = 0.0;
        fld<double>(llobj, 0x298) = 0.0;
        if (*f3 != '\0' && 0 < n) {
            write_window_list(at(s, 0x48), [n]() { return n; }, all, llobj, 0x290, 0x298, iifnc,
                              llfnc, llwin, win_w, llwrz, m, obj_no);
        }
    }
    n = ext::f140195b60(s);
    fld<double>(llobj, 0x1e0) = 0.0;
    fld<double>(llobj, 0x1e8) = 0.0;
    if (*f1 != '\0' && 0 < n) {
        write_window_list(at(s, 0x78), [n]() { return n; }, all, llobj, 0x1e0, 0x1e8, iifnc, llfnc,
                          llwin, win_w, llwrz, m, obj_no);
    }
    n = ext::f140195b90(s);
    fld<double>(llobj, 0x1f0) = 0.0;
    fld<double>(llobj, 0x1f8) = 0.0;
    if (*f1 != '\0' && 0 < n) {
        write_window_list(at(s, 0xa8), [n]() { return n; }, all, llobj, 0x1f0, 0x1f8, iifnc, llfnc,
                          llwin, win_w, llwrz, m, obj_no);
    }
    n = ext::f140195c50(s);
    fld<double>(llobj, 0x2c0) = 0.0;
    fld<double>(llobj, 0x2c8) = 0.0;
    if (0 < n) {
        write_window_list(at(s, 0x168), [n]() { return n; }, all, llobj, 0x2c0, 0x2c8, iifnc,
                          llfnc, llwin, win_w, llwrz, m, obj_no);
    }
    n = ext::f140195bc0(s);
    fld<double>(llobj, 600) = 0.0;
    fld<double>(llobj, 0x260) = 0.0;
    if (*f1 != '\0' && 0 < n) {
        write_window_list(at(s, 0xd8), [n]() { return n; }, all, llobj, 600, 0x260, iifnc, llfnc,
                          llwin, win_w, llwrz, m, obj_no);
    }
    n = ext::f140195bf0(s);
    fld<double>(llobj, 0x268) = 0.0;
    fld<double>(llobj, 0x270) = 0.0;
    if (*f1 != '\0' && 0 < n) {
        write_window_list(at(s, 0x108), [n]() { return n; }, all, llobj, 0x268, 0x270, iifnc,
                          llfnc, llwin, win_w, llwrz, m, obj_no);
    }
    n = ext::f140195c20(s);
    fld<double>(llobj, 0x278) = 0.0;
    fld<double>(llobj, 0x280) = 0.0;
    if (*f2 != '\0' && 0 < n) {
        write_window_list(at(s, 0x138), [n]() { return n; },
                          [](void* w) { return fld<std::int32_t>(w, 0x174) != 0; }, llobj, 0x278,
                          0x280, iifnc, llfnc, llwin, win_w, llwrz, m, obj_no);
    }
}

} // namespace def::dbio::g3d
