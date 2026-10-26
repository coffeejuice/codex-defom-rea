# UNIT

Unit system: 1 = SI, 2 = English (inferred from the unit-dependent defaults, e.g. environment temperature 20 vs 68).

* registry id 30 (`0x1e`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x8 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` into a local. Only if the value differs from `M+0x84c` **and** is 1 or 2: store it, `FUN_1401f4100(S)` (reset unit-dependent environment defaults, below) and `FUN_140235c30(v)` (global unit flag `0x1406fe778` = 1 for 2 (English) else 0, and `FUN_1403f8b70`: `0x14070ba70` likewise, then `FUN_1403f9f20`). Other values are ignored silently.

## Model fields

`M+0x84c` (`S+0xfc`) int.

## Default

**0, and never explicitly initialised.** No constructor, static initialiser or INI step writes `S+0xfc`:

* The default chain `FUN_1401e7c50(S)` → `FUN_1401e7e90` sets `S+0xe4..+0x100` but skips `+0xfc` (checked in the instruction listing of `0x1401e7e90..0x1401e8030`: no store to `+0xfc`).
* Every store to `S+0xfc`/`M+0x84c` in the binary (displacement sweep): this handler (`0x14039adab`), the setter `FUN_1401f40b0` (`0x1401f40dc`; its only caller `FUN_1401c8f70` has no callers and is in no pointer table), the S copy `FUN_1401e8040` (`0x1401e80ce`, used by the step copies `FUN_1401f3660/36f0/37b0/3820`), the DB loaders `FUN_1401eb870` (`0x1401ec3ca`, `0x1401ecdfa`) and `FUN_1401effc0` (`0x1401f1357`, `0x1401f300f`), and the interactive menu `FUN_14000ce70` (`0x14000d2db`, prompt `" Enter Unit System: 1 - SI; 2 - English "`).
* Models are never static: they are stack locals or members of stack locals (`FUN_14001c9f0` app object `local_a7e8`, model at `+0x190` via `FUN_14001c770`; `FUN_14001c500` `-m` path; `FUN_1401c5810` DB-backed KFWRIT; `FUN_1401b6ea0`). So the field holds whatever the stack held.
* INI files (`FUN_140009f20`): before `DEF_PRE3.INI` / `DEF_PRE.INI` the model's vtable slot 0 (`FUN_1401c8ba0`, `_DB_STEP_DATA::vftable` `0x14052ae10`) sets the dimension to 3 / 2; the INI itself is read as an ordinary KEY file (`FUN_1401b5f00`), so only a `UNIT` keyword inside it changes the unit. `DEF_MULTI.INI` is the same.
* The KEY header comment (`* DEFORM 3D`, `DEFORM-2D`, `DEFORM-MACH2` …, first comment only, in `FUN_1401b6520`) calls the same slot 0. On a dimension change `FUN_1401c8ba0` → `FUN_1401cb440` + `FUN_1401c8c00` → `FUN_1401e7c50` rebuilds the defaults but leaves `S+0xfc` as it is, re-running `FUN_1401ea6b0` with the current unit.

The exact start value is therefore not determined by static code (uninitialised stack). Observed with the oracle runner: a keyword program with no `UNIT` (`KFWRIT` / `OUT.KEY` via menu 2 → 1, with and without `* DEFORM 3D`) writes `UNIT 0` and SI environment values (`UNTE2H 1.0`, `BLZMAN 5.669E-011`, `ENVTMP 0 20.0`, `CNVCOF 0 0.02`), i.e. the field was 0 (fresh, zero-filled stack pages). **Port: initialise to 0.**

With 0 the two unit tests disagree, and the port must keep both: construction (`FUN_1401ea6b0`) tests `== 2` (English) and so takes SI; the reset `FUN_1401f4100` tests `== 1` (SI) and so would take English, but it is only reached with 1 or 2. A KEY `UNIT 1` on a fresh model counts as a change from 0, so it runs `FUN_1401f4100` and resets the ENVTMP/CNVCOF/ENVATM/ACVCOF tables even though the values stay SI.

## KEY writer

`"%-6s %7i\n"` after MESHNO.

## Notes

`FUN_1401f4100(S)` re-initialises the tables at `M+0xb18` (ENVTMP), `M+0xc10` (CNVCOF), `M+0xc88` (ENVATM), `M+0xd00` (ACVCOF) to template 0 (constant), and sets: UNIT 1 → UNTE2H 1.0, BLZMAN 5.669e-11, ENVTMP 20.0, CNVCOF 0.02, ENVATM 1.69, ACVCOF 2.5e-4, ENVMPR 1.26e-9, ENVMPT 8.8e-15; UNIT 2 → UNTE2H 0.107, BLZMAN 3.306e-15, ENVTMP 68.0, CNVCOF 7.7e-6, ENVATM 1.69, ACVCOF 1e-5, ENVMPR 3.19e-8, ENVMPT 2.25e-13. It does not touch the ENVTMP region flag `M+0xb10`. So UNIT must precede those keywords in a KEY file, or it overwrites them.

## Helpers

`FUN_140413de0`, `FUN_1401f4100`, `FUN_140235c30`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 490 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
