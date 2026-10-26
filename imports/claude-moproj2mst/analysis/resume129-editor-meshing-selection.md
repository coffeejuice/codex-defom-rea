# CP129: stateful private meshing object selection

Native meshing selection commits the previous object's four temporary scalars, then always reloads the requested object, including the same row. Save explicitly invokes this callback with the actual combo current index before copying all remembered records to current model objects. This supports a bounded finite3D private0->1->0 transition and a stateful private Save API. It must not be implemented by copying Spring's same-row early return or by silently replacing the broader legacy const Apply adapter.

Only this new report was written. No source/test/shared-header/CMake edits, build/test, GUI, original execution or mutable REA operation occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Reached caller and actual ownership

The constructor connection receipt has UI+9b8 `activated(int)` -> `slotCmboxRemeshObjectsActivated(int)` (source line159). Meta dispatch1410db92a..db947 reads the delivered int into EDX and calls140eceb60 at1410db93d. No inspected route connects this control's currentIndexChanged to that handler. Programmatic setCurrentIndex is therefore distinct from explicit activation.

Fill140ec0a40 resets editor+100=-1, rebuilds/sorts the displayed object list, allocates the editor+f8 temporary slab (stride158), and copies four fields from each eligible actual model object. At140ec1100 it sets combo current index0; at140ec1110 it explicitly calls140eceb60(0), then stores previous index0. This explicit call is required even though the programmatic setter moved selection. Existing positive-unique-number label sorting remains the private admission; no new label/item-data inference is needed.

Ownership mapping:

- Native editor+f8 is one retained per-editor temporary array. Its four recovered values map to the single MeshingPage.objects vector; do not add a second remembered record vector or alias live model storage.
- Native editor+100 is the last form loaded by the controller. It is distinct from UI+9b8's actual current combo index.
- The existing firstInterference/firstSizeRatio/firstTolerance/firstNodeCount/radio fields should become the CURRENT visible controls; retain names for compatibility rather than duplicate them for every row.
- The GuiModel remains external and unchanged by activation. Actual private lifecycle apply already pins session.sourceModel and sourceRemesh before page calls. Only Save copies remembered records into still-present model objects and then updates the same supplied RemeshState.
- The full native158-byte record, allocation/destruction and unrelated mesh fields are not reconstructed by this four-scalar projection. Native14085e050's four-field contract remains controlling.

## Exact callback order

| Original address | Effect |
| --- | --- |
|140eceb88..eb9d|Negative requested index returns before old-form reads.|
|140ecebaa..ebbf|Null native temporary array returns. An empty typed vector alone must not manufacture this positive-index pointer witness.|
|140ecebcc..ebd3|Stored index==-1 skips previous-form writeback.|
|140ecebf8..ec70|Read negative-interference radio; resolve dimension predicate.130ea0 means dimension==2; a real3D model takes its false branch.|
|140eced55..ed85|3D nonnegative choice parses UI+970, absolute-value helper141470a78, stores old temporary+b8.|
|140ecede1..ee19|Negative choice does the same then XORs sign bit at1421d3d30 before storing+b8.|
|140ecee70..ee9b|Read UI+920, toDouble(nullptr), store previous+c0 size ratio.|
|140eceef2..ef22|Read UI+918, toDouble(nullptr), absolute helper, store previous+d0 tolerance.|
|140ecef69..ef7f|Read UI+938 spin value, convert int to double, store previous+c8 node count.|
|140ecef88 onward|Immediately index REQUESTED target; there is no equality comparison/early return against previous index.|
|140ecefaa..ecf02f|Set the two interference radios from target+b8. COMISD0,value/JBE takes the nonnegative choice for either signed zero.|
|140ecf02f..f255|3D loads abs(interference), size ratio and raw tolerance into text controls with QString::number(g,6).|
|140ecf27f..f29e|CVTTSD2SI target+c8, then QSpinBox::setValue; use existing explicit0..1000000000 spin profile.|
|140ecf2b3|Only after reload, store requested index as previous index.|

Source model getter130e50 resolves the actual editor model/step and returns its dimension (historical missing-owner fallback3 is not permission to substitute an absent candidate model). Existing private helpers already require a real3D model. The2D UI+950 path stays excluded.

Same-row example: visible ratio1.23456789 is committed in full to its temporary, then reloaded as1.23457. It does NOT stay as the original visible string, unlike Spring. The next callback can then commit the rounded value. A negative radio with magnitude0 commits negative zero; the following reload chooses the nonnegative radio and text0 while the temporary still holds negative zero until another writeback. Preserve this sequence rather than normalizing the temporary early.

