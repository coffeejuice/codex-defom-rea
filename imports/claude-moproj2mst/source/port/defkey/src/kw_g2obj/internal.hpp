// kw-g2obj internals: raw field access at recovered offsets, the dimension tests, and forwards to
// the shared KEY I/O helpers.
//
// LLOBJ_DATA keeps its property (0x1ea0) and movement (0x2160) sub-structures opaque
// (defmodel/layout/llobj_data.hpp, owned by the skeleton thread), so the readers and writers
// here address them by their offsets, exactly as the decompile does: at<double>(obj, 0x1ea8) is
// *(double*)(obj + 0x1ea8). The offsets are the ones in docs/re/keywords/<KW>.md.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::key::g2obj {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::LLOBJ_DATA;

template <class T, class C>
inline T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&base) + off);
}
template <class T, class C>
inline const T& at(const C& base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&base) + off);
}
inline DYNARY* table_at(LLOBJ_DATA& obj, std::size_t off) { return &at<DYNARY>(obj, off); }

// The model is addressed by offset too (its member names are still being settled by p1-03).
// FUN_1400015e0 / FUN_1400015f0: model+0x1c == 2 / == 3.
inline bool is_2d(const DB_STEP_DATA& m) { return at<std::int32_t>(m, 0x1c) == 2; }
inline bool is_3d(const DB_STEP_DATA& m) { return at<std::int32_t>(m, 0x1c) == 3; }

// model+0x1744: the flags FUN_1401c5810 stores for the KEY writer.
inline std::uint32_t writer_flags(const DB_STEP_DATA& m) { return at<std::uint32_t>(m, 0x1744); }

namespace io {

// Forwards to the shared KEY I/O helpers (key-common packet: defkey/line_io.hpp,
// defkey/table_io.hpp), under the helper names this packet's code uses.
inline int read_text_line2(std::FILE* f, char* dst, int n) { return read_string_next_line(f, dst, n); }  // 140413ab0
inline int read_i(std::FILE* f, std::int32_t* a, bool skip_blank) { return def::key::read_i(f, a, skip_blank); }  // 140413de0
inline int read_ii(std::FILE* f, std::int32_t* a, std::int32_t* b) { return def::key::read_ii(f, a, b); }  // 140413ee0
inline int read_d(std::FILE* f, double* a) { return def::key::read_d(f, a); }  // 140414990
inline int read_dd(std::FILE* f, double* a, double* b) { return def::key::read_dd(f, a, b); }  // 140414a40
inline int read_d3(std::FILE* f, double* a, double* b, double* c) { return def::key::read_d3(f, a, b, c); }  // 140414c20
inline int read_d4(std::FILE* f, double* a, double* b, double* c, double* d) { return def::key::read_d4(f, a, b, c, d); }  // 140414e40
inline int read_d6(std::FILE* f, double* a, double* b, double* c, double* d, double* e, double* g)  // 140415080
{
    double* const out[6] = {a, b, c, d, e, g};
    return def::key::read_d6(f, out);
}
inline int read_d7(std::FILE* f, double* a, double* b, double* c, double* d, double* e, double* g,
                   double* h)  // 1404151c0
{
    double* const out[7] = {a, b, c, d, e, g, h};
    return def::key::read_d7(f, out);
}
inline int read_id(std::FILE* f, std::int32_t* a, double* b) { return read_i_d(f, a, b); }  // 1404154c0
inline int read_idd(std::FILE* f, std::int32_t* a, double* b, double* c) { return read_i_dd(f, a, b, c); }  // 140415680
inline int read_id3(std::FILE* f, std::int32_t* a, double* b, double* c, double* d) { return read_i_d3(f, a, b, c, d); }  // 140415880
inline int read_id7(std::FILE* f, std::int32_t* a, double* b, double* c, double* d, double* e,
                    double* g, double* h, double* k)  // 140415d70
{
    double* const out[7] = {b, c, d, e, g, h, k};
    return read_i_d7(f, a, out);
}
inline int read_iid(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c) { return read_ii_d(f, a, b, c); }  // 140415f00
inline int read_iid4(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c, double* d, double* e,
                     double* g)  // 140416220
{
    double* const out[4] = {c, d, e, g};
    return read_ii_d4(f, a, b, out);
}
inline int tokenize_line(std::FILE* f, std::int32_t* count, int max_tok) { return read_tokens(f, count, max_tok); }  // 140413900
inline const char* token(int i) { return def::key::token(i); }  // 140413a50
inline std::int32_t token_int(int i) { return def::key::token_int(i); }  // 14047b6c0(140413a50(i))
inline double token_double(int i) { return def::key::token_double(i); }  // 14048754c(140413a50(i))
inline int table_read(std::FILE* f, DYNARY* t) { return read_table_body(f, t, 0); }  // 14041a800
inline int table_write(std::FILE* f, DYNARY* t, int mode) { return write_table_body(f, t, mode); }  // 1402f3130

} // namespace io
} // namespace def::key::g2obj
