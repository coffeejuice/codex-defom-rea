# OBJPOS drop: notes on §5.12 / §5.13 from the listing

Corrections and additions to [OBJPOS_drop_solver.md](OBJPOS_drop_solver.md) §5.12 (equilibrium
test `FUN_140369c19`) and §5.13 (rigid update / step search `FUN_14036d3fd`), found while porting
them (packet p67-3, `port/defcmd/src/drop/settle_eqm.cpp`, `settle_step.cpp`). Source: listing
`out/p67/drop-solver/L_all.txt`, annotated listing with constant values
`out/p673/d/L2.txt` (`FuncListing.java`). **[fact]** = read in the listing.

## 1. Argument lists **[fact]**

Read at the call sites `0x14035f0ec`, `0x140361839` (equilibrium test), `0x140361402` (step
search) and from the callees' stack use (frames `0x2a8` and `0x368`).

* `FUN_140369c19` has 31 arguments: `RT, PT, X, XU, IT, F, FU, BC, SW, FID, NRM, XE, flags, VN,
  &nb, &nb', &nn, nodes, GAP, XB, d, &W, &kc, &damp, &p15, &stop, &nk, K, list840, &shift,
  &verbose`. Used: `X, BC, NRM, &nn, GAP, d, &p15, &stop, &nk, K, list840, &shift, &verbose`.
* `FUN_14036d3fd` has 40 arguments: `RT, PT, X, XU, IT, F, FU, BC, SW, FID, NRM, XE, flags, VN,
  &h, &nb, &nb', &nn, nodes, &ins, GAP, XB, d, &W, &kc, &damp, &p15, DAT_140704ea0,
  DAT_140704db0, DAT_140704dd0, &nk, K, list840, list890, list8e0, &nv, v, &g, &redrop,
  &verbose`. Arguments 28–30 are never touched. `&h` is the solver's current step
  `DAT_140704da0` (= dt).
