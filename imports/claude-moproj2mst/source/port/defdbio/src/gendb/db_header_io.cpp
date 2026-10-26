// Fortran-side helpers of the GENDB frame (packet p4-01). Loop counters the Fortran code keeps in
// static storage (DAT_14070e128.., DAT_1407076f0.., DAT_14070dcf4) are locals here: each routine
// sets them before use and nothing else reads them.
#include "db_header_io.hpp"

#include <cstdint>
#include <cstring>
#include <ctime>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/dbcore/util.hpp>
#include <defmodel/port.hpp>
#include <defrt/clock.hpp>
#include <defrt/ftn/helpers.hpp>

namespace def::dbio::gendb {

using namespace def::dbio::dbcore;

namespace {

// Row k (1-based) word w of the integer / real dynamic-array tables (as dbdyn.cpp addresses them)
std::int32_t& int_row(std::int32_t k, std::int32_t w)
{
    return g_db.words()[(0x140b48658 - 0x140b45b80) / 4 + static_cast<std::ptrdiff_t>(k) * 10 + (w - 1)];
}
std::int32_t& real_row(std::int32_t k, std::int32_t w)
{
    return g_db.words()[(0x140b46718 - 0x140b45b80) / 4 + static_cast<std::ptrdiff_t>(k) * 10 + (w - 1)];
}

} // namespace

// @src 1403ae0b6  open the DB unit
void open_db(const char* name, const std::int32_t* len, std::int32_t* unit, const std::int32_t* mode,
             std::int32_t* ierr, const std::int32_t* verbose)
{
    if (*verbose == 1) {
        // list-directed WRITE(*,*) 'OPENING DATABASE FILE = ', name(1:len): no caller passes 1
        DEF_NOT_PORTED(0x1403ae0b6);
    }
    if (*unit < 1 || 0x31 < *unit) {
        *unit = 3;
    }
    const std::int32_t open_mode = 4;
    char status[4];
    char_assign(status, *mode == 1 ? "OLD " : "NEW ", 4, 4); // str 140bafe70 / 140bb0160
    fortran_open(&open_mode, unit, name, status, ierr, 0x400, 3); // 140407e81
    (void)len;
    if (*ierr != 0) {
        *ierr = 99; // the error print that follows needs *verbose == 1 (see above)
    }
}

// @src 1403ae06f  CLOSE(unit) (FUN_1404b53a4 of the Fortran runtime)
void close_db(const std::int32_t* unit)
{
    const std::int32_t mode = 0;
    std::int32_t ios = 0;
    fortran_open(&mode, unit, "", "", &ios, 0, 0);
}

// @src 1403b3f73  directory words of the step to write
void set_step_words(const std::int32_t* mesh, const std::int32_t* step, const std::int32_t* interval,
                    const std::int32_t* sim_no, const std::int32_t* sim_mode,
                    const std::int32_t* family, const std::int32_t* version)
{
    g_db.put_sim_no = *sim_no;
    g_db.put_interval = *interval;
    g_db.put_step = *step;
    g_db.put_mesh_no = *mesh;
    g_db.put_sim_mode = *sim_mode;
    g_db.step_family = *family;
    g_db.step_version = *version;
}

// @src 1404b33e4  CHARACTER assignment with blank padding
void char_assign(char* dst, const char* src, std::int32_t dlen, std::int32_t slen)
{
    if (dlen - slen < 1) {
        std::memcpy(dst, src, static_cast<std::size_t>(dlen));
    }
    else {
        std::memcpy(dst, src, static_cast<std::size_t>(slen));
        std::memset(dst + slen, ' ', static_cast<std::size_t>(dlen - slen));
    }
}

// @src 14044a4b1  ADJUSTL (NUL counts as blank), returns the trimmed length
std::int32_t adjust_left(char* s, std::int32_t n)
{
    const std::int32_t len = n < 0 ? 0 : n;
    auto blank = [](char c) { return c == '\0' || c == ' '; };
    std::int32_t first = 1;
    while (first <= len && blank(s[first - 1])) {
        first = first + 1;
    }
    if (len < first) {
        return 0;
    }
    std::int32_t last = len;
    while (0 < last && blank(s[last - 1])) {
        last = last - 1;
    }
    std::int32_t out = 0;
    for (std::int32_t i = first; i <= last; i = i + 1) {
        out = out + 1;
        s[out - 1] = s[i - 1];
    }
    if (len <= out) {
        return out;
    }
    char_assign(s + out, " ", len - out, 1); // str 140bb1da0
    return out;
}

// @src 1403b400b  record 2 of the file
// @src 140444283  (copy n characters, by 140469eb6 one at a time; inlined)
// @src 140469eb6  (one character)
void set_record2(char* title, const char* smi, const std::int32_t* istart, const std::int32_t* ismtyp,
                 const std::int32_t* nrsets, const std::int32_t* nxtrec, const std::int32_t* w33,
                 const std::int32_t* w34)
{
    const std::int32_t n = adjust_left(title, 0x50);
    if (n < 0x50) {
        char_assign(title + n, " ", 0x50 - n, 1); // str 140bb0420
    }
    std::memcpy(g_db.h.title, title, 0x50);
    std::memcpy(g_db.h.smi1, smi, 8);
    std::memcpy(g_db.h.smi2, smi + 8, 8);
    std::memcpy(g_db.h.smi3, smi + 0x10, 8);
    std::memcpy(g_db.h.smi4, smi + 0x18, 8);
    g_db.h.istart = *istart;
    g_db.h.w33 = *w33;
    g_db.h.nxtrec = *nxtrec;
    g_db.h.ismtyp = *ismtyp;
    g_db.h.w34 = *w34;
    g_db.h.nrsets = *nrsets;
}

// @src 1403bc640  RFREE the dynamic arrays of the tables
void free_dynamic_arrays()
{
    if (g_dyn_words == 0) {
        return;
    }
    const std::int32_t ni = g_db.dyn_hdr[0];
    for (std::int32_t k = 1; k <= ni; k = k + 1) {
        std::int32_t size = int_row(k, 1);
        const std::int32_t idx = int_row(k, 2);
        if (0 < size && 0 < idx) {
            rfree(rm_word(kRmIntBase, idx), &size, &g_dyn_words); // 14043c520
        }
    }
    const std::int32_t nr = g_db.dyn_hdr[10];
    for (std::int32_t k = 1; k <= nr; k = k + 1) {
        const std::int32_t size = real_row(k, 1);
        const std::int32_t idx = real_row(k, 2);
        if (0 < size && 0 < idx) {
            std::int32_t words = size * 2;
            rfree(rm_word(kRmRealBase, idx), &words, &g_dyn_words);
        }
    }
}

// @src 1403bc2c4  set up the dynamic-array tables for the step to write
void set_dynamic_tables(const std::int32_t* ni, const std::int32_t* nr, const std::int32_t* int_sizes,
                        const std::int32_t* real_sizes)
{
    free_dynamic_arrays();
    for (std::int32_t k = 1; k <= 50; k = k + 1) {
        for (std::int32_t w = 1; w <= 10; w = w + 1) {
            int_row(k, w) = 0;
        }
    }
    for (std::int32_t k = 1; k <= 200; k = k + 1) {
        for (std::int32_t w = 1; w <= 10; w = w + 1) {
            real_row(k, w) = 0;
        }
    }
    g_db.dyn_hdr[0] = *ni;
    g_db.dyn_hdr[10] = *nr;
    g_db.dyn_hdr[13] = 2000;
    g_db.dyn_hdr[3] = 500;
    for (std::int32_t k = 1; k <= g_db.dyn_hdr[0]; k = k + 1) {
        int_row(k, 1) = int_sizes[k - 1];
    }
    for (std::int32_t k = 1; k <= g_db.dyn_hdr[10]; k = k + 1) {
        real_row(k, 1) = real_sizes[k - 1];
    }
    alloc_dynamic(); // 1403bc85b
}

// @src 1403bcda9  get / put real dynamic array k
void dynamic_real(const std::int32_t* mode, const std::int32_t* k, double* data, std::int32_t* n,
                  std::int32_t* w2, std::int32_t* w3, std::int32_t* w4, std::int32_t* w5, void*, void*,
                  std::int32_t* ierr)
{
    *ierr = 0;
    if (g_db.dyn_hdr[10] < *k) {
        *ierr = 1;
        return;
    }
    const std::int32_t size = real_row(*k, 1);
    const std::int32_t idx = real_row(*k, 2);
    if (idx == 0) {
        *ierr = 2;
        return;
    }
    if (*mode == 1) {
        *w5 = real_row(*k, 8);
        *n = size;
        *w2 = real_row(*k, 5);
        *w3 = real_row(*k, 6);
        *w4 = real_row(*k, 7);
        def::rt::ftn::copy_words8(rm_word(kRmRealBase, idx), data, n);
    }
    else {
        real_row(*k, 7) = *w4;
        real_row(*k, 8) = *w5;
        real_row(*k, 5) = *w2;
        real_row(*k, 6) = *w3;
        if (*n != size) {
            *ierr = 3;
            return;
        }
        def::rt::ftn::copy_words8(data, rm_word(kRmRealBase, idx), n);
    }
}

// @src 1403bcaa6  get / put int dynamic array k
void dynamic_int(const std::int32_t* mode, const std::int32_t* k, std::int32_t* data, std::int32_t* n,
                 std::int32_t* w2, std::int32_t* w3, std::int32_t* w4, std::int32_t* w5, void*, void*,
                 std::int32_t* ierr)
{
    *ierr = 0;
    if (g_db.dyn_hdr[0] < *k) {
        *ierr = 1;
        return;
    }
    const std::int32_t size = int_row(*k, 1);
    const std::int32_t idx = int_row(*k, 2);
    if (idx == 0) {
        *ierr = 2;
        return;
    }
    if (*mode == 1) {
        *w5 = int_row(*k, 8);
        *n = size;
        *w2 = int_row(*k, 5);
        *w3 = int_row(*k, 6);
        *w4 = int_row(*k, 7);
        def::rt::ftn::copy_words4(rm_word(kRmIntBase, idx), data, n);
    }
    else {
        int_row(*k, 7) = *w4;
        int_row(*k, 8) = *w5;
        int_row(*k, 5) = *w2;
        int_row(*k, 6) = *w3;
        if (*n != size) {
            *ierr = 3;
            return;
        }
        def::rt::ftn::copy_words4(data, rm_word(kRmIntBase, idx), n);
    }
}

// @src 140235800  local date
// @src 140235890  (_time64, inlined)
// @src 140235870  (_localtime64, inlined)
void local_date(std::int32_t* month, std::int32_t* day, std::int32_t* year)
{
    const std::tm t = def::rt::local_now();
    *month = t.tm_mon + 1;
    *day = t.tm_mday;
    *year = t.tm_year;
}

} // namespace def::dbio::gendb
