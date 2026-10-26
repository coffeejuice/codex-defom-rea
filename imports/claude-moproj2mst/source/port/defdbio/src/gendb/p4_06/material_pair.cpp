// GENDB inter-material (phase transformation) writers (packet p4-06): the LLTFM list writer
// 14033a5a0 and the pair record writer 140337f50. Word meanings: scratch dbmap/material.md §4;
// the decompile is the reference. Pair fields are read at their byte offsets as in the decompile.
#include <defdbio/gendb/p4_06_material.hpp>

#include <cstring>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::gendb {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::field;
using model::Material;
using model::PhasePair;
using model::PhasePairList;

namespace {

inline std::int32_t i32(const PhasePair* p, std::size_t off) { return field<std::int32_t>(p, off); }
inline double f64(const PhasePair* p, std::size_t off) { return field<double>(p, off); }
inline const DYNARY* fn(const PhasePair* p, std::size_t off) { return &field<DYNARY>(p, off); }

// FNCLOC (1401bb3d0) on the pair's model (+0x778) with owner word 0 (a fresh local 0 per call in
// the original); returns the 1-based LLFNC start
inline std::int32_t fncloc(const PhasePair* p, const std::int32_t* len, std::int32_t* iifnc, double* fid)
{
    std::int32_t start = 0;
    const std::int32_t attr = 0;
    ext::f1401bb3d0(field<DB_STEP_DATA*>(p, 0x778), len, iifnc, &attr, fid, &start);
    return start;
}

} // namespace