* Its trial detection (`0x14036f14d`) passes 28 arguments to `FUN_1403704c9`: the solver's list
  with `Xt` for `X`, `tolscale = &DAT_140706210` (0.0), the contact count into
  **`DAT_140706218`** (not the solver's `nc` `DAT_140704d88`) and `flag = &ins` (0). The trial
  therefore overwrites `BC, FID, NRM, XE, GAP` but leaves `nc` (and `kc`) at the values of the
  last real detection; after the accepted move `BC` and `GAP` describe the **last trial**, not
  the accepted position, until the main routine detects again.
* The initial drop of the step search (`0x14036d5ea`) gets 21 arguments, the same list as the
  main routine's call: `RT, PT, X, XU, IT, F, FU, BC, SW, FID, NRM, XE, flags, VN, &nb, &nb',
  nodes, &ins, &redrop, d, &p15`. It is called when `nn < 1` as well as when no node is in
  contact.
* `FUN_1404ded0c` is called as `(x, y, origin, T, &n, &mode)` (input, output, origin, frame).

## 2. Equilibrium test `FUN_140369c19` (§5.12)

* The control flow and decisions of §5.12 are confirmed. All FP tests are `COMISD` with a `JP`
  guard, so each is false when an operand is NaN (`q < best`, `z > 1e-14`, `z < −1e-14`,
  `q < cL`, `cL > cos 179°`, `GAP < gmin`, `|M'_a| > 1e-5`, `dot(Fc, d) <= cos 91°`).
* nk = 0 needs `cnt >= 3`, nk = 1 needs `cnt >= 2` (`0x14036cc66`); with `nk >= 2` any contact
  stops (`0x140369d69`). After a one-sided decision the nk = 0 block jumps back to `0x140369dc3`
  (the nk = 1 test), which falls into the moment test.
* **Static carry-over:** the most-opposite search stores its winner in `DAT_140705d40`, the same
  static that receives the cross products of the side test. The search starts at `best = 2.0`
  and normally updates on the first candidate; if no `q < best` (NaN directions), `ref` becomes
  the previous call's last cross product. The frame `T` (`DAT_140705be0`) is a static written by
  `FUN_1404cf5c0`, which leaves it unchanged for a zero `d` (cannot happen after the set-up).
* The 6-double copies (`d00 → d40 → c90`, `M6 ± K(1:3,1)` at `0x14036ac4b`/`0x14036ace2`) move
  three slots (`0x140705d18.., 0x140705d58.., 0x140705ca8.., 0x140705db8.., 0x140705f48..`) that
  no instruction reads; `FUN_140365bfc` reads and writes only 3 components (listing
  `0x140365bfc`). The mirrored moment is therefore `M' = T·(mirror(M + P) − P)` with 3-vectors.
* Moment test early return: `gmin < 0 and |gmin| > p15 * 2.0` (`0x14036a144..0x14036a178`),
  `gmin` over the list in order with strict `<` from `1e10`.
* `nk = 0` and no supporting contact: shift `Δ_i = ((d_i + Fm_i) * p15) * 10.0`; then
  `c_i = Δ_i + c_i`, `X(:,n) = X(:,n) + Δ` for `n = 1..nn`, `A_k = Δ + A_k`, `CT = Δ + CT`
  (`0x14036a3a0..0x14036a645`), `shift = 1`.
* Written-only statics: `DAT_140705d90`, `DAT_140705d98` (constants copied in), `DAT_140705e08`,
  `DAT_140705e0c` (index of the minimum gap), `DAT_140705cc0` as a GAP copy.

## 3. Step search `FUN_14036d3fd` (§5.13)

* **`h0` is always written.** `wmax` starts at `0.0` and is raised only by ordered `>` tests
  (`0x14036ea22`, `0x14036ea5c`, `0x14036ea8c`), so it is never NaN or negative. Either
  `wmax > 0` → `h0 = (g * 0.017453292519943295) / wmax` (`0x14036eab2`) or `wmax == 0` →
  `h0 = *h`, `w := 0` in the body COMMON (`ce38..ce48`), `mode = 0` (`0x14036eafd`). The
  sentence "wmax NaN: h0 keeps its value from the previous call" in §5.13 is wrong (an infinite
  `wmax` gives `h0 = 0`).
* **The 90 limit.** After a trial with **no contact at all** (`cntN == 0`, also for `nn < 1`):
  `if r >= 90.0 (DAT_140bafd18): ACCEPT(h, full)` (`0x14036f1e2`), before the growth step. §9
  ("90 only for nn < 1") is wrong; the §5.13 pseudo-code lacks this branch. The full-step test
  `r >= 130` (`SETNC` at `0x14036f1b0`) is computed before the trial's contact list is built but
  taken after it, so `cntN` is the trial's count in the `5·cnt` override.
* Search, as compiled (`hs` = the step of the trial, `hc` = the step variable `DAT_1407061c8`):

```
r = (g * hs) / h0
r >= 130                         : accept full (s = hs)
cntN == 0:  r >= 90              : accept full
            lo = hs; if hs > hi  : hc = hs + h0; next trial
            else                 : hc = (hs + hi) * 0.5; |hi − hs| < h0 * 0.001f → accept (s = hc * 0.5)
cntN > 0, fresh nodes nf > 0:
            gm > 0 or gm >= −p15 : accept (s = hs * 0.5)
            else                 : hc = (hs + lo) * 0.5; hi = hs; |hs − lo| < h0 * 0.001f → accept (s = hc * 0.5)
cntN > 0, nf == 0                : as cntN == 0 without the 90 test
```
  The growth test is `hs > hi` (spec: `hi < h`; the same for ordered values, NaN takes the
  bisection). `0.001f` = `DAT_140bafd20` = 0.0010000000474974513.
* **Accept, mode > 0:** the rotation always uses the step **variable** `hc`: `h_new` in the two
  bisection-converged branches (already stored at `0x14036fc90`, `0x14036ffb7`, `0x140370056`),
  `hs` otherwise; no half factor. `ang = |dot(ax, w * hc)| * ax` into `cdf0..ce00`, move about
  `P1` with tracking.
* **Accept, mode 0** (`0x1403700d5`): `if cntN > 5 * cnt: s = hc`; `if r >= 50: s = hc`;
  `O_i = (v_i * s) + c_i`, `ang_i = w_i * s` into `cdf0..ce00`, move from `c` to `O` with
  tracking. In the gm branches `s = hs * 0.5`, in the converged branches `s = hc * 0.5` with
  `hc = h_new`, in the full branches `s = hs`.
* Dead paths kept in the port: `cnt < 3` after the `cnt == 1`/`2` tests, `cnt < 1` before the
  constraint block, `nf < 1` after `nf != 0`, and the return on the flag `DAT_1407061e4` of the
  empty stub `FUN_14001cd40` (never written; it would skip freeing `Xt`).
* Written-only statics: `DAT_140706170` (accept kind), `DAT_140706178`, `DAT_140706108`,
  `DAT_140706138`, `DAT_140706250`, `DAT_14070624c`, and `DAT_140706230` on the mode > 0 path.
