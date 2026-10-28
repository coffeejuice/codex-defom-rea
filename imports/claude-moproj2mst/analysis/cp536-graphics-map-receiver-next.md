# CP536 graphics-map receiver successor

The next bounded increment is a private lookup prefix for the actual editor's
input-connector-to-graphics map at a separately qualified Created2 or Destroyed2
slot entry. CP535's backend gates do not supply live receiver delivery or graphics
ownership. Keep public graph execution guarded and preserve the preceding port,
link and notifier effects when this successor refuses.

## Evidence identity and limits

Workspace: C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst.
Immutable original: evidence/original-binaries/DEF_GUI_MO_64.exe, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

This memo follows read-only retained-source review and bounded offline DUMPBIN
14.44.35228.0/section-aware PE reads. No original process, GUI, REA session, build,
source edit or deployment was used. The byte hashes below are independently read
original PE bytes, not newly retained assembly files. Reproduce and retain bounded
assembly/metadata receipts before using newly selected bodies in implementation.
Runtime unwind spans give selected code/data bounds, not whole-program closure;
14018bb30 includes embedded switch-table data, which must not be called executed
instructions. Nothing here establishes fresh Open/Save/Close parity.

Retained qualifications and navigation:

- analysis/cp535-graph-backend-gates-contract.md: two current-backend reads,
  successful private link byte and the observation seam; still no public enablement.
- analysis/cp533-ordered-object-row-connection-contract.md: row-only clear,
  Destroyed2, reciprocal link and Created2 order, distinct from the main input.
- analysis/cp527-read-from-db-row-port-guard-contract.md: graph and graphics-add
  static qualification; actual receiver registrations remain unobserved.
- evidence/gui_mo/s1/s1-01/spec.md: imported Ghidra/source qualification, static only.
- analysis/wave3-closure-next-14016f870.asm: retained constructor instructions;
  SHA-256 `afb169ee160beaf5a89f53cd1e7872f29e856042bb6e6af66cbc81683eea98a9`.
- evidence/gui_mo/s1/moproj_vt/v1a.c:207,211: annotated registration candidates;
  file SHA-256 `f3551e72e88b1efdb2c45293598b7e907b684be18ee0cf67c014937f40d5ecb6`.
  Instruction bytes and directly decoded literals control over annotations.

## Registration and exact Qt slot binding

Editor constructor 14016f870 uses notifier owner 143d76700, receiver=this and
registration flag0 (append/deduplicate route):

| Event | Registration call | Key literal | Encoded method literal |
|---|---|---|---|
| Created2 | 14016ff98 | 1421d7bf8: 2signalDiagEditorConnectionCreated2() | 1421d7bd0: 1slotDiagEditorConnectionCreated2() |
| Destroyed2 | 14016ffe6 | 1421d7c98: 2signalDiagEditorConnectionDestroyed2() | 1421d7c70: 1slotDiagEditorConnectionDestroyed2() |

These calls occur inside construction; they are not evidence of a completed,
published, live editor at a later row event. Do not infer registration absence
from the current converter's missing receiver model.

PE Qt metadata 1421de840 and string table 1421def20 define local method45 as
slotDiagEditorConnectionCreated2(ISfDiagItemConnector*) and method47 as
slotDiagEditorConnectionDestroyed2(ISfDiagItemConnector*). The metadata accessor
14018d170 permits dynamic metadata through QObject data+28 when present and otherwise
returns static metaobject143d821c0. Normal qt_metacall14018d230 delegates to its
base, then routes local IDs below0x55 through 14018bb30. Switch table14018d014
selects the following blocks:

| Local ID | Block/call | Virtual slot | Both identified vtables resolve to |
|---|---|---|---|
| 45 | 14018c45a / call14018c476 | +3e8 | 14017f750 |
| 47 | 14018c4d9 / call14018c4f5 | +3f8 | 14017f940 |

Both SfDiagEditor vtable1421d8e98 and SfDiagCellEditor vtable1421cded8 have those
targets. Created2 wrapper14017f750 calls virtual+578 at14017f770; both tables
resolve it to14017f600. Destroyed2 wrapper14017f940 calls virtual+580 at14017f960;
both resolve it to14017f790. Runtime dynamic metadata or another actual concrete
receiver must be separately qualified rather than silently forced to this pair.

## Callback-visible native order

