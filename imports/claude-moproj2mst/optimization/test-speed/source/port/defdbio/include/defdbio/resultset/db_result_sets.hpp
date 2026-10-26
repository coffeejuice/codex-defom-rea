// defdbio/resultset/db_result_sets.hpp: the two datarm23 front-end routines that read a DB's
// header (FUN_1403d84b0, "OPENING DATABASE FILE = ") and its result-set directory
// (FUN_1403d96fc, "END OF READING DATABASE RESULT SET INFORMATION").
//
// Both open the DB on Fortran unit 3, run one DB operation through FUN_1403b5a04 and close the
// unit. The original takes every argument by reference (Fortran); the port keeps pointers where
// the original leaves an output unchanged on an error.
//
// Print flag: both routines echo what they read with Fortran WRITEs when *print == 1. Every
// caller in DEF_PRE_64 (FUN_1401dc3f0, FUN_1401dd0d0, FUN_1401e51d0) passes a local that holds 0,
// so that branch is unreachable and not ported (a call with print == 1 stops in
// def::rt::not_ported).
#pragma once

#include <cstdint>

namespace def::dbio::resultset {

// The outputs of FUN_1403d84b0 (its parameters 3..13), written only when the header was read.
struct DbHeader {
    char title[80];      // p3  TITLE (record 2 words 1..20)
    char smi1[8];        // p4  version text
    char smi2[8];        // p5  release date
    char smi3[8];        // p6  first simulation date
    char smi4[8];        // p7  last simulation date
    std::int32_t istart; // p8
    std::int32_t ismtyp; // p9
    std::int32_t nrsets; // p10 entries in the directory
    std::int32_t nxtrec; // p11 next free record
    std::int32_t family; // p12 2, 3 or 23
    std::int32_t version; // p13 version number (FUN_1403b32b5)
};

// @src 1403d84b0
// `name` is the Fortran file name of `name_len` characters (the caller pads it with blanks,
// FUN_140323bf0). On return *err is 0 (header read; `out` written), 99 (the file did not open)
// or the DB operation's code (0x65, 100, 0x12d; `out` unchanged).
void read_db_header(const char* name, const std::int32_t* name_len, DbHeader* out, std::int32_t* err,
                    const std::int32_t* print);

// @src 1403d96fc
// Reads words 1..10 of directory entries 1..*count into `words` (10 per entry, whatever *ncols
// says; *count == 0 means NRSETS of the last header read and is written back), then counts
// NUMRST / NUMRMS (globals_p1_13.hpp) over NRSETS entries of `words` taken *ncols words apart.
// `words` must hold max(*count, NRSETS) * max(10, *ncols) words: the original writes 10 per entry
// into the caller's *count * *ncols buffer (FUN_1401dc3f0 sizes it with 5 columns for 3D
// databases before version 3000, which overruns it there).
// *err: 0 ok, 99 the file did not open, 0x65 a directory chunk did not read.
void read_result_sets(const char* name, const std::int32_t* name_len, std::int32_t* count,
                      std::int32_t* words, const std::int32_t* ncols, const std::int32_t* unused6,
                      std::int32_t* err, const std::int32_t* print);

} // namespace def::dbio::resultset
