// Main menu 6 "Inter-Object Data Modification" (packet p10-02): FUN_140019c20, its displays and
// its forms (object status, interactions, positioning, BCC generation). The record m is
// main+0xc0 {1, 4, -, step}; the positioning and BCC fields follow it (see menu56.hpp).
// Behaviour and quirks: docs/re/menus/MENU5_6.md (all kept as in the disassembly), e.g.
// * "Modify Object Interactions? [Y]" opens the form only on an answer starting with n/N; any
//   other answer shows the status again;
// * the interaction form appends the pair (1, 1) when the pair list's count is 0 (the KEY reader
//   does not count the pairs it adds), and the CNTACT choice is mirrored when the reverse pair
//   exists;
// * friction item 3 reads the definition method with the "Friction Type" prompt again;
// * positioning item 3 of the Interference method shows the object to be positioned as the
//   default of the reference object.
#include <def_pre/menu56.hpp>

#include <cstdio>
#include <cstring>

#include <def_pre/ext_p10_01.hpp>
#include <def_pre/ext_p10_02.hpp>
#include <def_pre/forms.hpp>
#include <def_pre/menu7.hpp>
#include <defcmd/objpos.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "p10_01_common.hpp"

namespace def::model {
bool llobj_not_kind7(const LLOBJ_DATA* o); // 140160f10 (port/defmodel/src/material/state_remap.cpp)
}

