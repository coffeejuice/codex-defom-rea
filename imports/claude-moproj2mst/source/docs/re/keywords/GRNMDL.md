# GRNMDL (id 109, group 4 material)

Grain model selector (inferred from the name). Read and discarded by this version.

## Handlers
read: case 0x11 of `FUN_140384a50` -> `FUN_140413de0(fp, &local_c4, 0)` (a local int, discarded);
write: not written (registry handler2 is the stub `0x1402f75d0`; `FUN_1403cdca0` has no inline
code for it, and no `FUN_1402f7720(0x6d)` call exists in the decompiled writers);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: GRNMDL <mat:%i> <v:%i>   rest of line: fgets(0x400), "," -> " ", sscanf "%i"
                                  into a local that is never used
```

## Model mapping
Nothing stored. Side effect only: the material is created if `mat` is unknown.

## Defaults
n/a (nothing stored).

## Validation, errors, return codes
* Rest of the keyword line blank (whitespace only) or EOF -> 2
  (`ERROR: Keyword GRNMDL Reading Error. \n`).
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Non-numeric text is accepted silently. Never returns 3.

## Unit conversion
none

## KEY grammar (write)
Not written. A KEY read and written back by DEF_PRE loses this keyword (confirmed: the oracle
output of `key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY`, which contains it, has no
GRNMDL line).

## Helpers
- `0x140384a50` - group 4 parser
- `0x140413de0` - one-int line reader

## Corpus check
3 instances in 1 file (`key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY` line 55, ...), all `GRNMDL m 0`; consumed exactly.

## Open questions
none
