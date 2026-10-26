# CP241 L3 live non-kind-7 object count implementation

The model now owns `countNonType7Objects()`, a scalar query that directly calls `def::pre::step_count_objects(step.get())`. It preserves the helper's unchecked live-step precondition. No geometry admission, null-step fallback, or C ABI change was added.

Only the two live expressions in `expressBasePostLoad` and `hteObjectTreeAdd` changed. Their `merge` short circuit, object-slot loop bound, refusal text, order, and return/throw boundaries remain in place. The query is distinct from `objectCount()`, which reports `u1378` slots.

`test_engine_l3.cpp` creates four linked objects with kinds 1, 7, 0, and 7. It checks a non-kind-7 result of 2 against four total slots and the native helper. It then unlinks the kind-0 object while retaining `u1378 == 4`; an explicit position-4 lookup returns null, the count becomes 1, and the owner query still agrees with the helper. The null case is established by linked-list position, not an object-ID gap. No null-step runtime call was made because the native helper does not define that case.

The focused candidate build used `python admin/build_candidate.py --label cp241-l3-non-type7-count --target m2m_mdt_engine_l3` without `--then-full`. Its receipt is `analysis/cp241-l3-non-type7-count-build.json`: configure and target build exited 0, with `source_stable_during_build: true` and source-tree SHA-256 `42e7e6adb9200a730bfaba7b079668d03ed220ea66af3ceacfa8905b08103c27`. The built `m2m_mdt_engine_l3.exe` SHA-256 was `f090bf8b79dc42d6c51e8ed69a2b399fe5b678efe9c589bfcf78ae4fbbe91ebd`. Registered `ctest --test-dir build/candidate -R '^m2m\.mdt\.engine_l3$' --output-on-failure` passed 1/1. `git diff --check` passed.

Owned source SHA-256 after build:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `dff0bf83a6130d82e3496b1b3f65b54874df6be75ba71c55a1df1614976c8b10` |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | `eb454c6c9fabcc03bd4517908325f546bd48419e562f09db799bd33908614141` |
| `source/apps/moproj2mst/src/engine/loader_l3.cpp` | `bb6d64cd7aa5654c86a45c403aea326b05fd3bf6020aeb6b25514dcd06b85bf2` |
| `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` | `17c95d2ac6aab7559e2662df47d8427089abc059497db7938cf89b5b9dfbc517` |

Evidence remains source/decompile-backed through the CP240 plan and PASS review. The direct test covers the owner value query; it does not exercise either FormingExpress/HeatTransferExpress refusal caller. No fresh original application, GUI, or REA comparison was performed here. No full build or freeze was run.