namespace def::pre {

using model::Cls_1401f46e0;
using model::DB_STEP_DATA;
using model::DYNARY;
using model::field;
using model::LLOBJ_DATA;
using p1001::first_char;
using p1001::option_label;
using p1001::problem;
using p1001::self;

namespace {

DB_STEP_DATA* stp(MenuRec* m) { return static_cast<DB_STEP_DATA*>(m->step); }
std::int32_t& ri32(MenuRec* m, std::size_t off) { return field<std::int32_t>(m, off); }
double& rf64(MenuRec* m, std::size_t off) { return field<double>(m, off); }
double* rvec(MenuRec* m, std::size_t off) { return &field<double>(m, off); }
std::int32_t& pi32(Cls_1401f46e0* p, std::size_t off) { return field<std::int32_t>(p, off); }
std::int32_t nobj(MenuRec* m) { return field<std::int32_t>(m->step, 0x1378); }

// 1401d6ea0: the object with number id
LLOBJ_DATA* find_object(MenuRec* m, std::int32_t id) { return model::object_list_find(&stp(m)->objects, id); }

template <class... A>
std::string fmt(const char* f, A... a)
{
    char buf[208];
    format200(buf, f, a...);
    return std::string(buf);
}

constexpr double kBig = 1e30; // 140509b50

void plain_header(MenuRec* m)
{
    std::fprintf(stdout, "\n\n");
    std::fprintf(stdout, " INPUT PREP. -\n");
    std::fprintf(stdout, "-----------------------------------------------------\n");
    std::fprintf(stdout, " [%s]\n\n", problem(m));
}

// the default of a "0 - Constant; 1 - Time Function; 2 - Pressure Function" question: constant
// 0, time (type 1) 1, pressure (type 4) 2, else the last method answered in this form (R12D)
std::int32_t method_default(const DYNARY& d, std::int32_t last)
{
    if (dynary_is_constant(d)) { // 14012fa90
        return 0;
    }
    if (d.type == 1) {
        return 1;
    }
    return d.type == 4 ? 2 : last;
}

} // namespace

// ---- displays -----------------------------------------------------------------------------------

// @src 140014780  Display Defined Objects
void show_objects(MenuRec* m)
{
    plain_header(m);
    std::fprintf(stdout, "                  ***  Object Status  ***                         \n\n");
    std::fprintf(stdout, "  Object     Object     No. of     No. of     \n");
    std::fprintf(stdout, "  Number      Type      Nodes     Elements   \n");
    std::fprintf(stdout, " ========   ========   ========   ========  \n");
    for (std::int32_t i = 1; i <= nobj(m); i = i + 1) {
        auto* o = static_cast<LLOBJ_DATA*>(model::step_object_at(*stp(m), i)); // 1401d6ed0
        const std::int32_t t = field<std::int32_t>(o, 0xc);
        if (t != 7) {
            std::fprintf(stdout, "     %2d    %8s    %6d      %6d           \n", field<std::int32_t>(o, 0x8),
                         option_label(0x1405beca0, t + 1).c_str(), field<std::int32_t>(o, 0x880),
                         field<std::int32_t>(o, 0x884));
        }
    }
    std::fprintf(stdout, "  Graphics Options: 0 - Exit; 1 - Without Die Mesh; 2 - All Entities\n");
    const std::int32_t v = read_int(self(m), 0, 2, std::string("  Graphics Option =     0 ? "), true);
    if (v != -0xf423d && v != 0) {
        not_implemented();
    }
}

// @src 140004a90  Object Interaction Modification table
void show_interaction(MenuRec*, Cls_1401f46e0* p)
{
    std::fprintf(stdout, "\n  INPUT PREP. - INTER OBJECT -\n\n");
    std::fprintf(stdout, " ***  Object Interaction Modification Menu  ***\n\n");
    std::fprintf(stdout, "  Number     Keywrd       Values\n");
    std::fprintf(stdout, "  ======     ===========  ===============================\n");
    std::fprintf(stdout, "       1     Object Pair   %d    %d \n", pi32(p, 0x168), pi32(p, 0x16c));
    std::fprintf(stdout, "       2     CNTACT       %s   Separable(%s) \n", option_label(0x1405bef80, pi32(p, 0x170)).c_str(),
                 option_label(0x1405beab0, pi32(p, 0x174)).c_str());
    std::fprintf(stdout, "       3     FRCFAC        %s  \n", option_label(0x1405bf000, pi32(p, 0x0)).c_str());
    std::fprintf(stdout, "    ");
    print_dynary_row(field<DYNARY>(p, 0x1b8), 4, "IHTCOF"); // 1400016d0
    std::fprintf(stdout, "       5     SEPRES        %s  \n\n", option_label(0x1405bf080, pi32(p, 0x178)).c_str());
}

// @src 140004870  Object Positioning Menu
void show_positioning(MenuRec* m)
{
    std::fprintf(stdout, "\n  INPUT PREP. - INTER OBJECT -\n\n");
    std::fprintf(stdout, "  ***  Object Positioning Menu  ***\n\n");
    const std::int32_t method = ri32(m, 0x90);
    std::fprintf(stdout, "   1 - Position Method          [  %s ]\n", option_label(0x1405bf100, method - 1).c_str());
    std::fprintf(stdout, "   2 - Object to be Positioned  [  %d ]\n", ri32(m, 0x48));
    if (method == 1) {
        rt::crt::fprintf(stdout, "   3 - X, Y Offset Values       [  %lf  %lf  %lf ]\n", rf64(m, 0x18), rf64(m, 0x20),
                         rf64(m, 0x28));
        std::fprintf(stdout, "   4 - Position the Object\n");
    }
    else if (method == 2) {
        std::fprintf(stdout, "   3 - Reference Object         [  %d ]\n", ri32(m, 0x4c));
        rt::crt::fprintf(stdout, "   4 - Relative Velocity        [  %lf  %lf  %lf ]\n", rf64(m, 0x30), rf64(m, 0x38),
                         rf64(m, 0x40));
        rt::crt::fprintf(stdout, "   5 - Interference Depth       [  %lf]\n", rf64(m, 0x50));
        std::fprintf(stdout, "   6 - Position the Object     \n");
    }
    else if (method == 3) {
        rt::crt::fprintf(stdout, "   3 - Center                   [   %lf  %lf  %lf ]\n", rf64(m, 0x58), rf64(m, 0x60),
                         rf64(m, 0x68));
        rt::crt::fprintf(stdout, "   4 - Axis                     [   %lf  %lf  %lf ]\n", rf64(m, 0x70), rf64(m, 0x78),
                         rf64(m, 0x80));
        rt::crt::fprintf(stdout, "   5 - Rotation Angle           [      %lf ]\n", rf64(m, 0x88));
        std::fprintf(stdout, "   6 - Position the Object     \n");
    }
    std::fprintf(stdout, "\n");
}

// @src 140002770  Boundary Conditions Generation menu
void show_bcc_menu(MenuRec* m)
{
    std::fprintf(stdout, "\n  INPUT PREP. - INTER OBJECT -\n\n");
    std::fprintf(stdout, " ***  Boundary Conditions Generation  ***\n\n");
    rt::crt::fprintf(stdout, "   1 - Change Error Limit [      %lf]\n", rf64(m, 0x98));
    std::fprintf(stdout, "   2 - Generate Boundary Conditions\n");
    std::fprintf(stdout, "   3 - Initialize BCC\n\n");
}

// @src 1400126f0  menu 6 list and selection
std::string inter_object_select(MenuRec* m)
{
    plain_header(m);
    std::fprintf(stdout, "  ***  Inter-Object Data Definition Menu  ***\n\n");
    std::fprintf(stdout, "     1 - Display Defined Objects\n");
    std::fprintf(stdout, "     2 - Modify Object Interactions\n");
    std::fprintf(stdout, "     3 - Position Objects\n");
    std::fprintf(stdout, "     4 - Generate Inter-object Boundary Conditions\n");
    std::fprintf(stdout, "\n");
    return ext::select(m, false, 0, 0);
}

// ---- forms --------------------------------------------------------------------------------------

// @src 140002750  reverse pair
Cls_1401f46e0* reverse_pair(MenuRec* m, Cls_1401f46e0* p)
{
    const std::int32_t a = pi32(p, 0x168);
    const std::int32_t b = pi32(p, 0x16c);
    if (a == b) {
        return nullptr;
    }
    return step_find_pair(stp(m), b, a); // 14019d5a0
}

// @src 140012870  Object Interaction Modification
std::string form_interaction(MenuRec* m)
{
    DB_STEP_DATA* s = stp(m);
    if (field<std::int32_t>(s, 0x13d8) == 0) { // pair count
        Cls_1401f46e0* p = model::pair_append(&s->inter_obj); // 1401faaa0
        if (p == nullptr) {
            return "";
        }
        step_set_current_pair(s, p);
        pi32(p, 0x168) = 1;
        pi32(p, 0x16c) = 1;
    }
    Cls_1401f46e0* p = step_current_pair(s);
    if (p == nullptr) {
        p = model::pair_nth(&s->inter_obj, 1); // 14019d5e0(step, 1)
        step_set_current_pair(s, p);
    }
    Cls_1401f46e0* rev = reverse_pair(m, p);
    show_interaction(m, p);
    std::int32_t method = 0; // R12D
    while (true) {
        const std::string sel = ext::select(m, true, 1, 5);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1': {
            const std::string prompt = fmt(" Enter Object Pair =     %d    %d ?", pi32(p, 0x168), pi32(p, 0x16c));
            std::int32_t v[2] = {pi32(p, 0x168), pi32(p, 0x16c)};
            read_ints(m, 1, nobj(m), v, prompt, 2, true);
            p = step_find_pair(s, v[0], v[1]);
            if (p == nullptr) {
                p = model::pair_append(&s->inter_obj);
                pi32(p, 0x168) = v[0];
                pi32(p, 0x16c) = v[1];
            }
            step_set_current_pair(s, p);
            rev = reverse_pair(m, p);
            show_interaction(m, p);
            break;
        }
        case '2': {
            std::fprintf(stdout, "  0 - None\n");
            std::fprintf(stdout, "  1 - Slave-Master (Separable)\n");
            std::fprintf(stdout, "  2 - Master-Slave (Separable)\n");
            std::fprintf(stdout, "  3 - Slave-Master (Non-separable)\n");
            std::fprintf(stdout, "  4 - Master-Slave (Non-separable)\n");
            std::int32_t cur = 0;
            if (pi32(p, 0x170) == 2) {
                cur = pi32(p, 0x174) != 0 ? 2 : 4;
            }
            else if (pi32(p, 0x170) == 1) {
                cur = pi32(p, 0x174) != 0 ? 1 : 3;
            }
            const std::int32_t v = read_int(self(m), 0, 4, fmt("\n  CNTACT =     %d ?", cur), false);
            if (v == 0) {
                pi32(p, 0x170) = 0;
            }
            else if (v == 1) {
                pi32(p, 0x170) = rev != nullptr ? 2 : 1;
                pi32(p, 0x174) = 1;
            }
            else if (v == 2) {
                pi32(p, 0x170) = rev != nullptr ? 1 : 2;
                pi32(p, 0x174) = 1;
            }
            else if (v == 3) {
                pi32(p, 0x170) = rev != nullptr ? 2 : 1;
                pi32(p, 0x174) = 0;
            }
            else if (v == 4) {
                pi32(p, 0x170) = rev != nullptr ? 1 : 2;
                pi32(p, 0x174) = 0;
            }
            break;
        }
        case '3': {
            std::fprintf(stdout, " Enter Friction Type: 1 - Constant; 2 - Columb\n");
            const std::string prompt = fmt("Friction Type =     %d ?", pi32(p, 0x0));
            const std::int32_t t = read_int(self(m), 1, 2, prompt, true);
            if (t != -0xf423d) {
                pi32(p, 0x0) = t;
            }
            std::fprintf(stdout, " Enter Friction Factor Definition Method:\n");
            std::fprintf(stdout, "     0 - Constant; 1 - Time Function; 2 - Pressure Function\n");
            method = method_default(field<DYNARY>(p, 0x18), method);
            std::fprintf(stdout, " Friction Factor Definition Method =     %d ?", method);
            const std::int32_t r = read_int(self(m), 0, 2, prompt, true); // the Friction Type prompt again
            if (r != -0xf423d) {
                method = r;
            }
            if (method == 0) {
                const double d =
                    read_double_range(m, 0.0, 1.0, fmt(" FRCFAC  =      %lf ? ", field<double>(p, 0x20)));
                if (kNoValue != d) {
                    field<double>(p, 0x20) = d;
                }
            }
            else if (0 < method && method < 3) {
                not_implemented();
            }
            break;
        }
        case '4': {
            std::fprintf(stdout, " Enter Interface Heat Transfer Coef. Definition Method:\n");
            std::fprintf(stdout, "     0 - Constant; 1 - Time Function; 2 - Pressure Function\n");
            method = method_default(field<DYNARY>(p, 0x1b8), method);
            const std::int32_t r = read_int(
                self(m), 0, 2, fmt("Interface Heat Transfer Coef. Definition Method =     %d ?", method), true);
            if (r != -0xf423d) {
                method = r;
            }
            if (method == 0) {
                const double d =
                    read_double_range(m, 0.0, kBig, fmt("  IHTCOF =        %lf ? ", field<double>(p, 0x1c0)));
                if (kNoValue != d) {
                    field<double>(p, 0x1c0) = d;
                }
            }
            else if (0 < method && method < 3) {
                not_implemented();
            }
            break;
        }
        case '5': {
            // str 14050aa00 "... 1 - User (%), ..." is a format: the VS2008 CRT drops the '%' of
            // the conversion "%)" and prints the ')' (the oracle shows "User ()")
            std::fprintf(stdout, " Pressure Separation Criteria: 0 - System, 1 - User (), 2 - User (Value)\n");
            const std::int32_t v =
                read_int(self(m), 0, 2, fmt("  Separation Criteria =     %d ?", pi32(p, 0x178)), true);
            if (v != -0xf423d) {
                pi32(p, 0x178) = v;
            }
            const double d = read_double_range(m, 0.0, kBig, fmt(" SEPRES =   %lf ?", field<double>(p, 0x180)));
            if (kNoValue != d) {
                field<double>(p, 0x180) = d;
            }
            break;
        }
        case 'R':
        case 'r':
            show_interaction(m, p);
            break;
        default:
            break;
        }
    }
}

