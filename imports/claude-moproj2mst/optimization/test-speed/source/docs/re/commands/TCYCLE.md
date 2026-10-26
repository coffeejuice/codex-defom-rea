# TCYCLE (id 18, group 6, `case 0x12` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1701-1750, dispatcher
listing `0x14037d981..0x14037dc51`, closure `out/p67/tcycle.c`). Shared mechanics:
[_common.md](_common.md). Facts are marked **[fact]**, deductions **[inferred]**.

## 1. Summary

Sets the time increment per step (`DTMAX` first value, `model+0x920`) so that the remaining steps
of the current operation fill a prescribed cycle time. The time already used is measured in the
**open database** (`model+0x3390`, opened by an earlier `DBREAD`): the global time span (TNOW) of
the saved steps of one operation (simulation number). Same code in 2D and 3D **[fact]**.

```
TCYCLE <k> <n> <T> <d2> <d3>
```

`dt = ((1 + n) * T - (span + d2 + d3)) / k` **[fact]**. Meaning of the arguments **[inferred]**
from the formula and the operation shift (section 4 step 4): `k` = number of steps left to run,
`n` = cycle index (0 = first cycle), `T` = cycle time, `d2`, `d3` = durations of up to two
further operations that follow the measured one (each non-zero one moves the measured operation
one simulation number back). Corpus: `TCYCLE 1 0 5 0 0` (375 times, all in
`key/3D/EN/Cogging/Cogging_Multiple_Ops_Lab1/Cogging_Multiple_Ops_Lab1.MST`).

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the TCYCLE line | `FUN_140416100(f, &k, &n, &T, &d2, &d3)` | all five outputs zeroed, `fgets(buf, 0x400)`, `,` → space (`FUN_140323970`), `sscanf("%i %i %Flf %Flf %Flf")` (`%Flf` = double). Missing values stay 0 |

* The dispatcher also zeroes `T`, `d2`, `d3` before the call (`0x14037d981..`) **[fact]**.
* Quirk **[fact]**: the reader's return value (1 on EOF / blank line) is discarded; the case tests
  the dispatcher status `[RSP+0x12d4]`, which is 0 at entry (`0x140378e47`). A failed read is
  therefore never reported: all values are 0, `k = 0`, and nothing is changed (step 6).
* `%i` accepts `0x` / leading-zero octal. Exactly one line is consumed.

## 3. Model fields

| Field | Access | Name |
|---|---|---|
| `model+0x3390` | read (saved at dispatcher entry, `[RSP+0x12c8]`) | open DB object `db` (`docs/re/dbmap/stepdriver.md`, branch `claude/project-thread-bz5k5e`) |
| `model+0x740` | read | directory index of the step loaded by DBREAD (set by `FUN_1401d7f40`; 0 at model reset) |
| `model+0x920` | **written** | DTMAX[0] = `S+0x1d0` (time increment per step; `docs/re/keywords/DTMAX.md`) |
| `db+0xac8` | read | NRSETS, number of directory entries |
| `db+0xad8` | read | directory, 0x28 bytes per entry: `+4` StepNo, `+0x14` SimNo, `+0x1c` dim, `+0x24` has-arrays flag |
| `db+0xae8`, `db+0x1900` | written by `FUN_1401dd250` | header cache (0xe0 bytes per entry: `+0` NOBJ, `+4` H[1], `+8` H[2], `+0x10` H[3], `+0x28` stroke); "cache complete" flag |

H[2] and H[3] of a step header are the global times at the **start** and the **end** of the step
(corrected after oracle run; the earlier text said both are TNOW): in `COG_P1_B1.DB` step 24 has
H[2] = 10.774, H[3] = 10.79475098 (= TNOW), step 25 has 10.79475098 / 11.87, and the negative
(start-of-operation) entries have H[2] = H[3] (-13: 9.41 / 9.41, -25: 10.79475098 / 10.79475098)
(`dbinfo --step N --header`). So `span = H[3](last) - H[2](first)` is the duration of the
operation; the arithmetic below is unchanged. Only `model+0x920` is changed; DSMAX (`model+0x930`) and the time/stroke flag
`model+0x990` are **not** touched (unlike the DTMAX keyword reader) **[fact]**.

