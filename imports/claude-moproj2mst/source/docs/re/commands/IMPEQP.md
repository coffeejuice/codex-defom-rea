# IMPEQP — import an equipment (press) KEY file

Shared mechanics: [common.md](common.md). Decompile: `deform-rea-work/out/p89/impeqp.c`.

## 1. Summary

| | |
|---|---|
| Registry id | 17 |
| Dispatcher | `FUN_140378e00` case 0x11 (assembly checked at 0x14037d8fd) |
| Handler | `FUN_1401ac680(model, filename, &id)` → `FUN_1401b82b0(model, filename, id)` |
| External program | none (in-process) |

Runs the KEY interpreter on the named file **directly on the live model**, then makes sure object
`id` exists (find or create). Used by Multiple-Operations master files (`.MST`), e.g.
`v11.0.2\2D\LABS\BEND_2D\BEND_2D.MST`:

```
IMPEQP    	2
Equipments/Equipment00008.KEY
```

## 2. Argument grammar

```
IMPEQP <id>            rest of keyword line, FUN_140413de0(f, &id, 0): "%i"
<KEY file name>        fgets(1024) + FUN_140323c50 (trailing ' ', '\r', '\n' removed)
```

The `%i` reader status is not stored (an empty line gives id = 0). File-name line at EOF → 2.
Otherwise the case returns 0 (handler result ignored).

## 3. Behaviour

`FUN_1401b82b0(model, name, id)`:
1. `FUN_1401b5f00(model, name, &0, &0)`: no pre/post fix-ups (flag 0); `strcpy(model+0x2f8a, name)`;
   `fopen(name, "r")` (failure → `"ERROR: Keyword file %s open error!\n"`, status 1);
   `model+0x1740 = 0`; KEY interpreter `FUN_1401b6520(model, file)`; `model+0x1740 = 0`; fclose;
   `strcpy(model+0x2f8a, " ")`.
2. Status 0 → `FUN_14019ccc0(model, id)` (id < 1 → nothing; else find object id or append empty
   objects up to it, `model+0x1378` = count; see GFREAD §2). Status ≠ 0 → nothing.

The object number used by the file's keywords comes from the keywords themselves (`MOVCTL 2 …`,
`STROKE 2 …`), not from `id`; `id` only guarantees the slot exists.

Side effect: the interpreter honours the comment header. The first non-empty comment line whose
first word is `DEFORM` + `2D`/`3D`/`F2`/`F3`, or `DEFORM-2D`, `DEFORM-3D`, `DEFORM-F2`, `DEFORM-F3`,
`DEFORM-MACH2`, … calls the model's set-dimension virtual (`(*model->vtbl[0])(model, &dim)`) —
importing a file whose header names the other dimension switches the model (inferred risk; the
same happens for KFREAD).

## 4. File format

An ordinary KEY file. Install examples (read-only):
`v11.0.2\2D\LABS\BEND_2D\Equipments\Equipment00001.KEY … 00009.KEY` (2D, keywords `PRSNAM`,
`MOVCTL` (with a table), `STROKE`, `FORCE`, `ANGMOV`, `CNTRAX`, `OBJUPD`), and the 3D sets used by
`v11.0.2\3D\LABS\Gear_blank\Gear_blank.MST` / `Spike_Forging.MST`. The press library
`v11.0.2\PressLibrary\{Hammer,Mechanical,Screw Press}\*.DAT` is a different format, not read by
this command (no reference found; inferred GUI-only).

## 5–6. Launch / read-back

None.

## 7. Messages

`"ERROR: Keyword file %s open error!\n"` plus the KEY interpreter's own messages. Case return 0
except EOF on the file-name line (2). `FUN_1401ac680` always returns 1.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401ac680` | handler (wrapper) |
| `FUN_1401b82b0` | read KEY into model, then `FUN_14019ccc0` |
| `FUN_1401b5f00` | open + KEY interpreter |
| `FUN_14019ccc0` | find-or-create object |

## 9. Open questions

* Whether `PressLibrary\*.DAT` is ever read by DEF_PRE (not via IMPEQP).
