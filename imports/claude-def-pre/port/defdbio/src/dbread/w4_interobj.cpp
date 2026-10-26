// dbread/w4_interobj.cpp: ReadInterObjInfo (1401d5c10) and the LLBRT record reader 1401f58a0
// (helper W4 of packet p-dbread; docs/re/dbmap/interobj.md sections 1.2 and 2). The GENDB mirror
// is gendb/p4_06/interobj_record.cpp (1401f7730). Record fields are written at their byte offsets
// as in the decompile (param_1 is an int*, index k is byte 4k).
#include <defdbio/dbread/w4.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/dbread/w2.hpp>

#include <cstdint>

#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/w4_access.hpp"

namespace def::dbio::dbread {

using model::Cls_1401f46e0;
using model::DB_STEP_DATA;
using model::DYNARY;
using model::LLOBJ_DATA;
using namespace w4d;

namespace {

inline std::int32_t ri(const double* rec, std::size_t k) { return static_cast<std::int32_t>(rec[k]); } // cvttsd2si
inline DYNARY* dyn(Cls_1401f46e0* e, std::size_t off) { return reinterpret_cast<DYNARY*>(at(e, off)); }

// part of @src 1401f58a0: 1401309b0(e + off, IIFNC, LLFNC, fnum, &0) with a fresh zero byte
inline void load_fn(Cls_1401f46e0* e, std::size_t off, std::int32_t* iifnc, double* llfnc, std::int32_t fnum)
{
    char flag = 0;
    read_func_from_db_array(dyn(e, off), iifnc, llfnc, fnum, &flag);
}

// part of @src 1401f58a0: friction type (word 2) into +0x0
void friction_type(Cls_1401f46e0* e, const double* rec)
{
    const std::int32_t k = ri(rec, 2);
    if (k == 1) {
        fi(e, 0) = 1;
    } else if (k == 2) {
        fi(e, 0) = 2;
    } else if (k == 3) {
        fi(e, 0) = 3;
    } else if (k == 4) {
        fi(e, 0) = 4;
    }
}

// part of @src 1401f58a0: one friction coefficient set. set 0: flag byte +4, ref +8, code +0x10,
// DYNARY +0x18 (value +0x20), pairs +0x108; set 1: +5, +0xc, +0x14, +0x90 (+0x98), +0x138.
// Record words: code ic (< 0: reference -code, value iv), value iv, function if, extras ix..ix+2.
void friction_set(Cls_1401f46e0* e, int set, const double* rec, std::size_t ic, std::size_t iv,
                  std::size_t ifn, std::size_t ix, std::int32_t* iifnc, double* llfnc)
{
    const std::size_t flag = set == 0 ? 0x4 : 0x5;
    const std::size_t ref = set == 0 ? 0x8 : 0xc;
    const std::size_t code = set == 0 ? 0x10 : 0x14;
    const std::size_t fn = set == 0 ? 0x18 : 0x90;
    const std::size_t pairs = set == 0 ? 0x108 : 0x138;
    if (ri(rec, ic) < 0) {
        f<std::uint8_t>(e, flag) = 1;
        fi(e, ref) = -ri(rec, ic);
        fd(e, fn + 8) = rec[iv];
        return;
    }
    f<std::uint8_t>(e, flag) = 0;
    fi(e, code) = ri(rec, ic);
    switch (fi(e, code)) {
    case 0:
        fi(e, fn + 4) = 0;
        fd(e, fn + 8) = rec[iv];
        break;
    case 1:
        fi(e, fn + 4) = 1;
        fi(e, fn + 0x10) = 1;
        break;
    case 2:
        fi(e, fn + 4) = 1;
        fi(e, fn + 0x10) = 4;
        break;
    case 3:
        fi(e, fn + 4) = 1;
        fi(e, fn + 0x10) = 3;
        break;
    case 4:
        fi(e, fn + 4) = 1;
        fi(e, fn + 0x10) = 0x15;
        break;
    case 5:
        fi(e, fn + 4) = 3;
        fi(e, fn + 0x10) = 4;
        fi(e, fn + 0x14) = 3;
        fi(e, fn + 0x18) = 0x15;
        fi(e, fn + 0x1c) = ri(rec, ix);
        fi(e, fn + 0x20) = ri(rec, ix + 1);
        fi(e, fn + 0x24) = ri(rec, ix + 2);
        break;
    case 6:
        fi(e, fn + 4) = 0;
        fd(e, pairs) = rec[ix];
        fd(e, pairs + 8) = rec[ix + 1];
        break;
    case 7:
        fi(e, fn + 4) = 0;
        fd(e, pairs + 0x10) = rec[ix];
        fd(e, pairs + 0x18) = rec[ix + 1];
        break;
    case 8:
        fi(e, fn + 4) = 0;
        fd(e, pairs + 0x20) = rec[ix];
        fd(e, pairs + 0x28) = rec[ix + 1];
        break;
    default:
        break;
    }
    if (fi(e, fn + 4) != 0) {
        load_fn(e, fn, iifnc, llfnc, ri(rec, ifn));
    }
}

// part of @src 1401f58a0: IHTCOF (DYNARY +0x1b8): words 7 (function, 0 = constant word 6) and 8 (kind;
// < 0 also sets the reference +0x230 / +0x234)
void heat_transfer(Cls_1401f46e0* e, const double* rec, std::int32_t* iifnc, double* llfnc)
{
    if (ri(rec, 7) == 0) {
        fi(e, 0x1bc) = 0;
        fd(e, 0x1c0) = rec[6];
    } else {
        const std::int32_t k = ri(rec, 8);
        if (k == 1) {
            fi(e, 0x1bc) = 1;
            fi(e, 0x1c8) = 1;
        } else if (k == 2) {
            fi(e, 0x1bc) = 1;
            fi(e, 0x1c8) = 4;
        } else if (k == 3) {
            fi(e, 0x1bc) = 1;
            fi(e, 0x1c8) = 3;
        }
        load_fn(e, 0x1b8, iifnc, llfnc, ri(rec, 7));
    }
    if (ri(rec, 8) < 0) {
        fi(e, 0x230) = -ri(rec, 8);
        fi(e, 0x234) = 1;
    }
}

// part of @src 1401f58a0: INTRST (DYNARY +0x2b8): kind word ik, function / value word iv
void interface_resistance(Cls_1401f46e0* e, const double* rec, std::size_t ik, std::size_t iv,
                          std::int32_t* iifnc, double* llfnc)
{
    if (ri(rec, ik) == 0) {
        fi(e, 0x2bc) = 0;
        fd(e, 0x2c0) = rec[iv];
    } else {
        const std::int32_t k = ri(rec, ik);
        if (k == 1) {
            fi(e, 0x2bc) = 1;
            fi(e, 0x2c8) = 3;
        } else if (k == 2) {
            fi(e, 0x2bc) = 1;
            fi(e, 0x2c8) = 4;
        }
        load_fn(e, 0x2b8, iifnc, llfnc, ri(rec, iv));
    }
}

// part of @src 1401f58a0: WMODEL (+0x340): kind word ik (> 0: coefficients from the function named
// in word ik + 1; < 0: reference)
void wear_model(Cls_1401f46e0* e, const double* rec, std::size_t ik, std::int32_t* iifnc, double* llfnc)
{
    if (ri(rec, ik) == 0) {
        fi(e, 0x340) = 0;
    } else if (ri(rec, ik) < 1) {
        if (ri(rec, ik) < 0) {
            fi(e, 0x340) = 3;
            fi(e, 0x378) = -ri(rec, ik);
        }
    } else {
        const double* p = llfnc + (iifnc[ri(rec, ik + 1) * 2 + -2] + -1);
        if (ri(rec, ik) == 1) {
            fi(e, 0x340) = 1;
            fd(e, 0x348) = p[0];
            fd(e, 0x350) = p[1];
            fd(e, 0x358) = p[2];
            fd(e, 0x360) = p[3];
        } else if (ri(rec, ik) == 2) {
            fi(e, 0x340) = 2;
            fd(e, 0x368) = p[0];
            fd(e, 0x370) = p[1];
        }
    }
}

// part of @src 1401f58a0: windows: words iw / iw + 1 first / last LLWIN row (0: none); one window
// per row appended to +0x188 (1402694a0), +0x170 = k, +0x174 = 1, then 14025de80
void windows(Cls_1401f46e0* e, const double* rec, std::size_t iw, double* llwin, std::int32_t w_llwin,
             double* llwrz, std::int32_t* iifnc, double* llfnc)
{
    const double first = rec[iw];
    std::int32_t n = 0;
    if (static_cast<std::int32_t>(first) != 0) {
        n = static_cast<std::int32_t>(rec[iw + 1] - rec[iw]) + 1; // 1401f6507: subsd, cvttsd2si
    }
    for (std::int32_t k = 0; k < n; k = k + 1) {
        model::LLWINDEF_DATA* w = model::winlist_new(&e->u188);
        fi(w, 0x170) = k + 1;
        fi(w, 0x174) = 1;
        read_window(w, llwin + w_llwin * (static_cast<std::int32_t>(first) + -1 + k), llwrz, iifnc, llfnc);
    }
}

// part of @src 1401f58a0: RGDCNT words (+0x37c, +0x380, +0x384, +0x388)
void rigid_contact(Cls_1401f46e0* e, const double* rec, std::size_t i0)
{
    fi(e, 0x37c) = ri(rec, i0);
    fi(e, 0x380) = ri(rec, i0 + 1);
    fi(e, 0x384) = ri(rec, i0 + 2);
    fd(e, 0x388) = rec[i0 + 3];
}

} // namespace

// @src 1401f58a0  word 0 contact flag (0: nothing else); 2D layout (1400015e0) and 3D layout
//                 (1400015f0) of the remaining words
void read_inter_obj_record(Cls_1401f46e0* e, const double* rec, std::int32_t* iifnc, double* llfnc,
                           double* llwin, std::int32_t w_llwin, double* llwrz)
{
    if (ri(rec, 0) == 0) {
        fi(e, 0x170) = 0;
        return;
    }
    {
        const std::int32_t k = ri(rec, 0);
        if (k == 1) {
            fi(e, 0x170) = 1;
        } else if (k == 2) {
            fi(e, 0x170) = 2;
        }
    }
    if (rt::dimension_is_2(e->owner)) {
        {
            const std::int32_t k = ri(rec, 0x10);
            if (k == 0) {
                fi(e, 0x178) = 0;
            } else if (k == 1) {
                fi(e, 0x178) = 1;
            } else if (k == 2) {
                fi(e, 0x178) = 2;
            }
        }
        fd(e, 0x180) = rec[0x11];
        fd(e, 0x238) = rec[0x12];
        friction_type(e, rec);
        friction_set(e, 0, rec, 5, 3, 4, 0x18, iifnc, llfnc);
        if (fi(e, 0) == 4) {
            friction_set(e, 1, rec, 0x23, 0x21, 0x22, 0x24, iifnc, llfnc);
        }
        heat_transfer(e, rec, iifnc, llfnc);
        // FRCROT (DYNARY +0x240): word 0xd on, function word 0xe (value when off)
        if (ri(rec, 0xd) == 0) {
            fi(e, 0x244) = 0;
            fd(e, 0x248) = rec[0xe];
        } else {
            fi(e, 0x244) = 1;
            fi(e, 0x250) = 1;
            load_fn(e, 0x240, iifnc, llfnc, ri(rec, 0xe));
        }
        interface_resistance(e, rec, 0x13, 0x14, iifnc, llfnc);
        windows(e, rec, 0x16, llwin, w_llwin, llwrz, iifnc, llfnc);
        rigid_contact(e, rec, 0x1b);
        wear_model(e, rec, 0x1f, iifnc, llfnc);
    }
    if (rt::dimension_is_3(e->owner)) {
        if (ri(rec, 1) == 0) {
            fi(e, 0x174) = 1;
        } else {
            fi(e, 0x174) = 0;
        }
        {
            const std::int32_t k = ri(rec, 0xc);
            if (k == 0) {
                fi(e, 0x178) = 0;
            } else if (k == 1) {
                fi(e, 0x178) = 1;
            } else if (k == 2) {
                fi(e, 0x178) = 2;
            }
        }
        fd(e, 0x180) = rec[0xd];
        friction_type(e, rec);
        friction_set(e, 0, rec, 5, 3, 4, 0xe, iifnc, llfnc);
        if (fi(e, 0) == 4) {
            friction_set(e, 1, rec, 0x2c, 0x2a, 0x2b, 0x2d, iifnc, llfnc);
        }
        heat_transfer(e, rec, iifnc, llfnc);
        interface_resistance(e, rec, 0x13, 0x14, iifnc, llfnc);
        // CNTMTH (+0x330 on, +0x334, +0x338): words 0x1f..0x21
        if (ri(rec, 0x1f) == 0) {
            fi(e, 0x330) = 0;
        } else {
            fi(e, 0x330) = 1;
            fi(e, 0x334) = ri(rec, 0x20);
            fd(e, 0x338) = rec[0x21];
        }
        wear_model(e, rec, 0x22, iifnc, llfnc);
        windows(e, rec, 0x24, llwin, w_llwin, llwrz, iifnc, llfnc);
        rigid_contact(e, rec, 0x26);
    }
}

// @src 1401d5c10  for every ordered pair (i, j) of objects with kind != 7 (i == j included) a new
//                 record (1401faaa0), ids +0x168 / +0x16c; boundary pairs (bm of i outer, bs of j
//                 inner) out of 1..NB print an error, else record (bs - 1) * NB + bm is read while
//                 +0x170 is still 0
void read_inter_obj_info(DB_STEP_DATA* m)
{
    std::int32_t dim = model::llobj::int_at_1c(m); // 140158e90
    model::select_index_tables(&dim);              // 1401c7d40
    const std::int32_t start = m->u1dc0[aix(0x1406e82ec)];
    double* a = static_cast<double*>(m->u1b78);
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* oi = m->objects.at(i + 1); // 14017aa80
        if (oi == nullptr || fi(oi, 0xc) == 7) {
            continue;
        }
        for (std::int32_t j = 0; j < m->u1378; j = j + 1) {
            LLOBJ_DATA* oj = m->objects.at(j + 1);
            if (oj == nullptr || fi(oj, 0xc) == 7) {
                continue;
            }
            Cls_1401f46e0* e = model::pair_append(&m->inter_obj); // 1401faaa0
            fi(e, 0x168) = fi(oi, 8);
            fi(e, 0x16c) = fi(oj, 8);
            for (std::int32_t bm = fi(oi, 0x3588); bm < fi(oi, 0x3588) + fi(oi, 0x888); bm = bm + 1) {
                for (std::int32_t bs = fi(oj, 0x3588); bs < fi(oj, 0x3588) + fi(oj, 0x888); bs = bs + 1) {
                    if (bm < 1 || m->u13f0 < bm || bs < 1 || m->u13f0 < bs) {
                        rt::crt::printf("ERROR in _DB_STEP_DATA::ReadInterObjInfoFromDB()!\n");
                    } else if (fi(e, 0x170) == 0) {
                        read_inter_obj_record(
                            e, a + (start + -1) + m->u1dc0[aix(0x1406e83d8)] * ((bs + -1) * m->u13f0 + -1 + bm),
                            static_cast<std::int32_t*>(m->u2220), static_cast<double*>(m->u2228),
                            static_cast<double*>(m->u2230), m->u1dc0[aix(0x1406e83e0)],
                            static_cast<double*>(m->u2238));
                    }
                }
            }
        }
    }
}

} // namespace def::dbio::dbread
