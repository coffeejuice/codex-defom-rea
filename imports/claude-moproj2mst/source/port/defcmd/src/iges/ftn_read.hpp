// Fortran formatted READ for the IGES library (packet p8-03): the parts of the PathScale runtime
// (OPEN 1404b5054, READ 1404b5cd8, REWIND 1404b5a10, CLOSE 1404b53a4, STOP 1404b5acc) that the
// IGES pass-1 routines use. defrt/ftn/fio.hpp (packet p67-7) has the WRITE side only; the read
// side lives here until fio grows formatted READ (see the p8-03 report).
//
// External unit 7 is read sequentially, one record per line. The runtime opens formatted files in
// text mode: CR LF is read as LF, a lone CR stays in the record; a last line without LF is a
// record. Records shorter than a format are blank padded, longer ones are cut by the format.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

namespace def::cmd::iges::ftnrd {

// OPEN(unit, FILE=name(1:len) (trailing blanks dropped), STATUS='OLD', ACTION='READ', ERR=):
// false on error.
bool open_old(int unit, const char* name, std::size_t len);
// REWIND(unit)
void rewind(int unit);
// CLOSE(unit)
void close(int unit);
// Reads the next record of `unit` into `rec`: 0 ok, 2 end of file.
int read_record(int unit, std::string& rec);
// Closes every unit (tests).
void reset();

// Field i of width w starting at column c (1-based) of `rec`, blank padded.
std::string field(const std::string& rec, int c, int w);

// Iw input: blanks ignored, optional sign, digits. false on an illegal character.
bool read_int(const std::string& f, std::int32_t* out);

// Ew.0 input of the internal string f (all of it is the field): blanks ignored, all blanks 0,
// sign, digits, optional '.', exponent letter E/D/Q (either case) with optional sign, or a
// signed exponent without letter. false on an illegal character.
bool read_real_e0(const std::string& f, double* out);
// The runtime's REAL*8 text conversion (FUN_1404c971c) of a field with the blanks already removed:
// not correctly rounded (19-digit integer cut to 53 bits, one scaling by 10**|e|), see the .cpp.
double ew0_value(const char* s);

// The runtime's fatal error for a READ without ERR=/END= branch: writes the three-line message
// to stderr ("? FORTRAN Runtime Error:" / what / "? READ(UNIT=<unit>,..."), then exits with -1
// (C stdio is flushed, as the oracle shows). `unit_text` is "INTERNAL" or the unit number.
[[noreturn]] void runtime_error(const char* what, const char* unit_text);
// STOP (1404b5acc): exit code 0, C stdio flushed.
[[noreturn]] void stop();

// Test hook: when set, runtime_error/stop call it (with the bytes that would go to stderr, and
// the exit code) instead of exiting; the hook must not return (throw).
extern std::function<void(const std::string& stderr_bytes, int code)> g_exit_hook;

} // namespace def::cmd::iges::ftnrd
