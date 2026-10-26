# CP80: smallest missing first-Open Boolean mutation edge

Retained evidence still does not establish when fresh Open first creates a missing Boolean Workpiece/Cutter. The mutation leaf is closed; the original occurrence and its ordering against Open return are missing. The smallest **timing** witness is narrower than reconstructing the entire timer route: observe a successful engine addObject return from one of Boolean Apply's two exact call sites, on the actual selected editor/model owner, and order it against the selected Open entry/return. That proves creation timing without proving which event caused Apply. Causal timer parity remains a separate dependency.

Only this report was written. No source/build/tests/GUI/native execution/mutable REA. Original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Statements below retain their body/source-profile qualification.

## What creates the object

CP62's complete retained Apply1411e2870 body binds these normal-success edges:

| Role | Current-text decision | First-free snapshot | Engine addObject | Return continuation | Later slot rename |
| --- | --- | --- | --- | --- | --- |
| Workpiece |1411e2922..2939 compares exact `Add New Object`|1411e2944, then+1|1411e2955 ->140057ba0|1411e295a|1411e29c6|
| Cutter |1411e2b93..2baa independently compares that text|1411e2bb5, then+1|1411e2bc6 ->140057ba0|1411e2bcb|1411e2c40|

The returned object number is read from result+8; the operation's Boolean values and object name are then updated. Workpiece handling precedes Cutter. The first-free slot snapshot precedes addObject and later rename uses that captured slot. Do not move rename ahead of creation, infer completion from callee entry, or invent null-result/failure recovery. A successful addObject return and later successful Apply/rename completion are separate observable boundaries.

An Apply entry alone is insufficient: current combo text may select an existing object and bypass these add sites. Constructor1411e2480 ->setupUi ->Fill1411e25b0 initializes a selection snapshot, not an unconditional add. The retained selected Fill body has no direct addObject call; this is not a complete negative claim about all Qt/base/signal descendants. The Boolean editor itself is allocated separately by factory1411e0510, allocation0x50 at11e065b ->ctor11e0677; allocating that QWidget is not creating its engine Workpiece/Cutter.

## Routes to Apply, and where first-Open reachability remains missing

An admitted `UpdateDataInWidgetArea` uses custom notifier1402cf3c0's explicit raw invoke type1. Its actual impl receiver runs140dae340, resolves the live current guard impl+18, checks its cast and calls virtual+1c0 at140dae3ab. For the actual Boolean vtable142cd9b18 that slot is1411e2870. This leg is synchronous once admitted; its missing real receiver/current/editor identity is not supplied by an isolated helper allocation.

The selected outer selection path1405f36d0 emits UpdateData at5f4524 **before** Leave, new factory/Fill and Show. Actual property index0 takes a different propsToEngine branch. Thus showing/filling a new first editor and requesting stack index1 cannot retroactively make the preceding emission apply that new editor. No direct second UpdateData after Show is proven in the selected outer body; indirect/reentrant Qt descendants are not exhausted. This actual emission/current-editor invocation is the smallest missing **method edge** for enabling the existing synchronous helper on a real reached path.

The diagram route requests1000ms timers at17085f, then requires actual timer delivery/activation, live receiver and the correct Auto0 thread branch before183160 modelUpdated. It fits first, then reads current model/item and sends selection notifications. Sender block-signals, cleared connection receiver, notifier+141 suppression, active-project/loading gates, current property index and editor lifetime can each prevent Apply. Registration count, elapsed interval and Save's zero-wait poll098129 prove none of these runtime outcomes. CP72 and CP79 retain those exact distinctions.

Native Save can independently reach UpdateData through widget update5f1e20 at5f22af before propsLoad5f22c4 and outer propsToEngine5ef54a. Therefore a matching final saved file, saved Boolean values, later side-file output or a Save-time creation cannot establish that Open had created the object earlier. Likewise CP75's actual main-window constructor return only proves owner lineage/historical initializer reachability; CP76 found no current+3c8 read. CP77 closes primary QObject/QWidget/window address identity while leaving base effects/current fields unknown. None supplies a first-Open event occurrence.

