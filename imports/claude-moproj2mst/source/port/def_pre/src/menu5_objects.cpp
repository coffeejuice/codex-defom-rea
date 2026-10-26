// Main menu 5 "Object Data Modification" (packet p10-02): FUN_14001a050, its displays and its
// forms (characteristics, die geometry, BC functions, remeshing criteria). The record m is
// main+0x08: data = the current LLOBJ_DATA, step = the DB_STEP_DATA; m+0x18 is the die
// definition type (main+0x20). Offsets below are object+x (layout/llobj_data.hpp).
// Behaviour and quirks: docs/re/menus/MENU5_6.md (all kept as in the disassembly), e.g.
// * a "%d" conversion given a double prints the low 32 bits of its bit pattern (RMSTEP, MGSIZR,
//   MGWxxx, 2D REFTMP prompts: the double is passed in the integer register) and the 2D MGTELM
//   row prints the int at +0x2d84 as the double whose bits are that zero-extended int;
// * remesh items 6-10 store the answer even when it is "no value" (-999997), unless the last
//   int read by the form (RMSTEP / 2D MGNELM) was "no value"; item 10 (MGWCUV) stores into
//   MGWSTR (+0x2d38);
// * 2D characteristics 6 (STROKE) and 10 (CNTRAX) format their prompts but never print them;
// * 2D characteristics 3 reads the ZSTR type with the stale prompt "\n".
#include <def_pre/menu56.hpp>

#include <cstdio>
#include <cstring>
#include <iostream>

#include <def_pre/ext_p10_01.hpp>
#include <def_pre/ext_p10_02.hpp>
#include <def_pre/forms.hpp>
#include <def_pre/menu7.hpp>
#include <defcmd/objpos.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "p10_01_common.hpp"

namespace def::pre {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::field;
using model::LLOBJ_DATA;
using p1001::first_char;
using p1001::option_label;
using p1001::problem;
using p1001::self;

namespace {

LLOBJ_DATA* obj(MenuRec* m) { return static_cast<LLOBJ_DATA*>(m->data); }
DB_STEP_DATA* stp(MenuRec* m) { return static_cast<DB_STEP_DATA*>(m->step); }
double& f64(LLOBJ_DATA* o, std::size_t off) { return field<double>(o, off); }
std::int32_t& i32(LLOBJ_DATA* o, std::size_t off) { return field<std::int32_t>(o, off); }
bool is_3d(MenuRec* m) { return field<std::int32_t>(m->step, 0x1c) == 3; }
std::int32_t dim_type(MenuRec* m) { return field<std::int32_t>(m->data, 0xc); }

// "%d" given a double: the low 32 bits of the double's bit pattern (x64 varargs pass it in the
// integer register too, and the CRT reads 4 bytes)
std::int32_t low32(double d)
{
    std::uint64_t u = 0;
    std::memcpy(&u, &d, sizeof u);
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(u));
}

// "%lf" given an int zero-extended into its 8-byte slot
double bits_as_double(std::uint32_t v)
{
    const std::uint64_t u = v;
    double d = 0.0;
    std::memcpy(&d, &u, sizeof d);
    return d;
}

// format200 into a 200-byte buffer, as a std::string (the prompt-then-read idiom)
template <class... A>
std::string fmt(const char* f, A... a)
{
    char buf[208];
    format200(buf, f, a...);
    return std::string(buf);
}

// @data 140508b50  -999997.0 (kNoValue), 140509b50  1e30, 14050a5d0  -10.0
constexpr double kBig = 1e30;

void object_header(MenuRec* m)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP. - OBJECT -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
}

void table_head()
{
    std::fprintf(stdout, " Number  Keywrd  Values\n");
    std::fprintf(stdout, " ======  ======  ===============================\n");
}

// the angular-movement question of 3D item 7 and 2D item 9 (identical code at 140017d0d and
// 140018a60)
void ask_angular(MenuRec* m, LLOBJ_DATA* o)
{
    std::fprintf(stdout, " Enter Angular Movement Control Method: 0 - Torque; 1 - RPUT\n");
    const std::int32_t v =
        read_int(self(m), 0, 1, fmt("  Angular Movement Control Method =     %d ? ", i32(o, 0x2788)), true);
    if (v != -0xf423d) {
        i32(o, 0x2788) = v;
    }
    if (i32(o, 0x2788) == 0) {
        rt::crt::fprintf(stdout, "  Torque = %lf ? ", f64(o, 0x27c8));
    }
    else {
        rt::crt::fprintf(stdout, "  RPUT = %lf ? ", f64(o, 0x2840));
    }
    const double d = read_double(m);
    if (kNoValue != d) {
        if (i32(o, 0x2788) == 0) {
            f64(o, 0x27c8) = d;
        }
        else {
            f64(o, 0x2840) = d;
        }
    }
}

// object types 1..4 (plastic, porous, elastic, elasto-plastic) only
bool deformable(MenuRec* m)
{
    const std::int32_t t = dim_type(m);
    return t == 1 || t == 2 || t == 3 || t == 4;
}

} // namespace

// ---- displays -----------------------------------------------------------------------------------

// @src 140001690  warning
void warn_not_applicable()
{
    std::fprintf(stdout, "\n\n  *** WARNING: Not Applicable for this Object\n");
}

// @src 140006bd0  list tag
std::string not_applicable_tag(MenuRec* m, std::int32_t item)
{
    const std::int32_t t = dim_type(m);
    if (t == 7 || (t == 0 && 5 < item)) {
        return std::string("(Not Applicable)");
    }
    return std::string(" "); // str 140508694
}

