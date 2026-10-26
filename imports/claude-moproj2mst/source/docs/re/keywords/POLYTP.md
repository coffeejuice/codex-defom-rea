# POLYTP (id 299, group 2, case 0x9b)

Per-element integer "polygon type" of the object's mesh (`mesh = *(obj+0x3c00)`, read by
`FUN_140198620(obj,&0)`). The values live in an int vector at `mesh+0x68`, one per element of the
mesh element array (`FUN_140003500(mesh)` = element count). Values in 1001..1999 mark elements that
belong to surface patch `value-1000`. The distinct patch ids are collected into a set at
`mesh+0x40`. The corpus has values like 1013, matching DIEPNT's patch id 13. **3D only**.

## KEY grammar (read)
    POLYTP <objNo:%i> <count:%i> <default:%i>
    then <count> lines:  <elem:%i> <value:%i>
Header and records each use `FUN_140413ee0` (one `fgets` per line, comma→space, `"%i %i"`). Each
record is exactly one line; blank lines count as records (they read as index 0, which is ignored).
**In 2D** (`!FUN_1400015f0(model)`) the case does nothing and returns 0, so nothing is consumed and
the KEY loop sees `count`, `default` and the records as unknown keywords (each line gives the
"WARNING: Keyword %s not found. Skip …" message and is skipped).

## Model storage
1. `arr = FUN_1402f9a50(mesh)`: if the element count n < 1, it returns NULL. Otherwise it resizes
   `mesh+0x68` to n ints filled with 0 (`FUN_140159b60`) and returns a pointer to element 0.
2. all n entries = `default`.
3. for each record: if 1 ≤ elem ≤ n, `arr[elem-1] = value`; other indices are dropped.
4. `FUN_1402fced0(mesh)`: `FUN_1402fce80` clears the set at `mesh+0x40` and sets
   `mesh+0x48 = 0`. Then for i = 1..n, `FUN_1402fd7f0(mesh,i)` is true when 1000 < arr[i-1] < 2000
   and (`mesh+0x48` < 1 or == arr[i-1]-1000). Because `+0x48` was just reset, that is always true
   for values in range. For each such element, `arr[i-1]-1000` is inserted into the set if absent.

## Read errors and validation
* header helper fails (EOF, or blank rest of line) → **2**.
* `mesh` NULL is not checked (`FUN_140003500` would dereference it).
* element count < 1 (no elements read yet) → **2**, and the records are **not** consumed.
  POLYTP must follow the mesh DIEGEO (see DIEGEO.md).
* record read failures do not stop the loop. The case returns the status of the **last** record
  read: EOF or blank line on the last record → **2**, earlier failures are masked. With
  count ≤ 0 → 0.
Return-code handling is as in `_common.md` §1.

## KEY writer
`FUN_1402ab210(model, f, obj, objNo)`, called unconditionally by `FUN_1402a27a0` after the mesh
DIEGEO lines. It runs inside the block guarded by `FUN_140203a20(obj)` (`obj+0x3990` flag, nodes > 0,
elements > 0) and `obj+0x3994 > 0`; see DIEGEO.md for the chain from `FUN_14029e250`. That block
can also run for a 2D object whose `obj+0x39c0 == 3` (via `FUN_1402a26c0`), in which case 2D
output contains a POLYTP that the 2D reader skips.
`FUN_1402ab210` calls `FUN_1402f5090(f, 299, objNo, arr = FUN_140199fa0(mesh), n, 0)` and returns
0, ignoring that function's result. `FUN_1402f5090` with id 299 does this:
* `n == 0` or `arr == NULL` → writes nothing.
* all `arr[i]` equal to `arr[0]` → `"%-6s %7i %7i %7i\n"` name, objNo, 0, arr[0]
  (so a uniform mesh, including all-0, gives `POLYTP obj 0 v`).
* else: `cnt` = number of entries != 0 (the default argument). Header
  `"%-6s %7i %7i %7i\n"` name, objNo, cnt, 0. Then for each i with arr[i] != 0:
  `" %7i"` i+1, then `FUN_1402f2fe0(f, 1, &arr[i], 10, 0)`, which prints `" %7i"` arr[i] and `"\n"`
  (see `_common.md` §3). Each record is therefore `" %7i %7i\n"`.
Round-trips exactly.

## Unit conversion
None.

## Helpers
* `FUN_140413ee0`: `"%i %i"` line reader.
* `FUN_140198620(obj,&k)`: `*(obj+0x3c00+k*8)` (mesh k).
* `FUN_140003500(mesh)`: element count (16-byte, 4-int element records).
* `FUN_1402f9a50(mesh)`: size the `mesh+0x68` vector to n and return its data, or NULL if n < 1.
* `FUN_140199fa0(mesh)`: data pointer of `mesh+0x68` (`FUN_140005100`, a copy-on-write detach).
* `FUN_1402fced0`, `FUN_1402fce80`, `FUN_1402fd7f0`, `FUN_1402fd8a0` (1000 < v < 2000),
  `FUN_1402fbb70` (`mesh+0x48`): rebuild the patch id set.
* `FUN_1402ab210`: writer wrapper. `FUN_1402f5090`: generic per-element int writer (only id 299
  produces output). `FUN_1402f2fe0`: int value-line writer.

## Corpus check
* `key\3D\EN\Cogging\Cogging_Multiple_Ops_Lab2\Objects\Object00004.KEY` (112 files use POLYTP):
  `POLYTP       4       0       0` (uniform form), parses.
* `key\3D\SI\Extrusion\Lagrangian\Extrude_T_T_LAG_Cold.KEY` line 236274: the non-uniform form
  parses exactly:
      POLYTP       2     128       0
          1016    1013

## Open questions
* None for the format. The use of the `mesh+0x40` set and `mesh+0x48` lies elsewhere (die surface
  patches).
