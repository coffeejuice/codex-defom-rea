# CP255 SPRING label implementation review

**PASS — implemented owner boundary, deferred caller and focused verification match the CP254 contract.** No required corrective change was found. This reviewer inspected existing evidence and hashes; no build/test execution was performed.

All five current source files and three selected executables match every SHA-256 in `resume255-spring-labels-hashes.json`. Its after-test tree hash matches the final `cp255-spring-labels-focused-lf-build.json` before/after stable source hash. Final configure/build exit codes are zero. Use this final LF receipt, not only the earlier superseded build, when binding this review.

## Source and guard order

The general header forward-declares only `SpringComboLabels` and adds the reference-output const method; the model implementation includes the complete IO record. The old lifecycle helper is gone. `copyEditorSpringComboLabels` retains one unguarded count read, range refusal before reserve/traversal, local initialized labels, reserve, and formatted zero placeholder.

It requests numbers 1..count using `this->object(i)` once each, refuses null, skips kind 7 before name access, scans +0x828 for NUL before 0x54, reads signed object number +8, checks printable ASCII, and formats the exact `%-3d - %.*s` into 64 bytes. Negative snprintf length or length >=64 refuses. Order/duplicates and empty names remain intact. Successful `labels.second = labels.first` precedes the sole output assignment; both default indices are zero. No native pointer/name view escapes.

All ordinary false paths leave result untouched because they modify only locals. Allocation/other ordinary exceptions still propagate; no noexcept/catch/transaction promise was added. Existing objectCount remains unguarded and nullable; existing numbered object lookup admits geometry before nullable resolution. Thus count-zero/absent step returns placeholder without a geometry check, invalid count refuses before lookup, and positive parked count throws at the first object request. The method does not substitute ordinal traversal or issue an incarnation identity.

## Deferred caller and fallback

The lifecycle Extra3D branch retains an explicit optional-returning lambda. It constructs local labels only when invoked, returns nullopt on false and completed owned values on success. It is not eagerly called at initial editor admission or capture construction.

The unchanged IO implementation invokes deferred labels after STRAIN_DST following earlier scalar reads and before readSprings. Producer false still removes actual-label context and follows the existing later SPRING/legacy fallback rules; it does not immediately refuse the whole page or skip the spring read. Geometry exceptions propagate instead of becoming nullopt. The retained ordered-I/O caller test creates objects during the last scalar read and checks subsequent IDs10/20; an eager extraction would fail it.

## Tests and receipts

New direct cases meaningfully check zero-count and **explicitly reset absent-step** placeholder success; both catalog vectors/index reset; dense numbered ordering; explicitly seeded kind-7 skip despite malformed name; owned strings surviving native name changes and step destruction; partial accumulation followed by a missing number; invalid reported counts with metadata restored; nonterminated/non-ASCII names; formatted length64 refusal versus length63 success; empty name; and last-scan-index NUL followed by independent formatting refusal. Every false case uses distinctive vectors and indices. Signed-number formatting and the upper count acceptance are source-preserved rather than exhaustively runtime-enumerated; no blocker follows for this literal move.

The parked viewer addition requires positive count, invokes the command through the existing unsupported-error helper and checks all output fields remain sentinels. It directly catches an omitted per-object guard. There is deliberately no parked-zero runtime fixture; the receipt correctly qualifies that outcome by source inspection. Last-scan-index NUL refusal does not independently prove the reason for refusal by its final result alone; source inspection distinguishes valid scan followed by buffer-cap failure.

The final elevated focused log records lifecycle, viewer and SPRING IO tests 3/3 passing. The earlier sandboxed SPRING IO run failed fixture directory creation; the receipt preserves those access errors and distinguishes the authorized rerun rather than calling the first run a pass. Binding/include preflight reports no issues. Full source-stable integration/freeze remain root work; no full validation result is inferred from the focused run.

Existing tests establish output/refusal behavior and deferred observation, not every internal operation count, actual Qt insertion or all exception allocation paths. Single count/per-number lookup and formatting order are established by inspected source. No production observer is needed for this extraction.

## Evidence limits

CP120's placeholder reconciliation controls over CP119's superseded bare-None statement: the captured formatter produces `0   - None`, and selection uses visible text. Count/NUL/ASCII/buffer caps remain reconstruction admission rules, not original safe limits. This review introduces no fresh original process observation, new recovered symbol, arbitrary encoding proof, physical/reentrant Qt witness, ObjectKey/ownership closure, public editor/Save completion or cache1/accepted-Close authority.

Only this review report was written. No source, tests, builds, shared recovery state, GUI, original process, or mutable REA state was changed.
