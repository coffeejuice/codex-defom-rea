# CP253 initial Editor300 dimension preflight implementation

## Change

Implemented the reviewed one-expression migration in `openInitialEditorInto`:

```cpp
if (!control || model.dimension() != 3) {
```

The preceding report reset, `AlreadyStarted` check, `constructionStarted` assignment, and single `simCtl()` lookup retain their positions. Numeric-locale validation remains after dimension admission; source identities and page effects remain after locale validation. No common model API, recovery plan/state, or other production source was edited by this implementation.

The lifecycle test now calls the actual `openInitialEditorInto` seam for live dimensions 2 and 0, an absent step, live dimension 3 with an unrecovered locale, and a refused session retried after repairing its model. Invalid dimensions are paired with an unrecovered locale to verify `InvalidModel` precedence. Assertions cover preflight stage, issue, construction state, completeness, reached-page/page flags, and null source identities. The `Extra3DIo` fixture counts all three read entry points and retains its write/remove effect log; each preflight asserts zero reads and zero effects. The absent-step case asserts the step remains absent.

## Validation

- Focused build: `python admin/build_candidate.py --label cp253-editor-dimension-preflight-focused --target m2m_mdt_simcontrol300_lifecycle --target m2m_mdt_gui_model_dimension` — passed; target names confirmed.
- Focused CTest: `m2m.mdt.simcontrol300_lifecycle` and `m2m.mdt.gui_model_dimension` — 2/2 passed.
- `admin/check_new_cpp_bindings.py` on both changed C++ files — passed with no issues.
- `git diff --check` — passed.
- No full build or freeze was run.

## Evidence, hashes, and limits

Focused build receipt: `analysis/cp253-editor-dimension-preflight-focused-build.json`; it records source-tree SHA-256 `d21fa9b741e2ce0c1f62a141ddec6ad4664ef3e95f3793effe2c7b41604c8231`, target list, and build success. Test executable SHA-256 values:

- `m2m_mdt_simcontrol300_lifecycle.exe`: `A611E9C986B35F628A6780BC334B1AB820DD13F26EAB7DF14B62E0E698555041`
- `m2m_mdt_gui_model_dimension.exe`: `B804968DF2C213CBD9ECCFA9F507EBFA4F022E7B6FA441710E9D74EBE2B761BF`

Changed source SHA-256 values:

- `simcontrol300_lifecycle.cpp`: `56506EFEC7A78BEB1B9FC001157FD7671BF16CC53154ADBAD2ADCD3E74FBAFFD`
- `test_simcontrol300_lifecycle.cpp`: `DA4D02660B63F6CC71D3A7AACBD9CD88CE6F3A6F8708BF6A981A78CB885F7FB0`

These reconstructed caller tests establish the adapter's report/effect prefixes for the tested synchronous fixtures. They do not establish fresh original runtime behavior, concurrent mutation safety, physical Qt behavior, successful public Save, or completion of the editor lifecycle. The retained native getter attribution and qualification in the CP252/CP253 reviews remain controlling; this change does not claim original graceful handling of null or zero-dimensional models.

## Review correction and rerun

The independent CP253 implementation review found that constructing `GuiModel(nullptr, 0)` still allocates a zero-dimensional step, so the original absent-step fixture was invalid. The test now constructs a normal 3D model, calls `model.step.reset()` for the absent case, and uses `REQUIRE(!model.step)` before the caller and `CHECK(!model.step)` after it. Other matrix cases and all no-I/O assertions are retained.

The original receipt label already existed, so the updated focused build was recorded as `analysis/cp253-editor-dimension-preflight-focused-r1-build.json` (same two requested targets; successful and source-stable). Focused CTests passed 2/2; binding preflight and `git diff --check` passed again. Updated test-source SHA-256: `21C189D5183CD479A08BE74958C31D8F128FD07E94B7F9B6D2474EEE298127EE`. Production source remains `56506EFEC7A78BEB1B9FC001157FD7671BF16CC53154ADBAD2ADCD3E74FBAFFD`. Rebuilt lifecycle test SHA-256: `89754EB6B771070B3F26179120AA2D19DC7EF533C8437C20159BA4EB73007D95`; dimension-test SHA-256 remains `B804968DF2C213CBD9ECCFA9F507EBFA4F022E7B6FA441710E9D74EBE2B761BF`.