// @src 140016c50  Object Interaction Status
std::string form_interaction_status(MenuRec* m)
{
    std::string ret;
    while (true) {
        plain_header(m);
        std::fprintf(stdout, "       ***  Object Interaction Status  ***                        \n\n");
        std::fprintf(stdout, "   1st Obj.   2nd Obj.   Relation (Separable)    \n");
        std::fprintf(stdout, "  ========   ========   ====================\n");
        for (std::int32_t i = 1; i <= nobj(m); i = i + 1) {
            for (std::int32_t j = i; j <= nobj(m); j = j + 1) {
                if (!model::llobj_not_kind7(find_object(m, i))) { // 140160f10
                    continue;
                }
                if (!model::llobj_not_kind7(find_object(m, j))) {
                    continue;
                }
                Cls_1401f46e0* p = step_find_pair(stp(m), i, j);
                if (p == nullptr) {
                    continue;
                }
                const std::int32_t rel = pi32(p, 0x170);
                if (rel == 2 || rel == 1) {
                    std::fprintf(stdout, "    %6d     %6d     %s (%s)\n", pi32(p, 0x168), pi32(p, 0x16c),
                                 option_label(0x1405bef80, rel).c_str(), option_label(0x1405beab0, pi32(p, 0x174)).c_str());
                }
            }
        }
        std::fprintf(stdout, "\n Modify Object Interactions? [Y]");
        const std::string ans = read_line(self(m));
        if (starts_with(ans, "n", false)) { // str 14050911c
            ret = form_interaction(m);
        }
        bool done = true;
        if (!starts_with(ans, "n", false)) {
            done = starts_with(ret, "/", false);
        }
        if (done) {
            return ret;
        }
    }
}

