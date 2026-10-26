# OTPRNG (id 240, group 2, case 0x60)

Per-object output range (output-printing control). It holds a mode, two doubles and an integer,
stored in the `obj+0x1ea0` sub-structure. A negative mode is kept as "mode 3 with count |mode|".
The registry flag is 5.

## KEY grammar (read)
    OTPRNG <objNo:%i> <mode> <d1> <d2> [<k>]
The rest of the keyword line is read by the tokenizer `FUN_140413900(f, &n, 6)` (group2-helpers.md,
"Line tokenizer"). Separators are blank, comma, tab and CR. The tokens are converted like this:
* t0 → `strtol` base 10;
* t1, t2 → `atof`;
* t3 → `strtol`, but only if the token count `n >= 4`.

Missing tokens convert to 0. Hex and octal are not recognised (`strtol` base 10, unlike `%i`).
There is one line; 2D and 3D are identical. A 5-field line (no `k`) is accepted.

## Model storage
* `obj+0x1ef8` (double) = d1; `obj+0x1f00` (double) = d2.
* `obj+0x1f08` (int) = k when `n >= 4`, else 0.
* mode < 0: `obj+0x1eec = 3`, `obj+0x1ef0 = |mode|` (`FUN_140484ab8` = abs).
* mode >= 0: `obj+0x1eec = mode`. `obj+0x1ef0` is untouched.
* Defaults (`FUN_140186200`): all 0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Tokenizer EOF → **return 2 immediately**, before anything is stored.
* An empty rest of line is not an error (count 1, slot 0 NULL): every field gets 0.

## KEY writer
Registry handler2 `FUN_1402af770`, called from `FUN_14029e250` (group2-writer-map.md step 17) after
`ELPSOL`, for objects of a type other than 5 or 7. If `model+0x1744 & 0x20000` it writes nothing.
Otherwise it calls `FUN_1402abd50(model, f, obj, N)`:

    m = (obj+0x1eec == 3) ? -(obj+0x1ef0) : obj+0x1eec;
    fprintf(f, "%-6s %7i %7i %20.10E %20.10E %7i\n", "OTPRNG", N, m, d1, d2, k);

Both functions return 0. (`FUN_1402abd50` writes nothing if `f` or `obj` is NULL.)

Round trip: a negative mode is kept. Reading mode **3** directly stores `+0x1eec = 3` without
touching `+0x1ef0`, so the writer prints `-(old +0x1ef0)`, which is `-0 = 0` by default (lossy).
A 5-field input is written back with an explicit `k = 0`.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x60: reader.
* `FUN_140413900` / `FUN_140413a50`: tokenizer and token access.
* `FUN_14047b6c0` (`strtol`) and `FUN_14048754c` (`atof`): token conversions.
* `FUN_140484ab8`: abs.
* `FUN_1402af770`: registry handler2.
* `FUN_1402abd50`: line writer.
* `FUN_140186200`: reset.

## Corpus check
* `2D/EN/HeatTreat/MRX_SRX_2D_EN.KEY`:
  `OTPRNG       3       0    0.0000000000E+000    0.0000000000E+000       0` (6 fields, current writer).
* `2D/EN/Creep/CREEP_Example1_EN.KEY`:
  `OTPRNG       1       0    0.0000000000E+000    0.0000000000E+000` (5 fields, older writer, k → 0).

Both parse. Of 1160 corpus lines, 798 have 5 fields and 362 have 6; all have mode 0. Many files
lack OTPRNG altogether (written with flag 0x20000).

## Open questions
* What modes 1–3, d1, d2 and k mean.
* Which KEY-write caller sets flag 0x20000.
