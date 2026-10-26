# OPSTOP (id 265, group 2, case 0x79)

Per-object list of operation-stop entries (`OPSTOP_DATA` records in an `OpStopLinkedList` at
`obj+0x20b8`; RTTI names). Each entry has an integer type; only type 1 carries a value (one double).

## KEY grammar (read)
    OPSTOP <objNo:%i> <idx:%i> <type:%i>        (rest of line, FUN_140413ee0, commas = spaces)
    [<value:%Flf>]                              (next line, only if idx != 0 and type == 1)

* `idx == 0`: clears the whole list (`type` ignored). The writer always emits `OPSTOP obj 0 0`
  first, so a written file replaces the list.
* `idx != 0`: entry `idx` is got-or-created; `type == 1` → the value is read from the **next
  line** with `FUN_140414990` (rest-of-line reader, so the value must be the first token of its
  own line; commas = spaces).
* A missing `type` reads as 0.

## Model storage
List (`SfLinkedList` layout, 0x20 bytes): `+0` vtable, `+8` head, `+0x10` tail, `+0x18` count.
Slot 1 of the `OpStopLinkedList` vtable (`FUN_14025aea0`) is the factory: `new` 0x20 bytes,
`FUN_14025ac10` constructor.

`OPSTOP_DATA` (0x20 bytes):

| offset | type | field |
|---|---|---|
| +0x00 | ptr | vtable (`OPSTOP_DATA`) |
| +0x08 | byte | active flag (base `SfLinkedData` init sets 1) |
| +0x0c | int | type (init 0) |
| +0x10 | ptr | next entry |
| +0x18 | double | value (init 0.0) |

Reader steps for `idx != 0`: `FUN_14025a990(list, idx)` appends default entries (type 0, value 0,
active 1) until `count >= idx` and returns entry `idx` (1-based walk; for `idx <= 1`, including
negative `idx`, the head, or NULL when the list is empty). Dynamic cast to `OPSTOP_DATA`; if NULL
nothing is stored and 0 is returned (the value line of a type-1 entry then stays in the stream).
Otherwise `+0x08 = 1`, `+0x0c = type`, and for type 1 `+0x18 = value`. Gap entries created by the
append loop keep type 0.

Vtable slots 2/3 (`FUN_14025ad20` / `FUN_14025ad80`) convert an entry from/to a double array
`{?, (double)type, value}` (value only for type 1); not used by the KEY path.

## Read errors and validation
* objNo EOF / < 1 → 2.
* `FUN_140413ee0` returns 1 (EOF or empty/blank rest of line) → 2, nothing changed.
* Type 1 and the value line is EOF or empty → 2 (the entry already has type 1 and value 0).
* No check of `type` (any int stored). Return-code effect: `_common.md` §1.

## KEY writer
`FUN_1402a4020(model, f, obj, objNo)`, called from `FUN_14029e250` for normal objects (type not 5
or 7) right after `FUN_1402a39e0` (3D: `FUN_1402a3880` + `FUN_1402a39e0`) and right before the
DYNARY writer `FUN_1402a41c0`; reached only when the preceding handler chain (ending with
`FUN_1402b4d40`) returned 0. Return value ignored by the caller. Not written for type-5 objects.

* `f` or `obj` NULL → return 3.
* `objNo == 0` → `obj+8` is printed.
* Always: `"%-6s %7i %7i %7i\n"` → `OPSTOP objNo 0 0`.
* For `i = 1 .. count` (count from `obj+0x1ea0+0x218` = `obj+0x20b8+0x18`): entry `i`; skipped
  if NULL; for a type-5 object it would also be skipped if inactive (`+8 == 0`), unreachable here.
  If the cast is non-NULL and `type == 1`:
  `"%-6s %7i %7i %7i\n"` objNo, `i`, 1, then `" %20.10E\n"` value.
* Return 0.

Round-trip: entries with type != 1 are not written; re-reading creates them as gap entries of
type 0 when a later type-1 entry exists, and drops trailing ones (the list is shorter).
Positions of type-1 entries are preserved.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x79 — reader; `FUN_140413ee0` (`%i %i`), `FUN_140414990` (`%Flf`).
* `FUN_14025ab20` — clear list (virtual delete of each entry; head/tail/count = 0).
* `FUN_14025a990` — get-or-create n-th; `FUN_14025a7d0` — append via factory; `FUN_14025a9e0`
  — n-th entry; `FUN_14019a580` — count (`+0x18`).
* `FUN_14025ade0` — dynamic cast to `OPSTOP_DATA`; `FUN_14025a640` / `FUN_14025a660` — set/get
  `+8`; `FUN_14025ac10`, `FUN_14025ac90`, `FUN_14025a580`, `FUN_14025a5e0` — constructors/init.
* `FUN_1402a4020` — writer; `FUN_14019a560` — count via `obj+0x1ea0`; `FUN_140161020` — type == 5.

## Corpus check
`key\2D\EN\InductionHardening\2d_Induction_Hardening_Process_EN.KEY`: `OPSTOP       1       0       0`
(also objects 2, 3), `key\3D\EN\Rolling\3D_Ring_Rolling_Example4\3D_Ring_Rolling_Example4_EN_Axial.KEY`
objects 1–5, `key\2D\EN\HeatTreat\MRX_SRX_2D_EN.KEY`. All corpus lines are the clear line only;
they parse. No type-1 entry in the corpus.

## Open questions
* Meaning of the entry types (only type 1 = value is evidenced); find the consumer of
  `obj+0x20b8` (operation-stop criteria UI / simulation).
