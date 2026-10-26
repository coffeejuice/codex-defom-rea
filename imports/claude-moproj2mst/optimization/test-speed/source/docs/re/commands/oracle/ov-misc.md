# Oracle check: CPYOBJ, SVSCAL, DOESCL, WRTOBJ, GENAXS, GENBCC, GENSUB, TCYCLE

Worker ov-misc. Cases `tests/commands/<CMD>/cases.json` (76), plus 23 extra cases in
`cases_v_ov-misc.json` of CPYOBJ, SVSCAL, GENAXS, GENBCC, TCYCLE (labels `<CMD>/v_*`, files
`v_*.KEY`). Results: `out/p67/oracle/res/<label>/`, compared with `res/_baseline/<seed>`; the
TCYCLE / COG cases (no baseline) with `TCYCLE/kzero` (DBREAD 24, no change) and
`TCYCLE/v_base25` (DBREAD 25). Scripts in `out/p67/ov-misc/`: `summ.py` (messages, files, diff),
`objcmp.py` (object section compare with renumbering), `genaxs.py` (bounding boxes, flags, `u`,
point shifts), `q2d.py` (GENSUB 2D scan simulation with float rounding). Step times of
`COG_P1_B1.DB` from `dbinfo --step N --header` (deformdb build of the main worktree). All runs
exit 0; stderr is empty in every case; no NEWEQ.MSG anywhere.

## Cases

