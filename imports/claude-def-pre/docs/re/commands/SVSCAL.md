# SVSCAL (id 12, group 6, `case 0xc` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1503-1528, closure
`out/p67/svscal.c`, assembly of `FUN_140132a60`/`FUN_140132bf0`). Shared mechanics:
[_common.md](_common.md). **[fact]** = read in code, **[inferred]** = deduced.

## 1. Summary

"State-variable scaling": maps the **effective strain** of one object through a user
piecewise-linear table, `strain := f(strain)`, element by element (and the nodal strain too when
ELMNOD strain option is 2). Despite the name it is a remapping, not a multiplication. Same code in
2D and 3D **[fact]**.

```
SVSCAL <objNo> <kind> <n>
x1 y1
...
xn yn
```

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the SVSCAL line | `FUN_140413fb0(f, &a, &b, &c, NULL)` | `fgets(1024)`, `,`→space, `sscanf("%i %i %i")`; all zeroed first, missing values 0; returns 1 only on EOF / sscanf −1 (blank line) |
| table body | `FUN_14041a800(f, t, 0)` | `n` points `x y` read with `fscanf("%Flf")` (free format, may span lines, commas not accepted), then one `fgets` swallows the rest of the last data line (only when `n > 0`). See `docs/re/keywords/_common.md` §3 (branch `claude/project-thread-qj8op8`) |

* `a` = object number, `b` = "kind", `c` = number of table points.
* **`b` has no effect** **[fact]**: the code sets the table's abscissa variable `t+0x10 = 7` when
  `b == 1`, but template `0x81` (`FUN_14012d2a0` case `0x81`: `ndim = 1`, `ncomp = 1`,
  `var[0] = 7`) has already set it to 7. The scaling routine only acts when `var[0] == 7`, so
  `b = 0`, 1, 5, … all behave the same.
* `c` is stored as the point count `t+0x1c` before the body is read; negative counts are clamped
  to 0 by the table allocator (`FUN_14012fad0`, `_common.md` §3) **[fact]**.

Lines consumed: keyword line + the table body (unless the object is missing, see §5).

## 3. Model fields

| Field | Meaning (docs/dbmap/objarrays.md, branch `claude/project-thread-bz5k5e`) |
|---|---|
| `obj+0x9e0` element-field container; `FUN_140183fd0(c)` returns `c+0x48` = **`obj+0xa28` STRAIN** (eObjVarTyp 0x2b) | written |
| `obj+0x8a0` node-field container; `FUN_140181320(c)` returns `c+0x98` = **`obj+0x938` nodal STRAIN** (id 0x1b) | written when option 2 |
| `obj+0x884` / `obj+0x880` | element / node count |
| `M = *(obj+0x3a98)` (`obj+15000`, owning model) | `M+0x16a4` = W[0x2b] (strain values per element: 1, or the integration-point count for ELMNOD option 3); `M+0x1664` = W[0x1b]; `M+0x88c` = ELMNOD strain option |

Both accessors first make sure the array exists: `FUN_1401836c0` / `FUN_14017ffe0` with id
0x2b / 0x1b → `FUN_140329e60(container, n, W, id)` (allocation of a missing field
**[inferred]**, not followed), then the region loader `FUN_1401d48d0`, which is a no-op in this
build because `FUN_1401d2b70` always returns 1 (objarrays.md §1) **[fact]**.

## 4. Algorithm

1. `FUN_140413fb0` ≠ 0 → **return 2** **[fact]**.
2. `obj = FUN_1401d6ea0(model, a)` (list find, see WRTOBJ.md §3 step 4); NULL → **return 2**
   (the table body is **not** read; its lines are then parsed as keywords by the KEY loop, e.g.
   `0.0` → `WARNING: Keyword 0.0 not found. Skip ...`) **[fact]**.
3. Table on the stack: `FUN_14012d050(t)` (construct, template 0), `FUN_14012d2a0(t, 0x81)`;
   `if (b == 1) t.var[0] = 7`; `t.cnt[0] = c` **[fact]**.
4. `FUN_14041a800(f, t, 0)` ≠ 0 (EOF inside the body) → `FUN_14012f410(t)` (free), **return 2**.
5. `FUN_140204bc0(obj, t)` **[fact]**:
   ```
   e = strain array (obj+0xa28)      nd = strain array (obj+0x938)
   if (t.var[0] == 7) {
       for i in 0 .. obj+0x884 * M+0x16a4 - 1:   e[i] = F(t, e[i])
       if (M+0x88c == 2)
           for i in 0 .. obj+0x880 * M+0x1664 - 1: nd[i] = F(t, nd[i])
   }
   ```
