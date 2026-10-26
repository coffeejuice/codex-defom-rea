// Main menu 3 "Simulation Controls Modification" (packet p10-01): FUN_140017540 and its five
// forms. The record m has data = the simulation-control block S (DB_STEP_DATA+0x750) and
// step = the DB_STEP_DATA; offsets below are S+x (layout/cls_1401e7b10.hpp).
//
// Quirks of the original kept as they are (from the disassembly):
// * the forms' item numbers follow the 3D list; the 2D lists number differently, so in 2D the
//   number shown is not the item edited (e.g. "4 OSCTRL" edits NSTEP);
// * Stopping "5" (EMAX) shows and stores S+0x640 (TNOW), not S+0x170 (14000e3ea / 14000e483);
// * Stopping "15" (DTPMAX) stores its three answers all at S+0x270 (14000ed51..14000ed75);
// * Iteration "6" (SOLMTD) lists the iteration methods and edits S+0x2e0 (ITRMTH);
// * the 3D SMAX/VMIN/LMAX rows end in "%\f": the VS2008 CRT drops the '%' of a conversion whose
//   type character is outside its format class table and prints the character (the oracle
//   shows "\t\f"), so the port prints "\f" directly and never passes the third value;
// * the selection ranges come from the records the main object's ctor sets (main+0x30 {1,5},
//   +0x48 {1,5}, +0x60 {1,17}, +0x78 {1,10}, +0x90 {1,5}), so some items cannot be selected.
#include "p10_01_common.hpp"

#include <cstdio>
#include <iostream>

#include <defrt/stdlib/legacy_printf.hpp>

// 1401f3ff0 set_smode is kw-g1a's (a private header of defkey)
#include "../../defkey/src/kw_g1a/sim_ctrl_ops.hpp"

