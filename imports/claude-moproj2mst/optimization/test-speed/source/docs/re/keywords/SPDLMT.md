# SPDLMT (id 155, group 2, case 0x0b)

The speed-limit table of a hydraulic press movement. Its table object is at `obj+0x2a20`
(template 10: `ndim 1`, abscissa variable code 5, `ncomp 1`). Alongside it:
* a "defined" flag at byte `obj+0x2a18`;
* two doubles at `obj+0x2a98` and `obj+0x2aa0`.

Evidence for the hydraulic link:
* The data-check routine `FUN_140167b50` clears the flag unless the movement type `obj+0x21c8`
  (see MOVCTL.md) is 7, 8, 9 or 10. The hydraulic types include 10, which the check reports as
  `"Obsolete hydraulic movement type"`.
* The 2D object summary prints the table under the name `"SPDLMT"`.

## KEY grammar (read)

    SPDLMT <objNo:%i> <n:%i> <a:%Flf> <b:%Flf>               (rest of the line, commas allowed)
    [n >= 1: n pairs "x v", free format (%Flf, no commas)]

* Line 0 is read by `FUN_140415680` (`"%i %Flf %Flf"`). Missing values become 0.
* The table is always reset with `FUN_14012fc70(obj+0x2a20, 10)`. The template stays 10
  (ndim 1, var code 5) whatever `n` is. Then `cnt[0] = n`.
* `FUN_14041a800` reads the body: `n` pairs (abscissa, value), then one `fgets` for the rest of
  the last data line. With `n <= 0` nothing is read (`cnt` is clamped to 0 by the reallocation,
  `_common.md` §3).
* Same in 2D and 3D.

## Model storage

| Field | Type | Value |
|---|---|---|
| byte `obj+0x2a18` | bool | 1 if `n >= 1`, else 0 |
| `obj+0x2a20` | table object | template 10 (`+0x2a24` ndim = 1, `+0x2a30` var = 5) |
| `obj+0x2a3c` | int | `cnt[0] = n` |
| `obj+0x2a98` | double | a |
| `obj+0x2aa0` | double | b |

**Side effect:** after the body has been read, whatever its status, if the flag is set and the
movement type `obj+0x21c8 == 1`, the type is changed to **8**. So SPDLMT must come after MOVCTL
to have this effect; the writer emits MOVCTL first.

## Read errors and validation

* Object number EOF or objNo < 1 → 2.
* Line-0 helper EOF or blank line → **2** directly. Nothing is changed except `a`, `b` and `n`,
  which the helper has zeroed or partly filled at `+0x2a98`/`+0x2aa0`.
* Body EOF → **2**. The flag, the reset table and the movement-type change are already applied.
* No range checks.

## KEY writer

`FUN_1402b11e0` (handler2). It is called from `FUN_1402a7c80` right after MOVCTL and before
PRSTRE. It is written for all object types and always returns 0.

* `obj+0x2a18 == 1`:
  * `"%-6s %7i %7i %20.10E %20.10E\n"`, name, N, `+0x2a3c` (n), `+0x2a98`, `+0x2aa0`;
  * then `FUN_1402f3130(f, obj+0x2a20, 0)`, one line `" %20.10E %20.10E\n"` per point;
  * nothing is written if `cnt < 1`.
* Else if `obj+0x2aa0 > 0.0` (`DAT_14051bed8 = 0.0`):
  * `"%-6s %7i %7i %20.10E %20.10E\n"`, name, N, `0`, `0.0`, `+0x2aa0`;
  * the 3rd field is a literal 0.0 (disassembly: `XORPD`), **not** `+0x2a98`.
* Else nothing.
* Round trip:
  * with no table, `a` is lost (it reads back as 0.0);
  * a flag of 1 with `cnt < 1` (only possible from the GUI) writes a header with no rows.

N follows the `obj+0x3f24` renumbering rule. Three-digit exponents.

## Unit conversion

None.

## Helpers

* `FUN_140415680`: `"%i %Flf %Flf"` reader.
* `FUN_14012fc70`, `FUN_14012d2a0`: table reset (template 10 → ndim 1, var code 5).
* `FUN_14041a800`: body reader.
* `FUN_1402b11e0`: writer.
* `FUN_1402f3130`: table writer.
* `FUN_1402b1360`: handler3. It returns `obj+0x2a20` for the GUI table editor, or 3 if the
  object is missing. It is not on the KEY path.
* `FUN_140167b50`: clears the flag for non-hydraulic movement types (not on the read path).
* `FUN_14039e670`: object copy (`FUN_1403a89a0(…, a, b)`).

## Corpus check

Only `2D\EN\Press_Modelling\Hydraulicpress_Controls_Example1_EN.KEY` uses SPDLMT (2 lines). It
parses: `MOVCTL 2 8 …` comes first, then

    SPDLMT       2       9  0.0000000E+000  0.0000000E+000
      0.0000000E+000  5.0000000E-001
      9.6070000E+001  4.7500000E-001

followed by 7 more pairs, then `STROKE`. That is an older `%15.7E` writer, and the reader is
free-format, so it parses the same.

## Open questions

* What `a` (`+0x2a98`) and `b` (`+0x2aa0`) are. They are probably press parameters, and the
  b-only line suggests b is a speed limit given without a table. The DB setter `FUN_1403a89a0`
  or the hydraulic movement code would settle it.
* The meaning of abscissa variable code 5 (probably stroke). Also check whether MOVCTL.md
  names it.
