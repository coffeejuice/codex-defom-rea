# Phase 6 and 7 command keywords: spec index

Specs for the command keywords that phases 6 (object commands) and 7 (contact) of
[port-plan-def-pre-batch.md](../../port-plan-def-pre-batch.md) port. All are switch cases of
the command dispatcher `FUN_140378e00`; shared mechanics (dispatch, return codes, argument readers,
test layout) are in [_common.md](_common.md). Everything was recovered statically from
`DEF_PRE_64.exe` v11.0.2, then checked against the original: all 210 test cases plus 64 extra
cases and 10 baselines were run on the oracle and compared with the predictions (reports in
[oracle/](oracle/): ov-objpos, ov-ifd, ov-ctc, ov-misc). Spec text changed by that check is marked
"(corrected after oracle run)".

## Status per command

"Bit-exact" means the spec gives every step and the compiled floating-point order a port needs to
reproduce the original's output byte for byte.

| Command | Id | Spec | Tests | Status | Main open items |
|---|---:|---|---:|---|---|
| OBJPOS: grammar, mode table, move/rotate/mirror/reference/two-object, coupled objects, STROKE | 7 | [OBJPOS.md](OBJPOS.md), [OBJPOS_geo_kernels.md](OBJPOS_geo_kernels.md) | 44 | bit-exact (3D mirror boundary rebuild in OBJPOS_interference_kernels.md §1.7) | die-geometry sin/cos must be transcribed (statically linked AMD libm, not correctly rounded) |
| OBJPOS interference (modes 2, 12, 102, 112) | 7 | [OBJPOS_interference.md](OBJPOS_interference.md), [OBJPOS_interference_kernels.md](OBJPOS_interference_kernels.md) | 39 | specified; 3D kernels bit-exact, 2D kernels listing-checked | pcube cell test not transcribed line by line, 2D arc contact refinement, 2D CRT atan/atan2 |
| OBJPOS drop (3D modes 7, 8, 9) | 7 | [OBJPOS_drop.md](OBJPOS_drop.md), [OBJPOS_drop_solver.md](OBJPOS_drop_solver.md) | 15 | specified with FP order (explicit rigid-body settling, at most 195 iterations) | nearest edge/corner fall-back `FUN_1404d41f8`, ray–quad children, contact-engine state after a drop |
| CPYOBJ | 13 | [CPYOBJ.md](CPYOBJ.md) | 10 | specified (no FP) | generic field copier `FUN_140329ef0`, a few unnamed fields |
| SVSCAL | 12 | [SVSCAL.md](SVSCAL.md) | 6 | bit-exact | on-demand allocation `FUN_140329e60` |
| DOESCL | 14 | [DOESCL.md](DOESCL.md) | 22 | bit-exact | – |
| WRTOBJ | 9 | [WRTOBJ.md](WRTOBJ.md) | 6 | specified (writer by reference to the group 2 writer map) | default of `model+0x1744`, negative output number |
| GENAXS | 19 | [GENAXS.md](GENAXS.md) | 7 | bit-exact for 3D | 2D z values, WPAXIS constructor defaults |
| GENBCC | 295 | [GENBCC.md](GENBCC.md) | 8 | bit-exact | URZ accessor sync `FUN_1401d48d0` |
| GENSUB | 20 | [GENSUB.md](GENSUB.md) | 7 | bit-exact (single-precision parsing noted) | whether AX_BIL.KEY survives error paths |
| TCYCLE | 18 | [TCYCLE.md](TCYCLE.md) | 10 | bit-exact | argument meanings inferred; whether DTMAX survives KFWRIT |
| INICTC | 5 | [INICTC.md](INICTC.md) | 5 | bit-exact | – |
| GENCTC 2D | 6 | [GENCTC.md](GENCTC.md) §4 | 16 (shared) | bit-exact except PathScale atan2/sincos | message unit (stderr inferred) |
| GENCTC 3D | 6 | [GENCTC.md](GENCTC.md) §5, [GENCTC_3d_search.md](GENCTC_3d_search.md), [GENCTC_3d_decide.md](GENCTC_3d_decide.md) | 17 more | main path specified with FP order | quadrilateral projection library (`FUN_1404d203b` fall-backs), ray–quad, corner-walk topology routines, one acceptance case to confirm on the oracle |

Test scripts: `tests/commands/<COMMAND>/*.KEY` with a `cases.json` per folder (OBJPOS also has
`cases_ifd.json`). 210 command files in all.

## Follow-up specs

All follow-ups are written: `OBJPOS_geo_kernels.md`, `OBJPOS_interference_kernels.md`,
`OBJPOS_drop_solver.md`, `GENCTC_3d_search.md`, `GENCTC_3d_decide.md` (linked in the table).
[fp-audit.md](fp-audit.md) re-checked every multi-operand floating-point formula of the
first-pass specs against the listings: 7 grouping errors, all in Fortran routines, were corrected
in place (marked "corrected by FP audit"); the rest are marked `[listing-checked]`. Not audited:
GENCTC 3D (its two specs were written from the listings), the bilinear solve and the 2D arc
refinement.

## Findings that change the plan

* **GENAXS is not `findaxis.f` and GENBCC is not `genbcc2.f`**: both are C++ code with no Fortran
  calls. GENAXS only acts for layout code 7 and hard-codes objects 1, 2, 4–7 (a missing one crashes
  the original). GENBCC changes nothing on 3D objects.
* **GENCTC 3D is far larger than the plan's 45 new functions**: about 160 Fortran routines
  (about 230 KB) from the solver's contact module plus a C locator library, although for a plain
  INICTC/GENCTC only the MDSPS2_REC search path does any work. Most of its messages go
  to `NEWEQ.MSG`, which DEF_PRE deletes at exit; optional files `SRCNT2.DAT`, `SKIP_PNTR.DAT` and
  `DIEGEO.DAT` in the working directory change its behaviour.
* **Decompiled sums are misleading**: Ghidra prints 3-term Fortran sums reversed and without the
  parentheses; the compiled order is `(x + y) + z`. Port sums from the listing, not the decompile.
  The FP audit found and fixed 7 such errors in the first-pass specs.
* **Floating-point traps for the port**: the die-geometry rotation calls a statically linked sin/cos
  (AMD libm in the VS2008 CRT) that differs from the host CRT on about 1% of angles, so it must be
  transcribed; the 3D mirror runs in single precision; rotation uses two
  degree-to-radian conventions with different sin/cos implementations; GENSUB parses with `%f`
  into float; TCYCLE's formula order is fixed in the listing.
* **Reader status ignored**: WRTOBJ, TCYCLE, GENAXS and GENSUB never report a bad argument line,
  because the dispatcher tests a status variable those cases do not set.
* **Crashes the port must reproduce or guard against**: a missing coupled object in OBJPOS, a
  missing GENAXS object, the automatic interference search with no candidate, CPYOBJ mode 0 with
  source = destination, GENSUB's readers at end of file.
* **Order-dependent results**: the mesh boundary faces are sorted with the CRT `qsort` (not stable)
  and the interference bucket search depends on node order, so the port must copy both algorithms
  rather than use equivalent ones.
