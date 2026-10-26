// dbread/w1_blocks.cpp: the object sub-blocks DBREAD fills from the IIOBJ / LLOBJ row
// (packet p-dbread, helper W1): Cls_1401860a0 (obj+0x1ea0, FUN_140186f30) with its body-force
// records (Cls_140258580, FUN_1402589a0), Cls_140189440 (obj+0x2d10, FUN_14018a180) and
// Cls_1401956d0 (obj+0x2e68, FUN_140195ff0). The classes are opaque slots in llobj_data.hpp, so
// members are written at the binary's offsets.
#include <defdbio/dbread/w1.hpp>

#include <cstring>

#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defmodel/layout/sf_linked_list.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/w1_access.hpp"

namespace def::dbio::dbread {

using w1::at;
using w1::copy8;
using w1::dyn;

namespace {

// one window list of a block: n windows from LLWIN rows first-1 .. first-1+n-1
void read_window_rows(void* list, std::int32_t n, std::int32_t first, bool set_kind2, double* llwin,
                      std::int32_t win_w, double* llwrz, std::int32_t* iifnc, double* llfnc)
{
    for (std::int32_t k = 0; k < n; k = k + 1) {
        model::LLWINDEF_DATA* w = model::winlist_new(static_cast<model::Cls_140269250*>(list)); // 1402694a0
        w->index = k + 1;
        if (set_kind2) {
            w->u174 = 2;
        }
        read_window(w, llwin + win_w * (first - 1 + k), llwrz, iifnc, llfnc); // 14025de80
    }
}

} // namespace

// @src 140186f30
void read_obj_block_1ea0(void* blk, std::int32_t* ii, double* r, std::int32_t* iifnc, double* llfnc)
{
    auto I = [blk](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(blk, off); };
    auto Q = [blk, r](std::ptrdiff_t dst, std::int32_t k) { copy8(blk, dst, r, static_cast<std::ptrdiff_t>(k) * 8); };
    auto RI = [r](std::int32_t k) -> std::int32_t { return static_cast<std::int32_t>(r[k]); };
    auto F = [blk, iifnc, llfnc](std::ptrdiff_t off, std::int32_t fn) {
        char with_shape = 0;
        read_func_from_db_array(dyn(blk, off), iifnc, llfnc, fn, &with_shape);
    };
    Q(0x18, 0);
    Q(0x08, 1);
    Q(0x00, 2);
    Q(0x10, 3);
    I(0x9c) = RI(0x34);
    I(0x94) = RI(0x35);
    I(0x98) = RI(0x36);
    I(0x78) = RI(0x42);
    I(0xa8) = RI(0x38);
    I(0xa4) = RI(0x39);
    Q(0xb0, 0x3a);
    Q(0xb8, 0x3b);
    const std::int32_t kind = RI(0x40);
    if (kind < 2) {
        I(0x138) = 0;
    } else if (kind < 0xc) {
        I(0x138) = 1;
    } else if (kind < 0x16) {
        I(0x138) = 2;
    }
    if (kind == 0 || kind == 10 || kind == 0x14) {
        I(0x144) = 0;
        Q(0x148, 0x41);
    } else {
        I(0x144) = 1;
        I(0x150) = 1;
        F(0x140, RI(0x41));
    }
    if (RI(0x43) == 0) {
        I(0xc4) = 0;
        Q(0xc8, 0x44);
    } else {
        if (RI(0x43) == 1) {
            I(0xd0) = 1;
        } else if (RI(0x43) == 2) {
            I(0xd0) = 0x1c;
        } else {
            I(0xd0) = 0x1d;
        }
        I(0xc4) = 1;
        F(0xc0, RI(0x44));
    }
    Q(0x58, 0x45);
    Q(0x60, 0x46);
    if (RI(0x48) < 0) {
        I(0x4c) = 3;
        I(0x50) = w1::crt_abs(RI(0x48)); // 140484ab8
    } else {
        I(0x4c) = RI(0x48);
    }
    I(0x68) = RI(0x4a);
    Q(0x70, 0x47);
    I(0x2b0) = RI(0x49);
    I(0xa0) = ii[0xe];
    auto* const owner = at<model::DB_STEP_DATA*>(blk, 0x2b8);
    auto& forces = at<model::linked::Cls_140258580List>(blk, 0x200);
    auto* const list218 = reinterpret_cast<model::SfLinkedList*>(w1::ptr(blk, 0x218));
    std::int32_t pf0;
    std::int32_t pf1;
    std::int32_t pl0;
    std::int32_t pl1;
    if (!rt::dimension_is_2(owner)) {
        I(0x28) = RI(0x2a);
        if (I(0x28) == 1) {
            Q(0x30, 0x2b);
            Q(0x38, 0x2c);
            Q(0x40, 0x2d);
        } else if (I(0x28) == 2) {
            I(0x48) = RI(0x2b);
        }
        I(0x1f8) = RI(0x2e);
        Q(0x1f0, 0x2f);
        if (at<double>(blk, 0x1f0) == 0.0) {
            I(0x1f8) = -1;
        }
        I(0x2b4) = ii[10];
        pf0 = ii[0x13];
        pf1 = ii[0x14];
        pl0 = ii[0x15];
        pl1 = ii[0x16];
    } else {
        I(0x80) = RI(0x11);
        I(0x90) = RI(0x12);
        Q(0x88, 0x13);
        I(0x28) = RI(0x37);
        if (I(0x28) == 1) {
            Q(0x30, 0x32);
            Q(0x38, 0x33);
            at<std::uint64_t>(blk, 0x40) = 0;
        } else if (I(0x28) == 2) {
            I(0x48) = RI(0x32);
        }
        I(0x7c) = RI(0x53);
        I(0x1f8) = RI(0x54);
        Q(0x1f0, 0x55);
        if (at<double>(blk, 0x1f0) == 0.0) {
            I(0x1f8) = -1;
        }
        pf0 = ii[0xf];
        pf1 = ii[0x10];
        pl0 = ii[0x11];
        pl1 = ii[0x12];
    }
    if (0 < pf0) {
        for (std::int32_t k = pf0; k <= pf1; k = k + 1) {
            auto* rec = model::geo_kw::wpaxis_add(forces, 0); // 140259950
            rec->id = (k - pf0) + 1;
            body_force_from_values(rec, llfnc + (iifnc[k * 2 - 2] - 1)); // 1402589a0
        }
    }
    if (0 < pl0) {
        for (std::int32_t k = pl0; k <= pl1; k = k + 1) {
            model::SfLinkedData* node = model::llobj_copy::sf_list_add(list218); // 14025a7d0
            node->from_values(llfnc + (iifnc[k * 2 - 2] - 1));                   // vtable slot 2
        }
    }
    if (!rt::dimension_is_2(owner)) {
        I(0x7c) = RI(0x51);
    }
    if (RI(0x56) == 0) {
        I(0x278) = 0;
    } else {
        I(0x278) = RI(0x56);
        F(0x238, RI(0x57));
    }
}

// @src 14018a180
void read_obj_block_2d10(void* blk, std::int32_t* ii, double* r, std::int32_t* iifnc, double* llfnc, double* llwin,
                         std::int32_t win_w, double* llwrz)
{
    (void)ii;
    auto I = [blk](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(blk, off); };
    auto Q = [blk, r](std::ptrdiff_t dst, std::ptrdiff_t src) { copy8(blk, dst, r, src); };
    auto RI = [r](std::ptrdiff_t off) -> std::int32_t { return static_cast<std::int32_t>(at<double>(r, off)); };
    Q(0xb8, 0xa0);
    Q(0xc0, 0xa8);
    Q(0xd0, 0xb0);
    Q(0xc8, 0xb8);
    auto* const owner = at<model::DB_STEP_DATA*>(blk, 0);
    if (!rt::dimension_is_2(owner)) {
        if (rt::dimension_is_3(owner)) {
            Q(0x30, 0xc0);
            Q(0x88, 0xe0);
            I(0x90) = RI(0xe8);
            Q(0x10, 0xf8);
            if (at<double>(blk, 0x10) <= 1e-8) { // DAT_14051bed0 (COMISD / JC: NaN keeps the value)
                at<double>(blk, 0x10) = 1.0;
            }
            Q(0x18, 0x100);
            Q(0x20, 0x108);
            Q(0x28, 0x110);
            Q(0x38, 0x118);
            I(0x74) = RI(0x128);
            I(0x78) = RI(0x130);
            I(0x7c) = RI(0x138);
            I(0x80) = RI(0x1b8);
            I(0x84) = RI(0xd8);
            I(0xb0) = RI(0x140);
            I(0xb4) = RI(0x148);
            Q(0xa0, 0x190);
            Q(0xa8, 0x198);
            const std::int32_t first = RI(0xc8);
            std::int32_t n;
            if (first < 1) {
                n = 0;
            } else {
                n = (RI(0xd0) - first) + 1;
            }
            read_window_rows(w1::ptr(blk, 0x40), n, first, true, llwin, win_w, llwrz, iifnc, llfnc);
        }
    } else {
        I(0x70) = RI(0xf0);
        Q(0x10, 0x100);
        Q(0x18, 0x110);
        Q(0x20, 0x118);
        Q(0x28, 0x120);
        Q(0x30, 0x128);
        I(0x74) = RI(0xf8);
        Q(0x38, 0x108);
        I(0xb0) = RI(0x140);
        I(0xb4) = RI(0x148);
        Q(0xa0, 0x150);
        Q(0xa8, 0x158);
        const std::int32_t first = RI(0x160);
        std::int32_t n;
        if (first < 1) {
            n = 0;
        } else {
            n = (RI(0x168) - first) + 1;
        }
        read_window_rows(w1::ptr(blk, 0x40), n, first, true, llwin, win_w, llwrz, iifnc, llfnc);
    }
}

// @src 140195ff0
void read_obj_block_2e68(void* blk, std::int32_t* ii, double* r, std::int32_t* iifnc, double* llfnc, double* llwin,
                         std::int32_t win_w, double* llwrz, std::int32_t version)
{
    (void)ii;
    auto R = [r](std::ptrdiff_t off) -> double { return at<double>(r, off); };
    // count of the rows [R(lo), R(hi)]: (int)(hi - lo) + 1 when (int)lo != 0 (the subtraction is in
    // doubles, 1401960b4)
    auto count = [&R](std::ptrdiff_t lo, std::ptrdiff_t hi) -> std::int32_t {
        if (static_cast<std::int32_t>(R(lo)) != 0) {
            return static_cast<std::int32_t>(R(hi) - R(lo)) + 1;
        }
        return 0;
    };
    auto rows = [&](std::ptrdiff_t list, std::int32_t n, std::ptrdiff_t lo) {
        read_window_rows(w1::ptr(blk, list), n, static_cast<std::int32_t>(R(lo)), false, llwin, win_w, llwrz, iifnc,
                         llfnc);
    };
    auto* const owner = at<model::DB_STEP_DATA*>(blk, 0x198);
    if (!rt::dimension_is_2(owner)) {
        if (9999 < version) {
            rows(0x48, count(0x290, 0x298), 0x290);
        }
    } else if (8999 < version) {
        rows(0x48, count(0x288, 0x290), 0x288);
    }
    rows(0x78, count(0x1e0, 0x1e8), 0x1e0);
    rows(0xa8, count(0x1f0, 0x1f8), 0x1f0);
    if (10999 < version) {
        std::int32_t n = 0;
        if (0 < static_cast<std::int32_t>(R(0x2c0))) {
            n = static_cast<std::int32_t>(R(0x2c8) - R(0x2c0)) + 1;
        }
        rows(0x168, n, 0x2c0);
    }
    rows(0xd8, count(0x258, 0x260), 0x258);
    rows(0x108, count(0x268, 0x270), 0x268);
    rows(0x138, count(0x278, 0x280), 0x278);
}

// @src 140258ed0
void body_force_set_08(void* r, const double* v)
{
    if (v != nullptr) {
        std::memcpy(w1::ptr(r, 0x08), v, 0x18);
    }
}

// @src 140258f50
void body_force_set_20(void* r, const double* v)
{
    if (v != nullptr) {
        std::memcpy(w1::ptr(r, 0x20), v, 0x18);
    }
}

// @src 140258fd0
void body_force_set_68(void* r, const double* v)
{
    if (v != nullptr) {
        std::memcpy(w1::ptr(r, 0x68), v, 0x18);
        rt::ftn::vec3_normalize(&at<double>(r, 0x68)); // 14023b720
    }
}

// @src 140259060
void body_force_set_80(void* r, const double* v)
{
    if (v != nullptr) {
        std::memcpy(w1::ptr(r, 0x80), v, 0x18);
        rt::ftn::vec3_normalize(&at<double>(r, 0x80));
    }
}

// @src 1402590f0
void body_force_set_98(void* r, const double* v)
{
    if (v != nullptr) {
        std::memcpy(w1::ptr(r, 0x98), v, 0x18);
    }
}

// @src 1402589a0
void body_force_from_values(void* r, double* v)
{
    auto I = [r](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(r, off); };
    auto Q = [r, v](std::ptrdiff_t dst, std::int32_t k) { copy8(r, dst, v, static_cast<std::ptrdiff_t>(k) * 8); };
    I(4) = static_cast<std::int32_t>(v[6]);
    switch (I(4)) {
    case 0:
    case 1:
    case 2:
        body_force_set_08(r, v);
        body_force_set_20(r, v + 3);
        I(0x38) = static_cast<std::int32_t>(v[7]);
        Q(0x40, 8);
        Q(0x48, 9);
        Q(0x50, 10);
        Q(0x58, 0xb);
        break;
    case 3:
        body_force_set_08(r, v);
        body_force_set_20(r, v + 3);
        I(0x60) = static_cast<std::int32_t>(v[7]);
        break;
    case 4:
        body_force_set_68(r, v);
        body_force_set_80(r, v + 3);
        body_force_set_98(r, v + 9);
        I(0xb0) = static_cast<std::int32_t>(v[7]);
        Q(0xb8, 8);
        break;
    case 5:
        // the original clears the first three LLFNC values of the record in place
        std::memset(v, 0, 0x18);
        body_force_set_20(r, v + 3);
        Q(0xb8, 8);
        break;
    case 7:
        body_force_set_20(r, v);
        body_force_set_08(r, v + 3);
        Q(0x40, 7);
        I(0xc8) = static_cast<std::int32_t>(v[8]);
        I(0xcc) = static_cast<std::int32_t>(v[9]);
        Q(0xd0, 10);
        Q(0xd8, 0xb);
        break;
    default:
        break;
    }
}

} // namespace def::dbio::dbread
