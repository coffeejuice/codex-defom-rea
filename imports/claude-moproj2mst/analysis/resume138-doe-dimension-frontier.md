# CP138: private supplied-owner Variable dimension prefix

The next source-closed isolated Study Save mutation is the per-variable dimension store inside native0x1400d2b60, using an explicitly supplied operation Project. It can update actual owned HeaderVariableRecord::dimension, continue through the empty-item branch and stop after a changed dimension before unqualified keyword snapshot/conversion. This is not a public reconciliation return: global/current-MO/lazy OprManager ownership still precedes it and remains missing. Keep public nonempty Save, refresh and payload guards.

Only this new report was written. No source/test/build, GUI/original process or mutable REA action occurred. CP137's private prune source was read, not changed. No Type3 row/owner/parser file was touched.

## Position in the native sequence

The source order is actual manager resolution/reconciliation0d2b60 -> prune91c820 -> XML device/output -> refresh -> cached-manager PAR writes. CP137 isolates only pruning and keeps public nonempty Save before all of it. The next helper must not call prune, create an operation manager or certify that the earlier global stage ran.

Retained complete reconciliation capture `resume103-doe-save-reconcile-1400d2b60.asm` (SHA `4E5A81664574951B2A327A00723BC13C1FF0153292BE67613F21C1F0598A59A2`) is tied to GUI PE SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Nonempty entry snapshots variables atd2ba6 before process/main-window/current-MO/Project/lazy operation-manager resolution. These owners are independent from the Study's saved Project and not supplied by the candidate Open manager receipt.

Once the operation-manager pointer is nonnull, native initializes a local default dimension3 and loops the earlier variable-pointer snapshot. The per-row instruction sequence is:

| Site | Effect |
| --- | --- |
|d2c84 | null variable pointer skips to next row |
|d2ca1 ->816ae0(var,0) | get ONLY first operation ID; no scan for another applicable operation |
|d2cba ->047dc0 | find operation by that ID in the resolved operation manager; absent skips row |
|d2cde ->05f0f0 | get operation dimension, default3 when op class is absent |
|d2cfb ->c0df50 | read current variable dimension at+0x24 |
|d2d07 | equal dimension skips the entire item/conversion branch |
|d2d25 ->0d8040 | store selected dimension at+0x24 |
|d2d43 ->0d0e60 | only now obtain/copy the variable's keyword/item list |
|d2d5b | empty snapshot goes to cleanup/next row |
|d2d74 onward | first item null also skips; nonnull begins keyword text/data conversion |

Exact empty-operation result:816ae0 initializes its return to-1, checks index0 against Var+38 count, and returns-1 on empty. Do not assume0 or try subsequent ops when first lookup fails. In the common supplied Project domain containing only positive operation IDs, that empty list has no matching operation and makes no dimension store.

The native dimension getterc0df50 is a14-byte leaf reading [this+24]. Setter0d8040 is a22-byte leaf writing its int argument to [this+24], with no sample-invalid, link, ID, index, geometry or notification effects. 05f0f0 initializes3, obtains opBase twice and, when nonnull, calls its virtual+0x190 dimension. It does not derive the dimension from the Variable or Study.

Selected raw normal-body hashes through RET, verified by bounded DUMPBIN:0d8040/22=`e55859db696a404373e81e1e920daf76927318d4ebf719c74a16fdbc7832ad38`;c0df50/14=`174a76fdc81e6379023c03d9beede2d78463f68897dcd0ba21e0bb71fcadbd93`;05f0f0/79=`e1185be41112f3763251838d66d44b4ad6e170509f26a22ef8732404022109d8`;816ae0/77=`689a8564a1cc89ce3e85499a8601bdcff95b86e710ce32676938281488840c49`;0d0e60/62=`29195aef873b3a82bf6b59b65ec22016c7f57aa5dade61079a4eb2d8730e588c`. No new raw artifact was written.

## Candidate owners already available

Study owns concrete HeaderVariableRecord objects; their +24 dimension field and header default3 are qualified. Var::ops is the modeled operation-ID list, and Var::items is the keyword/item count/value projection. The actual public matched Keyword reader still refuses, so tests supplying ops/items must be labeled explicit private value fixtures; a helper must not admit Keyword parsing or synthesize mappings from a flow chart.

Existing `operationById(const Project&,int)` in project.cpp searches that Project's owned Operation rows in order and returns the first matching ID. Existing `opDim(const Operation&)` in objects.cpp maps the native05f0f0 logic. opBase in registry_rescue.cpp reads the Operation's own Project registry without inserting a missing key. A resolved operation must have the supplied operation Project backlink; otherwise its registry read would silently select a different owner. Standard known class dimension branches (or absent-class default3) may use this existing accessor; existing unsupported class-specific dim paths must retain their guards. No broad proof of arbitrary external OpClass virtual implementations follows.

