// Main menu item 7 (DB generation) and the console input helpers it uses (packet p4-01).
// The VS2008 std::string temporaries of the original (copies, by-value arguments) are plain
// std::string values here; nothing observable depends on them.
#include <def_pre/menu7.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <defdbio/gendb/ext.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/layout/cls_14001c770.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/str_ends_with.hpp>
#include <defrt/stdlib/str_helpers.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <utility>

namespace def::pre {

using model::Cls_14001c770;
using model::field;

namespace {

// 140007460 istream::getline(buf, 256, widen('\n')) on cin (140006c70 / 1400067a0: widen),
// 1400071c0 / 140004ca0: its sentry and buffer reads (the library routines; std::cin here)
void getline256(char* buf)
{
    buf[0] = '\0';
    std::cin.getline(buf, 0x100, '\n');
}

} // namespace

// @src 140002a60  vsprintf_s into a 200-byte buffer
void format200(char* buf, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    // the VS2008 CRT's formatting of doubles (prompts such as " TNOW  =      %lf ? ", p10-01)
    def::rt::crt::vsnprintf(buf, 200, fmt, ap);
    va_end(ap);
}

// 1401b1060: owner p8-05 (defrt str_helpers, p8-int decision)
std::string trim(std::string s) { return def::rt::str_trim(std::move(s)); }

// @src 1401b1390  digits only
bool is_digits(std::string s, std::int32_t start)
{
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(s.c_str()));
    for (std::int32_t i = start; i < n - start; i = i + 1) {
        if (s.c_str()[i] < '0' || '9' < s.c_str()[i]) {
            return false;
        }
    }
    return true;
}

// 1401b0870: owner p8-05 (defrt str_helpers)
std::int32_t to_int(std::string s) { return def::rt::str_to_int(std::move(s)); }

// 1401b0940: owner p8-05 (defrt str_helpers)
bool starts_with(std::string s, const char* p, bool exact) { return def::rt::str_starts_with(std::move(s), p, exact); }

// 1401b0cc0: owner p8-08 (defrt str_ends_with)
// @src 140002800  (string size, inlined)
bool ends_with(std::string s, const char* p) { return def::rt::str_ends_with(std::move(s), p); }

// @src 140009c50  read one answer line
std::string read_line(Cls_14001c770&)
{
    std::fflush(stdout);
    char buf[0x100];
    getline256(buf);
    std::string s = trim(std::string(buf));
    if (s.c_str()[0] == ' ') {
        s.assign(""); // str 140508cd0
    }
    if (ends_with(s, "\n")) { // str 140506938
        s = s.substr(0, s.size() - 1);
    }
    return s;
}

// @src 140008600  read an integer in [lo, hi]
std::int32_t read_int(Cls_14001c770&, std::int32_t lo, std::int32_t hi, const std::string& prompt,
                      bool complain)
{
    while (true) {
        std::fprintf(stdout, "%s", prompt.c_str());
        std::fflush(stdout);
        char buf[0x100];
        getline256(buf);
        std::string s = std::strlen(buf) == 0 ? std::string(prompt.c_str() + 0x17) : std::string(buf);
        s = trim(s);
        if (!is_digits(s, 0)) {
            return -0xf423d;
        }
        const std::int32_t v = to_int(s);
        if (lo <= v && v <= hi) {
            return v;
        }
        if (complain) {
            std::fprintf(stdout, " ??? Invalid Data; Valid Data Range = (     %d to      %d)\n", lo, hi);
        }
    }
}

// @src 140009e90  wait for <CR>
void hit_return()
{
    std::fprintf(stdout, "\n Hit <CR> to continue ...");
    std::fflush(stdout);
    char buf[0x100];
    getline256(buf);
}

// @src 14000a370  menu 7, DB generation
void menu_db_generation(Cls_14001c770& main)
{
    auto& m = main.step_a; // +0x190
    const bool ok = dbio::gendb::ext::object_checks(m); // 1401b9d50
    const char* problem = &field<char>(&main, 0x4d0);
    const char* db_name = &field<char>(&main, 0x3be8);
    char name[0x400];
    if (!dbio::file_exists(db_name)) { // 1401b8560
        std::strncpy(name, problem, std::strlen(problem) + 1);
        if (!ends_with(std::string(problem), ".DB")) { // str 140509204
            std::strcat(name, ".DB");
        }
    }
    else {
        std::strcpy(name, db_name);
    }
    std::int32_t opt = (!dbio::file_exists(name) || field<std::int32_t>(&main, 0x18c8) == 1) ? 2 : 1;
    if (ok) {
        bool done = false;
        std::int32_t trials = 0;
        do {
        again:
            std::fprintf(stdout, " Write step    %d to:  0 - Quit;  1 - Old DB;  2 - New DB\n",
                         field<std::int32_t>(&main, 0xa40));
            char prompt[208];
            format200(prompt, " DB Generation Option =      %d ? ", opt);
            const std::int32_t r = read_int(main, 0, 2, std::string(prompt), true);
            if (r != -0xf423d) {
                opt = r;
            }
            if (opt == 0) {
                return;
            }
            if (opt == 1) {
                std::fprintf(stdout, " Old Database = %s?", name);
                const std::string ans = read_line(main);
                if (!ans.empty()) {
                    std::strcpy(name, ans.c_str());
                }
                if (dbio::gendb::gendb(m, name, field<std::int32_t>(&main, 0x8d0), 1, 1) == 0) {
                    done = true;
                }
            }
            else if (opt == 2) {
                std::string base(problem);
                if (ends_with(base, ".DB")) {
                    base = base.substr(0, base.size() - 3);
                }
                std::fprintf(stdout, " New Database = %s.DB ?", base.c_str());
                const std::string ans = read_line(main);
                if (ans.empty()) {
                    std::sprintf(name, "%s.DB", base.c_str());
                }
                else {
                    std::strcpy(name, ans.c_str());
                }
                if (dbio::file_exists(name)) {
                    std::fprintf(stdout, " WARNING: Database already Exists; Overwriting\n");
                    std::fprintf(stdout, " OK to Overwrite [Y] ? ");
                    const std::string yn = read_line(main);
                    if (starts_with(yn, "n", false)) { // str 14050911c
                        goto again; // back to the question, not counted as a trial
                    }
                }
                std::fprintf(stdout, "   INFO   : Generating DEFORM Database %s\n", name);
                if (dbio::gendb::gendb(m, name, field<std::int32_t>(&main, 0x8d0), 1, 2) == 0) {
                    std::fprintf(stdout, "   INFO   : DEFORM Database Generated\n");
                    done = true;
                }
            }
            trials = trials + 1;
            if (20 < trials) {
                std::fprintf(stdout, "*** Number of trials exceeded the specified limit.\n");
                std::fprintf(stdout, "*** Text-based Pre-processor will be terminated.\n");
                std::exit(-1); // 14047ab18
            }
        } while (!done);
    }
    hit_return();
}

} // namespace def::pre
