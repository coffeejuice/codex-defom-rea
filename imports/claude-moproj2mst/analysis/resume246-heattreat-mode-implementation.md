# CP246 HeatTreat Mode implementation

Implemented the three named `GuiModel` commands and migrated the six Deformation, Diffusion and PhaseTransformation property slots through a private mode helper. The helper snapshots the bool before `opModel`; setters retain defaults → property flag → native word attempt → refresh, and getters retain word attempt → bool output. STEPDEF is unchanged. `withOwnerSimCtl`, `setScWord`, and the now-unused callback/type headers were removed after scoped search found no remaining consumers.

The L6 fixture now dispatches all three modes with true/false values and checks the corresponding words at SC+0x108, +0x110 and +0x10c; it confirms loaded schedule defaults are applied. Existing detached-step preservation assertions remain in the same fixture. The viewer geometry fixture now directly calls all three commands against a parked model, expects their geometry refusals, and verifies all three words are unchanged.

## Validation

- Focused candidate build: `python admin/build_candidate.py --label cp246-heattreat-mode-focused2 --target m2m_mdt_l6_slots --target m2m_mdt_viewer_box` — passed. No `--then-full` was used.
- CTests `m2m.mdt.l6_slots` and `m2m.mdt.viewer_box` — both passed (2/2).
- Candidate source fingerprint before/after build: `ef83db9c2c8ab3c7b69167566bbfa8dc5916f3f80c285ffe73664c718bd8b9de`, stable.
- Test executable SHA-256: `m2m_mdt_l6_slots.exe` `EA67201DB50BB03A6EBF686DED902C53576F1D233DD81224A62A275B621691DD`; `m2m_mdt_viewer_box.exe` `DF8E88283AF68D4D0016F9F5FEEA2FE19135F857F4766B186967BDA1AB899392`.
- An initial mistaken aggregate-target build failed because `m2m-mdt-tests` is not a configured Ninja target; the subsequent exact target build passed. A first focused attempt caught a test-only `qt::L` overload mismatch, corrected before the passing build.

## Evidence limits

These reconstructed-fixture tests establish the selected model command mappings, property dispatch, detached-step guard and parked geometry refusal in this candidate. They are not fresh original execution, do not establish Qt signal delivery/reentrancy, and do not establish complete HeatTreat lifecycle fidelity. Other HeatTreat aliases and broader model ownership remain outside this leaf.

## Independent review repair

Addressed the test-only HOLD review in `analysis/resume246-heattreat-mode-implementation-review.md`. The L6 Mode table now associates each mode with its actual property flag, seeds distinct values in all three native words before each setter, checks the selected flag/word and unchanged unselected words, then independently sets the getter's property flag and a distinct native sentinel before getter dispatch. The detached-step interval now invokes all three getters and verifies every saved native Mode word remains unchanged; the existing STEPDEF checks and exception-safe step restoration remain. Viewer parked snapshots now apply offsets relative to `&model.step->sim_ctrl`.

Repair validation: focused build `python admin/build_candidate.py --label cp246-heattreat-mode-repair4 --target m2m_mdt_l6_slots --target m2m_mdt_viewer_box` passed. `ctest --test-dir build/candidate --output-on-failure -R '^m2m\\.mdt\\.(l6_slots|viewer_box)$'` passed 2/2. An intermediate strengthened test run exposed an incorrect expected getter flag in the test setup; corrected by explicitly setting each property flag before its getter. No production files changed. The tests remain reconstructed fixture evidence, not fresh original execution or Qt reentrancy proof.