| Label | Verdict | Observed / what differed | Fix |
|---|---|---|---|
| CPYOBJ/badobj_3d | OK | `ERROR: Keyword CPYOBJ Reading Error.`, no change | – |
| CPYOBJ/mode0_2d | OK | object 3 section = object 2 section with number 3; others unchanged | – |
| CPYOBJ/mode0_3d | OK | object 3 section = object 1 section with number 3 (every line) | – |
| CPYOBJ/mode0_wp_3d | OK | object 1 section = object 2 section with number 1 | – |
| CPYOBJ/mode1_2d | OK | MGNELM 1 and 2 = 400 (2D int) | – |
| CPYOBJ/mode1_3d | OK | RMDPTH -0.5, MGNELM 3000 1500 copied to object 1 | – |
| CPYOBJ/mode2_3d | OK | MOVCTL 2 = type 1, dir (0,0,-1), speed 1 | – |
| CPYOBJ/mode3_2d | inconclusive | objects 1, 2 of SPIKE_SI have identical process data: no diff | test line fixed; `v_mode3_2d` added |
| CPYOBJ/mode3_3d | OK | AVGSTR 0.3, LMTSTR 3E-3, PENVOL 1E4 into object 1 | – |
| CPYOBJ/mode4_noop_3d | OK | no change | – |
| CPYOBJ/v_mode3_2d | OK | AVGSTR 0.25, ZSTR 1 5.0, PENVOL 2E5 2, REFTMP 300 copied to object 2 | new |
| SVSCAL/badobj | OK | Reading Error + `WARNING: Keyword 0.0 not found. Skip  0.5` / `1.0 … 1.5` | – |
| SVSCAL/chain_zero_3d | OK | OUT1 STRAIN 2 = 0.25, OUT 0.0 | – |
| SVSCAL/extrap_3d | OK | 0.5 | – |
| SVSCAL/kind0_3d | OK | 0.5 | – |
| SVSCAL/kind1_2d | OK | 0.5 | – |
| SVSCAL/kind1_3d | OK | 0.5, nothing else changed | – |
| SVSCAL/v_cog_interp | OK | 1867 non-uniform strains: F within print rounding (rel 5e-11), extrapolation above 0.2 | new |
| DOESCL/badmat_3d | OK | both lines exact | – |
| DOESCL/badobj_3d | OK | both lines exact | – |
| DOESCL/blankfactor_3d | OK | DOESCL Reading Error, then NDTMP Reading Error | – |
| DOESCL/fperv_3d | OK | `Scale 10.000000 FPERV  : Mat# 1 [1 0]`, VarFieldWidth line, FPERV 20 / 3 | – |
| DOESCL/fperv_both_3d | OK | `[0 1]`, FPERV 2 / 30 | – |
| DOESCL/fstres_nontab_3d | OK | message + VarFieldWidth, no change | – |
| DOESCL/fstres_tab_2d | OK | 18 flow-stress values × 1.1 exact to the print; abscissae untouched | – |
| DOESCL/loctmp_2d | OK | window 2 env 25→50, coef 0.3→0.6 | – |
| DOESCL/loctmp_win0_2d | OK | window 1: 25→50, 0.021→0.042, emissivity/flux 0 stay 0 | – |
| DOESCL/movctl_ham_t0_2d | OK | energy 2.712E7 → 1.356E7 | – |
| DOESCL/movctl_ham_t1_2d | OK | `Assign energy 10000000.000000 …`, energy 1E7 | – |
| DOESCL/movctl_ham_t2_2d | OK | efficiency 0.6 → 0.8 | – |
| DOESCL/movctl_ham_t3_2d | OK | no message, no change | – |
| DOESCL/movctl_screw_t0_2d | OK | 4.52E7 → 9.04E7 | – |
| DOESCL/movctl_screw_t2_2d | OK | efficiency 0.9 → 0.75 | – |
| DOESCL/movctl_speed_3d | OK | `Scale energy 3.000000 MOVCTL : Obj# 1 type 0`, speed 1 → 3 | – |
| DOESCL/movctl_speed_t2_3d | OK | `Assign efficiency …`, speed still scaled to 3 | – |
| DOESCL/ndtmp_2d | OK | uniform NDTMP 982.222 → 1964.444 | – |
| DOESCL/ndtmp_3d | OK | 1200 → 600 on object 2 only | – |
| DOESCL/unknown_kw_3d | OK | `Error on reading DOESCL : undefined keyword definition` | – |
| DOESCL/unsupported_kw_3d | OK | same message for STRAIN | – |
| DOESCL/young_2d | OK | 2.06843E5 → 1.034215E5, VarFieldWidth line | – |
| WRTOBJ/badobj | OK | no NONE.KEY, no message, OUT.KEY written | – |
| WRTOBJ/badpath | OK | no file at all (KEY loop stopped before KFWRIT), no message | – |
| WRTOBJ/flag0_3d | OK | OBJ2.KEY byte-identical to object 2 block of OUT.KEY | – |
| WRTOBJ/flag3_2d | OK | object 1 block renumbered 3 on every line incl. comment | – |
| WRTOBJ/flag5_3d | OK | renumbered 5; OUT2.KEY = OUT.KEY | – |
| WRTOBJ/noflag_2d | OK | name `OBJ1.KEY   ` trimmed; byte-identical block | – |
| GENAXS/corpus10 | OK | pair 4 blocked (overlap test), opened: G4 by u, G5 by -u (348 coordinates exact); record (6 7 -90 -80), 0.5h 9.9571056375 | – |
| GENAXS/corpus15 | OK | same | – |
| GENAXS/empty | OK | no change | – |
| GENAXS/layout6 | OK | no change | – |
| GENAXS/objpos14 | prediction wrong, spec right | OBJPOS 14 leaves +0x2588 = 0, so no spring reset; OBJPOS closed pair 4/5 by -u, GENAXS opened by u (exact vs `v_objpos14_ctl` within its print rounding) | test line fixed; spec §8 text fixed; `v_spring` added |
| GENAXS/tol0 | OK (weak) | same result as corpus15; tolerance not exercised | test line clarified; `v_tolB85`, `v_clearA_*` added |
| GENAXS/tolbig | OK | both pairs opened, type-7 record deleted | – |
| GENAXS/v_clearA_tol10 | OK | both clear: no moves, records (4 5 10 20), (6 7 -90 -80) | new |
| GENAXS/v_clearA_tol15 | OK | 15 >= 15 blocks pair 4 | new |
| GENAXS/v_clearA_Bblk | OK | G6 by u, G7 by -u, record (4 5 10 20) | new |
| GENAXS/v_tolB85 / v_tolB8499 | OK | 85 >= 85 blocks, 84.99 does not | new |
| GENAXS/v_spring | OK | spring reset: G4, G5 (-2.5,0,0); G6, G7, W (+1.5,0,0); springs, FORCE 4 zeroed; record (6 7 -88.5 -78.5) | new |
| GENAXS/v_objpos14_ctl | – | control state | new |
| GENBCC/axisym | OK | OUT.KEY = seed: SPIKE_SI already holds exactly the generated BCs (31 axis nodes, recomputed tol 0.0579) | test line notes it |
| GENBCC/axisym_obj2 | OK | object 2 (min r 7.62): no axis node, all edges 2 → `ECCTMP 2 0 2` | – |
| GENBCC/brick3d | OK | no change | – |
| GENBCC/empty | OK | Reading Error | – |
| GENBCC/gendb2d | OK | DB generated, OUT.KEY = seed | – |
| GENBCC/missing | OK | Reading Error | – |
| GENBCC/plane | SPEC WRONG (stderr / slot -1) | x = 1 codes cleared, all ECCTMP slots 2 (`ECCTMP 1 0 2`) as predicted, but **no stderr** `ECC array index(-1)`: NBE = sum(cnt − 1), no edge maps to slot -1 | spec §3, §4, §5, §7 corrected; test line fixed |
| GENBCC/torsion | inconclusive | axis path confirmed (same 31 nodes x = 1), but GEOTYP 3 after KFREAD changes the BCCDEF width 2→3 without re-layout: re-interpreted rows and heap garbage in object 2 (`223 1426128980 201850783 0`) not present in the control | test line fixed; spec §7 note; needs a GEOTYP 3 seed |
| GENBCC/v_plane_ctl, v_torsion_ctl | – | controls (GEOTYP line alone) | new |
| GENSUB/brick1 | OK | MESH.DAT (BCCTMP dropped, `BCCDEF 1 1` / `1 0 0 0`), INI `1 64 27` / `1 4 -1.0E+000`, AX_BIL deleted | – |
| GENSUB/brick2 | prediction wrong, spec right | object 2 block has `ECCTMP 2 36 0` > 27 elements → `\tERROR : ECCTMP could not be read` in DEF_CTRL.OUT, no MESH.DAT, AX_BIL.KEY left; INI `2 64 27` / `1 4 2.5` | test line fixed; spec §8 + §5 notes |
| GENSUB/empty | OK | no files | – |
| GENSUB/missing | OK | Reading Error, empty DEF_SUB.INI | – |
| GENSUB/obj0 | OK | MESH.DAT of object 3 stays, DEF_SUB.INI deleted | – |
| GENSUB/quad2d | OK | misaligned RZ: all 999 values = simulated scan with float rounding; `ELMCON 1 0 0`; INI `1 333 289` / `1 45 1.0` | – |
| GENSUB/tet3d | OK | no MESH.DAT, AX_BIL.KEY left (inference confirmed), INI off 5 − 3 = 2 | test line made exact |
| TCYCLE/corpus | inconclusive (not visible) | step 24 is DSMAX mode: no DTMAX line in OUT.KEY | test line fixed; `v_t25_corpus` |
| TCYCLE/empty | OK | no change, no message | – |
| TCYCLE/exceed | OK | `ERROR: Total cycle time(1.38475) has exceeded defined cycle time(0)` = H3(24) − H2(−13) | – |
| TCYCLE/gendb | OK (indirect) | GENDB prints `WARNING: DSMAX and DTMAX cannot be defined together. DSMAX will be used.` | test line fixed |
| TCYCLE/kzero | OK | reference | – |
| TCYCLE/nodb | OK | no change, no message | – |
| TCYCLE/positive | inconclusive (not visible) | as corpus | test line; `v_t25_pos` |
| TCYCLE/shift1 | inconclusive (not visible) | as corpus | test line; `v_t25_shift1` |
| TCYCLE/shift2 | inconclusive (not visible) | as corpus | test line; `v_t25_shift2` |
| TCYCLE/single | OK | DTMAX 9.9999892475E+5 = 1e6 − (11.87 − 10.79475098) | test line made exact |
| TCYCLE/v_base25 | – | DB DTMAX 1.0752490170 | new |
| TCYCLE/v_t25_corpus / pos / shift1 / shift2 / exceed | OK | 3.9247509830; 2.9999989248E+5; 2.4999902881E+5 (SimNo 6); 4.9999872250E+5 (SimNo 5, 9.41 − 8.3); −1.0752490170 + message `(1.07525)` | new |
| TCYCLE/v_t25_gendb | OK | DTMAX 2.9999989248E+5 read back from NEW.DB step −26 | new |
| TCYCLE/v_gendb24_read | – | DSMAX-mode DB read back shows only DSMAX | new |

