# REFPOS (id 215, group 2, case 0x47)

Per-object reference position. Mode 1 gives explicit coordinates (x, y, z). Mode 2 gives an
integer (judging by the corpus value `240`, probably a node number). Mode 0 means none. The values
live in the `obj+0x1ea0` sub-structure.

## KEY grammar (read)
    REFPOS <objNo:%i> <mode:%i> <a:%Flf> <b:%Flf> <c:%Flf>
The rest of the keyword line is read by `FUN_140415880` (`"%i %Flf %Flf %Flf"`, commas → blanks,
missing values → 0). It is one line, the same in 2D and 3D: three coordinates are read in 2D too.
* mode 1: `a b c` = coordinates.
* mode 2: `a` = the integer value. It is read as a double and truncated (`(int)a`), so `240`,
  `240.0` and `240.9` all give 240. `b` and `c` are ignored.

## Model storage
* `obj+0x1ec8` (int) = mode. It is stored as read, for any value.
* mode 1: `obj+0x1ed0`, `+0x1ed8`, `+0x1ee0` (doubles) = a, b, c. `obj+0x1ee8` is untouched.
* mode 2: `obj+0x1ee8` (int) = `(int)a`. The coordinates are untouched.
* any other mode (0, 3, negative …): `obj+0x1ee8 = 1`. The coordinates are untouched.
* Defaults (`FUN_140186200`): mode 0, coordinates 0.0, `+0x1ee8 = 0`.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* `FUN_140415880` returns 1 (EOF, or the rest of the line is blank) → 2. The mode was zeroed by the
  helper, so the mode-0 branch has run (`+0x1ee8 = 1`).
* No range check on the mode, but see the writer: any mode other than 0, 1 or 2 makes the
  **writer** fail.

## KEY writer
Inline in `FUN_14029e250` (group2-writer-map.md step 14), after `ROTSYM` (3D) and `TMPLMT`, before
`TRGVOL`. Objects of type 5 and 7 never get it.
* `obj+0x1ec8 == 0`: nothing is written.
* mode 1: `fprintf("%-6s %7i %7i %20.10E %20.10E %20.10E\n", "REFPOS", N, 1, x, y, z)`. The three
  coordinates are written in 2D as well.
* mode 2: `fprintf("%-6s %7i %7i %7i\n", "REFPOS", N, 2, *(int*)(obj+0x1ee8))`.
* any other mode: `FUN_14029e250` returns **3** at once. The rest of the object block is not written,
  and the caller aborts the KEY write. Reading `REFPOS n 3` (or a negative mode) is therefore
  accepted by the reader but cannot be written back.

Modes 0, 1 and 2 round-trip exactly (mode 2 keeps the integer). A mode-0 line that has values is
dropped.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x47: reader.
* `FUN_140415880`: `%i %Flf %Flf %Flf` line reader.
* `FUN_14029e250`: writer (inline).
* `FUN_140186200`: `obj+0x1ea0` sub-structure reset (defaults).

## Corpus check
* `3D/EN/Cogging/Cogging_Multiple_Ops_Lab1/Objects/Object00001.KEY`:
  `REFPOS       1       1   -1.0000000000E+002    0.0000000000E+000    0.0000000000E+000`.
* `2D/EN/HeatTreat/MRX_SRX_2D_EN.KEY`: `REFPOS       3       2     240`.

Both parse exactly. Across the corpus there are 54 lines: 45 with mode 1 and 9 with mode 2. No other
modes occur.

## Open questions
* The meaning of the mode-2 integer (node number or object number?) is not visible here. Its use
  in the solver would settle it.
