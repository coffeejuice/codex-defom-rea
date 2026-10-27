# CP529 internal object-row port identity

Native Forming2D expansion rows with slot code -2 or a defined object own
one optional Object2d input and one optional Object2d output port. Code -1
Undefined has neither. Native `140082760` creates a missing input first,
then a missing output; repeated ensure calls preserve existing identities.
The row accessors expose temporary zero-or-one-element lists. Each port's
selected capacity is one, with ordered reciprocal peers initially empty.

Implement an internal model owned by the actual FlowItem instance and
indexed by object slot. Operation_Id is not item identity because duplicate
IDs are possible. Keep port handles stable across expansion-vector copies
and row-container growth; rendered ExpansionRow remains value-only. Keep
FlowItem prev/next as projections of the main Control2d link. The m2m C ABI
and current Save XML schema remain unchanged: native Save serializes
expansion rows, not row-port peer edges.

This checkpoint only creates and preserves port identities during
buildExpansion for mapped -2 and defined slots. It does not connect row
ports, dispatch notifications, create editor graphics lines, lift CP528's
selected multi-item guard or claim fresh original Open/Save/Close parity.
Those effects require ordered mutations and actual notifier context in
subsequent work.

Original immutable GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP528 independently verified native row input/output accessors
`140187fd0..1401880e0`, setters `140187940..14018798d`, and port creation
`140082760..1400830ff`; hashes and qualifications are in
`analysis/cp528-defined-object-row-port-guard-contract.md`. CP527 verified
reciprocal connector link/clear and ordered conditional callbacks, retained
in `analysis/cp527-read-from-db-row-port-guard-contract.md`.
