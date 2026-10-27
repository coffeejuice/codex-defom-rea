# CP533 private ordered Read From DB row connection

This synchronous diagnostic starts only after an explicitly qualified
native row branch and already-established main Control2d source→target
edge. Both FlowItems must belong to the same live ConversionSession Project,
own stable capacity-one Object2d ports at the selected object slot, and the
target rendered row label must be exactly `  Read From DB`. The literal
source-occurrence string and nonblank qualification are a private caller
declaration, not evidence that public Open reached the backend gate.

The selected native sequence in `140075050` is: clear target input at
`140075e7c`; clear source output at `140075eaf`; enter Destroyed2 on target
input at `140076101`; after a normal return, link the two cleared ports at
`140076148`; then enter Created2 on that input at `1400763ae`. The private
function follows that sequence using CP531 reciprocal clear and CP532
actual-owner notifier prefixes. If Destroyed2 is unsuppressed, `flag90=false`
is retained and execution stops before signal-key construction with both
row ports cleared and no link. If the actual owner is suppressed, both event
callees return with no notifier-map/callback effects and the reciprocal link
is installed. A future qualified Created2 unknown-receiver path may retain
the already-installed link. Invalid entry state refuses before mutation;
this is an adapter boundary, not a native rejection.

Focused tests cover suppressed complete topology, unsuppressed partial
clear/reset with an existing reciprocal peer, owner-map preservation,
and rejection of changed target labels or absent main edge. This private
row-only helper neither represents the earlier main-input Destroyed2 event
nor changes the public selected Read From DB graph guard, Save, C ABI or
Close. Current production state still lacks an observed whole-model+0x18
backend pointer and qualified graph receiver population. Fresh original
Open/Save/Close parity remains unverified.

The immutable GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP527 independently verified complete native graph connect
`140075050..1400763fd` hash
`66107ba099b8b6517dc8e6308c74bd6a62e29fa0069a43f2df863ca845133b35`,
connector clear `140161f90..14016205d` hash
`bf6a1899ea74f8bf45df72dc27c93177f7ce263c744969487dbda1a64a93895f`
and dispatcher `1402cf3c0..1402cf738` hash
`d1ed25625be971f56a377e5620ad47bf020b8499c30500e61e15977379e33b5`.
The retained `evidence/gui_mo/s1/s1-01/dc/diag2.c` call sites and
`evidence/gui_mo/strings.txt` bind exact event keys; PE instruction bytes
remain the controlling source for the selected sequence.
