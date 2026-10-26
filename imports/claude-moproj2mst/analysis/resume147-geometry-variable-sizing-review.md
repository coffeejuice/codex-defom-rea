# CP147 variable-sizing design review

The zero-size sizing algorithm is source-closed under the proposed actual-owner/empty-values guards. One concrete **test-feasibility gap** remains: the existing test hook does not observe the proposed direct mutable lookup. Resolve that forwarding observer boundary before claiming the28-lookup/late-mutation coverage. No other concrete design blocker was found. This is a read-only design review, not a source implementation or runtime pass.

Only this report was written. No source/build/test/GUI/original execution or REA/database access. I inspected frozen CP146 Viewer, actual mutable lookup/COW and ordinary/post leaf implementations, and independently matched all eight new native byte hashes and all28 registration tuples. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Verified receivers and zero-size effects

Independent extraction from native140853510..1408541f5 found24 normal ctor140b3efd0 calls followed by4 post ctor140b3f060 calls. Ordered normal ids are01,02,03,04,05,06,08,09,0a,0b,0c,0d,0e,0f,10,18,19,1b,1c,1a,1d,22,23,11, in successive8-byte slots010..0c8. Post ids1e,1f,20,21 occupy0d0,0d8,0e0,0e8. Every tuple matches actual `Cls_14017ef20::kRegs`; do not sort the nonmonotonic ids.

Native vtable slots1427eac90 and1427eacc0 contain1409e49d0 and140b3f0e0, respectively. Ordinary key-1 updates dim0 to1, saving prior cur0 in prev0, then dim1 to actual D[id]=0, saving prior cur1 in prev1. Dim2/prev2 are unchanged. Low32 multiplication of1*0*dim2 yields0 regardless of dim2. An actual empty values count0 takes the resize equal-count return with no temporary/vector allocation, detach, data copy or bound-slot publication. The port's early count comparison implements the same branch. The four post receivers reject key-1 before reading D[id] or changing dimensions/values/slots.

Preflight all24 ordinary D[id] values0, actual28 count/order/types/ids/slot bindings, unique registry/value bodies, empty values and null slots. Positive/nonempty sizes must not reach the leaf on this cut; their already-updated dimensions and earlier receivers would otherwise expose unqualified allocation/unwind effects. Post D widths and the six BC D[12h..17h] widths are separate from the ordinary registered ids and need not be0. Positive-request/null BC results may remain recorded failed allocations; sizing must not promote those nulls into valid spans or invent a new BC-success precondition when it only uses empty StateVar storage.

## Mutable lookup, stage and partial-effect requirements

The native dispatcher snapshots count then performs actual mutable lookup and virtual+10 for each ordinal. The port's `def::model::ext::fun_14019a860` calls `CowList::mutable_at`, which detaches first. Execute that actual operation once per admitted ordinal; do not replace it with a const registry read. Revalidate after the actual lookup and before dereferencing/calling an unqualified returned receiver. Completed ordinal advances only after the actual leaf returns.

Initial uniqueness makes the normal lookup a no-detach operation. To fulfill the plan's changed-identity rule, capture the actual initial collection body identity and ordered receiver identities before the loop, and retain/recheck them alongside count/type/id/binding/value-body/source/destination/BC facts. Checking only “current body unique, current count28 and equal ids” after lookup would miss a completed alias-induced detach, whose new body may already be unique. Do not dereference a historical slot pointer after its body/owner changed merely because its address looks plausible.

Use only the initial high callback at MainBcAttemptsCommitted, with successful actual source/main-coordinate receipts, all six BC attempt records/current fields, n1 and untouched seventh BC field. Per-entry rechecks must tolerate the legitimate prior ordinary dim0/dim1 changes while refusing changed owner/span/registry/width. A later rejection retains earlier leaves and lookup effects without restoring dimensions or replaying the prefix. VariablesSizedCommitted remains parked before memcpy/Stop/retirement/builder; no high completer or generic resume is admitted.

## Concrete test-seam gap

The proposed tests promise to observe28 actual mutable lookups and mutate a late lookup after several leaves. In the reviewed source, `members/ext_deps_wiring.cpp::fun_14019a860` directly returns `&sv_list(self)->mutable_at(i)`. It has no observer or callback. Existing `llobj::ext::g_hook` routes different packet wrappers; `llobj/ext.cpp` has no case for14019a860 or native14086e340. The main reset's140329dc0 hook fires earlier and cannot establish the late-loop timing. A subclass spy is also outside the exact normal/post type guard.

Resolve this as an explicitly scoped **forwarding lookup observer/wrapper** in the assigned private lane, or narrow the claimed test coverage. The observer must not supply a fabricated receiver/result, skip the actual mutable lookup, invoke it twice or call the unrestricted whole setter. For an alias-detach test it must be able to place the declared alias before the real lookup, then validate the real resulting body/receiver afterward. Any observer is test instrumentation, not evidence of a native callback at that leaf. Coordinate any needed extra shared file/interface before edits; the stated Viewer/test-only write set cannot silently mutate ext wiring. This is a test-contract blocker, not evidence that the normal bounded leaf algorithm is unsafe.

With a genuine observer, the proposed tests are meaningful: snapshot/order-check actual receiver and values-body identities; verify24 normal current/previous fields and4 unchanged post receivers; mixed/zero BC allocations; one COW/arrays checkpoint/timer2/unknown scene; no data publication; and a late width/receiver/owner change retaining earlier completed leaves with current/later leaves untouched. Shared-list detachment must remain an actual consumed lookup effect, followed by guard. Repeated refresh/raw repair must not rerun leaves. Keep CP146 BC-only cohorts behind a real ordinary-width guard, preserving CP145 negative-BC and earlier completed paths. No OOM or positive resize is needed.

## Independent immutable receipts

| Half-open native range | SHA-256 |
| --- | --- |
|140853510..1408541f5 registration|`aecbe0879c3dece31b36f7266176ab1503e3a296e224ef46458561257f059c17`|
|140b3efd0..140b3f028 normal ctor|`1b330c4cfca4b6e8fdb6cb15caef3107f67ffb3e29c9dcce6420e9444cb1ec16`|
|140b3f060..140b3f09e post ctor|`127204e16b3a3d1a0d82e92c4f071e296119175d086918878b9c18f2032eecf4`|
|1427eac90..1427eac98 normal slot|`6faf07550dd40affcd31cecbe01ab1f4f5e9ad757eaf714c02c15891f47f9ed2`|
|1427eacc0..1427eacc8 post slot|`fddb3978da2365d264c006a432d03cfbb938fc2a8e753bc3b51bc5fa86bc2312`|
|1409e49d0..1409e4a3c normal leaf|`e217c2e3f540250a19f472a3cc73c76e99609827c94f0fe27e2ce8f01cec4b06`|
|140b3f0e0..140b3f145 post leaf|`bddde05d0b080be81b784389b4fedf92abe03bc9c5ce14c284759262c526228d`|
|1409e46c0..1409e494c resize|`29f8e0e8332005c0155c0cd835ac67c4f2e85ef4bd9322e3f61cfd048cc0dbaa`|

Reviewed plan SHA-256 `b4f72cfe8e0a7b5daf0198c81a05ba9445698bae4401c26615714e12c8f5bc85`; frozen starting Viewer `c5eaf75cd46fa68ad5b91b0638b42d8405df1d1fbd3367acc37968ea2d2f0294`; producer test `eb0a452c2cdb048d4a85d99095f9521d3fc3ed345072ca025d636725ae4f6d82`. No production/source or test file was changed by this review.
