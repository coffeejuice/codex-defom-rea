# CP233: HeatTreat reader dimension predicates through the owner query

Replace the two direct-step dimension predicates in `source/apps/moproj2mst/src/ops/heattreat_htwz.cpp` with calls to the existing `GuiModel::dimension() const noexcept`. This removes a bounded pair of raw aggregate reads from the shared HeatTreat schedule/property dispatch body without editing a common model file, changing admission, or waiting for live ObjectKey issuance.

Root confirmed no ownership conflict: CP232 owns `engine.hpp`, `gui_model.cpp`, `simcontrol300_owner.cpp`, and the frequency-text test; CP233 loader is currently a read-only design. This recommendation owns only the HeatTreat implementation file, with `tests/p16/test_htwz.cpp` reserved only if an additional edge-case assertion is justified. It makes no edit to CP232's files or CP233's proposed `loader.cpp` / `test_db_load.cpp` seam.

This is a read-only design. Only this report is written; no source, tests, builds, shared recovery state, original process, GUI control or mutable REA state changed. No tests ran for the report.

## Exact source change

The anonymous-namespace predicates currently appear around lines 67–68:

```cpp
bool is2D(const GuiModel* m) {
    return m && m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 2;
}
bool is3D(const GuiModel* m) {
    return m && m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 3;
}
```

Replace only their returned expressions:

```cpp
bool is2D(const GuiModel* m) { return m && m->dimension() == 2; }
bool is3D(const GuiModel* m) { return m && m->dimension() == 3; }
```

Keep the helpers and every call site in place. In particular, do not combine the predicates into an `else means 3D` classification, cache a dimension across parsing, move a read ahead of the model/object guards, add geometry refusal, or change model construction. No declaration, model implementation, raw getter, callback, identity type or C ABI change is required.

`readV12` uses `is2D` then `is3D` around lines 275–276 to select supported model dimensions, and uses `is2D` again around line 316 for the existing 2D V1 zone-region refusal. `readFile` uses `is2D` around line 378 when selecting legacy file versions. These helpers participate in the common reader used by the property/open path, operation loader and MST writer; they are not isolated unused helpers.

## Source equivalence and supported-state qualifications

The inspected `GuiModel::dimension()` in `src/engine/gui_model.cpp` returns exactly `step ? step->dimension : 0`. It is unguarded, nonmutating and `noexcept`; it neither issues a token nor dereferences an object/control. `include/m2m/engine.hpp` states those scalar semantics. `source/port/defmodel/include/defmodel/layout/db_write_var.hpp` declares `std::int32_t dimension` and checks `DEF_LAYOUT_OFFSET(DB_WRITE_VAR, dimension, 0x01c)`; `DB_STEP_DATA` inherits that base at offset zero in its inspected layout. The old byte-offset read and the owner query therefore read the same field in the supported layout.

The complete predicate mapping matters:

| Reached state | Existing predicates | Proposed predicates |
|---|---|---|
| `m == nullptr` | Both short-circuit false without model access. | Same `m &&` short circuit; method not called. |
| Live model, `step == nullptr` | Both false after nullable-step check. | `dimension()` returns 0; both comparisons false. |
| Existing step, dimension 2 | `is2D` true, `is3D` false. | Same. |
| Existing step, dimension 3 | `is2D` false, `is3D` true. | Same. |
| Existing step, any other integer, including 0/negative | Both false. | Same exact comparisons; no normalization. |
| Existing step on a geometry-parked model | Predicates read the scalar without a guard. | Existing scalar query also stays unguarded; no new throw. |

The mapping is for valid, synchronously accessed C++ model/storage states. Neither expression supports a dangling model pointer, a concurrently destroyed/replaced `unique_ptr`, or undefined layout tampering. The converter's existing execution assumptions remain; this two-line migration creates no concurrency guarantee or protection against those states. Preserve each predicate evaluation where it currently occurs rather than hoisting a snapshot across later native work.

This is deliberately different from CP226's adjacent DieStress scratch read: that old callback performed geometry admission and unchecked native dereference. These HeatTreat predicates themselves are nullable and unguarded, so the CP224 scalar query preserves their actual behavior. Do not generalize this substitution to another raw read solely because it also reads `+0x1c`.

## Minimum validation and exclusive test seam

