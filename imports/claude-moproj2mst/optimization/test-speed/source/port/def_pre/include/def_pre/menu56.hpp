// def_pre/menu56.hpp: the edit menus 5 (Object Data Modification, FUN_14001a050) and 6
// (Inter-Object Data Modification, FUN_140019c20) of the TEXT_PRE menu shell, their forms and
// displays, and the DB_STEP_DATA helpers only they call (packet p10-02). Behaviour:
// docs/re/menus/MENU5_6.md.
//
// Both menus take a menu record of the main object Cls_14001c770 (forms.hpp MenuRec): menu 5
// main+0x008 {1, 10, object, step}, menu 6 main+0x0c0 {1, 4, -, step}. The forms read the main
// object's fields that follow the record at fixed offsets from it, as the original does:
//   menu 5  rec+0x18 int   die geometry definition type (main+0x020)
//   menu 6  rec+0x18 3 doubles offset, +0x30 3 doubles relative velocity, +0x48 int object to
//           position, +0x4c int reference object, +0x50 double interference depth, +0x58 3
//           doubles centre, +0x70 3 doubles axis, +0x88 double angle, +0x90 int position method,
//           +0x98 double BCC error limit, +0xa0 int object of "Initialize BCC" (main+0x0d8..0x160)
// so a record must sit in a block of at least 0xa8 bytes (tests use a byte image of
// main+0x000..0x190).
//
// The forms return the answer that ended them: "/" (exit to the main menu) or "" (E, e, 0).
// Input comes from std::cin, output goes to stdout.
#pragma once

#include <cstdint>
#include <string>

#include <def_pre/forms.hpp>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
class LLOBJ_DATA;
class Cls_1401f46e0;
} // namespace def::model

