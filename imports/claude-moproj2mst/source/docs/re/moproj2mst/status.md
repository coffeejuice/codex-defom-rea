# moproj2mst stage 1: G1 gate status

Integration branch `claude/m2m-int` at the merge of `claude/m2m-g1` c2ddd1b0 (2026-10-22). Fresh build `build-int33`.

| check | result |
|---|---|
| tier 1 (`ctest -L m2m`) | 63/63 pass |
| tier 2 (`m2m_tier2`, `runner/tier2.txt`) | 11/11 PASS (53 excluded) |
| tier 3 (`m2m_tier3`, oracle A) | 45 PASS, 0 DIFF, 0 CHANGED, 2 SKIP (size) |
| big projects 29, 30 (copies, path > 260 chars) | 2/2 PASS, project files unchanged |
| runner `--self-test` | passed |
| ledger port-needed rows (`g1/unported.tsv`) | 8/8 ported (`g1/unported.tsv` class `ported`); every open ledger row is classified out of scope |
| owners.tsv | one owner per address |
| `main` | untouched |

## Cases (oracle A)

| case | result | note |
|---|---|---|
| 01_COG_P1_B1 | PASS |  |
| 02_COG_P1_B10 | PASS |  |
| 03_COG_P1_B10_NEGATIVE_FEED | PASS |  |
| 04_COG_P1_B1_LEFT_MASTER | PASS |  |
| 05_COG_P1_B1_MINUS_X | PASS |  |
| 06_BILLET | PASS |  |
| 07_CHAMF1 | PASS |  |
| 08_HEAT1 | PASS |  |
| 09_IMPBIL1 | PASS |  |
| 10_MAT1 | PASS |  |
| 11_THEXP | PASS |  |
| 12_VDIE | PASS |  |
| 13_XY | PASS |  |
| 14_XZ | PASS |  |
| 15_YX | PASS |  |
| 16_YZ | PASS |  |
| 17_ZX | PASS |  |
| 18_ZY | PASS |  |
| 19_XY_H01 | PASS |  |
| 25_Inc718_D420_GFM_V_DIES_Angle2_18 | PASS |  |
| 26_Inc718_D420_GFM_V_DIES_Angle2_19 | PASS |  |
| 27_COG_P1_B1 | PASS |  |
| 28_Inc718_D420_GFM_V_DIES_Angle2_18 | PASS |  |
| 29_2026_04_INC718_diam_450_Option_1_v35 | PASS | read-only copy, 700618 bytes |
| 30_2026_06_Inc718_diam_420_v41 | PASS | read-only copy, 687442 bytes |
| 31_DB_deconstruction_01 | PASS |  |
| 32_DB_deconstruction_02 | PASS |  |
| 33_DB_deconstruction_03 | PASS |  |
| 34_DB_deconstruction_04 | PASS |  |
| 35_DB_deconstruction_05 | PASS |  |
| 36_DB_deconstruction_06 | PASS |  |
| 37_DB_deconstruction_07 | PASS |  |
| 38_DB_deconstruction_08 | PASS |  |
| 39_brick_mesh_01 | PASS |  |
| 41_NewProject_1 | PASS |  |
| 43_vdie2 | PASS |  |
| 44_vdie3 | PASS |  |
| 45_vdie_1 | PASS |  |
| 46_MO2_Boolean_Oprations | PASS | waived L27 SfOprAirConvect2D->SfOprHeatTransferExpress2D; L57 SfOprHeatResting2D->SfOprHeatTransferExpress2D |
| 47_MO2_Extrusion_and_Boolean_Cycles | PASS |  |
| 48_MO2_Operation_Cycles2 | PASS |  |
| 49_MO2_Boolean_Oprations | PASS | waived L27 SfOprAirConvect2D->SfOprHeatTransferExpress2D; L57 SfOprHeatResting2D->SfOprHeatTransferExpress2D |
| 50_MO2_Extrusion_and_Boolean_Cycles | PASS |  |
| 51_MO2_Operation_Cycles2 | PASS |  |
| 52_BEND_2D | PASS | waived L27 SfOprAirConvect2D->SfOprHeatTransferExpress2D |
| 57_Gear_blank | PASS |  |
| 58_Spike_Forging | PASS |  |

Excluded from the v11 oracle (cases.tsv `exclusion`): 53_Cogging_Multiple_Ops_Lab1, 55_Multiple_Operations_Example1, 56_Multiple_Operations_Example1.

## Ledger (`ledger.tsv`, 1121 rows)

| status | rows |
|---|---|
| ported | 887 |
| library | 116 |
| out-of-scope | 78 |
| spec | 39 |
| pending | 1 |

Rows not ported (status spec or pending), with the G1 classification from `g1/unported.tsv`:

