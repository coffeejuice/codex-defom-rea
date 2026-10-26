# Operation loader and save-current-operation: packet plan

Scope (coordinator, 2026-10-22): the lazy loader `0x140051340` / OB vt+0x58 `0x1405ef430` for every
operation class, and the cogging geometry step. Analysis: `loader-spec.md`. The engine is the
DEF_PRE port (`port/`), linked into the converter since `5593de66`; `tools/fmatch.py` maps GUI
functions to DEF_PRE twins.

## Class slots on the load and save paths

25 concrete operation classes override some of OB vt+0x58 (load), vt+0xb8, vt+0x78, vt+0x1b8,
vt+0xc0 (load slots) and vt+0x68, vt+0xd0, vt+0xd8, vt+0xe8, vt+0x80 (save-current-operation
slots); `SfOprBase` gives the defaults (`0x1405ef430`, `0x1405f1bb0`, `0x1405f6520`, `0x1405f1c00`,
`0x1405ef510`, `0x1405f1e20`, `0x1405f22e0`, `0x1405f2350`). Table: `vtables.tsv` filtered to
`SfOpr*` (generated in the MDT thread; regenerate with the snippet in the thread notes).

## Packets

| Id | Content | Depends on |
|---|---|---|
| L0 | GUI model wrapper (`0x140c7a5b0`, 0x278 bytes: DEF_PRE `DB_STEP_DATA`, sim control, viewer list) and the `0x140c7xxxx` methods the loader calls; OB vt+0x58 skeleton; `0x140051340`; mode-0 decision `0x140055360` with the dialog policy; KEY path `0x1400569d0` (materials `0x1408959e0`, objects `0x14005aec0` -> `0x14005b030` / `0x14005b2f0` / `0x14005cc00`, equipment `0x1403568a0`); previous-operation chain vt+0x2a0 (`0x1405f5b70`, CopyMirror `0x140e25980`); primitive table `0x140c7ebe0` | engine link |
| L1 | Cogging load slots (`0x140750650`, `0x1407507b0` with `0x140765dd0`, `0x140766740`, DEF_CNT.DAT) and save slots (`0x14075c260` -> `0x140762ec0`: `0x140766200`, `0x140763e50`, `0x140762f30`) | L0, V |
| L2 | HeatTransfer family, HTFurnace, Pre | L0 |
| L3 | Forming, Forming2D, Express family | L0 |
| L4 | MultiBlow family | L0 |
| L5 | Boolean, Convert2Dto3D, CopyMirror | L0 |
| L6 | HeatTreat, Distortion, Diestress, DiestressBM family | L0 |
| L7 | Cycle, Group, DoeOutput, Report, ShapeOptim | L0 |
| V | 3D view box (`0x1411f5540`, display settings defaults, QVector3D float boxes): only if Aleksandr chooses the faithful option | L0 |
| S | generic save-current-operation `0x140052ec0` (OPI sync `0x140057370`, props vt+0x60, `0x140054540`) and the engine KEY writer of OB vt+0x68 | scope thread |

Open decisions for Aleksandr: dialog policies (setup type, missing restart file, counter-blow
hammer) and the depth of V.
