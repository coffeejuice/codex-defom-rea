# Oracle check: OBJPOS geometric modes, geo kernels, mirror rebuild, bucket search (worker ov-objpos)

Cases: `tests/commands/OBJPOS/cases.json` (37), `cases_gk.json` (5), `cases_ik.json` (4); extra
cases in `cases_v_ov-objpos.json` (8). Oracle results: `deform-rea-work/out/p67/oracle/res/OBJPOS/<label>/`.
Method: the messages between `Keyword File = [ ? ] ?` and `Hit <CR>` are compared with the prediction;
OUT.KEY is diffed per keyword block against the seed's baseline; moved, rotated and mirrored values
(RZ, DRZ, URZ, DIEGEO, CNTRAX, SYMSUF, STRESS, STNCMP, MATAXI, ELMCON, ECCTMP) are recomputed in
Python from the spec formulas and compared at the KEY's 10 digits. Where GENDB wrote a DB, the
recomputed doubles are searched as big-endian 8-byte patterns in NEW.DB (bit check). Scripts are in
the session scratchpad (`ovobj/`: `summ.py`, `num.py`, `fnd.py` FNDBND + VS2008 qsort, `bits*.py`, `ctr.py`).

## Per case

| Case | Verdict | What was checked / what differed | Fix |
|---|---|---|---|
| 1-basic_2d, 1-basic_3d | OK | message; RZ, DIEGEO, CNTRAX (+CNTRA2 3D) += d; STROKE unchanged | - |
| 1-coupled_2d, 1-coupled_3d | OK | `(coupled)` line in 2D, three lines in 3D, all objects moved | - |
| 11-basic_2d, 11-basic_3d, 11-coupled_3d | OK | STROKE += d for main and coupled objects | - |
| 13-sub0_3d | OK | YZ centring of ref 1 by (0,-18,-35), object 2 by (0,0,7.5): both recomputed from the bboxes | - |
| 13-sub1_3d | OK | WPAXIS item created as `WPAXIS 2 1 3 1` with zero vectors; object 3 moved by (0,0,20) = proj(0) | spec open item 7 closed |
| 13-sub2_3d, 13-coupled_3d | OK | (0,0,33.5) and (0,0,35) recomputed; coupled follows | - |
| 13-noref_3d, 14-noB_3d, 14-noref_3d | OK | `ERROR: Keyword OBJPOS Data Error.` only, model unchanged | - |
| 14-sub1_3d | OK | object 1 +15 z, object 3 -15 z (ref 2 height 15), no message | - |
| 3-basic_2d, 3-coupled_2d | OK | RZ, DIEGEO (6.6a), MATAXI += angle, CNTRAX centre about (5,20) | - |
| 3-basic_3d, 3-off_3d, 3-ydir_3d, 3-coupled_3d | OK | RZ with 6.3 (both frame branches), DIEGEO with 6.4; CNTRAX not rotated when the axis is zero (3-off_3d) | - |
| 3-zero_3d | OK | message, no change | - |
| 4-basic_2d, 4-oblique_2d | OK | RZ (6.6b), ELMCON 1<->3, DIEGEO type-1 points mirrored and order reversed | - |
| 4-basic_3d, 4-oblique_3d | OK | RZ, DIEGEO float mirror (a double mirror misses by up to 4e-6), ELMCON 1<->3, 5<->7, DIEGEO faces reversed, ECCTMP via FNDBND, CNTRAX centre -> (4,4,4) | - |
| 4-zero_3d | OK | message, no change | - |
| 5-basic_2d, 5-first_2d, 5-noop_3d | OK | repeat (0,-5); (0,0) message without change; 3D no message | - |
| 6-basic_2d, 6-coupled_2d | OK | centre (31.75, 68.9057) = DIEGEO bbox centre, RZ rotated about it | - |
| 6-basic_3d | OK | origin (6,7.5,7.5) (quad rule of a box), RZ and DIEGEO | - |
| err-undef_2d/3d, err-unknown_2d/3d | OK | Data Error / nothing | - |
| gk-tensor-y_3d, gk-tensor-obl_3d | OK | STRESS, STNCMP, RZ, DRZ, URZ, SYMSUF: KEY and **bit-exact in NEW.DB** (all 29868 + 29868 tensor doubles) | - |
| gk-6-tri_3d | OK | origin printed (0.500461, 0.502164, -0.21001) reproduced by the triangle rule over FNDBND faces; RZ 1268/1269 rows at 10 digits (1 last digit on a 2e-6 value) | geo open item 3 closed |
| gk-tensor_2d | inconclusive -> fixed | seed DEF_BOOLEAN_OUT.KEY fails KFREAD (`ELMCON Reading Error`, `KFREAD Reading Error`), then `No Mesh or Die Defined for Object 1` + Data Error | seed replaced by Resistance_Spot_Welding_..._Hold_cycle_SI.KEY, GENDB dropped (refused); rerun: STRESS, STNCMP, RZ, DRZ, URZ, MATAXI OK |
| gk-4-type2_2d | OK | line record ends mirrored and swapped, r5 = 0; TPLOGY vertex data follow the new end points | observation added to OBJPOS.md 6.6 |
| ik-mirror-brick-3d, ik-mirror-tet-3d | OK | ECCTMP face list (order and node order, 36 / 144 faces) reproduced by FNDBND (1.2-1.5) + re-mapping (1.7) | note: tie order of the qsort not exercised (see below) |
| ik-m102-bucket-3d | OK | die bottom ends at z = 11 (not 15); `interference positioned` message | - |
| ik-m102-bucket2-3d | OK (prediction refined) | die back at z = 15; the distance is 2.0000000000000004 (CNTRAX z = 4.44e-16) | prediction line updated |
| v-bits-rot_3d | OK | 40.6 deg: RZ/DRZ/STRESS/STNCMP bit-exact with the Fortran SIN/COS (AMD: 44-66%); DIEGEO bit-exact (234/234) only with AMD libm + the exact 4x4 product sequence | specs: confirmed |
| v-tensor_2d, v-mirror-bits_2d | OK | KEY level (GENDB refuses the seed: volume compensation) | - |
| v-mirror-bits_3d | OK | float mirror bit-exact in NEW.DB (RZ 192/192, DIEGEO 168/168) | - |
| v-dir0_3d | OK | D = 0, angle 30: all nodes -> (0,0,0) (zero static frame), STRESS/STNCMP/YLDS NaN (`-1.#IND`), YLDS now written with 4978 rows, DRZ/URZ written with count 0 | geo 1.2 text corrected |
| v-tplogy_2d | OK (observation) | translate: TPLOGY vertex data of the type-2 edge follow the edge | - |
| v-ecflx-ctrl_2d, v-ecflx_2d | OK | after the 2D mirror ECPRES follows the reversed faces, ECHFLX and ECAFLX stay on the old face indices (geo 4.1) | - |

