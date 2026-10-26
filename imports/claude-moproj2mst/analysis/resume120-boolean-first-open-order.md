# CP120: the concrete tree-item data slot does not establish Boolean Apply

The default delegate's role-13 request now has concrete item producers. The inspected object-tree construction routes install 16 item tables whose data slot all forwards to Qt's stored-role getter. None supplies an application data override that calls Boolean Apply. This closes a specific callback hypothesis left by CP117; it does not establish that a layout timer ran during Open, or exclude effects through later style, geometry, filter or widget callbacks. Preserve the public first-Open/PostPending guards.

Only this report was written. Local immutable PE reads and DUMPBIN 14.44.35228.0 ran; no original process, GUI, build, tests, source edits or mutable REA analysis was used. One read-only REA binary_session diagnostic responded successfully with open=false and cleanup_pending=false. The earlier connection failure did not recur on that call; this does not assert an analysis provider or target was reopened. Further analysis used local files.

## Concrete item ownership, distinct from the internal node

Boolean objectTreeFill1411df400 uses1405eea60, which binds/fills the internal node collection through1400c3980. Growing the collection calls1400c3af0, allocates0x30 and constructs1400c0290. That object calls QObject's constructor and installs1421573c8. Its +8 must not be mistaken for QStandardItem::data: it is an internal QObject node, distinct from a Qt model item.

The Qt item route is base objectTreeAdd1405f15e0. After locating the custom type0x3f1 parent item, its node loop calls1400c4a10 at1405f1996 for a missing object row and1400c7200 at1405f1940 for an existing row. The former creates the primary object item via1400c5d40 at1400c4b6e and then conditionally creates node-property children. The latter updates or creates those property children according to the internal node flags. The retained Boolean fill path thus supplies a source-conditioned producer chain, rather than a runtime assertion that a particular item exists on first Open.

In1400c5d40, primary constructor140d4c050 is called atc5db0. It installs142a14b48 atd4c07c/83 after its base constructor. The companion column item is allocated0x10, calls imported QStandardItem(QString) atc5f6a and installs14210eb18 atc5f75/7c. Both tables have +8=140138a24. The companion is attached through QStandardItem::setChild atc5ff2; parent insertion uses appendRow or insertRow atc5ee9/c5f05.

The property producers inspected from1400c4a10 and1400c7200 call constructors below. Each constructor installs the listed table, and every listed table has +8=140138a24. Companion text items in these producers use14210eb18 as above. This is a bounded producer census, not proof against a later item/table replacement.

| Constructor | Installed primary item table |
| --- | --- |
|140d4c050|142a14b48|
|140d4c530|142a14b98|
|140d4c6c0|142a14be8|
|140d4c850|142a14c38|
|140d4c9e0|142a14c88|
|140d4cb70|142a14cd8|
|140d4d020|142a14dc8|
|140d4d1b0|142a14e18|
|140d4d340|142a14e68|
|140d4d4d0|142a14eb8|
|140d4d660|142a14f08|
|140d4d7f0|142a14f58|
|140d4dca0|142a15048|
|140d4de30|142a15098|
|140d4dfc0|142a150e8|
|140d4d980|142a14fa8|

## Role13 reaches storage, not an application override

The six-byte thunk140138a24 jumps through IAT141e0b7b0, named QStandardItem::data(int), to QtGui65529050. Together with CP117 this gives the conditional chain:

QStyledItemDelegate::sizeHint6553c1c0 -> QModelIndex::data(role13) at6553c1fa -> fresh model data65529fd0 -> actual item's +8 at6552a069 ->140138a24 ->65529050.

The last getter reads item-private+18, scans its stored role/value vector, and compares each role at65529096. Only role2 is remapped to0 at905c..9064; role13 remains13. A missing role creates an invalid QVariant through import65696990 at90a7. A match copies the stored QVariant through656966e8 at90bd. PE import names identify these as QVariant() and QVariant(QVariant const&). There is no item virtual, signal emission or native Apply/add call in this getter body. The actual stored value/type is still an input: no absence of role13, QSize value, or arbitrary QVariant handler behavior is invented.

Accordingly, CP117's valid-QSize return suffix can finish without application Apply after this getter returns. Invalid data instead continues to delegate style-option initialization6553bbf0 and actual style+e0 at6553c303; non-QSize valid data takes the conversion-handler branch6553c259. Those are separate conditional boundaries. A size calculation never serves as evidence of an Apply occurrence.

