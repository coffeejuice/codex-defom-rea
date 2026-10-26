// Main menu 4 "Material Properties Modification" (packet p10-01): FUN_1400157c0 and its three
// property forms. The record m = main+0xa8 has step = the DB_STEP_DATA (set by the main loop) and
// data = the current Material (set here). Material offsets: layout/material.hpp.
//
// Quirks kept from the original:
// * the Material Name answer is read into the selection string, so the answer "/" leaves menu 4;
// * "R" in the property forms redisplays the main material menu (140001ae0), not the form;
// * the elastic form's title says "Plastic Properties".
#include "p10_01_common.hpp"

#include <cstdio>

#include <defmodel/layout/material.hpp>
#include <defmodel/step/material_select.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::pre {

using namespace p1001;

namespace {

model::Material* mat(MenuRec* m) { return static_cast<model::Material*>(m->data); }
model::DB_STEP_DATA& step_data(MenuRec* m) { return *static_cast<model::DB_STEP_DATA*>(m->step); }

void property_head(MenuRec* m, const char* title)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP. - MATERIAL -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
    std::fprintf(stdout, title, mat(m)->number);
    std::fprintf(stdout, " Number  Keywrd  Values\n");
    std::fprintf(stdout, " ======  ======  ===============================\n");
}

} // namespace

// @src 140001ae0  material menu display
static void show_material(MenuRec* m)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP.\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
    std::fprintf(stdout, " ***  Material Properties Modification Menu  ***\n\n");
    std::fprintf(stdout, "   1 - Material Number [    %d]\n", mat(m)->number);
    std::fprintf(stdout, "   2 - Material Name   [%s]\n", mat(m)->name);
    std::fprintf(stdout, "   3 - Plastic Properties\n");
    std::fprintf(stdout, "   4 - Elastic Properties\n");
    std::fprintf(stdout, "   5 - Thermal Properties\n");
    std::fprintf(stdout, "   6 - Material Data Retrieval\n");
}

// @src 140001c00  plastic properties display
static void show_plastic(MenuRec* m)
{
    property_head(m, " ***  Plastic Properties of Material   %d ***\n\n");
    rt::crt::fprintf(stdout, "   1     MATDEN  %lf\n", mat(m)->matden);
    rt::crt::fprintf(stdout, "   2     FRAE2H  %lf\n", mat(m)->frae2h);
    std::fprintf(stdout, "   3     FSTRES   \n");
}

// @src 140001d20  elastic properties display
static void show_elastic(MenuRec* m)
{
    property_head(m, " ***  Plastic Properties of Material   %d ***\n\n"); // str 140506bc8 (sic)
    print_dynary_row(mat(m)->young, 1, "YOUNG");
    print_dynary_row(mat(m)->poison, 2, "POISON");
    print_dynary_row(mat(m)->expand, 3, "EXPAND");
}

// @src 140001e40  thermal properties display
static void show_thermal(MenuRec* m)
{
    property_head(m, " ***  Thermal Properties of Material   %d ***\n\n");
    print_dynary_row(mat(m)->thrcnd, 1, "THRCND");
    print_dynary_row(mat(m)->heatcp, 2, "HEATCP");
    print_dynary_row(mat(m)->emsvty, 3, "EMSVTY");
    std::fprintf(stdout, "   4     LTHEAT    \n");
    std::fprintf(stdout, "   5     TMPSOL    \n");
    std::fprintf(stdout, "   6     TMPLIQ    \n");
}

// @src 14000f340  plastic properties form
static std::string form_plastic(MenuRec* m)
{
    show_plastic(m);
    while (true) {
        const std::string sel = ext::select(m, true, 1, 3);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1': {
            rt::crt::fprintf(stdout, " MATDEN  =      %lf ? ", mat(m)->matden);
            const double v = read_double(m);
            if (kNoValue != v) mat(m)->matden = v;
            break;
        }
        case '2': {
            rt::crt::fprintf(stdout, " FRAE2H  =      %lf ? ", mat(m)->frae2h);
            const double v = read_double(m);
            if (kNoValue != v) mat(m)->frae2h = v;
            break;
        }
        case '3':
            std::fprintf(stdout, "Enter Flow Stress Definition Method:  N for Function N; -N for Routine N\n");
            not_implemented();
            break;
        case 'R':
        case 'r':
            show_material(m);
            break;
        default:
            break;
        }
    }
}

// @src 14000f850  elastic properties form
static std::string form_elastic(MenuRec* m)
{
    show_elastic(m);
    while (true) {
        const std::string sel = ext::select(m, true, 1, 3);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1': // YOUNG
        case '2': // POISON
        case '3': // EXPAND
            edit_not_supported();
            break;
        case 'R':
        case 'r':
            show_material(m);
            break;
        default:
            break;
        }
    }
}

// @src 14000fd10  thermal properties form
static std::string form_thermal(MenuRec* m)
{
    show_thermal(m);
    while (true) {
        const std::string sel = ext::select(m, true, 1, 6);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1': // THRCND
        case '2': // HEATCP
        case '3': // EMSVTY
            edit_not_supported();
            break;
        case '4':
        case '5':
        case '6':
            not_implemented();
            break;
        case 'R':
        case 'r':
            show_material(m);
            break;
        default:
            break;
        }
    }
}

// @src 1400157c0  menu 4
void menu4_material(MenuRec* m)
{
    bool quit = false;
    model::DB_STEP_DATA& s = step_data(m);
    m->data = model::step_material_by_id(s, model::step_current_material(s)); // 1401ca9c0, 1401d7060
    if (m->data == nullptr) {
        m->data = model::step_material_get_or_add(s, 1);
        if (m->data == nullptr) {
            return;
        }
    }
    std::string sel;
    std::string sub;
    while (true) {
        show_material(m);
        sel = ext::select(m, false, 0, 0);
        bool redisplay = false;
        switch (first_char(sel)) {
        case '0':
        case 'E':
        case 'e':
            quit = true;
            break;
        case '1': {
            const std::int32_t r = ask_int(m, 1, 300, " Material Number =     %d ? ", mat(m)->number);
            if (r != -0xf423d) {
                m->data = model::step_material_get_or_add(s, r);
                model::step_set_current_material(s, r);
            }
            break;
        }
        case '2':
            std::fprintf(stdout, "MTNAME = ");
            sel = read_line(self(m)); // into the selection string (see the file comment)
            if (!sel.empty()) {
                copy_cstr(mat(m)->name, sel.c_str());
            }
            break;
        case '3': sub = form_plastic(m); break;
        case '4': sub = form_elastic(m); break;
        case '5': sub = form_thermal(m); break;
        case '6': not_implemented(); break;
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
