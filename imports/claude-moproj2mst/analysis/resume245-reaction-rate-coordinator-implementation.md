# CP245 reaction-rate coordinator implementation

Implemented the reviewed private `StagedEditor300::commitReactionRateSelection(GuiModel&, int)` and its finite result. The public callback retains binding, temporary/shared-owner preflight and cached ACVCOF control projection. The private member performs one `model.simCtl()` lookup, refuses a null or replaced control, compares the temporary address against live `envtmp`, `cnvcof`, `envatm`, then `acvcof`, and performs the ordered suffix: index 3, selection-zero live dimension clear, live variable from `[0,3,12,1]`, temporary variable. Neither the control nor curve borrow escapes. Geometry exceptions propagate after controls projection.

Strengthened missing/replaced-control and live-alias refusal tests with distinct index, live dimension/variable, temporary dimension/variable, and table storage identity sentinels. Added a callback-level parked geometry regression to the existing viewer geometry fixture; it retains the page before parking, confirms the specific geometry guard exception, and checks projected controls with unchanged index and curve fields. It never calls parked `pageState() noexcept`.

Focused validation (Release candidate, no `--then-full`):

- `python admin/build_candidate.py --label cp245-reaction-rate-final --target m2m_mdt_viewer_box --target m2m_mdt_simcontrol300_reaction_rate`: passed.
- `ctest --test-dir build/candidate -R '^m2m\.mdt\.(simcontrol300_reaction_rate|viewer_box)$' --output-on-failure`: 2/2 passed.
- `git diff --check` on the four owned files: passed.

SHA-256 at completion:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/simcontrol300_owner.hpp` | `ca2e95a060aa5eeb8cf3443a9346003c2ec973dc2d6456343a5952f93d8c6e02` |
| `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp` | `f08d32772e1578d6941eccf6fc1832491fb347f158d4d6f4e6c0074e34df476a` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_reaction_rate.cpp` | `5426bf10a46ff2120f557315ffdba37ca0b1fe50d2a0177ba5aa34ead0336790` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `504d31f1d32e566af3ba1b6f3d8d7cd529f810defc41d96ace5e6c17e24d373c` |
| `build/candidate/bin/m2m_mdt_simcontrol300_reaction_rate.exe` | `9c47c3a12d97842f86364a07d6b5510ff58cc569f923bdda37c3cff0022bb1ad` |
| `build/candidate/bin/m2m_mdt_viewer_box.exe` | `43117a4cdb8e1bb646ee05ffa835354361d1d52dba1d910e53d0094cb2192c18` |

The tests exercise explicit private callback delivery on constructed staged owners, including the model's real parked geometry path. They do not prove physical QWidget delivery, reentrancy, public editor construction, Save completion, or every intermediate state among ordinary stores. The exact suffix order was reviewed in source. Original behavior remains qualified by CP135's bounded static trace; this change adds no fresh original-process differential evidence. Root owns grouped/full validation and source-stable freeze.
