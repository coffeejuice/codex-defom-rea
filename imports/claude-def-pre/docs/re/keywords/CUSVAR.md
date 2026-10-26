# CUSVAR (id 198, group 2, case 0x36)

Custom (user-named) state variable of the object: a `StateVarCustom` entry in the object's
state-variable list `obj+0xb58`, identified by an integer id, holding an `nr × m` double table
attached to nodes, elements or a free number of rows.

## KEY grammar (read)

    CUSVAR <objNo> <varId:%i> <type:%i> <count:%i> <default:%Flf> <m:%i>   rest of line, commas allowed
    count × { <row:%i> <m × value:%Flf> }

Header by `FUN_140416700`: `fgets`, commas → spaces, `sscanf("%i %i %i %Flf %i")`, all outputs 0
first, partial lines accepted; `sscanf == -1` (blank) or EOF → 1. Records by
`FUN_1404199a0(f, data, count, nr, m)` (free layout, final `fgets`).

`type`: 0 = free table with `nr = count` rows, 1 = per node, 2 = per element.

## Model storage

1. `var = FUN_140185d20(obj+0xb58, varId, type, m, count)`: finds the custom variable with this
   id (`FUN_140185e00`, RTTI cast to `StateVarCustom`) or creates it (`FUN_140186010`); sets
   `var+0xc = type`; rows `var+0x10` = `count` (type 0), the owning object's `+0x880` nodes
   (type 1) or `+0x884` elements (type 2), other types keep the previous row count; the previous
   row count is saved in `var+0x1c`. `var+0x14 = m` (previous saved in `var+0x20`). Then
   `FUN_140301380(var, 0)` reallocates the data vector `var+0x28` to `rows × m × (third
   dimension)` when the size changes, **without** keeping old values.
2. `FUN_140301620(var, default)`: every data value = default.
3. Records: `data[(row-1)*m + j]` for `1 ≤ row ≤ nr` (`nr = var+0x10`); others read and dropped.
4. On success: `*FUN_14019a4e0(model) = varId` (`FUN_14032f630` stores the id at offset 0 of the
   object returned for the model; its role was not traced).

## Read errors and validation

* Prologue → **2**.
* `FUN_140416700` nonzero → **2** (no variable created).
* `FUN_1404199a0` nonzero (EOF in a record / final `fgets`) → **2**; the variable keeps what was
  read. Else **0**. No validation of `type`, `m` or `count`.

## KEY writer

`FUN_14029e250` step 61 (writer map): for each entry of `obj+0xb58` (`FUN_14032a140` count,
`FUN_140185f80(list, i)`; NULL entries skipped) the handler `FUN_1402b7310` is called with
ref = (object, `FUN_14032ec90(entry)` = id); it calls `FUN_1402a4a80(model, f, obj, 0x4e, N, id)`
→ `FUN_1402f6e40(f, "CUSVAR", N, id, obj, 0.0)`. The registry handler2 of CUSVAR is the no-op.

* Variable not found (no `StateVarCustom` with that id) → returns 1, nothing written (status
  ignored).
* Header `"%-6s %7i %7i %7i %7i %20.10E %7i\n"` = name, N, varId, type, count, default, m.
* `type == 0`: count = `nr`, default 0.0, **every** row written: `" %7i"` + m × `" %20.10E"`
  (5 per line, continuation lines with 8 spaces).
* other types: count = rows with a value `!= 0.0` (all values equal → count 0), and rows with a
  nonzero value are written. The default field is always 0.0, **even when all values equal a
  nonzero constant**: then the line says `count 0, default 0.0` and the variable reads back as
  all zeros (writer bug, data loss).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x36; `FUN_140416700` (header), `FUN_1404199a0` (`group2-helpers.md`).
* `FUN_140185d20` (get/create + size), `FUN_140185e00` (find by id), `FUN_140185ed0`,
  `FUN_140300ec0` (set type), `FUN_140300f30` (set rows), `FUN_140300f90` (set m),
  `FUN_140301380` (resize), `FUN_140301620` (fill), `FUN_140301050` (data pointer),
  `FUN_140300f00`/`FUN_140300f60`/`FUN_140300ee0` (rows / m / type getters), `FUN_14032f630`.
* `FUN_1402b7310`, `FUN_1402a4a80` case 0x4e, `FUN_1402f6e40`, `FUN_1402f5360`, `FUN_1402f2e80`
  (`group2-statevar-writers.md`).

## Corpus check

No corpus file uses CUSVAR.

## Open questions

* `FUN_140301380` sizes by a third factor `FUN_140300fc0(var, 0)` that the reader never sets;
  presumably 1. Not traced.
* What `FUN_14019a4e0(model)` returns (where the last read custom variable id is stored).
