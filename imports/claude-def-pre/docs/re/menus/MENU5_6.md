# TEXT_PRE edit menus 5 and 6 (DEF_PRE_64.exe, packet p10-02)

Recovered from the decompile and the listing (code only; Ghidra copy `ghidra_p1002`), checked
against 32 scripted oracle sessions (`tests/menus/MENU5_6/cases.json`, generator
`make_cases.py`). Port: `port/def_pre/src/menu5_objects.cpp`, `menu6_inter_object.cpp`,
`menu56_common.cpp`; API `def_pre/menu56.hpp`; seams `def_pre/ext_p10_02.hpp`.

Notation: `obj+x` = LLOBJ_DATA field, `step+x` = DB_STEP_DATA field, `rec+x` = byte offset
from the menu record (the record is 0x18 bytes {lo, hi, data, step}; the forms read the main
object's fields that follow it). Prompts are quoted exactly (leading/trailing blanks matter).
"read_int(lo,hi)" = 140008600 (p4-01), "read_double" = 140008030, "read_double_range(lo,hi)" =
140008220, "read_doubles(n)" = 140007a80 (p10-01), "select(lo,hi)" = 140007d30 (shell).
"no value" = -999997.0 (DAT_140508b50) for the double readers, -0xf423d for read_int; a form
keeps the old value on "no value".

## 1. Common conventions

* Every selection prompt is `" Enter Selection (%d-%d, R, E, /): "`; forms call it with explicit
  bounds (1..n), the two menus with the record's {lo, hi}: menu 5 {1, 10} (main+0x08; the 2D list
  shows 12 items but 11/12 cannot be selected), menu 6 {1, 4} (main+0xc0).
* Forms return a string: `"/"` (answer `/`) or `""` (answers `E`, `e`, `0`). Menu 5 and menu 6
  return to the main menu when the selection or the last form's answer is `/`, or on E/e/0.
* Dispatch is on the first character (`strncpy(c, s, 1)`) in the switch-style forms (menu 6,
  BC functions, die geometry, characteristics 3D, interaction, BCC) and on whole-string equality
  (`std::string ==` / 1400030d0 compare) in menu 5, remesh, 2D characteristics and positioning.
* `R`/`r` redisplays; where the loop head also displays, the screen is printed twice (menu 5,
  die geometry, positioning).
* String tables (p1-02 `option_tables_data.hpp`): object type 0x1405beca0 (index type+1; type 7
  -> element 0 "Undefined"), movement 0x1405bede0, torque/RPUT 0x1405beeb0, die definition
  0x1405bef00, contact relation 0x1405bef80, no/yes 0x1405beab0, friction 0x1405bf000,
  separation 0x1405bf080, position method 0x1405bf100 (index method-1), PENVOL 0x1405bf280.
* Headers: `"\n\n"`, `" INPUT PREP. - OBJECT -\n"` (object forms) / `" INPUT PREP. -\n"`
  (menu 6) / `" INPUT PREP.\n"` (menu 5), `"-----------------------------------------------------\n"`,
  `" [%s]\n\n"` with step+0x340 (problem id). The inter-object forms start with
  `"\n  INPUT PREP. - INTER OBJECT -\n\n"`.

## 2. Menu 5, Object Data Modification (14001a050, record main+0x08)

Entry: id = 1401caa00(step, 1) (section 6); rec.data = object with that id (1401d6ea0); if none,
14019ccc0(step, 1) (get or create object 1); if still none, return.

Loop: list 140006d10, select, then by the answer:

| answer | action |
|---|---|
| n in 4..10 and type 7, or n in 6..10 and type 0 | `"\n\n  *** WARNING: Not Applicable for this Object\n"` (140001690) |
| `1` | `"  Object Number =     %d ? "` (obj+8) read_int(1,100); a valid id: 14019ccc0(step,id) (creates it, lower missing ids become type-7 placeholders), step+0x1b4c = id, rec.data = it |
| `2` | `" OBJNAM = "`, read_line; non-empty: unbounded strcpy to obj+0x828 |
| `3` | `" Enter Object Type:  1 - %s; ... 6 - %s\n"` (types 1..6); type 7: nothing is read; else `" OBJTYP =     %d ? "` (type+1) read_int(0,7): 0 -> 7, v -> v-1 |
| `4` | characteristics form (section 3) |
| `5` | die geometry form (section 4) |
| `6`, `7`, `9` | `"\n\n Function not implemented yet\n"` (1400017d0) |
| `8` | BC function form 140011cd0: menu 140002450, select(1,2); 1/2 not implemented; R shows the **menu 5 list** (140006d10) |
| `10` | remeshing form (section 5) |
| `R`, `r` | list again (then the loop head shows it again) |
| `E`, `e`, `0` | leave |

