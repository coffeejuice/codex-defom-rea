// def_pre/forms.hpp: the edit menus 3 (simulation controls) and 4 (material) of the TEXT_PRE menu
// shell and the form readers they share with menus 5-6 (packet p10-01; p10-02 uses the helpers).
//
// Every menu function of the original takes a pointer to a 0x18-byte menu record of the main
// session object Cls_14001c770 (records at main+0x008, +0x030, +0x048, ... +0x0c0, +0x188):
//     { int lo; int hi; data*; step* }
// lo/hi are the selection range the prompt 140007d30 shows when it is not given explicit bounds,
// `data` is the block being edited (sim-control S = step+0x750, a Material, ...), `step` the
// DB_STEP_DATA. Menu 3 gets main+0x30 and uses the four records that follow it (+0x18..+0x70) for
// its sub-forms; menu 4 gets main+0xa8. The main loop 14001ad90 (p5-01) fills data/step before
// the call.
//
// Input comes from std::cin (the original's istream at PTR_DAT_14063a030), output goes to stdout.
#pragma once

#include <cstdint>
#include <string>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
class Material;
} // namespace def::model

namespace def::pre {

// One menu record (layout of the records in Cls_14001c770).
struct MenuRec {
    std::int32_t lo;   // 0x00
    std::int32_t hi;   // 0x04
    void* data;        // 0x08
    void* step;        // 0x10 DB_STEP_DATA*
};
static_assert(sizeof(MenuRec) == 0x18);

// "no answer" value of the double readers (DAT_140508b50); callers keep the old value on it
inline constexpr double kNoValue = -999997.0;

// ---- menus --------------------------------------------------------------------------------------

// 140017540: menu 3 "Simulation Controls Modification": m = main+0x30 (m[0..4] used)
void menu3_sim_controls(MenuRec* m);
// 1400157c0: menu 4 "Material Properties Modification": m = main+0xa8
void menu4_material(MenuRec* m);

// ---- form readers (shared with p10-02) ----------------------------------------------------------

// 140007a80: flush stdout, read one line (255 chars), trim (1401b1060); for i in 0..n-1 take the
// i-th blank-delimited field (field_of), stop at the first that is not all digits (is_digits,
// 1401b1390), else out[i] = to_double(field). Note the field rules of field_of: with several
// fields field 0 keeps its trailing blank and so never passes; a single number fills every slot.
void read_doubles(MenuRec* m, double* out, std::int32_t n);
// 140008030: flush, read a line, trim; a number (is_number) -> to_double, else kNoValue
double read_double(MenuRec* m);
// 140008220: loop: print prompt, read, trim; not a number -> kNoValue; lo <= v <= hi -> v; else
// " ??? Invalid Data; Valid Data Range = (     %lf to      %lf)\n" and ask again.
double read_double_range(MenuRec* m, double lo, double hi, const std::string& prompt);

// 1400016b0: "Editing function is not supported in this version.\n" (the arguments are unused)
void edit_not_supported();
// 1400017d0: "\n\n Function not implemented yet\n"
void not_implemented();
// 1400016d0: one table row of a DYNARY property: constant -> "   %d     %s   %lf\n", else
// "   %d     %s   %d Function\n" and the (abscissa, value) rows of its first axis
void print_dynary_row(const model::DYNARY& d, std::int32_t number, const char* name);

// ---- string helpers -----------------------------------------------------------------------------

// 1401b08d0: sscanf("%lf"), 0.0 when it does not convert
double to_double(std::string s);
// 1401b1240: the n-th field of s delimited by `delim` (see read_doubles for the quirks)
std::string field_of(std::string s, char delim, std::int32_t n);
// 1401b1450: characters [start, size-start) are digits, '-' or '.' (an empty range is true)
bool is_number(std::string s, std::int32_t start);
// 14012fa90: DYNARY is a constant (ndim == 0)
bool dynary_is_constant(const model::DYNARY& d);

} // namespace def::pre
