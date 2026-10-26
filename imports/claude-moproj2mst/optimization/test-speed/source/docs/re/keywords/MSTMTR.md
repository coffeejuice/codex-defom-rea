# MSTMTR (id 116, group 4 material)

List of material numbers attached to this material (inferred: the "master material" of a
mixture, listing its constituent / phase materials; e.g. `MSTMTR 1 3` followed by 2, 3, 4 in the
heat-treatment examples where material 1 is the mixture and 2..4 its phases).

## Handlers
read: case 0x18 of `FUN_140384a50` (inline code);
write: inline in `FUN_1403cdca0` (registry handler2 is the stub `0x1402f75d0`);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
oldCount = material[+0x1ccc]
line 1: MSTMTR <mat:%i> <n:%i>     rest of line: FUN_140413de0 (fgets, ',' -> ' ', sscanf "%i")
                                    straight into material+0x1ccc (zeroed first)
        blank/EOF -> return 2 (count is then 0; nothing else changes)
if n >= 1:
    flag(+0x1cc8) = 1
    free(old list +0x1cd0); list = calloc(n, 4)
    repeat n times: fscanf("%Flf", &d)  (whitespace-separated tokens, any line layout)
                    EOF -> return 2 (list partly filled, rest 0)
                    list[i] = (int)d
    NO trailing fgets: the stream stays right after the last number (the interpreter's next
    "%39s" skips the newline).
if n <= 0:
    if flag == 1 && list != NULL && oldCount > 0:
        for each of the oldCount old entries e: if material e exists (FUN_14029c2c0),
            set *(int*)(thatMaterial + 0x1cc8) = 0
        flag = 0
    the old list memory is NOT freed (pointer kept; count is now n).
```
* A failed conversion inside the list (non-numeric token) is not detected; `d` keeps its previous
  value (uninitialised on the first entry) and the stream does not advance.
* Values are read as doubles and truncated, so `2.9` gives 2.

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| n | material | +0x1ccc | int (may be <= 0) |
| entries | `int[n]` at material+0x1cd0 | +0x1cd0 (pointer) | int* (calloc) |
| (side) | material | +0x1cc8 | int flag: 1 after a list with n >= 1 |
| (side) | each previously listed material | +0x1cc8 | cleared when this material's list is removed (n <= 0) |

The reader never sets the listed materials' own +0x1cc8; it only clears them.

## Defaults
`FUN_14028cdc0` sets +0x1cc8 = 0, +0x1ccc = 0, +0x1cd0 = NULL.

## Validation, errors, return codes
* Count line blank or EOF -> 2 (`ERROR: Keyword MSTMTR Reading Error. \n`).
* EOF inside the list -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Material numbers in the list are not checked (they need not exist). Never returns 3.

## Unit conversion
none

## KEY grammar (write)
```
inline in FUN_1403cdca0, after JOMINY:
  "%-6s %7i %7i\n"   "MSTMTR", mat, n(+0x1ccc)
  for i in 0 .. n-1:  " %7i\n"   list[i]          (one entry per line)
always written (n <= 0 -> header only, with the negative value printed as is)
```
Round trip: count and entries round-trip; the flag +0x1cc8 is not written, it is re-derived
(set to 1 when n >= 1). Clearing of other materials' flags happens only on read with n <= 0.

## Helpers
- `0x140413de0` - one-int line reader
- `0x14029c2c0` - find material by number
- `0x1403cdca0` - per-material writer (inline)

## Corpus check
740 instances in 440 files: `MSTMTR m 0` (716), `MSTMTR m 3` + 3 lines (16, e.g.
`key/2D/EN/HeatTreat/HT_CARB1_EN.KEY` line 147: 2, 3, 4), `MSTMTR m 7` + 7 lines (8, e.g.
`key/2D/EN/ResistanceSpotWelding/Resistance_Spot_Welding_elstoplastic_model_with_phase_transformation_Cooling_off_cycle_EN.KEY`
line 214: 4..8 ...); all consumed exactly (script check).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every MSTMTR block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning and other users of the flag at +0x1cc8 (only cleared here). The memory leak / stale
pointer on n <= 0 is harmless for KEY I/O.
