# ANGMO2 (id 260, group 2, case 0x74)

The angular movement about the second (3D-only) rotation axis CNTRA2. It is the same layout as
ANGMOV, moved by `+0x130`:
* method at `obj+0x28b8` (0 = torque, 1 = RPUT/angular velocity; inferred from ANGMOV, since I
  found no menu string for ANGMO2 itself);
* method-0 table at `obj+0x28f0`;
* method-1 table at `obj+0x2968`.

## KEY grammar (read)

    ANGMO2 <objNo:%i> <mode:%i> <kind:%i> <v:%Flf>
    [kind 1 or 2: (int)v pairs "x y", free format]

* **3D only** (`FUN_1400015f0`). In 3D the grammar and every rule are exactly those of
  ANGMOV.md (line reader `FUN_140415f00`, kinds 0/1/2 → constant / variable code 1 / variable
  code 10, body `FUN_14041a800`).
* Not 3D: the case reads nothing after the object number and returns 0. The rest of the line
  (and any table lines) is then read by the interpreter as unknown keywords, each giving a
  warning and a skipped line.

## Model storage

| Field | mode 0 | mode 1 |
|---|---|---|
| method `obj+0x28b8` (int) | 0 | 1 |
| table T | `+0x28f0` | `+0x2968` |
| T.ndim | `+0x28f4` | `+0x296c` |
| constant T+8 | `+0x28f8` | `+0x2970` |
| T.var[0] | `+0x2900` | `+0x2978` |
| T.cnt[0] | `+0x290c` | `+0x2984` |

* Any other mode: nothing is stored, nothing more is read, and the case returns 0.
* T is reset with `FUN_14012fc70(T, 0)` before the kind is applied.

## Read errors and validation

* Object number EOF or objNo < 1 → 2.
* 3D: line helper EOF or blank line → **2**, with nothing changed. Body EOF → **2**.
* Not 3D → 0.

## KEY writer

`FUN_1402afe80` (handler2). It is called from `FUN_1402a7c80` **only in 3D**, after CNTRAX and
before CNTRA2. It is written for all object types.

The logic is identical to ANGMOV's writer `FUN_1402b0610`:
* mode outside 0/1 → nothing;
* constant: `"%-6s %7i %7i %7i %20.10E\n"` (mode, 0, `T+8`);
* table: `"%-6s %7i %7i %7i %7i\n"` (mode, kind 1/2 from var code 1/10, `T.cnt[0]`) followed by
  `FUN_1402f3130(f, T, 0)`;
* ndim not 0/1, or a var code that is not 1/10 → **return 3**. The rest of the movement block
  (CNTRA2, OBJUPD, OBJSTP) is then skipped silently; the caller ignores the status.

Three-digit exponents; N follows the `obj+0x3f24` renumbering rule.

## Unit conversion

None.

## Helpers

* `FUN_140415f00`, `FUN_14012fc70`, `FUN_14041a800`: reader side.
* `FUN_1402afe80`: writer.
* `FUN_1402f3130`: table writer.
* handler3 `0x1402f7610`: no-op. Unlike ANGMOV, there is no table-pointer handler.
* `FUN_140179900`: reads the active constant.

## Corpus check

All 491 lines are 3D lines of the form `mode 1 kind 0`, and every one parses. Example
`3D\SI\Multiple_Operations\New_MO_Release_Examples_SI\Equipments\Equipment00004.KEY`:

    ANGMO2       1       1       0    0.0000000000E+000

There are no table-form lines in the corpus.

## Open questions

* None beyond ANGMOV's (the meaning of variable codes 1 and 10).
