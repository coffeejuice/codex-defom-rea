# HTFF state trace: global furnace unit selection

The numeric mismatch has a specific source explanation: furnace defaults depend on the process-wide Fortran unit system, separately from the new model's UNIT field. Serial output contains the converted furnace defaults; two-worker output contains the unconverted defaults. The preceding project-unit schedule fits this mechanism exactly. The actual last setter call at runtime remains to be captured before claiming the complete dynamic causal chain.

Recovery remains paused. This report uses read-only bounded source, input metadata and retained raw captures; no builds, converter executions, tests, source changes, fixture changes or original applications were performed.

## Confirmed raw evidence

Retained files under `build/test-speed-candidate/t/ab244c6e` are all 8,543 bytes:

| Capture | SHA-256 |
| --- | --- |
| serial `m2m_folder_capture_25632_1054950754700/generated-simctrl.key` | 6f702539f8fd52dd55028bb43d0593e85aebff419df569857bf4135b7f30a6c9 |
| serial expected; two-worker expected | 6f702539f8fd52dd55028bb43d0593e85aebff419df569857bf4135b7f30a6c9 |
| two-worker `m2m_folder_capture_12548_1068772284200/generated-simctrl.key` | b6162ff55d58d30b3fa8bc865549c74a83981032f8a635ca312e0f026dd95d0c |

Both generated files have 173 lines. Differences occur inside furnace region records emitted by ENVTMP mode 10, rather than an arbitrary group of unrelated simulation controls. The record numbers are furnace curve numbers, not top-level KEY group IDs.

| Furnace curve/value | Serial | Two-worker | Source correspondence |
| --- | ---: | ---: | --- |
| 2, temperatures | -17.777777778 | 0 | (0 - 32) / 1.8 |
| 4, temperature | 1315.5555556 | 2400 | (2400 - 32) / 1.8 |
| 5, temperature | 454.44444444 | 850 | (850 - 32) / 1.8 |
| 8, row 3 col 3 | 1177.2 | 0.4 | 0.4 times unit_factor(0x14, 2), factor 2943 |
| 8, row 4 temperature | 20 | 68 | (68 - 32) / 1.8 |
| 8, row 6 temperatures | 237.77777778 / 275.55555556 | 460 / 528 | Fahrenheit-to-Celsius conversion |
| 8, row 6 radiation value | 5.66979e-11 | 3.306e-15 | factor 0x13 mode 2, 17150 |

These values match `Cls_1403753f0::set_defaults` constants followed by `convert_units(2)` exactly. The source's broad comment about SI defaults is misleading when read literally: the explicit constants and conversion branch establish what happens. The two-worker values are the seeded values; the serial values are the converted values.

## Confirmed source chain

`port/defmodel/src/members/furnace.cpp::set_defaults` initializes all furnace curves, then executes `if (ext::fun_140235c10() != 2) convert_units(2);`. `members/ext_deps_wiring.cpp` maps that function to `def::rt::ftn::unit_system()`. In `port/defrt/src/ftn/units.cpp`, unit_system returns 1 when `g_units.unit_flag == 0`, otherwise 2. `units.hpp` declares one inline process-wide g_units object; this is separate from sc.unit and from the converter's thread_local GUI startup-unit profile.

The source writers to that flag found in the conversion closure are:

- `port/defkey/src/kw_g1a/read_g1a.cpp`, UNIT parser: only when a valid incoming UNIT differs from the model's current unit, it assigns sc.unit, resets unit defaults and calls the model unit hook or `rt::ftn::set_unit_system(sc.unit)`.
- `port/defdbio/src/dbread/lead.cpp`: after reading the DB simulation controls, calls set_unit_system with the model unit at offset 0x84c (DB step + sim-control UNIT).
- `port/defrt/src/ftn/unit_system.cpp::set_unit_system`: 1 sets unit_flag 0, 2 sets it 1, other codes set it 0, then rebuilds conversion-unit tables.

`src/engine/loader.cpp::newModel` first constructs GuiModel, then calls `def::key::set_model_unit(step, p.unit, true)`. In `port/defkey/src/item_writers/model_unit.cpp`, that setter modifies only sc.unit and its selected environment defaults; it does not update the Fortran global unit system. Its reset helper also does not reset furnace curves. During GuiModel construction, DB_STEP_DATA initialization and guiSimCtrlInit call sim_ctrl_init_u3a8, which creates furnace curves and calls their set_defaults. Therefore a newly constructed SI model can retain furnace defaults selected by an English global left by a preceding conversion. Setting model UNIT correctly afterward does not repair those curves.