List 140006d10: `" ***   Object Data Modification Menu ***\n\n"`, `"   1 - Object Number[    %d]\n"`,
`"   2 - Object Name  [%s]\n"`, `"   3 - Object Type  [%s ]\n"`, items 4..10 (and 11, 12 in 2D) each
followed by the tag 140006bd0: `"(Not Applicable)"` for type 7, or type 0 and item > 5, else `" "`.

## 3. Characteristics (1400179f0)

Display 140003f50 (`" ***  Characteristics of Object    %d ***\n\n"`, `" Number  Keywrd  Values\n"`, ...).
3D rows: AVGSTR 1ea8, LMTSTR 1ea0, PENVOL 1eb0, REFTMP 1eb8, STROKE 22d0/22d8/22e0, MOVCTL by
label(21c8) + movement rows 140002510 (type 2 hammer 1f0/1f8/1e8 of mv, 3 screw 280..298, 4 press
318/320, always `"              Direction     = %f   %f   %f\n"` 50/58/60; mv = obj+0x2160),
ANGMOV label(2788) with value 2840 when 2788 == 1 else 27c8, CNTRAX 28c0..28e8, ROTSYM 2058 /
2078.. / 2060... 2D rows: ZSTR (int 1f30, 1f28), PENVOL 1eb0 + label(1f20), REFTMP, STROKE (2),
MOVCTL, SPDLMT DYNARY row (1400016d0, obj+0x2a20), ANGMOV label(2788), CNTRAX 2790/2798/27a0,
TMPLMT 1f10, ANGMVY label(2ac0), REFPOS.

3D form, select(1,9): 1-3 not implemented; 4 `" REFTMP  =      %lf ? "` read_double -> 1eb8;
5 `" STROKE =   %lf  %lf  %lf? "` read_doubles(3) into 22d0 **and falls into 6**; 6 movement form
140015e90(obj+0x2160); 7 angular: `" Enter Angular Movement Control Method: 0 - Torque; 1 - RPUT\n"`,
`"  Angular Movement Control Method =     %d ? "` read_int(0,1) -> 2788, then `"  Torque = %lf ? "`
(27c8) or `"  RPUT = %lf ? "` (2840) read_double; 8 center/direction read_doubles(3) into 28c0 and
28d8, **falls into 9**; 9 `" Angle   =   %lf\n"` read_double -> 2058, vector (2078) and point (2060)
read_doubles(3); R redisplay.

2D form, select(1,13), equality compare: 1/2/3/4 need type 1..4 else warning and the form ends;
1 AVGSTR read_double_range(0,1e30); 2 LMTSTR; 3 needs step+0x850 == 2 (else warning, end), prints
`"Z strain rate definition type (0-2)  =     %d ?"` and calls read_int(0,2) with the **stale prompt
"\n"** (the loop's initial string), then ZSTR; 4 PENVOL then `" PENVOL is: 0 - Variable; 1 -
Constant  =     %d ?"` read_int(0,1); 5 `" REFTMP =  %d ? "` given the **double** (low 32 bits of its
bits); 6 STROKE: the prompt is formatted but **never printed**, read_doubles(2); 7 movement form;
8 1400149e0(obj+0x2a20, "SPDLMT"); 9 type 0 only (else warning, end): angular; 10 (type 0 needs
step+0x850 == 2) `"  Enter Center of rotation (X,Y) and current angle\n"`, prompt not printed,
read_doubles(3) into 2790, 2798, **28b0**; 11 TMPLMT; 12 type 0 only, then not implemented;
13 `" Reference method 1 - Coord; 2 - Node =     %d ?"` read_int(1,2) -> 1ec8; 1: REFPOS 1ed0/1ed8
read_doubles(2); else `"  Node number =     %d ? "` read_int(1,999999) -> 1ee8.

Movement form 140015e90(mv): `" Enter Movement Control Method: \n"`, the five labels, read_int(0,4)
-> mv+0x68; 0 force 1400149e0(mv+0x70, "Force"), 1 speed (mv+0xe8, "Speed"), 2/3/4 read_doubles
of (1f0,1f8,1e8) / (280..298) / (318,320) after their `" =>"` prompts; then `" Direction Vector =
%lf  %lf  %lf ?"` read_doubles(3) into mv+0x50.

Movement value 1400149e0(d, name): default = d+0x54 (routine); when 0 and d not constant: type 1
-> 2, type 2 -> 1, **else an uninitialised stack int [RSP+0x30]** (see section 8);
`" Movement Definition Method =   %d ?"` read_int(-9999, 2) (a '-' never passes is_digits); < 0:
d+0x54 = |v|; 0: `" %s =  %lf ? "` read_double -> d+8; > 0 not implemented.

