#include "defmodel/keyword_registry.hpp"

#include <cstring>

namespace def::model {

namespace {
// DAT_1406fed58: 40 bytes (the next referenced global is at 0x1406fed80).
char g_keyword_name[40];
}  // namespace

// @src 1402f7720  FUN_1402f7720
const char* keyword_name(int id) {
    const KeywordRecord* r = find_keyword(id);
    if (r == nullptr) {
        std::strcpy(g_keyword_name, "ERROR: KEYWORD NOT EXISTS!");
    } else {
        std::strcpy(g_keyword_name, r->name);
    }
    return g_keyword_name;
}

// @src 1401b6520  FUN_1401b6520, the branch after FUN_1402f76c0 returns 0x131.
bool unknown_keyword_is_silent(const char* name) {
    return std::strcmp(name, "SRFNRG") == 0;
}

// @src 1403987e0  FUN_1403987e0, the branch after FUN_1402f76c0 returns 0x131.
bool unknown_keyword_is_silent_1403987e0(const char* name) {
    return name[0] == '1' || name[0] == '2';
}

std::string unknown_keyword_warning(const char* name, const char* rest_of_line) {
    std::string s = "WARNING: Keyword ";
    s += name;
    s += " not found. Skip ";
    s += rest_of_line;
    s += '\n';
    return s;
}

// @src 1401b6520  FUN_1401b6520 / FUN_1403987e0: fprintf(stdout, "WARNING: Keyword %s not found. Skip %s\n", ...)
void print_unknown_keyword_warning(std::FILE* out, const char* name, const char* rest_of_line) {
    std::fprintf(out, "WARNING: Keyword %s not found. Skip %s\n", name, rest_of_line);
}

}  // namespace def::model
