# DSMAX_NSTEP

Stroke increment computed from a total stroke and a step count.

* registry id 49 (`0x31`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x1b (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf %i` (`FUN_140416aa0`) → d, n. **The helper status is not checked** (the code tests a variable that is still 0), so this keyword never reports a reading error. Then `obj = FUN_1401d6ea0(M, PDIE[0])` (object with that id in the object list `M+0x1380`); if `obj`, `n != 0` and `d != 0.0`: `M+0x930` (DSMAX) = `(|d| - |obj+0x22e0|) / n`. No DTMAX reset and no flag recomputation (unlike DSMAX).

## Model fields

Writes `M+0x930` only.

## Default

–

## KEY writer

Not written; the result appears as DSMAX.

## Notes

`obj+0x22e0` is the **Z component of the object's stroke vector** (`+0x22d0` X, `+0x22d8` Y): its accumulated displacement, keyword STROKE (group 2, `STROKE.md` on the group 2 branch). So DSMAX_NSTEP gives `(|total| - |stroke Z|) / n`, the remaining Z stroke split over n steps, for the object named by `PDIE[0]`, in 2D as well (where the Z stroke is whatever an earlier 3D read or move left, usually 0).

Every access to `+0x22e0` (displacement sweep): readers `FUN_140003f50` (`0x1400040d2`) and `FUN_1400179f0` (`0x140017c52`) (text menus, `" STROKE = "`), `FUN_1401e2180` (`0x1401e2671`, copies X/Y and, in 3D only, Z into the Fortran DB arrays at the indices in `0x1406e8530/34`), `FUN_1402b0350` (`0x1402b043e`, STROKE KEY writer), `FUN_14039aa90` (`0x14039b55a`, this keyword), `FUN_14039e670` (`0x14039e761`, passes X/Y/Z to `FUN_1403a83d0`); readers and writers `FUN_140167e90` (`0x140168ec2`/`0x140168f11`, mechanical-press consistency check) and `FUN_140378e00` (`0x14037bf5e..0x14037e07b`, three places that add an applied translation); writer `FUN_1402007d0` (`0x1402009b4`, movement type 6). The STROKE reader fills it through a pointer to `+0x22d0` and does not show up in the sweep.

## Helpers

`FUN_140416aa0`, `FUN_1401d6ea0`, `FUN_14017aaf0`, `FUN_140484558` (fabs). Shared mechanics: [_common.md](_common.md).

## Corpus

No corpus KEY file contains this keyword; grammar is from code only.