## 4. Die geometry (1400162e0, type at rec+0x18 = main+0x20)

g = obj->u3c00[0] (140198620). While the type is 0: 140012570(allow 0): `" Die Geometry Definition
Type:  1 - Polygons;  2 - Points\n"`, `"  Definition Type =     %d ? "` read_int(0,2) (item 1 of the
menus: read_int(1,2)); still 0 -> return "". Type 1: menu 140004510 (7 items; point count
coords.size()/3, connectivity count elems.size()/4, "0" texts when g is null) + select(1,7):
1 type again (leaves the loop when it changes); 2 140012110 points; 4 140012340 connectivity;
3/5/6 not implemented; 7 140002690 (3D with geometry: _MeshGenBase sequence, seam); R. Type 2:
140004700 (3D only; prints nothing in 2D) + select(1,3): 1, 2, 3 not implemented, R.
Displays 140012110/140012340: nothing when the array is empty; range 140008a00 with
`"  Display Range (Start, End) =     1    %d ?"` (one number n gives n..n; two fields never pass,
see p10-01 read_doubles field rules; an out-of-range number complains and asks again), then the
table and `"\n Hit <CR> to continue ..."`.

## 5. Remeshing criteria (140010a90)

Display 1400020c0: RMDPTH 2dc8, RMSTRK 2dd0, RMTIME 2de0, **RMSTEP `%d` given the double 2dd8**,
3D MGNELM 2d88/2d8c/2d90/2d94, MGSIZR 2d20, MGWTMP 2d28, MGWSTN 2d30, MGWSTR 2d38, MGWCUV 2d48,
MGGRID 2dc0/2dc4; 2D MGNELM 2d80, **MGTELM `%lf` given the int 2d84** (zero-extended bits), the
rest renumbered, MGERR 2db0/2db8.
Form select(1,11): 1 RMDPTH read_double_range(-10, 1e30); 2 RMSTRK, 3 RMTIME (0, 1e30);
4 `" RMSTEP   =     %d ? "` (double bits) read_int(0,9999) -> (double) at 2dd8; 5 3D
140009100(1,999999, 2 ints) into 2d88/2d8c, 2D read_int(1,999999) -> 2d80; 6..10 MGSIZR, MGWTMP,
MGWSTN, MGWSTR, MGWCUV prompts with `%d` given the doubles; the answer is stored **unless the last
int read by this form (RMSTEP or 2D MGNELM, register R14D, 0 at entry) was "no value"** - so "no
value" -999997 itself is stored; item 10 stores into **2d38** (MGWSTR); 11 MGGRID 2 ints.

## 6. Step helpers

1401caa00(step, refresh): n = 1401cae00(step, 0) (last object id whose type != 7); if n <
step+0x1b4c: current = 0; else if current == 0: current = first object neither type 7 nor 0
(1401cac80), else first type 0 (1401cad40). 1401cab30 counts objects of type != 7 (n-th object
1..step+0x1378). The searches give 0 when exhausted (the loop's result variable is only set on a
match). Current pair: step+0x1b50 (1401caae0 / 1401cab10). 14019d5a0(step, a, b) = 1401facf0 on
step+0x13c8.

## 7. Menu 6, Inter-Object Data Modification (140019c20, record main+0xc0)

No object of type != 7: `"  No Object Defined; "` + hit return. Loop: menu 1400126f0 + select:
0/E/e leave; 1 object status 140014780 (`"     %2d    %8s    %6d      %6d           \n"` id, type,
obj+0x880, +0x884; `"  Graphics Option =     0 ? "` read_int(0,2), 1/2 not implemented);
2 status 140016c50 (its result is dropped); 3 positioning (ret); 4 BCC (ret).

Status 140016c50: pairs (i, j), i <= j, both objects not type 7, with relation 1/2:
`"    %6d     %6d     %s (%s)\n"`; `"\n Modify Object Interactions? [Y]"` read_line; **an answer
starting with n/N opens the form** 140012870 and the status returns its answer; any other answer
shows the status again (unless the earlier answer was "/").

