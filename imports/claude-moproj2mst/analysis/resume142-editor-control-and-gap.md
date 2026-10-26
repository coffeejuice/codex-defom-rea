# CP142 — GUI release gate and next Editor300 cut

## GUI-control release remains unverifiable

No supported release operation plus verifiable receipt/status is exposed. The installed Computer Use API's full `sky` window-client surface has no control release or release-status method; the `mcp__cua_repl` surface offers JavaScript reset but no release receipt. The tool inventory's release-named matches are Linear release management only. `analysis/gui-control-lifecycle-assessment.json` still records `unsupported_release_api`, and `python tools/gui_validation_status.py` reports `gui_control: release_unverified`, `release_ready: false`, and blocks dependent GUI work. This check acquired no GUI session and establishes nothing about whether an overlay may currently be active. Fresh original/GUI captures remain blocked until both a release operation and independently verifiable completion status are supported.

## Next source-closed Editor300 cut: private solver6 Apply

The latest editor300 implementation checkpoint, `analysis/resume141-editor-next.md`, proposed bounded solver6 Iteration Fill; current `simcontrol300.cpp::fillIteration3D` and `tests/mdt/test_simcontrol300_explicit_fill.cpp` now contain that increment. Current owner state also contains `explicitParameterText[4]` and the explicit visibility-only toggle. The next narrow source-supported gap is `applyIteration3D`: its initial solver admission guard still excludes solver6, so the source-qualified solver6 form cannot be applied.

Retained original evidence, tied to `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, is `analysis/wave2-save-diffs-300-save-140ec3130.asm` and `analysis/wave2-save-diffs-300-apply-140ec3130.c`. Native Apply checks the explicit solver radio (`UI+0xb00`); when selected, it writes parameter count 4, reads QLineEdit texts at `UI+[0xb18,0xb28,0xb38,0xb48]`, calls `QString::toDouble` with a null success pointer for each, and stores the returned doubles in order at `SC+[0x2f0,0x2f8,0x300,0x308]`. The method/thermal selections and later validated convergence fields are handled after this solver branch. The existing candidate already implements source-ordered stores for CG then applies solver/method/thermal fields and can stop on convergence validation; add the explicit case without changing the remaining order.

Bounded implementation proposal:

- Admit the solver6 Apply case only when all four `IterationPage.explicitParameterText` optionals are populated. This is candidate-state admission: it avoids inventing edit text for a non6→6 selection whose initial Fill did not populate these values. It is not a claim that the original UI validates optional existence.
- Set `sc.u2e8=4`; for indices 0 through 3, convert the stored text with existing `qt::toDouble` and immediately store to `sc.solmtd_p[index]`, preserving the original read/convert/store order. Then continue the already implemented method/thermal writes and convergence validation unchanged.
- Keep this private Apply slice independent of the public editor lifecycle. Current session-bound Editor300 still stops before native setup/fill at the palette guard `0x6504abb0`; this projection cannot be called full UI/Open/Save parity.

Focused tests should start from solver6 Fill (which supplies the optionals), replace all four edit values with valid finite decimal texts, and assert count4, all four converted values at the exact offsets, and untouched neighboring fields. A second case should make a later convergence field Invalid and prove that the earlier explicit parameter stores and solver/method/thermal writes remain visible, while the existing sequential validation stop is preserved. A missing optional should refuse before any model mutation; this is the bounded candidate guard, not an original validation result. Avoid claims about arbitrary invalid/overflow text, Qt failure timing, public radio signal delivery, or post-Open radio switches without populated edit text.

Suggested exclusive source/test ownership is `source/apps/moproj2mst/src/engine/simcontrol300.cpp` and a new focused `source/apps/moproj2mst/tests/mdt/test_simcontrol300_explicit_apply.cpp`; no header, owner, lifecycle, public palette, CMake, or GUI change is needed for this private projection. The existing explicit Fill and toggle tests should remain intact.

## Evidence limits

This is a read-only tool-surface and source-evidence audit. No app-control session, GUI, build, test, original execution, or mutable REA operation was used. Release readiness and native public Editor300 completion remain separate blockers.
