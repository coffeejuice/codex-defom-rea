# DIEPNT (id 300, group 2, case 0x9c)

Die point sets of the object's mesh (`mesh = *(obj+0x3c00)`). The mesh keeps a vector of
point-set records at `mesh+0x50`. Each record holds an id, a type, an integer `extra` (in the
corpus, the surface patch id matching POLYTP value-1000) and a list of 5-double points: x, y, z
plus two more values (1.0 and 0.3 in the corpus). **3D only.**

## KEY grammar (read)
    DIEPNT <objNo:%i> <setId:%i> <type:%i> <count:%i> [<extra:%i>]
    then <count> lines:  <i:%i> <x:%Flf> <y:%Flf> <z:%Flf> <p4:%Flf> <p5:%Flf>
The header comes from `FUN_140414100` (`"%i %i %i %i"`, rest of the keyword line, comma→space;
a missing value is 0). Each point is one line read by `FUN_140415ac0`
(`"%i %Flf %Flf %Flf %Flf %Flf"`). The leading index is ignored. Blank lines are not skipped:
a blank point line fails. Several DIEPNT records per object are normal, one per set.
**In 2D** the case does nothing and returns 0, so the line is not consumed (see POLYTP.md for the
consequence).

## Model storage
Point-set record (element of the vector at `mesh+0x50`, created by `FUN_1402fea10`):

| Offset | Field |
|---|---|
| +0x00 | int id (`FUN_1402feac0`) |
| +0x04 | int type (`FUN_1402b7470` / set `FUN_1402fe8e0`) |
| +0x08 | int extra (set `FUN_1402fe900`; the writer reads it only when type == 1, `FUN_1402b7480`) |
| +0x10 | point list (`FUN_1402b74b0`), elements of 0x28 bytes: +0 x, +8 y, +0x10 z, +0x18 p4, +0x20 p5 |

1. `set = FUN_1402fd020(mesh, &setId, &type)`: linear search of `mesh+0x50` for `id == setId`.
   If found, its type is updated when it differs. If not found, a new record
   {id = setId, type, extra 0, empty list} is appended. Returns the record.
2. `set+8 = extra` (always, whatever the type).
3. the point list is cleared (`FUN_1402ff5f0`).
4. for each point line: a zeroed point (`FUN_1402b7860`), then x, y, z (`FUN_1402fe850`),
   +0x18 = p4 (`FUN_1402fe8a0`), +0x20 = p5 (`FUN_1402fe8c0`), appended with `FUN_1402fed80`.

## Read errors and validation
* header helper fails (EOF, or blank rest of line) → **2**.
* `mesh == NULL` → **2** (nothing more consumed).
* a point line fails (EOF or blank line) → **2** immediately. The set already exists and holds the
  points read so far.
* `count ≤ 0` → set created/updated and cleared, return 0.
* no check of type or setId values. Return-code handling is as in `_common.md` §1.

## KEY writer
`FUN_1402ab2b0(model, f, obj, objNo)`, called by `FUN_1402a27a0` after POLYTP when
`FUN_140160db0(obj)` is true (`obj+0x3990` flag set, mesh non-NULL, and `mesh+0x50` vector
non-empty, `FUN_1402f93b0`). The surrounding conditions are as for POLYTP.md.
For each set k = 1..N (`FUN_1402fd3e0(mesh,&k)`):
* points `cnt` = list size. If `cnt == 0` the set is skipped entirely.
* `seq` = running number of non-empty sets written so far (1, 2, …). Sets are renumbered, and the
  stored id is **not** written.
* `"%-6s %7i %7i %7i %7i %7i\n"` name, objNo, seq, type, cnt, (type == 1 ? extra : 0)
* for each point j: `" %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n"` j+1, x, y, z, p4, p5
Returns 0.
Round-trip differences: set ids become 1..k in order, empty sets vanish, and `extra` of a non-type-1
set is written as 0.
Note: `FUN_1402ab530` (the DIEEDG writer) also prints the DIEPNT name for edge sets of unknown type;
see DIEEDG.md.

## Unit conversion
None.

## Helpers
* `FUN_140414100`: 4-int header line. `FUN_140415ac0`: `"%i"` + 5×`%Flf` line.
* `FUN_1402fd020(mesh,&id,&type)`: find-or-append point set. `FUN_1402fd3e0(mesh,&k)`: set k
  (1-based; out of range → a static empty record). `FUN_14019a820(mesh+0x50)`: set count.
* `FUN_1402fe900`: set extra. `FUN_1402ff5f0`: clear list. `FUN_1402fed80`: append point.
  `FUN_1402b7860`: zero point. `FUN_1402fe850` / `FUN_1402fe8a0` / `FUN_1402fe8c0`: point setters.
  `FUN_1402b7420` / `7430` / `7440` / `7450` / `7460`: point getters (+0 … +0x20).
* `FUN_140160db0`: writer condition. `FUN_1402ab2b0`: writer.

## Corpus check
`key\3D\SI\Extrusion\Lagrangian\Extrude_T_T_LAG_Cold.KEY` (2 files use DIEPNT, 28 occurrences)
parses exactly, with 1- and 2-point sets:
    DIEPNT       2       5       1       2       3
           1   -9.5000000000E+000    1.0499200000E+001    5.0000000000E+000    1.0000000000E+000    3.0000000000E-001
           2   -9.5000000000E+000    3.0306100000E+000    5.0000000000E+000    1.0000000000E+000    3.0000000000E-001

## Open questions
* Meaning of type (only 1 appears in the corpus) and of p4/p5. Users of `mesh+0x50` would settle it.