Run the existing `tests/p16/test_htwz.cpp` target; expected CMake target/CTest name are `m2m_p16_htwz` / `m2m.p16.htwz`, to be confirmed from configured metadata. The fixture creates real engine models with object 1 and explicitly documents synthetic reader inputs rather than a recovered sample file.

Existing cases already check the meaningful dispatch results:

- `old_format_without_engine_model_reads_nothing`: null-model legacy format refusal/result.
- `v2_3d_with_engine_model`: 3D V2 parsing retains media/steps.
- `v1_2d_with_engine_model`: 2D V1 reaches the existing unsupported region path; the same bytes on a 3D model stop at the incompatible token rather than reaching it.
- `v12_needs_object_one`: model/object preconditions remain before legacy parsing.
- `v3_with_engine_model_uses_v3_reader`: V3 selection with an existing model.

For this reversible expression relocation, source equivalence plus that existing focused target is sufficient; do not add a test mirroring the two expressions. CP224's `test_viewer_box.cpp` already exercises `dimension()` for 2/3, missing step and parked geometry; leave it unchanged and retain it in root's grouped regression/full run.

If root wants an additional dispatch edge case, the only narrow useful addition is in the existing P16 test file: extend `v3_with_engine_model_uses_v3_reader` with a separately created model whose `step` is reset, parse `kFull`, and assert the same V3 step result. `kFull` has `NO_ECTMFN 0`, so this reaches the unguarded dimension dispatch without later needing `m->object()` or native control. Keep any old-format missing-step check separate: `readV12` has its own earlier explicit missing-step return that this migration does not change. Do not invoke parked object/ECTMFN work merely to test a scalar predicate.

No new quoted include or test registration is needed for the two-line production change: `heattreat_htwz.cpp` already includes `m2m/engine.hpp`. Retain the native layout includes because other functions in the file still use them. Any optional new assertion must use the existing Testkit entry point and `CHECK`/`CHECK_EQ`/`REQUIRE`; no C `assert` or `REQUIRE_EQ`. The existing `readBytes` path writes a temporary file; root's launcher must retain its configured `TEMP`/`TMP` under `M2M_TEST_SCRATCH`. No missing-file fixture or external path is needed.

Root owns grouped focused validation, full source-stable launcher and freeze. This recommendation does not claim a new candidate result or reuse prior checkpoint counts as validation for this change.

## Evidence and original-behavior limits

Reviewed CP222's property alias ledger, CP224's dimension implementation/review, CP225's object mutation audit, CP226's command contract, and current CP233 editor/loader next-leaf notes. The actual change is justified by the current C++ field/query equivalence above, not by assuming that a method named `dimension()` must match every raw predicate.

`source/docs/re/moproj2mst/mdt/L6/spec.md`, section 6, identifies the shared reader bodies and historical retained Ghidra/decompile provenance. It states that the single HeatTreat implementation serves open/property, loader and P16 writer paths, and records the unsupported V1 2D region and missing engine-model cases. The source annotations map these predicates to GUI `0x140c7c0e0` / `0x140c7c0a0`; they do not prove that the reconstructed scalar method has an original public API counterpart or original return type. The new call is an adapter representation of an existing scalar read.

The same specification records no original active-operation oracle for the full HeatTreat chain. P16's test header says the inputs follow the reconstructed `fscanf` sequence and no sample file exists for that packet. These tests qualify current reader dispatch and retained values only; they add no fresh original runtime witness, whole-project parity or proof that the original executed a particular branch.

## Reconciliation with CP225

This leaf performs no object creation/removal/recreation, bulk reload, dimension mutation, binding change or identity admission. It does not issue an ObjectKey and cannot certify live object storage. CP225's blockers remain: hidden native dimension replacement, placeholder creation, deletion of retained empty nodes/additional tails, public aggregate replacement and raw consumer aliases.

Within this same HeatTreat file, `readV12` still acquires the raw aggregate/object, later V3 ECTMFN parsing still retrieves a raw object, `engineDefaults` still reaches native control through public storage, and `propsFromSimCtl` still accepts an untyped control pointer. This recommendation removes only the two scalar raw reads. It does not claim the file, property family, viewer or model is closed, and it does not alter StepIdentity qualification or R300 cache1 refusal/original-witness gates.
