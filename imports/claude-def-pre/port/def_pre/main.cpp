// def_pre: entry point of the C++ port of DEF_PRE_64.exe batch mode.
//
// main FUN_14001c9f0: program information (140224d80), environment (1401b8e10), NEWEQ.MSG and the
// -m utilities (def_pre/m_cli.hpp, which exits when an -m/-d/-o/-h/-r/-s/-t option was given),
// then the TEXT_PRE session: INI / DEF_MULTI.INI driver and the menu loop on stdin
// (def_pre/menu_shell.hpp, packet p5-01).
#include <def_pre/m_cli.hpp>
#include <def_pre/menu_shell.hpp>
#include <defmodel/program_info.hpp>
#include <defrt/stdlib/stdio_compat.hpp>

#include <cstdio>
#include <cstring>
#include "def_pre_version.h"

// part of @src 14001c9f0  main (the -m part is def::pre::main_m, the session def::pre::main_session)
int main(int argc, char** argv)
{
    def::rt::init_stdio_compat(); // printf as the 2014 CRT: E+000 exponents, 1.#INF
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--port-version") == 0) { // port-only option
            std::puts("def_pre port " DEF_PRE_VERSION " (-m utilities, menu shell)");
            return 0;
        }
    }
    def::model::set_program_info("TEXT_PRE", "11.0", 0x44c, "DEFORM-23"); // 140224d80, str 14050cbe4
    def::pre::set_program_environment();                                  // 1401b8e10
    bool handled = false;
    const int code = def::pre::main_m(argc, argv, &handled);
    if (handled) {
        return code;
    }
    return def::pre::main_session(argc, argv);
}
