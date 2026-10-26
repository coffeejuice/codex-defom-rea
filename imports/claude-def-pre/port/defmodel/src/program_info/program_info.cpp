// Program information setters (packet p5-01). Header: defmodel/program_info.hpp.
#include <defmodel/program_info.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::model {

namespace {

// 1401b8560 (ported in defdbio as def::dbio::file_exists, which defmodel cannot call): local
// copy of the same three lines
bool file_readable(const char* name)
{
    if (name == nullptr || std::strlen(name) == 0) return false;
    std::FILE* f = _fsopen(name, "r", 0x40); // 14047b934
    if (f == nullptr) return false;
    std::fclose(f);
    return true;
}

} // namespace

void reset_program_info()
{
    g_info_dim = 2;
    g_program_number = 100;
    std::strcpy(g_program_name, "NEW"); // str 1405deeb0
    std::strcpy(g_version, "1.0");      // str 1405deeb4
    std::strcpy(g_product, "DEFORM");   // str 1405deeb8
}

void set_program_info(const char* name, const char* version, std::int32_t number,
                      const char* product)
{
    set_program_name(name);
    set_version(version);
    set_program_number(number);
    set_product(product);
}

// @src 140224e70
void set_program_name(const char* s) { std::strncpy(g_program_name, s, 0x31); }

// @src 140224f20  version, then " (Service Pack <v>)  " / " (<v>)  " from the VERSION line of
// pack.dat (Beta, RC, SP or CUSTOM in the line pick the second form), when it fits in 99
// @src 1401b9020  (an empty call on the pack.dat name)
void set_version(const char* s)
{
    std::strncpy(g_version, s, 99);
    const char* dir = info_dim() == 2 ? std::getenv("DEFORM_DIR") : std::getenv("DEFORM3_DIR");
    if (dir == nullptr) return;
    char name[1024];
    std::sprintf(name, "%s", dir);
    std::strcat(name, "/PACK/");
    std::strcat(name, "pack.dat");
    if (!file_readable(name)) return;
    std::FILE* f = _fsopen(name, "rt", 0x40);
    if (f == nullptr) return;
    char line[1024];
    char* v = nullptr;
    for (;;) {
        if (std::fgets(line, 0x3ff, f) == nullptr) {
            std::fclose(f);
            return;
        }
        v = std::strstr(line, "VERSION");
        if (v != nullptr) break;
    }
    const std::size_t n = std::strlen(line);
    char* end = line + (n - 1);
    if (*end == '\n') {
        *end = '\0';
        end = line + (n - 2);
    }
    for (v += std::strlen("VERSION"); v < end && *v == ' '; ++v) {
    }
    const char* beta = std::strstr(line, "Beta");
    const char* rc = std::strstr(line, "RC");
    const char* sp = std::strstr(line, "SP");
    const char* custom = std::strstr(line, "CUSTOM");
    char add[1024];
    if (beta == nullptr && rc == nullptr && sp == nullptr) {
        if (custom == nullptr) {
            std::strcpy(add, " (Service Pack ");
            std::strcat(add, v);
        } else {
            for (; v < end && *v == ' '; ++v) {
            }
            std::strcpy(add, " (");
            std::strcat(add, v);
        }
    } else {
        std::strcpy(add, " (");
        std::strcat(add, v);
    }
    std::strcat(add, ")  ");
    if (std::strlen(g_version) + std::strlen(add) < 99) std::strcat(g_version, add);
    std::fclose(f);
}

// @src 140225310
void set_program_number(std::int32_t v) { g_program_number = v; }
// @src 140225330
void set_product(const char* s) { std::strncpy(g_product, s, 99); }
// @src 1402253d0
std::int32_t info_dim() { return g_info_dim; }
// @src 140225450
const char* version() { return g_version; }
// @src 140224dd0
const char* product_name_for_dim(std::int32_t dim) { return dim == 2 ? "DEFORM-2D" : "DEFORM-3D"; }

} // namespace def::model