## 4. Algorithm

All floating-point work is in the dispatcher (listing `0x14037da6e..0x14037dc49`).

1. Read the arguments (section 2).
2. `local_418 = local_428 = 0.0`, `local_438 = local_408 = 0` (outputs of step 5) **[fact]**.
3. `sim = FUN_1401dde10(db, model+0x740)`: 0 when `db+0xad8 == NULL` or the index is outside
   `0 .. db+0xac8-1`; else directory entry `idx`'s SimNo (`+0x14`) **[fact]**.
   If `sim <= 0` → nothing more (no DB open, or SimNo 0) **[fact]**.
4. `FUN_1401dd250(db, 0)`: if `db+0xac8 > 0` and `db+0x1900 == 0`, allocate the header cache
   `db+0xae8` if absent (`NRSETS` records of 0xe0 bytes), read the step header of **every**
   directory entry into it (`FUN_1401dfb80`, see the DBREAD header-cache description,
   stepdriver.md 1.6), re-read the DB header (`FUN_1401dd0d0`), set `db+0x1900 = 1`. Done once
   per open DB **[fact]**; file errors inside are silent and leave cache records as constructed.
5. Operation shift **[fact]** (constant `0.0` at `0x14051bed8`):
   `if (d2 > 0.0) sim -= 1;` then `if (d3 > 0.0) sim -= 1;` (`COMISD` / `JBE`: NaN does not shift).
6. `FUN_1401df8a0(db, sim, &dim, &nobj, &h1, &stepFirst, &stepLast, &tFirst, &tLast, &sFirst, &sLast)`
   **[fact]**: scans the directory in order; for each entry with `+0x24 != 0` (has arrays) and
   SimNo `== sim`: the **first** such entry gives `dim` (`+0x1c`), `nobj` (cache `+0`), `h1`
   (cache `+4`), `stepFirst = stepLast` (`+4`), `tFirst` = cache `+8` (H[2]) and `sFirst` = cache
   `+0x28`; every **later** one overwrites `stepLast`, `tLast` = cache `+0x10` (H[3]) and
   `sLast`. Returns 0 when nothing matched or the arrays are missing (the result is stored in
   `[RSP+0x65b0]` and never used). Only `tFirst` / `tLast` are used below.
7. `span = tLast - tFirst` (`SUBSD`); `if (0.0 > span) span = 0.0` **[fact]**. With a single
   matching step `tLast` stays 0.0, so `span = max(0, -tFirst)`; with none, `span = 0`.
8. Continue only if `span >= 0.0` (`COMISD span, [0x14051bed8]`; `JC` skips: only a NaN span
   fails) **and** `k > 0` **[fact]**. Otherwise nothing is written.
9. Floating-point order (listing `0x14037db98..0x14037dbf8`), all double, SSE2 **[fact]** [listing-checked]:
   ```
   total = (span + d2) + d3;                         // ADDSD, ADDSD
   dt    = ((1.0 + (double)n) * T - total) / (double)k;   // 1.0 = DAT_140509c90; CVTDQ2PD, ADDSD, MULSD, SUBSD, CVTDQ2PD, DIVSD
   ```
   Note the operand order `1.0 + (double)n` (constant first) and a true division by `k`.
10. If `0.0 >= dt` (`COMISD 0, dt`; `JC` skips the message when `0 < dt`; a NaN `dt` also
    skips it): `printf("ERROR: Total cycle time(%g) has exceeded defined cycle time(%g)\n", total, T)`
    **[fact]** (string `0x140555c70`, `printf` `0x14047af54`).
11. `model+0x920 = dt` — **also when it is <= 0** (the error does not stop the store) **[fact]**.

Status: the case breaks with `iVar4 = 0` on every path → dispatcher returns 0 **[fact]**.

## 5. Outputs

