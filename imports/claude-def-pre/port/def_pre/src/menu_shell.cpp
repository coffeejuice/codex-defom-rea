// The TEXT_PRE menu shell: main without -m, INI / DEF_MULTI.INI driver, main menu loop, selection
// prompt, menus 1, 2, 8, 9 (packet p5-01). Header: def_pre/menu_shell.hpp.
// Decompiles: deform-rea-work/out/p510/p5.c, out/p501/menus.c; listings out/p501/loop_asm.txt.
#define _CRT_SECURE_NO_WARNINGS
#include <def_pre/menu_shell.hpp>

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>

#include <defcmd/dispatch.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/layout/cls_14001c770.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/str_helpers.hpp>

#if __has_include(<def_pre/menu7.hpp>)
#include <def_pre/menu7.hpp>
#define P501_HAVE_MENU7 1
#endif
#if __has_include(<def_pre/forms.hpp>)
#include <def_pre/forms.hpp>
#define P501_HAVE_FORMS 1
#endif
#include <defcmd/ctc3d/common.hpp> // owner of DAT_14077a7bc (p67-7's statics image)
#if __has_include(<defcmd/launch.hpp>)
#include <defcmd/launch.hpp>
#define P501_HAVE_LAUNCH 1
#endif
#if __has_include(<def_pre/menu56.hpp>)
#include <def_pre/menu56.hpp>
#define P501_HAVE_MENU56 1
#endif

// P5 gate markers (harness/gate/p5_gate.py): defined only while the interim copies are compiled
#ifndef P501_HAVE_MENU7
extern "C" const char p501_interim_140009c50 = 1;
extern "C" const char p501_interim_140009e90 = 1;
extern "C" const char p501_interim_1401b1060 = 1;
extern "C" const char p501_interim_1401b0cc0 = 1;
#endif