The map is at `editor.private(+28)+30`, keyed by the supplied input-connector
pointer. This map is distinct from the logical FlowItem prev/next chain, reciprocal
ObjectRowPort peers and other editor-private maps.

Created2 implementation14017f600:

1. Contains query140d9d7f0 at14017f629; an existing key returns at14017f6ba.
2. Absent key reaches allocation(size0x28) at14017f63f, followed by constructor
   140188f60(new,inputConnector,editor) at14017f660 on the nonnull allocation path.
3. Obtain mutable value slot through1413c87d0 at14017f69b, then publish the
   constructed pointer with the store at14017f6a8.
4. Call QGraphicsItem::show on constructed+10 at14017f6b4, IAT141e074e0.

The allocation-null arm stores null and still reaches the later constructed+10
show call. Do not invent successful-null construction, safe early return or
allocation-failure cleanup for a future complete implementation.

Destroyed2 implementation14017f790:

1. Contains query140d9d7f0 at14017f7b0; absent key returns at14017f835.
2. Present key obtains mutable value slot through1413c87d0 at14017f7d0 and loads
   its value. A nonnull graphics value receives deleting virtual+18, flag1, at
   14017f80b. Null skips that deleting call.
3. Only after normal deletion return (or the null-value branch), remove the key
   through1401848a0 at14017f830.

A refusal before/during deletion must not remove the key first. Created2 must not
publish before construction or show before publication. Mutable value lookup may
carry detach/allocation effects even for an existing key; do not replace it with
a const lookup beyond a separately qualified prefix.

Contains wrapper140d9d7f0 calls14139a530 at140d9d808, compares the returned node
with the map's actual sentinel at140d9d815 and returns a byte. 14139a530 traverses
map nodes and calls key accessor1413cc960 and comparison1413d7e30. Its visible
stores are stack-local; those two leaf dependencies still need retained bounded
inspection before claiming the entire contains route is inert.

Constructor140188f60 immediately constructs QObject(parent=null) at140188f83,
reads the supplied editor's QGraphicsScene at140188f92, and constructs its
QGraphicsItem subobject at+10 with parent=null and that scene at140188fa6. It then
installs vtables1421de758 and1421de628 and allocates a separate0x60 payload.
Therefore construction can affect native QObject/scene ownership before map
publication; a logical edge allocation does not implement this constructor.

## Reproducible PE byte bindings

All intervals are [start,end), hashes SHA-256 of mapped original file bytes.

| Selection | Hash |
|---|---|
| 14018d170..14018d1bd | 15a68cd2f13a0abb3aae2d3e1a0139dc00398353e326284250f55779046141f5 |
| 14018d230..14018d2ab | 5050c37ff4d1a6da34d48de8944cf0b1f4991d5185b4b341fb739cbce4c1acaa |
| 14018bb30..14018d168 | 4989a481069b930095accfcc54f7b2534e84c0666e0af124733db0f7e5664575 |
| 14017f750..14017f77b | 08d921b58a022562282c00bf66745fd0ea757b677e3fa9a16e23e1374f5d2660 |
| 14017f940..14017f96b | fc3c59988ebbf54e6b7056f070926b9b7c644fb26807e23b8d7ba9ba1c3be464 |
| 14017f600..14017f6bf | 672471792bd698b9b3a95ad1d5a523d7b41a4abf7fdc5708be04b9f201439055 |
| 14017f790..14017f83a | 34824ccc2e9847d1f4d3ad27cb133c297fea34e39709476061a2abef0de84921 |
| 140188f60..140189109 | 48a6dce6c594a11226f30db5e70bc82e15542713d88549215177fd3180d3cccd |
| 140d9d7f0..140d9d835 | 72608f44c0737e93c3c54b59453f857f4a9677038de20a642fe891028f1f8d8b |
| 14139a530..14139a60d | 19dec3123fd4166c32757820d0750450e5e780b717a4041e3f7fba5d397f6d68 |
| 1413c87d0..1413c8884 | d072054d6fad489c7a4437e6cf992c8bd1a3d666c34693161ee22c3e7f1d6ac3 |
| 1401848a0..140184ab1 | d99d254cc7ba95c13f5941f606fd17654c51f507bde6730ed851c1ca2bb11c2a |
| metadata1421de840..1421def20 | 66ab11598e302cba791753fc272925fe95a378f1287f1c59436f205b3bca6f89 |
| derived vtable1421ce2c0..1421ce2d8 (+3e8..+400) | 0584646b4f8d0ab48662d2432a7199f635d8282605d63537c57df1da1f33ddf6 |
| derived vtable1421ce450..1421ce460 (+578..+588) | fb101fb6751457d5b8354ff8d857826ddc6b380642939a0f360c6ca59acc467a |