Counts: 46 listed cases: 45 OK (one with a refined prediction), 1 inconclusive (gk-tensor_2d, fixed
and re-run OK). 8 extra cases, all consistent with the specs. No SPEC WRONG in the transforms.

## CNTRAX / CNTRA2 under the three transforms (asked by the coordinator)

* Translate `FUN_14021d0b0` -> `FUN_14021dba0`: both centres are moved **unconditionally** (no
  axis test, no die gate). Evidence: `1-basic_3d`, `11-basic_3d`, `1-coupled_3d` shift CNTRAX and
  CNTRA2 with a zero axis; the interference modes share this translate.
* Rotate: gated by `FUN_140203980` and by a non-zero axis; with a zero axis the centre stays
  (`3-off_3d`, origin (10,-5,3), CNTRAX stays (0,0,0)).
* 3D mirror `FUN_1401fea60`: no gate; `4-oblique_3d` moves the centre (0,0,0) to (4,4,4), axis stays 0.

## Spec corrections and confirmations

1. OBJPOS.md 6.1 (translate, movement item): old: CNTRAX/CNTRA2 centre `+= d` (no condition
   stated). New: explicitly unconditional (no axis or die test), unlike rotate and drop. Evidence:
   `1-basic_3d`, `11-basic_3d`; `FUN_14021dba0`.
2. OBJPOS.md 6.2 step 4: added that the zero-axis test also keeps the centre. Evidence `3-off_3d`.
3. OBJPOS.md 6.3 / OBJPOS_geo_kernels.md 3.2: point and vector rotation bit-exact with
   `(a/45)*(pi/4)` and the Fortran SIN/COS (`v-bits-rot_3d`, 40.6 deg); D = 0 maps all nodes to O
   at program start (`v-dir0_3d`).
4. OBJPOS.md 6.4 / geo_kernels 3.1: old "transcription not checked against DEF_PRE". New: die
   geometry is bit-exact with AMD libm sin/cos and the exact I*T, (A*R)*C, apply sequence; UCRT or
   the Fortran kernel or a `(p-O)R+O` shortcut are not (63/234). Evidence `v-bits-rot_3d`, `FUN_1402214c0`.
5. OBJPOS.md 6.5: float mirror bit-exact (`v-mirror-bits_3d`); boundary rebuild order of
   OBJPOS_interference_kernels.md 1.2-1.7 reproduces ECCTMP (`ik-mirror-*`, `4-*_3d`).
6. OBJPOS.md 6.6 step 1: type-1 point lists are reversed too (geo 4.2 step 2); TPLOGY vertex
   coordinates of type-2 edges follow the edge end points after mirror and translate (oracle
   observation, code not followed: KFWRIT may regenerate them). Evidence `gk-4-type2_2d`, `v-tplogy_2d`.
7. OBJPOS.md 7.5 / open item 7: new WPAXIS item = `WPAXIS ref 1 3 1`, vectors A and B zero.
   Evidence `13-sub1_3d`, `FUN_140259950`.
8. OBJPOS_geo_kernels.md 1.2: old: "`OBJPOS/3-zero_3d` exercises D = 0". New: 3-zero_3d has angle 0
   and D = (0,0,1); `v-dir0_3d` shows D = 0 gives NaN tensors (STRESS, STNCMP and YLDS; YLDS then
   written with all rows) (corrected after oracle run).
9. OBJPOS_geo_kernels.md 1.2/1.3: tensor kernels bit-exact (3D DB) / KEY-exact (2D); 4.1 ECHFLX /
   ECAFLX quirk confirmed (`v-ecflx_2d`); 4.2 confirmed; open items 2, 3 closed; test table: the
   gk-tensor_2d seed was replaced.

## For other workers

* OBJPOS_interference_kernels.md (owner of the ik spec): (a) 1.2-1.7 reproduce the oracle face lists
  exactly, but with these meshes a stable sort gives the same lists (every equal-sum run has
  distinct (F1,F2)), so the VS2008 qsort tie behaviour of 1.5 is not yet confirmed by any case; a
  mesh with two boundary faces of equal node sum and equal (F1,F2) would be needed. (b) 7 / 2.4:
  `ik-m102-bucket2-3d` moves the die by 2.0000000000000004, not exactly 2 (CNTRAX z goes from -2
  to 4.44e-16); the predicted `h = -2` is exact only to 1 ulp, so the distance comes from a
  computation that rounds (worth a note for the bit-exact port).
* 2D GENDB is refused for the Resistance-welding seed (`Volume compensation activated`; seed
  check, not OBJPOS) and no other 2D case runs GENDB, so no 2D case has a DB bit check.
