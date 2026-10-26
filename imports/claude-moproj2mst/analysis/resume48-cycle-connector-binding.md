# Checkpoint 48: Cycle corpus fixture binding

## Exact disposable input

The retained `runner/cases.tsv` resolves case **51_MO2_Operation_Cycles2** to:

`C:\Program Files\SFTC\DEFORM\v11.0.2\2D\2D_Examples\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\MO2_Operation_Cycles2.moproj`

The corresponding saved MST is `MO2_Operation_Cycles2.MST` beside the project. The installed project file exists at that exact path and its SHA-256 at inspection was `6E6AE059FA8460D681B4492885A9ACD64C79DDB84858C85C6ABBF78B23EB467E`. Treat it as immutable input: make fresh disposable copies for any original/candidate pair and retain dependencies from the copied-files manifest. The case row reports 30 copied files (538,484 bytes), seven skipped files (440,820 bytes), and one required project reference `Objects/Object00005.KEY`; review the runner case manifest before deciding which dependencies are needed by the chosen lifecycle path.

## Does this fixture exercise Cycle's corrected connector flags?

**No.** The inspected XML shows three DiagramItems in document order:

| Index | Operation id | Class | Connection request | Group membership |
|---:|---:|---|---|---|
| 0 | 1 | `SfOprForming2D` | to index 1 | outside Cycle group |
| 1 | 2 | `SfOprForming2D` | none | inside Cycle group |
| 2 | 3 | `SfOprCycle` | none | group bounds cover row 0, column 1; `CycleCount=9` |

The parser attempts only edge `SfOprForming2D -> SfOprForming2D`. It does not request an edge to or from the Cycle item, so Cycle's `0x301b` connector flags do not affect admission. The project is useful as a complete Cycle serialization/lifecycle fixture (one cycle, one member, nine repetitions) but cannot validate the corrected combined-connector initializer. This refines the prior resume47 recommendation: case 51 was identified correctly as a Cycle lifecycle case, but its actual graph does not test Cycle connector admission.

It also gives no evidence for DoeOutput. The complete-project DoeOutput sample identified in `s1-01/spec.md` is case 52 BEND_2D, but the retained analysis says its DoeOutput is not in the flow chart and has no DiagramItem; therefore it cannot exercise DoeOutput connector admission either.

## Fixture for the corrected-flag behavior

Use the already retained source-level parser case `cycle_and_doe_output_have_original_combined_connectors` in `source/apps/moproj2mst/tests/p03/test_connector_contract.cpp` for direct flag-dependent graph admission: it constructs the real registry and tests each class between 2D and 3D Forming nodes. For an original-versus-candidate full-project comparison, create a separate disposable derivative of case 51 under the authorized fixture workflow, adding only a Cycle bridge with explicit incoming and outgoing edges and preserving a base copy. Record it as a synthetic modified-project case, not as corpus case 51 parity. Run original first and candidate on separate pristine copies, then compare Open state, requested and accepted edge graph, saved `.moproj`/`.MST` plus all side-file hashes/bytes, statuses/failures, and Close/reset state. Since the extra Cycle item may affect cycle-group semantics and writer traversal, qualify those changed behaviors separately; do not conflate a connector admission result with full unmodified corpus parity.

## Relation to the 16-case probe

`analysis/connector-admission-original.json` runs the original 241-byte admission function with mocked type and owner-index callbacks over 16 combinations. It does not read this project or its flags, run GUI Open/Save/Close, or compare outputs. Case 51's unmodified graph also does not invoke Cycle connector admission. Therefore neither item alone validates the corrected Cycle flags end to end; the focused converter parser test establishes the bounded source behavior, and a modified disposable original/candidate comparison would be needed to test integration with the native project lifecycle.

## Evidence consulted

- `source/docs/re/moproj2mst/runner/cases.tsv`, row `51_MO2_Operation_Cycles2` (exact input path, saved MST, dependencies)
- Installed `MO2_Operation_Cycles2.moproj` at the path above (diagram items, connection indices, group bounds, CycleCount)
- `source/docs/re/moproj2mst/s1-01/spec.md` (BEND_2D DoeOutput is unwalked/not in chart)
- `source/docs/re/moproj2mst/s1-04/spec.md` (Cycle serialization contract)
- `analysis/connector-admission-original.json` (probe qualification)
- `source/apps/moproj2mst/tests/p03/test_connector_contract.cpp` (direct Cycle/DoeOutput parser cases)

Read-only inspection only. No project files were copied or changed, and no build, original application, GUI control, or REA database was used.
