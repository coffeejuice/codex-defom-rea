# P17: cross-class helpers (units, DB paths, *!SIMTYP, DOE path)

Binary `DEF_GUI_MO_64.exe` (copy), image base 0x140000000. Decompiles: `gui_mo/s1/p17/a1.c` … `a12.c`
on Sim2 (Ghidra copy `gproj_p17`). Port: `apps/moproj2mst/src/model/units.cpp`, `src/model/paths.cpp`,
`src/mst/doe.cpp`, declarations in `include/m2m/p17.hpp` and the G0 seams. Answers s1-04 O-9/O-11,
s1-05 Q1/Q2, s1-01 Q2/Q8, s1-03 Q1.

## 1. Unit evaluator `1402ede00` and the unit table `0x143d76368`

`bool eval(table, int kind, int unitSys, QString text, double* out, QString* unit)`:
```
if (parse(table, kind, text, out, unit))                         // 1402edb70
    r = toSystem(table, kind, unitSys, *unit, *out, out);        // 1402ed750
    *unit = QString(unitName(kind, unitSys));                    // 140917de0
return r;                                                        // false when the parse fails
```
* `parse` (`1402edb70`): `*out = 0`. Kind not in the table: only kind 0x2b parses, `toDouble(text)`.
  Otherwise `parts = text.split(" ", KeepEmptyParts)`, then `removeAll("")` (`1400acf20`). One part:
  `toDouble(parts[0])`, the unit stays as passed. Two or more: for each unit `u[i]` of the category,
  if `u[i] == parts[1]` (case-sensitive): `*out = toDouble(parts[0])`, `*unit = parts[1]`, stop when
  the number is valid. Extra parts are ignored.
* `toSystem` (`1402ed750`): `target = unitName(kind, unitSys)`; empty → `*out = v`, true only for kind
  0x2b; else `convert(kind, unit, v, target, out)`.
* `convert` (`1402ed5c0`): `i = index(unit)`, `j = index(target)` (`1402ed330`, `QString::compare`
  case-insensitive, -1 when absent); both found → `*out = conv[i][j](v)`, true; else out untouched.
  `conv(v)` (`1402ee890`) = `flag ? (v + offset) * factor : v * factor + offset`.
* `unitName` (`140917de0`): `0x1427bc000 + kind*0x28` (SI) when `unitSys == 1`, else `+0x14` (English).
* Table: `QMap<int, Category*>` built at start-up by `1402eaaf0` (static init `141ca0c60`), one
  `1402ea840(map, kind, name, label, n, units, ints, factor, offset, flag)` per kind 0..0x2a; entry
  `conv[i][j]` = arrays at index `j*n + i`, defaults (`1402ee5e0`) 1.0 / 0 / 0. Nothing else writes the
  arrays (all in initialised .data; the int lists in .bss only give the count). Kind 0x1f's second unit
  pointer is null (null QString). Generated into `src/model/units_table.inc` by `tools/gen_units.py`.
* Data facts the port keeps: SI name of 0x13 is `J/sec/mm^2/C^4)` (stray `)`, so SI conversion of that
  kind always fails); F→K factor is negative (-5/9 with offset 255.37…) as stored.
* Callers: `14060c4e0` (MultiBlow, kind 0x20, ignores the result), `1405fb980` (kind 0, uses the value
  only on success) and the FormingExpress/HeatTreat property readers (P14, P15).

## 2. DB paths
* `1400a4f60(P, out, full)` = `140818d50(full ? P+0x18 : completeBaseName(P+0x18), ".DB")`.
* `1400b9e80(sub, out, withDir)`: `sub+0x18 == 4` → `1400bbee0`; else `1400b9df0` =
  `sub->project ? 1400a4f60(project, withDir) : null`.
* `1400bbee0(sub, withDir)`: if `sub+0x38` (DbName) is empty it is set to `1400bc290(sub)`: the first
  `"DieStress" + sprintf("%05d.DB", i)`, i = 1, 2, …, that no sub-project's DbName equals.
  Result `cleanPath(withDir ? 1400bc160(sub) + sep + name : name)`, `1400bc160` =
  `cleanPath(projectDir + sep + "DieStress")`.

## 3. `*!SIMTYP` and the equipment path
* `140050d00` returns a copy of OPI+0x120. The only setter is `140050c80` (called by the factory with
  the registry key after the legacy mapping). So the code prints the mapped name (e.g. AirConvect2D →
  `SfOprHeatTransferExpress2D`); 2013 MSTs with legacy names come from an older build (s1-03 Q1).
* Equipment path (`1400a5970(P)->vt+0x208`): `1400a5970` = `P+8 ? 140044e60(P+8)` (equipment manager
  at M+0x19e0, created on demand) `: null`; `vt+0x208` = `14006d380` for every manager:
  `item = vt+0x1c8(id)` (`14006cea0`, first row whose id matches) → `cleanPath(item+0x28)` or null.
  `14006d380` is P08's (`itemKeyPathClean`).

## 4. GUI unit global `0x143d79724` (s1-01 Q2)
.bss, no static initialiser, no registry/ini/command-line read. Writers: `14001c110` (New Project:
`*param_6 == 2 ? 2 : 1`) and `140127960` (`SfProjectNew::slotOkClicked`: English radio ? 2 : 1).
Readers include the project ctor `140089880` (`1400a2150(P, global)` before `<Unit>` is read). A fresh
GUI process therefore has 0 until the user creates a new project. `guiStartupUnit()` returns 0.

## 5. DOE `.MST.TEMP` path (s1-05 7, s1-02 2.4/3.3/4.4)
* `140131530`, `140131a50`, `1401321a0` as in s1-05 7.2/7.3 (literals checked against the exe: five
  75-byte banner lines `1421ac5d0..1421ac710`, `"*!SUBPRJ "`, `"*!PRJNAM "`, `"*"` `1421a8e8c`, rule `1421ac780`).
* `1400b5c80(sub)` → `1400a77b0(project, sub+0x1c)`: id 0 → 0; else the first index k ≥ 1 with that id; -1.
* `1400ba000`, `1400ba350`, `1400a51e0` as in s1-05 7.1; `1400ab770` = copy of doe+0x98.
* The DOE model queries `14091bbb0`/`14091bdb0` are P09's. No side file is written.
