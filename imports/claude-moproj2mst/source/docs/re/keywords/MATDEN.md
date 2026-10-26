# MATDEN (id 124, group 4 material)

One double per material, written only for 3-D models. Meaning inferred: a material density
(distinct from MASDEN; always 0 in the corpus).

## Handlers
read: case 0x20 of `FUN_140384a50` -> `FUN_140414990(fp, material+0x1ee8)`;
write: inline in `FUN_1403cdca0` (registry handler2 is the stub `0x1402f75d0`);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: MATDEN <mat:%i> <v:%Flf>   rest of line: fgets(0x400), ',' -> ' ', sscanf "%Flf"
                                    (v zeroed first)
rest of line blank or EOF -> 1 -> parser returns 2 (v = 0.0)
```
Read in 2-D and 3-D models alike.

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| v | material | +0x1ee8 | double |

## Defaults
`FUN_14028cdc0` sets +0x1ee8 = 0.0.

## Validation, errors, return codes
* Rest of line blank or EOF -> 2 (`ERROR: Keyword MATDEN Reading Error. \n`), value 0.0.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
inline in FUN_1403cdca0, after PMITT, only if FUN_1400015f0(modelData) (modelData+0x1c == 3):
  "%-6s %7i %20.10E\n"   "MATDEN", mat, *(double*)(material+0x1ee8)
```
`modelData` is the first argument of `FUN_1403cdca0` (the object that holds the material list
at +0x1328). Asymmetry: a 2-D KEY containing MATDEN is read, but the value is dropped on write.

## Helpers
- `0x140414990` - one-double line reader
- `0x1400015f0` - "model is 3-D" test (`*(int*)(p+0x1c) == 3`)
- `0x1403cdca0` - per-material writer (inline)

## Corpus check
242 instances in 174 files, all `MATDEN m <double>` (all 0.0); consumed exactly. 173 of the
files are under `key/3D/`, the other is `materials/AISI-1020(Machining)_e000008.KEY`; none is a
2-D KEY, matching the 3-D-only write condition.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every MATDEN block of those files with the read grammar above and re-wrote it with the write formats above; all 26 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).
MATDEN appears in all oracle outputs of 3-D runs (the `materials/*` round trips and the
`variants/3D__*` files) and in none of the `variants/2D__*` outputs.

## Open questions
Meaning of the value (inferred density) and its consumer.