Counts over the 76 original cases: OK 67 (incl. GENAXS/tol0 weak, TCYCLE/gendb indirect,
TCYCLE/kzero reference), prediction wrong / spec right 2 (GENAXS/objpos14, GENSUB/brick2), SPEC
WRONG 1 (GENBCC/plane), inconclusive 6 (CPYOBJ/mode3_2d, GENBCC/torsion, TCYCLE/corpus,
positive, shift1, shift2). The inconclusive ones are covered by the new `v_*` cases except
GENBCC/torsion.

## Spec corrections

1. **GENBCC.md §3 (field table) and §4 "Edge → slot"** — old: "NBE = obj+0x88c = sum of loop
   node counts; the last nLoops edge numbers give slot -1, stderr `ECC array index(-1) is out of
   range[0 -- NBF]` and a write to `ECCTMP[-1]` on the non-axisymmetric path". New: NBE is
   recomputed by `FUN_140165d30` (called inside the NBF helper `FUN_140165170`) as the sum over
   loops of `FUN_14017ebc0(loop)` = `cnt - 1` in 2D (loop node lists repeat the first node; 3D:
   face count `+0x10`); every `l = 1..NBE` maps to a real edge slot, no slot -1, no stderr, no
   out-of-bounds write for a consistent boundary. Evidence: `GENBCC/plane` stderr empty and
   `ECCTMP 1 0 2` (all slots 2); code `FUN_140165d30` @0x140165d30, `FUN_14017ebc0` @0x14017ebc0,
   `FUN_140305520` @0x140305520. §5 (stderr: nothing) and §7 updated.