// @src 140006d10  menu 5 list
void show_object_menu(MenuRec* m)
{
    LLOBJ_DATA* o = obj(m);
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP.\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
    std::fprintf(stdout, " ***   Object Data Modification Menu ***\n\n");
    std::fprintf(stdout, "   1 - Object Number[    %d]\n", i32(o, 0x8));
    std::fprintf(stdout, "   2 - Object Name  [%s]\n", &field<char>(o, 0x828));
    const std::int32_t t = i32(o, 0xc);
    const std::string type = t == 7 ? option_label(0x1405beca0, 0) : option_label(0x1405beca0, t + 1);
    std::fprintf(stdout, "   3 - Object Type  [%s ]\n", type.c_str());
    std::fprintf(stdout, "   4 - Object Characteristics %s\n", not_applicable_tag(m, 4).c_str());
    std::fprintf(stdout, "   5 - Rigid Die/Object Border Definition %s \n", not_applicable_tag(m, 5).c_str());
    std::fprintf(stdout, "   6 - Node Definition %s\n", not_applicable_tag(m, 6).c_str());
    std::fprintf(stdout, "   7 - Element Definition %s\n", not_applicable_tag(m, 7).c_str());
    std::fprintf(stdout, "   8 - Boundary Condition Function Definition %s\n", not_applicable_tag(m, 8).c_str());
    std::fprintf(stdout, "   9 - Boundary Condition Generation %s\n", not_applicable_tag(m, 9).c_str());
    std::fprintf(stdout, "  10 - Remeshing/Mesh Generation Criteria %s\n", not_applicable_tag(m, 10).c_str());
    if (field<std::int32_t>(m->step, 0x1c) == 2) {
        std::fprintf(stdout, "  11 - User Defined Mesh Generation %s\n", not_applicable_tag(m, 11).c_str());
        std::fprintf(stdout, "  12 - Fracture Definition %s\n", not_applicable_tag(m, 12).c_str());
    }
    std::fprintf(stdout, "\n");
}

// @src 140002510  movement rows
void show_movement_rows(MenuRec*, void* mv)
{
    const std::int32_t t = field<std::int32_t>(mv, 0x68);
    if (t == 2) {
        rt::crt::fprintf(stdout, "               Enery           = %lf\n", field<double>(mv, 0x1f0));
        rt::crt::fprintf(stdout, "               Blow Efficiency = %lf\n", field<double>(mv, 0x1f8));
        rt::crt::fprintf(stdout, "               Object Mass     = %lf\n", field<double>(mv, 0x1e8));
    }
    else if (t == 3) {
        rt::crt::fprintf(stdout, "               Enery             = %lf\n", field<double>(mv, 0x280));
        rt::crt::fprintf(stdout, "               Blow Efficiency   = %lf\n", field<double>(mv, 0x288));
        rt::crt::fprintf(stdout, "               Moment of Inertia = %lf\n", field<double>(mv, 0x290));
        rt::crt::fprintf(stdout, "               Ram Movement/Rev. = %lf\n", field<double>(mv, 0x298));
    }
    else if (t == 4) {
        rt::crt::fprintf(stdout, "               Total Displacement = %lf\n", field<double>(mv, 0x318));
        rt::crt::fprintf(stdout, "               # of Strokes per unit time = %lf\n", field<double>(mv, 0x320));
    }
    rt::crt::fprintf(stdout, "              Direction     = %f   %f   %f\n", field<double>(mv, 0x50),
                     field<double>(mv, 0x58), field<double>(mv, 0x60));
}

// @src 140003f50  characteristics table
void show_characteristics(MenuRec* m)
{
    LLOBJ_DATA* o = obj(m);
    object_header(m);
    std::fprintf(stdout, " ***  Characteristics of Object    %d ***\n\n", i32(o, 0x8));
    table_head();
    rt::crt::fprintf(stdout, "   1     AVGSTR  %lf  \n", f64(o, 0x1ea8));
    rt::crt::fprintf(stdout, "   2     LMTSTR  %lf  \n", f64(o, 0x1ea0));
    if (is_3d(m)) {
        rt::crt::fprintf(stdout, "   3     PENVOL  %lf  \n", f64(o, 0x1eb0));
        rt::crt::fprintf(stdout, "   4     REFTMP  %lf  \n", f64(o, 0x1eb8));
        rt::crt::fprintf(stdout, "   5     STROKE  %lf\t%lf\t%lf \n", f64(o, 0x22d0), f64(o, 0x22d8), f64(o, 0x22e0));
        std::fprintf(stdout, "   6     MOVCTL  by: %s\n", option_label(0x1405bede0, i32(o, 0x21c8)).c_str());
        show_movement_rows(m, &field<std::byte>(o, 0x2160));
        const std::int32_t a = i32(o, 0x2788);
        const double v = a == 1 ? f64(o, 0x2840) : f64(o, 0x27c8);
        rt::crt::fprintf(stdout, "   7     ANGMOV  by: %s   =  %lf\n", option_label(0x1405beeb0, a).c_str(), v);
        rt::crt::fprintf(stdout, "   8     CNTRAX      %lf  %lf  %lf\n", f64(o, 0x28c0), f64(o, 0x28c8), f64(o, 0x28d0));
        rt::crt::fprintf(stdout, "                     %lf  %lf  %lf\n", f64(o, 0x28d8), f64(o, 0x28e0), f64(o, 0x28e8));
        rt::crt::fprintf(stdout, "   9     ROTSYM  A   %lf \n", f64(o, 0x2058));
        rt::crt::fprintf(stdout, "                 V   %lf  %lf  %lf\n", f64(o, 0x2078), f64(o, 0x2080), f64(o, 0x2088));
        rt::crt::fprintf(stdout, "                 P   %lf  %lf  %lf\n\n", f64(o, 0x2060), f64(o, 0x2068), f64(o, 0x2070));
        return;
    }
    rt::crt::fprintf(stdout, "   3     ZSTR    %d    %lf  \n", i32(o, 0x1f30), f64(o, 0x1f28));
    rt::crt::fprintf(stdout, "   4     PENVOL  %lf  %s\n", f64(o, 0x1eb0), option_label(0x1405bf280, i32(o, 0x1f20)).c_str());
    rt::crt::fprintf(stdout, "   5     REFTMP  %lf  \n", f64(o, 0x1eb8));
    rt::crt::fprintf(stdout, "   6     STROKE  %lf\t%lf\t \n", f64(o, 0x22d0), f64(o, 0x22d8));
    std::fprintf(stdout, "   7     MOVCTL  by: %s\n", option_label(0x1405bede0, i32(o, 0x21c8)).c_str());
    print_dynary_row(field<DYNARY>(o, 0x2a20), 8, "SPDLMT"); // 1400016d0
    std::fprintf(stdout, "   9     ANGMOV  by: %s \n", option_label(0x1405beeb0, i32(o, 0x2788)).c_str());
    rt::crt::fprintf(stdout, "  10     CNTRAX    %lf    %lf   %lf \n", f64(o, 0x2790), f64(o, 0x2798), f64(o, 0x27a0));
    rt::crt::fprintf(stdout, "  11     TMPLMT    %lf\n", f64(o, 0x1f10));
    std::fprintf(stdout, "  12     ANGMVY  by: %s \n", option_label(0x1405beeb0, i32(o, 0x2ac0)).c_str());
    std::fprintf(stdout, "  13     REFPOS   \n");
}