The HTFF input is SI and its existing Task00001/Task00002 SimCtrl.KEY files have UNIT 1, are 89/83 lines long, and contain no FRNNAM/furnace-default region block. The model's existing furnace curves thus need not be replaced by that input. `loadSimCtrl` imports controls when OprFlag is nonzero; the target moproj has OprFlag 1. In loader_l2, furnacePostLoad enables the furnace but only rebuilds its curves when curve_count is zero. The preinitialized nonempty curves survive. furnacePropsLoad copies them to the furnace properties. furnacePropsToEngine later writes name and curve-8 environment values and selected step controls, not a full unit conversion of all furnace curves.

`saveSimCtrl` does not select or normalize units. It writes flags 0xf, then write_simctrl_key_stream writes the banner and group1. g1b::write_envtmp sees furnace_on != 0 and serializes every existing furnace curve under ENVTMP mode 10. This preserves the earlier initialization difference in the saved file.

## Last cases and state writes

Read-only fixture inspection establishes project Unit and existing SimCtrl.KEY UNIT values below. These do not prove the final global value: KEY import may skip its global write when units already agree, and a DB load or other KEY inside a conversion can be the actual final setter.

| Ordinal | Case | Project Unit / task KEY UNIT | Presence before target |
| --- | --- | --- | --- |
| 50 | B:R11, corpus 11_THEXP | SI / 1 | Serial and two-worker target lane |
| 52 | B:82_R52_BEND_2D | English / 2 | Serial, two-worker and four-worker target lane |
| 56 | B:103_B_HT3F | SI / 1 | Serial, two-worker and four-worker target lane |
| 58 | B:105_B_COG7 | English / 2 | Serial and two-worker target lane |
| 59 | B:106_B_F2ACT | SI / 1 | Serial only |
| 60 | B:107_B_HTFF | SI / 1 | Target |

The serial suffix ends English COG7 -> SI F2ACT -> SI HTFF. The two-worker target suffix ends SI HT3F -> English COG7 -> SI HTFF. The four-worker target suffix ends SI HT3F -> SI HTFF. This predicts global SI at serial/four-worker target initialization and English at two-worker initialization, matching the raw default-conversion signature. The exact global read and setter ordering within those executions has not been logged, so the table's predicted final globals remain a source-backed hypothesis.

## Smallest confirmation and test implications

The next bounded confirmation should log the Fortran unit_system plus model UNIT at entry/exit for COG7 and F2ACT, and at each furnace set_defaults entry/exit while HTFF is being constructed. Also record each set_unit_system caller and incoming unit. Retain the captured curve-4 temperature or curve-8 row-3 value before/after initialization. This distinguishes a stale process-global read at construction from a later unit-conversion error without changing semantics.

A test-only model microcase can set the Fortran global to 1 versus 2, construct otherwise identical SI GuiModels, and compare furnace curve 4 before and after set_model_unit. The source predicts 1315.5555556 versus 2400 despite the same final model UNIT. A controlled sequence COG7 -> HTFF versus COG7 -> F2ACT -> HTFF then checks whether those projects supply the predicted global states in the actual converter path. These are proposed experiments, not tests executed here.

This is a real raw output regression in sharding, not only adjusted-comparator noise. Keep serial default. A four-worker matching log avoids this particular predecessor state but does not establish general equivalence. Resetting the Fortran global per child/case, assigning it from p.unit before construction, changing furnace initialization or adding comparison masks would alter converter state semantics and exceeds behavior-preserving test optimization while recovery is paused. If parallel throughput is pursued, worker scheduling must preserve the serial initialization state or be proven equivalent with raw receipts; this trace alone does not authorize an implementation change.

## Source fingerprints

- `source/port/defmodel/src/members/furnace.cpp`: `bb8ac2cddcb562127928afba78627397f9508a14d88e1059b5192f6c0df4a86a`
- `source/port/defrt/src/ftn/units.cpp`: `7e24313bec892554fc2e99696982063ea052c08b7b5a7a549fa95225cf82b93f`
- `source/port/defrt/src/ftn/unit_system.cpp`: `f881ed4536b7cef62f45c630396e3d4d380c9ebf3025fbe4edc17037651cc6fd`
- `source/port/defkey/src/kw_g1a/read_g1a.cpp`: `f6b6682e4664e8e57262ef7fb04d7b9b9b94228f0bf4cfe0acf2ac95ed8c7d98`
- `source/port/defkey/src/item_writers/model_unit.cpp`: `b1cb58df6edd60e6ad78430d2a8b0b11388f654f5885fa81d7ed4cfe923c9466`
