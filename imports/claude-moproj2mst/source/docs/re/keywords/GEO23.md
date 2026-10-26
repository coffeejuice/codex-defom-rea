# GEO23 (id 283, group 2, case 0x8b)

A 2D→3D geometry conversion mode, stored in a sub-structure at `obj+0x2be8` (call it **G23**,
initialised with a back pointer by `FUN_1401890e0(obj+0x2be8, obj+0x3a98)` in `FUN_14015ef60`).
The mode is 0, 1 or 2. Mode 1 carries an integer and 3 doubles; mode 2 carries 2 doubles. Its
companions are CRDSYS, GEOSEC and MSHSEC, which are written by the same function. The names of
the modes (for example revolve or extrude) are not evidenced in DEF_PRE.

## KEY grammar (read)
    GEO23 <objNo:%i> <mode> [<k>]          line 0, tokenised (max 2 tokens, strtol base 10)
    mode 1:  <a:%Flf> <b:%Flf> <c:%Flf>     next line (FUN_140414c20)
    mode 2:  <a:%Flf> <b:%Flf>              next line (FUN_140414a40)
    mode 0 or any other value: no further line
Line 0 is split by `FUN_140413900(f,&cnt,2)` and the tokens are converted with `FUN_14047b6c0`
(decimal `strtol`). A missing token counts as 0, so an empty rest of line means mode 0.
`k` is used only in mode 1. The value lines use the `%Flf` helpers, which accept commas and allow
partial lines (missing values become 0). Same grammar in 2D and 3D.

## Model storage
G23 field offsets (relative to `obj`):

| Offset | Type | Set by |
|---|---|---|
| +0x2be8 | int mode | see below |
| +0x2bf0, +0x2bf8, +0x2c00 | double | mode 1 a, b, c |
| +0x2c08 | int | mode 1 `k` (only when line 0 has 2 tokens) |
| +0x2c10, +0x2c18 | double | mode 2 a, b |
| +0x2d08 (G23+0x120) | pointer | back pointer (owner/model), used by `FUN_140189150` |

* mode 0: `FUN_140189150(G23, 0)`, which sets `mode = 0` **only if** the back pointer is non-NULL.
  Otherwise mode is left unchanged.
* mode 1: `FUN_140189150(G23, 1)` (if the back pointer is set, temporarily sets mode =
  `FUN_1401ee4c0(ptr+0x750)` ∈ {1,2}). Then `k` → +0x2c08 if `cnt > 1`. Then the 3 doubles are read.
  `FUN_1401891c0(G23, &1)` sets mode = 1 (it accepts only 0..2). Then a, b, c → +0x2bf0/+0x2bf8/+0x2c00.
* mode 2: `FUN_140189150(G23, 1)`, then the 2 doubles are read, mode = 2, and a, b → +0x2c10/+0x2c18.
  +0x2c08 is **not** touched, even when a second token is present.
* any other mode value: nothing is stored, and nothing more is read.
Fields of the mode that was not selected keep their previous values.

## Read errors and validation
* tokenizer EOF → **2**.
* mode 1/2: value-line helper returns 1 (EOF, or blank line) → **2**. By then the mode may already
  have been changed by `FUN_140189150`, but the doubles are not stored.
* otherwise 0. No range checks. Return-code handling is as in `_common.md` §1.

## KEY writer
**Not written by `FUN_14029e250`.** The only writer is `FUN_1402a9c80(model, f, obj, objNo)`,
which is reachable only from the uncalled exporter `FUN_1401c6920` (dead code in DEF_PRE_64; see
CRDSYS.md). Its output, in order:
1. CRDSYS: `"%-6s %7i %7i\n"` with `obj+0x2c38`.
2. GEO23, depending on `obj+0x2be8`:
   * 0: `"%-6s %7i %7i\n"` name, objNo, 0
   * 1: `"%-6s %7i %7i %7i\n"` name, objNo, 1, `+0x2c08`; then `" %20.10E %20.10E %20.10E\n"`
     with +0x2bf0, +0x2bf8, +0x2c00
   * 2: `"%-6s %7i %7i %7i\n"` name, objNo, 2, `+0x2c08`; then `" %20.10E %20.10E\n"` with
     +0x2c10, +0x2c18
   * other: no GEO23 line
3. GEOSEC: `"%-6s %7i %7i %7i\n"` with +0x2c24, (+0x2c21 != 0)
4. MSHSEC: `"%-6s %7i %7i %7i\n"` with +0x2c28, (+0x2c20 != 0)
Always returns 0. `objNo` is the caller's argument; there is no `+0x3f24` renumbering here.
Round-trip gaps: in mode 2 the written `k` is ignored on read. Mode 0 reads back as 0 only when
the back pointer is set.

## Unit conversion
None.

## Helpers
* `FUN_140413900`, `FUN_140413a50`, `FUN_14047b6c0`: tokenizer and strtol.
* `FUN_140414c20`, `FUN_140414a40`: 3 and 2 double line readers.
* `FUN_140189150(G23, flag)`: reset mode (flag 0 → 0; flag 1 → `FUN_1401ee4c0` result 1/2), only
  if the back pointer at G23+0x120 is set.
* `FUN_1401891c0(G23, &m)`: mode = m if 0 ≤ m ≤ 2.
* `FUN_1401890e0(G23, p)`: store the back pointer (object init `FUN_14015ef60`).
* `FUN_1402a9c80`: writer (dead). `FUN_1401c6920`: uncalled exporter.

## Corpus check
No corpus file uses GEO23.

## Open questions
* The meaning of mode 1/2 and of a, b, c and k. Readers of these offsets are `FUN_1401467f0`,
  `FUN_140147f90`, `FUN_140153c00` and `FUN_140154270` (operand sweep `out/g2/J_imm.txt`).
  Decompiling them would name the fields. Not needed for a byte-exact port.
