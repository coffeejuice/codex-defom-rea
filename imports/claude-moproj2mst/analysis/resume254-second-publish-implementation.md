# CP254 secondary publication-order implementation

The canonical empty-secondary path now publishes its constructed model through `displaySetSecond` before assigning its backlink or querying destination/main controls. A later reconstructed main geometry guard therefore leaves the display owning the published partial secondary. Constructor failure remains before publication; no catch, rollback, extra admission, named-creator change, flag update or geometry refresh was added.

## Source and original evidence

Read the CP254 order contract and PASS review; CP252 default implementation PASS review and named-display/no-step contract; retained complete secondary creator in `evidence/gui_mo/s1/mdt/L4/vs/b.c`; current scene creator, guard, constructor, control getter, destructor and display teardown/forget code. The original v11.0.2 EXE identity retained by that evidence is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP252's independently decoded `0x140cf9990..0x140cf9ac8` bytes have retained SHA-256 `635617aa3ec47b312986217bfea91ee855f820285531448a005b9a1b431218ec`. This work reuses those qualified immutable inspections; it did not independently disassemble or execute the original.

The retained native creator publishes at `0x140cf9a2f`, then resolves secondary/backlink, destination control and source control before GEOTYP copying and current-guard return. Current `scene.cpp::displaySecond` now follows that bounded publication position while retaining the CP252 dimension getter (`dimensionOr(3)`), existing-secondary shortcut, missing-main return and new-control-before-main-control order. After handoff, its finite local raw pointer comes from `dm.second.get()` and the unique owner resides in `secondOwned`. A later `simCtl()` guard throws without unwinding that owner. `displayDropSecond` destroys the owner; the existing destructor/backlink/forget and weak-lifetime machinery clears observations. No ownership helper changed.

## Caller verification

The ordinary caller table covers live dimensions 2, 3 and present 0 and now seeds main GEOTYP 173, checks successful copying, backlink/owner identity, unchanged main identity and flags, plus reuse. Existing absent-step fallback, main-destruction, explicit-drop and null-main tests remain.

The existing real parked-main fixture seeds GEOTYP before parking. After its earlier pure-refusal timer/index-table assertions, it requires both secondary slots empty, gets a fresh ordinary-model constructor default and verifies it differs from the source sentinel, then invokes production `displaySecond`. The specific geometry error propagates, while guard/unique owner/backlink/dimension are retained and destination GEOTYP remains the observed constructor default. Reuse returns the same secondary while the main still refuses its control query; explicit drop expires an independent `GuiModelGuard` and empties both slots. Whole main step bytes, first-object bytes, geometry-variable body identities/counts, per-model tables, viewer, flags/state, picking state, main/backlink and camera remain unchanged; a later parked poll still refuses.

New ordinary and secondary constructors legitimately initialize native state and may change process globals. Their block is deliberately after the pure-refusal global snapshots. No constructor-global equality is claimed or enforced. Reuse is inferred to bypass main-control access from a normal return while the same parked main still throws, together with the static first branch; no production query observer was added.

## Validation

Confirmed configured CTests `m2m.mdt.display_selection` (#120) and `m2m.mdt.viewer_box` (#244), targets `m2m_mdt_display_selection` and `m2m_mdt_viewer_box`. Ran `python admin/build_candidate.py --label cp254-second-publish-focused --target m2m_mdt_display_selection --target m2m_mdt_viewer_box`: configure/build passed, source stable. Corrected accidental display-selection line-ending churn, then repeated only that focused build under `cp254-second-publish-focused-final`; both steps passed and source was stable. The final source tree hash is `ced6ebfea701951fddd48c45e9fd7cd35b8a45e3066516531287b1440077df7c` (before = after; 3563 files).

Final `ctest --test-dir build/candidate -R '^m2m[.]mdt[.](display_selection|viewer_box)$' --output-on-failure` passed **2/2**, 0 failed, 0.45 seconds real. Log: `build/logs/cp254-second-publish-focused-final-ctest.log`. Binding preflight passed all three changed C++ files, with supported Testkit entry points/macros and resolvable includes; receipt `analysis/cp254-second-publish-bindings-preflight.json`. The new complete control include resolves to `source/port/defmodel/include/defmodel/layout/cls_1401e7b10.hpp`. `git diff --check` passed. Final diff: 68 insertions, 3 deletions across the three owned C++ files. No full build, full CTest run, freeze, plan/shared-state edit or deployment occurred; root retains integration/L5 regression/full-run/freeze ownership.

## Limits

This closes the reviewed stable synchronous canonical empty-secondary publication order. The public representation still permits inconsistent null-guard/live-owner or cross-aliased main/secondary states; earlier owner replacement can invalidate cached main pointers in those externally fabricated cases. No universal alias guarantee, foreign-owner error-policy parity, callback/reentrant/concurrent replacement guarantee or invariant redesign is claimed. Constructor/allocation failure is source-reviewed only: `make_unique`, lifetime/native-step initialization and GUI controls precede handoff. No global-new fixture or allocator hook was added. Original allocator/new-handler/constructor cleanup, physical Qt registration/guard exceptions and SEH disposition remain unobserved. The parked exception is the converter's reconstructed geometry barrier, whereas the original getter is a raw nullable projection. Original named-map-miss/no-step remains a distinct later unchecked load; no named creator was modified. This does not qualify accepted Close, end-to-end lifecycle completion, ObjectKey, native storage mutation generation, reader continuation or cache1 access. No original process, GUI-control session or mutable REA session was opened.

## Final SHA-256 bindings

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/scene.cpp` | `e8a1b402280b133fd9013327c1db82f3d5671e7dc71736316ff9b11d1abc532d` |
| `source/apps/moproj2mst/tests/mdt/test_display_selection.cpp` | `3644e5e77533976289de32b4c583ef2abeba8140a1ba82a6c7d94804e05e2b40` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `a89797d1cfa91616824dbe389d0a2d0a39684da520c5328329d8d3d2f8441a16` |
| `build/candidate/bin/m2m_mdt_display_selection.exe` | `7715803a2220dcd2b5408a02a82c6253896bb584594605362575d43172e98d17` |
| `build/candidate/bin/m2m_mdt_viewer_box.exe` | `93df62afa45c2422c76fade572ebde2863bedf950322c733117a585e9fa12d98` |
| `analysis/cp254-second-publish-focused-final-build.json` | `66c1af791d18d7030c07fa19633f4abecd75fca47c6ab84d1a00e25c36d7be29` |
| `build/logs/cp254-second-publish-focused-final-ctest.log` | `2ebda3eec9bbc3fa6a37503e7631fbc7341b0066db2549a662501eafefc1579c` |
| `analysis/cp254-second-publish-bindings-preflight.json` | `fdc5741982d20cf6df438cdb990451f1cff3cd8b937acf330c463450ed81bd5d` |
