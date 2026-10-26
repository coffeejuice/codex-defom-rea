# CP112 B120–B122 DOE output seam audit

## Decision

No missing B120–B122 side-file effect is currently safe to integrate as a public output branch without crossing an earlier unclosed state/action or owner boundary. Do not add a new helper or wire existing helpers into public Save. The retained mismatches compare no-study `_before` inputs against later GUI-created after-states; the only post-state candidate comparison (B120) exercised the active Type 0 path and preserved the inactive DOE files. For active DOE input, current source stops before complete variable/geometry import or at the Save reconciliation/refresh boundaries.

## What the raw comparisons establish

- **B120:** the `_before` project is a Type 0 project with no DOE study. Its expected after-state adds Type 2 `DOE Study 1` and seven Control files. The old disposable candidate post-state comparison began with all seven files already present and preserved them unchanged while only rewriting view data in `.moproj`; it selected subproject 0, so it did not load the Type 2 study or regenerate any DOE output. This is preservation evidence from an older candidate build, not a current active-DOE Save acceptance result.
- **B121:** the comparator reports one `.moproj` difference and eight missing expected outputs (`.DS`, `.MST.TEMP`, `DEF_DOE.INI`, current/sample PAR, parser INI, InputTable XML and `temp.KWD`). Its `_before` project has no Type 2 study, variable or DOE directory; the after-state contains a TLOC variable and GUI-created sample/control state. The retained raw comparator does not include a candidate byte dump, so there is no exact byte range to repair from this diff.
- **B122:** the comparator reports one `.moproj` difference and ten missing outputs: seven DOE control files and three `.GEO` files. The `_before` project has no study, geometry variable or DOE Output operation. The after-state includes all three. The retained original notes say the geometry “+” action copied the GEO variants before Save. The historical folder diff does not establish that public Save generated those copies.

These raw results remain useful historical transition evidence. They do not establish that the expected after-folder is the output of Open–Save from the corresponding before-folder.

## Newest owner/effect evidence and reachable boundaries

CP112's owner review separates current paths:

1. A no-variable Type 2 study reaches the candidate XML open/write/close and `NormalWriterReturn` journal, then stops at `write.cpp`'s `requireRefreshDelivery`. Native current-PAR and fixed sample outputs follow the synchronous `DoeTableRefresh` dispatch. No candidate-side normal callback return exists, so adding another PAR writer test would be detached from public Save.
2. A nonempty-variable Type 2 Save stops earlier at `VariableDimensionReconciliation`; native `0x1400d2b60` operation-manager/reconciliation work precedes prune and InputTable output. A Study-local prune or writer cannot be inserted ahead of that missing stage without changing native ordering.
3. CP113 now qualifies the B122 manager→Var→geometry-row ownership, Geometry filename argument/path construction, and metadata append. It still does not find a later native open/read consumer for the composed GEO paths. The candidate `writeB122GeometryVariablePrefix` is a retained-fixture metadata serializer that emits InputTable bytes to `engine::FileEffects`; it neither runs through actual Type2 Save nor creates/copies the three GEO payloads. It already has an exact fixture test, so another serializer/test would add no source-closed public effect.

The exact public Save guard is occurrence-dependent: for an empty-variable study, the XML writer can return true or ordinary false and the next native event is the notifier dispatch; for variables, reconciliation must be closed first. The fact that InputTable XML is written before refresh for the empty-variable branch does not make that writer a fit for B121/B122: their after-states contain nonempty variables, and their `_before` files have no study at all. No currently reachable source-closed output effect accounts for these raw deltas.

## Next evidence needed

Use an input that already contains the desired state and capture a fresh original/candidate result with the active Type 2 path selected. That is necessary to distinguish input-action mismatch from Save output. For B120's zero-variable state, first close the live table receiver/registration/refresh normal-return owner after XML. For B121, close the source-qualified variable/Keyword import and the earlier operation reconciliation before expecting variable Save outputs. For B122, close the actual filename/QFileInfo and metadata import continuation, then identify the later GEO payload consumer before claiming GEO file effects. Preserve the existing guards until those dependencies have owners and ordered failure behavior.

## Evidence and limits

Reviewed retained raw-comparison reports: `analysis/resume48-diff-b120.md` (SHA-256 `C0162204C72702E002DD0A5B00E51B8E8EE95118FFADC83BC274803C4BDA62DC`), `resume48-diff-b121.md` (`B3C9641390E99E0C192C93C5138143FFC29F9E0979878A24D9978BF62448598E`) and `resume48-diff-b122.md` (`31171C5EA32B56CC647576A7711AC5F8A2369CE165C22A211038896946CED038`). The B120 post-state candidate result is `resume49-b120-poststate-result.md` (`441CCC508E1C06C7E9CA7E7BA3131B3931E72D9494406873B29419DEF0159AB3`).

Reviewed newest owner evidence: `resume112-doe-save-callback-owner-next.md` (`5761A73A80CA168FCC15D131FF022B26E3DA099EB990E54926C3F31573D2DE7E`) and `resume113-geometry-owner-and-path.md` (`DA48036ECF110D814BF40C9EC1E05ECD4D972533A8EA9EEE242D2F7374B20F07`). Candidate serializer source/test were read only: `geometry_variable_writer.cpp` (`A425514DCE9B4CC42F563F4CE04CD63F27CF03B4C8B05CAA2B00C23C388592D1`) and `test_b122_geometry_variable_writer_prefix.cpp` (`EEFCB1935986CC8ED6CEF452D901512BFC9218F0FC7903CD5F5D78DCDDD770EE`).

No source/test edits, build, GUI/original execution, or mutable REA database operation was performed.
