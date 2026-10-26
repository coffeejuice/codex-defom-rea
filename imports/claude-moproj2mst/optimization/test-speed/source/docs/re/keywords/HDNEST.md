# HDNEST (id 218, group 2, case 0x4a)

Per-object hardness-estimation settings (by name): an on/off/mode int, a second int and two
doubles stored in the object at `obj+0x1f44..0x1f58`. Written on one line for every object.

## KEY grammar (read)

```
HDNEST <objNo:%i> <mode:%i> <i2:%i> <d1:%Flf> <d2:%Flf>
```

All on the keyword line: `FUN_140415ff0` (`fgets`, commas become spaces,
`sscanf("%i %i %Flf %Flf")`; missing trailing values are 0, a non-numeric token ends the
conversion with the rest 0, extra text ignored). Same in 2D and 3D.

## Model storage

| Field | Offset | Type |
|---|---|---|
| `mode` | `obj+0x1f48` | int — **always** stored (also when 0) |
| `i2` | `obj+0x1f44` | int — stored only if `mode != 0` |
| `d1` | `obj+0x1f50` | double — stored only if `mode != 0` |
| `d2` | `obj+0x1f58` | double — stored only if `mode != 0` |

With `mode == 0` the other three keep their previous values (object constructor values or an
earlier `HDNEST`).

## Read errors and validation

* object number EOF / `< 1` → **2** (a missing object is created);
* `FUN_140415ff0` returns 1 (EOF, or nothing but whitespace after the object number) → **2**;
  note the parser stores `mode` (0, as zeroed by the helper) before checking the status, so a
  failed read still sets `obj+0x1f48 = 0`;
* no value checks, no messages.

## KEY writer

Registry handler2 `FUN_1402b4c40`, called by `FUN_14029e250` at writer-map step 43 (after `DATOM`,
before `HDNOBJ`), status checked (always 0). One line, always (no condition):

```
"%-6s %7i %7i %7i %20.10E %20.10E\n"   name, N, obj+0x1f48, obj+0x1f44, obj+0x1f50, obj+0x1f58
```

`N` = `obj+0x3f24` if > 0, else `obj+8`. Round trip: exact when `mode != 0`; with `mode == 0` the
written `i2 d1 d2` are the stored (old) values, not those of the file.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x4a (p_main.c lines 2291–2300).
* `FUN_140415ff0`: `"%i %i %Flf %Flf"` rest-of-line reader (group2-helpers.md).
* `FUN_1402b4c40`: handler2 (writer); handler3 is the no-op `0x1402f7610`.

## Corpus check

A scripted pass over every corpus KEY file (record counts, values per record, next line is a keyword) accepted all 1082 occurrences of HDNEST.

`C:/Users/alext/Projects/DEFORM_corpus/key/2D/EN/MultipleOperations/New_MO_Release_Examples_EN/MO2_Extrusion_and_Boolean_Cycles/Objects/Object00007.KEY`
and `.../plane stress/Ela_PlaneStress_EN.KEY`, `key/3D/EN/NAFEMS_Benchmarks/NAFEMS_T2.KEY`: parse
exactly; all corpus examples seen have `mode 0`:

```
HDNEST       4       0       0    0.0000000000E+000    0.0000000000E+000
```

## Open questions

* Constructor defaults of `obj+0x1f44..0x1f58` (what a `mode 0` object writes) not traced
  (object constructor; corpus shows zeros).
* Meaning of `mode` / `i2` values not evidenced in the code read here.
