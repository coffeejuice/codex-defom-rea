# Oracle check: INICTC and GENCTC (2D and 3D) (worker ov-ctc)

Cases: `tests/commands/INICTC/cases.json` (5), `tests/commands/GENCTC/cases.json` (16),
`cases_s3d.json` (10), `cases_d3d.json` (7); 23 extra cases in
`tests/commands/GENCTC/cases_v_ov-ctc.json` (`v_*.KEY`). Results:
`deform-rea-work/out/p67/oracle/res/<label>/`. Method: messages between `Keyword File = [ ? ] ?`
and `Hit <CR>` plus the whole stdout (unit-6 lines land earlier, see below), stderr.txt and the
files/ listing; OUT.KEY parsed per object (RZ, BCCDEF) and compared node by node with
`res/_baseline/<seed>/files/OUT.KEY` (scripts `deform-rea-work/out/p67/ov-ctc/kdiff.py`, `tab.py`).

Seeds: UPSET_Brick obj 2 = 4×4×4 nodes, x ∈ {0,4,8,12}, y ∈ {0,5,10,15}, z ∈ {0,4,11,15},
node = 1 + ix + 4·iz + 16·iy; x = 0 face fixed in x, y = 0 face fixed in y (quarter model);
seed codes: −1 on the 16 top nodes, −3 on the 16 bottom nodes. Dies 1 (z 15..55) and 3
(z −40..0) are rigid boxes with a mesh. SPIKE_SI obj 1: −3 on nodes 1-14, −2 on node 318.

Answers to the five questions:

1. 2D GENCTC messages go to **stderr** (CR LF line ends), not stdout (`v_spike_push1004`).
2. NEWEQ.MSG is never left in files/ (no case has it, while DIE_00k.GEO files are captured):
   deleted at exit, as specified.
3. "Accept at distance 0 for status 0/−2000": status 0 (and 2) can never reach the decision in
   GENCTC (mode-0 kernel skips them); the −2000/−k acceptance at distance 0 happens and is seen in
   `d3d_two_masters` (nodes up to 15 away accepted and moved to the nearest valid die polygon).
4. Meshed-master setup (`CNTACT 1 2 1`) works as guessed: die-1 nodes over the brick get −2;
   additionally obj 2 loses its −1 codes (die 1 is no longer its partner).
5. DIEGEO.DAT: `DIE_001.GEO`, `DIE_003.GEO` written in the specified format; OUT.KEY unchanged;
   18 of 54 polygons per die carry `n4 < 0` = disabled by symmetry planes (new finding).

## Per case