Interaction form 140012870: when the pair count step+0x13d8 is 0 a pair is appended, made current
and given (1, 1) (KEY reading does not count pairs, so this happens after every KEY read); no
current pair: the first. rev = pair (b, a) when a != b (140002750). Table 140004a90 (Object Pair,
CNTACT label + Separable, FRCFAC label(+0), `"    "` + IHTCOF DYNARY row (+0x1b8), SEPRES label(+0x178)).
select(1,5): 1 `" Enter Object Pair =     %d    %d ?"` 140009100(1, objects, 2) (one number n
gives (n, n)), find or append the pair, current, table; 2 five lines, `"\n  CNTACT =     %d ?"`
(current 0..4 from relation/separable) read_int(0,4) **without the range complaint**; 1..4 set
(relation, separable) = (1,1) (2,1) (1,0) (2,0), with the relation swapped 1<->2 when rev
exists; 0 relation 0; 3 `"Friction Type =     %d ?"` read_int(1,2) -> +0, method question with
default from the DYNARY +0x18 (constant 0, type 1 -> 1, type 4 -> 2, else the method last answered
in this form), printed `" Friction Factor Definition Method =     %d ?"` then read_int(0,2) **with
the Friction Type prompt again**; 0 -> `" FRCFAC  =      %lf ? "` read_double_range(0,1) -> +0x20;
1/2 not implemented; 4 the same for IHTCOF (+0x1b8, value +0x1c0, range 0..1e30, prompt passed to
read_int); 5 `" Pressure Separation Criteria: 0 - System, 1 - User (%), 2 - User (Value)\n"` - a
format: the VS2008 CRT prints `User ()` -, read_int(0,2) -> +0x178, `" SEPRES =   %lf ?"`
(0..1e30) -> +0x180; R table.

Positioning 140013790 (rec+0x18..0x90): menu 140004870, select(1, method 1 ? 5 : 6), equality
compare: 1 method read_int(1,3) -> +0x90; 2 object to position read_int(1, objects) -> +0x48;
method 1: 3 offset read_doubles(3) (+0x18), 4 position; method 2: 3 `" Reference Object =
%d ?"` **showing +0x48** -> +0x4c, 4 relative velocity (+0x30), 5 `" Interference Depth =   %lf ?"`
read_double (+0x50), 6 position; method 3: 3 centre (+0x58), 4 axis (+0x70), 5 angle (+0x88), 6.
Position: object +0x48 (printf `"ERROR  : Object %d Undefined\n"` for type 7); 1 14021d0b0(obj,
offset, &0, &1) and `"Object %d has been moved by (%g, %g %g).\n"`; 2 reference checks (type 7,
no die geometry 140203980 and no mesh +0x87c) then 1401a0a10(step, obj, ref, velocity, &depth,
&dist, disp, 1, &byte 0) -> "is interference positioned w.r.t." / "interference positioning
failed."; 3 14021d2f0(obj, centre, axis, angle) and the rotation message; then hit return.

BCC 1400170b0: menu 140002770 (error limit +0x98), select(1,3): 1 `"  Enter Error Limit =   %lf ?"`
read_double; 2 1401a6c80(step, limit) + hit return; 3 1400145d0: `"  Object Number  =     %d ?"`
read_int(1, objects) -> +0xa0; object with a mesh (+0x87c): the ints obj+0x990[0 ..
step+0x1640 * obj+0x880) below 0 become 0, `"  Contact for object   %d  initialized\n\n"`, hit return.

## 8. Open items

* 1400149e0's default for a non-constant movement function of type other than 1/2 is the
  uninitialised [RSP+0x30]. Called from the 2D characteristics (item 8) that slot is the start of
  the SSO buffer of the selection string 140007d30 left at its [RSP+0x80] (bytes of the answer, its
  NUL, then stale bytes taken as 0 by the port; the oracle case `m5_char_2d` shows 56 = "8").
  Through 140015e90 the slot lies in read_int's frame (the size word of its trimmed answer); the
  port passes 0 there (no case reaches it).
* read_int with a prompt shorter than 0x17 characters (2D characteristics 3, "\n") reads its
  empty-answer default past the text (byte 7 of the string object's size word in the original, 0):
  the port's std::string gives the same "" for SSO strings; scripts avoid empty answers there.
* 1401a6c80 (GENCTC 3D, p67-6) is called through `ext::genctc_3d`, which forwards to
  `def::cmd::genctc_3d` (defcmd/contact_cmds.hpp). Its cube routines (1401a7710 ..., p67-7,
  `defcmd/src/ctc3d/stubs_cube.cpp`) are still DEF_NOT_PORTED, so the replay skips
  `m6_bcc_generate_3d` unless MENU56_GENCTC=1.
* The _MeshGenBase sequence of 140002690 (p8-06/p8-07) is a seam (DEF_NOT_PORTED until those
  packets are in the tree); no case reaches it.
* 1401a0a10 is p67-2's exported `ifd::fun_1401a0a10`; menu 6 passes its byte 0 as the `one` flag.
