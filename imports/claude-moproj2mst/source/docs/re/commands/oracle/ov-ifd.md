# Oracle check: OBJPOS interference (modes 2/12/102/112) and drop (modes 7/8/9)

Worker ov-ifd. Cases `tests/commands/OBJPOS/cases_ifd.json` (47) and `cases_drop.json` (3),
plus 10 extra cases in `cases_v_ov-ifd.json` (labels `OBJPOS/v-ifd-*`, files `ifd_v_*.KEY`,
`drop_v_*.KEY`). Results: `out/p67/oracle/res/<label>/`, compared with the seed baselines
`res/_baseline/{UPSET_Brick,SPIKE_SI}` (per-object node/DIEGEO displacement, changed keywords;
script `out/p67/ov-ifd/summ.py`, full dump `out/p67/ov-ifd/summary.txt`). All 60 runs exit 0,
stderr empty, no NEWEQ.MSG or other files besides OUT.KEY.

"net" = change of the object's nodes against the seed (sum of all OBJPOS lines in the case).
Every moved object also has CNTRAX / CNTRA2 changed by the same translation for modes 1/2 (not for
drops, see correction 3).

## Cases

| Label | Verdict | Observed / what differed | Fix |
|---|---|---|---|
| 2-explicit-3d | OK | `Object 1 has been interference positioned w.r.t. Object 2.`, moved (0,0,-5) (net 0), STROKE unchanged | – |
| 2-depth-3d | OK | moved -5.5 (net -0.5) | – |
| 2-negdepth-3d | OK | moved -5.5 (fabs) | – |
| 2-movctldir-3d | OK | as 2-explicit-3d | – |
| 2-oblique-3d | OK | moved (0,5,-5) = w·7.0711, net (0,5,0) | ORACLE line added |
| 2-overlap-3d | OK | moved (0,0,+2) back to contact | – |
| 102-overlap-3d | OK (resolves the either/or) | moved (0,0,+2): the face behind the node is returned by the bucket query (hit -2), as kernels §2.4 / ik_m102_bucket2 predict | ORACLE line added |
| 12-explicit-3d | OK | STROKE 1 = (0,0,-5) | – |
| 102-explicit-3d | OK | (0,0,-5), STROKE unchanged | – |
| 112-explicit-3d | OK | STROKE 1 = (0,0,-5) | – |
| 2-deformable-3d | OK | workpiece moved -3 | – |
| 2-tet-3d | OK | workpiece moved -3 (net 8.9e-16) | – |
| 2-auto-3d | OK | w.r.t. Object 2, -5 | – |
| 12-auto-3d | OK | + STROKE -5 | – |
| 102-auto-3d | OK | w.r.t. 2, -5 | – |
| 112-auto-3d | OK | + STROKE -5 | – |
| 2-auto-behind-3d | OK | `w.r.t. Object 3`, workpiece moved -56 (net -55) | – |
| 2-auto-coupledexcluded-3d | OK | w.r.t. 3, `Object 1 has been moved by (0, 0, -3).` | – |
| 2-coupled-3d | OK | both -3, message as predicted | – |
| 12-coupled-3d | OK | STROKE 1 = STROKE 2 = (0,0,-3) | – |
| 2-coupled-movctldir-3d | OK | die 1 moved -55 (net -50), `Object 3 has been moved by (0, 0, 0).` | – |
| 2-nomovctldir-3d | OK | `Object 2 to Object 3 interference positioning failed.`, nothing moved | – |
| 2-undefined-target-3d | OK | `ERROR: Keyword OBJPOS Data Error.` | – |
| 2-undefined-mover-3d | OK | same | – |
| 2-nohit-3d | OK | failed message, die stays at +5 | – |
| 2-explicit-2d | prediction wrong, spec right | area 0 % → `Object 2 to Object 1 interference positioning failed.` (the CORRECTION line was right) | header rewritten |
| 2-area-2d | OK | `Object 2 is interference positioned w.r.t. Object 1.`, die 2 moved (1.57e-15, -12.8415477900); independent area calc gives -12.839 (within the 1e-3 area tolerance) | ORACLE line |
| 2-movctldir-2d | prediction wrong, spec right | area 0 → failed (no CORRECTION line had been added) | header fixed; MOVCTL fallback re-checked by v-ifd-area1-dir0-2d: identical to 2-area-2d |
| 12-explicit-2d | inconclusive (area 0) | failed; STROKE 2 += stale (0,10) | header fixed; v-ifd-area1-12-2d: STROKE 2 = displacement (−12.8415) |
| 102-explicit-2d | inconclusive (area 0) | failed | v-ifd-area1-102-2d: success, -12.8380134961 |
| 112-explicit-2d | inconclusive (area 0) | failed; STROKE 2 = (0,10) | v-ifd-area1-112-2d: STROKE 2 = displacement |
| 2-coupled-2d | inconclusive (area 0) | failed, die 3 not moved | v-ifd-area1-coupled-2d: `Object 3(coupled) has been moved by (-1.00394e-014, 81.9779).`, both dies +81.978 — the original prediction (die 3 behind wins, both move up ~82) holds |
| 12-coupled-2d | inconclusive (area 0) | failed, STROKE 2 = (0,10) | v-ifd-area1-12coupled-2d: STROKE 2 = STROKE 3 = (−1e-14, 81.978) |
| 2-target0-2d | OK | Data Error, nothing moved | – |
| 12-failed-stroke-2d | OK | failed, STROKE 2 = (0,10); area 0 would fail anyway | misleading CORRECTION line removed; v-ifd-failstroke-area1-2d (1 %) fails the same way |
| 7-drop-3d | OK | `Object 2 has been dropped successfully.`, moved (0,0,-3.001) (gap + p15/2) | ORACLE line |
| 7-default-tol-3d | OK | identical | ORACLE line |
| 7-tilted-3d | OK | rotated back exactly flat; net pure translation (0, -0.0190265095, -0.001) = (0, -5(1-cos5°), -0.001) | ORACLE line (+ v-ifd-rot-only-3d shows the tilt reaches the nodes) |
| 8-drop-axis-3d | OK | as 7-drop-3d | ORACLE line |
| 9-drop-vector-3d | OK | as 7-drop-3d | ORACLE line |
| 9-drop-zerovector-3d | OK | as 7-drop-3d | ORACLE line |
| 7-drop-coupled-3d | OK | `Object 1 has been moved by drop positioning.`, die 1 net 6.999 | ORACLE line |
| 7-drop-rigid-3d | OK (vague prediction) | success, but die 1 tips over the workpiece edge and falls past all surfaces until the iteration limit (box z -174.6..-122.1, x 84..135) | ORACLE line |
| 7-drop-up-3d | prediction wrong, spec right | success but no motion (part already touches die 3) | header fixed; v-ifd-drop-up-lifted-3d (lifted 3 first): moved (0,0,7.001) |
| 7-drop-zerodir-3d | OK | `Failure in drop-positioning object 2.` | – |
| 7-undefined-3d | OK | Data Error | – |
| 7-in-2d | OK | no message, OUT.KEY = baseline | – |
| drop-gap05-3d | OK | moved (0,0,-3.25) exactly | – |
| drop-inplace-3d | OK | no motion, success | – |
| drop-nohit-3d | OK | no motion, success | – |