| Label | Verdict | What differed | Fix |
|---|---|---|---|
| INICTC/brick_obj2 | OK | −1×16, −3×16 → 0; positives kept | — |
| INICTC/missing | OK | `ERROR: Keyword INICTC Reading Error. ` | — |
| INICTC/noline | OK | same message, model unchanged | — |
| INICTC/spike_all | prediction incomplete | also the −2 of node 318 is cleared | test line |
| INICTC/tet_obj2 | OK | −1×25, −3×25 → 0 | — |
| GENCTC/brick_0 | prediction wrong, spec right | auto tol comes from die 1 (0.6), not obj 2; OUT.KEY = baseline | test line; GENCTC.md §3 clarified |
| GENCTC/brick_large | prediction wrong | no extra node within 1.0; = baseline | test line |
| GENCTC/brick_small | OK | = baseline | test line |
| GENCTC/brick_noinictc | OK | = baseline, no unit-6 message | test line |
| GENCTC/brick_objpos | inconclusive | OBJPOS line had direction (2,0,0): die moved −30 in x, all top −1 lost | line fixed; `v_brick_objpos_fixed` run |
| GENCTC/noline | OK | Reading Error | — |
| GENCTC/spike_0, spike_tiny, text | OK (weak) | = baseline (seed contact nodes lie on the dies) | test lines; tol checked by `v_spike_auto_*` |
| GENCTC/spike_small | OK (weak) | = baseline | test line |
| GENCTC/spike_large | inconclusive | no node moved > 100·tol, no warning | test line; `v_spike_push1004` |
| GENCTC/spike_gendb | OK (messages) | DB generated, no error; DB content not inspected | — |
| GENCTC/spike_objpos | prediction wrong, spec right | die ends 0.2329 above (OBJPOS result) → node 318 loses −2 | test line |
| GENCTC/tet_0, tet_small, tet_large | OK (weak) | = baseline | test lines |
| GENCTC/d3d_move_rows | OK (refined) | only outer-face nodes + 5, 9; symmetry-face-only nodes skipped (shared-code test); 5, 9 coded, not moved | test line, decide §8 |
| GENCTC/d3d_noinictc_tol5 | OK | = d3d_move_rows | — |
| GENCTC/d3d_release | OK | top −1 released, not moved | — |
| GENCTC/d3d_repeat | OK | = one call | — |
| GENCTC/d3d_repeat_grow | OK | = d3d_move_rows | — |
| GENCTC/d3d_separation | OK | top nodes −1, moved to z = 15.5 | — |
| GENCTC/d3d_two_masters | SPEC WRONG / test does not exercise the claim | partner–partner contact (tol 16 ≥ gap 15): KOBJ lines on stdout, die polygons disabled, all −1, nodes moved to (30,y,15) etc., no −3, x code of 5, 9, 13 cleared | specs corrected; `v_two_masters_12` |
| GENCTC/s3d_gap_in | OK | top −1, z → 15.0009 | — |
| GENCTC/s3d_gap_out | OK | top −1 lost, not moved | — |
| GENCTC/s3d_penetrate | OK | top −1, z → 14.9995 | — |
| GENCTC/s3d_lateral | OK | only x = 8, 12 top nodes −1 (→ 15.0005) | — |
| GENCTC/s3d_on_edge | OK | x ≥ 4 top nodes −1, x = 0 lost | — |
| GENCTC/s3d_meshed_master_brick | OK (incomplete) | die-1 nodes 1, 2, 17, 18 → −2; obj 2 loses −1 | test line |
| GENCTC/s3d_meshed_master_tet | OK (incomplete) | same; nodes 1, 2, 17 moved onto the brick edge (x/y −0.0001 → ~2e-15) | test line |
| GENCTC/s3d_srcnt2_dat | OK | = s3d_gap_in | test line |
| GENCTC/s3d_skip_pntr | OK | = s3d_gap_in | test line |
| GENCTC/s3d_diegeo_dat | OK + new finding | GEO files as specified; 18/54 polygons `n4 < 0` | search spec §7.2, §11.3 |

Extra cases (`cases_v_ov-ctc.json`):

| Label | Result |
|---|---|
| v_brick_objpos_fixed | die at z = 14.9999; top nodes −1, moved to 14.9999, except 46-48 rejected (|d| = tol = 0.0001, strict `<`, round-off) — bit-exactness case |
| v_two_masters_12 | two-code rule exactly as decide §4.6.4: −1 z + −3 y (−3 x for 8, 12), not moved; 5, 9 only −1 |
| v_auto_in / v_auto_out (0.24 / 0.26) | both accepted → auto tol ≠ 0.25 |
| v_explicit025 (0.26, tol 0.25) | rejected |
| v_auto_lift0.4 / 0.6 / 1.0 / 2.0 / 4.0 | accepted / mixed (10 of 16) / rejected ×3 → auto tol 0.6 (die 1 mesh) |
| v_spike_auto_in / out (0.05 / 0.07) | node 318 kept (moved 0.0487) / released → 2D auto tol 0.0579 from obj 1 |
| v_spike_push1004 | stderr: three 1004 warnings (nodes 318, 327, 332), nothing committed |
| v_spike_push05 | nodes 318, 327 → −2 and moved; no warning |
| v_d0_x / v_d0_y / v_d0_far | nodes beside the shifted die rejected (0.00127 > tol); no distance-0 acceptance via symmetry-disabled faces |
| v_d0_x_geo | the shifted x-face is still disabled (`n4 < 0`) |
| v_two_masters_geo | = d3d_two_masters; die 1: 51/54, die 3: 42/54 polygons disabled |
| v_two_masters_skip | no unit-6 lines; result differs (die 1 y = 36 face valid, nodes moved there too); x-code loss of 5, 9, 13 persists |
| v_two_masters_srcnt2 | = d3d_two_masters |

Numbers checked: every moved 3D coordinate equals the die's displaced surface exactly at the KEY's
10 digits (15.0009, 14.9995, 15.24, 15.5, 0 / 15 for the side rows); 2D node 318 moved
0.0487197 for a 0.05 lift (die surface inclined, cos ≈ 0.974).

## Spec corrections

