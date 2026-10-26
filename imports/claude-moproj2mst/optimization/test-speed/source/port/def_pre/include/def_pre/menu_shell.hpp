// def_pre/menu_shell.hpp: main FUN_14001c9f0 without -m, the INI / DEF_MULTI.INI driver, the main
// menu loop and its items 1, 2, 8, 9 (packet p5-01). Specs: docs/port-plan-def-pre-batch.md §2,
// docs/re/commands/FILE_INPUT_MENU.md, MENU9_2D3D.md.
//
// Console input comes from std::cin (the original's istream at PTR_DAT_14063a030), output goes to
// stdout through the C library (printf) except menu 9's question (std::cout).
// Items owned by other packets are called through __has_include seams (menu_shell.cpp):
// 3/4 p10-01 <def_pre/forms.hpp>, 5/6 p10-02, 7 p4-01 <def_pre/menu7.hpp>.
#pragma once

#include <cstdint>
#include <string>

namespace def::model {
class Cls_14001c770;
}

namespace def::pre {

// part of @src 14001c9f0  main after the -m utilities (main_m) declined: builds the session object, the
// banner, the INI driver, the menu loop, NEWEQ.MSG deletion. Returns the exit code.
int main_session(int argc, char** argv);

// @src 1401b8e10  HOME, USER, DEFORM_DIR, DEFORM2_DIR, DEFORM3_DIR, DEFORM23_DIR, DEFORM_USER,
// DEF_HOME_DIR, DEFORM_PC_PATH = the program folder. The original takes the folder of its own
// executable; the port takes the DEFORM install folder: $DEF_PRE_PORT_INSTALL_DIR, else
// C:\Program Files\SFTC\DEFORM\v11.0.2 (where DEF_PRE_64.exe lives).
void set_program_environment();

// @src 140009f20  DEF_MULTI.INI (read, message, exit(0)) or DEF_PRE3.INI / DEF_PRE.INI, then the
// run-type report and "Hit <CR>"
void ini_driver(model::Cls_14001c770& main);

// @src 14001ad90  the main menu loop (at most 101 counted selections)
void menu_loop(model::Cls_14001c770& main);

// @src 14000a200  prints the main menu and returns the selection
std::string main_menu(model::Cls_14001c770& main);

// @src 140007d30  " Enter Selection (%d-%d, R, E, /): " until 140006420 accepts; returns the
// answer trimmed by 1401b1060. range = {lo, hi} of a menu record, read when !use_args.
std::string select_prompt(const std::int32_t* range, bool use_args, std::int32_t lo, std::int32_t hi);
// @src 140006420  "e", "E", "r", "R", "/", a non-number (atoi 0) or lo <= atoi <= hi
bool select_accepts(const std::int32_t* range, std::string s, bool use_args, std::int32_t lo,
                    std::int32_t hi);

// @src 14000ac60  menu 8: "Save Keyword Values in: <PROBID>.KEY", overwrite question, KEY write
void menu_save_key(model::Cls_14001c770& main);

// @src 140014ba0  menu 2 "File Input" (rec = main+0x170: {lo, hi, model, db})
void menu_file_input(model::Cls_14001c770& main);
// @src 14000bac0  menu 2 text and selection
std::string file_input_menu(model::Cls_14001c770& main);
// @src 14000b170  menu 2 option 2: DB step input
void file_input_db(model::Cls_14001c770& main);

// @src 14000afd0  menu 9: asks "Convert 2D to 3D? (y/n) " until N/n/e/E (rec = main+0x188)
void menu_convert_2d3d(model::Cls_14001c770& main);
// @src 14000af30  the question (std::cout)
void convert_2d3d_question();

// @src 14001d1bf  CLOSE(1, STATUS='DELETE'): NEWEQ.MSG is removed at the end of a menu session
void close_neweq_msg();

// @src 140002a90  vsprintf_s into a 100-byte buffer
void format100(char* buf, const char* fmt, ...);

// @data 1406e870c  zeroed by main before the session object is built (no batch reader found)
inline std::int32_t g_1406e870c = 0;

} // namespace def::pre
