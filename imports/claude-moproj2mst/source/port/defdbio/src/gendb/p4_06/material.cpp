// GENDB material writers (packet p4-06): the LLMPR list writer 14029bdf0, the record writer
// 140294ae0 and their helpers. Word meanings: scratch dbmap/material.md; the decompile is the
// reference.
//
// In the decompile of 140294ae0 param_1 is an int*: param_1[k] is the int at byte 4k and
// param_1 + k the object at byte 4k. The helpers I/D/F below take that same index k, so each line
// can be compared with the decompile directly (byte offset = 4k).
#include <defdbio/gendb/p4_06_material.hpp>

#include <cstring>
#include <vector>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/refcount.hpp>

#include <defdbio/gendb/p4_06_simctrl.hpp> // write_func_selfdesc (140131600)

namespace def::dbio::gendb {

using model::Cls_14028a4c0;
using model::Cls_14028b640;
using model::DB_STEP_DATA;
using model::DYNARY;
using model::field;
using model::Material;
using model::MaterialList;

namespace {

// int / double / DYNARY at int index k of the material (byte 4k), as param_1[k] in the decompile
inline std::int32_t& I(Material* m, std::size_t k) { return field<std::int32_t>(m, k * 4); }
inline const double& D(const Material* m, std::size_t k) { return field<double>(m, k * 4); }
inline DYNARY* F(Material* m, std::size_t k) { return &field<DYNARY>(m, k * 4); }

// *(param_1 + 0x882): the model back pointer (Material+0x2208)
inline DB_STEP_DATA* owner(const Material* m) { return field<DB_STEP_DATA*>(m, 0x2208); }

// (double)-v with the 32-bit wraparound of the original's NEG
inline double neg(std::int32_t v) { return static_cast<double>(static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v))); }

// FNCLOC (1401bb3d0) of a material function: allocates *len words, owner *attr, function number
// into *fid; returns the 1-based LLFNC start
inline std::int32_t fncloc(const Material* mat, const std::int32_t* len, std::int32_t* iifnc,
                           const std::int32_t* attr, double* fid)
{
    std::int32_t start = 0;
    ext::f1401bb3d0(owner(mat), len, iifnc, attr, fid, &start);
    return start;
}

// (v & 1 ^ v >> 31) - (v >> 31) == 1: v odd and positive, then rounded up to even
inline std::int32_t round_up_even(std::int32_t v) { return v % 2 == 1 ? v + 1 : v; }

} // namespace

// @src 14029b0d0  common property writer P(fn, const, type, n1, n2)
void write_material_property(const Material* mat, double* rec, std::int32_t* iifnc, double* llfnc, const DYNARY* f,
                             const std::int32_t* w_fn, const std::int32_t* w_const, const std::int32_t* w_type,
                             const std::int32_t* w_n1, const std::int32_t* w_n2, const std::int32_t* attr)
{
    const std::int32_t nvar = f->ndim;
    if (nvar == 0) {
        rec[*w_fn] = 0.0;
        rec[*w_const] = f->value;
    } else if (nvar == 1) {
        const std::int32_t v = f->var[0];
        if (v == 3) {
            rec[*w_type] = 0.0;
        } else if (v == 9) {
            rec[*w_type] = 1.0; // DAT_140509c90
        } else if (v == 0xc) {
            rec[*w_type] = 2.0; // DAT_140509fb8
        }
    } else if (nvar == 2) {
        rec[*w_type] = 3.0; // DAT_14051f1c8
        rec[*w_n1] = static_cast<double>(f->n[0]);
        rec[*w_n2] = static_cast<double>(f->n[1]);
    }
    if (f->ndim != 0) {
        std::int32_t start = 0;
        const std::int32_t len = ext::f140130290(f);
        ext::f1401bb3d0(owner(mat), &len, iifnc, attr, rec + *w_fn, &start);
        ext::f140131360(f, llfnc + (start - 1));
    }
}

// @src 14028e030  FRCMOD code and reals as six words
bool material_frcmod_words(const Material* mat, double* out)
{
    bool ok = true;
    std::int32_t code = 0;
    double p[5];
    if (!ext::f14028e140(mat, &code, p)) {
        out[0] = 0.0;
        out[1] = 0.0;
        out[2] = 0.0;
        out[3] = 0.0;
        out[4] = 0.0;
        out[5] = 0.0;
        ok = false;
    } else {
        out[0] = static_cast<double>(code);
        out[1] = p[0];
        out[2] = p[1];
        out[3] = p[2];
        out[4] = p[3];
        out[5] = p[4];
    }
    return ok;
}

// @src 14028c340  FRCMOD code 11: nine reals of the FRCMOD block
void material_frcmod_code11_reals(const void* frc, double* out)
{
    out[0] = field<double>(frc, 0x1b0);
    out[1] = field<double>(frc, 0x1b8);
    out[2] = field<double>(frc, 0x1d0);
    out[3] = field<double>(frc, 0x1d8);
    out[4] = field<double>(frc, 0x1e0);
    out[5] = field<double>(frc, 0x1e8);
    out[6] = field<double>(frc, 0x1f0);
    out[7] = field<double>(frc, 0x1c0);
    out[8] = field<double>(frc, 0x1c8);
}

// @src 14028acb0  grain record sub-table: code, n1 / constant, n2, plain layout
void write_grain_subtable(const Cls_14028a4c0* /*rec*/, double* code, double* n1, double* n2, double* out,
                          const DYNARY* f)
{
    std::int32_t c = 0;
    if (f->ndim == 0) {
        *code = 0.0;
        *n1 = f->value;
    } else if (f->ndim == 1) {
        if (f->var[0] == 3) {
            c = 1;
        } else if (f->var[0] == 0x17) {
            c = 2;
        }
        *code = static_cast<double>(c);
        *n1 = static_cast<double>(f->n[0]);
    } else if (f->ndim == 2) {
        if (f->var[0] == 3 && f->var[1] == 0x17) {
            c = 3;
        }
        *code = static_cast<double>(c);
        *n1 = static_cast<double>(f->n[0]);
        *n2 = static_cast<double>(f->n[1]);
    }
    ext::f140131360(f, out);
}

