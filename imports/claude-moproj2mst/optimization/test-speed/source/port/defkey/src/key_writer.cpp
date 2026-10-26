#define _CRT_SECURE_NO_WARNINGS
#include <defkey/key_writer.hpp>

#include <defmodel/keyword_registry.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key {

// part of @src 1403d4990 / 1401c5af0 / 1401c5a20 (inline fprintf in each)
void write_section_banner(std::FILE* f, const char* title) { def::rt::crt::fprintf(f, "*\n*  %s\n*\n", title); }

// @src 1401c5a20
void write_file_banner(std::FILE* f, unsigned dim_option, const char* product, const char* version)
{
    char buf[1024];
    if (dim_option == 0)
        def::rt::crt::snprintf(buf, sizeof buf, "%s V%s KEYWORD FILE (Qt)", product, version);
    else
        def::rt::crt::snprintf(buf, sizeof buf, "%s%dD V%s KEYWORD FILE (Qt)", "DEFORM-", static_cast<int>(dim_option), version);
    def::rt::crt::fprintf(f, "*\n*  %s\n*\n", buf);
}

const char* keyword_label(int id) { return def::model::keyword_name(id); }

} // namespace def::key