namespace def::pre {

using model::Cls_14001c770;
using model::DB_STEP_DATA;
using model::field;

namespace {

// ---- console helpers: p4-01's (def_pre/menu7.hpp) when present, else interim copies ----------
#ifdef P501_HAVE_MENU7
std::string con_trim(std::string s) { return trim(std::move(s)); }
bool con_ends_with(std::string s, const char* p) { return ends_with(std::move(s), p); }
std::string con_read_line(Cls_14001c770& main) { return read_line(main); }
void con_hit_return() { hit_return(); }
#else
#pragma message("P501-INTERIM 140009c50 140009e90 1401b1060 1401b0cc0 compiled (owner p4-01, def_pre/menu7.hpp absent)")

// 140007460 istream::getline(buf, 256) on std::cin (interim, owner p4-01)
void getline256(char* buf)
{
    buf[0] = '\0';
    std::cin.getline(buf, 0x100, '\n');
}
// 1401b1060 (owner p4-01; body in def::rt::str_trim)
std::string con_trim(std::string s) { return def::rt::str_trim(std::move(s)); }
// 1401b0cc0 interim copy (owner p4-01): s strictly longer than p and ends with p, A-Z folded
bool con_ends_with(std::string s, const char* p)
{
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(p));
    if (static_cast<std::int32_t>(s.size()) - n < 1) return false;
    std::string a = s.substr(s.size() - static_cast<std::size_t>(n));
    std::string b(p);
    for (std::int32_t i = 0; i < n; ++i) {
        char& x = a[static_cast<std::size_t>(i)];
        char& y = b[static_cast<std::size_t>(i)];
        if ('@' < x && x < '[') x = static_cast<char>(x + ' ');
        if ('@' < y && y < '[') y = static_cast<char>(y + ' ');
    }
    return std::strncmp(a.c_str(), b.c_str(), static_cast<std::size_t>(n)) == 0;
}
// 140009c50 interim copy (owner p4-01)
std::string con_read_line(Cls_14001c770&)
{
    std::fflush(stdout);
    char buf[0x100];
    getline256(buf);
    std::string s = con_trim(std::string(buf));
    if (s.c_str()[0] == ' ') s.assign("");
    if (con_ends_with(s, "\n")) s = s.substr(0, s.size() - 1);
    return s;
}
// 140009e90 interim copy (owner p4-01)
void con_hit_return()
{
    std::fprintf(stdout, "\n Hit <CR> to continue ...");
    std::fflush(stdout);
    char buf[0x100];
    getline256(buf);
}
#endif

void getline256_cin(char* buf)
{
    buf[0] = '\0';
    std::cin.getline(buf, 0x100, '\n');
}

// 1401b0940 with "n"/"y" (owner p8-05, defrt/stdlib/str_helpers.hpp)
bool answer_starts(const std::string& s, const char* p) { return rt::str_starts_with(s, p, false); }

// 1401b8560 (p1-03's def::dbio::file_exists)
bool file_readable(const char* name) { return dbio::file_exists(name); }

const std::int32_t* rec(Cls_14001c770& main, std::size_t off)
{
    return reinterpret_cast<const std::int32_t*>(reinterpret_cast<std::byte*>(&main) + off);
}

char* problem_id(Cls_14001c770& main) { return main.step_a.u340; } // main+0x4d0

int read_key(DB_STEP_DATA* m, const char* name)
{
    const char reset = 1;
    const std::int32_t flags = 0;
    return cmd::dispatch_hooks().read_keyword_file(m, name, &reset, &flags);
}

// ---- items of other packets ------------------------------------------------------------------
void item_menu3(Cls_14001c770& main)
{
#ifdef P501_HAVE_FORMS
    menu3_sim_controls(reinterpret_cast<MenuRec*>(reinterpret_cast<std::byte*>(&main) + 0x30));
#else
    (void)main;
    DEF_NOT_PORTED(0x140017540);
#endif
}
void item_menu4(Cls_14001c770& main)
{
#ifdef P501_HAVE_FORMS
    menu4_material(reinterpret_cast<MenuRec*>(reinterpret_cast<std::byte*>(&main) + 0xa8));
#else
    (void)main;
    DEF_NOT_PORTED(0x1400157c0);
#endif
}
void item_menu7(Cls_14001c770& main)
{
#ifdef P501_HAVE_MENU7
    menu_db_generation(main);
#else
    (void)main;
    DEF_NOT_PORTED(0x14000a370);
#endif
}
// 1402240d0 (owner p8-07): launch_program(mode, text, dirKind 0)
void launch_dir0(const char* text)
{
#ifdef P501_HAVE_LAUNCH
    cmd::launch_program(1, text, 0);
#else
    (void)text;
    DEF_NOT_PORTED(0x1402240d0);
#endif
}

} // namespace

// ---- menu 5 / 6 seams (p10-02) ------------------------------------------------------------------
// Set by p10-02's code (or tests); null stops with DEF_NOT_PORTED.
using MenuRecFn = void (*)(void* rec);
#ifdef P501_HAVE_MENU56
// Owner p10-02 (def_pre/menu56.hpp).
MenuRecFn g_menu5_object = [](void* rec) { menu5_objects(static_cast<MenuRec*>(rec)); };            // 14001a050(main+8)
MenuRecFn g_menu6_interobject = [](void* rec) { menu6_inter_object(static_cast<MenuRec*>(rec)); }; // 140019c20(main+0xc0)
#else
MenuRecFn g_menu5_object = nullptr;      // 14001a050(main+8)
MenuRecFn g_menu6_interobject = nullptr; // 140019c20(main+0xc0)
#endif

// @src 140002a90
void format100(char* buf, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buf, 100, fmt, ap);
    va_end(ap);
}

