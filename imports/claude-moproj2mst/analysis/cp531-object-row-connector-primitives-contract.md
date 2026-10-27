# CP531 Object2d row-connector primitives

Native `140075050` clears the target input before the source output, then
conditionally emits Destroyed2, links the pair through connector `+0x78`,
and conditionally emits Created2. `140161f90` / `140161eb0` remove reciprocal
peer entries when either endpoint is cleared. CP531 adds the internal
`clearObjectRowPort` and `linkObjectRowPorts` primitives to the stable
CP529 item-owned port identities. The focused test checks reciprocal peers,
target/source replacement and clearing one endpoint. It does not connect
these ports in public `parseSubProject` or change the m2m C ABI.

`linkObjectRowPorts` performs the clear and link sequence only for already
qualified Object2d capacity-one output/input pairs on different FlowItems.
Its atomic call is suitable only when the intervening native Destroyed2
dispatch is known to be omitted or has been handled separately. No current
production call site proves that condition. The CP528 public guard therefore
remains. Invalid or unqualified endpoints return unsupported before mutation;
this is an adapter boundary, not a claimed native refusal.

The original GUI is SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP527 independently checked connector link body `140161940..140161ea2`
hash `1e4449f8184e9f72486d5deea57165ea918b7a90557ca2c43ca70e3c406c3b87`,
unlink `140161eb0..140161f81` hash
`d89ba620861bfc1d642fe3dcc0b6f9c2feb440400219872b64100a7699417cc9`,
clear `140161f90..14016205d` hash
`bf6a1899ea74f8bf45df72dc27c93177f7ce263c744969487dbda1a64a93895f`,
and graph connect `140075050..1400763fd` hash
`66107ba099b8b6517dc8e6308c74bd6a62e29fa0069a43f2df863ca845133b35`.
These are static PE qualifications, not a fresh original Open/Save/Close
comparison.

The CP531 notifier audit found that the current actual shared
`SessionNotificationState` defaults to unsuppressed and has no qualified
Created2/Destroyed2 receiver lists. The original editor constructor
registers both receivers. A constructor-time null backend pointer does not
prove its value at graph relayout. Therefore the next increment must model
the ordered notification prefix with actual owner identity and a qualified
current backend/receiver state, stopping before unknown live callbacks.
The public selected-route guard cannot be lifted from this checkpoint.
