# SYMSUF (id 262, group 2, case 0x76)

Symmetry surfaces of a 3D object: a list of planes, each given as a point and a normal
(two 3-vectors), in the linked list `obj+0x2e80`.

## KEY grammar (read)

3D only (`FUN_1400015f0`). In 2D the case returns 0 **without reading anything**, so the two
data lines would then be parsed as keywords.

    SYMSUF <obj> <id:%i> <x:%i>            FUN_140413ee0; x is read and ignored (the writer prints 0)
    <p1:%Flf> <p2:%Flf> <p3:%Flf>          FUN_140414c20: first vector  -> entry+0x08,+0x10,+0x18
    <q1:%Flf> <q2:%Flf> <q3:%Flf>          FUN_140414c20: second vector -> entry+0x20,+0x28,+0x30

One line per vector. Commas are allowed, missing values are 0, extra tokens are ignored.

## Model storage

* List head `obj+0x2e80` (`+0` first, `+8` last, `+0x10` count). Entries are 0x40 bytes:
  `+0` id, `+8..+0x30` six doubles, `+0x38` next. `FUN_140257340` zero-inits them.
* `FUN_1402580f0(list, id)` finds the entry whose `+0` equals id. If there is none,
  `FUN_140257e90(list, id)` appends a new one with `+0 = id`. A repeated id overwrites the
  existing entry in place.
* The corpus pattern (point, then unit normal) suggests the meaning point/normal; the code does
  not name the fields.

## Read errors and validation

* Line 0 EOF/blank → **2**.
* The status of the first vector line is overwritten without a check: only the second line's
  EOF/blank → **2**. The entry is created before the vectors are read.
* No id range check (0 and negative ids are accepted).

## KEY writer

`FUN_1402a3880(model, f, obj, N)`, 3D only (group2-writer-map.md step 57′, written before
WPAXIS). For position p = 1..count it takes `FUN_140258090(list, p)` (the p-th entry in list
order) and, when that is non-NULL, writes:

    "%-6s %7i %7i %7i\n"                 SYMSUF, N, entry id, 0
    " %20.10E %20.10E %20.10E\n"         +0x08, +0x10, +0x18
    " %20.10E %20.10E %20.10E\n"         +0x20, +0x28, +0x30

It returns 0 (3 if `f`/`obj` NULL); the status is ignored. Entries are written in list (creation)
order with their stored ids, so the output round-trips.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140413ee0` | `%i %i` header |
| `FUN_140414c20` | 3-double line |
| `FUN_1402580f0` / `FUN_140257e90` / `FUN_140257340` | find by id / append / init entry |
| `FUN_140158e70` / `FUN_140258090` | count / entry by position |
| `FUN_1402a3880` | writer |

## Corpus check

`3D\EN\Forging\SquareRing.KEY` line 7058 parses:
`SYMSUF       1       1       0` / `  0.0000000E+000  0.0000000E+000  0.0000000E+000` /
`  1.0000000E+000  0.0000000E+000  0.0000000E+000`.
All 122 occurrences (48 files) are in 3D files.

## Open questions

* None.
