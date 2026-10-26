# WINRSE (id 229, group 2, case 0x55)

Window region for an "RSE" condition (the name suggests a link to RSEDEF, written later in the
object block; not verified): shape only, no origin, functions or mode. Records are `LLWINDEF_DATA` entries of list
`obj+0x2fd0`, reset to BC kind 10. Record layout, list and shape helpers: see WINPRS.md.

## KEY grammar (read)

    WINRSE <objNo:%i> <win:%i> <k:%i>        FUN_140413ee0 ("%i %i")

3D model (`FUN_1400015f0`): `k` must be 1 (box):

    8 lines  <i:%i> <x:%Flf> <y:%Flf> <z:%Flf>     i = 1..8 (FUN_14041b7d0(f,w,1), type 1)

2D model: `k` = number of points, `k >= 1`:

    k lines  <i:%i> <x:%Flf> <y:%Flf>              i = 1..k (FUN_14041b6a0(f,w,k,1))

Dimension unset: treated like 3D for the body (`k` must be 1) but the 3D-only pre-check is
skipped.

## Model storage

`w = FUN_140269bc0(obj+0x2fd0, win)`, `FUN_14025d1a0(w, 10)` (kind 10). 3D/unset:
`+0x210 = 1`, 8 corners at `+0x180` (stride 3), then `FUN_140267760` (6 face centres at
`+0x218`, `+0x214 = 6`). 2D: `+0x178 = k`, points at `+0x180` (stride 2), `+0x210` stays 1.
All other fields keep their reset values (origin 0, `+0x368 = 0`, tables reset).

## Read errors and validation

Return **2**: header EOF/blank; 3D and `k != 1` (checked **before** the window is created);
unset dimension and `k != 1` (after the window is created and reset); 2D and `k < 1` (after
reset); body EOF/blank line or index mismatch (`FUN_14041b6a0`/`FUN_14041b7d0` → 1). The
"WindefShape invalid" path (return 3) is unreachable because `+0x210` is forced to 1.

## KEY writer

`FUN_14029e250` step 52 (not object types 5/7), every entry of `obj+0x2fd0`:

* 3D (`FUN_1400015e0` false and `FUN_1400015f0` true):
  `"%-6s %7i %7i %7i\n"` ("WINRSE", N, `w+0x170`, `w+0x210`), then `FUN_1402f4890(f, w, 0)`
  → 8 lines `" %7i %20.10E %20.10E %20.10E\n"`.
* 2D: `"%-6s %7i %7i %7i\n"` ("WINRSE", N, `w+0x170`, `w+0x178`), then `FUN_1402f4890(f, w, 0)`
  → per point `" %7i %20.10E %20.10E\n"` (indexed, matching the reader's index check).
* Dimension unset: nothing.

Round-trip breaks: a 2D placeholder window (gap below `win`, 0 points) is written as
`k = 0` and re-read fails (`k < 1` → 2); a 3D placeholder is written with type 1 and no corner
lines (next record misread).

## Unit conversion

None.

## Helpers

`FUN_140413ee0` (header), `FUN_14041b6a0` (2D, indexed), `FUN_14041b7d0` (3D box),
`FUN_140267760`; list/shape writers in WINPRS.md.

## Corpus check

No corpus file uses WINRSE.

## Open questions

* What the RSE window is used for (kind 10); check RSEDEF's spec / solver use.
