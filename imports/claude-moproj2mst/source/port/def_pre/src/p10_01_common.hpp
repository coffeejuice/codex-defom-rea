// Private helpers of the menu 3 / menu 4 sources (packet p10-01).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include <def_pre/ext_p10_01.hpp>
#include <def_pre/forms.hpp>
#include <def_pre/menu7.hpp>
#include <defmodel/layout/cls_14001c770.hpp>
#include <defmodel/options/option_tables.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::pre::p1001 {

using model::field;

// The data block (+8) and the step (+0x10) of a menu record, as raw bytes.
inline std::byte* data(MenuRec* m) { return static_cast<std::byte*>(m->data); }
inline std::byte* step(MenuRec* m) { return static_cast<std::byte*>(m->step); }
inline std::int32_t& i32(MenuRec* m, std::size_t off) { return field<std::int32_t>(m->data, off); }
inline double& f64(MenuRec* m, std::size_t off) { return field<double>(m->data, off); }
// step+0x340, the problem name every menu header shows
inline const char* problem(MenuRec* m) { return &field<char>(m->step, 0x340); }

// p4-01's console helpers take the main object but do not read it; the original passes the menu
// record (param_1) through to them.
inline model::Cls_14001c770& self(MenuRec* m) { return *reinterpret_cast<model::Cls_14001c770*>(m); }

// Element `index` of a static std::string option table whose element 0 is at `table` (0x28
// bytes per element), as `table + index * 0x28` addresses it. The tables are p1-02's constexpr
// copies; an address outside every table reads whatever the original's memory held there, which
// the port cannot reproduce: it gives "" (no case reaches it with in-range keyword values).
inline std::string option_label(std::uint64_t table, std::int64_t index)
{
    const std::uint64_t addr = table + static_cast<std::uint64_t>(index * 0x28);
    for (const auto& t : model::options::kOptionTables) {
        for (const auto& l : t.labels) {
            if (l.object == addr) {
                return std::string(l.text);
            }
        }
    }
    return std::string();
}

// The prompt-then-read idiom: format200 (140002a60) into a 200-byte buffer, copy into a
// std::string, pass it by value.
inline std::int32_t ask_int(MenuRec* m, std::int32_t lo, std::int32_t hi, const char* fmt, std::int32_t v)
{
    char buf[208];
    format200(buf, fmt, v);
    return read_int(self(m), lo, hi, std::string(buf), true);
}
inline double ask_double(MenuRec* m, double lo, double hi, const char* fmt, double v)
{
    char buf[208];
    format200(buf, fmt, v);
    return read_double_range(m, lo, hi, std::string(buf));
}

// The original copies answers with an unbounded strcpy into fixed char fields of the layout
// classes (TITLE 0x51, MTNAME 0x41 bytes); a longer answer runs into the following members, as
// there. The copy goes through the raw bytes so that it does not depend on the member type.
inline void copy_cstr(void* dst, const char* src)
{
    std::memcpy(dst, src, std::strlen(src) + 1);
}

// The first character of an answer (strncpy(c, s, 1): '\0' for an empty answer).
inline char first_char(const std::string& s) { return s.c_str()[0]; }

} // namespace def::pre::p1001