## Save-currentIndex and numeric guard boundary

Save140ec1350 reads actual UI+9b8 currentIndex at140ec138a and calls140eceb60 at13a0 BEFORE the object loop. Thus a programmatic move0->1 with stored index still0 causes Save to commit row0's old controls, reload row1 without first writing row1, then copy all records. Current index-1 causes callback no-op; Save still walks remembered records. Never force requested index to stored index or default0.

After callback, Save visits remembered labels in order, looks up each current model object and copies four scalars if present; missing objects are skipped. Only after that loop does the shared remesh global gate run. Retain CP128 disabled-method admission and current enabled-invalid reconstruction preflight; do not move disabled global logic ahead of object copies.

There is a material finite-profile boundary.141470a78 is not merely an inline sign mask: it has nonfinite classification and additional calls. Its finite branch clears sign at141470b39 and returns the resulting bits. The new activation path should initially require finite values for visited absolute-value inputs and target controls, retain the0..1e9 spin-value qualification and existing Qt C-locale adapter semantics, and leave native nonfinite CRT/FP behavior guarded. Unvisited records retain their bits unchanged. Do not reject every record because another unvisited row contains NaN.

Do not import Spring's ok/finite guard wholesale: existing const meshing Apply already uses qt::toDouble without checking ok, so ordinary failed text/empty text maps through that existing value adapter. Preserve that compatibility scope. Native FP-environment/math-error behavior beyond it is not newly established. For the new finite stateful API, preflight unsupported OLD numeric profiles before mutation; if TARGET profile is unsupported after old commit, retain old values and a PreviousCommitted report, stopping before target-control reload. Do not claim the skipped native target effects for that excluded profile.

## Minimal shared API proposal

Root should coordinate the shared header before worker edits. Append two indices to MeshingPage, initialize them explicitly in Fill, and keep the existing visible-field names:

```cpp
int storedObjectIndex = -1;
int comboObjectIndex = -1;
enum class MeshingObjectEvent { InitialLoad, Activated, SaveCurrentIndex };
enum class MeshingObjectStage { Preflight, PreviousCommitted, ControlsReloaded, Complete };
struct MeshingObjectReport {
    ApplyResult outcome = ApplyResult::UnsupportedSnapshot;
    MeshingObjectStage stage = MeshingObjectStage::Preflight;
    bool previousCommitted = false, sameRow = false, controlsReloaded = false;
};
bool setMeshingComboCurrentIndex(MeshingPage&, int);
MeshingObjectReport transitionMeshingObject3D(
    GuiModel&, MeshingPage&, int requestedIndex, MeshingObjectEvent);
ApplyResult applyMeshingSelection3D(GuiModel&, RemeshState&, MeshingPage&);
```

The programmatic setter accepts-1 or an in-range row and changes only comboObjectIndex. Explicit activation/Save passes the delivered/current index. Known-negative requests are the native no-op; reject out-of-range positive indexes before old effects as candidate admission, without pretending the unchecked native array access is safe. InitialLoad uses stored=-1/combo0 and skips stale controls. For a positive row validate same supplied model3D, index consistency and the supported previous control profile; commit old scalars once, qualify/load target, then update stored index. sameRow is diagnostic, never a reason to skip reload.

Keep existing `applyMeshing3D(..., const MeshingPage&)` as the explicitly isolated first-form compatibility adapter. It must not accept a changed selection context and silently roundtrip row0. For nonempty pages, guard that legacy context to stored0/combo0; current first-form callers produced by Fill remain valid. New mutable applyMeshingSelection3D calls transition(SaveCurrentIndex), copies already committed items without a second selected-row conversion, then runs the existing same-owner RemeshState tail. It retains the callback's mutations to visible controls/index/temporary across private calls.

Do not introduce a finite mutable overload with the old name: overload selection would silently change existing nonconst callers' numeric behavior. The distinct name makes the admission boundary explicit. Root can replace just the private lifecycle Meshing-page call at simcontrol300_lifecycle.cpp with the new stateful API; its existing owner checks remain. After all page blocks, lifecycle already clears the temporary vector like native140eb52f0. Do not resurrect it for a repeated Save, synthesize native allocation ownership, or reset unrelated indexes as a fabricated destructor effect.

## Exclusive files and focused acceptance

Worker proposal: existing `src/engine/simcontrol300.cpp` plus NEW `tests/mdt/test_simcontrol300_meshing_selection.cpp` and an implementation receipt. Root/shared owner: `include/m2m/engine/simcontrol300.hpp` declarations/fields and the one private `src/engine/simcontrol300_lifecycle.cpp` callsite. No public factory/loader, Spring, remesh IO, Qt primitive or CMake edits are needed. Existing first-form snapshot tests and CP128 disabled-remesh target should remain unchanged.