| address | packet | role | class | reason |
|---|---|---|---|---|
| 1400135a0 | P16 | accessor (spec section 10) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: accessor (spec section 10) |
| 1400667b0 | P07 | accessor (spec section 10) | out of scope | list entry-0 check called only from engine-model code 0x1408ed830 (no engine model in the converter) |
| 140068b40 | P16 | accessor (spec section 10) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: accessor (spec section 10) |
| 14006b8a0 | P16 | accessor (spec section 10) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: accessor (spec section 10) |
| 14006c210 | P16 | accessor (spec section 10) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: accessor (spec section 10) |
| 1400711a0 | P16 | model object ctor (spec 9) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: model object ctor (spec 9) |
| 1400a43e0 | P03 | project subdir Operations | out of scope | mkpath of project subdirs, side effect (opinfo.cpp skips it by design) |
| 1400a49d0 | P03 | project subdir DoeStudies | out of scope | DoeStudies subdir path in the skeleton DOM, replaced at open |
| 1400b5680 | P03 | SUB ctor (GUI new sub-project) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: SUB ctor (GUI new sub-project) |
| 1400d0e60 | P16 | DOE var entry list (+0x30) | out of scope (DOE) | DOE var entry list |
| 1400d7fc0 | P16 | DOE entry +0x18 | out of scope (DOE) | DOE entry +0x18 |
| 1400d8000 | P16 | DOE entry +0x20 | out of scope (DOE) | DOE entry +0x20 |
| 140136ab0 | G0 | set KFWRIT mode | out of scope | KFWRIT mode set: only seeded through SfPreGenDb(Express) vt+0x1c8 (DEF_PRE GENDB class, not called by the converter) |
| 140136ae0 | G0 | reset KFWRIT mode | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: reset KFWRIT mode |
| 14029ee50 | P16 | BM props +0xb8 write .Wzd | out of scope | DiestressBM props +0xb8 writes the .Wzd side file |
| 1402a16c0 | P16 | BM .Wzd XML writer (field map) | out of scope | DiestressBM .Wzd XML writer (side file) |
| 1403047c0 | P09 | QString copy (+0) | out of scope (DOE) | QString copy in emitDoeVariables 0x1401322d0 |
| 14044b5d0 | P11 | cog+0xd8 billet block | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: cog+0xd8 billet block |
| 1405f79a0 | P09 | default +0x1f8 false | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: default +0x1f8 false |
| 1405fd4d0 | P15 | op member ctor (vbase layout) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: op member ctor (vbase layout) |
| 1405fe2e0 | P09 | op info ctor (+0x80 flags) | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: op info ctor (+0x80 flags) |
| 14075dcb0 | P11 | model+0x540 SfOprCogData | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: model+0x540 SfOprCogData |
| 14075dcd0 | P11 | cog+0x2e20 manipulator block | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: cog+0x2e20 manipulator block |
| 14075dcf0 | P11 | cog+0x1170 die block | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: cog+0x1170 die block |
| 14075dd10 | P11 | model +0x508 Cycle_Time | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: model +0x508 Cycle_Time |
| 14075dd30 | P11 | model +0x4f8 Swaging_Stroke | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: model +0x4f8 Swaging_Stroke |
| 14075dd50 | P11 | model +0x500 Num_Steps_DSMAX | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: model +0x500 Num_Steps_DSMAX |
| 140817870 | P16 | DOE var +0x20 | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: DOE var +0x20 |
| 140818420 | P16 | DOE var points a | out of scope (DOE) | DOE var points, .COR writer (not run) |
| 140818490 | P16 | DOE var points b | out of scope (DOE) | DOE var points, .COR writer (not run) |
| 140821e40 | P13 | DB_STEP_DATA set_dimension (twin 1401c8ba0) | out of scope | DB_STEP_DATA temporary in the MultiBlow writer (no MST byte, s1-04 8) |
| 140824560 | P13 | DB_STEP_DATA dtor (twin 1401cb2c0) | out of scope | DB_STEP_DATA dtor, Convert2Dto3D temporaries |
| 14085f580 | P13 | Cls_14018b220 dtor | out of scope | Cls_14018b220 dtor, Convert2Dto3D temporaries |
| 1408d3d70 | P13 | Cls_1401e7b10 dtor | out of scope | Cls_1401e7b10 dtor, Convert2Dto3D temporaries |
| 140909c50 | P16 | write .COR file | out of scope (DOE) | .COR side-file writer |
| 14091ba60 | P09 | DefDoeModel v1b0 | out of scope (DOE) | SfMoDoeManager / DefDoeModel vt+0x1b0 |
| 1411638a0 | P13 | free-distortion BCC block writer (3D INI, getters not decompiled) | out of scope | free-distortion BCC block of the DEF_MACHDIST_CTRL2.INI side file (plan 2.1) |
| 1411e09a0 | P12 | BooleanProperties +0xb8 save -> 1411e1820 | out of scope | BooleanProperties +0xb8: Boolean.BWZ side-file save (plan 2.1) |
| 1411e0b80 | P12 | INI absolute path | out of scope | INI path for the DEF_MACHDIST_CTRL2.INI side file (plan 2.1) |
| 1411e2450 | P12 | set byte +0x91 | out of scope | no call path from open/save, the op writers or any Sf*/Def* vtable slot the port calls; ledger role: set byte +0x91 |

Paste-path stubs in `src/stubs/p07.cpp` (`140066930` OBJ list copy, `140066ab0` OBJPOS list copy) are not reached by open or save; ledger rows out-of-scope (P07).