namespace def::pre {

using namespace p1001;

namespace {

void header(MenuRec* m, bool blank)
{
    if (blank) {
        std::fprintf(stdout, "\n\n");
    }
    std::fprintf(stdout, " INPUT PREP. - SIMULATION CONTROL -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
}

void table_head()
{
    std::fprintf(stdout, " Number  Keywrd  Values\n");
    std::fprintf(stdout, " ======  ======  ===============================\n");
}

bool is_3d(MenuRec* m) { return field<std::int32_t>(m->step, 0x1c) == 3; }

} // namespace

// @src 1400017f0  USRDEF display
static void show_usrdef(MenuRec* m)
{
    header(m, false);
    std::fprintf(stdout, "\n ***  User Defined Variables (USRDEF)  ***\n");
    std::fprintf(stdout, " [\n");
    for (std::size_t i = 0; i < 10; ++i) {
        std::fprintf(stdout, " %s\n", reinterpret_cast<const char*>(data(m) + 0x688 + i * 0x51));
    }
    std::fprintf(stdout, " ]\n");
}

// @src 1400018e0  Processing Conditions display
static void show_processing(MenuRec* m)
{
    header(m, true);
    std::fprintf(stdout, "  ***  Processing Conditions Modification Menu  ***\n\n");
    table_head();
    rt::crt::fprintf(stdout, "   1     GEOERR   %lf\t%lf\t%lf\t%lf\n", f64(m, 0x660), f64(m, 0x668), f64(m, 0x670),
            f64(m, 0x678));
    rt::crt::fprintf(stdout, "   2     PENINF   %lf\n", f64(m, 0x3a8));
    rt::crt::fprintf(stdout, "   3     UNTE2H   %lf\n", f64(m, 0x3b0));
    rt::crt::fprintf(stdout, "   4     TINTGF   %lf\n", f64(m, 0x3b8));
    print_dynary_row(field<model::DYNARY>(m->data, 0x3c8), 5, "ENVTMP");
    rt::crt::fprintf(stdout, "   6     BLZMAN   %lf\n", f64(m, 0x4b8));
    print_dynary_row(field<model::DYNARY>(m->data, 0x4c0), 7, "CNVCOF");
    std::fprintf(stdout, "\n");
}

// @src 140003520  Process Definition display
static void show_process(MenuRec* m)
{
    header(m, true);
    std::fprintf(stdout, "  ***  Process Definition Modification Menu  ***\n\n");
    table_head();
    std::fprintf(stdout, "   1     TITLE   %s\n", reinterpret_cast<const char*>(data(m)));
    if (is_3d(m)) {
        std::fprintf(stdout, "   2     SMODE   %s\n", option_label(0x1405be8c0, i32(m, 0xf4) - 1).c_str());
        std::fprintf(stdout, "   3     MESHNO  %d\n", i32(m, 0xf0));
        std::fprintf(stdout, "   4     UNIT    %s\n", option_label(0x1405be960, i32(m, 0xfc) - 1).c_str());
        rt::crt::fprintf(stdout, "   5     TNOW    %lf\n\n", f64(m, 0x640));
        return;
    }
    std::fprintf(stdout, "   2     CURSIM  %d\n", i32(m, 0xe4));
    std::fprintf(stdout, "   3     SIMNAM  %s\n", reinterpret_cast<const char*>(data(m) + 0x92));
    std::fprintf(stdout, "   4     STYPE   %s\n", option_label(0x1405bf180, i32(m, 0xf8) - 1).c_str());
    std::fprintf(stdout, "   5     SMODE   %s\n", option_label(0x1405be8c0, i32(m, 0xf4) - 1).c_str());
    std::fprintf(stdout, "   6     GEOTYP  %s\n", option_label(0x1405bf200, i32(m, 0x100) - 1).c_str());
    std::fprintf(stdout, "   7     MESHNO  %d\n", i32(m, 0xf0));
    std::fprintf(stdout, "   8     UNIT    %s\n", option_label(0x1405be960, i32(m, 0xfc) - 1).c_str());
    rt::crt::fprintf(stdout, "   9     TNOW    %lf\n\n", f64(m, 0x640));
}

// @src 140003880  Stopping & Step Controls display
static void show_stopping(MenuRec* m)
{
    header(m, true);
    std::fprintf(stdout, " ***  Stopping & Step Controls Modification Menu  ***\n\n");
    table_head();
    std::fprintf(stdout, "   1     NSTART  %d\n", i32(m, 0x160));
    std::fprintf(stdout, "   2     STPINC  %d\n", i32(m, 0x164));
    if (is_3d(m)) {
        std::fprintf(stdout, "   3     PDIE    %d\n", i32(m, 0x168));
        std::fprintf(stdout, "   4     NSTEP   %d\n", i32(m, 0x16c));
        rt::crt::fprintf(stdout, "   5     EMAX    %lf\n", f64(m, 0x170));
        rt::crt::fprintf(stdout, "   6     TMAX    %lf\n", f64(m, 0x178));
        // str 140507800.. "...%lf\t%lf\t%\f\n" (see the file comment)
        rt::crt::fprintf(stdout, "   7     SMAX    %lf\t%lf\t\f\n", f64(m, 0x180), f64(m, 0x188));
        rt::crt::fprintf(stdout, "   8     VMIN    %lf\t%lf\t\f\n", f64(m, 0x198), f64(m, 0x1a0));
        rt::crt::fprintf(stdout, "   9     LMAX    %lf\t%lf\t\f\n", f64(m, 0x1b0), f64(m, 0x1b8));
        std::fprintf(stdout, "  10     STPDEF  %s\n", option_label(0x1405be9b0, i32(m, 0x1c8) - 1).c_str());
        rt::crt::fprintf(stdout, "  11     DEMAX   %lf\n", f64(m, 0x260));
        rt::crt::fprintf(stdout, "  12     DSMAX   %lf\n", f64(m, 0x1e0));
        rt::crt::fprintf(stdout, "  13     DTMAX   %lf\t\n", f64(m, 0x1d0));
        rt::crt::fprintf(stdout, "  14     SLDERR  %lf\n", f64(m, 0x268));
        rt::crt::fprintf(stdout, "  15     DTPMAX  %lf\t%lf\t%lf\n", f64(m, 0x270), f64(m, 0x2a0), f64(m, 0x2a8));
        rt::crt::fprintf(stdout, "  16     DVMAX   %lf\t%lf\n", f64(m, 0x278), f64(m, 0x280));
        rt::crt::fprintf(stdout, "  17     DTSUB   %lf\n\n", f64(m, 0x2c0));
        return;
    }
    std::fprintf(stdout, "   3     PDIE    %d    %d\n", i32(m, 0x168), i32(m, 0x9b4));
    std::fprintf(stdout, "   4     OSCTRL  %d    %d      %d     %d\n", i32(m, 0x9b8), i32(m, 0x9bc), i32(m, 0x9c0),
                 i32(m, 0x9c4));
    std::fprintf(stdout, "   5     NSTEP   %d\n", i32(m, 0x16c));
    rt::crt::fprintf(stdout, "   6     EMAX    %lf\n", f64(m, 0x170));
    rt::crt::fprintf(stdout, "   7     TMAX    %lf\n", f64(m, 0x178));
    rt::crt::fprintf(stdout, "   8     SMAX    %lf\t%lf\t\n", f64(m, 0x180), f64(m, 0x188));
    rt::crt::fprintf(stdout, "   9     VMIN    %lf\t%lf\t\n", f64(m, 0x198), f64(m, 0x1a0));
    rt::crt::fprintf(stdout, "  10     LMAX    %lf\t%lf\t\n", f64(m, 0x1b0), f64(m, 0x1b8));
    std::fprintf(stdout, "  11     STPDEF  %s\n", option_label(0x1405be9b0, i32(m, 0x1c8) - 1).c_str());
    rt::crt::fprintf(stdout, "  12     DEMAX   %lf\n", f64(m, 0x260));
    rt::crt::fprintf(stdout, "  13     DSMAX   %lf\n", f64(m, 0x1e0));
    rt::crt::fprintf(stdout, "  14     DTMAX   %lf\t\n", f64(m, 0x1d0));
    rt::crt::fprintf(stdout, "  15     SLDERR  %lf\n", f64(m, 0x268));
    rt::crt::fprintf(stdout, "  16     DTPMAX  %lf\t%lf\t%lf\n", f64(m, 0x270), f64(m, 0x2a0), f64(m, 0x2a8));
    rt::crt::fprintf(stdout, "  17     DVMAX   %lf\t%lf\n", f64(m, 0x278), f64(m, 0x280));
    rt::crt::fprintf(stdout, "  18     MDSOBJ  %d\t%d\t%d\t%lf\n\n", i32(m, 0x288), i32(m, 0x28c), i32(m, 0x290),
            f64(m, 0x298));
}

// @src 140005520  Iteration Controls display
static void show_iteration(MenuRec* m)
{
    header(m, true);
    std::fprintf(stdout, "  ***  Iteration Controls Modification Menu  ***\n\n");
    table_head();
    rt::crt::fprintf(stdout, "   1     CVGERR   %lf\t%lf\n", f64(m, 0x2d0), f64(m, 0x2d8));
    std::fprintf(stdout, "   2     INIGES   N/A\n");
    std::fprintf(stdout, "   3     ITRMTH   %s\n", option_label(0x1405bea30, i32(m, 0x2e0) - 1).c_str());
    std::fprintf(stdout, "   4     ITRMXD   %d\n", i32(m, 0x310));
    std::fprintf(stdout, "   5     DEFBWD   %s\n", option_label(0x1405beab0, i32(m, 0x314)).c_str());
    const std::int32_t solmtd = i32(m, 0x2e4);
    if (solmtd < 1 || 4 < solmtd) {
        std::fprintf(stdout, "   6     SOLMTD   %d(%s)\n", solmtd, "Invalid");
    }
    else {
        std::fprintf(stdout, "   6     SOLMTD   %s\n", option_label(0x1405beb00, solmtd - 1).c_str());
    }
    std::fprintf(stdout, "   7     ITRMXT   %d\n", i32(m, 0x31c));
    std::fprintf(stdout, "   8     TMPBWD   %s\n", option_label(0x1405beab0, i32(m, 0x320)).c_str());
    const std::int32_t solmtt = i32(m, 0x318);
    if (solmtt < 1 || 3 < solmtt) {
        std::fprintf(stdout, "   9     SOLMTT   %d(%s)\n", solmtt, "Invalid");
    }
    else {
        std::fprintf(stdout, "   9     SOLMTT   %s\n", option_label(0x1405beba0, solmtt - 1).c_str());
    }
    std::fprintf(stdout, "  10     LATENT   %s\n", option_label(0x1405beab0, i32(m, 0x324)).c_str());
}

// @src 14000ccf0  Simulation Controls menu text and selection
static std::string show_sim_controls(MenuRec* m)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP. -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
    std::fprintf(stdout, "  ***  Simulation Controls Modification Menu  ***\n\n");
    std::fprintf(stdout, "     1 - Process Definition\n");
    std::fprintf(stdout, "     2 - Stopping & Step Controls\n");
    std::fprintf(stdout, "     3 - Iteration Controls\n");
    std::fprintf(stdout, "     4 - Processing Conditions\n");
    std::fprintf(stdout, "     5 - User Defined Variables\n\n");
    return ext::select(m, false, 0, 0);
}

// @src 14000ce70  Process Definition form
static std::string form_process(MenuRec* m)
{
    show_process(m);
    while (true) {
        const std::string sel = ext::select(m, false, 0, 0);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1': {
            std::fprintf(stdout, " TITLE  = ");
            const std::string t = read_line(self(m));
            copy_cstr(data(m), t.c_str());
            break;
        }
        case '2': {
            std::fprintf(stdout,
                         " Enter Simulation Mode: 1 - Isothermal; 2 - Heat Transfer; 3 - Non-Isothmeral\n");
            const std::int32_t r = ask_int(m, 1, 3, " SMODE  =      %d ? ", i32(m, 0xf4));
            if (r != -0xf423d) {
                key::g1a::ops::set_smode(*static_cast<model::Cls_1401e7b10*>(m->data), r); // 1401f3ff0
            }
            break;
        }
        case '3': {
            const std::int32_t r = ask_int(m, 1, 9999, " MESHNO  =      %d ? ", i32(m, 0xf0));
            if (r != -0xf423d) i32(m, 0xf0) = r;
            break;
        }
        case '4': {
            std::fprintf(stdout, " Enter Unit System: 1 - SI; 2 - English\n");
            const std::int32_t r = ask_int(m, 1, 2, "  UNIT   =     %d ? ", i32(m, 0xfc));
            if (r != -0xf423d) i32(m, 0xfc) = r;
            break;
        }
        case '5': {
            const double v = ask_double(m, 0.0, 1e30, " TNOW  =      %lf ? ", f64(m, 0x640));
            if (kNoValue != v) f64(m, 0x640) = v;
            break;
        }
        case 'R':
        case 'r':
            show_process(m);
            break;
        default:
            break;
        }
    }
}