// @src 1401b8e10
void set_program_environment()
{
    const char* cfg = std::getenv("DEF_PRE_PORT_INSTALL_DIR");
    char dir[0x110];
    std::snprintf(dir, sizeof dir, "%s",
                  cfg != nullptr && *cfg != '\0' ? cfg : "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2");
    char e[0x110];
    std::sprintf(e, "HOME=%s", dir);
    _putenv(e);
    std::sprintf(e, "USER=%s", dir);
    _putenv(e);
    std::sprintf(e, "DEFORM_DIR=%s", dir);
    _putenv(e);
    (void)std::getenv("DEFORM_DIR"); // read, unused
    std::sprintf(e, "DEFORM2_DIR=%s", dir);
    _putenv(e);
    std::sprintf(e, "DEFORM3_DIR=%s", dir);
    _putenv(e);
    std::sprintf(e, "DEFORM23_DIR=%s", dir);
    _putenv(e);
    std::sprintf(e, "DEFORM_USER=%s", dir);
    _putenv(e);
    std::sprintf(e, "DEF_HOME_DIR=%s", dir);
    _putenv(e);
    std::sprintf(e, "DEFORM_PC_PATH=%s", dir);
    _putenv(e);
}

// @src 14001d1bf  CLOSE(unit, STATUS='DELETE') with unit = |p| (0 -> 6), p = -1 from main; the
// unit is stored like 14001ce7b does (without its DAT_14077a7c0 write)
void close_neweq_msg()
{
    const std::int32_t p = -1;
    std::int32_t unit = -p;
    if (-p < 0) unit = p;
    std::int32_t& msg_unit = def::cmd::ctc3d::dat_14077a7bc;
    msg_unit = unit;
    if (p == 6) return;
    if (unit == 0) {
        unit = 6;
        msg_unit = 6;
    }
    std::remove("NEWEQ.MSG"); // unit 1 is NEWEQ.MSG
}

// @src 140006420
bool select_accepts(const std::int32_t*, std::string s, bool, std::int32_t lo, std::int32_t hi)
{
    if (s == "e" || s == "E") return true;  // str 140508558, 140508554
    if (rt::str_to_int(s) == 0) return true; // 1401b0870
    if (s == "r" || s == "R") return true;  // str 140508550, 14050854c
    if (s == "/") return true;              // str 140508548
    const std::int32_t v = rt::str_to_int(s);
    return lo <= v && rt::str_to_int(s) <= hi;
}

// @src 140007d30
std::string select_prompt(const std::int32_t* range, bool use_args, std::int32_t lo, std::int32_t hi)
{
    std::string out;
    for (;;) {
        if (!use_args) {
            lo = range[0];
            hi = range[1];
        }
        std::fprintf(stdout, " Enter Selection (%d-%d, R, E, /): ", lo, hi);
        std::fflush(stdout);
        char buf[0x100];
        getline256_cin(buf);
        out.assign(buf);
        if (select_accepts(range, out, use_args, lo, hi)) return con_trim(out);
        if (out.empty()) {
            std::fprintf(stdout, "\n Enter:  %d - %d  to Select or Modify\n", lo, hi);
            std::fprintf(stdout, "         E to Exit\n");
            std::fprintf(stdout, "         / to Exit to Main Menu\n");
            std::fprintf(stdout, "         R to Redisplay the Menu\n\n");
        } else {
            std::fprintf(stdout, "\n");
            std::fprintf(stdout, "??? Invalid Input\n");
            std::fprintf(stdout, "\n");
        }
    }
}

// @src 14000a200
std::string main_menu(Cls_14001c770& main)
{
    std::fprintf(stdout, "\n");
    std::fprintf(stdout, "*****  DEFORM Input Preparation Main Menu  *****\n");
    std::fprintf(stdout, "\n");
    std::fprintf(stdout, "   1 - Change Problem ID [%s]\n", problem_id(main));
    std::fprintf(stdout, "   2 - File Input\n");
    std::fprintf(stdout, "   3 - Simulation Controls Modification\n");
    std::fprintf(stdout, "   4 - Material Properties Modification\n");
    std::fprintf(stdout, "   5 - Object Data Modification\n");
    std::fprintf(stdout, "   6 - Inter-Object Data Modification\n");
    std::fprintf(stdout, "   7 - DEFORM Simulation Data Generation\n");
    std::fprintf(stdout, "   8 - Keyword Values Save\n");
    std::fprintf(stdout, "   9 - Convert the 2D DB to 3D keyword file\n");
    std::fprintf(stdout, "\n");
    return select_prompt(rec(main, 0), false, 0, 0);
}