* stdout: only the message of step 10 (`%g` of `total` then `T`).
* Model: `model+0x920`. Visible in a KEY written afterwards only through the DTMAX writer, which
  prints `M+0x920` only when the time flag `M+0x990 == 1` (DTMAX.md) **[fact, oracle]**: after
  `DBREAD 24` (DSMAX mode) OUT.KEY keeps `DSMAX 2.48` and has no DTMAX line; after `DBREAD 25`
  (time steps) the new DTMAX is printed. GENDB stores it **[oracle]**: `TCYCLE/v_t25_gendb` reads
  step -26 back from NEW.DB with the same DTMAX. In DSMAX mode GENDB's checker prints
  `WARNING: DSMAX and DTMAX cannot be defined together. DSMAX will be used.` (TCYCLE does not
  clear DSMAX, unlike the DTMAX keyword) **[oracle, TCYCLE/gendb]**.
* Side effect: the DB header cache of every step is loaded (`db+0xae8`, `db+0x1900`), which also
  updates the global min/max table `db+0xaf0` used by post-processing displays **[fact]**.
* No files written. Return code 0.

## 6. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140416100` | `%i %i %Flf %Flf %Flf` line reader | 281 | read fully |
| `FUN_1401dde10` | SimNo of directory entry `idx` (bounds-checked) | 83 | read fully |
| `FUN_1401dd250` | load the header cache of all entries once | 621 | read fully |
| `FUN_1401dfb80` | read one step header into the cache (Fortran unit 3, `FUN_1403adc74` open, `FUN_1403daf3e` read) | 3747 | summarised (stepdriver.md 1.6) |
| `FUN_1401dd0d0` | re-read DB header record 2 (`FUN_1403d84b0`) | 372 | summarised |
| `FUN_1401df8a0` | first/last saved step of a SimNo: times, step numbers, stroke | 561 | read fully |
| `printf` `0x14047af54` | CRT | | |

## 7. Open items

* The argument meanings in section 1 are inferred; the code only fixes the arithmetic.
* `FUN_1401dfb80` / `FUN_1403daf3e` error behaviour when a step header cannot be read (stale
  cache values would then enter `tFirst` / `tLast`) is not followed.
* Whether GENDB writes `model+0x920` for a step-based (`M+0x990 != 1`) model: the DB read back
  (`TCYCLE/v_gendb24_read`) shows only DSMAX, which proves nothing either way.

## 8. Tests

`tests/commands/TCYCLE/` (DB step `COG_P1_B1.DB`; its directory has 7 operations: steps -1/1,
-2/2, -3/3, -4..11, -12/12, -13..24, -25/25 per the cached `-m simStepList` run):
`corpus.KEY` (`TCYCLE 1 0 5 0 0` after DBREAD 24), `positive.KEY` (large T), `exceed.KEY`
(T = 0 → error message, negative DTMAX stored), `shift1.KEY` / `shift2.KEY` (`d2`, `d2+d3` > 0),
`single.KEY` (DBREAD 25, operation with one saved step pair), `kzero.KEY` (k = 0, no change),
`empty.KEY` (no arguments: read failure ignored), `nodb.KEY` (no DB open, no change), `gendb.KEY`
(DTMAX through GENDB).

Oracle results (2026-10-21, `docs/re/commands/oracle/ov-misc.md`): the step-24 cases cannot show
DTMAX (DSMAX mode); only the message of `exceed` (`Total cycle time(1.38475)`) checks the
arithmetic there. `cases_v_ov-misc.json` repeats the formula cases after `DBREAD 25`
(time-based): `v_t25_corpus` 3.9247509830, `v_t25_pos` 2.9999989248E+5, `v_t25_shift1`
(SimNo 6) 2.4999902881E+5, `v_t25_shift2` (SimNo 5, span 9.41 - 8.3) 4.9999872250E+5,
`v_t25_exceed` -1.0752490170 with `Total cycle time(1.07525)`, `single` 9.9999892475E+5 — all
equal to the formula with the header times above; `v_base25` gives the DB value 1.0752490170.
