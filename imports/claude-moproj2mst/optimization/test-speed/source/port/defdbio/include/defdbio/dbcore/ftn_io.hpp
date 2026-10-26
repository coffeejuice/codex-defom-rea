// defdbio/dbcore/ftn_io.hpp: the Fortran I/O that datarm23 runs on (packet p1-12).
//
// The DB is a Fortran direct-access file: OPEN(..., ACCESS='DIRECT', FORM='UNFORMATTED',
// RECL=512) in FUN_140407e81, records of 512 bytes read and written by FUN_14044b62a (INTEGER),
// FUN_14044bc76 (CHARACTER*4) and FUN_14044c2c2 (REAL*8). The port keeps the Fortran unit
// numbers and the by-reference argument style, so that callers ported from the decompile pass
// the same things. The Fortran runtime itself is not ported; what the DB code observes of it is
// reproduced here:
//   * a direct READ of a record that is not completely in the file fails (ERR= taken);
//   * a direct WRITE fills the rest of a short record with zero bytes and extends the file;
//   * OPEN with STATUS='OLD' of a missing file fails; IOSTAT is nonzero on any failure.
// deformdb::RandomAccessFile / WritableFile implement the same 512-byte record access for the
// deformdb reader and writer; they cannot serve here because the DB core updates files in place
// (op 4 rewrites record 2, the index records and the chunk of an existing file).
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

namespace def::dbio::dbcore {

inline constexpr std::int32_t kRecordBytes = 0x200; // RECL=0x200 in FUN_140407e81
// @data 140638020  INTEGER words per record (FUN_14044b62a)
inline constexpr std::int32_t kIntsPerRecord = 128;
// @data 140638024  CHARACTER*4 items per record (FUN_14044bc76)
inline constexpr std::int32_t kChars4PerRecord = 128;
// @data 140638028  REAL*8 words per record (FUN_14044c2c2)
inline constexpr std::int32_t kRealsPerRecord = 64;

// IOSTAT values the port returns. Callers of FUN_140407e81 only test IOSTAT against 0 (or turn
// any failure into their own code), so the values are the port's.
inline constexpr std::int32_t kIosFileNotFound = 29;
inline constexpr std::int32_t kIosOpenFailed = 30;
inline constexpr std::int32_t kIosFileExists = 10;
inline constexpr std::int32_t kIosNotOpen = 101; // CLOSE of a unit that is not open is not an error
inline constexpr std::int32_t kIosNoExist = 0x65; // FUN_140407e81: INQUIRE found no file

// Name as the Fortran runtime uses it: the `len` characters of `name`, trailing blanks removed
// (a NUL ends the name early, as the C strings handed down from the C++ side do).
std::string fortran_name(const char* name, std::int32_t len);

// @src 140407e81  OPEN / CLOSE of a Fortran unit by mode:
//   0 CLOSE(unit, STATUS=status)          1, 2 OPEN formatted sequential
//   3 OPEN FORM='UNFORMATTED'             4, 6 OPEN ACCESS='DIRECT', FORM='UNFORMATTED', RECL=512
//   5 as 4 with ACTION='READ'             7, 8, 9 nothing, IOSTAT=0
//  10 OPEN formatted with ACTION='READ'   other modes: nothing (IOSTAT unchanged)
// For modes 1..6 and STATUS='OLD' a missing file gives IOSTAT=0x65 without an OPEN.
void fortran_open(const std::int32_t* mode, const std::int32_t* unit, const char* file,
                  const char* status, std::int32_t* iostat, std::int32_t file_len,
                  std::int32_t status_len);

// @src 14044b62a  INTEGER block I/O: mode 1 READ, 2 WRITE of `*n` words of `buf` from record
// `*rec` on, 128 words per record. `*rec` is advanced to the last record used; `*err` = 1 on an
// I/O error (the record where it happened stays in `*rec`), 0 otherwise. `*n` < 1 does nothing.
void block_io_int(const std::int32_t* mode, const std::int32_t* unit, void* buf,
                  const std::int32_t* n, std::int32_t* rec, std::int32_t* err);
// @src 14044bc76  same for CHARACTER*4 items, 128 per record
void block_io_char4(const std::int32_t* mode, const std::int32_t* unit, void* buf,
                    const std::int32_t* n, std::int32_t* rec, std::int32_t* err);
// @src 14044c2c2  same for REAL*8 words, 64 per record
void block_io_real8(const std::int32_t* mode, const std::int32_t* unit, void* buf,
                    const std::int32_t* n, std::int32_t* rec, std::int32_t* err);

// Port-side helpers (tests, callers that need to know what is open).
bool unit_is_open(std::int32_t unit);
std::string unit_file(std::int32_t unit);
void close_all_units();

// The original dies inside the Fortran runtime in a few cases (an INQUIRE on an all-blank file
// name). The port prints what the runtime printed and ends the process with the same status;
// tests install a hook that throws instead. Belongs in defrt once it has a Fortran-runtime part.
using RuntimeFaultHook = void (*)(std::uint32_t status);
void set_runtime_fault_hook(RuntimeFaultHook hook);
[[noreturn]] void runtime_fault(std::uint32_t status);
inline constexpr std::uint32_t kStatusAccessViolation = 0xC0000005u;

} // namespace def::dbio::dbcore
