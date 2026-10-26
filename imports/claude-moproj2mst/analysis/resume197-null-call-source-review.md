# CP197 null-parent CALL source review

## Decision: PASS for the repaired source

The independently retained generation-two issuance anchor closes the source blocker. The reviewed implementation satisfies the bounded source-only architectural CALL contract. Root still owns compilation, executable regression checks and the successor freeze; this source review does not assert those results.

## Resolved finding: replacement call generation

The initial review blocked a self-authenticating mutable call/slot pair. The repair adds private `issuedNullParentCallOccurrence_` in `doe_task_factory.hpp:97`, outside the public call and slot records. It is assigned only by the CALL commit and revoked on scope release. A replacement public occurrence can no longer appoint its own accepted generation identity.

At `task_factory.cpp:1073`, CALL preparation copies the newly allocated occurrence into a local shared owner before any publication. Publication moves the two optional records (covered by existing nonthrowing move static assertions) and the shared owner (nonthrowing standard shared-pointer move assignment), then updates RSP and phase. No additional allocating operation occurs inside this commit sequence.

Terminal validation retains the existing tuple and slot checks, then compares the current call occurrence against the private anchor using both live pointer and ownership control-block identity (`task_factory.cpp:534..535`). Missing, replaced, copied or imported generation identity therefore reaches the specific `DOE Task factory CP197 requires issued null-parent CALL occurrence` refusal before the ordinary terminal stop. Public predecessor states also reject a retained private issued anchor, so a partial public rewind cannot silently authorize a replay.

The coherent diagnostic rewind now explicitly resets the private anchor through the existing test friend seam together with clearing call/slot records and restoring RSP/phase. The full predecessor snapshot confirms restoration, followed by a fresh coherent resume. This is a diagnostic restoration facility, not a claimed native reverse-execution behavior.

Complete factory snapshots include the anchor value and a separate shared ownership control-block comparison. The added regression at `test_doe_task_factory.cpp:8635` retains the original witness and tests fresh replacement, copy/reinstall, an imported actual generated occurrence, and a missing private anchor. It coherently rebinds the mutable slot in each case, preserving the formerly bypassed tuple consistency. All modes require exact full error-message equality, unsupported error code, and complete damaged-state/count equality for Report, DOE and Custom keys under budgets 0 and 18. The ordinary callee-entry stop cannot satisfy that assertion. The earlier tuple/slot corruption coverage remains intact.

The former blocker is resolved in these exact reviewed bytes. No remaining concrete source blocker was found. The repair and its regression were inspected statically; no executable reproduction or suite was run by this reviewer.

## Checks that passed

Compared the four released source/test files line by line against their exact paths in `build/wave3-checkpoint196-verified/source.zip` (archive paths omit `source/`). Read the revised CP197 plan, conditional PASS plan review, implementation report, CP196 repaired PASS source review and freeze receipt. The receipt identifies source tree `da68d34433002b1e9022a551e075a37d2da4398f3e095954c1d285469ed19de2`; the retained CP196 implementation/freeze satisfies the successor gate, rather than the obsolete proposal-only wording alone.

The source retains private Session issuance and exact first Report, owner/control-block, capability, frame/token/private-allocation and saved initializer occurrence checks. Synthetic standalone null state cannot execute case 17. Unknown and nonnull diagnostics preserve their separate JE/load guards. Existing inherited stack, saved/restored RDI/RBX, task/private backlinks, TEST flags, taken-null JE, sparse fields, reference markers and incomplete-state validators extend to the new terminal.

At budget 18, case 17 prepares a new call occurrence and slot locally, then publishes the two optional records with nonthrowing moves, sets RSP from `S+8` to `S`, and selects `BeforeNullParentHelperEntry`, whose cursor is `0x2aca0`. It leaves the initializer tuple (`0x131417 -> 0x13141c`) intact as generation-one history. `liveReturnSlot()` derives initializer liveness only through its RET predecessor, returns no live initializer at the caller CALL predecessor, and projects generation two/value `0x13144e` after the new CALL. No callee instruction, TLS query, helper result, private `+0x40` store, reference increment, return or completion is supplied.

The phase loop validates before terminal stops under every budget. Existing terminal reentry has no replay. Premature call/slot records and partial rewinds fail guards; the inspected coherent diagnostic rewind clears both new records and restores RSP/phase before resuming. Six new Testkit cases cover the exact normal transition with complete predecessor comparison, initializer consumption, Session versus standalone routes, corruption matrices, budget/reentry and rewind behavior. Snapshots include the new history, slot, initializer/source occurrence and owner control blocks. These source checks do not constitute execution results.

## Evidence limit and reviewed bytes

Read `resume163-connector-qobject-static.json`: its original QtCore listing is a bounded unwind-region static artifact, not runtime reachability or a complete helper contract. Retained lines 44..45 list bytes `E8 52 98 EF FF` at VA `0x67131449`, target `0x6702aca0`, next instruction VA `0x6713144e`. The modeled RVAs therefore match the CALL bytes and architectural return address. No original image was executed or independently rehashed. Callee/TLS/owner/return behavior remains outside this increment.

Working-file SHA-256 at review:

| File under `source/apps/moproj2mst/` | SHA-256 |
| --- | --- |
| `src/doe/task_factory.cpp` | `ddfd6aab190ac0b70105654083ec6fdacb20dac8d16c2d5b22b3f128a37ae11b` |
| `include/m2m/doe_task_runtime.hpp` | `ae71876b4b9a0cf8e425b424cee88935ffcfed63adafb62644a828497b9a0e9f` |
| `include/m2m/doe_task_factory.hpp` | `c3864e45e518f715e4b3ae6d54dbfa28be30683d5293609bc087704be3801894` |
| `tests/mdt/test_doe_task_factory.cpp` | `c41c6b3921528d4b4475169e266e1100f276ed0ab471722e5ad9dd3770b29ed9` |

Only this review is retained as a write. No source/test/build, GUI, mutable REA database, parent/DMO/Claude/original artifact or deployment was changed. No executable test was run.