// @src 140004510  die geometry menu
void show_die_menu(MenuRec* m)
{
    LLOBJ_DATA* o = obj(m);
    model::OBJ_GEO3D* g = model::geo_kw::surface(*o, 0); // 140198620
    object_header(m);
    std::fprintf(stdout, " ***  Die Geometry of Object   %d ***\n\n", i32(o, 0x8));
    std::fprintf(stdout, "    1 - Definition Type           [ %s ]\n",
                 option_label(0x1405bef00, field<std::int32_t>(m, 0x18)).c_str());
    if (g == nullptr) {
        std::fprintf(stdout, "    2 - Display Point Coordinates [       0 ]\n");
    }
    else {
        std::fprintf(stdout, "    2 - Display Point Coordinates [      %d ]\n",
                     static_cast<std::uint32_t>(g->coords.size() / 3));
    }
    std::fprintf(stdout, "    3 - Modify Point Coordinates  \n");
    if (g == nullptr) {
        std::fprintf(stdout, "   4 - Display Connectivity    [        0 ]\n");
    }
    else {
        std::fprintf(stdout, "    4 - Display Connectivity      [      %d ]\n",
                     static_cast<std::uint32_t>(g->elems.size() >> 2));
    }
    std::fprintf(stdout, "    5 - Modify Connectivity     \n");
    std::fprintf(stdout, "    6 - Check Die Geometry      \n");
    std::fprintf(stdout, "    7 - Generate Mesh        \n\n");
}

// @src 140004700  die geometry menu, points only (3D)
void show_die_menu_points(MenuRec* m)
{
    if (!is_3d(m)) {
        return;
    }
    LLOBJ_DATA* o = obj(m);
    model::OBJ_GEO3D* g = model::geo_kw::surface(*o, 0); // 140198620
    object_header(m);
    std::fprintf(stdout, " ***  Die Geometry of Object   %d ***\n\n", i32(o, 0x8));
    std::fprintf(stdout, "    1 - Definition Type           [ %s ]\n",
                 option_label(0x1405bef00, field<std::int32_t>(m, 0x18)).c_str());
    if (g == nullptr) {
        std::fprintf(stdout, "    2 - Display Point Coordinates [       0 ]\n");
    }
    else {
        std::fprintf(stdout, "    2 - Display Point Coordinates [      %d ]\n",
                     static_cast<std::uint32_t>(g->coords.size() / 3));
    }
    std::fprintf(stdout, "    3 - Modify Point Coordinates  \n\n");
}

// @src 140002450  BC function menu
void show_bc_function_menu(MenuRec* m)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP. -OBJECT - BOUND. CONDITIONS -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
    std::fprintf(stdout, " ***    Function Definition of Object #   %d ***\n\n", i32(obj(m), 0x8));
    std::fprintf(stdout, " 1 - Boundary Condition Function Number [  ]\n");
    std::fprintf(stdout, " 2 - Function Definition: (#, Time, Value)\n\n");
}

// @src 1400020c0  remeshing criteria
void show_remesh_criteria(MenuRec* m)
{
    LLOBJ_DATA* o = obj(m);
    object_header(m);
    table_head();
    rt::crt::fprintf(stdout, "    1    RMDPTH        %lf\n", f64(o, 0x2dc8));
    rt::crt::fprintf(stdout, "    2    RMSTRK        %lf\n", f64(o, 0x2dd0));
    rt::crt::fprintf(stdout, "    3    RMTIME        %lf\n", f64(o, 0x2de0));
    std::fprintf(stdout, "    4    RMSTEP        %d\n", low32(f64(o, 0x2dd8))); // a double, see the file comment
    if (is_3d(m)) {
        std::fprintf(stdout, "    5    MGNELM        %d\t%d\t%d\t%d\n", i32(o, 0x2d88), i32(o, 0x2d8c), i32(o, 0x2d90),
                     i32(o, 0x2d94));
        rt::crt::fprintf(stdout, "    6    MGSIZR        %lf\n", f64(o, 0x2d20));
        rt::crt::fprintf(stdout, "    7    MGWTMP        %lf\n", f64(o, 0x2d28));
        rt::crt::fprintf(stdout, "    8    MGWSTN        %lf\n", f64(o, 0x2d30));
        rt::crt::fprintf(stdout, "    9    MGWSTR        %lf\n", f64(o, 0x2d38));
        rt::crt::fprintf(stdout, "   10    MGWCUV        %lf\n", f64(o, 0x2d48));
        std::fprintf(stdout, "   11    MGGRID        %d\t%d\n\n", i32(o, 0x2dc0), i32(o, 0x2dc4));
        return;
    }
    std::fprintf(stdout, "    5    MGNELM        %d\n", i32(o, 0x2d80));
    // an int passed to "%lf" (MOV R8D: zero-extended)
    rt::crt::fprintf(stdout, "    6    MGTELM        %lf\n", bits_as_double(static_cast<std::uint32_t>(i32(o, 0x2d84))));
    rt::crt::fprintf(stdout, "    7    MGSIZR        %lf\n", f64(o, 0x2d20));
    rt::crt::fprintf(stdout, "    8    MGWTMP        %lf\n", f64(o, 0x2d28));
    rt::crt::fprintf(stdout, "    9    MGWSTN        %lf\n", f64(o, 0x2d30));
    rt::crt::fprintf(stdout, "   10    MGWSTR        %lf\n", f64(o, 0x2d38));
    rt::crt::fprintf(stdout, "   11    MGWCUV        %lf\n", f64(o, 0x2d48));
    std::fprintf(stdout, "   12    MGGRID        %d\t%d\n\n", i32(o, 0x2dc0), i32(o, 0x2dc4));
    rt::crt::fprintf(stdout, "   13    MGERR         %lf\t %lf\n", f64(o, 0x2db0), f64(o, 0x2db8));
}