// @src 140013790  Position Objects
std::string form_positioning(MenuRec* m)
{
    double dist = 0.0;                  // local_208
    double disp[3] = {0.0, 0.0, 0.0};   // local_200..local_1f0
    while (true) {
        show_positioning(m);
        const std::int32_t method = ri32(m, 0x90);
        const std::string sel = ext::select(m, true, 1, method == 1 ? 5 : 6);
        const char c = first_char(sel);
        const auto is = [&sel](const char* t) { return sel == t; };
        bool position = false;
        if (is("1")) {
            std::fprintf(stdout, " Enter Position Method: 1 - Offset; 2 - Interference; 3 - Rotation\n");
            const std::int32_t v = read_int(self(m), 1, 3, fmt("Position Method =     %d ?", ri32(m, 0x90)), true);
            if (v != -0xf423d) {
                ri32(m, 0x90) = v;
            }
        }
        else if (is("2")) {
            const std::int32_t v =
                read_int(self(m), 1, nobj(m), fmt(" Object to be Positioned =     %d ?", ri32(m, 0x48)), true);
            if (v != -0xf423d) {
                ri32(m, 0x48) = v;
            }
        }
        else if (is("3") && ri32(m, 0x90) == 1) {
            rt::crt::fprintf(stdout, " (X, Y, Z) Offset = %lf  %lf  %lf", rf64(m, 0x18), rf64(m, 0x20), rf64(m, 0x28));
            read_doubles(m, rvec(m, 0x18), 3);
        }
        else if (is("3") && ri32(m, 0x90) == 2) {
            // the default shown is the object to be positioned (+0x48)
            const std::int32_t v =
                read_int(self(m), 1, nobj(m), fmt(" Reference Object =    %d ?", ri32(m, 0x48)), true);
            if (v != -0xf423d) {
                ri32(m, 0x4c) = v;
            }
        }
        else if (is("4") && ri32(m, 0x90) == 2) {
            rt::crt::fprintf(stdout, "  Relative Velocity = %lf  %lf  %lf", rf64(m, 0x30), rf64(m, 0x38), rf64(m, 0x40));
            read_doubles(m, rvec(m, 0x30), 3);
        }
        else if (is("5") && ri32(m, 0x90) == 2) {
            rt::crt::fprintf(stdout, " Interference Depth =   %lf ?", rf64(m, 0x50));
            const double d = read_double(m);
            if (kNoValue != d) {
                rf64(m, 0x50) = d;
            }
        }
        else if (is("3") && ri32(m, 0x90) == 3) {
            rt::crt::fprintf(stdout, " (X, Y, Z) Center = %lf  %lf  %lf", rf64(m, 0x58), rf64(m, 0x60), rf64(m, 0x68));
            read_doubles(m, rvec(m, 0x58), 3);
        }
        else if (is("4") && ri32(m, 0x90) == 3) {
            rt::crt::fprintf(stdout, " (X, Y, Z) Axis = %lf  %lf  %lf", rf64(m, 0x70), rf64(m, 0x78), rf64(m, 0x80));
            read_doubles(m, rvec(m, 0x70), 3);
        }
        else if (is("5") && ri32(m, 0x90) == 3) {
            rt::crt::fprintf(stdout, " Rotation Angle (Deg) =    %lf ?", rf64(m, 0x88));
            const double d = read_double(m);
            if (kNoValue != d) {
                rf64(m, 0x88) = d;
            }
        }
        else if ((is("4") && ri32(m, 0x90) == 1) || is("6")) {
            position = true;
        }
        if (position) {
            LLOBJ_DATA* o = find_object(m, ri32(m, 0x48));
            if (field<std::int32_t>(o, 0xc) == 7) {
                std::printf("ERROR  : Object %d Undefined\n", field<std::int32_t>(o, 0x8));
            }
            const std::int32_t pm = ri32(m, 0x90);
            if (pm == 1) {
                const char a = 0;
                const char b = 1;
                cmd::objpos::translate(o, rvec(m, 0x18), &a, &b); // 14021d0b0
                rt::crt::fprintf(stdout, "Object %d has been moved by (%g, %g %g).\n", field<std::int32_t>(o, 0x8),
                                 rf64(m, 0x18), rf64(m, 0x20), rf64(m, 0x28));
            }
            else if (pm == 2) {
                LLOBJ_DATA* r = find_object(m, ri32(m, 0x4c));
                if (field<std::int32_t>(r, 0xc) == 7) {
                    std::fprintf(stdout, "ERROR  : Object %d Undefined\n", field<std::int32_t>(r, 0x8));
                }
                else if (!cmd::objpos::has_die_geometry(r) && field<std::int32_t>(r, 0x87c) == 0) { // 140203980
                    std::fprintf(stdout, "ERROR  : No Mesh or Die Defined for Object %d\n", field<std::int32_t>(r, 0x8));
                }
                char one = 0;
                if (ext::interference_position(stp(m), o, r, rvec(m, 0x30), rvec(m, 0x50), &dist, disp, 1, &one)) {
                    std::fprintf(stdout, "Object %d is interference positioned w.r.t. Object %d.\n",
                                 field<std::int32_t>(o, 0x8), field<std::int32_t>(r, 0x8));
                }
                else {
                    std::fprintf(stdout, "Object %d to Object %d interference positioning failed.\n",
                                 field<std::int32_t>(o, 0x8), field<std::int32_t>(r, 0x8));
                }
            }
            else if (pm == 3) {
                cmd::objpos::rotate(o, rvec(m, 0x58), rvec(m, 0x70), rf64(m, 0x88)); // 14021d2f0
                rt::crt::fprintf(stdout,
                                 "Object %d has been rotated by %g w.r.t. origin(%g, %g, %g) direction(%g, %g, %g).\n",
                                 field<std::int32_t>(o, 0x8), rf64(m, 0x88), rf64(m, 0x58), rf64(m, 0x60),
                                 rf64(m, 0x68), rf64(m, 0x70), rf64(m, 0x78), rf64(m, 0x80));
            }
            hit_return(); // 140009e90
        }
        switch (c) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case 'R':
        case 'r':
            show_positioning(m);
            break;
        default:
            break;
        }
    }
}