// @src 14001ad90
void menu_loop(Cls_14001c770& main)
{
    bool run = true;
    std::int32_t count = 0;
    do {
        std::string sel = main_menu(main);
        char c = 0;
        std::strncpy(&c, sel.c_str(), 1);
        switch (c) {
        case '/':
        case 'R':
        case 'r':
            continue; // 14001b178: not counted
        case '0':
        case 'E':
        case 'e': {
            std::fprintf(stdout, "\nAre you sure [N]?");
            const std::string a = con_read_line(main);
            if (answer_starts(a, "y")) run = false; // str 140509364
            break;
        }
        case '1': {
            std::fprintf(stdout, "Problem ID = ");
            sel = con_read_line(main);
            if (!sel.empty()) {
                if (con_ends_with(sel, ".DB")) sel = sel.substr(0, sel.size() - 3); // str 140509204
                std::strcpy(problem_id(main), sel.c_str());
            }
            break;
        }
        case '2':
            main.u178 = &main.step_a;
            main.u180 = &main.db_a;
            menu_file_input(main);
            break;
        case '3':
            main.u038 = &main.step_a.sim_ctrl; // main+0x8e0
            main.u040 = &main.step_a;
            item_menu3(main);
            break;
        case '4':
            main.u0b8 = &main.step_a;
            item_menu4(main);
            break;
        case '5':
            main.u018 = &main.step_a;
            if (g_menu5_object == nullptr) DEF_NOT_PORTED(0x14001a050);
            g_menu5_object(reinterpret_cast<std::byte*>(&main) + 8);
            break;
        case '6':
            main.u0d0 = &main.step_a;
            if (g_menu6_interobject == nullptr) DEF_NOT_PORTED(0x140019c20);
            g_menu6_interobject(reinterpret_cast<std::byte*>(&main) + 0xc0);
            break;
        case '7':
            item_menu7(main);
            break;
        case '8':
            menu_save_key(main);
            break;
        case '9':
            menu_convert_2d3d(main);
            break;
        default:
            break;
        }
        ++count;
        if (count > 100) break;
    } while (run);
}

// @src 14000ac60
void menu_save_key(Cls_14001c770& main)
{
    char name[0x400];
    const char* id = problem_id(main);
    std::strncpy(name, id, std::strlen(id) + 1);
    std::strcat(name, ".KEY"); // str 140509310 (4 bytes + NUL from 140509314)
    std::fprintf(stdout, "  Save Keyword Values in: %s\n", name);
    std::fprintf(stdout, "  If OK, Enter <CR>; Otherwise, Enter File Name\n");
    std::fprintf(stdout, "  => ");
    const std::string a = con_read_line(main);
    if (!a.empty()) std::strcpy(name, a.c_str());
    if (file_readable(name)) {
        std::fprintf(stdout, " ERROR  : File exists overwriting : %s\n", name);
        std::fprintf(stdout, " OK to Overwrite [Y] ? ");
        const std::string ok = con_read_line(main);
        if (answer_starts(ok, "n")) return; // str 14050911c
    }
    std::fprintf(stdout, "  *** INFO: Generating New Keyword File %s\n", name);
    const std::uint32_t flags = 0;
    (void)cmd::write_key_file(&main.step_a, name, &flags);
    std::fprintf(stdout, " *** INFO: New Keyword File Generated\n");
}

