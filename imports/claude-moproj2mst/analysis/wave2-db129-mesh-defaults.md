# GUI mesh weights

The B129 mesh-weight difference is a GUI engine initializer difference, rather than a normalization or selected-editor effect. The original GUI initializes strain and strain-rate weights to `0.25` and curvature weight to `0.5` in both dimensions. DEF_PRE initializes the same values, then overrides them to `1` for a 2D owner.

The [exact original static receipt](wave2-db129-mesh-static-evidence.json) records both binaries, `.pdata` function ranges, code hashes, disassemblies, and directly decoded constants. GUI `0x14085d9b0..0x14085dc92` stores `0x1421d3d10 = 0.25` at mesh block `+0x20/+0x28`, and `0x142101640 = 0.5` at `+0x38`. It enters window initialization immediately afterward, without a 2D weight override. Dimension checks later select MGSIZR (`0x14085daae`), the third 3D element-count field, and RMDPTH; those remain intact.

The shared [initializer](../source/port/defmodel/src/llobj_ctor/obj_defaults.cpp) now guards only DEF_PRE's three 2D weight assignments with `!g_gui_engine_variant`. The existing inline variant flag defaults false in `defmodel/step/db_step.hpp`; `gui_model.cpp` sets it true before constructing converter engine models, and existing sim-control initialization already uses it. No new API or per-object mode is introduced.

Mesh copy `0x14085dd40` copies all six scalar doubles at block `+0x10..+0x38` before its `byDim` branch. Retained [L5/d1.c](../evidence/gui_mo/s1/mdt/L5/d1.c) and `llobj_copy_parts.cpp` agree. Thus both primary and work-model mesh settings retain initialized or explicitly imported weights; a later editor-independent copy should not normalize or replace them. Explicit KEY values remain reader-controlled.

The new [focused test](../source/port/defmodel/tests/test_gui_mesh_defaults.cpp) checks GUI constructor/reset values in both dimensions, preserved DEF_PRE values in both dimensions, unchanged dimension-sensitive adjacent defaults, and preservation of custom weights across a 2D-to-3D mesh-settings copy. Target: `defmodel_test_gui_mesh_defaults`; CTest: `defmodel.gui_mesh_defaults`.

This worker used offline PE reads and DUMPBIN only, with no original execution, UI or build. The coordinator reported all 99 CTests passed in the combined C23/mesh build, and that raw historical B129 project files now match. The historical comparison is training evidence, not fresh original acceptance.