The helper hashes bind selections, not a claim that all their descendants or
allocation/destruction branches have been recovered.

## Private prefix acceptance

Implement only after the integrating owner releases the CP535 source freeze.
Retain completeForPublicGraph=false. Entry is an explicitly reached receiver slot,
not an assertion that the global dispatcher selected or invoked that receiver.
Bind the live ConversionSession, exact editor generation/map owner, same input-port
generation, event and source occurrence. A nonblank private qualification may
label a diagnostic declaration; it must never become native liveness proof.

The actual private map state must have a producer and lifetime owner. Do not default
an unrepresented native map to empty, use a standalone map as the actual editor,
or seed population solely to make a branch pass. Public integration needs actual
construction/population/destruction history. A prepared diagnostic projection must
remain visibly distinct from that public owner.

Acceptance for the narrow lookup prefix:

- Created2 existing key: return without allocation, replacement, show or topology
  mutation. Preserve an existing null value as present rather than absent.
- Destroyed2 absent key: return without mutable lookup, deletion or removal.
- Created2 absent key: stop before allocation14017f63f; no graphics object, map
  insertion or show is manufactured.
- Destroyed2 present key: stop before mutable lookup14017f7d0 until its native
  effects are qualified; retain the existing entry and exact value identity.
- Unknown population refuses before lookup; foreign/released editor, wrong map,
  stale input port and invalid occurrence refuse before mutation. Retrying a
  consumed prefix must not replay native effects or silently re-read changed state.
- If an observation seam is needed, verify owner generation and ordered relevant
  map/port state across it. Its mutation remains reached state on refusal, with no
  rollback or fabricated normal return. CP535 now applies this rule to peer lists.
- A later guard preserves already reached CP535 clears/link and notifier flag90
  effects. Keep independent Destroyed2 and Created2 outcomes and do not turn a
  receiver-prefix return into full graph/public success.

## Public dependencies and smallest next investigation

Current backend at each CP535 read, unsuppressed key conversion, ordered notifier
snapshot, weak guard result, actual editor pointer, ordinary/dynamic Qt metadata,
map population and normal receiver return must belong to the same selected
occurrence. Static constructor registrations and constructor-null backend values
do not provide those facts. Project Close must not be equated with destruction of
the main-window/session-owned editor.

Graph placement also emits signalUpdateWidget at14015afb6 after rectangle relayout,
connector rebuild140158cc0,1401599b0 and14014e850; retained source is
 evidence/gui_mo/s1/moproj/v5.c:280-362. The fixed direct registration inventory has
UpdateWidget candidates140388625,140f99dd2,14104deda and14134f3f3 in
analysis/wave2-doe-session-notifier-direct-key-candidates.json. No actual complete
population, receiver effects or normal return is thereby established.

The related editor clear/refresh wrapper140153f70 calls virtual+318 at140153fb1
(derived target1401725d0), then emits DiagItemClicked at1401541ef,
DoeTableRefresh at1401543ec and UpdateMainView at1401545e9. Keep its exact caller
reachability distinct from the row-only branch. DoeTableRefresh has a real table
candidate: ctor registration1400ebc5c, slot1400f4e10, QTimer::stop at1400f4e25,
UI virtual1400f4e38->1400f47e0, table+98=true before widget enable and actual
current-Project lookup. See analysis/resume65-doe-refresh-owner.md. None of those
static edges closes same-occurrence delivery or makes refresh inert.

First retain/recheck the metadata blocks, two wrappers and add/remove bodies above;
inspect1413cc960 and1413d7e30 to close contains without a guessed comparator. Then
implement and test the private lookup prefix. The next substantive effect requires
recovering complete constructor140188f60 plus QObject/QGraphicsItem/scene effects,
or the concrete graphics deleting-destructor and1401848a0 removal route. Keep those
as separate increments with their earlier committed effects retained. No new broad
GUI acquisition is needed for this bounded static successor.
