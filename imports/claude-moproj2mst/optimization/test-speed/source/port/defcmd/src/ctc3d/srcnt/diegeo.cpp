// DIEGEO.DAT -> DIE_00k.GEO (14003cd08, 14002b89c) and the SRCNT debug print 14003c742
// (GENCTC_3d_search.md sec. 11.3). Packet p67-7, group "srcnt".
#include "srcnt_internal.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/fio.hpp>

#include <cstring>
#include <string>

namespace def::cmd::ctc3d {

namespace srcnt_detail {

namespace fio = def::rt::ftn::fio;

namespace {
constexpr u64 k_die_name = 0x1405c68c0; // s_DIE_000_GEO_1405c68c0, CHARACTER*11 'DIE_000.GEO'

// internal WRITE(buf(off+1:off+w), '(Iw)') v: the record is exactly w characters
void internal_write_i(u64 off, i32 v, int w)
{
    const std::string s = fio::format_i(v, w);
    std::memcpy(mp<char>(k_die_name + off), s.data(), static_cast<std::size_t>(w));
}
} // namespace

// part of @src 14003cd08
void die_name_set(i32 k)
{
    if (k < 10) {
        internal_write_i(6, k, 1); // format (I1), str 1405c6910
    } else if (k < 100) {
        internal_write_i(5, k, 2); // format (I2), str 1405c6960
    }
}

// part of @src 14003cd08
void die_name_reset(i32 k)
{
    if (9 < k) {
        internal_write_i(5, I(0x1405c69c0), 1); // (I1) of the constant 0 at DAT_1405c69c0
    }
}

const char* die_name() { return mp<char>(k_die_name); }

// part of @src 14002b89c: 1000 FORMAT(I8), str 1405c64d0
void geo_write_count(i32 n) { fio::Write(81, "(I8)").i(n).end(); }

// part of @src 14002b89c: 1001 FORMAT(I6,3F16.7), str 1405c6530
void geo_write_point(i32 i, const double* xyz) { fio::Write(81, "(I6,3F16.7)").i(i).d(xyz[0]).d(xyz[1]).d(xyz[2]).end(); }

// part of @src 14002b89c: 1002 FORMAT(5I8), str 1405c6590
void geo_write_poly(i32 j, i32 n1, i32 n2, i32 n3, i32 n4) { fio::Write(81, "(5I8)").i(j).i(n1).i(n2).i(n3).i(n4).end(); }

} // namespace srcnt_detail

using namespace srcnt_detail;
namespace fio = def::rt::ftn::fio;

// @src 14002b89c  writes DIE_00k.GEO on unit 81: point count, points (cube coordinates), polygon
// count, polygons with n4 = n1 for triangles and n4 = -|n4| when EI2 != 1
void fun_14002b89c(i32* obj, i32* inst, i32* npts, i32* npoly, char* name, i32* dbgpoly, std::int64_t name_len)
{
    // OPEN(UNIT=81, FILE=name, STATUS='UNKNOWN') (str 140bad440); the runtime drops trailing blanks
    std::string fname(name, static_cast<std::size_t>(name_len > 0 ? name_len : 0));
    while (!fname.empty() && fname.back() == ' ') {
        fname.pop_back();
    }
    fio::open_unknown(81, fname.c_str());
    fio::rewind(81); // FUN_1404b5a10
    geo_write_count(*npts);
    const i32 n = *npts;
    I(0x14064c948) = 1;
    if (0 < n) {
        do {
            i32 p = I(0x14064c948);
            fun_140227fc0(obj, inst, &p, DP(0x14064c950));
            geo_write_point(I(0x14064c948), DP(0x14064c950));
            I(0x14064c948) += 1;
        } while (I(0x14064c948) <= n);
    }
    geo_write_count(*npoly);
    const i32 np = *npoly;
    I(0x14064c948) = 1;
    if (0 < np) {
        do {
            i32 p = I(0x14064c948);
            fun_140227e00(obj, inst, &p, IP(0x14064c970), IP(0x14064c980));
            i32 field = 2;
            p = I(0x14064c948);
            fun_140228260(obj, inst, &p, &field, IP(0x14064c984), IP(0x14064c988));
            if (*dbgpoly != 0 && *dbgpoly == I(0x14064c948)) {
                fio::Write(6)
                    .a("In OUTAMG2: KOBJ,IGSET,I,IVISB ", 31) // str 140bad450
                    .i(*obj)
                    .i(*inst)
                    .i(I(0x14064c948))
                    .i(I(0x14064c984))
                    .end();
            }
            if (I(0x14064c980) == 3) {
                I(0x14064c97c) = I(0x14064c970);
            }
            if (I(0x14064c984) != 1) {
                I(0x14064c97c) = ineg(iabs(I(0x14064c97c)));
            }
            geo_write_poly(I(0x14064c948), I(0x14064c970), I(0x14064c974), I(0x14064c978), I(0x14064c97c));
            I(0x14064c948) += 1;
        } while (I(0x14064c948) <= np);
    }
    fio::close(81);
}

// @src 14003cd08  DIE_00k.GEO for every partner k (file name built in the DIE_000.GEO buffer)
void fun_14003cd08(i32* obj, i32* inst, i32* dbgpoly)
{
    const i32 np = dat_14077867c;
    I(0x14066aa4c) = 1;
    if (np < 1) {
        return;
    }
    do {
        I(0x14066aa50) = I(0x140771918ull + 4 * static_cast<std::int64_t>(I(0x14066aa4c)));
        I(0x14066aa54) = I(0x140778684ull + 4 * static_cast<std::int64_t>(I(0x14066aa50)));
        const std::int64_t l = static_cast<std::int64_t>(I(0x14066aa54)) * 300 + static_cast<std::int64_t>(I(0x14066aa50)) * 3;
        I(0x14066aa5c) = I(0x14076b6a8ull + 4 * l);
        I(0x14066aa58) = I(0x14076e588ull + 4 * l);
        die_name_set(I(0x14066aa50));
        if (*dbgpoly != 0) {
            i32 field = 2;
            fun_140228260(obj, inst, dbgpoly, &field, IP(0x14066aa60), IP(0x14066aa64));
            fio::Write(6).a("IN OUTDIE").i(*dbgpoly).i(I(0x14066aa60)).end(); // str 140bad7e0
        }
        fun_14002b89c(IP(0x14066aa50), IP(0x14066aa54), IP(0x14066aa5c), IP(0x14066aa58), mp<char>(0x1405c68c0), dbgpoly, 11);
        die_name_reset(I(0x14066aa50));
        I(0x14066aa4c) += 1;
    } while (I(0x14066aa4c) <= np);
}

// @src 14003c742  debug print of polygon *poly on unit *unit (list-directed): 'NBCDL', node,
// NI4 buffer (8) per node; 'ICURVE', EI6 buffer (8); 'IVISB', EI2
void fun_14003c742(i32* obj, i32* inst, i32* poly, i32* unit)
{
    fun_140227e00(obj, inst, poly, IP(0x14066a9c0), IP(0x14066a9e0));
    I(0x14066a9e4) = 1;
    if (I(0x14066a9e0) >= 1) {
        do {
            I(0x14066a9e8) = iabs(I(0x14066a9c0ull + 4 * static_cast<std::int64_t>(I(0x14066a9e4) - 1)));
            i32 field = 4;
            fun_140228830(obj, inst, IP(0x14066a9e8), &field, IP(0x14066a9f0), IP(0x14066aa10));
            fio::Write w(*unit);
            w.a("NBCDL").i(I(0x14066a9e8)); // str 140bad7b0
            for (int k = 0; k < 8; ++k) {
                w.i(I(0x14066a9f0ull + 4 * static_cast<u64>(k)));
            }
            w.end();
            I(0x14066a9e4) += 1;
        } while (I(0x14066a9e4) <= I(0x14066a9e0));
    }
    i32 field = 6;
    fun_140228260(obj, inst, poly, &field, IP(0x14066aa20), IP(0x14066aa40));
    {
        fio::Write w(*unit);
        w.a("ICURVE"); // str 140bad7c0
        for (int k = 0; k < 8; ++k) {
            w.i(I(0x14066aa20ull + 4 * static_cast<u64>(k)));
        }
        w.end();
    }
    field = 2;
    fun_140228260(obj, inst, poly, &field, IP(0x14066aa44), IP(0x14066aa48));
    fio::Write(*unit).a("IVISB").i(I(0x14066aa44)).end();
}

} // namespace def::cmd::ctc3d
