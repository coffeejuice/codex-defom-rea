# LTHEAT (id 101, group 4 material)

Latent heat (inferred from the name). This version reads the line and throws the value away; the value is even read with `%i`, so a corpus value such as `.0000000E+00` does not convert (sscanf returns 0, no error).

## Handlers
read: case 9 of `FUN_140384a50` -> `FUN_140413de0(fp, &local_f0, 0)` (a local int, discarded);
write: not written (registry handler2 is the stub `0x1402f75d0`; `FUN_1403cdca0` has no inline
code for it, and no `FUN_1402f7720(0x65)` call exists in the decompiled writers);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: LTHEAT <mat:%i> <v:%i>   rest of line: fgets(0x400), "," -> " ", sscanf "%i"
                                  into a local that is never used
```

## Model mapping
Nothing stored. Side effect only: the material is created if `mat` is unknown.

## Defaults
n/a (nothing stored).

## Validation, errors, return codes
* Rest of the keyword line blank (whitespace only) or EOF -> 2
  (`ERROR: Keyword LTHEAT Reading Error. \n`).
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Non-numeric text is accepted silently. Never returns 3.

## Unit conversion
none

## KEY grammar (write)
Not written. A KEY read and written back by DEF_PRE loses this keyword (confirmed: the oracle
outputs of `key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY` and
`materials/JIS-S45C(HeatTreatment)_s000006.KEY`, which contain it, have no LTHEAT line).

## Helpers
- `0x140384a50` - group 4 parser
- `0x140413de0` - one-int line reader

## Corpus check
7 instances in 2 files (`key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY` lines 44, 116, 188, `materials/JIS-S45C(HeatTreatment)_s000006.KEY`), all `LTHEAT m <double>` on one line; consumed exactly (script check).

## Open questions
none (obsolete keyword; the latent heat now lives elsewhere, e.g. PHASLH - not verified).