Counts (50 assigned cases): OK 42 (102-overlap-3d resolved its either/or prediction), prediction
wrong / spec right 3 (2-explicit-2d, 2-movctldir-2d, 7-drop-up-3d), inconclusive 5 (12-, 102-,
112-explicit-2d, 2-coupled-2d, 12-coupled-2d: area 0 makes them fail before the checked feature;
all re-run with 1 % in `cases_v_ov-ifd.json` and confirmed), SPEC WRONG 0. One spec gap filled
(CNTRAX in drops, correction 3).

## Spec corrections / additions

1. **OBJPOS_interference.md §7.3 / OBJPOS_interference_kernels.md §4.6** — claim "area 0 % never
   converges, fails after 100 steps": **confirmed**. Evidence: the 7 area-0 2D cases fail, the
   1 % variants succeed; listing `0x14033fade` `CMP EAX,0x64; JG` (100 steps), relative test
   `0x14033fb9d..0x14033fbb7` with `JP` (NaN = not converged). Refinement added: `s` equals `lo`
   only up to rounding, `A(s)` is 0 or tiny positive (NaN or ±Inf relative error), `hi` never
   moves because `σ = σlo = +1` on every step.
2. **OBJPOS_interference.md §1** — addition: the interference move also translates CNTRAX / CNTRA2
   of each moved object (also with a zero axis vector). Evidence `OBJPOS/2-depth-3d` (CNTRAX 1 z
   -0.5), `OBJPOS/2-area-2d`.
3. **OBJPOS_drop.md §4 step 6** (corrected after oracle run) — `FUN_14021dfa0(mv, M)` (called via
   `FUN_1401fd730`) transforms the point/vector pairs `mv+0x630/+0x648` and `mv+0x760/+0x778`
   only when the vector is non-zero (|·| >= 1e-12); these are CNTRAX / CNTRA2 [inferred]. So a drop
   leaves CNTRAX/CNTRA2 with a zero axis unchanged while the nodes move (modes 1/2 move them
   always). Evidence `OBJPOS/7-drop-3d` (CNTRAX 2 stays z 3, nodes at -0.001),
   `OBJPOS/7-drop-coupled-3d` (CNTRAX 1 stays 10, nodes 6.999); code `0x14021dfa0`
   (`out/p67/ov-ifd/dfa0.c`).
4. **OBJPOS_drop_solver.md §5.12** — consequence added: no test compares contact normals with the
   drop direction, so a body touching a fixed surface does not move for any direction (it is in
   contact at the first detection and geometrically stable). Evidence `OBJPOS/7-drop-up-3d`
   (no motion), `OBJPOS/v-ifd-drop-up-lifted-3d` (+7.001).
5. **Numeric confirmations recorded in the specs** (no change of claims): drop moves by gap +
   p15/2 (`7-drop-3d` -3.001, `drop-gap05-3d` -3.25); 3D back-off sign rule (`2-auto-behind-3d`,
   `2-coupled-movctldir-3d`) and its 2D analogue (`v-ifd-area1-coupled-2d`); bucket face-behind
   rule (`102-overlap-3d`); iteration limit reported as success (`7-drop-rigid-3d`).
