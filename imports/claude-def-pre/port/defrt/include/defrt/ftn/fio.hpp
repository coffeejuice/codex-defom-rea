// defrt/ftn/fio.hpp: the parts of the PathScale Fortran I/O runtime (FUN_1404b5ef4 WRITE,
// FUN_1404b5054 OPEN, FUN_1404b53a4 CLOSE, READ, REWIND) that the ported Fortran routines use
// (first user: packet p67-7, the GENCTC 3D contact engine; the OBJPOS drop solver p67-3 can share
// it).
//
// One object per WRITE statement; the record is written when end() is called:
//
//     fio::Write(6).a("Interference detected, KOBJ=").i(kobj).end();           // WRITE(6,*)
//     fio::Write(1, "(' Info: Node ',I7,' of Object ',I6)").i(n).i(obj).end();  // WRITE(1,1001)
//
// Units: 6 is standard output, written straight to file descriptor 1, one write per record (the
// runtime does not share the C library's stdout buffer, so its records appear where the C
// buffer has been flushed, as in the original). Records end with "\n" on unit 6 and "\r\n" on
// files (both seen in the oracle's outputs). Unit 1 is NEWEQ.MSG (opened by main, deleted at
// exit); its records are appended with the file closed between statements, so that the file can
// be removed at any time. Any other unit must be opened with open_old / open_unknown first.
//
// Formatted output supports the edit descriptors the ported formats use: nX, Iw, Fw.d, Ew.d, A,
// Aw, '...' / "..." literals, '/', repeat counts (3F15.7) and parenthesised groups; integer and
// real fields overflow to '*'. List-directed output follows the PathScale layout as far as it has
// been checked (status in docs/re/status/p67-7.tsv): the record starts with one blank, character
// items are copied as they are, an integer is written as a blank separator, a sign position and
// the digits.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace def::rt::ftn::fio {

class Write {
public:
    explicit Write(int unit);                // list-directed WRITE(unit,*)
    Write(int unit, const char* format);     // formatted WRITE(unit,fmt), format with parentheses
    Write& i(std::int32_t v);                // INTEGER*4 item
    Write& d(double v);                      // REAL*8 item
    Write& r(float v);                       // REAL*4 item
    Write& a(const char* s);                 // CHARACTER item (NUL-terminated)
    Write& a(const char* s, std::size_t len); // CHARACTER*len item
    void end();                              // write the record(s)

    // The text end() would write (for tests), without the line ends.
    std::string text() const;

private:
    struct Item {
        char kind; // 'i', 'd', 'r', 'a'
        std::int64_t iv;
        double dv;
        std::string sv;
    };
    int unit_;
    std::string format_;
    bool list_;
    std::vector<Item> items_;

    std::string render() const;
};

// OPEN(UNIT=u, FILE=name, STATUS='OLD', ERR=...): true when the file exists and was opened.
bool open_old(int unit, const char* name);
// OPEN(UNIT=u, FILE=name, STATUS='UNKNOWN'): creates or opens for writing (position at start).
bool open_unknown(int unit, const char* name);
// CLOSE(u). A unit whose last operation was a WRITE ends there: the file is truncated at the
// current position (sequential ENDFILE on close), so rewriting a longer old file leaves no tail.
void close(int unit);
// REWIND(u)
void rewind(int unit);
// READ(u,*) n values of INTEGER*4: returns the IOSTAT-style status (0 ok, 1 end of file,
// 2 error). Values not reached keep their old contents.
int read_list_i32(int unit, std::int32_t* out, int n);

// Formats one Iw / Fw.d / Ew.d field as the runtime does (exposed for tests).
std::string format_i(std::int64_t v, int w);
std::string format_f(double v, int w, int dd);
std::string format_e(double v, int w, int dd);

// Closes every unit (tests).
void reset();

// STOP without a stop code (FUN_1404b5acc called with the blank code ' ', as in MDSPS2_REC): the
// runtime prints " STOP " only when its option bit DAT_14072fba0 & 0x20 is set (0 in the image,
// no writer in the engine) and the code only when it is not blank, so nothing is printed; then
// every unit is closed and the process exits with status 0 (FUN_1404b8b88 -> FUN_1404b9e88(0)).
[[noreturn]] void stop();
// STOP [code] (FUN_1404b5acc): writes the stop code (when it is not a single blank) and CR LF to
// standard output, closes every unit and ends the process with exit status 0. The runtime's
// optional " STOP " prefix (an I/O flag word bit 0x20) is not reproduced.
[[noreturn]] void stop(const char* code, std::size_t len);

} // namespace def::rt::ftn::fio
