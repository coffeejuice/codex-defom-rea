# CSFREQ (id 235, group 2, case 0x5b)

Per-object frequency for induction heating. It is either a constant or a 1-D table against one
of three abscissa variables, and it is stored in a table object (`_common.md` §3, 0x78 bytes) at
`obj+0x1f60`. The registry flag is 7.

## KEY grammar (read)
    CSFREQ <objNo:%i> <type:%i> <v:%Flf>
    [type 1..3: v (truncated to int) = point count P, then P records "x f" (free format)]
Line 0 (the rest of the keyword line) is read by `FUN_1404154c0` (`"%i %Flf"`, commas → blanks).
* type 0: constant frequency `v`. Only this line is read.
* type 1, 2, 3: `P = (int)v`. Then the table body is read by `FUN_14041a800(f, obj+0x1f60, 0)`
  (`_common.md` §3, "Table reader"): with `ndim = 1` and `ncomp` taken as stored (1 after a reset),
  that is P × (`x`, then ncomp values), whitespace-separated with `fscanf("%Flf")` (commas **not**
  allowed), followed by one `fgets` for the rest of the last line when P × ncomp > 0.
* any other type (negative, ≥ 4): nothing is stored, returns 0. The body lines, if present, are
  then read as keywords.

## Model storage
Table `T = obj+0x1f60` (`+4` ndim, `+8` constant, `+0x10` var[0], `+0x1c` cnt[0]):

| type | `T+4` (0x1f64) | `T+8` (0x1f68) | `T+0x10` (0x1f70) | `T+0x1c` (0x1f7c) |
|---|---|---|---|---|
| 0 | 0 | v | — | — |
| 1 | 1 | — | 1 | (int)v |
| 2 | 1 | — | 0x1c | (int)v |
| 3 | 1 | — | 0x1d | (int)v |

Other fields (`ncomp` at `T+0x40`, template id) are not touched. The reset `FUN_140186200` calls
`FUN_14012d2a0(T, 0)`, which sets template 0 = default: ndim 0, ncomp 1, constant 0.0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* `FUN_1404154c0` returns 1 (EOF or blank rest of line) → **return 2 immediately**, with nothing stored.
* The table reader returns 1 (EOF inside the body) → 2. A non-numeric token is not detected.
* P ≤ 0: no body is read (the count stays 0 or negative, and the table reader clamps it, see `_common.md`).

## KEY writer
Registry handler2 `FUN_1402af830`, called from `FUN_14029e250` (group2-writer-map.md step 47) after
`RHTFLX` (3D) or `BCCRHT`, before `VOLCRG`, for objects of a type other than 5 or 7. It is always written.
* ndim 0: `fprintf("%-6s %7i %7i %20.10E\n", "CSFREQ", N, 0, T+8)`.
* ndim 1 and var[0] 1 / 0x1c / 0x1d: `fprintf("%-6s %7i %7i %7i\n", "CSFREQ", N, 1|2|3, cnt[0])`.
* anything else (for example ndim 1 with another var code): **return 3**, which stops the object block.

Then, for every case that did not return 3, it calls `FUN_1402f3130(f, T, 0)` (`_common.md` §3, "Table
writer"). For ndim 1 that is one line per point: `" %20.10E"` x, then `ncomp` × `" %20.10E"` (5 per
line), then `"\n"`. Nothing is written when cnt[0] < 1, or for ndim 0.

The handler returns 0 otherwise. The record round-trips exactly. One exception: a header with
P < 0 is written back with count 0, because the table re-allocation clamps negative counts to 0.

## Unit conversion
None.

## Registry handler3
Handler3 `FUN_1402afa70(model, ?, ref, double **out)` returns `*out = obj+0x1f60` (the table
address); it returns 3 when the object is missing. Its only caller is the registry accessor
`FUN_1402f7a70`, which has no caller (port-plan-def-pre-batch.md, "Still unresolved"). It is not
used by the KEY reader or writer, so it does **not** matter for KEY output.

## Helpers
* `FUN_14038b8a0` case 0x5b: reader.
* `FUN_1404154c0`: `%i %Flf` line reader.
* `FUN_14041a800`: table reader.
* `FUN_1402af830`: handler2 writer.
* `FUN_1402f3130`: table writer; `FUN_1402f2e80`: value-line writer.
* `FUN_1402afa70`: handler3 (table accessor, unused).
* `FUN_140186200` / `FUN_14012d2a0`: reset.

## Corpus check
* `2D/EN/InductionHeating/2D_DualFrequencyHeating-100+10000-HotSwitching_EN.KEY` line 4920 (type 2):

      CSFREQ       2       2       2
          1.0000000000E+002    5.0000000000E-001
          1.0000000000E+004    5.0000000000E-001

* The `…-Combined_EN.KEY` variant has a type 3 record with the same shape.
* `Object00007.KEY`: `CSFREQ       4       0    0.0000000000E+000`.

All parse exactly. The corpus has 1062 type 0, 10 type 2 and 10 type 3 records; no type 1.

## Open questions
* What abscissa variable codes 1, 0x1c and 0x1d mean (the variable-name table in the solver
  or GUI would settle it).