// @src 14000bac0
std::string file_input_menu(Cls_14001c770& main)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP. -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", static_cast<DB_STEP_DATA*>(main.u178)->u340);
    std::fprintf(stdout, " ***  KEYWORD Input Menu  *** \n\n");
    std::fprintf(stdout, "     1 - Input KEYWORD File\n");
    std::fprintf(stdout, "     2 - Input DEFORM Data Base\n");
    std::fprintf(stdout, "     3 - Input IGES File\n");
    std::fprintf(stdout, "     4 - Input ANSYS/PREP7 File\n");
    std::fprintf(stdout, "     5 - Input STL File\n");
    std::fprintf(stdout, "     6 - Input PATRAN Neutral File\n");
    std::fprintf(stdout, "     7 - Input IDEAS Universal File\n");
    std::fprintf(stdout, "     8 - Input UGII Neutral File\n\n");
    return select_prompt(rec(main, 0x170), false, 0, 0);
}

// @src 140014ba0
void menu_file_input(Cls_14001c770& main)
{
    auto* m = static_cast<DB_STEP_DATA*>(main.u178);
    bool stop = false;
    for (;;) {
        const std::string sel = file_input_menu(main);
        char c = 0;
        std::strncpy(&c, sel.c_str(), 1);
        if (c == '1') {
            do {
                char name[0x400];
                std::strncpy(name, m->u340, std::strlen(m->u340) + 1);
                std::strcat(name, ".KEY");
                if (!file_readable(name)) std::strcpy(name, "?");
                std::fprintf(stdout, "\n Keyword File = [ %s ] ? ", name);
                const std::string a = con_read_line(main);
                if (!a.empty()) std::strcpy(name, a.c_str());
                if (file_readable(name)) {
                    if (!stop) {
                        std::fprintf(stdout, "\n *** INFO: Reading Keyword File\n");
                        (void)read_key(m, name);
                        std::fprintf(stdout, "\n *** INFO: Keyword File Read\n");
                        con_hit_return();
                    }
                    break;
                }
                std::fprintf(stdout, "\n ??? Can not find: %s\n", name);
                std::fprintf(stdout, "\n Re-enter File Name? [Y] ");
                const std::string r = con_read_line(main);
                if (answer_starts(r, "n")) {
                    stop = true;
                    break;
                }
            } while (!stop);
            continue;
        }
        if (c == '2') {
            file_input_db(main);
            continue;
        }
        if (static_cast<unsigned char>(c - '3') < 6) {
            char text[112];
            switch (c) {
            case '3': std::sprintf(text, "DEF_CVT3 KEY IGE"); break;
            case '4': std::sprintf(text, "DEF_CVT3 KEY ANS"); break;
            case '5': std::sprintf(text, "DEF_CVT3 KEY STL"); break;
            case '6': std::sprintf(text, "DEF_CVT3 KEY PDA"); break;
            case '7': std::sprintf(text, "DEF_CVT3 KEY STB"); break;
            default: std::sprintf(text, "DEF_CVT3 KEY UG2"); break;
            }
            launch_dir0(text);
            if (file_readable("SCRATCH.KEY")) (void)read_key(m, "SCRATCH.KEY");
            con_hit_return();
            continue;
        }
        if (c == '/' || c == '0' || c == 'E' || c == 'e') return;
    }
}

