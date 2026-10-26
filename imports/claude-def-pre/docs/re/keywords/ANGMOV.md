# ANGMOV (id 156, group 2, case 0x0c)

The angular movement control of the object: a method and a constant or table value.
* `obj+0x2788` holds the method. The text menu prints `" Enter Angular Movement Control
  Method: 0 - Torque; 1 - RPUT"`. 0 is torque; 1 is "RPUT", i.e. rotation per unit time
  (angular velocity).
* Each method has its own table object (`_common.md` §3; 0x78 bytes): torque at `obj+0x27c0`,
  angular velocity at `obj+0x2838`. The menu's constant prompts `"  Torque = %lf ? "` and
  `"  RPUT = %lf ? "` read the constants `+0x27c8` and `+0x2840`.
* The rotation axis is CNTRAX. In 3D a second axis uses ANGMO2/CNTRA2.

## KEY grammar (read)

    ANGMOV <objNo:%i> <mode:%i> <kind:%i> <v:%Flf>          (rest of the line, commas allowed)
    [kind 1 or 2: n = (int)v table points follow, free format:
        x1 y1
        ...
        xn yn ]

* Line 0 is read by `FUN_140415f00` (`"%i %i %Flf"`). Missing fields become 0.
* `mode` (`%i`, so hex and octal are accepted):
  * `0` → torque table `T = obj+0x27c0`;
  * `1` → angular velocity table `T = obj+0x2838`;
  * anything else: the parser **reads nothing more** and returns 0. Any table lines that
    follow would then be read as keywords (unknown keyword warning, line skipped). The writer
    never produces this.
* `kind` for mode 0/1. `T` is first reset by `FUN_14012fc70(T, 0)`: template 0 is the default,
  `ndim 0`, `ncomp 1`, constant 0.
  * `0`: constant. `T.ndim = 0`, `T+8 = v`. No table follows.
  * `1`: table over abscissa variable code **1**. `T.ndim = 1`, `T.var[0] = 1`,
    `T.cnt[0] = (int)v` (C truncation of the double).
  * `2`: table over abscissa variable code **10**. `T.ndim = 1`, `T.var[0] = 10`,
    `T.cnt[0] = (int)v`.
  * any other value: `T` stays the reset constant 0 and no table is read.
* The table body is then read by `FUN_14041a800(f, T, 0)` (`_common.md` §3 /
  group2-helpers.md):
  * a constant reads nothing;
  * `ndim 1`: `cnt` pairs `x v` with `fscanf("%Flf")`, free format (commas are **not**
    accepted here), then one `fgets` for the rest of the last data line;
  * `cnt <= 0` gives 0 points and no `fgets`.
* Dimension: the same in 2D and 3D.

## Model storage

| Field | Type | Set to |
|---|---|---|
| `obj+0x2788` | int | mode (only when mode is 0 or 1) |
| `T+0x00` | int | 0 (template id) |
| `T+0x04` | int | ndim: 0 or 1 |
| `T+0x08` | double | v (kind 0), else 0 |
| `T+0x10` | int | variable code 1 or 10 (kinds 1 and 2), else 0 |
| `T+0x1c` | int | point count `(int)v` (kinds 1 and 2) |
| `T+0x28` / `T+0x48` | double* | abscissae / values (reallocated by the body reader) |

* T = `+0x27c0` for mode 0 (constant at `+0x27c8`, ndim `+0x27c4`, var `+0x27d0`, count
  `+0x27dc`).
* T = `+0x2838` for mode 1 (constant at `+0x2840`, ndim `+0x283c`, var `+0x2848`, count
  `+0x2854`).
* The table of the other mode is left unchanged.

## Read errors and validation

* Object number EOF or objNo < 1 → 2.
* Line-0 helper EOF or blank line → **2**, with nothing changed.
* `obj+0x2788` is set before the body is read. An EOF in the body (`fscanf == -1`) → **2**,
  and the table is left partly read.
* Non-numeric table tokens are not detected (group2-helpers.md).
* No other validation: the reader accepts negative counts and unknown kinds.

## KEY writer

`FUN_1402b0610` (handler2), called from `FUN_1402a7c80` after FORCE. It is written for every
object type, including type 5.

* `obj+0x2788 == 0` (T = `+0x27c0`) or `== 1` (T = `+0x2838`). Any other mode → **nothing
  written**, returns 0.
* T.ndim == 0: `"%-6s %7i %7i %7i %20.10E\n"`, name, N, mode, `0`, `T+8`.
* T.ndim == 1:
  * `T.var[0] == 1` → kind 1; `== 10` → kind 2; any other code → **return 3**.
  * `"%-6s %7i %7i %7i %7i\n"`, name, N, mode, kind, `T.cnt[0]`. **The count is written as an
    int (`%7i`) where the reader expects `%Flf`**. `sscanf` reads `"      4"` as 4.0, so it
    still round-trips.
* T.ndim is neither 0 nor 1 → **return 3**.
* Then `FUN_1402f3130(f, T, 0)` (`_common.md` §3 table writer):
  * a constant writes nothing;
  * ndim 1 writes, per point, `" %20.10E"` x followed by `" %20.10E\n"` (ncomp 1);
  * it writes nothing at all if `cnt < 1`.
* A return of 3 makes `FUN_1402a7c80` stop. ANGMVY, CNTRAX, ANGMO2, CNTRA2, OBJUPD and OBJSTP
  of that object are then **not written**. `FUN_14029e250` and `FUN_1401c6d00` ignore the
  status of `FUN_1402a7c80`, so the KEY write itself does not fail.
* The KEY reader can only produce ndim 0 or 1 with var code 1 or 10, so the error paths are
  reachable only from GUI-built tables.
* Round-trip notes:
  * an unknown kind (≥ 3) is re-written as `mode 0 0.0E+000` (a constant 0);
  * a negative count is written in the header but with no rows, and reads back the same way.

## Unit conversion

None.

## Helpers

* `FUN_140415f00`: `"%i %i %Flf"` line reader.
* `FUN_14012fc70`: table reset (keeps `+0x54`, `+0x60`).
* `FUN_14041a800`: table body reader.
* `FUN_1402b0610`: writer.
* `FUN_1402f3130`, `FUN_1402f2e80`: table writer and value-line writer.
* `FUN_1402b0970`: handler3. It returns the address of the active table (`+0x27c0` for mode 0,
  `+0x2838` for mode 1), or 3 if the object is missing. It is used by the GUI table editor and
  is not on the KEY path.
* `FUN_1400179f0`: text menu.
* `FUN_140179840`: reads the active constant.

## Corpus check

All corpus lines parse. Most are `mode 1 kind 0` (907 2D + 416 3D lines). There are 12
`mode 0 kind 0` lines, e.g. `3D\EN\Rolling\3D_Ring_Rolling_Example_1.KEY`
(`ANGMOV 3 0 0 0.0000000E+000`). There are 2 table lines, in
`3D\EN\Special_Applications\3D_Spinning_Motion.KEY`:

    ANGMOV       2       1       1       2
        0.0000000000E+000    4.6472000000E+000
        1.0000000000E+000    5.0652000000E+000

This table line matches the writer format exactly.

## Open questions

* The meaning of abscissa variable codes 1 and 10. The same codes are used by MOVCTL tables
  (see MOVCTL.md); 1 is most likely time. The variable-code name table would settle it.
