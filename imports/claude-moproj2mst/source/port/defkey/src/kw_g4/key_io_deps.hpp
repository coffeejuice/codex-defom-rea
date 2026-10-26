// kw_g4/key_io_deps.hpp: the shared KEY I/O helpers (key-common: defkey/line_io.hpp,
// table_io.hpp, crt_scan.hpp) under the names the group 4 code uses, one inline forward each,
// with the address it stands for.
#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>

namespace def::key::g4dep {

using def::model::DYNARY;

// ---- line helpers: one fgets(0x400), ',' -> ' ', outputs zeroed, sscanf. 1 = EOF or blank -----
inline int line_i(std::FILE* f, std::int32_t* a, char skip_blank) { return read_i(f, a, skip_blank != 0); } // 140413de0
inline int line_ii(std::FILE* f, std::int32_t* a, std::int32_t* b) { return read_ii(f, a, b); }             // 140413ee0
inline int line_iii(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, std::int32_t* n_out)
{
    return read_iii(f, a, b, c, n_out); // 140413fb0
}
inline int line_d(std::FILE* f, double* a) { return read_d(f, a); }                  // 140414990
inline int line_dd(std::FILE* f, double* a, double* b) { return read_dd(f, a, b); } // 140414a40
inline int line_d4(std::FILE* f, double* a, double* b, double* c, double* d)
{
    return read_d4(f, a, b, c, d); // 140414e40
}
inline int line_d6(std::FILE* f, double* a, double* b, double* c, double* d, double* e, double* g)
{
    double* const out[6] = {a, b, c, d, e, g};
    return read_d6(f, out); // 140415080
}
inline int line_id(std::FILE* f, std::int32_t* t, double* a) { return read_i_d(f, t, a); } // 1404154c0
inline int line_idd(std::FILE* f, std::int32_t* t, double* a, double* b)
{
    return read_i_dd(f, t, a, b); // 140415680
}
inline int line_id4(std::FILE* f, std::int32_t* t, double* a, double* b, double* c, double* d)
{
    double* const out[4] = {a, b, c, d};
    return read_i_d4(f, t, out); // 140415990
}
// 140413ab0: discards the rest of the line, reads the next one into dst (size-1 chars, trimmed).
inline int line_string(std::FILE* f, char* dst, std::int32_t size) { return read_string_next_line(f, dst, size); }

// ---- tables -------------------------------------------------------------------------------------
// 14041b270: "t a b c" header (re-init type 0), extra value, then the body.
inline int read_table(std::FILE* f, DYNARY* t, std::int32_t want_extra, double* extra)
{
    return read_table_header(f, t, want_extra, extra);
}
// 14041a800 is def::key::read_table_body, 1402f3130 def::key::write_table_body.
// 1402f2e80: count values, " %20.10E" each, "\n" + 8 spaces before every per_line-th value.
inline int write_values(std::FILE* f, std::int32_t count, const double* v, std::int32_t per_line, char alt)
{
    return write_doubles(f, count, v, per_line, alt != 0);
}
// 1402f3aa0: YOUNG-style header line, then the body.
inline int write_table(std::FILE* f, const char* name, std::int32_t mat, const DYNARY* t,
                       std::int32_t has_extra, const double* extra)
{
    return write_table_header_d(f, name, mat, t, has_extra, extra);
}
// 1402f40f0: HEATCP-style header line (int extra), then the body.
inline int write_table_iextra(std::FILE* f, const char* name, std::int32_t mat, const DYNARY* t,
                              std::int32_t has_extra, const std::int32_t* extra)
{
    return write_table_header_i(f, name, mat, t, has_extra, extra);
}

// ---- CRT: the binary's scan formats ("%i", "%Flf", "%i %Flf") as its CRT read them -----------
inline int key_fscanf(std::FILE* f, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vfscanf(f, fmt, ap);
    va_end(ap);
    return r;
}
inline int key_sscanf(const char* s, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vsscanf(s, fmt, ap);
    va_end(ap);
    return r;
}

// ---- model --------------------------------------------------------------------------------------
// 1400015f0 (p1-01, inlined as in p1-03's sim_ctrl.cpp): dimension word (+0x1c) == 3.
inline bool model_is_3d(const def::model::DB_STEP_DATA* m) { return m->dimension == 3; }

} // namespace def::key::g4dep
