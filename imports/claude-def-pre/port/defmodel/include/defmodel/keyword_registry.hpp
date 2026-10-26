#pragma once
// Keyword registry: the 304-keyword table of DEF_PRE_64.exe and its accessors (0x1402f7680-0x1402f7b10).

#include <cstdio>
#include <string>

// @data 140544400  keyword registry, 304 records x 0x30 (kKeywordTable, generated)
#include "defmodel/keyword_registry_table.hpp"

namespace def::model {

// Id every name lookup returns for an unknown keyword (0x131 = 305; the table has ids 0..303).
inline constexpr int kKeywordNotFound = 0x131;

namespace detail {
constexpr bool c_str_equal(const char* a, const char* b) {
    while (*a != '\0' && *a == *b) {
        ++a;
        ++b;
    }
    return *a == *b;
}
}  // namespace detail

// @src 1402f78d0  FUN_1402f78d0: first record whose name equals `name` (strcmp == 0), scanning in table order.
// No case folding and no trimming.
constexpr const KeywordRecord* find_keyword(const char* name) {
    for (int i = 0; i < kKeywordCount; ++i) {
        if (detail::c_str_equal(name, kKeywordTable[i].name)) return &kKeywordTable[i];
    }
    return nullptr;
}

// @src 1402f7950  FUN_1402f7950: first record whose id equals `id`.
constexpr const KeywordRecord* find_keyword(int id) {
    for (int i = 0; i < kKeywordCount; ++i) {
        if (kKeywordTable[i].id == id) return &kKeywordTable[i];
    }
    return nullptr;
}

// @src 1402f76c0  FUN_1402f76c0: the KEY readers' lookup. Returns the id and stores the group code, or returns
// kKeywordNotFound and stores 0.
constexpr int lookup_keyword(const char* name, int* group) {
    const KeywordRecord* r = find_keyword(name);
    if (r == nullptr) {
        *group = 0;
        return kKeywordNotFound;
    }
    *group = static_cast<int>(r->group);
    return r->id;
}

// @src 1402f7680  FUN_1402f7680: id for a name, or kKeywordNotFound. Nothing in the binary calls it.
constexpr int keyword_id(const char* name) {
    const KeywordRecord* r = find_keyword(name);
    return r == nullptr ? kKeywordNotFound : r->id;
}

// @src 1402f7790  FUN_1402f7790: id of the record at table position `index` when 0 < index < 304, else
// kKeywordNotFound. It indexes by position, not by id, and rejects position 0. Nothing calls it.
constexpr int keyword_id_at(int index) {
    int result = kKeywordNotFound;
    if (0 < index && index < kKeywordCount) result = kKeywordTable[index].id;
    return result;
}

// @src 1402f77e0  FUN_1402f77e0: flag (+0x14) of keyword `id`, 0 if unknown. Nothing calls it.
constexpr int keyword_flag(int id) {
    const KeywordRecord* r = find_keyword(id);
    return r == nullptr ? 0 : r->flag;
}

// @src 1402f7830  FUN_1402f7830: handler slot 1 (+0x18) of keyword `id`, 0 if unknown. Nothing calls it.
constexpr std::uint64_t keyword_handler1(int id) {
    const KeywordRecord* r = find_keyword(id);
    return r == nullptr ? 0 : r->handler1;
}

// @src 1402f7880  FUN_1402f7880: group code of keyword `id`, 0 if unknown. Nothing calls it.
constexpr int keyword_group(int id) {
    const KeywordRecord* r = find_keyword(id);
    return r == nullptr ? 0 : static_cast<int>(r->group);
}

// @src 1402f7720  FUN_1402f7720: name of keyword `id`, or "ERROR: KEYWORD NOT EXISTS!". Like the original it
// strcpy's into one static buffer (DAT_1406fed58) and returns it, so each call overwrites the
// text an earlier call returned. Not thread-safe, on purpose.
const char* keyword_name(int id);

// What FUN_1401b6520 (the KEY reader loop) does with a name the registry does not know, after it
// has read the rest of the line with fgets: it prints a warning unless the name is SRFNRG.
bool unknown_keyword_is_silent(const char* name);

// The same decision in FUN_1403987e0, the other caller of FUN_1402f76c0: it stays silent for any
// name whose first character is '1' or '2', and has no SRFNRG exception.
bool unknown_keyword_is_silent_1403987e0(const char* name);

// The warning both readers print: "WARNING: Keyword %s not found. Skip %s\n" with the name and the
// rest of the line as fgets left it (normally ending in '\n', so the output ends in a blank line).
std::string unknown_keyword_warning(const char* name, const char* rest_of_line);

// fprintf of that warning to `out` (the original writes to stdout, __iob_func() + 0x30).
void print_unknown_keyword_warning(std::FILE* out, const char* name, const char* rest_of_line);

}  // namespace def::model
