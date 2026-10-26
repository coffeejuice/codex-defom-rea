// Signature checks and the version number of datarm23. See dbcore/dbops.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbcore/dbops.hpp>
#include <defrt/ftnstr/ftnstr.hpp>

#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defrt/ftn/helpers.hpp>

#include "internal.hpp"

#include <cstring>

namespace def::dbio::dbcore {

namespace {

using def::rt::ftn::ftn_strncmp;

// Reads the 20 characters of record 1 (5 CHARACTER*4 items) of `unit`.
void read_sig(const std::int32_t* unit, char sig[20], std::int32_t* ierr)
{
    const std::int32_t mode = 1;
    const std::int32_t n = 5;
    std::int32_t rec = 1;
    block_io_char4(&mode, unit, sig, &n, &rec, ierr);
}

bool is(const char* sig, const char* lit)
{
    const std::int32_t len = 0x14;
    std::int32_t r = 0;
    ftn_strncmp(sig, lit, &len, &r);
    return r == 0;
}

// The signature check of FUN_1403fbab0 / fbda5 / fc05c: OPEN(3, FILE=file(1:200), STATUS='OLD',
// ACCESS='DIRECT', ACTION='READ'), read record 1, CLOSE(3). Returns false (and *ierr = 1) when
// the OPEN or the READ fails; `close_on_open_error` is true only for FUN_1403fbab0.
bool read_file_sig(const char* file, char sig[20], std::int32_t* ierr, bool close_on_open_error)
{
    const std::int32_t unit = 3;     // DAT_14070bab0 / bae8 / bb18
    const std::int32_t open5 = 5;
    const std::int32_t close0 = 0;
    std::int32_t ios = 0;
    std::int32_t close_ios = 0;
    *ierr = 0;
    fortran_open(&open5, &unit, file, "OLD ", ierr, 200, 3); // str 140bb1310
    if (*ierr != 0) {
        if (close_on_open_error) {
            fortran_open(&close0, &unit, "", "", &close_ios, 0, 0); // FUN_1404b53a4: CLOSE(3)
        }
        *ierr = 1;
        return false;
    }
    read_sig(&unit, sig, ierr);
    fortran_open(&close0, &unit, "", "", &ios, 0, 0); // CLOSE(3)
    if (*ierr != 0) {
        *ierr = 1;
        return false;
    }
    return true;
}

using FileCheck = void (*)(const char*, std::int32_t*, std::int32_t*);

// part of @src 140224930 / 140224a20 / 140224b10
std::int32_t run_check(const char* name, std::int32_t* err, FileCheck check)
{
    char buf[0x400] = {}; // DAT_1406fdaf8 (0) then 1023 zero bytes
    std::int32_t yes = 0;
    std::int32_t e = 0;
    if (name == nullptr) {
        return 0;
    }
    rt::to_fortran(buf, name, static_cast<std::int32_t>(sizeof buf)); // 140323bf0 (p1-10)
    check(buf, &yes, &e);
    if (e != 0) {
        *err = e;
        yes = 0;
    }
    return yes;
}

} // namespace

// @src 1403b1e45  2D signature of the open unit (strings 140615ef0, 140615f10)
void check_sig_2d(const std::int32_t* unit, std::int32_t* yes, std::int32_t* ierr)
{
    char sig[20];
    *ierr = 0;
    *yes = 0;
    read_sig(unit, sig, ierr);
    if (*ierr != 0) {
        *ierr = 1;
        return;
    }
    *yes = (is(sig, "SFTC-DEFORM-2D-DB450") || is(sig, "SFTC-DEFORM-2D-DB720")) ? 1 : 0;
}

// @src 1403b1ff5  3D signature (string 140615f30)
void check_sig_3d(const std::int32_t* unit, std::int32_t* yes, std::int32_t* ierr)
{
    char sig[20];
    *ierr = 0;
    *yes = 0;
    read_sig(unit, sig, ierr);
    if (*ierr != 0) {
        *ierr = 1;
        return;
    }
    if (is(sig, "SFTC-DEFORM-3D-DB230")) {
        *yes = 1;
    }
}

// @src 1403b26e5  combined signatures (strings 140615fd0, 140615ff0, 140616010)
void check_sig_23(const std::int32_t* unit, std::int32_t* yes, std::int32_t* ierr)
{
    char sig[20];
    *ierr = 0;
    *yes = 0;
    read_sig(unit, sig, ierr);
    if (*ierr != 0) {
        *ierr = 1;
        return;
    }
    if (is(sig, "SFTC-DEFORM-23-DB100") || is(sig, "SFTC-DEF-2D-DB100-00") ||
        is(sig, "SFTC-DEF-3D-DB100-00")) {
        *yes = 1;
    }
}

// @src 1403fbab0  (strings 1406262e0, 140626300)
void file_sig_2d(const char* file, std::int32_t* yes, std::int32_t* ierr)
{
    char sig[20];
    *yes = 0;
    if (!read_file_sig(file, sig, ierr, true)) {
        return;
    }
    if (is(sig, "SFTC-DEFORM-2D-DB450") || is(sig, "SFTC-DEF-2D-DB100-00")) {
        *yes = 1;
    }
}

// @src 1403fbda5  (strings 140626320, 140626340)
void file_sig_3d(const char* file, std::int32_t* yes, std::int32_t* ierr)
{
    char sig[20];
    *yes = 0;
    if (!read_file_sig(file, sig, ierr, false)) {
        return;
    }
    if (is(sig, "SFTC-DEFORM-3D-DB230") || is(sig, "SFTC-DEF-3D-DB100-00")) {
        *yes = 1;
    }
}

// @src 1403fc05c  (strings 140626360, 140626380, 1406263a0)
void file_sig_23(const char* file, std::int32_t* yes, std::int32_t* ierr)
{
    char sig[20];
    *yes = 0;
    if (!read_file_sig(file, sig, ierr, false)) {
        return;
    }
    if (is(sig, "SFTC-DEFORM-23-DB100") || is(sig, "SFTC-DEF-2D-DB100-00") ||
        is(sig, "SFTC-DEF-3D-DB100-00")) {
        *yes = 1;
    }
}

// @src 140224930  2D check of a C file name
std::int32_t is_db_2d(const char* name, std::int32_t* err) { return run_check(name, err, file_sig_2d); }
// @src 140224a20  3D check
std::int32_t is_db_3d(const char* name, std::int32_t* err) { return run_check(name, err, file_sig_3d); }
// @src 140224b10  combined check
std::int32_t is_db_23(const char* name, std::int32_t* err) { return run_check(name, err, file_sig_23); }

// @src 1402248c0  any of the three
std::int32_t is_deform_db(const char* name, std::int32_t* err)
{
    if (is_db_2d(name, err) != 0) {
        return 1;
    }
    if (is_db_3d(name, err) != 0) {
        return 1;
    }
    return is_db_23(name, err) != 0 ? 1 : 0;
}

// @src 1403b32b5  version number (strings 1406164b0..1406166c0). Each test is COMISD of SMI1
// taken as a double against the literal, JP then JZ: an ordered equality.
void db_version(std::int32_t* version)
{
    const std::int32_t family = g_db.file_family;
    *version = 0;
    double v;
    std::memcpy(&v, g_db.h.smi1, 8); // DAT_140b6cb50
    const auto is_v = [&](const char* lit) {
        double l;
        std::memcpy(&l, lit, 8);
        return l == v;
    };
    if (family == 2) {
        if (is_v(" 5.0    ") || is_v(" 5.0 CNV")) { *version = 5000; return; }
        if (is_v(" 5.1    ") || is_v(" 5.1 CNV")) { *version = 5100; return; }
        if (is_v(" 5.2    ")) { *version = 5200; return; }
        if (is_v(" 5.3    ")) { *version = 5300; return; }
        if (is_v(" 6.0    ")) { *version = 6000; return; }
        if (is_v(" 7.0    ")) { *version = 7000; return; }
        if (is_v(" 7.10   ")) { *version = 7100; return; }
        if (is_v(" 7.20   ")) { *version = 7200; return; }
        if (is_v(" 8.00   ")) { *version = 8000; return; }
        if (is_v(" 8.10   ")) { *version = 8100; return; }
        if (is_v(" 8.20   ")) { *version = 8200; return; }
        if (is_v(" 9.00   ")) { *version = 9000; return; }
        if (is_v(" 9.01   ")) { *version = 9010; return; }
        *version = 9100;
        return;
    }
    if (family == 3) {
        if (is_v(" 2.3    ")) { *version = 2300; return; }
        if (is_v(" 2.4    ") || is_v(" 3.0    ")) { *version = 3000; return; }
        if (is_v(" 3.1.1  ")) { *version = 3100; return; }
        if (is_v(" 3.2    ")) { *version = 3200; return; }
        if (is_v(" 3.3    ")) { *version = 3300; return; }
        if (is_v(" 4.0    ")) { *version = 4000; return; }
        if (is_v(" 5.0    ")) { *version = 5000; return; }
        if (is_v(" 5.1    ")) { *version = 5100; return; }
        if (is_v(" 6.0    ")) { *version = 6000; return; }
        if (is_v(" 6.01   ")) { *version = 6010; return; }
        *version = 6100;
        return;
    }
    if (family == 0x17) {
        static const char* const lit[] = {"10.00   ", "11.00   ", "12.00   ", "13.00   ",
                                          "14.00   ", "15.00   ", "16.00   ", "17.00   ",
                                          "18.00   ", "19.00   ", "20.00   "};
        for (int k = 0; k < 11; ++k) {
            if (is_v(lit[k])) {
                *version = (10 + k) * 1000;
                return;
            }
        }
        *version = 10000;
        return;
    }
    detail::ftn_write_text("Error on getting DB Version");
}

} // namespace def::dbio::dbcore