2. **GENBCC.md §7 (new note)** — `GEOTYP` set after the mesh changes the BCCDEF width without
   re-layout (2→3 for GEOTYP 3); GENBCC then indexes past the 2-wide arrays. Evidence:
   `GENBCC/torsion` vs `GENBCC/v_torsion_ctl` (object 2 garbage row only after GENBCC). Belongs
   partly to the GEOTYP keyword spec (not mine): GEOTYP 3 after RZ/BCCDEF leaves inconsistent
   arrays.
3. **TCYCLE.md §3** — old: "H[2] and H[3] of a step header are both TNOW". New: H[2] = global time
   at the start of the step, H[3] = at its end (TNOW); equal only for the negative
   (start-of-operation) entries. `span = H[3](last) - H[2](first)` (formula unchanged). Evidence:
   `dbinfo --header` of COG_P1_B1.DB steps -12/12/-13/24/-25/25; `TCYCLE/v_t25_shift2` (span
   9.41 − 8.3) and `exceed` (1.38475).
4. **TCYCLE.md §5** — GENDB storing `model+0x920`: [inferred] → confirmed (`v_t25_gendb`, DTMAX
   read back from NEW.DB); KEY visibility only with `M+0x990 == 1` confirmed (step 24: no DTMAX
   line); new: in DSMAX mode GENDB warns `WARNING: DSMAX and DTMAX cannot be defined together.
   DSMAX will be used.` (`TCYCLE/gendb`).
5. **GENSUB.md §4.1 / §5 / §7** — AX_BIL.KEY left after the converter's error / return-4 paths:
   [inferred] → confirmed (`brick2`, `tet3d`); float rounding of MESH.DAT confirmed on 999 values
   (`quad2d`); §8 brick2 description corrected (ECCTMP 36 > 27 error path, no MESH.DAT).
6. **GENAXS.md §8** — old: "objpos14 … so the spring reset can trigger". New: OBJPOS 14 leaves
   `+0x2588` at 0 (no spring reset); it closes pair 4/5 by −u and GENAXS reopens it. The spring
   reset itself (§4 step 1) is confirmed by `GENAXS/v_spring`; all four branches of step 8 and the
   `>=` tolerance tests by `v_clearA_*`, `v_tolB85`, `v_tolB8499`.

## For other workers

* GEOTYP keyword (group 1 spec owner): a GEOTYP change after the mesh is read changes
  `m+0x1640` (BCCDEF width) without re-laying out the per-node arrays; see correction 2.
* OBJPOS mode 14 (OBJPOS owner): in the corpus cogging seed it moves gripper pair 4/5 by exactly
  −u of GENAXS (`v_objpos14_ctl` vs seed: −(0, −18.3739366, 7.67947042)) and leaves the MOVCTL
  type 5 spring value at 0 — useful as a cross-check of the OBJPOS 14 spec.