// ---- forms --------------------------------------------------------------------------------------

// @src 1400149e0  one movement value
void form_movement_value(MenuRec* m, DYNARY* d, const char* name, std::int32_t stale)
{
    std::int32_t v = d->u54; // routine number
    if (v == 0 && !dynary_is_constant(*d)) { // 14012fa90
        if (d->type == 1) {
            v = 2;
        }
        else {
            // [RSP+0x30], a stack slot this function never writes (see MENU5_6.md)
            v = stale;
            if (d->type == 2) {
                v = 1;
            }
        }
    }
    std::fprintf(stdout, "  -N - Routine N;   0 - Constant;   1 - Stroke Function;   2 - Time Function\n");
    const std::int32_t r = read_int(self(m), -9999, 2, fmt(" Movement Definition Method =   %d ?", v), true);
    if (r != -0xf423d) {
        v = r;
    }
    if (v < 0) {
        d->u54 = v < 0 ? -v : v;
    }
    else if (v == 0) {
        rt::crt::fprintf(stdout, " %s =  %lf ? ", name, d->value);
        const double x = read_double(m);
        if (kNoValue != x) {
            d->value = x;
        }
    }
    else {
        not_implemented(); // 1400017d0
    }
}

// @src 140015e90  movement control
void form_movement(MenuRec* m, void* mv)
{
    std::fprintf(stdout, " Enter Movement Control Method: \n");
    std::fprintf(stdout, "     0 - %s; 1 - %s; 2 - %s; 3 - %s; 4 - %s; \n", option_label(0x1405bede0, 0).c_str(),
                 option_label(0x1405bede0, 1).c_str(), option_label(0x1405bede0, 2).c_str(),
                 option_label(0x1405bede0, 3).c_str(), option_label(0x1405bede0, 4).c_str());
    const std::int32_t r =
        read_int(self(m), 0, 4, fmt(" Movement Control Method =     %d ? ", field<std::int32_t>(mv, 0x68)), true);
    if (r != -0xf423d) {
        field<std::int32_t>(mv, 0x68) = r;
    }
    const std::int32_t t = field<std::int32_t>(mv, 0x68);
    if (t == 0) {
        form_movement_value(m, &field<DYNARY>(mv, 0x70), "Force");
    }
    else if (t == 1) {
        form_movement_value(m, &field<DYNARY>(mv, 0xe8), "Speed");
    }
    else if (t == 2) {
        std::fprintf(stdout, " Enter (Total Energy, Blow Efficiency, Mass) of the hammer \n");
        std::fprintf(stdout, " =>");
        double v[3] = {field<double>(mv, 0x1f0), field<double>(mv, 0x1f8), field<double>(mv, 0x1e8)};
        read_doubles(m, v, 3);
        field<double>(mv, 0x1f0) = v[0];
        field<double>(mv, 0x1f8) = v[1];
        field<double>(mv, 0x1e8) = v[2];
    }
    else if (t == 3) {
        std::fprintf(stdout, " Enter (Total Energy, Blow Efficiency, Moment of Inertia, Ram Movement/Rev.)\n");
        std::fprintf(stdout, "     of the ScrewPress =>");
        double v[4] = {field<double>(mv, 0x280), field<double>(mv, 0x288), field<double>(mv, 0x290),
                       field<double>(mv, 0x298)};
        read_doubles(m, v, 4);
        field<double>(mv, 0x280) = v[0];
        field<double>(mv, 0x288) = v[1];
        field<double>(mv, 0x290) = v[2];
        field<double>(mv, 0x298) = v[3];
    }
    else if (t == 4) {
        std::fprintf(stdout, " Enter (Total Displacement, # of strokes per unit time)  \n");
        std::fprintf(stdout, "     of the Mechanical Press =>");
        double v[2] = {field<double>(mv, 0x318), field<double>(mv, 0x320)};
        read_doubles(m, v, 2);
        field<double>(mv, 0x318) = v[0];
        field<double>(mv, 0x320) = v[1];
    }
    rt::crt::fprintf(stdout, " Direction Vector =   %lf  %lf  %lf ?", field<double>(mv, 0x50), field<double>(mv, 0x58),
                     field<double>(mv, 0x60));
    read_doubles(m, &field<double>(mv, 0x50), 3);
}

