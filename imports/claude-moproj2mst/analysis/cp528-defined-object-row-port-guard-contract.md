# CP528 defined-object row-port guard correction

CP527 guarded selected multi-item graph routes only for mapped -2 Read From
DB slots. The original also creates object-row input/output ports for
defined-object slots. A defined object's text label can equal “Read From
DB”, causing the same secondary-link branch even when its object ID is
nonnegative. CP528 therefore guards a selected multi-item graph route when
any matched Operation's slot ID differs from -1. The existing CP526
all-Undefined (-1) route stays admitted. This is a conservative
reconstruction limitation, not a claim that native rejects those objects.

The original row owns one optional input and one optional output port;
native getters expose temporary zero-or-one lists. Port creation preserves
existing identities on repeated calls. Each selected Forming2D port has
capacity one and an ordered reciprocal peer list. A row labeled exactly
“Read From DB” can trigger target input/source output clear, reciprocal
linking, and conditional ordered Destroyed2/Created2 callbacks after the
main Control2d link. Ordinary defined labels still have unlinked native
row ports, which the current model also omits. Stable row-port state is a
separate CP529 implementation task; `FlowItem::prev/next` stays a projection
of the main edge.

CP528's source-bound regression exercises -2 and defined slots with
ordinary and “Read From DB” names across selected sentinel, explicit,
direct and recurrence profiles. Guarding occurs before placement mutation;
parsed names, expansion rows and earlier parse-time effects remain reached.
The guard does not globally ban associated Operations and does not establish
fresh original Open/Save/Close parity.

The immutable original GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP527 verified graph connect `140075050..1400763fd` SHA-256
`66107ba099b8b6517dc8e6308c74bd6a62e29fa0069a43f2df863ca845133b35`.
The CP528 PE audit independently resolved row input/output accessors and
setters, port creation `140082760..1400830ff`, and defined-object branch.
Eight new complete `.pdata` spans covered 635 contiguous bytes and 149
addressed records, with zero gaps/mismatches; two port-setter leaves were
29 bytes each. Defined slot record lookup `140061250..1400612c3` hash
`7a3180257a9c66683fbb54ed3379014dab5efdad20d15336a9a80314567f9b04`,
record-name getter `14004d730..14004d76f` hash
`97ae0f6722e123583a2e6b88977b6924c8d6e1f7eefce90337cef7c0ec7ba6f9`,
row input getter `140187fd0..140188050` hash
`a60edf10e58cea138c48e56c723b9c2da70c3ede028133c84d8055495ce82071`,
output getter `140188060..1401880e0` hash
`47d626449548feff0f821c1220dbbe30d11f42c693fd903e08dd487a5ba203a6`,
input setter `140187940..14018795d` hash
`8d657731ecf1f330116a96a7f1af77f2cd2e5ef48ab2839ce7170a2b57b82c28`,
output setter `140187970..14018798d` hash
`6093822151b3791017acf3c9a60a9ca9a4eeca53ec7ee48fd04c3f4fbfba0aca`.
Normal defined labels leave row ports unlinked; a target label exactly
`Read From DB` triggers CP527's secondary link. Native Save serializes row
count and labels but not these row-port peer edges.
