// kw_g1b_keyloop.hpp: a test-only KEY reading loop that feeds kw-g1b's keywords from a whole KEY
// file and skips everything else.
//
// The real loop (FUN_1401b6520) and the other groups' parsers belong to other packets. To skip a
// keyword it does not own, this loop drops the rest of the keyword line and then every following
// line until one starts with a registered keyword name or a comment ('*'). That is enough for the
// corpus (data lines are numbers or names) and is only used to position the file for kw-g1b.
// UNIT (id 30, kw-g1a) is applied here because it resets kw-g1b's unit-dependent defaults
// (FUN_1401f4100; the model starts with UNIT 0, as an empty KEY round trip shows).
#pragma once

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>

#include "kw_g1b_fixture.hpp"

namespace kwg1b_test {

// Dimension of a KEY file: the first comment naming DEFORM decides (2D when it says 2D).
inline int key_dimension(const std::string& path)
{
    std::FILE* f = std::fopen(path.c_str(), "r");
    if (!f)
        return 3;
    char line[1024];
    int dim = 3;
    while (std::fgets(line, sizeof line, f)) {
        if (line[0] != '*')
            break;
        if (std::strstr(line, "DEFORM")) {
            dim = (std::strstr(line, "2D") || std::strstr(line, "F2")) ? 2 : 3;
            break;
        }
    }
    std::fclose(f);
    return dim;
}

inline bool line_starts_keyword(const char* line)
{
    while (*line == ' ' || *line == '\t')
        ++line;
    if (*line == '*')
        return true;
    char tok[40] = {};
    int n = 0;
    while (line[n] && !std::isspace(static_cast<unsigned char>(line[n])) && n < 39) {
        tok[n] = line[n];
        ++n;
    }
    if (n == 0)
        return false;
    int group = 0;
    return def::model::lookup_keyword(tok, &group) != def::model::kKeywordNotFound;
}

// Skip the rest of the current line and every data line up to the next keyword/comment.
inline void skip_data(std::FILE* f)
{
    char line[1024];
    if (!std::fgets(line, sizeof line, f))
        return;
    for (;;) {
        long pos = std::ftell(f);
        if (!std::fgets(line, sizeof line, f))
            return;
        if (line_starts_keyword(line)) {
            std::fseek(f, pos, SEEK_SET);
            return;
        }
    }
}

struct LoopResult {
    int g1b_keywords = 0;  // kw-g1b keywords read
    int errors = 0;        // non-zero codes from read_keyword
};

// Reads `f` like FUN_1401b6520 would for kw-g1b's keywords. Error lines go to `log` with the
// loop's texts ("ERROR: Keyword %s Reading Error. \n" / "Data Error").
inline LoopResult run_key_loop(DB_STEP_DATA& m, std::FILE* f, std::FILE* log)
{
    LoopResult r;
    char name[40];
    while (std::fscanf(f, "%39s", name) == 1) {
        if (name[0] == '\0' || name[0] == '*') {
            char rest_of_line[1024];
            if (!std::fgets(rest_of_line, sizeof rest_of_line, f))
                break;
            continue;
        }
        int group = 0;
        int id = def::model::lookup_keyword(name, &group);
        if (id == def::model::kKeywordNotFound) {
            char rest_of_line[1024];
            if (!std::fgets(rest_of_line, sizeof rest_of_line, f))
                rest_of_line[0] = '\0';
            continue;
        }
        if (group == 1 && def::key::g1b::handles(id)) {
            ++r.g1b_keywords;
            int rc = def::key::g1b::read_keyword(m, f, id);
            if (rc == 2) {
                ++r.errors;
                if (log)
                    std::fprintf(log, "ERROR: Keyword %s Reading Error. \n", name);
            } else if (rc == 3) {
                ++r.errors;
                if (log)
                    std::fprintf(log, "ERROR: Keyword %s Data Error. \n", name);
            }
            continue;
        }
        if (id == 30) {  // UNIT (kw-g1a): FUN_14039aa90 case 8
            int v = 0;
            def::key::read_i(f, &v, false);
            if (v != m.sim_ctrl.unit && (v == 1 || v == 2)) {
                m.sim_ctrl.unit = v;
                unit_reset(m);
            }
            continue;
        }
        skip_data(f);
    }
    return r;
}

// The kw-g1b part of a KEY writer output: from "*\n*  Iteration Controls\n*\n" up to the next
// section banner after "User Defined Variables" (or the end). Line ends normalised to "\n".
inline std::string g1b_sections(std::string text)
{
    std::string t;
    t.reserve(text.size());
    for (char c : text)
        if (c != '\r')
            t.push_back(c);
    std::size_t b = t.find("*\n*  Iteration Controls\n*\n");
    if (b == std::string::npos)
        return std::string();
    std::size_t u = t.find("*\n*  User Defined Variables\n*\n", b);
    if (u == std::string::npos)
        return t.substr(b);
    std::size_t e = t.find("\n*\n*  ", u + 10);
    if (e == std::string::npos)
        return t.substr(b);
    return t.substr(b, e + 1 - b);
}

inline std::string read_file(const std::string& path, bool text_mode = false)
{
    std::FILE* f = std::fopen(path.c_str(), text_mode ? "r" : "rb");
    if (!f)
        return std::string();
    std::string s;
    char buf[65536];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0)
        s.append(buf, n);
    std::fclose(f);
    return s;
}

// The same selection as skip_data, done once over the whole text (ftell/fseek on a text-mode
// stream is slow on large KEY files): keeps comments, kw-g1b's and UNIT's keyword lines and the
// lines after them up to the next keyword line; drops other keywords with their data lines.
inline std::string g1b_lines_only(const std::string& text)
{
    std::string out;
    bool keep = false;
    std::size_t i = 0;
    while (i < text.size()) {
        std::size_t e = text.find('\n', i);
        std::size_t next = e == std::string::npos ? text.size() : e + 1;
        std::string line = text.substr(i, next - i);
        if (line_starts_keyword(line.c_str())) {
            const char* p = line.c_str();
            while (*p == ' ' || *p == '\t')
                ++p;
            if (*p == '*') {
                out += line;
                keep = false;
            } else {
                char tok[40] = {};
                std::sscanf(p, "%39s", tok);
                int group = 0;
                int id = def::model::lookup_keyword(tok, &group);
                keep = id == 30 || (group == 1 && def::key::g1b::handles(id));
                if (keep)
                    out += line;
            }
        } else if (keep) {
            out += line;
        }
        i = next;
    }
    return out;
}

// Read a KEY file with run_key_loop into a fresh model and return the written kw-g1b sections.
// The file is read in text mode like the original's (CRLF -> LF), filtered with g1b_lines_only.
inline std::string round_trip(const std::string& key_path, std::FILE* log = nullptr, LoopResult* res = nullptr,
                              int* write_rc = nullptr)
{
    Model model(key_dimension(key_path));
    std::FILE* in = std::fopen(key_path.c_str(), "r");
    if (!in)
        return "<cannot open " + key_path + ">";
    std::fclose(in);
    std::FILE* f = file_from(g1b_lines_only(read_file(key_path, true)));
    LoopResult r = run_key_loop(*model, f, log);
    std::fclose(f);
    if (res)
        *res = r;
    return write_all(*model, write_rc);
}

}  // namespace kwg1b_test