// @src 14000de20  Stopping & Step Controls form
static std::string form_stopping(MenuRec* m)
{
    show_stopping(m);
    while (true) {
        const std::string s = ext::select(m, false, 0, 0);
        auto int_item = [&](std::size_t off, std::int32_t lo, std::int32_t hi, const char* fmt) {
            const std::int32_t r = ask_int(m, lo, hi, fmt, i32(m, off));
            if (r != -0xf423d) i32(m, off) = r;
        };
        auto dbl_item = [&](std::size_t off, double hi, const char* fmt) {
            const double v = ask_double(m, 0.0, hi, fmt, f64(m, off));
            if (kNoValue != v) f64(m, off) = v;
        };
        auto vec_item = [&](std::size_t off, const char* fmt) {
            rt::crt::fprintf(stdout, fmt, f64(m, off), f64(m, off + 8), f64(m, off + 0x10));
            read_doubles(m, &f64(m, off), 3);
        };
        if (s == "1") {
            const std::int32_t r0 = ask_int(m, -99999, 99999, " NSTART =      %d ? ", i32(m, 0x160));
            std::int32_t r = r0;
            if (r0 == 0) {
                const std::int32_t cur = i32(m, 0x160);
                if (0 < cur) {
                    r = -1 - cur; // 14000e092: 0 turns a positive start step into -(n+1)
                }
                else {
                    i32(m, 0x160) = r;
                    continue;
                }
            }
            if (r != -0xf423d) i32(m, 0x160) = r;
        }
        else if (s == "2") int_item(0x164, 1, 9999, " STPINC =    %d ? ");
        else if (s == "3") int_item(0x168, 1, 100, " PDIE   =     %d ? ");
        else if (s == "4") int_item(0x16c, 1, 100000, " NSTEP   =     %d ? ");
        else if (s == "5") dbl_item(0x640, 1e30, " EMAX  =      %lf ? ");
        else if (s == "6") dbl_item(0x178, 1e30, " TMAX  =      %lf ? ");
        else if (s == "7") vec_item(0x180, "SMAX   =   %lf  %lf  %lf ? ");
        else if (s == "8") vec_item(0x198, "VMIN   =   %lf  %lf  %lf ? ");
        else if (s == "9") vec_item(0x1b0, "LMAX   =   %lf  %lf  %lf ? ");
        else if (s == "10") {
            std::fprintf(stdout, " Enter Simulation Mode: 1 - User; 2 - System; 3 - Non-Temperature\n");
            int_item(0x1c8, 1, 3, " STPDEF =      %d ? ");
        }
        else if (s == "11") dbl_item(0x260, 1e30, " DEMAX  =      %lf ? ");
        else if (s == "12") dbl_item(0x1e0, 1e30, " DSMAX  =      %lf ? ");
        else if (s == "13") dbl_item(0x1d0, 1e30, " DTMAX  =      %lf ? ");
        else if (s == "14") dbl_item(0x268, 1e30, " SLDERR =      %lf ? ");
        else if (s == "15") {
            std::fprintf(stdout, "\n Enter Max. Temperature Increment, Min. Time Increment, Max. Time Increment\n");
            rt::crt::fprintf(stdout, "DTPMAX   =   %lf  %lf  %lf ? ", f64(m, 0x270), f64(m, 0x2a0), f64(m, 0x2a8));
            double v[3] = {f64(m, 0x270), f64(m, 0x2a0), f64(m, 0x2a8)};
            read_doubles(m, v, 3);
            f64(m, 0x270) = v[0]; // 14000ed51
            f64(m, 0x270) = v[1]; // 14000ed63 (sic)
            f64(m, 0x270) = v[2]; // 14000ed75 (sic)
        }
        else if (s == "16") {
            rt::crt::fprintf(stdout, "DVMAX   =   %lf  %lf ? ", f64(m, 0x278), f64(m, 0x280));
            read_doubles(m, &f64(m, 0x278), 2);
        }
        else if (s == "17") dbl_item(0x2c0, 2.0, " DTSUB  =      %lf ? "); // DAT_140509fb8
        else if (s == "r" || s == "R") show_stopping(m);
        else if (s == "e" || s == "E" || s == "0") return "";
        else if (s == "/") return "/";
    }
}