namespace def::pre {

// ---- menus --------------------------------------------------------------------------------------

// @src 14001a050  menu 5 "Object Data Modification" (m = main+0x08)
void menu5_objects(MenuRec* m);
// @src 140019c20  menu 6 "Inter-Object Data Modification" (m = main+0xc0)
void menu6_inter_object(MenuRec* m);

// ---- menu 5: displays ---------------------------------------------------------------------------

// @src 140006d10  the menu 5 list
void show_object_menu(MenuRec* m);
// @src 140006bd0  " " or "(Not Applicable)" after item `item` of the list
std::string not_applicable_tag(MenuRec* m, std::int32_t item);
// @src 140001690  "  *** WARNING: Not Applicable for this Object"
void warn_not_applicable();
// @src 140003f50  "Characteristics of Object" table
void show_characteristics(MenuRec* m);
// @src 140002510  the movement-control rows of the table (mv = object+0x2160)
void show_movement_rows(MenuRec* m, void* mv);
// @src 140004510  "Die Geometry of Object" menu (3D and 2D, 7 items)
void show_die_menu(MenuRec* m);
// @src 140004700  "Die Geometry of Object" menu, 3 items (3D only; prints nothing in 2D)
void show_die_menu_points(MenuRec* m);
// @src 140002450  "Function Definition of Object" (BC functions)
void show_bc_function_menu(MenuRec* m);
// @src 1400020c0  remeshing criteria table
void show_remesh_criteria(MenuRec* m);

// ---- menu 5: forms ------------------------------------------------------------------------------

// @src 1400179f0  item 4, Object Characteristics
std::string form_characteristics(MenuRec* m);
// @src 140015e90  movement control (MOVCTL) of mv = object+0x2160
void form_movement(MenuRec* m, void* mv);
// @src 1400149e0  one movement value: routine / constant / function (d = DYNARY of the block).
// `stale` stands for the uninitialised stack int [RSP+0x30] the original shows as the default
// for a function of a type other than 1 / 2 (MENU5_6.md section "Uninitialised default").
void form_movement_value(MenuRec* m, model::DYNARY* d, const char* name, std::int32_t stale = 0);
// @src 1400162e0  item 5, Rigid Die/Object Border Definition
std::string form_die_geometry(MenuRec* m);
// @src 140012570  "Die Geometry Definition Type" (allow_none: range 0..2, else 1..2)
void ask_die_definition_type(MenuRec* m, bool allow_none);
// @src 140012110  point coordinates of the die (xyz: n points of 3 doubles)
void show_die_points(MenuRec* m, const double* xyz, std::int32_t n);
// @src 140012340  element connectivity of the die (nodes: n elements of 4 ints)
void show_die_elements(MenuRec* m, const std::int32_t* nodes, std::int32_t n);
// @src 140002690  "7 - Generate Mesh" of the die menu (3D, object with geometry)
void generate_die_mesh(MenuRec* m);
// @src 140011cd0  item 8, Boundary Condition Function Definition
std::string form_bc_functions(MenuRec* m);
// @src 140010a90  item 10, Remeshing/Mesh Generation Criteria
std::string form_remesh(MenuRec* m);

// ---- menu 6 -------------------------------------------------------------------------------------

// @src 1400126f0  the menu 6 list and its selection
std::string inter_object_select(MenuRec* m);
// @src 140014780  item 1, Display Defined Objects
void show_objects(MenuRec* m);
// @src 140016c50  item 2, Object Interaction Status (and the modification form on "n")
std::string form_interaction_status(MenuRec* m);
// @src 140012870  Object Interaction Modification
std::string form_interaction(MenuRec* m);
// @src 140004a90  the Object Interaction Modification table of pair p
void show_interaction(MenuRec* m, model::Cls_1401f46e0* p);
// @src 140002750  the pair (b, a) of pair p (a, b) when a != b, else null
model::Cls_1401f46e0* reverse_pair(MenuRec* m, model::Cls_1401f46e0* p);
// @src 140013790  item 3, Position Objects
std::string form_positioning(MenuRec* m);
// @src 140004870  Object Positioning Menu
void show_positioning(MenuRec* m);
// @src 1400170b0  item 4, Generate Inter-object Boundary Conditions
std::string form_bcc(MenuRec* m);
// @src 140002770  Boundary Conditions Generation menu
void show_bcc_menu(MenuRec* m);
// @src 1400145d0  "3 - Initialize BCC": clear negative contact flags of one object
void initialize_bcc(MenuRec* m);

// ---- shared form helpers ------------------------------------------------------------------------

// @src 140008a00  two ints "lo hi" in [*lo, *hi] (display ranges); see the field rules of
// read_doubles: only a single number passes
void read_int_pair(MenuRec* m, std::int32_t* lo, std::int32_t* hi, const std::string& prompt,
                   bool complain);
// @src 140009100  n ints from one line into out[0..n-1], each in [lo, hi]
void read_ints(MenuRec* m, std::int32_t lo, std::int32_t hi, std::int32_t* out,
               const std::string& prompt, std::int32_t n, bool complain);
// @src 1400030d0  std::string::compare(pos, n, s, len) (VS2008)
std::int32_t string_compare(const std::string& a, std::size_t pos, std::size_t n, const char* s,
                            std::size_t len);

// ---- DB_STEP_DATA helpers -----------------------------------------------------------------------

// @src 1401ca9e0  step+0x1b4c = id (current object of menu 5)
void step_set_current_object(model::DB_STEP_DATA* s, std::int32_t id);
// @src 1401caa00  current object of menu 5 (refresh: reset when beyond the last, else default)
std::int32_t step_current_object(model::DB_STEP_DATA* s, bool refresh);
// @src 1401caae0  step+0x1b50 = pair (current pair of menu 6)
void step_set_current_pair(model::DB_STEP_DATA* s, model::Cls_1401f46e0* p);
// @src 1401cab10  step+0x1b50
model::Cls_1401f46e0* step_current_pair(model::DB_STEP_DATA* s);
// @src 1401cab30  number of objects whose type is not 7
std::int32_t step_count_objects(model::DB_STEP_DATA* s);
// @src 1401cac80  id of the first object whose type is neither 7 nor 0, else 0
std::int32_t step_first_deformable(model::DB_STEP_DATA* s);
// @src 1401cad40  id of the first object of type 0, else 0
std::int32_t step_first_rigid(model::DB_STEP_DATA* s);
// @src 1401cae00  id of the last object at or below `from` (0: all) whose type is not 7, else
// (from below the count) the first such object, else 0
std::int32_t step_last_object(model::DB_STEP_DATA* s, std::int32_t from);
// @src 14019d5a0  inter-object pair (a, b) of the step, or null
model::Cls_1401f46e0* step_find_pair(model::DB_STEP_DATA* s, std::int32_t a, std::int32_t b);

} // namespace def::pre