Focused cases:

1. Actual3D model with numbers2/10/another eligible object: Fill order10 before2, initial stored/combo0. Activate0->1->0 with edited controls; commit only the old temporary, load target g6/radio/spin, leave live model untouched until stateful Apply. Third unvisited scalar bits stay exact.
2. Same-row activation and Save must BOTH commit then reload, including precision-sensitive text and negative-zero radio behavior. Test two callback occurrences without claiming a complete second native Save after full temporary cleanup.
3. Programmatic combo move does not commit/load. Stateful Save uses actual current index, writes the prior loaded row once, does not prematurely roundtrip target, then copies all remembered records to actual model objects in order. Negative current index skips old commit.
4. Missing current model object still skips only that copy; later objects/global state continue. Disabled remesh still ignores method/scaling/internal-checkbox while prior object effects occur.
5. Unsupported target finite profile stops after the old temporary commit with PreviousCommitted; target UI/stored index are not fabricated. Positive out-of-range, wrong dimension and wrong stage are preflight guards. Preserve unvisited NaN bits; do not add an all-record finite guard.
6. Const first-form compatibility still passes existing tests and refuses a changed selection context; actual private lifecycle uses the new mutable name, with same sourceModel/sourceRemesh owner checks and public palette refusal intact.

No test or compile ran in this read-only handoff.

## Evidence receipt

Retained handler decompile SHA-256 `8ef0bddd07f59fa2ffb008c3fc7276d8249700a787afa943c6f4041671f10894`; Fill decompile `241216a6b6bb51d6f8e04f62a635c4d3daa1378271392a12109950dcc6be1f84`; constructor connection table `1621275823b3e0dfb75cc8822ae961a13a8e701de6a70f690bba3bda9ee50335`; slot map `9bbf83751e7f7606f039392b6e775f9a20cf3984fb1e3654b9e32c6d4a669661`.

Half-open PE ranges rehashed directly and instruction meanings checked with bounded DUMPBIN:

| Range | SHA-256 |
| --- | --- |
|140eceb60..140ecf2cd complete handler|`49b658b9bee06716baddd4df1e69db8d9641b558517df54c3d76bc594bf7c2a9`|
|140eceb88..140ecebd9 negative/null/previous gates|`b882e0a08b117b96bc98fa6196c206b4d17bc6821763a46651a0cc83f8b353fb`|
|140ecec70..140ecef88 previous writes|`166fb63dfb2353f2d45d2c02e33ec07a50ec566229f7b9ddc9b5ab6219c8faad`|
|140ecef88..140ecf2c5 unconditional target reload|`57334da73b74e51c01d22bf91ddfe3f49df26b6470f262a5d20306380ea60082`|
|140ec10f7..140ec1128 Fill explicit selection/callback|`f17dff455391f27a01f2e3f3d63a336e479b49896afb6dd9b27e46bc39d50aa5`|
|140ec1374..140ec13a5 Save currentIndex/callback|`7da1cba76bb6a03500d06ee41944d73af61cec8f0c80fa1207035718ef66587b`|
|1410db92a..1410db947 activated dispatch|`065866323e273ff056771929a20ab6735b1359f4e881ad291af80af7be79c7bb`|
|140130e50..140130e98 actual dimension getter|`3b4ae90af5da95087f64c599a25214ba68ef97cf8ce3e2380a8f2bbaf50ef1a7`|
|140130ea0..140130ed3 dimension==2|`5522b3400c3666684a37f59c348a0a83cdf5d01317a7237bb3556850d09b5d92`|
|141470a78..141470b64 absolute helper|`1e2ced106fd92883befb023db7c562094b9a9d39dcb766dd3de668d931bc4fb4`|
|141470b2f..141470b54 finite absolute-value return|`d7e7328e0ff95a255816c75d3a7d198c9524794e9b87c079d7ac36a7f5c57fa5`|

Pinned DUMPBIN tool SHA-256 remains `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Only analysis tools executed; no raw capture file or other report was written. Read-time candidate identities: simcontrol300.cpp `260331d646fdc5edf712278c6e056142996a8b4df54a59bfd1db8c96b9b52abc`; simcontrol300.hpp `f06a8a7ea6874cb00b327ff9cf8e6938199f939eac9f44def2dedb99b982ce4f`; lifecycle.cpp `dd22c2e00fbd459822e2e5cf0e9325ac3e9997f5d553ce50559157cca19ab486`. No reconstructed executable/deployment was inspected or produced.