6. `FUN_14012f410(t)`; return 0.

### Interpolation `F(t, x)` = `FUN_140132bf0` → `FUN_140132a60` **[fact]** (listing `0x140132a60..0x140132ca9`)

Let `X = t+0x28` (abscissae), `Y = t+0x48` (values), `n = t.cnt[0]`.

1. `FUN_14012f610(t)`: every `cnt[k] >= 1` for `k < ndim`, else **F = 0.0** (so `n <= 0` sets
   all strains to 0).
2. Segment `i`:
   * `x > X[n-1]` (`COMISD; JBE`) → `i = n-2` (linear **extrapolation** above);
   * else `x < X[0]` → `i = 0` (extrapolation below);
   * else the first `i` in `0..n-2` with `X[i] <= x <= X[i+1]` (a knot shared by two segments
     takes the lower one); none found (only for `n == 1`, or NaN) → `i = -1`.
3. `i < 0` → F = 0.0. Otherwise, in this order:
   ```
   r = (x - X[i]) / (X[i+1] - X[i])        SUBSD, SUBSD, DIVSD (true division)
   F = Y[i] + r * (Y[i+1] - Y[i])          SUBSD, MULSD, ADDSD
   ```
   No FMA, no reciprocal, double precision throughout. [listing-checked]
* `n == 1`: `x == X[0]` or `x > X[0]` → 0.0; `x < X[0]` → `i = 0` reads `X[1]`/`Y[1]` past the
  one-point arrays (undefined) **[fact]**; a port should reproduce 0.0 for the defined cases.
* Equal abscissae in the chosen segment → division by zero → ±inf/NaN propagate **[fact]**.

## 5. Outputs and errors

No stdout message from the command. Return codes: 2 for a blank/EOF first line, an unknown
object, or EOF inside the table body (KEY loop prints `ERROR: Keyword SVSCAL Reading Error. `);
0 otherwise, also when nothing is changed (object without mesh: counts 0) **[fact]**.

## 6. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140413fb0` | `%i %i %i` reader (5th arg = optional sscanf count out) | 317 | read fully |
| `FUN_1401d6ea0` / `FUN_14017aaf0` | object find | 39/85 | read fully |
| `FUN_14012d050` | table constructor (zero, template 0) | 174 | read fully |
| `FUN_14012d2a0` | table template; case `0x81` read | 3877 | case 0x81 + prologue read |
| `FUN_14041a800` | table body reader | 1034 | from `_common.md` §3 |
| `FUN_14012f410` | table destructor | 99 | read |
| `FUN_140204bc0` | strain remap | 338 | read fully |
| `FUN_140132bf0` / `FUN_140132a60` | 1-D interpolation / segment search | 185/384 | read fully + listing |
| `FUN_14012f610` | all point counts ≥ 1 | 69 | read fully |
| `FUN_140183fd0`, `FUN_140181320`, `FUN_1401836c0`, `FUN_14017ffe0`, `FUN_140182980`, `FUN_14017f020` | field accessors (ensure + return array) | | read; `FUN_140329e60` not followed |
| `FUN_1401d48d0` | region loader (dead here) | 3392 | read, dead |

## 7. Open items

* `FUN_140329e60` (field allocation on demand) not followed: if an object has no STRAIN array
  the remap probably runs over a freshly zeroed array, i.e. sets every value to `F(0)`
  **[inferred]**.

## 8. Oracle check (2026-10-21)

All 6 cases agree (`docs/re/commands/oracle/ov-misc.md`), including the two `WARNING: Keyword 0.0
not found. Skip  0.5` lines of `badobj`. The seed strains are uniformly 0, so `v_cog_interp`
(`cases_v_ov-misc.json`, DBREAD 24 of `COG_P1_B1.DB`, 1867 non-zero element strains 0..0.296,
table (0,0) (0.1,1) (0.2,1.5)) checks interior segments, the shared-knot rule and extrapolation
above the table: every value equals `F` of section 4 within the 10-digit print (max relative
difference 5e-11, the rounding of the printed inputs). Only element STRAIN changed (ELMNOD 10).