// @src 14000bc80  Iteration Controls form
static std::string form_iteration(MenuRec* m)
{
    show_iteration(m);
    const std::string newton = option_label(0x1405bea30, 0);
    const std::string direct = option_label(0x1405bea30, 1);
    const std::string expl = option_label(0x1405bea30, 2);
    const std::string no = option_label(0x1405beab0, 0);
    const std::string yes = option_label(0x1405beab0, 1);
    while (true) {
        const std::string s = ext::select(m, false, 0, 0);
        auto int_item = [&](std::size_t off, std::int32_t lo, std::int32_t hi, const char* fmt) {
            const std::int32_t r = ask_int(m, lo, hi, fmt, i32(m, off));
            if (r != -0xf423d) i32(m, off) = r;
        };
        if (s == "1") {
            rt::crt::fprintf(stdout, " CVGERR =  %lf  %lf ? ", f64(m, 0x2d0), f64(m, 0x2d8));
            double v[2] = {f64(m, 0x2d0), f64(m, 0x2d8)};
            read_doubles(m, v, 2);
            f64(m, 0x2d0) = v[0];
            f64(m, 0x2d8) = v[1];
        }
        else if (s == "2") std::fprintf(stdout, "Not avaliable anymore\n");
        else if (s == "3") {
            std::fprintf(stdout, " Iteration Method: 1 - %s; 2 - %s; 3 - %s\n", newton.c_str(), direct.c_str(),
                         expl.c_str());
            int_item(0x2e0, 1, 3, " ITRMTH =      %d ? ");
        }
        else if (s == "4") int_item(0x310, 1, 9999, " ITRMXD   =     %d ? ");
        else if (s == "5") {
            std::fprintf(stdout, " Deformation Bandwidth Optimization: 1 - %s; 2 - %s;\n", no.c_str(), yes.c_str());
            int_item(0x314, 0, 1, " DEFBWD =      %d ? ");
        }
        else if (s == "6") {
            std::fprintf(stdout, " Solution Method: 1 - %s; 2 - %s; 3 - %s\n", newton.c_str(), direct.c_str(),
                         expl.c_str());
            int_item(0x2e0, 1, 3, " SOLMTD =      %d ? "); // S+0x2e0 (sic)
        }
        else if (s == "7") int_item(0x31c, 1, 9999, " ITRMXD   =     %d ? ");
        else if (s == "8") {
            std::fprintf(stdout, " Temperature Bandwidth Optimization: 1 - %s; 2 - %s;\n", no.c_str(), yes.c_str());
            int_item(0x320, 0, 1, " TMPBWD =      %d ? ");
        }
        else if (s == "9") {
            std::fprintf(stdout, " Solution Method: 1 - %s; 2 - %s; 3 - %s\n", newton.c_str(), direct.c_str(),
                         expl.c_str());
            int_item(0x318, 1, 3, " SOLMTT =      %d ? ");
        }
        else if (s == "10") {
            std::fprintf(stdout, " Latent Heat Calculation: 1 - %s; 2 - %s;\n", no.c_str(), yes.c_str());
            int_item(0x324, 0, 1, " LATENT =      %d ? ");
        }
        else if (s == "r" || s == "R") show_iteration(m);
        else if (s == "e" || s == "E" || s == "0") return "";
        else if (s == "/") return "/";
    }
}

