# CP171 read-only operation/property dispatch and FRCROT next cut

## Decision

After CP164's Type2 FRCROT selector/text handoff, **no later FRCROT file/scanner/table cut is source-ready as an actual owner**. The next useful source change is not justified by the retained evidence. Preserve the hard refusal before converter call `0x14091111d`. The converter and Type3 table projection remain disconnected from the actual file invocation and its native owner.

The non-DOE dispatch audit also finds no new source-ready public operation/property arm. CP165's dispatch report remains current: Distortion `vt+0x198` is already integrated; Report's property `vt+0x60` mapping exists, but its Save `vt+0x68` depends on the unmodeled Report document owner from Open; generic unknown-class and active Report Save refusals remain correct. Therefore there is no safe generic dispatch expansion here.

## Exact evidence and boundary

- `analysis/resume164-frcrot-source-review.md` records the admitted Type2 profile: registry ID 276, empty operations, retained entity `[1]`, invocation-local selector `[1,1]`, dimension 3, parameter `-1`, function true, enum false, and three exact text strings. It confirms a terminal unsupported guard before `0x14091111d`; no file API, scanner, parser, table setter, notifier, or deferred-link completion is reached.
- `analysis/resume164-frcrot-file-next.md` and `analysis/resume163-frcrot-integration-evidence.json` bound the static converter at `0x140916530..0x140917055` (SHA-256 `43aef4d26f957c36cc8858d3b46e67eb54d62b82eaf324fd1047ee153fcc7110`). At `0x140916570` it resolves relative `temp.KWD`; QFile open with mode `0x12` at `0x1409165d8` gates all later effects. On success, QTextStream writes nominal text and closes; then a relative FILE reopen occurs at `0x14091668a`, `%39s` scanner at `0x1409166b1`, and group-3 parser at `0x140916756`. The caller ignores scanner EAX. Upper/lower stages are conditional on preceding parser status. Static code establishes call order only; it does not establish working directory, file identity, codec/bytes, reopen result, scanner destination/cursor, parser outcome, or actual invocation occurrence.
- `source/apps/moproj2mst/src/doe/frcrot_table_prefix.hpp` is explicitly a projection over prepared scan outcomes. It is not wired as the `temp.KWD` file owner and cannot license scanner outcomes or DYNARY changes. Converting that seam into public Type3 table behavior would invent the missing file/scanner owner contract.
- `analysis/resume165-op-dispatch-next.md` records why Report's actual document publication and later owner-bound reads are prerequisites to its Save arm. It likewise confirms Distortion is already integrated and unknown classes must not be silently accepted.

The narrow blocker is same-occurrence native evidence connecting the actual Type2 row through `0x14091111d`, the opened `temp.KWD` file identity and bytes/codec, FILE reopen and scanner buffer/cursor, and parser status. Without that chain, a later table owner cut would be a detached prepared-observation helper, not reconstructed dispatch.

## Shortest integrated tests once the blocker clears

1. Keep the current contextual FRCROT reader test and malformed-profile matrix. Assert the exact `[1,1]` handoff and all retained owner fields, unsupported status before `0x14091111d`, and no file/scanner/table/notifier/DYNARY/sample/deferred-link effects.
2. Once same-occurrence evidence supplies outcomes, add one integrated actual-reader-to-file-owner test for QFile open failure (no stream/scanner), then open success with explicit resolved path/file identity and codec/output bytes, FILE reopen plus `%39s` return/buffer/cursor, and first parser status. Add upper/lower phases only behind their observed parser gates.
3. Only after the file-owner test exists, connect the observed parser output to the smallest Type3 table owner case and assert one actual table mutation plus failure/partial-state ordering. Do not use the standalone projection test as a substitute.

For ordinary operation/property dispatch, retain the current Report refusal tests until Open's `SubProject+0x30` document producer, generation/lifetime, and Save caller path are modeled; then add one Open→property→class-Save order test with same-instance owner assertions. No build or test was run for this audit.

## Scope

Read-only source and retained-report review within `imports/claude-moproj2mst`. No source, executable, test, shared state, build, GUI, live REA, or original process was changed or invoked.