// @src 1400179f0  Object Characteristics
std::string form_characteristics(MenuRec* m)
{
    LLOBJ_DATA* o = obj(m);
    show_characteristics(m);
    if (is_3d(m)) {
        while (true) {
            const std::string sel = ext::select(m, true, 1, 9);
            switch (first_char(sel)) {
            case '/':
                return "/";
            case '0':
            case 'E':
            case 'e':
                return "";
            case '1':
            case '2':
            case '3':
                not_implemented();
                break;
            case '4': {
                LLOBJ_DATA* c = obj(m);
                rt::crt::fprintf(stdout, " REFTMP  =      %lf ? ", f64(c, 0x1eb8));
                const double d = read_double(m);
                if (kNoValue != d) {
                    f64(obj(m), 0x1eb8) = d;
                }
                break;
            }
            case '5':
                rt::crt::fprintf(stdout, " STROKE =   %lf  %lf  %lf? ", f64(o, 0x22d0), f64(o, 0x22d8), f64(o, 0x22e0));
                read_doubles(m, &f64(o, 0x22d0), 3);
                [[fallthrough]]; // 140017ca2: on into item 6
            case '6':
                form_movement(m, &field<std::byte>(o, 0x2160));
                break;
            case '7':
                ask_angular(m, o);
                break;
            case '8':
                std::fprintf(stdout, "  Define Center of Axis by: center point and a direction vector:\n");
                rt::crt::fprintf(stdout, "  Center Coord. = %lf  %lf  %lf ? ", f64(o, 0x28c0), f64(o, 0x28c8),
                                 f64(o, 0x28d0));
                read_doubles(m, &f64(o, 0x28c0), 3);
                rt::crt::fprintf(stdout, "  Direc. Vector = %lf  %lf  %lf ? ", f64(o, 0x28d8), f64(o, 0x28e0),
                                 f64(o, 0x28e8));
                read_doubles(m, &f64(o, 0x28d8), 3);
                [[fallthrough]]; // 140017eaf: on into item 9
            case '9': {
                std::fprintf(stdout, " Define rotational symmetry data, angle and axis by a vector and point :\n");
                rt::crt::fprintf(stdout, " Angle   =   %lf\n", f64(o, 0x2058));
                const double d = read_double(m);
                if (kNoValue != d) {
                    f64(o, 0x2058) = d;
                }
                rt::crt::fprintf(stdout, "  Vector = %lf  %lf  %lf ? ", f64(o, 0x2078), f64(o, 0x2080), f64(o, 0x2088));
                read_doubles(m, &f64(o, 0x2078), 3);
                rt::crt::fprintf(stdout, "  Point = %lf  %lf  %lf ? ", f64(o, 0x2060), f64(o, 0x2068), f64(o, 0x2070));
                read_doubles(m, &f64(o, 0x2060), 3);
                break;
            }
            case 'R':
            case 'r':
                show_characteristics(m);
                break;
            default:
                break;
            }
        }
    }
    while (true) {
        std::string prompt = "\n"; // str 140506938, the stale prompt of item 3
        const std::string sel = ext::select(m, true, 1, 13);
        const auto is = [&sel](const char* s) { return sel == s; };
        if (is("1") || is("2") || is("3") || is("4")) {
            if (!deformable(m)) {
                warn_not_applicable();
                return "";
            }
        }
        if (is("1")) {
            const double d = read_double_range(m, 0.0, kBig, fmt(" AVGSTR =     %lf ? ", f64(o, 0x1ea8)));
            if (kNoValue != d) {
                f64(o, 0x1ea8) = d;
            }
        }
        else if (is("2")) {
            const double d = read_double_range(m, 0.0, kBig, fmt("  LMTSTR =     %lf ? ", f64(o, 0x1ea0)));
            if (kNoValue != d) {
                f64(o, 0x1ea0) = d;
            }
        }
        else if (is("3")) {
            if (field<std::int32_t>(m->step, 0x850) != 2) {
                warn_not_applicable();
                return "";
            }
            std::fprintf(stdout, "Z strain rate definition type (0-2)  =     %d ?", i32(o, 0x1f30));
            const std::int32_t v = read_int(self(m), 0, 2, prompt, true);
            if (v != -0xf423d) {
                i32(o, 0x1f30) = v;
            }
            const double d = read_double_range(m, 0.0, kBig, fmt(" ZSTR =     %lf ? ", f64(o, 0x1f28)));
            if (kNoValue != d) {
                f64(o, 0x1f28) = d;
            }
        }
        else if (is("4")) {
            const double d = read_double_range(m, 0.0, kBig, fmt(" PENVOL   =     %lf ? ", f64(o, 0x1eb0)));
            if (kNoValue != d) {
                f64(o, 0x1eb0) = d;
            }
            const std::int32_t v = read_int(
                self(m), 0, 1, fmt(" PENVOL is: 0 - Variable; 1 - Constant  =     %d ?", i32(o, 0x1f20)), true);
            if (v != -0xf423d) {
                i32(o, 0x1f20) = v;
            }
        }
        else if (string_compare(sel, 0, sel.size(), "5", 1) == 0) {
            // " REFTMP =  %d ? " given the double (low 32 bits, see the file comment)
            const double d = read_double_range(m, 0.0, kBig, fmt(" REFTMP =  %d ? ", low32(f64(o, 0x1eb8))));
            if (kNoValue != d) {
                f64(o, 0x1eb8) = d;
            }
        }
        else if (string_compare(sel, 0, sel.size(), "6", 1) == 0) {
            prompt = fmt(" STROKE =      %lf  %lf ? ", f64(o, 0x22d0), f64(o, 0x22d8)); // never printed
            double v[2] = {f64(o, 0x22d0), f64(o, 0x22d8)};
            read_doubles(m, v, 2);
            f64(o, 0x22d0) = v[0];
            f64(o, 0x22d8) = v[1];
        }
        else if (string_compare(sel, 0, sel.size(), "7", 1) == 0) {
            form_movement(m, &field<std::byte>(o, 0x2160));
        }
        else if (string_compare(sel, 0, sel.size(), "8", 1) == 0) {
            // the stack slot [RSP+0x30] of 1400149e0 is, called from here, the start of the SSO
            // buffer of the trimmed selection 140007d30 returned ([RSP+0x80] of its frame): the
            // answer's bytes, its NUL, then bytes the port cannot know (taken as 0)
            std::uint32_t stale = 0;
            for (std::size_t k = 0; k < 4 && k < sel.size(); ++k) {
                stale |= static_cast<std::uint32_t>(static_cast<unsigned char>(sel[k])) << (8 * k);
            }
            form_movement_value(m, &field<DYNARY>(o, 0x2a20), "SPDLMT", static_cast<std::int32_t>(stale));
        }
        else if (string_compare(sel, 0, sel.size(), "9", 1) == 0) {
            if (dim_type(m) != 0) {
                warn_not_applicable();
                return "";
            }
            ask_angular(m, o);
        }
        else if (string_compare(sel, 0, sel.size(), "10", 2) == 0) {
            if (dim_type(m) == 0 && field<std::int32_t>(m->step, 0x850) != 2) {
                warn_not_applicable();
                return "";
            }
            std::fprintf(stdout, "  Enter Center of rotation (X,Y) and current angle\n");
            prompt = fmt("  CNTRAX =  %lf  %lf  %lf ? ", f64(o, 0x2790), f64(o, 0x2798), f64(o, 0x28b0)); // never printed
            double v[3] = {f64(o, 0x2790), f64(o, 0x2798), f64(o, 0x28b0)};
            read_doubles(m, v, 3);
            f64(o, 0x2790) = v[0];
            f64(o, 0x2798) = v[1];
            f64(o, 0x28b0) = v[2];
        }
        else if (string_compare(sel, 0, sel.size(), "11", 2) == 0) {
            const double d = read_double_range(m, 0.0, kBig, fmt("  TMPLMT =  %lf ? ", f64(o, 0x1f10)));
            if (kNoValue != d) {
                f64(o, 0x1f10) = d;
            }
        }
        else if (string_compare(sel, 0, sel.size(), "12", 2) == 0) {
            if (dim_type(m) != 0) {
                warn_not_applicable();
                return "";
            }
            not_implemented();
        }
        else if (string_compare(sel, 0, sel.size(), "13", 2) == 0) {
            const std::int32_t v = read_int(
                self(m), 1, 2, fmt(" Reference method 1 - Coord; 2 - Node =     %d ?", i32(o, 0x1ec8)), true);
            if (v != -0xf423d) {
                i32(o, 0x1ec8) = v;
            }
            if (i32(o, 0x1ec8) == 1) {
                std::fprintf(stdout, "Enter reference point coordinates\n");
                rt::crt::fprintf(stdout, " REFPOS =   %lf %lf ? ", f64(o, 0x1ed0), f64(o, 0x1ed8));
                double p[2] = {f64(o, 0x1ed0), f64(o, 0x1ed8)};
                read_doubles(m, p, 2);
                f64(o, 0x1ed0) = p[0];
                f64(o, 0x1ed8) = p[1];
            }
            else {
                std::fprintf(stdout, "  Enter reference node\n");
                const std::int32_t n =
                    read_int(self(m), 1, 999999, fmt("  Node number =     %d ? ", i32(o, 0x1ee8)), true);
                if (n != -0xf423d) {
                    i32(o, 0x1ee8) = n;
                }
            }
        }
        else if (string_compare(sel, 0, sel.size(), "r", 1) == 0 || string_compare(sel, 0, sel.size(), "R", 1) == 0) {
            show_characteristics(m);
        }
        else if (string_compare(sel, 0, sel.size(), "e", 1) == 0 || string_compare(sel, 0, sel.size(), "E", 1) == 0 ||
                 string_compare(sel, 0, sel.size(), "0", 1) == 0) {
            return "";
        }
        else if (string_compare(sel, 0, sel.size(), "/", 1) == 0) {
            return "/";
        }
    }
}