// @src 14000b170
void file_input_db(Cls_14001c770& main)
{
    auto* m = static_cast<DB_STEP_DATA*>(main.u178);
    auto* db = static_cast<model::Cls_1401dbfe0*>(main.u180);
    char name[0x400];
    for (;;) {
        std::strncpy(name, m->u340, std::strlen(m->u340) + 1);
        if (!con_ends_with(std::string(m->u340), ".DB")) std::strcat(name, ".DB");
        if (!file_readable(name)) std::strcpy(name, "?");
        std::fprintf(stdout, "\n DEFORM Database = [ %s ] ? ", name);
        const std::string a = con_read_line(main);
        if (!a.empty()) std::strcpy(name, a.c_str());
        if (file_readable(name)) break;
        std::fprintf(stdout, "\n     ERROR  : Unable to Open Database: %s\n", name);
        std::fprintf(stdout, "\n Try Another Database [Y]? ");
        const std::string r = con_read_line(main);
        if (answer_starts(r, "n")) return;
    }
    if (dbio::load_db_info(*db, name, false) != 0) {
        std::fprintf(stdout, "\n     ERROR in reading %s\n", name);
        return;
    }
    const std::string release(reinterpret_cast<const char*>(db) + 0xaa1);
    const std::string first_run(reinterpret_cast<const char*>(db) + 0xaa9);
    const std::string current(reinterpret_cast<const char*>(db) + 0xab1);
    std::fprintf(stdout, "\nINPUT PREP. - DATABASE INPUT -\n");
    std::fprintf(stdout, "-------------------------------------\n");
    std::fprintf(stdout, " [ %s ] \n", m->u340);
    std::fprintf(stdout, "\n ***  DEFORM Database Input  ***\n\n");
    const char* dbname = reinterpret_cast<const char*>(db) + 0x650;
    std::fprintf(stdout, "      Database Name:           %s\n", dbname);
    std::fprintf(stdout, "      Simulation Title:        %s\n\n", reinterpret_cast<const char*>(db) + 0xa50);
    const float ver = static_cast<float>(field<std::int32_t>(db, 0xabc)) / 1000.0f; // DAT_1405094e8
    rt::crt::fprintf(stdout, "      DEFORM System Version:   %.2f\n", static_cast<double>(ver));
    std::fprintf(stdout, "      DEFORM System Release:   %s\n", release.substr(0, 8).c_str());
    std::fprintf(stdout, "      Initial Run Date:        %s\n", first_run.substr(0, 8).c_str());
    std::fprintf(stdout, "      Current Simulation Date: %s\n", current.substr(0, 8).c_str());
    std::fprintf(stdout, "      No. of Result Sets:           %d\n", field<std::int32_t>(db, 0xacc));
    std::fprintf(stdout, "\n\n      Step Numbers:\n");
    std::int32_t def_step = 0;
    std::int32_t printed = 1;
    for (std::int32_t i = 0; i < db->uac8; ++i) {
        const auto* e = static_cast<const std::byte*>(db->uad8) + static_cast<std::size_t>(i) * 0x28;
        if (field<std::int32_t>(e, 0x24) != 0) {
            std::fprintf(stdout, "  %5d", field<std::int32_t>(e, 4));
            if (printed == (printed / 10) * 10) std::fprintf(stdout, "\n");
            ++printed;
            def_step = field<std::int32_t>(e, 4);
        }
    }
    std::fprintf(stdout, "\n\n Step Number = %d ?", def_step);
    std::string ans = con_read_line(main);
    if (ans.empty()) {
        char buf[112];
        format100(buf, "%d", def_step); // str 1405093e0
        ans.assign(buf);
    }
    std::fprintf(stdout, "\n    INFO   : READING STEP %d FROM %s\n", rt::str_to_int(ans), dbname);
    (void)cmd::dispatch_hooks().db_read_step(m, db, rt::str_to_int(ans), 1, 0);
    std::fprintf(stdout, "    INFO   : STEP   %d READ\n", rt::str_to_int(ans));
    con_hit_return();
}

// @src 14000af30
void convert_2d3d_question()
{
    std::cout << '\n';
    std::cout << "Convert 2D to 3D? (y/n) " << '\n';
    std::cout.flush();
}

// @src 14000afd0
void menu_convert_2d3d(Cls_14001c770& main)
{
    for (;;) {
        convert_2d3d_question();
        const std::string a = select_prompt(rec(main, 0x188), true, 0, 0);
        int flag = -1;
        if (a == "Y" || a == "y") flag = 1;
        if (a == "N" || a == "n" || a == "e" || a == "E") flag = 0;
        if (flag == 0) return;
    }
}