// @src 14028aa70  GRNDAT mesoscale grain record
void write_grain_record(const Cls_14028a4c0* rec, double* out)
{
    out[0] = static_cast<double>(ext::f14029cd20(rec));
    for (std::int32_t i = 0; i < ext::f14029cd20(rec); i = i + 1) {
        out[i + 1] = field<double>(rec, 8 + static_cast<std::size_t>(i) * 8);
    }
    out[0x17] = field<double>(rec, 0x68);
    out[0x18] = static_cast<double>(field<std::int32_t>(rec, 0x70));
    double* data = out + 0x25;
    if (ext::f14029cd30(rec) == 0) {
        const DYNARY* f1 = &field<DYNARY>(rec, 0x78);
        write_grain_subtable(rec, out + 0x19, out + 0x1c, out + 0x1d, data, f1);
        const std::int32_t a = ext::f140130290(f1);
        const DYNARY* f2 = &field<DYNARY>(rec, 0xf0);
        write_grain_subtable(rec, out + 0x1a, out + 0x1f, out + 0x20, data + a, f2);
        const std::int32_t b = ext::f140130290(f2);
        data = data + a + b;
    } else {
        out[0x19] = -1.0; // DAT_140522fc8
        out[0x1a] = -1.0;
    }
    write_grain_subtable(rec, out + 0x1b, out + 0x22, out + 0x23, data, &field<DYNARY>(rec, 0x168));
}

// @src 14028b4a0  GRNDAT mode-2 record list: list function of record function numbers
void write_grain_record_list(const Cls_14028b640::RecordList* list, DB_STEP_DATA* m, const std::int32_t* attr,
                             std::int32_t* iifnc, double* llfnc, double* count_word, double* fn_word)
{
    std::int32_t start = 0;
    std::int32_t len = 0;
    std::int32_t n = model::grndat_record_list_count(list); // 14029cd50
    *count_word = static_cast<double>(n);
    if (0 < n) {
        ext::f1401bb3d0(m, &n, iifnc, attr, fn_word, &start);
        double* ids = llfnc + (start - 1);
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const auto* r = static_cast<const Cls_14028a4c0*>(ext::f14028aff0(list, i + 1));
            if (r != nullptr) {
                len = ext::f14028a650(r);
                ext::f1401bb3d0(m, &len, iifnc, attr, ids + i, &start);
                write_grain_record(r, llfnc + (start - 1));
            }
        }
    }
}