// @src 14000d670  Processing Conditions form
static std::string form_processing(MenuRec* m)
{
    show_processing(m);
    while (true) {
        const std::string sel = ext::select(m, false, 0, 0);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1':
            rt::crt::fprintf(stdout, " GEOERR =   %lf  %lf  %lf  %lf ? ", f64(m, 0x660), f64(m, 0x668), f64(m, 0x670),
                    f64(m, 0x678));
            read_doubles(m, &f64(m, 0x660), 4);
            break;
        case '2': {
            const double v = ask_double(m, 0.0, 1e30, " PENINF  =      %lf ? ", f64(m, 0x3a8));
            if (kNoValue != v) f64(m, 0x3a8) = v;
            break;
        }
        case '3': {
            const double v = ask_double(m, 0.0, 1e30, " UNTE2H  =      %lf ? ", f64(m, 0x3b0));
            if (kNoValue != v) f64(m, 0x3b0) = v;
            break;
        }
        case '4': {
            const double v = ask_double(m, 0.0, 1.0, " TINTGF  =      %lf ? ", f64(m, 0x3b8)); // DAT_140509c90
            if (kNoValue != v) f64(m, 0x3b8) = v;
            break;
        }
        case '5':
            edit_not_supported(); // ENVTMP
            break;
        case '6': {
            rt::crt::fprintf(stdout, " BLZMAN  =      %lf ? ", f64(m, 0x4b8));
            const double v = read_double(m);
            if (kNoValue != v) f64(m, 0x4b8) = v;
            break;
        }
        case '7':
            edit_not_supported(); // CNVCOF
            break;
        case 'R':
        case 'r':
            show_processing(m);
            break;
        default:
            break;
        }
    }
}

