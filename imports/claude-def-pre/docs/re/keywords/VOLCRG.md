# VOLCRG (id 236, group 2, case 0x5c)

Per-object volume charge or source, judging by the induction and resistance-heating examples. It
has a class (0, 1, 2 encoded as the tens digit of the type) and a value that is either a constant
or a 1-D table against variable 1. The value is stored in the table object (`_common.md` §3) at
`obj+0x1fe0`, and the class at `obj+0x1fd8`. The registry flag is 7.

## KEY grammar (read)
    VOLCRG <objNo:%i> <type:%i> <v:%Flf>
    [type 1, 11, 21: P = (int)v points, then P records "x value" (free format)]
Line 0 is read by `FUN_1404154c0` (`"%i %Flf"`, commas → blanks). Let `t` = type:
* class: `t < 2` → 0 (this includes negative values); `2 <= t < 12` → 1; `12 <= t <= 21` → 2;
  `t > 21` → **return 3** (Data Error) before anything else is stored.
* `t ∈ {0, 10, 20}`: constant `v`.
* `t ∈ {1, 11, 21}`: table with P = (int)v points. The body is read by `FUN_14041a800(f, obj+0x1fe0, 0)`
  exactly as for CSFREQ (whitespace-separated `%Flf`, P × (x, ncomp values), then a final `fgets`).
* any other `t <= 21` (for example 2–9, 12–19, negative): only the class is stored, returns 0.

## Model storage
* `obj+0x1fd8` (int) = class 0/1/2. Default 0 (`FUN_140186200`).
* Table `T = obj+0x1fe0`:
  * constant: `T+4` (0x1fe4) = 0, `T+8` (0x1fe8) = v;
  * table: `T+4` = 1, `T+0x10` (0x1ff0) = 1 (var code), `T+0x1c` (0x1ffc) = (int)v.
* `ncomp` (`T+0x40`) and the template id are not touched. The reset uses `FUN_14012d2a0(T, 0)`:
  ndim 0, ncomp 1, constant 0.0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* `FUN_1404154c0` returns 1 → return 2 immediately, with nothing stored.
* `t > 21` → 3, with nothing stored.
* The table reader returns 1 (EOF in the body) → 2.

## KEY writer
Registry handler2 `FUN_1402afb10`, called right after CSFREQ (group2-writer-map.md step 47), for
objects of a type other than 5 or 7. It is always written. Let `base` = 0 / 10 / 20 for class
0 / 1 / 2. For any other class value `base` is an **uninitialised local**; the reader never stores
such a value.
* ndim 0: `fprintf("%-6s %7i %7i %20.10E\n", "VOLCRG", N, base, T+8)`.
* ndim 1 and var[0] == 1: `fprintf("%-6s %7i %7i %7i\n", "VOLCRG", N, base + 1, cnt[0])`.
* otherwise: **return 3**, which stops the object block.

Then `FUN_1402f3130(f, T, 0)` writes the table body as for CSFREQ (one `" %20.10E"` x and ncomp
values per point). It returns 0.

Round trip: types 0/1, 10/11 and 20/21 are exact. A type in 2–9 or 12–19 changes only the class,
so the type is written back as the class base (+1 if a table was already present). For example,
`VOLCRG n 5 3.0` after a reset is written as `VOLCRG n 10 0.0` (lossy).

## Unit conversion
None.

## Registry handler3
`FUN_1402afcd0` returns `*out = obj+0x1fe0` (3 if the object is missing). Like CSFREQ's, it is only
reachable through the uncalled accessor `FUN_1402f7a70`, so it **does not matter for KEY output**.

## Helpers
* `FUN_14038b8a0` case 0x5c: reader.
* `FUN_1404154c0`: line reader.
* `FUN_14041a800`: table reader.
* `FUN_1402afb10`: handler2 writer.
* `FUN_1402f3130` / `FUN_1402f2e80`: table writers.
* `FUN_1402afcd0`: handler3 (unused).
* `FUN_140186200`: reset.

## Corpus check
* `2D/EN/InductionHardening/2d_Induction_Hardening_Process_EN.KEY` line 11756 parses exactly:

      VOLCRG       2      11       5
          0.0000000000E+000    1.0000000000E+004
          3.5000000000E+001    1.0000000000E+004
          …

  (5 point lines follow).
* Also checked: `2D/EN/ResistanceHeating/Resistance_heat1_EN.KEY`: `VOLCRG       3      20    0.0000000000E+000`.
* The corpus has 1073 records of type 0, 1 of type 10, 4 of type 11 and 4 of type 20.

## Open questions
* What the class (tens digit) means, and what the value's units are.
