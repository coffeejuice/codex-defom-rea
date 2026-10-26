# ECCDEF (id 241, group 2, case 0x61)

Per boundary edge (2D) / boundary face (3D) integer code of the object's **deformation boundary
condition**, the "code" member of the DEF triple `ECCDEF` (code, int) / `ECDEFN` (function number,
int) / `ECPRES` (pressure, double). It is one of 12 arrays in the object's "edge/face
contact-condition" block `obj+0xae8..0xb40`, allocated per boundary edge/face by `FUN_1401858d0`
(the int reader's error text calls the slot index `iECCArrayAdr`).

This file is the **detailed reference for all 12 EC keywords** (ECCDEF ECDEFN ECPRES ECCTMP ECTMFN
ECHFLX ECCATM ECATFN ECAFLX ECCRHT ECRHFN ECRFLX); the other files only list their differences.

## KEY grammar (read)

    ECCDEF <objNo:%i> <count:%i> [<default:%i>]          (rest of keyword line, sscanf "%i %i")
    then <count> records, free format (fscanf, whitespace incl. newlines separates):
      2D:  <idx:%i> <n1:%i> <n2:%i>                 <value:%i> x m
      3D:  <idx:%i> <n1:%i> <n2:%i> <n3:%i> <n4:%i> <value:%i> x m
    then the rest of the last record's line is consumed (fgets)

* `m` = number of components = `model+0x16fc` (state-variable width table `W[0x41]`, see
  `claude-deform-rea-dbmap/docs/re/dbmap/objarrays.md`: every entry defaults to 1 and 0x41..0x4c
  are not overridden). Corpus always has m = 1.
* Header: the whole rest of the keyword line, `sscanf(line, "%i %i")`, **no comma replacement**.
  * `sscanf == -1` (blank line / EOF) → status 1.
  * 2 conversions → every slot `0..NS-1` and every component gets `default`
    (`NS = FUN_140165170(obj)`, below). 1 conversion → no fill, existing values kept.
  * `count < 1` → status 0, nothing more is read (the header line is already consumed).
* Records: `idx` is read and **ignored** (only used as a placeholder); node numbers locate the
  edge/face. Commas are not accepted inside records (fscanf). Only `fscanf == -1` (EOF) is detected
  → status 1; a non-numeric token makes fscanf return 0 silently, the token stays in the stream
  and the record's remaining variables keep the previous record's values (first record:
  uninitialised stack).
* After the `count` records one `fgets` reads the rest of the line (EOF → status 1).
* 2D vs 3D is decided per record by `FUN_1400015e0(FUN_140158f50(obj))`, i.e. the model
  dimension of the object's model pointer `obj+0x3a98` (`model+0x1c == 2`); anything not 2D uses
  the 3D record layout.

### How an edge/face is located (slot lookup)

Boundary data (built by ELMCON, see "Model storage"): `obj+0xbd8` = number of boundary
loops/surfaces `L` (≤ 100); loop `k` record at `obj+0xbe0 + 0x30·k`: `+0` int node count `n_k`,
`+8` int* node list, `+0x10` int face count, `+0x18` int* face nodes (4 per face), `+0x20` int*
face elements, `+0x28` model pointer. `obj+0x87c` = "mesh present" (ELMCON sets 1),
`obj+0x880` = node count, `obj+0x884` = element count.

* **Slot count** `NS = FUN_140165170(obj)`: 0 if `obj+0x87c`, `obj+0x884` or `obj+0xbd8` is 0.
  Else `FUN_140165d30` recomputes `obj+0x88c = Σ_k FUN_14017ebc0(loop k)` (2D: `n_k − 1`;
  3D: face count `+0x10`; other dimension: −1), then 2D: `NS = obj+0x88c − 2 + 2·L`; 3D:
  `NS = obj+0x88c` (with one surface, = its face count); negative → 0.
* **2D** `FUN_140200c90(obj, n1, n2)`: −1 if `obj+0x87c == 0`. Otherwise loops `k = 0..L−1` with
  base `off_k = Σ_{i<k} (n_i + 1)`; for `j = 0..n_k−1` tests the pair `(nodes[j], nodes[j+1])`
  against `(n1, n2)` **in either order**; first match returns `off_k + j`. No match → −1.
  So each loop reserves `n_k + 1` slots, slot `off_k + j` is the edge from node list entry `j`
  to `j+1`; with a closed node list (last = first, which `n_k − 1` edges per loop implies) the
  real edges are `j = 0..n_k−2`, and `NS` stops two slots short of `Σ(n_k+1)` (the last loop's two
  spare slots). Note `j = n_k−1` reads `nodes[n_k]`, one past the `n_k`-int allocation
  (`FUN_14017e8a0`); a coincidental match there gives slot `off_k + n_k − 1` (= `NS` for the last
  loop).
* **3D** `FUN_140200ff0(obj, n1, n2, n3, n4)`: −1 if `obj+0x87c == 0`. Face nodes at
  `F = *(int**)(obj+0xbf8)` (surface 0 only), face count `obj+0xbf0`. Mode is chosen **once from
  face 0**: `F[0] != F[3]` → quad mode, else triangle mode (triangles are stored `a b c a`).
  Quad mode: face `i` matches when each of its 4 nodes is one of `{n1,n2,n3,n4}`; triangle mode:
  each of its first 3 nodes is one of `{n1,n2,n3}` (`n4` ignored). Unordered set-membership test
  (duplicates allowed); returns the first matching face index `i`, else −1.

Storage per record: slot `s`; int version (`FUN_14041a440`, ECCDEF): `s < 0 || s ≥ NS` →
`printf("ERROR in readEdgeFaceVarInt, iECCArrayAdr\n")` (stdout), record dropped, reading
continues; else `dst[s·m + c] = value[c]`. Double version (`FUN_14041a090`): stored when `s ≥ 0`
(no upper check), silently dropped otherwise.

## Model storage

* `obj+0xae8` → `int*` array of `NS` ints (ECCDEF), element `[slot·m + c]`.
* The 12-pointer block `obj+0xae8` is (int, int, double) × 4: `0xae8` ECCDEF, `0xaf0` ECDEFN,
  `0xaf8` ECPRES, `0xb00` ECCTMP, `0xb08` ECTMFN, `0xb10` ECHFLX, `0xb18` ECCATM, `0xb20` ECATFN,
  `0xb28` ECAFLX, `0xb30` ECCRHT, `0xb38` ECRHFN, `0xb40` ECRFLX.
* Allocation: ELMCON (case 0x1d) sets `obj+0x87c = 1` and, unless `FUN_14039aa20(model)`
  (`model+0x1b5c` byte) is set, calls `FUN_1401fc290(obj)`: rebuilds the boundary (2D: loops via
  `FUN_14032d65e`; 3D: one surface via `FUN_140354ed0`), `FUN_140165d30`, then `FUN_1401631e0`:
  `FUN_1401856e0` frees all 12 arrays, `FUN_1401858d0(obj+0xae8, NS, -1)` callocs each with
  **NS entries** (int: `NS·4` bytes, double: `NS·8` bytes; zero filled; nothing if `NS ≤ 0`).
  The allocation ignores `m`: with `m > 1` the readers would write past the end.
* Consequences: EC keywords must come after the object's ELMCON; a later ELMCON wipes them.
  Without a mesh `NS = 0` and every lookup returns −1 (int version prints the error per record).
* No flags or other fields are touched.

## Read errors and validation

Parser case: helper status nonzero → parser returns **2** ("Reading Error", printed and the KEY
loop continues, `_common.md` §1). Status 1 paths: EOF on header `fgets`, blank header
(`sscanf == -1`), EOF in any record fscanf, EOF on the final `fgets`. Everything else returns 0:
unmatched edges/faces, non-numeric tokens, `count ≤ 0`. Object number < 1 → 2 (group2-helpers.md).
Defaults: arrays are zero after allocation; no implicit default besides the header's.

## KEY writer

Registry handler2 `FUN_1402b6840` → `FUN_1402a4a80(model, f, obj, 0x41, N, 0)`; case 0x41 →
`FUN_1402f6600(f, "ECCDEF", N, obj, obj+0xae8 array, m = model+0x16fc, default = 0)`.
Called from `FUN_1402abf20` (writer-map step 54) only when `obj+0x87c != 0 && obj+0xbd8 > 0`, in the
order ECCDEF, ECDEFN, ECPRES, ECCTMP, ECTMFN, ECHFLX, ECCATM, ECATFN, ECAFLX, ECCRHT, ECRHFN,
ECRFLX. All statuses are 0 (the line writer's status is discarded by `FUN_1402a4a80`).
`N` = `obj+0x3f24` if > 0 else `obj+8`.

`FUN_1402f6600(f, name, N, obj, int *a, m, def)` (int) / `FUN_1402f6a10(…, double *a, m, double
def)` (double):

1. `f == NULL` → 2; `obj == NULL` or `a == NULL` → 0, nothing written; `m ≥ 2` → 1, nothing written.
2. Scan `i = 0..NS−1`; in 2D only slots whose `FUN_140200e20(obj, i)` nodes are both in
   `1..obj+0x880` are considered (gap slots give 0,0); in 3D all slots. `uniform` = every
   considered `a[i] == a[0]`; `cnt` = number of considered `a[i] != def` (exact `!=`, also for
   doubles).
3. Uniform: one line, count 0 and value `a[0]` (written even when it is 0):
   * int: `"%-6s %7i %7i %7i\n"` (name, N, 0, a[0])
   * double: `"%-6s %7i %7i %20.10E\n"` (name, N, 0, a[0])
4. Else header with the default (always 0) then records:
   * int: `"%-6s %7i %7i %7i\n"` (name, N, cnt, 0); double: `"%-6s %7i %7i %20.10E\n"` (…, 0.0)
   * for each slot `i` (all slots, no 2D filter) with `a[i] != def`: `k++` (1-based record number,
     incremented **before** the validity test), then
     * 2D: nodes `(p, q)` from `FUN_140200e20` → `FUN_140200ec0` (slot in loop `k` range
       `[off_k, off_k+n_k)` → `nodes[s−off_k]`, `nodes[s−off_k+1]`, else 0,0); written only if both
       in `1..obj+0x880`: int `" %7i %7i %7i %7i\n"` (k, p, q, a[i]); double
       `" %7i %7i %7i %20.10E\n"`.
     * 3D: `FUN_140201210` → the 4 stored face nodes `F[4i..4i+3]` (triangle: `a b c a`); written
       only if all 4 in `1..obj+0x880`: int `" %7i %7i %7i %7i %7i %7i\n"` (k, n1..n4, a[i]);
       double `" %7i %7i %7i %7i %7i %20.10E\n"`.
   * Only `a[i]` (component 0) is written.

Round trip: the reader re-locates every written edge/face (nodes come from the same tables).
Non-uniform output always has default 0, so 2D spare slots and unwritten slots read back 0.
In 2D a skipped invalid slot with a nonzero value is not counted in `cnt` but still advances `k`
(gaps in record numbers; harmless, idx is ignored). In 3D `cnt` counts slots whose record could be
skipped (a face with a node outside `1..obj+0x880`), which would make the reader consume the next
keyword line as a record; not expected with a valid mesh.

## Unit conversion

None (integer code).

## Helpers

| Address | Role |
|---|---|
| `FUN_14038b8a0` case 0x61 | parser dispatch → `FUN_14041a440(f, obj+0xae8, obj, model+0x16fc)` |
| `FUN_14041a440` | int edge/face array reader (`readEdgeFaceVarInt`) |
| `FUN_14041a090` | double edge/face array reader |
| `FUN_140165170` | slot count NS of the object |
| `FUN_140165d30` / `FUN_14017ebc0` | recompute `obj+0x88c` (Σ edges per loop / faces per surface) |
| `FUN_140200c90` | 2D edge (n1,n2) → slot |
| `FUN_140200ff0` | 3D face (n1..n4) → face index |
| `FUN_140200e20` / `FUN_140200ec0` | 2D slot → edge nodes (writer) |
| `FUN_140201210` | 3D face index → 4 face nodes (writer) |
| `FUN_1401fc290` | ELMCON: rebuild boundary, then `FUN_1401631e0` |
| `FUN_1401631e0` / `FUN_1401856e0` / `FUN_1401858d0` | free and (re)allocate the 12 EC arrays (NS entries) |
| `FUN_140256dc0` / `FUN_140257150` | calloc n ints / n doubles |
| `FUN_1402abf20` | writer step 54, calls the 12 handlers |
| `FUN_1402b6840` | ECCDEF write handler (code 0x41) |
| `FUN_1402a4a80` | state-variable writer, cases 0x41..0x4c |
| `FUN_1402f6600` / `FUN_1402f6a10` | int / double edge/face line writers |

## Corpus check

All `.KEY` files under `DEFORM_corpus\key` scanned with this grammar (header count, then `count`
records of 4 tokens in 2D / 6 in 3D, next line a keyword or `*`): every EC record block of all 12
keywords parses exactly (≈1.24 M records, 0 mismatches). Smallest file
`key\2D\EN\plane stress\Ela_PlaneStress_EN.KEY` line 212: `ECCDEF       1       0       0`
(uniform 0). Non-uniform 3D: `key\3D\EN\Resistance_Heating\3D_Brick_Resistance_heating_EN_Ex2.KEY`
25043 (`ECCRHT       1     380       0` / `       1       1       2     653     652       3`).

## Open questions

* Whether the 2D boundary node list is closed (last = first); inferred from `FUN_14017ebc0`
  returning `n_k − 1` edges. Settle by decompiling `FUN_14032d65e` or dumping a mesh.
* Meaning of `model+0x1b5c` (when set, ELMCON does not allocate the EC arrays) and the value of
  `model+0x16fc` in the KEY path (expected 1; settle with `FUN_1401d0fc0`).
* Physical meaning of the code values (corpus uses 0, 2, 3).