// @src 140294ae0  one LLMPR record
void write_material_record(Material* mat, double* rec, std::int32_t* iifnc, double* llfnc, const std::int32_t* slot,
                           const std::int32_t* slot_table)
{
    // 140294b07: 2D test of the model; true when the material has no model
    bool is_2d = true;
    if (owner(mat) != nullptr) {
        is_2d = def::rt::dimension_is_2(owner(mat)); // 1400015e0
    }
    std::int32_t start = 0;
    std::int32_t len = 0;
    double* const A = llfnc - 1; // A[start] = llfnc[start - 1]

    rec[0] = static_cast<double>(I(mat, 0));
    rec[1] = D(mat, 0x7ba);
    def::rt::to_fortran(reinterpret_cast<char*>(rec + 2), reinterpret_cast<const char*>(mat) + 0xc, 0x40);
    rec[10] = D(mat, 0x7b2);
    rec[0x2c] = D(mat, 0x7b4);
    rec[0x2b] = D(mat, 0x7b8);
    rec[0x2a] = D(mat, 0x7b6);

    // FPERV first table (+0x1ef0)
    if (I(mat, 0x7bd) == 0) {
        rec[0x32] = 0.0;
        rec[0x31] = D(mat, 0x7be);
    } else {
        if (I(mat, 0x7bd) == 1 && I(mat, 0x7c0) == 1) {
            rec[0x32] = 1.0;
        }
        len = ext::f140130290(F(mat, 0x7bc));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x31);
        ext::f140131360(F(mat, 0x7bc), A + start);
    }
    // FPERV second table (+0x1f68)
    if (I(mat, 0x7db) == 0) {
        rec[0x94] = 0.0;
        rec[0x93] = D(mat, 0x7dc);
    } else {
        if (I(mat, 0x7db) == 1 && I(mat, 0x7de) == 1) {
            rec[0x94] = 1.0;
        }
        len = ext::f140130290(F(mat, 0x7da));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x93);
        ext::f140131360(F(mat, 0x7da), A + start);
    }

    // FRCMOD
    double frc[6];
    material_frcmod_words(mat, frc);
    rec[0x46] = frc[0];
    rec[0x47] = frc[2];
    rec[0x48] = frc[3];
    rec[0x49] = frc[4];
    rec[0x4a] = frc[1];
    rec[0x95] = frc[5];
    if (frc[0] == 11.0) { // DAT_140522fe0
        double fid = 0.0;
        len = 9;
        start = fncloc(mat, &len, iifnc, slot, &fid);
        material_frcmod_code11_reals(F(mat, 0x6b4), A + start);
        rec[0x47] = 9.0;                                      // DAT_140522fe8
        rec[0x48] = static_cast<double>(static_cast<std::int32_t>(fid)); // cvttsd2si
    }
    if (frc[0] == 12.0) { // DAT_140524cf8
        len = ext::f140130200(F(mat, 0x6e4));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x47);
        ext::f140131360(F(mat, 0x6e4), A + start);
    }
    // UTSDAT (+0x1b10)
    if (I(mat, 0x6c5) == 0) {
        rec[0x6b] = 0.0;
        rec[0x6c] = D(mat, 0x6c6);
    } else if (I(mat, 0x6c5) == 1) {
        rec[0x6b] = 1.0;
        len = ext::f140130200(F(mat, 0x6c4));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x6c);
        ext::f140131360(F(mat, 0x6c4), A + start);
    }
    if (frc[0] == 17.0) { // DAT_14053a7b8
        len = ext::f140130290(F(mat, 0x702));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x48);
        ext::f140131360(F(mat, 0x702), A + start);
    }
    // HDNRUL
    if (I(mat, 0x19a) == 0) {
        rec[0x68] = 0.0;
    } else {
        rec[0x68] = 1.0;
    }

    // FSTRES: switch on the internal type + 1
    switch (I(mat, 0x14) + 1) {
    case 0:
        rec[0xb] = 0.0;
        break;
    case 1:
        rec[0xb] = 1.0;
        rec[0xc] = D(mat, 0x16);
        rec[0xd] = D(mat, 0x18);
        rec[0xe] = D(mat, 0x1a);
        rec[0xf] = D(mat, 0x1c);
        break;
    case 2:
        if (I(mat, 0x32) == 1) {
            rec[0xb] = 2.0;
        } else {
            rec[0xb] = 3.0;
        }
        rec[0xc] = static_cast<double>(I(mat, 0x25));
        rec[0xd] = static_cast<double>(I(mat, 0x26));
        rec[0xe] = static_cast<double>(I(mat, 0x27));
        len = ext::f14012fff0(F(mat, 0x1e));
        start = fncloc(mat, &len, iifnc, slot, rec + 0xf);
        ext::f140131360(F(mat, 0x1e), A + start);
        break;
    case 3:
        rec[0xb] = 4.0;
        rec[0xc] = D(mat, 0x3c);
        rec[0xd] = D(mat, 0x3e);
        rec[0xe] = D(mat, 0x40);
        rec[0xf] = D(mat, 0x42);
        rec[0x10] = D(mat, 0x44);
        break;
    case 4:
        rec[0xb] = 5.0;
        rec[0xc] = D(mat, 0x46);
        rec[0xd] = D(mat, 0x48);
        rec[0xe] = D(mat, 0x4a);
        rec[0xf] = D(mat, 0x4c);
        break;
    case 5:
        rec[0xb] = 6.0;
        // table +0x1b0
        if (I(mat, 0x6d) == 0) {
            rec[0xc] = 0.0;
            rec[0xe] = D(mat, 0x6e);
        } else if (I(mat, 0x6d) == 1) {
            if (I(mat, 0x70) == 3) {
                rec[0xc] = 1.0;
                rec[0xe] = static_cast<double>(I(mat, 0x73));
            } else {
                rec[0xc] = 2.0;
                rec[0xe] = static_cast<double>(I(mat, 0x73));
            }
        } else if (I(mat, 0x6d) == 2) {
            rec[0xc] = 3.0;
            rec[0xe] = static_cast<double>(I(mat, 0x73));
            rec[0xf] = static_cast<double>(I(mat, 0x74));
        }
        if (I(mat, 0x6d) != 0) {
            len = ext::f140130290(F(mat, 0x6c));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x12);
            ext::f140131360(F(mat, 0x6c), A + start);
        }
        // table +0x138
        if (I(mat, 0x4f) == 0) {
            rec[0xd] = 0.0;
            rec[0x10] = D(mat, 0x50);
        } else if (I(mat, 0x4f) == 1) {
            if (I(mat, 0x52) == 3) {
                rec[0xd] = 1.0;
                rec[0x10] = static_cast<double>(I(mat, 0x55));
            } else {
                rec[0xd] = 2.0;
                rec[0x10] = static_cast<double>(I(mat, 0x55));
            }
        } else if (I(mat, 0x4f) == 2) {
            rec[0xd] = 3.0;
            rec[0x10] = static_cast<double>(I(mat, 0x55));
            rec[0x11] = static_cast<double>(I(mat, 0x56));
        }
        if (I(mat, 0x4f) != 0) {
            len = ext::f140130290(F(mat, 0x4e));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x13);
            ext::f140131360(F(mat, 0x4e), A + start);
        }
        break;
    case 6:
        if (I(mat, 0x9e) == 1) {
            rec[0xb] = 7.0;
        } else {
            rec[0xb] = 8.0;
        }
        rec[0xc] = static_cast<double>(I(mat, 0x91));
        rec[0xd] = static_cast<double>(I(mat, 0x92));
        rec[0xe] = static_cast<double>(I(mat, 0x93));
        len = ext::f14012fff0(F(mat, 0x8a));
        start = fncloc(mat, &len, iifnc, slot, rec + 0xf);
        ext::f140131360(F(mat, 0x8a), A + start);
        break;
    case 7:
        rec[0xb] = 9.0;
        rec[0xc] = D(mat, 0xa8);
        rec[0xd] = D(mat, 0xaa);
        rec[0xe] = D(mat, 0xac);
        rec[0xf] = D(mat, 0xae);
        rec[0x10] = D(mat, 0xb0);
        rec[0x11] = D(mat, 0xb2);
        rec[0x12] = D(mat, 0xb4);
        rec[0x13] = D(mat, 0xb6);
        rec[0x14] = D(mat, 0xb8);
        rec[0x15] = D(mat, 0xba);
        rec[0x16] = D(mat, 0xbc);
        rec[0x4e] = D(mat, 0xbe);
        rec[0x4f] = D(mat, 0xc0);
        rec[0x50] = D(mat, 0xc2);
        break;
    case 8:
        rec[0xb] = 10.0;
        rec[0xc] = D(mat, 0xc4);
        rec[0xd] = D(mat, 0xc6);
        rec[0xe] = D(mat, 200);
        rec[0xf] = D(mat, 0xca);
        rec[0x10] = D(mat, 0xcc);
        rec[0x11] = D(mat, 0xce);
        break;
    case 9:
        rec[0xb] = 11.0;
        rec[0xc] = D(mat, 0xd0);
        rec[0xd] = D(mat, 0xd2);
        rec[0xe] = D(mat, 0xd4);
        rec[0xf] = D(mat, 0xd6);
        rec[0x10] = D(mat, 0xd8);
        break;
    case 10:
        rec[0xb] = 12.0;
        if (I(mat, 0x101) == 0) {
            rec[0xc] = 0.0;
            rec[0xe] = D(mat, 0x102);
        } else if (I(mat, 0x101) == 1) {
            if (I(mat, 0x104) == 3) {
                rec[0xc] = 1.0;
                rec[0xe] = static_cast<double>(I(mat, 0x107));
            } else {
                rec[0xc] = 2.0;
                rec[0xe] = static_cast<double>(I(mat, 0x107));
            }
        } else if (I(mat, 0x101) == 2) {
            rec[0xc] = 3.0;
            rec[0xe] = static_cast<double>(I(mat, 0x107));
            rec[0xf] = static_cast<double>(I(mat, 0x108));
        }
        if (I(mat, 0x101) != 0) {
            len = ext::f140130290(F(mat, 0x100));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x12);
            ext::f140131360(F(mat, 0x100), A + start);
        }
        break;
    case 0xb:
    case 0xd:
        // the two cases are identical apart from the code (13 / 15)
        rec[0xb] = I(mat, 0x14) + 1 == 0xb ? 13.0 : 15.0; // DAT_14053a7a8 / DAT_14053a7a0
        I(mat, 0x11e) = I(mat, 0x127);
        rec[0xc] = static_cast<double>(I(mat, 0x11e));
        len = ext::f140130230(F(mat, 0x120));
        if (0 < len) {
            rec[0xc] = static_cast<double>(I(mat, 0x127));
            start = fncloc(mat, &len, iifnc, slot, rec + 0xf);
            ext::f140131360(F(mat, 0x120), A + start);
        }
        len = ext::f140130290(F(mat, 0x13e));
        if (0 < len) {
            rec[0xd] = static_cast<double>(I(mat, 0x14e));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x10);
            ext::f140131360(F(mat, 0x13e), A + start);
        }
        len = ext::f140130290(F(mat, 0x15c));
        if (0 < len) {
            rec[0xe] = static_cast<double>(I(mat, 0x16c));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x11);
            ext::f140131360(F(mat, 0x15c), A + start);
        }
        break;
    case 0xc:
        rec[0xb] = 14.0;
        rec[0xc] = D(mat, 0xda);
        rec[0xd] = D(mat, 0xdc);
        rec[0xe] = D(mat, 0xde);
        rec[0xf] = D(mat, 0xe0);
        if (I(mat, 0xe3) == 0) {
            rec[0x10] = 0.0;
            rec[0x11] = D(mat, 0xe4);
        } else {
            len = ext::f140130380(F(mat, 0xe2));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x10);
            write_func_selfdesc(F(mat, 0xe2), A + start); // 140131600
        }
        break;
    case 0xe:
        rec[0xb] = 16.0;
        rec[0xc] = static_cast<double>(I(mat, 0x181));
        rec[0xd] = static_cast<double>(I(mat, 0x182));
        rec[0xe] = static_cast<double>(I(mat, 0x183));
        rec[0xf] = static_cast<double>(I(mat, 0x18a));
        len = ext::f140130090(F(mat, 0x17a));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x10);
        ext::f140131360(F(mat, 0x17a), A + start);
        break;
    case 0xf:
        rec[0xb] = neg(I(mat, 0x198));
        break;
    default:
        rec[0xb] = 3.0; // DAT_14051f1c8
    }

    // COARSE: a 20-value header function (kind 0xad) at w65
    if (I(mat, 0x834) == 0) {
        rec[0x41] = 0.0;
    } else {
        DYNARY hdr;                              // 14012d050
        model::dynary_reinit_keep(&hdr, 0xad);   // 14012fc70
        hdr.ncomp = 0x14;
        model::dynary_alloc(&hdr);               // 14012fad0
        if (I(mat, 0x837) == 0) {
            hdr.values[0] = static_cast<double>(I(mat, 0x834));
            hdr.values[1] = 0.0;
            hdr.values[2] = 0.0;
            hdr.values[3] = D(mat, 0x838);
        } else {
            double words[5] = {};
            const std::int32_t w_n2 = 4, w_n1 = 3, w_type = 2, w_const = 1, w_fn = 0;
            write_material_property(mat, words, iifnc, llfnc, F(mat, 0x836), &w_fn, &w_const, &w_type, &w_n1, &w_n2,
                                    slot);
            hdr.values[0] = static_cast<double>(I(mat, 0x834));
            hdr.values[1] = words[0];
            hdr.values[2] = 5.0; // DAT_140522f48
            hdr.values[3] = static_cast<double>(I(mat, 0x83d));
            hdr.values[4] = static_cast<double>(I(mat, 0x83e));
        }
        len = ext::f140130290(&hdr);
        if (0 < len) {
            start = fncloc(mat, &len, iifnc, slot, rec + 0x41);
            ext::f140131360(&hdr, A + start);
        }
    } // 14012f410

    // DIFBND (+0x2160), self-describing, always written
    len = ext::f140130380(F(mat, 0x858));
    start = fncloc(mat, &len, iifnc, slot, rec + 0x99);
    write_func_selfdesc(F(mat, 0x858), A + start);

    {   // YOUNG, POISON, EXPAND, THRCND, HEATCP: P(fn, const, type, n1, n2)
        const std::int32_t n2 = 0x19, n1 = 0x18, type = 0x17, cst = 0x20, fn = 0x21;
        write_material_property(mat, rec, iifnc, llfnc, F(mat, 0x19c), &fn, &cst, &type, &n1, &n2, slot);
    }
    {
        const std::int32_t n2 = 0x1c, n1 = 0x1b, type = 0x1a, cst = 0x22, fn = 0x23;
        write_material_property(mat, rec, iifnc, llfnc, F(mat, 0x1ba), &fn, &cst, &type, &n1, &n2, slot);
    }
    {
        const std::int32_t n2 = 0x1f, n1 = 0x1e, type = 0x1d, cst = 0x24, fn = 0x25;
        write_material_property(mat, rec, iifnc, llfnc, F(mat, 0x1d8), &fn, &cst, &type, &n1, &n2, slot);
    }
    rec[0x58] = D(mat, 0x1f6);
    {
        const std::int32_t n2 = 0x55, n1 = 0x54, type = 0x2f, cst = 0x26, fn = 0x27;
        write_material_property(mat, rec, iifnc, llfnc, F(mat, 0x1f8), &fn, &cst, &type, &n1, &n2, slot);
    }
    {
        const std::int32_t n2 = 0x57, n1 = 0x56, type = 0x30, cst = 0x28, fn = 0x29;
        write_material_property(mat, rec, iifnc, llfnc, F(mat, 0x216), &fn, &cst, &type, &n1, &n2, slot);
    }
    rec[99] = static_cast<double>(I(mat, 0x234));
    {   // MASDEN: n1 and n2 both go to word 1 (overwrites MATDEN for a 2-D table)
        const std::int32_t n2 = 1, n1 = 1, type = 0x53, cst = 0x51, fn = 0x52;
        write_material_property(mat, rec, iifnc, llfnc, F(mat, 0x7f8), &fn, &cst, &type, &n1, &n2, slot);
    }

    // EMSVTY (+0x8d8)
    if (I(mat, 0x237) == 0) {
        rec[0x2e] = 0.0;
        rec[0x2d] = D(mat, 0x238);
    } else {
        len = ext::f140130200(F(mat, 0x236));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x2e);
        ext::f140131360(F(mat, 0x236), A + start);
    }
    // HDNPHA (+0xf10)
    if (I(mat, 0x3c5) == 0) {
        rec[0x59] = 0.0;
        rec[0x5a] = D(mat, 0x3c6);
    } else if (I(mat, 0x3c5) == 1) {
        const std::int32_t v = I(mat, 0x3c8);
        if (v == 3) {
            rec[0x59] = 2.0;
        } else if (v == 9) {
            rec[0x59] = 3.0;
        } else if (v == 0xc) {
            rec[0x59] = 1.0;
        }
    } else if (I(mat, 0x3c5) == 2) {
        rec[0x59] = 4.0;
        rec[0x5b] = static_cast<double>(I(mat, 0x3cb));
        rec[0x5c] = static_cast<double>(I(mat, 0x3cc));
    }
    if (I(mat, 0x3c5) != 0) {
        len = ext::f140130290(F(mat, 0x3c4));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x5a);
        ext::f140131360(F(mat, 0x3c4), A + start);
    }
    // HDNTIM (+0xf88), written only when n != 0
    if (I(mat, 0x3e9) != 0) {
        len = ext::f140130200(F(mat, 0x3e2));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x5f);
        ext::f140131360(F(mat, 0x3e2), A + start);
    }
    // JOMINY (+0x1000)
    if (0 < I(mat, 0x407) && 0 < I(mat, 0x408)) {
        rec[0x60] = static_cast<double>(I(mat, 0x407));
        rec[0x61] = static_cast<double>(I(mat, 0x408));
        len = ext::f140130160(F(mat, 0x400));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x62);
        ext::f140131360(F(mat, 0x400), A + start);
    }
    // MSTMTR: list of the component materials' DB slots
    rec[0x5d] = static_cast<double>(I(mat, 0x733));
    if (0 < I(mat, 0x733)) {
        len = I(mat, 0x733);
        start = fncloc(mat, &len, iifnc, slot, rec + 0x5e);
        double* list = A + start;
        const std::int32_t* numbers = field<std::int32_t*>(mat, 0x734 * 4);
        for (std::int32_t i = 0; i < I(mat, 0x733); i = i + 1) {
            list[i] = static_cast<double>(slot_table[numbers[i] - 1]);
        }
    }

    // CREEP (+0xac8)
    std::int32_t prod = 0;
    std::int32_t np1 = 0;
    switch (I(mat, 0x2b2)) {
    case 0:
        rec[0x39] = 0.0;
        break;
    case 1:
    case 2:
    case 3:
    case 4: {
        // 1-D tables +0xbc0 / +0xc38 / +0xcb0 / +0xd28: n * (ncomp + 1), rounded up to even
        const std::int32_t c = I(mat, 0x2b2);
        const std::size_t base = c == 1 ? 0x2f0 : c == 2 ? 0x30e : c == 3 ? 0x32c : 0x34a;
        rec[0x39] = static_cast<double>(c); // 1.0 / 2.0 / 3.0 / 4.0
        rec[0x3a] = static_cast<double>(I(mat, base + 7));
        np1 = I(mat, base + 0x10) + 1;
        prod = I(mat, base + 7) * np1;
        len = round_up_even(prod);
        start = fncloc(mat, &len, iifnc, slot, rec + 0x40);
        ext::f140131360(F(mat, base), A + start);
        break;
    }
    case 5:
        rec[0x39] = 5.0;
        rec[0x3a] = static_cast<double>(I(mat, 0x2d9));
        rec[0x3b] = static_cast<double>(I(mat, 0x2da));
        rec[0x3c] = static_cast<double>(I(mat, 0x2db));
        len = ext::f14012fff0(F(mat, 0x2d2));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x40);
        ext::f140131360(F(mat, 0x2d2), A + start);
        break;
    case 6:
        rec[0x39] = 6.0;
        rec[0x3a] = static_cast<double>(I(mat, 699));
        rec[0x3b] = static_cast<double>(I(mat, 700));
        rec[0x3c] = static_cast<double>(I(mat, 0x2bd));
        len = ext::f14012fff0(F(mat, 0x2b4));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x40);
        ext::f140131360(F(mat, 0x2b4), A + start);
        break;
    case 7:
    case 8:
        rec[0x39] = I(mat, 0x2b2) == 7 ? 7.0 : 8.0; // DAT_140522f38 / DAT_140522f40
        I(mat, 0x369) = I(mat, 0x371);
        rec[0x3a] = static_cast<double>(I(mat, 0x369));
        len = ext::f140130230(F(mat, 0x36a));
        if (0 < len) {
            rec[0x3a] = static_cast<double>(I(mat, 0x371));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x3d);
            ext::f140131360(F(mat, 0x36a), A + start);
        }
        len = ext::f140130290(F(mat, 0x388));
        if (0 < len) {
            rec[0x3b] = static_cast<double>(I(mat, 0x398));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x3e);
            ext::f140131360(F(mat, 0x388), A + start);
        }
        len = ext::f140130290(F(mat, 0x3a6));
        if (0 < len) {
            rec[0x3c] = static_cast<double>(I(mat, 0x3b6));
            start = fncloc(mat, &len, iifnc, slot, rec + 0x3f);
            ext::f140131360(F(mat, 0x3a6), A + start);
        }
        break;
    case 9:
        rec[0x39] = neg(I(mat, 0x368));
        break;
    default:
        break;
    }

    // DIFCOE (+0x950)
    if (I(mat, 0x254) == 0) {
        rec[0x35] = 0.0;
        rec[0x38] = D(mat, 600);
    } else if (I(mat, 0x254) == 1) {
        rec[0x35] = 1.0;
        rec[0x36] = static_cast<double>(I(mat, 0x25d));
        rec[0x37] = static_cast<double>(I(mat, 0x25e));
        len = ext::f140130290(F(mat, 0x256));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x38);
        ext::f140131360(F(mat, 0x256), A + start);
    } else if (I(mat, 0x254) == 2 || I(mat, 0x254) == 3) {
        const bool two = I(mat, 0x254) == 2;
        const std::size_t base = two ? 0x274 : 0x292;
        rec[0x35] = two ? 2.0 : 3.0;
        rec[0x36] = static_cast<double>(I(mat, base + 7));
        rec[0x37] = static_cast<double>(I(mat, base + 0x10) + 1);
        len = round_up_even(static_cast<std::int32_t>(rec[0x36]) * static_cast<std::int32_t>(rec[0x37]));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x38);
        ext::f140131360(F(mat, base), A + start);
    } else if (I(mat, 0x254) == 4) {
        rec[0x35] = neg(I(mat, 0x2b0));
    }

    // GRNDAT (+0x1078): switch on mode + 1
    switch (I(mat, 0x5fa) + 1) {
    case 0:
        rec[0x6d] = neg(I(mat, 0x5fb));
        break;
    case 1:
        rec[0x6d] = 0.0;
        break;
    case 2: {
        // mode 1: nine consecutive 1-D tables in one function
        rec[0x6d] = 1.0;
        len = ext::f14028b990(F(mat, 0x41e));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x82);
        double* p = A + start;
        rec[0x6e] = I(mat, 0x426) == 3 ? 1.0 : 2.0;
        rec[0x6f] = static_cast<double>(I(mat, 0x429));
        ext::f1401317c0(F(mat, 0x422), p);
        p = p + I(mat, 0x429) * (I(mat, 0x432) + 1);
        rec[0x70] = I(mat, 0x444) == 3 ? 1.0 : 2.0;
        rec[0x71] = static_cast<double>(I(mat, 0x447));
        ext::f1401317c0(F(mat, 0x440), p);
        p = p + I(mat, 0x447) * (I(mat, 0x450) + 1);
        if (I(mat, 0x41e) == 0) {
            rec[0x72] = 0.0;
        } else if (I(mat, 0x462) == 3) {
            rec[0x72] = 1.0;
        } else {
            rec[0x72] = 2.0;
        }
        rec[0x73] = static_cast<double>(I(mat, 0x465));
        ext::f1401317c0(F(mat, 0x45e), p);
        p = p + I(mat, 0x465) * (I(mat, 0x46e) + 1);
        if (I(mat, 0x480) == 3) {
            rec[0x74] = 1.0;
        } else if (I(mat, 0x480) == 8) {
            rec[0x74] = 2.0;
        } else {
            rec[0x74] = 3.0;
        }
        rec[0x75] = static_cast<double>(I(mat, 0x483));
        ext::f1401317c0(F(mat, 0x47c), p);
        p = p + I(mat, 0x483) * (I(mat, 0x48c) + 1);
        if (I(mat, 0x41f) == 0) {
            rec[0x76] = 0.0;
        } else if (I(mat, 0x49e) == 3) {
            rec[0x76] = 1.0;
        } else {
            rec[0x76] = 2.0;
        }
        rec[0x77] = static_cast<double>(I(mat, 0x4a1));
        ext::f1401317c0(F(mat, 0x49a), p);
        p = p + I(mat, 0x4a1) * (I(mat, 0x4aa) + 1);
        if (I(mat, 0x4bc) == 3) {
            rec[0x78] = 1.0;
        } else if (I(mat, 0x4bc) == 8) {
            rec[0x78] = 2.0;
        } else {
            rec[0x78] = 3.0;
        }
        rec[0x79] = static_cast<double>(I(mat, 0x4bf));
        ext::f1401317c0(F(mat, 0x4b8), p);
        p = p + I(mat, 0x4bf) * (I(mat, 0x4c8) + 1);
        if (I(mat, 0x420) == 0) {
            rec[0x7a] = 0.0;
        } else if (I(mat, 0x4da) == 3) {
            rec[0x7a] = 1.0;
        } else {
            rec[0x7a] = 2.0;
        }
        rec[0x7b] = static_cast<double>(I(mat, 0x4dd));
        ext::f1401317c0(F(mat, 0x4d6), p);
        p = p + I(mat, 0x4dd) * (I(mat, 0x4e6) + 1);
        if (I(mat, 0x4f8) == 3) {
            rec[0x7c] = 1.0;
        } else if (I(mat, 0x4f8) == 8) {
            rec[0x7c] = 2.0;
        } else {
            rec[0x7c] = 3.0;
        }
        rec[0x7d] = static_cast<double>(I(mat, 0x4fb));
        ext::f1401317c0(F(mat, 0x4f4), p);
        const std::int32_t nc8 = I(mat, 0x504);
        const std::int32_t n8 = I(mat, 0x4fb);
        if (I(mat, 0x421) == 0) {
            rec[0x7e] = 0.0;
        } else if (I(mat, 0x516) == 3) {
            rec[0x7e] = 1.0;
        } else if (I(mat, 0x4f8) == 8) { // 14029912d: the 8th table's variable (+0x13e0), kept
            rec[0x7e] = 2.0;
        } else {
            rec[0x7e] = 3.0;
        }
        rec[0x7f] = static_cast<double>(I(mat, 0x519));
        ext::f1401317c0(F(mat, 0x512), p + n8 * (nc8 + 1));
        rec[0x80] = D(mat, 0x532);
        rec[0x81] = D(mat, 0x530);
        break;
    }
    case 3: {
        // mode 2: mesoscale grain records
        rec[0x6d] = 2.0;
        rec[0x6e] = static_cast<double>(I(mat, 0x534));
        auto* g2 = &field<Cls_14028b640>(mat, 0x534 * 4);
        rec[0x6f] = static_cast<double>(model::grndat_record_count(g2)); // 14028b780
        len = ext::f14028a650(&g2->main);
        start = fncloc(mat, &len, iifnc, slot, rec + 0x70);
        write_grain_record(&g2->main, A + start);
        write_grain_record_list(&g2->l1, owner(mat), slot, iifnc, llfnc, rec + 0x71, rec + 0x72); // +0x16f8
        write_grain_record_list(&g2->l2, owner(mat), slot, iifnc, llfnc, rec + 0x73, rec + 0x74); // +0x16e0
        write_grain_record_list(&g2->l3, owner(mat), slot, iifnc, llfnc, rec + 0x75, rec + 0x76); // +0x16c8
        write_grain_record_list(&g2->l4, owner(mat), slot, iifnc, llfnc, rec + 0x77, rec + 0x78); // +0x1710
        write_grain_record_list(&g2->l5, owner(mat), slot, iifnc, llfnc, rec + 0x79, rec + 0x7a); // +0x1728
        break;
    }
    case 5: {
        // mode 4: 2-D table
        rec[0x6d] = 4.0;
        rec[0x6e] = static_cast<double>(I(mat, 0x5d7));
        rec[0x6f] = static_cast<double>(I(mat, 0x5d8));
        const std::int32_t len4 = ext::f140130290(F(mat, 0x5d0));
        const std::int32_t start4 = fncloc(mat, &len4, iifnc, slot, rec + 0x70);
        ext::f140131360(F(mat, 0x5d0), A + start4);
        break;
    }
    case 6:
        rec[0x6d] = 5.0;
        rec[0x6e] = D(mat, 0x5ee);
        rec[0x6f] = D(mat, 0x5f0);
        rec[0x70] = D(mat, 0x5f2);
        rec[0x71] = D(mat, 0x5f4);
        rec[0x72] = D(mat, 0x5f6);
        rec[0x73] = D(mat, 0x5f8);
        break;
    default:
        break;
    }

    // BURGRS (+0x18f0): 1-D, 2-D (n1, n2 header) or 3-D (n1, n2, n3 header)
    {
        DYNARY* f = F(mat, 0x63c);
        std::int32_t code = -1;
        if (I(mat, 0x63d) == 0) {
            rec[0x8b] = 0.0;
            rec[0x8c] = D(mat, 0x63e);
        } else if (I(mat, 0x63d) == 1) {
            if (I(mat, 0x640) == 3) {
                code = 1;
            } else if (I(mat, 0x640) == 6) {
                code = 2;
            } else if (I(mat, 0x640) == 0x18) {
                code = 3;
            }
            if (0 < code) {
                rec[0x8b] = static_cast<double>(code);
                len = ext::f140130200(f);
                start = fncloc(mat, &len, iifnc, slot, rec + 0x8c);
                ext::f140131360(f, A + start);
            }
        } else if (I(mat, 0x63d) == 2) {
            if (I(mat, 0x640) == 3 && I(mat, 0x641) == 6) {
                code = 4;
            } else if (I(mat, 0x640) == 3 && I(mat, 0x641) == 0x18) {
                code = 5;
            } else if (I(mat, 0x640) == 6 && I(mat, 0x641) == 0x18) {
                code = 6;
            }
            if (0 < code) {
                rec[0x8b] = static_cast<double>(code);
                len = ext::f140130160(f) + 2;
                start = fncloc(mat, &len, iifnc, slot, rec + 0x8c);
                double* p = A + start;
                p[0] = static_cast<double>(f->n[0]);
                p[1] = static_cast<double>(f->n[1]);
                ext::f140131360(f, p + 2);
            }
        } else if (I(mat, 0x63d) == 3) {
            code = 7;
            rec[0x8b] = 7.0;
            len = ext::f14012fff0(f) + 3;
            start = fncloc(mat, &len, iifnc, slot, rec + 0x8c);
            double* p = A + start;
            p[0] = static_cast<double>(f->n[0]);
            p[1] = static_cast<double>(f->n[1]);
            p[2] = static_cast<double>(f->n[2]);
            ext::f140131360(f, p + 3);
        }
    }
    // ALPHA (+0x1968), NDISFM (+0x19e0), RECVRY (+0x1a58): 0 const, 1 var 3, 2 var 8, 3 (3, 8)
    // with an n1, n2 header. The three blocks of the original differ only in the words and in
    // RECVRY setting the code variable to 0 for nvar 0 (not used afterwards).
    const std::size_t dis[3] = {0x65a, 0x678, 0x696};
    const std::int32_t w_code[3] = {0x8d, 0x8f, 0x91};
    for (int k = 0; k < 3; ++k) {
        DYNARY* f = F(mat, dis[k]);
        double* wc = rec + w_code[k];
        double* wf = rec + w_code[k] + 1;
        std::int32_t code = -1;
        if (I(mat, dis[k] + 1) == 0) {
            *wc = 0.0;
            *wf = D(mat, dis[k] + 2);
        } else if (I(mat, dis[k] + 1) == 1) {
            if (I(mat, dis[k] + 4) == 3) {
                code = 1;
            } else if (I(mat, dis[k] + 4) == 8) {
                code = 2;
            }
            if (0 < code) {
                *wc = static_cast<double>(code);
                len = ext::f140130200(f);
                start = fncloc(mat, &len, iifnc, slot, wf);
                ext::f140131360(f, A + start);
            }
        } else if (I(mat, dis[k] + 1) == 2) {
            if (I(mat, dis[k] + 4) == 3 && I(mat, dis[k] + 5) == 8) {
                code = 3;
            }
            if (0 < code) {
                *wc = static_cast<double>(code);
                len = ext::f140130160(f) + 2;
                start = fncloc(mat, &len, iifnc, slot, wf);
                double* p = A + start;
                p[0] = static_cast<double>(f->n[0]);
                p[1] = static_cast<double>(f->n[1]);
                ext::f140131360(f, p + 2);
            }
        }
    }

    // ELRST (+0x1cd8)
    if (I(mat, 0x737) == 0) {
        rec[0x33] = 0.0;
        rec[0x34] = D(mat, 0x738);
    } else {
        rec[0x33] = I(mat, 0x73a) == 3 ? 1.0 : 2.0;
        len = ext::f140130200(F(mat, 0x736));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x34);
        ext::f140131360(F(mat, 0x736), A + start);
    }
    // PMEAB (+0x1d50): n1, n2 at w137, w138 (3D) or w131, w132 (2D)
    if (I(mat, 0x755) == 0) {
        rec[100] = 0.0;
        rec[0x65] = D(mat, 0x756);
    } else {
        if (I(mat, 0x755) == 1) {
            rec[100] = I(mat, 0x758) == 3 ? 1.0 : 2.0;
        } else if (I(mat, 0x755) == 2) {
            rec[100] = 4.0;
            if (!is_2d) {
                rec[0x89] = static_cast<double>(I(mat, 0x75b));
                rec[0x8a] = static_cast<double>(I(mat, 0x75c));
            } else {
                rec[0x83] = static_cast<double>(I(mat, 0x75b));
                rec[0x84] = static_cast<double>(I(mat, 0x75c));
            }
        }
        len = ext::f140130290(F(mat, 0x754));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x65);
        ext::f140131360(F(mat, 0x754), A + start);
    }
    // PMITT (+0x1dc8)
    if (I(mat, 0x773) == 0) {
        rec[0x66] = 0.0;
        rec[0x67] = D(mat, 0x774);
    } else {
        rec[0x66] = I(mat, 0x776) == 3 ? 1.0 : 2.0;
        len = ext::f140130200(F(mat, 0x772));
        start = fncloc(mat, &len, iifnc, slot, rec + 0x67);
        ext::f140131360(F(mat, 0x772), A + start);
    }

    // ANISO (+0x1e40): base word 131 (3D) / 133 (otherwise)
    const std::int32_t b = def::rt::dimension_is_3(owner(mat)) ? 0x83 : 0x85; // 1400015f0
    const std::int32_t am = I(mat, 0x790);
    if (am == 1) {
        rec[b] = 1.0;
    } else if (am == 2) {
        rec[b] = 2.0;
    } else if (am == 3) {
        rec[b] = 3.0;
    } else if (am == 4) {
        rec[b] = 4.0;
    } else if (am == 5) {
        rec[b] = neg(I(mat, 0x7b0));
    } else {
        rec[b] = 0.0;
    }
    if (I(mat, 0x790) == 0) {
        rec[b + 1] = 0.0;
    } else if (0 < I(mat, 0x790)) {
        rec[b + 2] = static_cast<double>(I(mat, 0x7a2));
        const std::int32_t sub = I(mat, 0x793);
        if (sub == 0) {
            rec[b + 1] = 0.0;
        } else if (sub == 1) {
            if (I(mat, 0x796) == 3) {
                rec[b + 1] = 1.0;
                rec[b + 4] = static_cast<double>(I(mat, 0x799));
            } else {
                rec[b + 1] = 2.0;
                rec[b + 3] = static_cast<double>(I(mat, 0x799));
            }
            len = I(mat, 0x799) * I(mat, 0x7a2) + I(mat, 0x799);
        } else if (sub == 2) {
            if (I(mat, 0x796) == 7) {
                rec[b + 1] = 3.0;
                rec[b + 3] = static_cast<double>(I(mat, 0x799));
                rec[b + 4] = static_cast<double>(I(mat, 0x79a));
            } else {
                rec[b + 1] = 4.0;
                rec[b + 3] = static_cast<double>(I(mat, 0x799));
                rec[b + 4] = static_cast<double>(I(mat, 0x79a));
            }
            len = I(mat, 0x7a2) * I(mat, 0x799) * I(mat, 0x79a) + I(mat, 0x799) + I(mat, 0x79a);
        }
        // len keeps its previous value for sub codes other than 0, 1, 2 (as the original)
        if (I(mat, 0x793) != 0) {
            start = fncloc(mat, &len, iifnc, slot, rec + (b + 5));
            ext::f140131360(F(mat, 0x792), A + start);
        }
    }

    // SIZEMD, TXTURE, SINTER, GBMOBI
    rec[0x42] = static_cast<double>(I(mat, 0x854));
    rec[0x96] = static_cast<double>(I(mat, 0x855));
    rec[0x97] = static_cast<double>(I(mat, 0x856));
    rec[0x98] = static_cast<double>(I(mat, 0x857));
    rec[0x9a] = static_cast<double>(I(mat, 0x876));
    rec[0x9b] = D(mat, 0x878);
    rec[0x9c] = D(mat, 0x87a);
    rec[0x9d] = D(mat, 0x87c);
    rec[0x9e] = D(mat, 0x87e);
    // GBENGY (+0x17f0), self-describing, always written
    len = ext::f140130380(F(mat, 0x5fc));
    start = fncloc(mat, &len, iifnc, slot, rec + 0x9f);
    write_func_selfdesc(F(mat, 0x5fc), A + start);
    rec[0xa0] = D(mat, 0x61a);
    rec[0xa1] = D(mat, 0x61c);
    // NUCSIZ (+0x1878), self-describing, always written
    len = ext::f140130380(F(mat, 0x61e));
    start = fncloc(mat, &len, iifnc, slot, rec + 0xa2);
    write_func_selfdesc(F(mat, 0x61e), A + start);
}

// @src 14029bdf0  LLMPR: one record per used material, in increasing material number
void write_material_list(const MaterialList* list, double* llmpr, std::int32_t* iifnc, double* llfnc,
                         const std::int32_t* width)
{
    std::int32_t slot = 0;
    std::int32_t maxno = ext::f14029c410(list);
    if (0 < maxno) {
        // 14047780c operator new(maxno * 4) ... 140479908 free
        std::vector<std::int32_t> table(static_cast<std::size_t>(maxno));
        ext::f14029ba10(list, table.data(), &maxno);
        for (std::int32_t n = 1; n <= maxno; n = n + 1) {
            Material* mat = model::material_list_find(list, n); // 14029c2c0
            if (mat != nullptr && ext::f1401b0620(mat) && (slot = field<std::int32_t>(mat, 4), 0 < slot)) {
                double* rec = llmpr + (slot - 1) * *width;
                write_material_record(mat, rec, iifnc, llfnc, &slot, table.data());
            }
        }
    }
}

} // namespace def::dbio::gendb
