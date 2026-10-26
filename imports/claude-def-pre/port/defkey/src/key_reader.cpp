#define _CRT_SECURE_NO_WARNINGS
#include <defkey/key_reader.hpp>

#include <cstring>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key {

// @src 1401b6520
int read_key_stream(std::FILE* f, KeyReaderHooks& hooks, std::FILE* out)
{
    int status = 0;
    int last_error = 0;
    int group = 0;
    bool dim_set = false;
    char name[40];
    char line[kLineSize];
    // "%s %s" targets: 256-byte stack buffers in the original, uninitialised and kept across
    // comment lines (a one-word comment leaves the second word of an earlier one). Sized to the
    // line here so a long word cannot overflow.
    char word1[kLineSize] = {};
    char word2[kLineSize] = {};
    const bool has_sub = hooks.has_sub_model();
    auto set_dim = [&](int model_dim, int sub_dim) {
        hooks.set_model_dimension(model_dim);
        dim_set = true;
        if (has_sub) hooks.set_sub_model_dimension(sub_dim);
    };

    for (;;) {
        if (crt::fscanf(f, "%39s", name) == -1) return last_error;
        if (std::strlen(name) == 0 || name[0] == '*') {
            const char* got = std::fgets(line, kLineSize, f);
            if (!dim_set && got != nullptr && std::strlen(got) != 0 && *got != '\n') {
                crt::sscanf(line, "%s %s", word1, word2);
                if (std::strcmp(word1, "DEFORM") == 0) {
                    if (std::strcmp(word2, "3D") == 0 || std::strcmp(word2, "F3") == 0)
                        set_dim(3, 2);
                    else if (std::strcmp(word2, "2D") == 0 || std::strcmp(word2, "F2") == 0)
                        set_dim(2, 3);
                } else if (std::strcmp(word1, "DEFORM-3D") == 0 || std::strcmp(word1, "DEFORM-F3") == 0) {
                    set_dim(3, 2);
                } else if (std::strcmp(word1, "DEFORM-2D") == 0 || std::strcmp(word1, "DEFORM-F2") == 0) {
                    set_dim(2, 3);
                } else if (std::strcmp(word1, "DEFORM-MACH2") == 0 ||
                           std::strcmp(word1, "DEFORM-DIESTRESS2") == 0 ||
                           std::strcmp(word1, "DEFORM-MACHDIST2") == 0 ||
                           std::strcmp(word1, "DEFORM-MICROSTRUCTURE2") == 0) {
                    set_dim(2, 3);
                }
            }
            continue;
        }
        const int id = def::model::lookup_keyword(name, &group);
        if (id == def::model::kKeywordNotFound) {
            std::fgets(line, kLineSize, f);
            if (!def::model::unknown_keyword_is_silent(name))
                def::model::print_unknown_keyword_warning(out, name, line);
            continue;
        }
        if (1 <= group && group <= 7)
            status = hooks.parse_keyword(group, f, id);
        else
            status = 3;
        if (status == 0) continue;
        last_error = status;
        if (status == 2) {
            def::rt::crt::fprintf(out, "ERROR: Keyword %s Reading Error. \n", name);
        } else if (status == 3) {
            def::rt::crt::fprintf(out, "ERROR: Keyword %s Data Error. \n", name);
        } else {
            return status;
        }
    }
}

} // namespace def::key
