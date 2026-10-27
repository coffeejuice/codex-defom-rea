# CP527 selected Read From DB row-port guard

The native graph has main Control2d ports and separate object-row Object2d
ports. `FlowItem::prev/next` represents only the main edge. When a selected
multi-item relayout contains a matched Operation with object ID -2, native
`140073900` creates a “Read From DB” row and its ports. Native `140075050`
can connect corresponding row ports after connecting the main edge; it also
performs ordered conditional Destroyed2/Created2 dispatch. The current
reconstruction cannot represent that extra reciprocal edge, so CP527
returns `M2M_E_UNSUPPORTED` before selected placement mutations for these
profiles. This is an implementation limitation, not native rejection.

The guard uses first-match `operationById` and scans only selected
multi-item graph routes: zero-origin recurrence, nonzero/free/explicit/mixed
pairs, three-item collision/negative/direct, four-to-seven positive or
direct, negative multi and eight-negative routes. It does not globally ban
associated Operations or change first-associated all-Undefined CP526.
Parsed item names, expansion rows and any earlier parse-time effects remain
in reached state; the guard promises no rollback. Defined-object row ports
require a separate audit.

For the minimal matched A/B -2 pair with missing CellPos, A Rect0 and B
`(205,0,0,0)`, the native order is: clear B main input; clear A main output;
conditionally dispatch Destroyed2(B main input); link main ports; clear B
row7 input and A row7 output; conditionally dispatch Destroyed2(B row7
input); link row7 ports; conditionally dispatch Created2(B row7 input);
then the editor adds/shows the main graphics connection. Native callback
delivery depends on suppression and live receiver registrations; no
unconditional runtime callback result is claimed.

Independent DUMPBIN14.44.35228.0 verification of immutable original GUI
SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
covered 15 complete `.pdata` spans, 17,981 contiguous bytes and zero
mapped-PE gaps/mismatches. DUMPBIN emitted 3,540 addressed records,
including embedded jump-table data; these are not all executed instructions.
Connector link `140161940..140161ea2` SHA-256
`1e4449f8184e9f72486d5deea57165ea918b7a90557ca2c43ca70e3c406c3b87`,
unlink `140161eb0..140161f81` SHA-256
`d89ba620861bfc1d642fe3dcc0b6f9c2feb440400219872b64100a7699417cc9`,
clear `140161f90..14016205d` SHA-256
`bf6a1899ea74f8bf45df72dc27c93177f7ce263c744969487dbda1a64a93895f`,
graphics add `14017f600..14017f6bf` SHA-256
`672471792bd698b9b3a95ad1d5a523d7b41a4abf7fdc5708be04b9f201439055`,
dispatcher `1402cf3c0..1402cf738` SHA-256
`d1ed25625be971f56a377e5620ad47bf020b8499c30500e61e15977379e33b5d`.
Qt metadata binding resolves editor Created2/Destroyed2 slots and vtable
`+0x578` to graphics connection creation; actual live registration remains
unobserved.

The next implementation needs stable per-row port identities and ordered
reciprocal peers, plus callback-visible prefixes and a graphics map keyed by
input port. A guard alone does not complete this behavior or original
Open/Save/Close acceptance.

## CP528 integration qualification

The subsequent PE audit found that defined-object rows also own native
object ports, and their labels can equal “Read From DB”. CP528 broadens the
selected-route guard from slot ID -2 to every slot ID other than -1. The
CP527 evidence and original narrower checkpoint remain retained above.
