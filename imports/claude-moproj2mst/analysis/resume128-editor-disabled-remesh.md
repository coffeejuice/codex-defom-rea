# CP128 implementation: disabled private remesh skips unused method admission

Private applyMeshing3D now requires method-radio1/3 only when remeshEnabled is true. A disabled page still commits/copies its remembered per-object four scalars, then writes only RemeshState.enabled=0. Method, scaling, internal-element setting and every other singleton field remain current. Full public Session/editor palette construction remains guarded.

## Native contract and implementation

Controlling report `analysis/resume128-editor300-next.md` SHA-256 is `dea828dbb0bae37de44ee2e5512a23ac5558c660f412444ce8035e3987cacecb`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Native140ec1350 first calls the current-form handler at13a0, then loops and copies four scalars to each still-present actual model object at14df. Only afterward the3D global branch reads disabled/enabled radios. Disabled140ec15c4 calls1400ab150 with byte0, then15c9 jumps179d before method/scaling/internal-checkbox reads. Getter1402c7ed0 returns143d91238; setter1400ab150 stores exactly receiver+29. Exact half-open byte hashes and retained instruction/decompile identities are pinned in the controlling report.

The only production change is to make the existing method exclusion conditional on form.remeshEnabled, plus its explanatory comment (+3/-1 lines against frozen CP127). No object loop, conversion, missing-object skip, enabled-byte assignment or enabled-tail setter moved. Wrong dimension/missing step and enabled-invalid method still return UnsupportedSnapshot before object/state effects under existing reconstruction admission. This does not newly claim the native partial effects of an excluded enabled profile.

Unused private method sentinels0/99/INT_MIN do not describe new physical native radio states. They demonstrate that this branch does not consume that projection. The disabled scaling text/checkbox remain unconsumed as before; no parser or QWidget state was broadened.

## Dedicated focused cases

New `tests/mdt/test_simcontrol300_disabled_remesh.cpp` contains4 Release-active Testkit cases:

1. Actual3D GuiModel objects plus the actual ConversionSession::remeshState member. Disabled Apply succeeds for unused method sentinels, still writes the selected-first rounded/absolute/clamped four scalars and the unvisited remembered bits, preserves an unvisited NaN payload and signed zero, preserves unrelated object fields, and writes only enabled0 in the same shared owner. The current method, raw scaling NaN bits, other bool/byte fields, explicit inter-field byte array, filter and uniform parameters are all compared individually. No unsafe C++ struct-padding memcmp is used.
2. A real remembered middle object is removed through existing defmodel::object_list_remove after Fill. Apply skips its now-missing lookup, updates the present objects before and after it, then stores enabled0. The removed pointer is never used afterward; no synthetic model success/failure hook was added.
3. Re-enabled invalid-method Apply remains preflight refusal, retaining current object and singleton fields with no effects.
4. A wrong-dimension model and an actual absent-step model remain refused even though the disabled method is unused. Object and singleton state stay unchanged.

These tests are private page/model calls. One uses actual Session-owned remesh storage, but neither it nor the others execute the full public Session editor callback, original GUI or native event/constructor path. No test file/directory side effects or test scratch were added.

## Static preflight and frozen source

Verified exact production path before editing. Compared its complete small diff against `build/wave3-checkpoint127-verified/source.zip`; checked all new quoted/defmodel/Testkit include targets, supported CHECK/CHECK_EQ/REQUIRE macros,4 unique TEST_CASE names, one main, no C assert/REQUIRE_EQ or raw NUL source, balanced braces and whitespace. Existing object-removal declaration/implementation was inspected for lifetime and link ownership; the test calls its actual maintained linked-list removal and never dereferences the deleted object.

`git diff --check` reported no errors. No compiler/build/CTest, GUI, original execution or mutable REA was run. Static preflight does not establish compile/test success. Expected dedicated root target is `m2m_mdt_simcontrol300_disabled_remesh` / CTest `m2m.mdt.simcontrol300_disabled_remesh`, discovered by the existing test glob after root configuration.

| Exclusive file | SHA-256 |
| --- | --- |
|`source/apps/moproj2mst/src/engine/simcontrol300.cpp`|`260331d646fdc5edf712278c6e056142996a8b4df54a59bfd1db8c96b9b52abc`|
|`source/apps/moproj2mst/tests/mdt/test_simcontrol300_disabled_remesh.cpp`|`457cd866bf861233fbcb83e0fb346616cc56dd7f5a24bd47e72e47937eb53ab3`|

Only these two source/test files and this new receipt were written. No shared headers, Spring/native IO files, lifecycle, Session, public loader/factory, CMake or build artifact changed. No reconstructed executable/deployment was inspected or produced. Source is frozen/released for independent review and root's grouped validation; no further worker writes until a new release window.