// @src 140009f20
void ini_driver(Cls_14001c770& main)
{
    DB_STEP_DATA* m = &main.step_a;
    if (file_readable("DEF_MULTI.INI")) {
        std::fprintf(stdout, "Multiple operation keyword file reading\n");
        (void)read_key(m, "DEF_MULTI.INI");
        std::fprintf(stdout, "Multiple operation keyword processed");
        std::exit(0); // 14047ab18
    }
    std::fprintf(stdout, " *** INFO: Initialize Input Preparation Environment\n");
    if (file_readable("DEF_PRE3.INI")) {
        const std::int32_t d = 3;
        m->set_dimension(&d);
        (void)read_key(m, "DEF_PRE3.INI");
    } else if (file_readable("DEF_PRE.INI")) {
        const std::int32_t d = 2;
        m->set_dimension(&d);
        (void)read_key(m, "DEF_PRE.INI");
    }
    if (m->u1738 == 3) {
        if (m->u1378 < 1)
            std::fprintf(stdout, " ??? WARNING: No Object Defined\n");
        else
            std::fprintf(stdout, " *** INFO: Skip contact generation\n");
    }
    // the three run-type names, indexed by RUNTYP - 1 without a range check (1..3 in practice)
    static const char* const kRunType[3] = {"INITIAL", "CONTINUING", "REMESHING"};
    const std::int32_t rt_index = m->u1738 - 1;
    const char* name = (rt_index >= 0 && rt_index < 3) ? kRunType[rt_index] : "";
    if (rt_index < 0 || rt_index >= 3) DEF_NOT_PORTED(0x140009f20); // out-of-range read of the original
    std::fprintf(stdout, " *** INFO: Ready to Prepare Input for %s Run\n", name);
    if (std::strlen(problem_id(main)) != 0) std::fprintf(stdout, "\n%s\n", problem_id(main));
    con_hit_return();
}

// part of @src 14001c9f0  (main after the -m utilities; the entry is def_pre/main.cpp + main_m)
int main_session(int argc, char**)
{
    g_1406e870c = 0;
    // A 0x9bd0-byte local of main's 0xa868-byte frame in the original: main runs on fresh, zeroed
    // stack pages, so members no constructor writes read 0 there (sim control UNIT at +0x750+0xfc
    // is one). The port builds the object in zeroed storage to keep those values.
    struct Storage {
        void* p;
        Storage() : p(std::calloc(1, sizeof(Cls_14001c770))) {}
        ~Storage() { std::free(p); }
    } storage;
    struct Destroy {
        void operator()(Cls_14001c770* q) const { q->~Cls_14001c770(); }
    };
    std::unique_ptr<Cls_14001c770, Destroy> main(new (storage.p) Cls_14001c770());
    bool banner = true;
    if (file_readable("DEF_MULTI.INI") || file_readable("DEF_PRE3.INI") || file_readable("DEF_PRE.INI"))
        banner = false;
    if (argc < 2 && banner) {
        std::fprintf(stdout, "\n\n\n");
        std::fprintf(stdout, "           ******************************************************\n");
        std::fprintf(stdout, "           *                                                    *\n");
        std::fprintf(stdout, "           *            Welcome to DEFORM-23 System             *\n");
        std::fprintf(stdout, "           *                                                    *\n");
        std::fprintf(stdout, "           *              Input Preparation Module              *\n");
        std::fprintf(stdout, "           *                                                    *\n");
        std::fprintf(stdout, "           *                   Version %5s                    *\n", "11.0"); // str 14050cbe4
        std::fprintf(stdout, "           *                                                    *\n");
        std::fprintf(stdout, "           ******************************************************\n");
        std::fprintf(stdout, "\n\n\n");
    }
    ini_driver(*main);
    menu_loop(*main);
    close_neweq_msg();
    return 0;
}

} // namespace def::pre