// @src 140012570  die definition type
void ask_die_definition_type(MenuRec* m, bool allow_none)
{
    std::fprintf(stdout, " Die Geometry Definition Type:  1 - Polygons;  2 - Points\n");
    const std::string prompt = fmt("  Definition Type =     %d ? ", field<std::int32_t>(m, 0x18));
    const std::int32_t v = allow_none ? read_int(self(m), 0, 2, prompt, true) : read_int(self(m), 1, 2, prompt, true);
    if (v != -0xf423d) {
        field<std::int32_t>(m, 0x18) = v;
    }
}

// @src 140012110  die points
void show_die_points(MenuRec* m, const double* xyz, std::int32_t n)
{
    if (xyz == nullptr || n == 0) {
        return;
    }
    std::int32_t lo = 1;
    std::int32_t hi = n;
    read_int_pair(m, &lo, &hi, fmt("  Display Range (Start, End) =     1    %d ?", n), true);
    object_header(m);
    std::fprintf(stdout, " ***    Point Coordinates of Die    %d ***\n\n", i32(obj(m), 0x8));
    std::fprintf(stdout, "     No.           X                 Y                 Z \n");
    std::fprintf(stdout, " ------   ---------------   ---------------   ---------------\n");
    for (std::int32_t i = lo; i <= hi; i = i + 1) {
        const double* p = xyz + static_cast<std::ptrdiff_t>(i - 1) * 3;
        rt::crt::fprintf(stdout, "  %5d       %lf          %lf      %lf\n", i, p[0], p[1], p[2]);
    }
    hit_return(); // 140009e90
}

// @src 140012340  die elements
void show_die_elements(MenuRec* m, const std::int32_t* nodes, std::int32_t n)
{
    if (nodes == nullptr || n == 0) {
        return;
    }
    std::int32_t lo = 1;
    std::int32_t hi = n;
    read_int_pair(m, &lo, &hi, fmt("  Display Range (Start, End) =     1    %d ?", n), true);
    object_header(m);
    std::fprintf(stdout, " ***   Element Connectivity of Die    %d ***\n\n", i32(obj(m), 0x8));
    std::fprintf(stdout, "    No.    Pt. 1    Pt. 2    Pt. 3    Pt. 4\n");
    std::fprintf(stdout, " ------   ------   ------   ------   ------\n");
    for (std::int32_t i = lo; i <= hi; i = i + 1) {
        const std::int32_t* e = nodes + static_cast<std::ptrdiff_t>(i - 1) * 4;
        std::fprintf(stdout, "  %5d   %5d   %5d   %5d   %5d\n", i, e[0], e[1], e[2], e[3]);
    }
    hit_return(); // 140009e90
}