Retained original editor probe004 selects ctor eb26f0 rather than Boolean11e2480 and stops during that editor's first grid construction. It is not a Boolean first-Apply/addObject capture. Its selected native CLI startup Open is also explicitly qualified separately from interactive File/Open.

## Smallest resolving observation

For the narrow question “did this selected Boolean Apply create a new object before the selected Open returned?”, correlate:

1. Actual process/module/capture, selected Open owner and entry/return ordinals; distinguish CLI versus interactive source profile.
2. Actual Boolean editor/operation/GuiModel allocation generations at Apply1411e2870. Preserve nested invocation identity rather than inferring it from a reused address.
3. A140057ba0 invocation whose native return address is1411e295a or1411e2bcb, tied to that Apply's model; callee return/result pointer/object number and actual branch role. A direct call-site continuation observation can supply the same normal-success boundary. If the callee did not return normally, record an attempt/unknown completion instead of classifying successful creation.
4. An ordered Open return in the same captured timeline. A returned add before that boundary is an Open-time mutation; one after is later. Keep object-created, fields/names applied and Apply returned distinct. For “first such mutation,” additionally require gap-free monitoring over the selected interval and choose its first qualified mutation; a first observed hit in a gapped trace is not first execution.

This positive timing witness can bypass unresolved event-cause reconstruction. It cannot admit a synthetic timer, establish which registered event fired, or prove the complete fresh Open-to-Save lifecycle. To establish absence of creation during Open, a trace must be complete for the selected mutation sites and interval; no hits in today's partial evidence is insufficient. If the question is first engine object creation from any cause, all other creation paths/input loading must also be covered; the two-site witness above is specifically Boolean Apply creation.

## Bounded testable next contract

An independent safe increment is an **observation-only Boolean mutation-phase classifier**, not a public effect driver. It would accept explicitly supplied owner-bound method/mutation occurrences and Open/Save boundary ordinals, validate their generation/capture/source-site identity, and classify only a completed qualified mutation. It must return unavailable/reject for missing Open return, incomplete addObject completion, mismatched Apply/model/editor, foreign capture, duplicate/ambiguous ordinals, wrong call site or unsupported nested intervals. Do not infer timer readiness, receiver liveness, successful Apply completion or object absence from the classification.

Focused deterministic cases should distinguish: successful Workpiece return before Open return; successful Cutter return after Open but before Save; successful return during an explicitly bracketed Save poll; Apply-only/no-add remaining unclassified; entered-but-unreturned add remaining completion-unknown; foreign/colliding-address generations; missing/gapped trace not becoming “no creation”; and overlapping/nested Open/Save intervals remaining guarded. Preserve role order if both add sites belong to one Apply, and do not interpret slot number changes as a new event schedule. This classifier proposes a new source-free observation seam; it was not implemented or tested here.

The existing Boolean tests already exercise Add New Workpiece-before-Cutter and explicit two-callback Apply/refill behavior. Repeating those tests would not resolve native timing. Existing `BooleanSynchronousUpdateData` consumes an admitted emission before the effect and guards nesting/receiver/current identity; its synthetic inputs remain explicitly conditional. A later real integration needs actual owner population and the reached UpdateData/current-editor edge described above.

## Source/evidence qualifications

Inspected native evidence: `resume62-boolean-timing-1411e2870.asm`, `wave2-db133-boolean-contract.json`, CP64/70 UpdateData/Show overload contracts, CP72 timer/suppression report, CP75 constructor-owner receipt, CP76 getter-witness survey, CP77 main-window Qt lineage and `resume79-boolean-save-timer-readiness.md`. None contains the owner-correlated original Boolean addObject/Open-return occurrence required here.

Current inspected source under `source/apps/moproj2mst/`: Boolean lifecycle header SHA `976cd0c9b2388d3e14de53c906ebfa027f09dbe2c78699247198d289fabce69a`, implementation SHA `7340522bfce32301d5cbbfc970575aca0ce2fc0534d43a5bb43c4ebfadcfdd76`, Boolean editor tests SHA `1a095f88b9dfe7322b5ba1e11a39366c2643363f48da28b219bf40dc4c53a796`. These hashes qualify read-only inspection, not a new tested build. No native due values, owner fields or timing defaults were invented; public first-Open/PostPending guards remain necessary.