The operation Project is an explicit private input, not a forced alias of study.sub->project. Native current selection can differ from the saved Study's Project. Never consult a global current Project inside the helper, fall back to the Study's Project, choose another operation ID, or use Operation's cached opClass pointer in place of opBase's registry lookup.

## Smallest exclusive API and behavior

After root assignment, reserve NEW `src/doe/variable_dimension_prefix.hpp/.cpp` and NEW `tests/doe/test_variable_dimension_prefix.cpp`. No existing shared owner/Session or Type3 file is required. A suitable provisional API is:

```
VariableDimensionPrefixResult reconcileHeaderDimensions(
    Study& savedStudy, const Project& suppliedOperationProject);
```

The private result distinguishes SuppliedRowsCompleted from KeywordItemSnapshotBoundary, retaining changed-count/current-index diagnostics. It must explicitly remain incomplete for public Save, not act as a caller-minted reconciliation-success receipt. Native d2b60 itself returns void; these are host diagnostics only.

Validate coherent actual concrete HeaderVariableRecord ownership/borrowed aliases before mutation; foreign/null/aliased ownership remains a reconstruction bound. On that stable supplied-owner profile, take a local borrowed snapshot, visit original order, read first ops ID (or-1), use operationById, then verify the matched Operation belongs to the supplied Project and obtain opDim. Preserve a matched getter/owner refusal at its encounter, retaining earlier completed dimension stores. If unchanged, continue without inspecting items. If changed, store dimension first. Empty items can continue with no further model effect; nonempty items return the explicit boundary immediately after the retained store, before actual keyword-list/data/text conversion, leaving later rows unvisited.

The native QList snapshot/ref/allocation and first-null pointer representation are not modeled by the host Item values. The helper's empty-item continuation is a value-effect projection, not literal COW/ref/allocator parity. Never treat a nonempty Item value as proof of a native live first keyword pointer. No keyword text, nominal/upper/lower conversion, sample invalidation, reindex/renumber, link change, row removal or file/notification effect is included.

The supplied-owner helper snapshots its stable admitted collection at entry. It does NOT reproduce the native earlier snapshot across global lookups. Future public binding must carry the actual pre-global variable snapshot/source interval through those calls, preserve its owner/lifetime and reconcile against the actually selected Project. Recopying current vars after unknown global activity is not proof of the original snapshot. No private argument or diagnostic result can supply that missing occurrence.

## Focused test contract

Use real Session-owned lazy-imported header records, then explicit private ops/count values and actual supplied Project-owned Operation rows. Cases:

- first operation only: missing first ID skips even when a later ID matches; empty ops selects-1 with no match; missing operation preserves dimension and does not inspect nonempty items;
- a known2D class changes3->2; absent registry class yields3; equal dimensions skip even nonempty items; choose existing source-qualified classes/default rather than a callback-success stub;
- changing a record with nonempty items retains its dimension then stops before keyword work/later rows; earlier changes persist. With equal dimension on that record, traversal may proceed normally;
- matched foreign Operation Project backlink or existing unsupported class getter refuses at encounter without undoing earlier stores; malformed Study ownership refuses before any store;
- sample flags/tables/counts/mode, IDs/indexes, Description, links, files, selected index, I/O journals and notifications remain unchanged;
- distinct supplied operation Project controls dimension without replacing the Study owner or requiring false equality with the saved Project;
- public Save on the untouched nonempty fixture still refuses at VariableDimensionReconciliation before prune/XML. Directly calling this helper does not count as closing native global selection or justify public integration.

The helper tests may compose a known private dimension stage and CP137 prune explicitly to inspect value order, but must not report that as production native Save progression or skip the earlier owner contract.

## Exact remaining dependencies

Before public integration, the same Save occurrence must resolve process/main-window/global dynamic type, actual current MO/Project and lazy Mo+19c8 SfMoOprManager constructor/publication or an actual earlier null-return branch. A candidate Open manager receipt is explicitly not that witness. The pre-global variable snapshot must survive that interval. Only after qualified reconciliation return can public prune occur before XML open. The ordinary empty-variable unsuppressed branch separately needs invocation-bound refresh key codec/data and actual map/list/receiver/timer/normal return. Neither a supplied Project helper nor the private scalar/INI/prune stages closes them.

Source anchors inspected, not edited: objects.cpp `9B61208E180FA6385BC8D25C57200EFCD37824594C3A9BE209EC90CB3F511BA2`; project.cpp `00778DFA9E2D05ACF091AAD5D459D46B7F54F11720A8CD41E6057910497E5868`; registry_rescue.cpp `FB341CB7BECF1322C4B253F0C76DAD0414551535A6A405970455D6A101524F9D`; CP137 doe_study.cpp `D9DEECAE5E5AF237778E842120B05337DCE37A1AB97F098BB8FFDA7014DC3E2B`. Public guards remain and no source release or test pass is claimed by this evidence lane.