// @src 1400150f0  User Defined Variables form (m: the menu 3 record, data = S)
static std::string form_usrdef(MenuRec* m)
{
    while (true) {
        show_usrdef(m);
        std::fprintf(stdout, "\n\n Enter Selection (M, R, E, /): ");
        const std::string s = read_line(self(m));
        if (s == "M" || s == "m") {
            char buf[208];
            format200(buf, "\n Enter Line No. to be Modified (0 to Exit) = ");
            const std::int32_t n = read_int(self(m), 0, 10, std::string(buf), false);
            if (n == -0xf423d || n == 0) {
                return "";
            }
            std::fprintf(stdout, "\n Enter Line %d below\n", n);
            std::fflush(stdout);
            char line[0x50];
            line[0] = '\0';
            std::cin.getline(line, 0x50, '\n'); // 1400153af: 80 characters
            copy_cstr(data(m) + 0x688 + static_cast<std::ptrdiff_t>(n - 1) * 0x51, line);
        }
        else if (s == "R" || s == "r") {
            (void)show_sim_controls(m); // 14000ccf0: the answer is dropped
        }
        else if (s == "E" || s == "e") {
            return "";
        }
        else if (s == "/") {
            return "/";
        }
    }
}

// @src 140017540  menu 3
void menu3_sim_controls(MenuRec* m)
{
    bool quit = false;
    std::string sub; // the last sub-form's answer
    while (true) {
        const std::string sel = show_sim_controls(m);
        bool redisplay = false;
        auto run = [&](std::size_t k, std::string (*form)(MenuRec*)) {
            m[k].data = step(m) + 0x750;
            m[k].step = m->step;
            sub = form(&m[k]);
        };
        switch (first_char(sel)) {
        case '0':
        case 'E':
        case 'e':
            quit = true;
            break;
        case '1': run(1, form_process); break;
        case '2': run(2, form_stopping); break;
        case '3': run(3, form_iteration); break;
        case '4': run(4, form_processing); break;
        case '5': sub = form_usrdef(m); break;
        case 'R':
        case 'r':
            redisplay = true;
            break;
        default:
            break;
        }
        if (redisplay) {
            continue;
        }
        if (sel == "/" || sub == "/" || quit) {
            return;
        }
    }
}

} // namespace def::pre