// @src 140337f50  one LLTFM record
void write_phase_pair_record(const PhasePair* pp, double* rec, std::int32_t* iifnc, double* llfnc,
                             std::int32_t* counter)
{
    double* const A = llfnc - 1; // A[start] = llfnc[start - 1]
    std::int32_t start = 0;
    std::int32_t len = 0;
    rec[0] = static_cast<double>(i32(pp, 8));
    if (i32(pp, 0xc) == 0) {
        return;
    }
    // TTTD line kind
    const std::int32_t k = i32(pp, 0x10);
    if (k == 0) {
        rec[1] = 0.0;
    } else if (k == 1) {
        rec[1] = 1.0;
    } else if (k == 2) {
        rec[1] = 2.0;
    }
    if (i32(pp, 8) == 1 || i32(pp, 8) == 2) {
        *counter = *counter + 1;
        rec[0x1e] = static_cast<double>(*counter);
    }
    // TTTN: 80 blanks, then at most 80 characters of the name
    char* name = reinterpret_cast<char*>(rec) + 0x90;
    for (std::int32_t i = 0; i < 0x50; i = i + 1) {
        name[i] = ' ';
    }
    const char* src = reinterpret_cast<const char*>(pp) + 0x18;
    std::int32_t n = static_cast<std::int32_t>(std::strlen(src));
    if (0x4f < n) {
        n = 0x50;
    }
    std::strncpy(name, src, static_cast<std::size_t>(static_cast<std::int64_t>(n)));

    // PHASLH (+0x478)
    if (i32(pp, 0x47c) == 0) {
        rec[2] = 0.0;
        rec[3] = f64(pp, 0x480);
    } else {
        rec[2] = i32(pp, 0x488) == 3 ? 1.0 : 2.0;
        len = ext::f140130200(fn(pp, 0x478));
        start = fncloc(pp, &len, iifnc, rec + 3);
        ext::f140131360(fn(pp, 0x478), A + start);
    }
    // PHASVL (+0x4f0)
    if (i32(pp, 0x4f4) == 0) {
        rec[4] = 0.0;
        rec[5] = f64(pp, 0x4f8);
    } else {
        if (i32(pp, 0x4f4) == 1) {
            rec[4] = i32(pp, 0x500) == 3 ? 1.0 : 2.0;
        } else {
            rec[4] = 3.0;
            rec[0x1c] = static_cast<double>(i32(pp, 0x50c));
            rec[0x1d] = static_cast<double>(i32(pp, 0x510));
        }
        len = ext::f140130290(fn(pp, 0x4f0));
        start = fncloc(pp, &len, iifnc, rec + 5);
        ext::f140131360(fn(pp, 0x4f0), A + start);
    }
    // TRNSFP (+0x5e4 flag, +0x5e8)
    if (i32(pp, 0x5e4) == 0) {
        rec[0xe] = 0.0;
    } else {
        rec[0xe] = 1.0;
        if (i32(pp, 0x5ec) == 0) {
            rec[0xf] = 0.0;
            rec[0x10] = f64(pp, 0x5f0);
        } else {
            if (i32(pp, 0x5ec) == 1) {
                rec[0xf] = i32(pp, 0x5f8) == 3 ? 1.0 : 2.0;
            } else {
                rec[0xf] = 3.0;
                rec[0x1f] = static_cast<double>(i32(pp, 0x604));
                rec[0x20] = static_cast<double>(i32(pp, 0x608));
            }
            len = ext::f140130290(fn(pp, 0x5e8));
            start = fncloc(pp, &len, iifnc, rec + 0x10);
            ext::f140131360(fn(pp, 0x5e8), A + start);
        }
    }
    // PHEQVF (+0x5e0 code, +0x568), only for PHASTF type 2
    if (i32(pp, 8) == 2) {
        rec[0x21] = static_cast<double>(i32(pp, 0x5e0));
        if (i32(pp, 0x56c) == 0) {
            rec[0x22] = f64(pp, 0x570);
        } else {
            if (i32(pp, 0x56c) != 1) {
                rec[0x23] = static_cast<double>(i32(pp, 0x584));
                rec[0x24] = static_cast<double>(i32(pp, 0x588));
            }
            len = ext::f140130290(fn(pp, 0x568));
            start = fncloc(pp, &len, iifnc, rec + 0x22);
            ext::f140131360(fn(pp, 0x568), A + start);
        }
    }

    // TTTD: internal type +0x14
    double* p = nullptr;
    switch (i32(pp, 0x14)) {
    case 0:
    case 1:
    case 2:
    case 3:
        rec[6] = 1.0;
        rec[0xd] = static_cast<double>(i32(pp, 0x14)); // 0.0 / 1.0 / 2.0 / 3.0
        rec[8] = static_cast<double>(i32(pp, 0xb8));
        rec[9] = static_cast<double>(i32(pp, 0x94));
        rec[10] = static_cast<double>(i32(pp, 0x98));
        rec[0xb] = static_cast<double>(i32(pp, 0x9c));
        len = ext::f140336300(pp);
        start = fncloc(pp, &len, iifnc, rec + 7);
        ext::f140131360(fn(pp, 0x78), A + start);
        if (i32(pp, 0xb8) == 1) {
            rec[0xc] = f64(pp, 0x70);
        } else if (i32(pp, 0xb8) == 2) {
            p = A + start;
            p[len - 2] = f64(pp, 0x170);
            p[len - 1] = f64(pp, 0x178);
        }
        rec[0x31] = static_cast<double>(i32(pp, 0xf0));
        break;
    case 4:
        rec[6] = 2.0;
        rec[8] = static_cast<double>(i32(pp, 0x238));
        rec[9] = static_cast<double>(i32(pp, 0x214));
        rec[10] = static_cast<double>(i32(pp, 0x218));
        rec[0xb] = static_cast<double>(i32(pp, 0x21c));
        len = ext::f140336300(pp);
        start = fncloc(pp, &len, iifnc, rec + 7);
        ext::f140131360(fn(pp, 0x1f8), A + start);
        break;
    case 5:
        rec[6] = 3.0;
        rec[8] = 11.0; // DAT_140522fe0
        len = static_cast<std::int32_t>(rec[8]);
        start = fncloc(pp, &len, iifnc, rec + 7);
        p = A + start;
        for (int i = 0; i < 11; ++i) {
            p[i] = f64(pp, 0x270 + static_cast<std::size_t>(i) * 8);
        }
        break;
    case 6:
        rec[6] = 4.0;
        rec[8] = static_cast<double>(i32(pp, 0x2e4) * 2 + 4);
        len = static_cast<std::int32_t>(rec[8]);
        start = fncloc(pp, &len, iifnc, rec + 7);
        p = A + start;
        p[0] = f64(pp, 0x340);
        p[1] = f64(pp, 0x348);
        p[2] = f64(pp, 0x350);
        p[3] = f64(pp, 0x358);
        ext::f1401317c0(fn(pp, 0x2c8), p + 4);
        break;
    case 7:
        rec[6] = 5.0;
        rec[8] = 6.0;
        len = static_cast<std::int32_t>(rec[8]);
        start = fncloc(pp, &len, iifnc, rec + 7);
        p = A + start;
        for (int i = 0; i < 6; ++i) {
            p[i] = f64(pp, 0x360 + static_cast<std::size_t>(i) * 8);
        }
        break;
    case 8:
        rec[6] = 6.0;
        rec[8] = 4.0;
        len = static_cast<std::int32_t>(rec[8]);
        start = fncloc(pp, &len, iifnc, rec + 7);
        p = A + start;
        for (int i = 0; i < 4; ++i) {
            p[i] = f64(pp, 0x390 + static_cast<std::size_t>(i) * 8);
        }
        break;
    case 9:
        rec[6] = 7.0;
        rec[8] = 7.0;
        len = static_cast<std::int32_t>(rec[8]);
        start = fncloc(pp, &len, iifnc, rec + 7);
        p = A + start;
        for (int i = 0; i < 7; ++i) {
            p[i] = f64(pp, 0x3b0 + static_cast<std::size_t>(i) * 8);
        }
        break;
    case 10:
        rec[6] = 8.0;
        rec[8] = static_cast<double>(i32(pp, 0x414) << 1);
        len = static_cast<std::int32_t>(rec[8]);
        start = fncloc(pp, &len, iifnc, rec + 7);
        ext::f140131360(fn(pp, 0x3f8), A + start);
        break;
    case 0xb:
        rec[6] = 9.0;
        rec[0xd] = 0.0;
        rec[8] = static_cast<double>(i32(pp, 0x138));
        rec[9] = static_cast<double>(i32(pp, 0x114));
        rec[10] = static_cast<double>(i32(pp, 0x118));
        rec[0xb] = static_cast<double>(i32(pp, 0x11c));
        len = ext::f140336300(pp);
        start = fncloc(pp, &len, iifnc, rec + 7);
        ext::f140131360(fn(pp, 0xf8), A + start);
        if (i32(pp, 0x138) == 1) {
            rec[0xc] = f64(pp, 0x70);
        } else if (i32(pp, 0x138) == 2) {
            p = A + start;
            p[len - 2] = f64(pp, 1000);
            p[len - 1] = f64(pp, 0x3f0);
        }
        break;
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
        // codes 10 / 11 / 12 / 13 (DAT_14051bee8, 140522fe0, 140524cf8, 14053a7a8)
        rec[6] = static_cast<double>(i32(pp, 0x14) - 2);
        rec[7] = static_cast<double>(i32(pp, 0x1c0));
        len = ext::f140336300(pp);
        start = fncloc(pp, &len, iifnc, rec + 8);
        ext::f140131360(fn(pp, 0x180), A + start);
        break;
    case 0x10:
        rec[6] = static_cast<double>(static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(i32(pp, 0x470))));
        break;
    default:
        break;
    }

    // COARSN (+0x660 flag, +0x678)
    if (i32(pp, 0x660) == 0) {
        rec[0x25] = 0.0;
    } else {
        rec[0x25] = 1.0;
        if (i32(pp, 0x67c) == 0) {
            rec[0x26] = 0.0;
            rec[0x29] = f64(pp, 0x680);
        } else {
            if (i32(pp, 0x67c) == 1 && i32(pp, 0x688) == 3) {
                rec[0x26] = 1.0;
            }
            len = ext::f140130290(fn(pp, 0x678));
            start = fncloc(pp, &len, iifnc, rec + 0x29);
            ext::f140131360(fn(pp, 0x678), A + start);
        }
        rec[0x2a] = f64(pp, 0x668);
        rec[0x2b] = f64(pp, 0x670);
    }
    // IPSNRG (+0x6f0)
    if (i32(pp, 0x6f4) == 0) {
        rec[0x2c] = 0.0;
        rec[0x2f] = f64(pp, 0x6f8);
    } else {
        if (i32(pp, 0x6f4) == 1) {
            rec[0x2c] = i32(pp, 0x700) == 3 ? 1.0 : 2.0;
        } else {
            rec[0x2c] = 3.0;
            rec[0x2d] = static_cast<double>(i32(pp, 0x70c));
            rec[0x2e] = static_cast<double>(i32(pp, 0x710));
        }
        len = ext::f140130290(fn(pp, 0x6f0));
        start = fncloc(pp, &len, iifnc, rec + 0x2f);
        ext::f140131360(fn(pp, 0x6f0), A + start);
    }
    // VARSEL
    rec[0x30] = static_cast<double>(i32(pp, 0x768));
}