// @src 140002690  die mesh
void generate_die_mesh(MenuRec* m)
{
    if (!is_3d(m)) {
        return;
    }
    if (!cmd::objpos::has_geometry(obj(m))) { // 140160cc0
        return;
    }
    ext::mesh_rigid_die(obj(m), stp(m), problem(m));
}

// @src 1400162e0  Rigid Die/Object Border Definition
std::string form_die_geometry(MenuRec* m)
{
    model::OBJ_GEO3D* g = model::geo_kw::surface(*obj(m), 0); // 140198620(obj, &0)
    std::int32_t& type = field<std::int32_t>(m, 0x18);
    while (true) {
        while (type == 0) {
            ask_die_definition_type(m, true);
            if (type == 0) {
                return "";
            }
        }
        if (type == 1) {
            while (true) {
                show_die_menu(m);
                const std::string sel = ext::select(m, true, 1, 7);
                const char c = first_char(sel);
                if (c == '/') {
                    return "/";
                }
                if (c == '0' || c == 'E' || c == 'e') {
                    return "";
                }
                if (c == '1') {
                    ask_die_definition_type(m, false);
                    if (type != 1) {
                        break; // back to the type switch
                    }
                    continue;
                }
                switch (c) {
                case '2': {
                    // the count is taken before the write access detaches the points (140005e70)
                    const std::int32_t n = static_cast<std::int32_t>(static_cast<std::uint32_t>(g->coords.size() / 3));
                    show_die_points(m, &g->coords.mutable_at(0), n);
                    break;
                }
                case '3':
                case '5':
                case '6':
                    not_implemented();
                    break;
                case '4': {
                    const std::int32_t n = static_cast<std::int32_t>(static_cast<std::uint32_t>(g->elems.size() >> 2));
                    show_die_elements(m, &g->elems.mutable_at(0), n); // 140005100
                    break;
                }
                case '7':
                    generate_die_mesh(m);
                    break;
                case 'R':
                case 'r':
                    show_die_menu(m);
                    break;
                default:
                    break;
                }
            }
            continue;
        }
        if (type == 2) {
            while (true) {
                show_die_menu_points(m);
                const std::string sel = ext::select(m, true, 1, 3);
                const char c = first_char(sel);
                if (c == '/') {
                    return "/";
                }
                if (c == '0' || c == 'E' || c == 'e') {
                    return "";
                }
                if (c == '1') {
                    ask_die_definition_type(m, false);
                    if (type != 2) {
                        break;
                    }
                    continue;
                }
                if (c == '2') {
                    const std::int32_t n = static_cast<std::int32_t>(static_cast<std::uint32_t>(g->coords.size() / 3));
                    show_die_points(m, &g->coords.mutable_at(0), n);
                }
                else if (c == '3') {
                    not_implemented();
                }
                else if (c == 'R' || c == 'r') {
                    show_die_menu_points(m);
                }
            }
        }
        // any other type: back to the type switch (the original loops there)
    }
}

// @src 140011cd0  BC functions
std::string form_bc_functions(MenuRec* m)
{
    show_bc_function_menu(m);
    while (true) {
        const std::string sel = ext::select(m, true, 1, 2);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1':
        case '2':
            not_implemented();
            break;
        case 'R':
        case 'r':
            show_object_menu(m); // 140006d10: the menu 5 list, not the BC menu
            break;
        default:
            break;
        }
    }
}

