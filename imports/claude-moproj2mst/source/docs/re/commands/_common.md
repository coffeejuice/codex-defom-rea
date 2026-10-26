# Command keywords (registry group 6): shared mechanics

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (headless Ghidra decompiles of
`FUN_140378e00` and its callees, assembly where floating-point order matters). Every per-command
spec in this folder refers to this file. Facts are marked **[fact]** (read in the code) or
**[inferred]** (deduced, not followed to the end).

## 1. Dispatch

The KEY interpreter `FUN_1401b6520` reads a token with `fscanf("%39s")`, looks it up in the
registry (`docs/re/keyword_registry.tsv`, `FUN_1402f76c0`) and, for group 6, calls
**`FUN_140378e00(model, FILE *f, int id)`**. The file position is just after the keyword token,
still on the keyword's line. Unlike group 2, the dispatcher reads **no object number** first:
each case consumes its own arguments **[fact]**.

`FUN_140378e00` first saves `model+0x3390` (the open DB handle, used by DBREAD/TCYCLE). Ids
below 0x11f go through `switch(id)`; ids 0x11f and above through `switch(id - 0x11f)` **[fact]**.

| Id | Keyword | Case | Spec |
|---:|---|---|---|
| 5 | INICTC | `case 5` | [INICTC.md](INICTC.md) |
| 6 | GENCTC | `case 6` | [GENCTC.md](GENCTC.md) |
| 7 | OBJPOS | `case 7` | [OBJPOS.md](OBJPOS.md) |
| 9 | WRTOBJ | `case 9` | [WRTOBJ.md](WRTOBJ.md) |
| 12 | SVSCAL | `case 0xc` | [SVSCAL.md](SVSCAL.md) |
| 13 | CPYOBJ | `case 0xd` | [CPYOBJ.md](CPYOBJ.md) |
| 14 | DOESCL | `case 0xe` | [DOESCL.md](DOESCL.md) |
| 18 | TCYCLE | `case 0x12` | [TCYCLE.md](TCYCLE.md) |
| 19 | GENAXS | `case 0x13` | [GENAXS.md](GENAXS.md) |
| 20 | GENSUB | `case 0x14` | [GENSUB.md](GENSUB.md) |
| 295 | GENBCC | `case 0x127-0x11f = 8` (high switch) | [GENBCC.md](GENBCC.md) |

Return value: each case leaves a status in `iVar4`; at the end the dispatcher returns **0** when it
is 0 and **2** otherwise. Some cases return 1, 2 or 3 directly. The interpreter prints
`ERROR: Keyword %s Reading Error.` for 2 and `ERROR: Keyword %s Data Error.` for 3 and continues
with the next token (see the groups 1/3 `_common.md`, section 1, branch
`claude/project-thread-qj8op8`) **[fact]**.

Quirk worth keeping: several cases test `local_56d8._4_4_` (the status of an earlier read)
after a reader whose result they did not store, for example WRTOBJ after `FUN_140413ee0`. The
test is then always false, so a read failure there is not reported **[fact]**.

## 2. Model helpers used by several commands

| Address | What it does |
|---|---|
| `FUN_1400015e0(model)` | true when the model is 2D (`dim == 2`); `FUN_1400015f0` = 3D **[fact]** |
| `FUN_1401d6ea0(model, n)` | `FUN_14017aaf0(model+0x1380, n)`: find object number `n` in the object list; NULL when missing (lookup only, unlike `FUN_14019ccc0` used by group 2; see WRTOBJ.md for `FUN_14017aaf0`) |
| `FUN_14019ccc0(model, n)` | get-or-create object `n` (group 2 helper) |
| `FUN_1401611f0(obj)` | usable object: type `+0xc != 7` (`FUN_140160f10`) else prints `ERROR  : Object %d Undefined`; then `FUN_140160ba0(obj)` (mesh or die present) else prints `ERROR  : No Mesh or Die Defined for Object %d`; returns false after either message **[fact]** |
| `FUN_140160cc0(obj)` / `FUN_140203980(obj)` | geometry-present tests that branch on the dimension of the object's model (`obj+15000` = owning model); details in OBJPOS.md |
| `FUN_1401f3ae0(model+0x750)` | current simulation time (simulation-control object) |

Object fields used here (see `docs/re/dbmap/object.md`, branch `claude/project-thread-bz5k5e`,
and the group 2 keyword specs, branch `claude/project-thread-j4c7t9`): `+8` object number,
`+0xc` type (7 = empty slot), `+0x87c` has-mesh flag, `+0x880` node count, `+0x884` element
count, `+0x1ea0` process sub-object, `+0x2160` movement sub-object (`+0x21b0..+0x21c0`
direction, `+0x21c8` movement type), `+0x22d0..+0x22e0` STROKE, `+0x2d10` remesh sub-object,
`+0x2e68` BC windows.

## 3. Argument readers

The rest-of-line readers (`FUN_140414990` `%lf`, `FUN_140413de0` `%i`, `FUN_140413fb0`
`%i %i %i`, `FUN_1404154c0` `%i %lf`, `FUN_140415680` `%i %lf %lf`, ...), the tokenizer
`FUN_140413900` / `FUN_140413a50` with `strtol` (`FUN_14047b6c0`) and `atof` (`FUN_14048754c`),
the string reader `FUN_140413ab0` and the table reader `FUN_14041a800` are documented in
`docs/re/keywords/group2-helpers.md` (branch `claude/project-thread-j4c7t9`) and
`docs/re/keywords/_common.md` (branch `claude/project-thread-qj8op8`). Readers this folder needed
and those files do not list are described in the command spec that uses them.

Important for scripts: the tokenizer counts slot 0 even on an empty line and missing tokens
convert to 0, so a short OBJPOS line silently fills the remaining values with 0 **[fact]**.

## 4. Test scripts (`tests/commands/<COMMAND>/`)

Each command has a folder with:

* `*.KEY` command files. They start from a corpus seed with `KFREAD`, run the command, and
  write the result with `KFWRIT` (and `GENDB` where the DB matters):

  ```
  KFREAD 0
  UPSET_Brick.KEY
  OBJPOS 2 1 0.0 0.0 -1.0
  KFWRIT
  OUT.KEY
  ```

* `cases.json`: a list of cases for `tools/oracle/def_pre_oracle.py cases`. Each case runs one
  command file through menu 2 → 1 (`"stdin": "\n2\n1\nCMD.KEY\n\nE\nE\nY\n"`), places the seed
  from `C:/Users/alext/Projects/DEFORM_corpus/...` and the command file as inputs, and has a
  label `<COMMAND>/<option>`. The oracle has not run them yet; expected stdout lines quoted in the
  specs come from the code, not from a run.

Seeds (corpus copies, all SI): 3D `key/3D/SI/Forging/UPSET_Brick.KEY` (3 objects, brick mesh)
and `key/3D/SI/Forging/UPSET_Tet.KEY`; 2D `key/2D/SI/Forging/SPIKE_SI.KEY` (3 objects).
