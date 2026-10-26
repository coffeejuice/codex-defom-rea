# OBJTYP (id 145, group 2, case 0x01)

Object type code (rigid / plastic / elastic / porous / ... / empty) plus a second integer.
The KEY code is mapped to an internal type at `obj+0xc`; internal 7 means "empty object"
(`FUN_14019ccc0` creates placeholder objects with type 7, and `FUN_140161260` empties an object
and sets 7). The internal type decides what the object writer emits.

## KEY grammar (read)

    OBJTYP <objNo:%i> <code:%i> <aux:%i>

Rest of line read by `FUN_140413ee0` (`"%i %i"`, commas = spaces, missing values 0, extra
tokens ignored). Same in 2D and 3D.

## Model storage

| KEY code | internal `obj+0xc` |
|---|---|
| 1 | 0 |
| 2 | 1 |
| 3 | 2 |
| 4 | 3 |
| 5 | 4 |
| 6 | **unchanged** (no assignment; checked in the disassembly at `0x14038bb3a`) |
| 7 | 6 |
| anything else (0, negative, ≥ 8, non-numeric → 0) | 7 |

* `obj+0x2154` (int) = `aux`, always (also for code 6).
* If the resulting `obj+0xc == 7` (also when it was already 7 and the code was 6),
  `FUN_140161260(obj)` empties the object: `FUN_14015fbf0(obj, &0)`, releases the 100 owned
  sub-objects at `obj+0x3c00` (virtual call slot 2) and resets the 100 entries at `obj+0xbe0`
  (0x30 bytes each), then `+0xc = 7`, name `+0x828` = "" , byte `+0x3990 = 0`,
  `+0x87c..+0x88c` (5 ints) = 0.
* Default of a newly created object: `FUN_14015f0b0` (object init, run from the constructor
  `FUN_14015ec80` when the list has an owner model) sets `+0xc = 0` (KEY code 1) and the name to
  "". Placeholder objects created for missing lower numbers get `+0xc = 7`.

Consequence: KEY code 6 cannot set internal type 5. A new object read with `OBJTYP n 6` keeps
internal type 0 and is written back as code 1; only an object that already had internal type 5
(created in the GUI or by another path) keeps it.

## Read errors and validation

* EOF or blank rest of line (`FUN_140413ee0` returns 1) → **2** directly (nothing stored).
* Otherwise 0. No validation of the codes.

## KEY writer

`FUN_1402a25a0(model, f, obj, objNo)`, the first per-object call in `FUN_14029e250`, right after
the object comment block (`"*\n*  %s\n*\n"` with `"Data for Object #     %i"`):

    fprintf("%-6s %7i %7i %7i\n", "OBJTYP", objNo, code, obj+0x2154)

with `code` = internal 0→1, 1→2, 2→3, 3→4, 4→5, 5→6, 6→7, 7→**0**. Any other internal value →
return 3 without output; `FUN_14029e250` then resets `+0x3f24` and returns 3 (rest of the object
not written).

What `FUN_14029e250` does next depends on `obj+0xc` (after OBJTYP succeeded):

* **7 (empty)**: resets `+0x3f24` and returns 0 — the object consists of the comment block and
  the OBJTYP line only (`OBJTYP n 0 aux`). No OBJNAM.
* **5**: writes only the movement block `FUN_1402a7c80` (PRSNAM, MOVCTL, SPDLMT, PRSTRE, STROKE,
  FORCE, ANGMOV, ... OBJSTP; see MOVCTL.md), then `FUN_1402a9180` if byte `+0x2e05 == 0`, then
  `FUN_1402ab880` if `model+0x1744 & 0x1000000`; returns 0. No OBJNAM, AVGSTR, ... .
* **0–4, 6**: full object: OBJNAM (`FUN_1402a2530`), AVGSTR, LMTSTR, [3D: PENVOL inline /
  2D: ZSTR, PENVOL], USRSUB (id 253) if `+0x2150 != 0`, REFTMP, TMPLMT, ... (see
  `group2` writer order).

So the empty and type-5 objects are cut short by this control flow, not by per-keyword
conditions.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 1 — reader; `FUN_140413ee0` `"%i %i"` line reader.
* `FUN_140161260` — empty an object (type 7).
* `FUN_14019ccc0` / `FUN_14017a880` / `FUN_14015ec80` / `FUN_14015f0b0` — get-or-create object,
  constructor, default init (`+0xc = 0`).
* `FUN_1402a25a0` — writer; `FUN_14029e250` — per-object writer.

## Corpus check

550 files, 1385 records, all `OBJTYP n code aux` (4 tokens; 14 records in old 3D files (Cogging_Example_Old) have only 3 tokens,
`aux` then defaults to 0). Codes seen: 0..6 (2D: 0 ×15, 1 ×317, 2 ×141, 3 ×2, 4 ×80, 5 ×58,
6 ×252; 3D: 0..6). Example (CREEP_Example1_EN.KEY:131):

    OBJTYP       1       5       0

## Open questions

* Names of the internal types (strings not traced). KEY code 6 being ignored on read looks
  intentional or a bug; a GUI string table for object types would tell what internal 5 is.
* Meaning of `aux` (`+0x2154`).
