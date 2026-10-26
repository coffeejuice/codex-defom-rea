// dbread/w4_intermatl.cpp: ReadInterMatlInfo (1401d0180) and the LLTFM record reader 140336740
// (helper W4 of packet p-dbread; docs/re/dbmap/material.md section 4). The GENDB mirror is
// gendb/p4_06/material_pair.cpp (140337f50). Pair fields are written at their byte offsets as in
// the decompile; names in comments from layout/phase_pair.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbread/w4.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/dbread/w2.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>

#include "dbread/w4_access.hpp"

namespace def::dbio::dbread {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::PhasePair;
using namespace w4d;

namespace {

inline std::int32_t ri(const double* rec, std::size_t k) { return static_cast<std::int32_t>(rec[k]); } // cvttsd2si
inline DYNARY* dyn(PhasePair* pp, std::size_t off) { return reinterpret_cast<DYNARY*>(at(pp, off)); }

// part of @src 140336740: 1401309b0(pp + off, IIFNC, LLFNC, fnum, &0) with a fresh zero byte
inline void load_fn(PhasePair* pp, std::size_t off, std::int32_t* iifnc, double* llfnc, std::int32_t fnum)
{
    char flag = 0;
    read_func_from_db_array(dyn(pp, off), iifnc, llfnc, fnum, &flag);
}

// part of @src 140336740: LLFNC words of the function named in IIFNC row k (1-based), from its start
inline const double* fn_words(std::int32_t* iifnc, double* llfnc, std::int32_t k)
{
    return llfnc + (iifnc[k * 2 + -2] + -1);
}

} // namespace

// @src 1401d7090  -> 14029c260 on the material list (+0x1328)
model::Material* step_material_nth(DB_STEP_DATA* m, std::int32_t n)
{
    return model::material_list_nth(&m->materials, n);
}

// @src 140336740  word 0 PHASTF (0: nothing else), 1 TTTD kind, 0x12.. TTTN name (80 chars),
//                 2/3 PHASLH, 4/5 PHASVL (+0x1c/0x1d dims), 0xe..0x10 TRNSFP (+0x1f/0x20),
//                 0x21..0x24 PHEQVF, 6 TTTD type (7.. its data), 0x25..0x2b COARSN, 0x2c..0x2f
//                 IPSNRG, 0x30 VARSEL, 0x31 TTTD type 1 x
void read_phase_pair_record(PhasePair* pp, const double* rec, std::int32_t* iifnc, double* llfnc)
{
    fi(pp, 0x8) = ri(rec, 0);
    if (ri(rec, 0) == 0) {
        fi(pp, 0xc) = 0;
        return;
    }
    fi(pp, 0xc) = 1;
    {
        const std::int32_t k = ri(rec, 1);
        if (k == 0) {
            fi(pp, 0x10) = 0;
        } else if (k == 1) {
            fi(pp, 0x10) = 1;
        } else if (k == 2) {
            fi(pp, 0x10) = 2;
        }
    }
    std::strncpy(at(pp, 0x18), reinterpret_cast<const char*>(rec + 0x12), 0x50);
    text_byteswap(at(pp, 0x18), fi(pp->owner, 0x748), 10, 0); // 140324020
    rt::trim_fortran(at(pp, 0x18), 0x50); // 1403239f0
    // PHASLH (DYNARY +0x478)
    if (ri(rec, 2) == 0) {
        fi(pp, 0x47c) = 0;
        fd(pp, 0x480) = rec[3];
    } else {
        const std::int32_t k = ri(rec, 2);
        if (k == 1) {
            fi(pp, 0x47c) = 1;
            fi(pp, 0x488) = 3;
        } else if (k == 2) {
            fi(pp, 0x47c) = 1;
            fi(pp, 0x488) = 0xc;
        }
        load_fn(pp, 0x478, iifnc, llfnc, ri(rec, 3));
    }
    // PHASVL (DYNARY +0x4f0)
    if (ri(rec, 4) == 0) {
        fi(pp, 0x4f4) = 0;
        fd(pp, 0x4f8) = rec[5];
    } else {
        const std::int32_t k = ri(rec, 4);
        if (k == 1) {
            fi(pp, 0x4f4) = 1;
            fi(pp, 0x500) = 3;
        } else if (k == 2) {
            fi(pp, 0x4f4) = 1;
            fi(pp, 0x500) = 0xc;
        } else if (k == 3) {
            fi(pp, 0x4f4) = 2;
            fi(pp, 0x500) = 3;
            fi(pp, 0x504) = 0xc;
            fi(pp, 0x50c) = ri(rec, 0x1c);
            fi(pp, 0x510) = ri(rec, 0x1d);
        }
        load_fn(pp, 0x4f0, iifnc, llfnc, ri(rec, 5));
    }
    // TRNSFP (+0x5e4 on, DYNARY +0x5e8)
    if (ri(rec, 0xe) == 0) {
        fi(pp, 0x5e4) = 0;
    } else {
        fi(pp, 0x5e4) = 1;
        if (ri(rec, 0xf) == 0) {
            fi(pp, 0x5ec) = 0;
            fd(pp, 0x5f0) = rec[0x10];
        } else {
            const std::int32_t k = ri(rec, 0xf);
            if (k == 1) {
                fi(pp, 0x5ec) = 1;
                fi(pp, 0x5f8) = 3;
            } else if (k == 2) {
                fi(pp, 0x5ec) = 1;
                fi(pp, 0x5f8) = 0xc;
            } else if (k == 3) {
                fi(pp, 0x5ec) = 2;
                fi(pp, 0x5f8) = 3;
                fi(pp, 0x5fc) = 0xc;
                fi(pp, 0x604) = ri(rec, 0x1f);
                fi(pp, 0x608) = ri(rec, 0x20);
            }
            load_fn(pp, 0x5e8, iifnc, llfnc, ri(rec, 0x10));
        }
    }
    // PHEQVF (t at +0x5e0, DYNARY +0x568)
    fi(pp, 0x5e0) = ri(rec, 0x21);
    if (fi(pp, 0x5e0) == 0 || fi(pp, 0x5e0) == 4) {
        fi(pp, 0x56c) = 0;
        fd(pp, 0x570) = rec[0x22];
    } else {
        switch (fi(pp, 0x5e0) + -1) {
        case 0:
        case 4:
            fi(pp, 0x56c) = 1;
            fi(pp, 0x578) = 3;
            break;
        case 1:
        case 5:
            fi(pp, 0x56c) = 1;
            fi(pp, 0x578) = 0xc;
            break;
        case 2:
        case 6:
            fi(pp, 0x56c) = 2;
            fi(pp, 0x578) = 3;
            fi(pp, 0x57c) = 0xc;
            fi(pp, 0x584) = ri(rec, 0x23);
            fi(pp, 0x588) = ri(rec, 0x24);
            break;
        default:
            break;
        }
        load_fn(pp, 0x568, iifnc, llfnc, ri(rec, 0x22));
    }
    // TTTD type (word 6); 140336e86: (unsigned)(type - 1) > 12 goes to the default
    const std::int32_t tsel = ri(rec, 6) + -1;
    switch (static_cast<std::uint32_t>(tsel) > 0xcu ? -1 : tsel) {
    case 0: {
        const std::int32_t c = ri(rec, 0xd);
        if (c == 0) {
            fi(pp, 0x14) = 0;
            fi(pp, 0x88) = 0xc;
            fi(pp, 0x8c) = 6;
            fi(pp, 0x90) = 3;
        } else if (c == 1) {
            fi(pp, 0x14) = 1;
            fi(pp, 0x88) = 3;
            fi(pp, 0x8c) = 7;
            fi(pp, 0x90) = 0xe;
        } else if (c == 2) {
            fi(pp, 0x14) = 2;
            fi(pp, 0x88) = 0xc;
            fi(pp, 0x8c) = 6;
            fi(pp, 0x90) = 3;
        } else if (c == 3) {
            fi(pp, 0x14) = 3;
            fi(pp, 0x88) = 3;
            fi(pp, 0x8c) = 7;
            fi(pp, 0x90) = 0xe;
        }
        fi(pp, 0xb8) = ri(rec, 8);
        fi(pp, 0x6c) = fi(pp, 0xb8);
        const double fnum = rec[7];
        fi(pp, 0x94) = ri(rec, 9);
        fi(pp, 0x98) = ri(rec, 10);
        fi(pp, 0x9c) = ri(rec, 0xb);
        load_fn(pp, 0x78, iifnc, llfnc, static_cast<std::int32_t>(fnum));
        fi(pp, 0xf0) = ri(rec, 0x31);
        if (fi(pp, 0xb8) == 1) {
            fd(pp, 0x70) = rec[0xc];
        } else if (fi(pp, 0xb8) == 2) {
            const double* p = llfnc + (iifnc[static_cast<std::int32_t>(fnum) * 2] + -3);
            fd(pp, 0x170) = p[0];
            fd(pp, 0x178) = p[1];
        }
        break;
    }
    case 1: {
        fi(pp, 0x14) = 4;
        fi(pp, 0x238) = ri(rec, 8);
        const double fnum = rec[7];
        fi(pp, 0x214) = ri(rec, 9);
        fi(pp, 0x218) = ri(rec, 10);
        fi(pp, 0x21c) = ri(rec, 0xb);
        load_fn(pp, 0x1f8, iifnc, llfnc, static_cast<std::int32_t>(fnum));
        break;
    }
    case 2: {
        fi(pp, 0x14) = 5;
        const double* p = fn_words(iifnc, llfnc, ri(rec, 7));
        for (std::size_t k = 0; k < 11; k = k + 1) {
            fd(pp, 0x270 + k * 8) = p[k];
        }
        break;
    }
    case 3: {
        fi(pp, 0x14) = 6;
        const double* p = fn_words(iifnc, llfnc, ri(rec, 7));
        for (std::size_t k = 0; k < 4; k = k + 1) {
            fd(pp, 0x340 + k * 8) = p[k];
        }
        fi(pp, 0x2e4) = (ri(rec, 8) + -4) / 2;
        read_table_rows_1d(dyn(pp, 0x2c8), const_cast<double*>(p + 4)); // 140131ea0
        break;
    }
    case 4: {
        fi(pp, 0x14) = 7;
        const double* p = fn_words(iifnc, llfnc, ri(rec, 7));
        for (std::size_t k = 0; k < 6; k = k + 1) {
            fd(pp, 0x360 + k * 8) = p[k];
        }
        break;
    }
    case 5: {
        fi(pp, 0x14) = 8;
        const double* p = fn_words(iifnc, llfnc, ri(rec, 7));
        for (std::size_t k = 0; k < 4; k = k + 1) {
            fd(pp, 0x390 + k * 8) = p[k];
        }
        break;
    }
    case 6: {
        fi(pp, 0x14) = 9;
        const double* p = fn_words(iifnc, llfnc, ri(rec, 7));
        for (std::size_t k = 0; k < 7; k = k + 1) {
            fd(pp, 0x3b0 + k * 8) = p[k];
        }
        break;
    }
    case 7: {
        fi(pp, 0x14) = 10;
        const std::int32_t start = iifnc[ri(rec, 7) * 2 + -2];
        fi(pp, 0x414) = ri(rec, 8) / 2;
        read_table_rows_1d(dyn(pp, 0x3f8), llfnc + (start + -1)); // 140131ea0
        break;
    }
    case 8: {
        if (ri(rec, 0xd) == 0) {
            fi(pp, 0x14) = 0xb;
            fi(pp, 0x108) = 0xc;
            fi(pp, 0x10c) = 6;
            fi(pp, 0x110) = 3;
        }
        fi(pp, 0x138) = ri(rec, 8);
        fi(pp, 0x6c) = fi(pp, 0x138);
        const double fnum = rec[7];
        fi(pp, 0x114) = ri(rec, 9);
        fi(pp, 0x118) = ri(rec, 10);
        fi(pp, 0x11c) = ri(rec, 0xb);
        load_fn(pp, 0xf8, iifnc, llfnc, static_cast<std::int32_t>(fnum));
        if (fi(pp, 0x138) == 1) {
            fd(pp, 0x70) = rec[0xc];
        } else if (fi(pp, 0x138) == 2) {
            const double* p = llfnc + (iifnc[static_cast<std::int32_t>(fnum) << 1] + -3);
            fd(pp, 1000) = p[0];
            fd(pp, 0x3f0) = p[1];
        }
        break;
    }
    case 9:
    case 10:
    case 0xb:
    case 0xc:
        fi(pp, 0x14) = tsel + 3; // 0xc, 0xd, 0xe, 0xf
        fi(pp, 0x1c0) = ri(rec, 7);
        load_fn(pp, 0x180, iifnc, llfnc, ri(rec, 8));
        break;
    default:
        // 140337bbc: comisd 0.0, rec[6]; jbe -> only when 0.0 > rec[6] (false for NaN)
        if (0.0 > rec[6]) {
            fi(pp, 0x14) = 0x10;
            fi(pp, 0x470) = std::abs(ri(rec, 6)); // 140484ab8
        }
        break;
    }
    // COARSN (+0x660 on, DYNARY +0x678)
    if (ri(rec, 0x25) == 0) {
        fi(pp, 0x660) = 0;
    } else {
        fi(pp, 0x660) = 1;
        if (ri(rec, 0x26) == 0) {
            fi(pp, 0x67c) = 0;
            fd(pp, 0x680) = rec[0x29];
        } else {
            if (ri(rec, 0x26) == 1) {
                fi(pp, 0x67c) = 1;
                fi(pp, 0x688) = 3;
            }
            load_fn(pp, 0x678, iifnc, llfnc, ri(rec, 0x29));
        }
        fd(pp, 0x668) = rec[0x2a];
        fd(pp, 0x670) = rec[0x2b];
    }
    // IPSNRG (DYNARY +0x6f0)
    if (ri(rec, 0x2c) == 0) {
        fi(pp, 0x6f4) = 0;
        fd(pp, 0x6f8) = rec[0x2f];
    } else {
        const std::int32_t k = ri(rec, 0x2c);
        if (k == 1) {
            fi(pp, 0x6f4) = 1;
            fi(pp, 0x700) = 3;
        } else if (k == 2) {
            fi(pp, 0x6f4) = 1;
            fi(pp, 0x700) = 0x17;
        } else if (k == 3) {
            fi(pp, 0x6f4) = 2;
            fi(pp, 0x700) = 3;
            fi(pp, 0x704) = 0x17;
            fi(pp, 0x70c) = ri(rec, 0x2d);
            fi(pp, 0x710) = ri(rec, 0x2e);
        }
        load_fn(pp, 0x6f0, iifnc, llfnc, ri(rec, 0x2f));
    }
    fi(pp, 0x768) = ri(rec, 0x30);
}

// @src 1401d0180  LLTFM base = A + start(LLTFM) - 1, record (j * NMAT + i) * width; nMat^2 pairs
//                 are appended (also for empty records)
void read_inter_matl_info(DB_STEP_DATA* m)
{
    std::int32_t dim = model::llobj::int_at_1c(m); // 140158e90
    model::select_index_tables(&dim);              // 1401c7d40
    const std::int32_t start = m->u1dc0[aix(0x1406e82d8)];
    double* a = static_cast<double*>(m->u1b78);
    for (std::int32_t i = 0; i < m->n_materials; i = i + 1) {
        model::Material* from = step_material_nth(m, i + 1);
        if (from != nullptr) {
            for (std::int32_t j = 0; j < m->n_materials; j = j + 1) {
                model::Material* to = step_material_nth(m, j + 1);
                if (to != nullptr) {
                    PhasePair* pp = model::phase_pair_append(&m->phase_pairs); // 140339df0
                    fi(pp, 0) = fi(from, 0);
                    fi(pp, 4) = fi(to, 0);
                    read_phase_pair_record(pp,
                                           a + (start + -1) +
                                               m->u1dc0[aix(0x1406e83c4)] * (j * m->n_materials + i),
                                           static_cast<std::int32_t*>(m->u2220), static_cast<double*>(m->u2228));
                }
            }
        }
    }
}

} // namespace def::dbio::dbread
