# DIEEDG (id 301, group 2, case 0x9d)

Die edge sets of the object's mesh (`mesh = *(obj+0x3c00)`). The mesh keeps a vector of
edge-set records at `mesh+0x58`. Each record holds an id, a type (1 or 2), an `extra` int (type 1
only) and a list of node numbers. **3D only.**

## KEY grammar (read)
    DIEEDG <objNo:%i> <setId:%i> <type:%i> <count:%i> [<extra:%i>]
    then <count> integers (%i, whitespace separated, any line breaks)
The header comes from `FUN_140414100` (`"%i %i %i %i"`, rest of the keyword line, comma→space;
missing values are 0). Type 2 headers have no `extra`. The values are read with `count` ×
`fscanf("%i")`, so commas are **not** accepted there. No `fgets` follows, so the newline after the
last value stays in the stream, which is harmless for the next `%39s` keyword read.
**In 2D** the case does nothing and returns 0, so the line is not consumed (see POLYTP.md).

## Model storage
Edge-set record (vector at `mesh+0x58`, created by `FUN_1402fead0`): +0x00 id (`FUN_1402feb80`),
+0x04 type (`FUN_1402b74d0`, set by `FUN_1402fe930`), +0x08 extra (set by `FUN_1402fe950`; the
writer reads it only for type 1, `FUN_1402b74e0`), +0x10 int list (`FUN_1402b7510`).
1. `set = FUN_1402fd490(mesh, &setId, &type)`: find by id (updating the type if it differs), or
   append {id, type, extra 0, empty list}.
2. type == 1 → `set+8 = extra`.
3. type ∉ {1, 2} → return **2**. The set has already been created or its type changed.
4. clear the list (`FUN_14015a620`), then append each value read (`FUN_140220700`).

## Read errors and validation
* header helper fails (EOF, or blank rest of line) → **2**.
* `mesh == NULL` → **2**.
* type not 1 or 2 → **2** (after the side effect in step 3; values are not consumed).
* `fscanf` EOF on a value → **2**. A non-numeric token is not detected: `fscanf` returns 0, the
  value appended is the previous one (the variable is not reset per value), and the token stays
  in the stream, so every remaining value repeats it the same way.
* otherwise 0. Return-code handling is as in `_common.md` §1.

## KEY writer
`FUN_1402ab530(model, f, obj, objNo)`, called by `FUN_1402a27a0` after DIEPNT when
`FUN_140160e00(obj)` is true (`obj+0x3990` flag set, mesh non-NULL, and `mesh+0x58` non-empty,
`FUN_1402f93f0`). For each set k = 1..N (`FUN_1402fd740`):
* `cnt` = list size. If `cnt == 0` the set is skipped. `seq` = running number of written sets
  (renumbered from 1, including unknown-type sets).
* type 2: `"%-6s %7i %7i %7i %7i\n"` name, objNo, seq, 2, cnt
* type 1: `"%-6s %7i %7i %7i %7i %7i\n"` name, objNo, seq, 1, cnt, extra
* for both: the values as `" %7i"`, with `"\n"` before the 11th, 21st, … value (10 per line),
  then a final `"\n"`.
* any other type: `"%-6s %7i %7i %7i %7i\n"` with **the DIEPNT name** (`FUN_1402f7720(300)`),
  objNo, seq, type, 0, and no values. **Writer bug:** read back, this line becomes a DIEPNT
  record. It creates or retypes the point set with id `seq`, sets its extra to 0 and **clears its
  point list**, which corrupts a real DIEPNT set whose id equals `seq` (DIEEDG is written after
  DIEPNT). A byte-exact port must reproduce this output.
Returns 0.
Round-trip differences: ids are renumbered, empty sets dropped, unknown-type sets turn into
DIEPNT lines.

## Unit conversion
None.

## Helpers
* `FUN_140414100`: header line reader.
* `FUN_1402fd490(mesh,&id,&type)`: find-or-append edge set. `FUN_1402fd740(mesh,&k)`: set k
  (1-based; out of range → a static empty record). `FUN_14019a840(mesh+0x58)`: set count.
* `FUN_1402fe950`: set extra. `FUN_14015a620`: clear int list. `FUN_140220700`: append int.
  `FUN_14019a060`: list size. `FUN_140159a70(list,i)`: element i.
* `FUN_140160e00`: writer condition. `FUN_1402ab530`: writer.

## Corpus check
`key\3D\SI\Extrusion\Lagrangian\Extrude_T_T_LAG_Cold.KEY` (2 files use DIEEDG, 4 occurrences)
parses exactly: type 2, 57 values written 10 per line (6 lines, the last with 7).
    DIEEDG       2       1       2      57
        2543    2541    2539    2537    2535    2532    2530     899     898     897

## Open questions
* What the list values are (the corpus values look like node numbers) and what type 1's `extra`
  means. No type 1 set appears in the corpus.