// @src 14033a5a0  LLTFM: one record per ordered pair of used materials
void write_phase_pair_list(const PhasePairList* list, double* lltfm, std::int32_t* iifnc, double* llfnc,
                           const std::int32_t* width)
{
    std::int32_t counter = 0;
    DB_STEP_DATA* m = field<DB_STEP_DATA*>(list, 0x18);
    const std::int32_t maxno = ext::f14029c410(&field<model::MaterialList>(m, 0x1328));
    if (0 < maxno) {
        for (std::int32_t a = 1; a <= maxno; a = a + 1) {
            Material* from = model::step_material_by_id(*m, a); // 1401d7060
            if (from == nullptr) {
                continue;
            }
            for (std::int32_t b = 1; b <= maxno; b = b + 1) {
                Material* to = model::step_material_by_id(*m, b);
                if (to != nullptr && ext::f1401b0620(from) && ext::f1401b0620(to)) {
                    const std::int32_t slot_from = field<std::int32_t>(from, 4);
                    const std::int32_t slot_to = field<std::int32_t>(to, 4);
                    const std::int32_t t = slot_to - 1;
                    const std::int32_t nmat = ext::f140199c10(m);
                    double* rec = lltfm + *width * (t * nmat + -1 + slot_from);
                    const PhasePair* pp = ext::f14033a040(list, field<std::int32_t>(from, 0), field<std::int32_t>(to, 0));
                    if (pp != nullptr) {
                        write_phase_pair_record(pp, rec, iifnc, llfnc, &counter);
                    }
                }
            }
        }
    }
}

} // namespace def::dbio::gendb