## The two direct uniform-height calls do not establish a same-tree write

The true call at140dc23fa belongs to140dc1e00..140dc71cf. Its receiver is this setup owner's +18 field. That field receives a separately allocated0x28 object: imported QTreeView constructor atdc22c4, installed table14219b8a8 atdc22d2/22d9, field commitdc232d. The call passes dl=1 atdc23ec. This producer differs from the SfTreeView0x38/table142a0b158 producers committed to dock UI+50/+1c0 in CP117; no same-owner alias is established.

The other call141345532 passes edx=0. Its enclosing constructor141345390 calls imported QTreeWidget and installs142dc06e8. It cannot prove uniform heights enabled for the SfTreeView receiver. Thus constructor-produced private+420=0 remains the conditional baseline from CP117, while an actual later same-owner writer remains unobserved. This is not a global search proof against indirect/property writes.

## Exact occurrence gap and next action

The first temporal gap remains the actual zero-timer registration/dequeue on tree T before the selected Open completion. CP116's dispatcher sends the normal timer event at67142315 to the live record's object+10; matching layout timer654a8956 still needs its widget-data branch8978 and layout call8998. Static timer registration, matching numeric IDs or default item tables cannot supply that occurrence or T's lifetime.

For a positive creation claim, the minimal retained frame must additionally connect a nested actual widget-area UpdateData to current Boolean editor E, Apply1411e2870, E+38's resolved OPI O, and the normal add return1411e295a or1411e2bcb with the same O. The add receiver is OPI, not the separate engine GuiModel. Keep synchronous zero-slot postLoad creation1411df0e9/return0ee separately attributed; it does not validate Apply timing.

The next bounded static action is to identify the actual style producer for the reached invalid-role branch, or an actual later item/delegate replacement writer; do not re-open the now-resolved +8 inheritance question. When passive original observation becomes authorized, a same-thread nested trace should bind T/private+3a4 registration lifetime,67142315 target,654a8998 entry/return, actual item/table/role result and any nested UpdateData/Apply/add-return, inside a complete Open span. GUI handler completion is0271b0..028514. For CLI,001c94 precedes postOpen001ca9/001cae and is not whole-Open completion; retain selected startup completion001e4e. Current retained Forming Editor004 does not supply this Boolean frame. No new capture was attempted.

## Reproduction identities

Immutable originals: DEF_GUI_MO_64.exe SHA256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc; QtGui4_64.dll SHA256 c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887. Reproduce bounded disassembly with the verified DUMPBIN path in CP116 and /disasm:bytes /range:start,end-1; vtable and import values above are direct PE reads.

| Code range, end exclusive | SHA256 |
| --- | --- |
|EXE1405f15e0..1405f1af3|c1fa35b8d1a6af2604a4c9481a5ff25811dc29cff7b46f9029ab76681b44ac0f|
|EXE1400c4a10..1400c4e56|c5169a7613f2567eed24d47747ac637f70d4462ac0b4514b86be860a0b09cd3c|
|EXE1400c7200..1400c7b12|7890dff782d2c2f2c373d94862ded1f766512b294b53e72a45f88393850de7b3|
|EXE1400c5d40..1400c6023|eaf38427408a61e209257673f917f28ae9c067656ddac0653349290a3b8dbb11|
|EXE140d4c050..140d4c0b9|e6642085b72a4afc723aa734c0d8208541417c074b8d4d58731ec588a05d133f|
|QtGui65529050..655290cc|ffb5e799521368526eab58ceb895187e9991de2460662ed43db42010f161f56b|
|EXE140dc1e00..140dc71cf|4d76f2d1665b39cd9b928f34f6d3924a65c222248ed7c0a8b266d9b532db99bf|
|EXE141345390..1413455b8|4bf9064b3c5dad9a6cfdc2384c56eb2c8b3601d64e1f22a52558482fab4dd4f4|

Predecessor qualifications: analysis/resume112-boolean-postload-direct-edge.md; analysis/resume116-boolean-layout-timer-return.md; analysis/resume117-boolean-layout-helper-frontier.md; analysis/resume117-boolean-delegate-producers.md. This report carries forward the interrupted CP118 item-producer investigation; it does not claim a completed CP118 artifact.