// @src 140010a90  remeshing criteria
std::string form_remesh(MenuRec* m)
{
    LLOBJ_DATA* o = obj(m);
    show_remesh_criteria(m);
    std::int32_t last = 0; // R14D: the last int read (RMSTEP, 2D MGNELM)
    while (true) {
        const std::string sel = ext::select(m, true, 1, 11);
        if (sel == "1") {
            std::fprintf(stdout, " Enter interference depth to trigger remeshing\n");
            const double d = read_double_range(m, -10.0, kBig, fmt(" RMDPTH =     %lf ? ", f64(o, 0x2dc8)));
            if (kNoValue != d) {
                f64(o, 0x2dc8) = d;
            }
        }
        else if (sel == "2") {
            std::fprintf(stdout, "  Enter stroke of the primary die to trigger remeshing\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" RMSTRK =     %lf ? ", f64(o, 0x2dd0)));
            if (kNoValue != d) {
                f64(o, 0x2dd0) = d;
            }
        }
        else if (sel == "3") {
            std::fprintf(stdout, "  Enter elapse time to trigger remeshing\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" RMTIME =     %lf ? ", f64(o, 0x2de0)));
            if (kNoValue != d) {
                f64(o, 0x2de0) = d;
            }
        }
        else if (sel == "4") {
            std::fprintf(stdout, "  Enter # of steps to trigger remeshing\n");
            last = read_int(self(m), 0, 9999, fmt(" RMSTEP   =     %d ? ", low32(f64(o, 0x2dd8))), true);
            if (last != -0xf423d) {
                f64(o, 0x2dd8) = static_cast<double>(last); // CVTDQ2PD
            }
        }
        else if (sel == "5") {
            if (is_3d(m)) {
                std::fprintf(stdout, "  Enter approximate # of elements (surface, object)\n");
                const std::string p = fmt(" MGNELM =  %d %d ? ", i32(o, 0x2d88), i32(o, 0x2d8c));
                std::int32_t v[2] = {i32(o, 0x2d88), i32(o, 0x2d8c)};
                read_ints(m, 1, 999999, v, p, 2, true);
                i32(o, 0x2d88) = v[0];
                i32(o, 0x2d8c) = v[1];
            }
            else {
                std::fprintf(stdout, "  Enter approximate # of elements for the new mesh\n");
                last = read_int(self(m), 1, 999999, fmt(" MGNELM =  %d ? ", i32(o, 0x2d80)), true);
                if (last != -0xf423d) {
                    i32(o, 0x2d80) = last;
                }
            }
        }
        else if (sel == "6") {
            std::fprintf(stdout, "  Enter maximum size ratio between the largest and smallest elements\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" MGSIZR =      %d ? ", low32(f64(o, 0x2d20))));
            if (last != -0xf423d) {
                f64(o, 0x2d20) = d;
            }
        }
        else if (sel == "7") {
            std::fprintf(stdout, "  Enter weighting factor for temperature distribution\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" MGWTMP   =     %d ? ", low32(f64(o, 0x2d28))));
            if (last != -0xf423d) {
                f64(o, 0x2d28) = d;
            }
        }
        else if (sel == "8") {
            std::fprintf(stdout, "  Enter weighting factor for strain distribution\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" MGWSTN   =     %d ? ", low32(f64(o, 0x2d30))));
            if (last != -0xf423d) {
                f64(o, 0x2d30) = d;
            }
        }
        else if (sel == "9") {
            std::fprintf(stdout, "  Enter weighting factor for strain rate distribution\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" MGWSTR   =     %d ? ", low32(f64(o, 0x2d38))));
            if (last != -0xf423d) {
                f64(o, 0x2d38) = d;
            }
        }
        else if (string_compare(sel, 0, sel.size(), "10", 2) == 0) {
            std::fprintf(stdout, "  Enter weighting factor for curvature\n");
            const double d = read_double_range(m, 0.0, kBig, fmt(" MGWCUV =      %d ? ", low32(f64(o, 0x2d48))));
            if (last != -0xf423d) {
                f64(o, 0x2d38) = d; // 140011753: MGWSTR's slot
            }
        }
        else if (string_compare(sel, 0, sel.size(), "11", 2) == 0) {
            std::fprintf(stdout, "  Enter Grid Resolution for (X, Y)\n");
            const std::string p = fmt(" MGGRID =  %d %d ? ", i32(o, 0x2dc0), i32(o, 0x2dc4));
            std::int32_t v[2] = {i32(o, 0x2dc0), i32(o, 0x2dc4)};
            read_ints(m, 1, 999999, v, p, 2, true);
            i32(o, 0x2dc0) = v[0];
            i32(o, 0x2dc4) = v[1];
        }
        else if (sel == "r" || sel == "R") {
            show_remesh_criteria(m);
        }
        else if (sel == "e" || sel == "E" || sel == "0") {
            return "";
        }
        else if (sel == "/") {
            return "/";
        }
    }
}

// ---- the menu -----------------------------------------------------------------------------------

// @src 14001a050  menu 5
void menu5_objects(MenuRec* m)
{
    DB_STEP_DATA* s = stp(m);
    const std::int32_t id = step_current_object(s, true);     // 1401caa00(step, 1)
    m->data = model::object_list_find(&s->objects, id);       // 1401d6ea0
    if (m->data == nullptr) {
        m->data = model::llobj_ctor::get_or_create(s, 1);                 // 14019ccc0
        if (m->data == nullptr) {
            return;
        }
    }
    std::string ret;
    bool done = false;
    while (true) {
        show_object_menu(m);
        const std::string sel = ext::select(m, false, 0, 0);
        const std::int32_t n = to_int(sel);
        const std::int32_t t = dim_type(m);
        bool redisplay = false;
        if ((static_cast<std::uint32_t>(n - 4) < 7 && t == 7) || (static_cast<std::uint32_t>(n - 6) < 5 && t == 0)) {
            warn_not_applicable();
        }
        else if (sel == "1") {
            const std::int32_t v =
                read_int(self(m), 1, 100, fmt("  Object Number =     %d ? ", i32(obj(m), 0x8)), true);
            if (v != -0xf423d) {
                LLOBJ_DATA* o = model::llobj_ctor::get_or_create(s, v);
                if (o != nullptr) {
                    step_set_current_object(s, v);
                    m->data = o;
                }
            }
        }
        else if (sel == "2") {
            std::fprintf(stdout, " OBJNAM = ");
            const std::string name = read_line(self(m));
            if (!name.empty()) {
                // unbounded strcpy into the 0x54-byte name, as the original
                std::memcpy(&field<char>(obj(m), 0x828), name.c_str(), name.size() + 1);
            }
        }
        else if (sel == "3") {
            std::fprintf(stdout, " Enter Object Type:  1 - %s; 2 - %s; 3 - %s; 4 - %s; 5 - %s; 6 - %s\n",
                         option_label(0x1405beca0, 1).c_str(), option_label(0x1405beca0, 2).c_str(),
                         option_label(0x1405beca0, 3).c_str(), option_label(0x1405beca0, 4).c_str(),
                         option_label(0x1405beca0, 5).c_str(), option_label(0x1405beca0, 6).c_str());
            const std::int32_t cur = i32(obj(m), 0xc);
            if (cur != 7) { // type 7: the prompt " OBJTYP =     0 ? " is built and nothing is read
                const std::int32_t v = read_int(self(m), 0, 7, fmt(" OBJTYP =     %d ? ", cur + 1), true);
                if (v != -0xf423d) {
                    i32(obj(m), 0xc) = v == 0 ? 7 : v - 1;
                }
            }
        }
        else if (sel == "4") {
            ret = form_characteristics(m);
        }
        else if (sel == "5") {
            ret = form_die_geometry(m);
        }
        else if (sel == "6" || sel == "7") {
            not_implemented();
        }
        else if (sel == "8") {
            ret = form_bc_functions(m);
        }
        else if (sel == "9") {
            not_implemented();
        }
        else if (string_compare(sel, 0, sel.size(), "10", 2) == 0) {
            ret = form_remesh(m);
        }
        else if (sel == "r" || sel == "R") {
            show_object_menu(m); // and again at the top of the loop
            redisplay = true;
        }
        else if (sel == "e" || sel == "E" || sel == "0") {
            done = true;
        }
        if (redisplay) {
            continue;
        }
        if (sel == "/" || ret == "/") {
            return;
        }
        if (done) {
            return;
        }
    }
}

} // namespace def::pre