1. **GENCTC.md §3 (3D tolerance quirk)** — old: the automatic value of "the first slave" is kept.
   New: it is computed at driver step 3, before the partner list, for the first object slot that
   passes the step-2 filter (found, type ≠ 7/5, has a mesh) even if that object has no partners;
   in UPSET_Brick/UPSET_Tet that is the meshed rigid die 1, so `GENCTC 0` uses
   `0.05 × 12 = 0.6`, not `0.05 × 5 = 0.25`. Evidence: v_auto_out vs v_explicit025, v_auto_lift*;
   `FUN_1401a6c80` step order (§5.1 steps 2-4).
2. **GENCTC.md §1 table, §4.3, §9 item 2 (2D messages)** — old: unit 0 → stderr [inferred]. New:
   confirmed stderr, CR LF line ends; 1004 format and numbers as printed in v_spike_push1004
   (`distance      -0.9743939`).
3. **GENCTC.md §5.5 / GENCTC_3d_search.md §12 (stdout order)** — new: unit-6 lines are
   unbuffered and appear before earlier C++ output in a captured transcript (right after the
   `Keyword File = [ ? ] ?` prompt). Evidence d3d_two_masters stdout lines 52-54.
4. **GENCTC_3d_search.md §7.2 step 3 (SYMGRP2 `FUN_14002a857`)** — old: planes only from codes
   6xx, none otherwise. New: planes from O polygons with a common fixed code 1 in direction k and
   `|ER1·(±e_k)| ≥ 0.99` (`DAT_140bad3e0`), first polygon per direction/sign, tolerance
   `sqrt(ER2)·0.1` (`DAT_140bad2d0`), at most 100; UPSET seeds: x = 0 and y = 0. Evidence
   s3d_diegeo_dat GEO files.
5. **GENCTC_3d_search.md §7.2 step 4 / §3.3 EI2 (`FUN_14003d2ee`)** — new: partner polygons with
   EI2 = 1, `|ER1·N| ≥ 0.995` (`DAT_140bad7f0`) and centroid within the plane tolerance get
   EI2 = 0; the UPSET dies lose their x = 0 and y = 0 faces (18/54 each) for projection.
   Evidence s3d_diegeo_dat, v_d0_x_geo, v_d0_x/v_d0_y.
6. **GENCTC.md §5 note, GENCTC_3d_search.md §0/§12/§15 (SRCNT2/4/5 inactive)** — old: inactive
   for a plain GENCTC. New: inactive only while no two partners are within tol of each other;
   with tol ≥ die gap partner–partner contact activates SRCNT2/SRCNT4, prints
   ` Interference detected, KOBJ=  1` / `3` and ` Info: Checking Invisible definition`, and
   disables most die polygons (51/54, 42/54). Evidence d3d_two_masters, v_two_masters_geo/_skip.
7. **GENCTC_3d_decide.md §3.4 (kernel flag)** — old: with flag 0 "polygons with EI2 == flag are
   also valid". New: mode 0 never returns EI2 ∈ {0, 2, −1000}; mode −O returns only EI2 = 1 and
   EI2 = −O (search spec §9.2).
8. **GENCTC_3d_decide.md §4.6.2 / §7 item 2 (distance-0 acceptance)** — new: st2 = 0/2 is
   unreachable in GENCTC; st2 = −2000/−k gives acceptance at distance 0 and a move to the
   re-projection on the nearest valid polygon (d3d_two_masters: node 2 (4,0,0) → (30,0,15)); die 3
   then skipped (`best.obj ≠ p`). Open: fixed x code cleared on nodes 5, 9, 13 in that case.
9. **GENCTC.md §5.4** — old: "keeps the nearer one". New: two contacts within tol → both codes,
   no move; else first accepted partner (v_two_masters_12).
10. **GENCTC.md §5.5** — `DIE_000.GEO` → `DIE_00k.GEO` per partner (s3d_diegeo_dat).

## Notes for other workers

* OBJPOS (ov-objpos): `OBJPOS 2 2 0.0001 1 0.0 -1.0` after `OBJPOS 2 1 0.0 5.0` on SPIKE_SI
  leaves die 2 0.2329 above its seed position (`CNTRAX 2 5.838e-16 0.23288`); 3D
  `OBJPOS 1 2 0.0001 0 0 -1 2` after a 5 lift lands at −0.0001 (penetration = v2). Worth a look
  if the OBJPOS spec predicts touching contact in 2D.
* Porting: v_auto_lift0.6 and v_brick_objpos_fixed are |d| = tol borderline cases decided by the
  round-off of the quad projection; good bit-exactness tests for the 3D kernel port.
