# CP161: empty KwOperations Session-route plan review

The proposed headless test increment is source-ready. An existing actual ConversionSession/recording StudyIo fixture already reaches the contextual Type2 lazy reader. Add the exact childless KwOperations case through that route; no production code, successful parser return, codec observation or public Save admission is needed. The important assertion refinement is that fresh header creation has already invalidated sample flags, while later sample/PAR I/O and payload loading remain unreached.

This is an independent read-only review of `resume161-acceptance-next.md` and the current reader/fixture. No source/test edit, compiler/build/test execution, GUI, live REA or mutable database operation occurred. Existing source hashes below bind this review, not a future test implementation.

## Actual call chain and fixture feasibility

The public conversion driver constructs its Session at `src/core/convert.cpp:205`, sets `ctx.session` only for WRITE_SAVE at line209, and binds the actual StudyIo to the Session notification owner at line221 and full-profile model manager at line225. After Open it activates SUB0 at line249. `activateSubProject` calls `parseSubProject` when the selected SUB is unloaded (`src/model/subproject.cpp:355`).

Within actual Type2 parsing, `subproject.cpp:385-392` requires the owned Study alias and configured StudyIo, constructs a borrowed InputTableNotifierContext from the actual Session/Project/SUB/Study/I/O identities, and passes it into the five-argument readLazyStudy when `ctx.session` is present. `activation.cpp:42-51` updates the Study name, reads the named Control/DoeInputTable.XML, copies that per-call context into InputTableSource and invokes readInputTableFromSource. Source lifetime is per-call; nothing needs to be retained on the Study.

The existing `tests/doe/test_keyword_notifier_entry.cpp:49-92` Rig already constructs a real ConversionSession, uses its Project, binds recording StudyIo to both canonical Session owners, creates and selects an owned Type2 SUB, seeds the named InputTable, sets `Ctx.session` and calls parseSubProject. Reusing that exact route is preferable to directly invoking the private Keyword helper with nullptr. It is a test of the real contextual reader subchain, not execution of the entire public converter/Open/activation path. That distinction must appear in its receipt.

The empty branch requires no actual notifier call, so an inert/unavailable process-codec owner is sufficient; do not inject Null codec knowledge to make the fixture run. A normal existing Session fixture is feasible. A full-profile explicit ProcessCodecOwner binding may be used if desired, but does not add evidence about native codec state. Bind notification/model owners before seeding the observation baseline; do not replace a bound notification owner later.

## Exact encounter and refusal

`keyword_reader_prefix.cpp:30-49` selects only the retained TLOC attribute/text shape and a KwOperations node with no attributes and **children.empty()**. Parsed childless DOM is the contract; a comment, raw text, CDATA, PI, unknown child or null child slot is not an empty operation list. Use exact childless XML for the positive test, with the existing admitted header mode0/dimension3/profile and fresh canonical Item list.

After named receiver lookup and typed canonical append, lines74-95 execute EmptyOperations: re-fetch the original row name on the same owned Study, verify the appended last Item, write parameter=-1, function=false, enumeration=false and nominalText exactly `TLOC    \t0.1`, then throw M2M_E_UNSUPPORTED at the diagnostic naming **0x140910abf before upper-text probe**. This branch precedes the `source && source->notifier` check at line97.

Consequently it does not enter enterSetOprHasDoeNotifierPrefix at all: no suppression read/return, flag90 reset, key construction, map/list population, attempt insertion or receiver delivery is reached. Do not label this as a successful suppressed notifier call or a validated notifier return. The contextual identity is carried into the reader but its notifier-entry validator is intentionally not invoked on this branch.

The exception prevents `doe_study.cpp:479-480` whole-container reindex and deferred-link resolution, later child dispatch and outer ID finalization. It also prevents the lazy caller's sample loop/current PAR calls at `activation.cpp:53-56`. Ordinary false parser returns are a different route and must not be used as the expected outcome.

## Required acceptance assertions

An actual A,B,A XML sequence through parseSubProject should establish the reader distinction that the direct-helper test cannot: earlier header/Description effects target the last Header B, but Keyword lookup and scalar commit target named owner A. Verify B's reached varId/Description changes, selectedVariableIndex remains1, A receives the sole typed Item and exact nominal text, B's Item list remains empty, and a Description after Keyword is unreached. If testing queued links, place real admitted LinkTo attributes before Keyword and verify the queued links are not resolved; do not manually assign a description to simulate parser order.

