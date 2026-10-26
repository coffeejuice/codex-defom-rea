# MGNELM (id 184, group 2, case 0x28)

Mesh-generation element counts for an object (name: "mesh number of elements"). 2D stores one
target count, 3D a group of four integers; the last one is a 0/1 flag.

## KEY grammar (read)
3D (`model+0x1c != 2`):

    MGNELM <objNo:%i> <a:%i> <b:%i> <c:%i> <flag:%i>        (rest of line, FUN_140414100)

Commas count as spaces; missing trailing values are 0 (corpus has 3D lines with only `a b`).

2D (`FUN_1400015e0` true): the rest of the line is tokenized (`FUN_140413900`, max 4 tokens,
separators space , \n \t \r), tokens converted with `strtol(...,10)` (decimal only, no hex/octal):

    MGNELM <objNo:%i> <n:dec> [<b:dec> [<flag:dec>]]

Token 0 is always used (empty line → 0). Tokens 1 and 2 are used only if the line has at least
2 tokens; a missing token 2 then gives 0. A 4th token is ignored. One line, no continuation.

## Model storage
* 3D: `a → obj+0x2d88`, `b → obj+0x2d8c`, `c → obj+0x2d90`, `flag → obj+0x2d94` (int32 each).
  After the read, `obj+0x2d94` outside 0..1 is set to 0.
* 2D: `n → obj+0x2d80`; if ≥ 2 tokens: `b → obj+0x2d8c`, `flag → obj+0x2d94` (no clamping in 2D).
  With a single token `obj+0x2d8c`/`obj+0x2d94` keep their previous values.
* Both: if the object's type `obj+0xc == 5`, the byte `obj+0x2e05` is set to 0 (enables writing of
  the mesh-settings block for type-5 objects, see writer). In 3D this happens even when the read
  failed.

## Read errors and validation
* objNo EOF / < 1 → 2 (common prologue, `group2-helpers.md`).
* 3D: helper returns 1 (EOF, or empty/blank rest of line) → 2; all four values are then 0
  (flag clamp leaves 0).
* 2D: tokenizer EOF → 2 immediately (no `+0x2e05` reset). Empty line is not an error (n = 0).
* No range checks besides the 3D flag clamp. Return codes: see `_common.md` §1 (2/3 print an
  error to stdout and the KEY loop continues).

## KEY writer
Inline in `FUN_1402a9180`, line 9 of the block (order table below).

* 3D: `"%-6s %7i %7i %7i %7i %7i\n"` name, objNo, `+0x2d88`, `+0x2d8c`, `+0x2d90`, `+0x2d94`.
* 2D: `"%-6s %7i %7i %7i %7i\n"` name, objNo, `+0x2d80`, `+0x2d8c`, `+0x2d94`.

Always written when the block is written. Round-trips (2D values are read with `strtol`, so a
negative or plain integer written by `%7i` reads back identically). `+0x2d88/+0x2d90` are not
written in 2D, `+0x2d80` not in 3D.

### Position in the mesh-settings writer `FUN_1402a9180`

`FUN_1402a9180(model, f, obj, objNo)` writes the remeshing/mesh-generation block of one object.
It is called from `FUN_14029e250`:

* object type 5 (`obj+0xc == 5`): after `FUN_1402a7c80`, only when the byte `obj+0x2e05 == 0`
  (reading `MGNELM` for a type-5 object clears it);
* other non-empty objects (type != 7): after `FUN_1402b5880` succeeded, only when bit `0x10000` of
  the write options `model+0x1744` is clear.

The caller ignores its return value. Line order (each step runs only if every previous registry
handler returned 0; the handlers listed here always return 0):

| # | keyword | writer |
|---|---|---|
| 1-3 | RMDPTH, RMSTRK, RMTIME | handlers `FUN_1402b49d0`, `FUN_1402b4aa0`, `FUN_1402b4b70` (see their specs) |
| 4 | RMSTEP | inline, `"%-6s %7i %20.10E\n"` from `obj+0x2dd8` (see its spec) |
| 5 | MGSIZR | handler `FUN_1402b44e0` |
| 6 | MGWTMP | handler `FUN_1402b4680` |
| 7 | MGWSTN | handler `FUN_1402b4750` |
| 8 | MGWSTR | handler `FUN_1402b4820` |
| 9 | MGNELM | inline (2D/3D formats differ) |
| 10 | MGTELM | inline |
| 11 | MGWCUV | handler `FUN_1402b45b0` |
| 12 | MGGRID | inline |
| 13 | MGERR | handler `FUN_1402b48f0` |
| 14 | MGWUSR | inline (2D/3D formats differ) |
| 15 | MGUSER | inline loop over the mesh windows |

MGSIZM is never written. `objNo` is the (possibly renumbered) number the caller prints; the
registry handlers print `obj+0x3f24` when > 0, which the caller has set to the same number.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x28 — reader.
* `FUN_140414100` — `%i %i %i %i` rest-of-line reader (3D).
* `FUN_140413900` / `FUN_140413a50` / `FUN_14047b6c0` — tokenizer, token access, `strtol` (2D).
* `FUN_1400015e0` — 2D test.
* `FUN_1402a9180` — mesh-settings block writer; called from `FUN_14029e250`.

## Corpus check
* 2D `key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\MeshSettings00001.KEY`:
  `MGNELM       1    1625   32000       0` — 3 tokens, parses (n=1625, b=32000, flag=0); format
  matches the 2D writer. 2D files with a single value (`MGNELM       1     500`,
  `key\2D\EN\Creep\CREEP_Example1_EN.KEY`) also parse.
* 3D `key\3D\EN\Diestress\3D_Shrinkfit.KEY`: `MGNELM       1    2000    2000       0       0`
  matches the 3D writer; `key\3D\EN\Diestress\3D_DieStress_Ex1.KEY` has
  `MGNELM       2    2000    1000` (c, flag = 0).
* The surrounding line order in both files matches the writer order table.

## Open questions
* Meaning of `+0x2d88/+0x2d8c/+0x2d90` in 3D and why 2D uses `+0x2d80` for the first value:
  check the remesh/mesh-generation consumers of these fields.
