# First implementation checkpoint

The converter now applies the original diagram connection admission rules in its Open parser. The candidate builds and all 95 registered converter tests pass. This is a verified implementation checkpoint, not completion of Open–Save–Close recovery.

## Behavioral correction

The inherited parser linked every valid target index and let later sources replace earlier incoming connections. Original constructor/type/admission evidence establishes two checks for this parser loop: connector types must be compatible, and a fresh source output cannot replace an occupied target input. The reconstruction now applies both checks before mutating links. It preserves the asymmetric 2D-input/3D-output conversion class and the first accepted incoming edge.

Independent review found that Cycle and DOE Output lacked their original class flags. Their restored values, `0x301b` and `0x20097`, give them combined 2D/3D connectors. All 28 currently registered reconstructed classes now initialize flags. This is not proof that the registry contains every original class; Group/CopyMirror registration and unknown-class behavior remain separate questions.

Original identity, resolved strings and fixed expectations are retained in [connector-type-contract.json](connector-type-contract.json). The [controlled original-function experiment](connector-admission-original.json) executed 241 exact bytes from admission function `0x140074f50` for 16 combinations. Type and owner-index callbacks were mocked; the experiment establishes admission branch behavior, not original GUI parsing or a complete conversion. Static constructor and initializer evidence supplies the type mapping. See the [independent review](connector-implementation-review.md).

The new regression failed two of its initial five cases before the patch. The final seven cases pass: the fixed 4×4 class matrix, first accepted input, rejected dimension followed by an accepted source, chains through already-used outputs, 2D-to-3D conversion, Cycle/DOE Output in both dimensions, and self-connection admission. Self-loop traversal is not claimed.

## Validation and test repairs

[Final build receipt](wave1-connector-final-v2-build.json): full Release build, source fingerprint unchanged before/after compilation, 2,998 source files. The original baseline binaries remain in `build/baseline`; the implementation candidate is in `build/candidate/bin`.

[Final test receipt](wave1-final-tests.json): 95/95 registered CTest tests passed in 18.750 seconds of test execution; no changed imported fixtures, missing test results or truncated output. Of these, 88 passed without logged gaps, six passed while logging gaps, and one is a diagnostic Save sweep that logs differences without failing. Optional fixture skips, historical reference limitations and permissive ABI assertions are explicitly retained.

The first relocated run stopped six tests on Windows path-handling errors. Test-only changes now compare planned effects without requiring nonexistent files, normalize relative path components, and preserve Windows case semantics. Real output containment still resolves existing filesystem ancestors. Full logs are retained; the earlier 1,024-byte passing-output truncation is detected and rejected by the runner. Failed/intermediate receipts are preserved.

The complete historical Save diagnostic covers 98 cases:

| Historical set | Cases | Comparator matches | Mismatches | Refusals |
|---|---:|---:|---:|---:|
| Corpus re-saves | 49 | 48 | 1 | 0 |
| Oracle B | 49 | 41 | 5 | 3 |
| Total | 98 | 89 | 6 | 3 |

These are inherited comparator results with stated normalization and session-history qualifications. They are not 89 raw-byte or fresh-reference proofs, and 89/98 is not a reconstruction-completion percentage. Seven corpus projects lack a completed historical re-save reference; three declared Oracle-B cases are excluded. The current default-answer policy still needs fresh original observation.

Open mismatches: `RS_55_Multiple_Operations_Example1`, DOE cases 120/121/122, database-step conversion 129, and Diestress case 98. Refusals: Cogging new-operation cases 131/131R and database-step Boolean case 133. Their per-file differences and missing effects remain in the final receipt and complete logs.

## Reference harness and next work

The reference harness freezes fixtures, source and candidate binaries; verifies a supported successful full-build receipt; records original-first chronology and observed default dialogs; and keeps raw comparisons separate from explicitly qualified views. Its 39 focused tests produced 38 passes and one Windows symlink-privilege skip. No full original GUI Open–Save–Close run occurred at this checkpoint.

The prepared [BILLET training case 003](../build/reference-harness/billet-training-003/case.json) passed [preflight](../build/reference-harness/billet-training-003/preflight.json): freeze intact, supported full-build receipt consistent with 2,998 source files and three binaries, and all three selected XML references present. This is local provenance consistency, not independent build attestation or runtime readiness. Earlier preparations 001/002 are superseded and must not be used for acceptance.

The next action is the fresh original-first lifecycle capture using case 003, followed by lifecycle/database-state and geometry recovery. Continue through [IMPLEMENTATION_PLAN.md](../IMPLEMENTATION_PLAN.md) and the [closure backlog](closure-audit.md), preserving original Save failure behavior and the existing C ABI. Complete Save-generated DOE files remain in scope.

All changes and scratch results are confined to this imported converter project. Claude's source/evidence stores and the parent DMO work were not modified. The [efficiency review](wave1-efficiency-review.md) records measurements and limitations without claiming unmeasured speed or token savings.