Seed **`suppressed=false` and `flag90=true`** for the primary no-notifier test. That catches accidental entry, which would clear flag90 on the existing operation1 path. Snapshot actual owner identity, suppression, flag90, key sentinel and attempt count after fixture setup and verify all remain unchanged. A second explicit suppressed=true profile can check bypass invariance, but cannot replace the unsuppressed test. Existing operation1 tests should continue to prove that this same contextual fixture can enter the notifier and clear flag90 when a real KwOperation id1 is supplied.

Seed sample table/count payloads before the read and assert that they remain unchanged. With fresh A/B header rows, **all three invalid flags become true before Keyword dispatch** (`variable_record.cpp:169`); expecting those flags unchanged would contradict native-qualified prefix effects. If using coherent preseeded owners rather than fresh rows, distinguish that different no-fresh-allocation invalidation profile explicitly. Empty operations preserve the receiver's existing operation union; no operation1 is added.

For I/O, take baselines after seeding the recording store. Assert exactly the named InputTable read and no reads of Samples_LHS.par/Samples_FF.par/Samples_USR.par/DEF_DOE.PAR, no additional text writes or XML writes, and no later loading/replacement of sample payloads. Fixture seeding itself produces textWrites and must not be mistaken for reader output. A seeded current-PAR file makes accidental later access observable rather than silently absent.

Keep the explicit 0x140910abf diagnostic as a required assertion, alongside retained prefix state. A generic unsupported exception could stop earlier and falsely pass no-notifier/no-I/O assertions. Negative DOM-shape/foreign/nonempty-Item cases can reuse existing profile guards without widening them; a bounded focused positive integration test plus the existing operation1 regression is the immediate meaningful increment.

## Save and acceptance limits

The resulting Study has nonempty variables and remains subject to `saveInputTable`'s existing 0x1400d2b60 reconciliation refusal (`doe_study.cpp:449-452`) and the Type2 writer's VariableDimensionReconciliation boundary. Optionally reassert that private Save refusal without effects; do not catch the reader refusal and classify a subsequent save as accepted. No public converter M2M_OK, completed Open, returning native notifier, `.moproj`/XML parity or full Open-to-Save-to-Close claim follows.

This closes candidate lifecycle-route test coverage identified by CP159. It does not move the historical 93 SAME/five DIFF ledger, resolve original DOE action-state mismatches, remove codec/data-owner/refresh/document guards or satisfy the eight objective gates. No original GUI observation is needed for this test-only increment; fresh original acceptance remains separately gated.

## Disposition and reviewed identities

**Source-ready, no blocker for the proposed test-only increment.** Reuse the existing notifier-entry fixture and auto-discovered DOE test target; production behavior needs no change. Root should assign the test file before edits and run the focused empty-reader/notifier/constructor/public-guard group. This review does not itself modify or run that group.

| Reviewed file | SHA-256 |
| --- | --- |
| analysis/resume161-acceptance-next.md | 03362cc8cd9bd60914348fac739aeb6073730a7e32abe583b7457bd1c90357f2 |
| src/doe/keyword_reader_prefix.cpp | 77c6b47b6a126dfc1566c3ea86eea28b1df46fcefecad6846657dd3bc7ff3027 |
| src/doe/activation.cpp | ffb76fcfb6c768bcf15ae77480ab781b5dca0c9594d1f42a6822b6131eb850d1 |
| src/model/subproject.cpp | 9d2ea605eaeb092019405ecb5cbfaac81c2031aad1ed034da6f1fc722d1975d7 |
| tests/doe/test_keyword_notifier_entry.cpp | 7458a06ddddf2b80d090da5c45bc1a4e5ae994fc2e7ee6a2f0bd6c1f0b6e4b6a |

Source/test paths in the table are relative to `source/apps/moproj2mst`. CP159's retained source review supplies the earlier native qualification and direct-helper limitations; this independent review verifies the current Session call chain and concrete headless fixture feasibility rather than introducing a new native behavior claim.
