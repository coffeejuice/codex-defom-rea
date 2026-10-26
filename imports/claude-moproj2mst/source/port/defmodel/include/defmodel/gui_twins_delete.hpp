// defmodel/gui_twins_delete.hpp: DEF_PRE_64 "delete object" (14019cdc0) and its callees, whose
// byte-identical twins the DEF_GUI_MO_64 Save calls (0x1408bdf10 via the GUI wrapper 0x140c7ca50;
// docs/re/moproj2mst/mdt/L1/spec_L1B.md 7.6). None of these addresses is in the DEF_PRE batch
// ledger; status rows in docs/re/status/mdt-delete.tsv. Bodies in src/gui_twins_delete/.
// Decompile: deform-rea-work/out/mdtdel/del.c, del2.c.
#pragma once

#include <cstdint>

namespace def::model {

class Cls_14017a6c0;
class Cls_1401f46e0;
class Cls_1401fa020;
class DB_STEP_DATA;
class LLOBJ_DATA;

// 14019cdc0: delete object `no` when the object list (+0x1380) has it (14017aaf0): BC references
// to it in the other objects (1401a9640), its inter-object records (1401fae90), the object emptied
// (140161260, kind 7), the virtual slot 1 (argument 0) of its +0x3bf8 object when set (the field is
// left as it is), then the empty objects at the end of the list are removed (14019ce70, flag 0).
// null_hit (port only, may be null): set when 14019ce70 reached a missing list position, where the
// original dereferences a null object; the port stops there instead.
void step_delete_object(DB_STEP_DATA& s, std::int32_t no, bool* null_hit = nullptr);

// 1401a9640: in every existing object 1..u1378 (1401d6ed0) with a mesh (+0x87c) whose number is
// not `no`, the BC entries (int array +0x990, nodes (+0x880) * width (model +0x1640)) equal to -no
// become 0.
void step_clear_bc_refs(DB_STEP_DATA& s, std::int32_t no);

// 1401fae90: remove (1401fab70) every inter-object record whose master (+0x168) or slave (+0x16c)
// is `no`; the walk starts again at the head after each removal.
void pair_remove_object(Cls_1401fa020* l, std::int32_t no);

// 1401fab70: unlink record p from the list (head, tail and count kept) and delete it; false when p
// is not in the list.
bool pair_remove(Cls_1401fa020* l, Cls_1401f46e0* p);

// 14019ce70: from list position u1378 down to 1 (1401d6ed0): with *flag, an object without
// geometry (140160cc0) and mesh (+0x87c) becomes kind 7; stop at the first object whose kind is
// not 7 (140160f10), else remove the object whose NUMBER is the position (1401b05c0) and
// decrement u1378. True when something was removed. null_hit as in step_delete_object.
bool step_trim_empty_tail(DB_STEP_DATA& s, const char* flag, bool* null_hit = nullptr);

// 1401b05c0: remove (14017a960) the object of number `no` (14017aaf0; nothing when missing).
void object_list_remove_number(Cls_14017a6c0* l, std::int32_t no);

// 14017a960: unlink object o from the list (head, tail and count kept) and delete it (14019a290
// with 1); false when o is not in the list.
bool object_list_remove(Cls_14017a6c0* l, LLOBJ_DATA* o);

} // namespace def::model