// @src 1400145d0  Initialize BCC
void initialize_bcc(MenuRec* m)
{
    const std::int32_t v =
        read_int(self(m), 1, nobj(m), fmt("  Object Number  =     %d ?", ri32(m, 0xa0)), true);
    if (v != -0xf423d) {
        ri32(m, 0xa0) = v;
    }
    LLOBJ_DATA* o = find_object(m, ri32(m, 0xa0));
    if (o == nullptr) {
        return;
    }
    if (field<std::int32_t>(o, 0x87c) == 0) {
        return;
    }
    const std::int32_t n = field<std::int32_t>(m->step, 0x1640) * field<std::int32_t>(o, 0x880);
    std::int32_t* flags = field<std::int32_t*>(o, 0x990);
    for (std::int64_t k = 0; k < n; k = k + 1) {
        if (flags[k] < 0) {
            flags[k] = 0;
        }
    }
    std::fprintf(stdout, "  Contact for object   %d  initialized\n\n", ri32(m, 0xa0));
    hit_return(); // 140009e90
}

// @src 1400170b0  Generate Inter-object Boundary Conditions
std::string form_bcc(MenuRec* m)
{
    while (true) {
        show_bcc_menu(m);
        const std::string sel = ext::select(m, true, 1, 3);
        switch (first_char(sel)) {
        case '/':
            return "/";
        case '0':
        case 'E':
        case 'e':
            return "";
        case '1': {
            rt::crt::fprintf(stdout, "  Enter Error Limit =   %lf ?", rf64(m, 0x98));
            const double d = read_double(m);
            if (kNoValue != d) {
                rf64(m, 0x98) = d;
            }
            break;
        }
        case '2':
            ext::genctc_3d(stp(m), rf64(m, 0x98)); // 1401a6c80
            hit_return();
            break;
        case '3':
            initialize_bcc(m);
            break;
        default:
            break;
        }
    }
}

// ---- the menu -----------------------------------------------------------------------------------

// @src 140019c20  menu 6
void menu6_inter_object(MenuRec* m)
{
    if (step_count_objects(stp(m)) == 0) { // 1401cab30
        std::fprintf(stdout, "  No Object Defined; ");
        hit_return();
        return;
    }
    std::string ret;
    bool done = false;
    while (true) {
        const std::string sel = inter_object_select(m);
        switch (first_char(sel)) {
        case '0':
        case 'E':
        case 'e':
            done = true;
            break;
        case '1':
            show_objects(m);
            break;
        case '2':
            (void)form_interaction_status(m); // its answer is dropped
            break;
        case '3':
            ret = form_positioning(m);
            break;
        case '4':
            ret = form_bcc(m);
            break;
        case 'R':
        case 'r':
            continue;
        default:
            break;
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
