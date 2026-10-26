# CP124 independent review: Type2 Constraint XML prefix

## Result

Within the frozen implementation's admitted owner profile, the serializer follows the retained native field and child sequence. The new lifecycle test covers ordered rows, duplicate IDs, prior-file retention on open failure, accepted XML on post-close error, and truncation before an unsupported owner refusal. I found no source mismatch for the positive-zero records produced by the qualified Type2 reader. One test-coverage distinction should remain explicit: the four native `setAttribute` calls are ordered, but serialized attribute iteration is governed by the existing Qt4-compatible hash-order serializer; a test of parsed fields is not a textual insertion-order test.

## Native contract compared

The retained original writer evidence summarized in `analysis/resume123-doe-active-next.md` (SHA-256 `A3D21A9260E01134881ECC951D46EAC0C3F3FDD3FBFFA01911492B6DE67567FC`) places `Constraints` after `Operations` and appends each `Constraint` row in manager-list order. For each row it reads `Id` from record `+0x04`, `Value` from `+0x18`, `ObjectNo` from `+0x08`, and `RegionNo` from `+0x0c`; it issues `setAttribute` calls in that sequence. It neither sorts/deduplicates nor reads the source-unwritten `+0x10` field. The selected positive-zero profile serializes as `Value="0"` under both qualified QtXml format branches.

Candidate `appendConstraintElements` is called immediately after the `Operations` child, keeps vector order and duplicates, accesses those same concrete fields, and issues the four `xml::setAttribute` calls in the native order. It does not read or synthesize `sourceField10Bits`. Its positive-value admission (`value == 0.0`) is deliberately narrower than arbitrary native doubles; the actual lazy-read records in the fixture have exact positive zero, so this guard does not alter the tested accepted path. As in the native path, XML output goes through the existing serializer rather than hand-built attribute text. That serializer replays Qt4-compatible hash ordering; therefore call order is source evidence, while literal attribute serialization order is not a reliable parity oracle.

## Open, failure, and continuation ordering

The implementation keeps the device open/truncate before building the DOM. For a coherent empty-variable Study with valid rows, it constructs the document, streams it, explicitly closes, observes the post-close error, records the normal writer return, and then reaches the unchanged table-refresh boundary. The test verifies duplicate ordered rows and actual recorded spans separately from physical durability. It also verifies:

- Open false leaves prior bytes and returns normally with `writerSucceeded == false`, after which the same refresh-boundary refusal occurs.
- Post-close error 7 retains the accepted XML, records a normal false writer result and sticky error, then reaches the same refresh boundary.
- Count mismatch, null row, and negative count are candidate-corrupt-owner guards after open/truncation and before serialization/normal-return/refresh. These artificial states are not claimed as native input cases.
- Empty constraints leave the prior complete empty-variable bytes unchanged.

This matches the source-closed boundary: the writer's normal false return does not suppress the later refresh dispatch attempt; the unavailable native receiver remains the next stop. The candidate preserves actual record and Study identities and does not replay the immediate XML bytes as a separate `FileEffects` write. No ordinary state Save or project DOM publication follows the refresh refusal in this fixture.

The one new test's `REQUIRE(attempt.closed)` on the post-open invalid-owner case is consistent with the recording device's RAII cleanup path, but that test does not qualify native exception cleanup schedules for impossible/corrupt owners. It should remain labeled a candidate guard case, not a native failure-matrix observation.

## Test assertion boundary

The positive-row test checks the four parsed attribute values and count, plus child position/order. It does not assert that the serialized byte sequence has `Id`, then `Value`, then `ObjectNo`, then `RegionNo` textually. That is appropriate unless a separate native QtXml output-order witness establishes such a byte order: the recorded call order does not establish QHash iteration order. If exact XML bytes are asserted later, compare against the existing Qt4-compatible serializer contract rather than a presumed `setAttribute` insertion order.

The reported five cases and static preflight are not a compiled/tested result: CP124's source owner explicitly left build and CTest to root. I ran no build, tests, GUI, original process, or mutable REA action.

## Frozen source identity

Reviewed candidate hashes from the CP124 source-freeze receipt `analysis/resume124-doe-constraint-xml-implementation.md` (SHA-256 `31703B38F2E87A32D80AC4E2EF6A04BB99AEAE3C3C07AF9F885C758CDCF443F3`):

- `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp`: `948D8FD0345E2534F797444F109DF71CF079723A72C1A671F74CFC4C42EC7ADB`
- `source/apps/moproj2mst/tests/doe/test_constraint_xml_prefix.cpp`: `70C6BA58C1E04A927F483A957B471727BE6A460E3008745CD6EAE28802738EE5`
