# CP144 high-frame/cache design review

No blocking design defect found in `resume144-geometry-high-cache-retirement.md`. The proposed cut can complete the already-entered high producer reader for actual zero-presence/skipped main objects, retire only its exact local frame, then execute a separate cache1 builder getter. This accepts the bounded design for source assignment, not an unreviewed implementation or runtime pass.

Only this report was written. No source/build/test/GUI/original execution or mutable REA access. I reviewed CP143's released Viewer/producer test, the existing low completer/step-borrow guard, CP135's which0 qualification and the native ranges below. Original retained DEF_GUI_MO_64.exe v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## First frame and effect order

The CP143 producer already installs a frame before its actual directory query and records successful step-owned allocation/count/cache1 in the same initial borrow. The new continuation must stay inside that callback. It must not call `withStep` again after installation or reinterpret the now-set cache as a reason to skip the remainder of the original active reader.

The source order is allocation/cache1, caller arrays checkpoint, actual ordered main-object decisions/real node helper, node Stop, reader AL1, outer version tail, optional N+8. Native14082dc81..dc8b is exactly AL=1 plus epilogue/RET after Stop. CP135's bounded which0 walk covers high6000/10999/11000/INT32_MAX; only the current-version getter occurs in the outer tail. No second reader or hidden high-version mutation must be inserted there.

The plan correctly keeps a parallel high completer rather than weakening low postVersion<6000. Implementation must require the exact installed shared frame/control block, live same model/current borrowed step, completed NodeSkipLoopCommitted/Stop, nonzero query result, high matching postVersion, id11/flag1/which0/dimension3, current DB/receipt identities and successful private producer receipt. Require current cache1 and actual A pointer equal to the recorded produced pointer, plus positive count equal to the guarded current maximum+2. No token supplied by a caller or generic pointer-length pair can replace those internal facts. Validate any variable arithmetic bounds before addition.

Retire only that exact initial invocation. The raw view/item/geometry/mesh/node/element identities belong to the same synchronous owner graph; preserve their prepared binding and recheck at any callback-sensitive seam. Unknown/changed owner, negative arrays count, malformed MAR selector, state/flag mismatch or nonzero main presence must preserve committed query/A/cache/arrays effects and keep the frame parked. Existing pending entry guards then prevent replay even after raw field repair.

For nonnull DB, existing `completeGeometryNodeSkip` remains zero-only: its positive-presence mismatch predicate explicitly requires null DB. Do not widen that predicate, accept equal widths, invoke allocation/copy helpers or form dead null/out-of-bounds MAR record pointers. This cut consumes no produced A pointee during main assignment.

## Separate cached builder getter

The second getter belongs after the builder's node count/element count/width reads. The proposed explicit `BuilderHighCached` round must perform its own real coordinate COW, then recheck actual same current node/element owner relationships and high/cache1/non-null-DB profile before choosing any reader/query path. A callback changing cache0, version, owner or supported profile must refuse after its COW, preserving the first reader's completed effects. It must not enter the new producer a second time or replay a previous frame.

Native cached reader14082d876 checks the actual cache after its entry/table setup; nonzero branches14082d880 AL=0 ->14082dc83, before Start/Read/loader/arrays/Stop. The outer which0 ignores that AL0 and still returns coordinates. Requiring reader AL1 here or inventing a second Stop would be wrong. The builder must consume the explicit round's returned coordinate/connectivity values directly, without a third legacy getter.

Use this explicit cached round for the supported high-produced/cached paths on later fresh refreshes as well; merely tagging the just-produced invocation while falling back to the legacy unchecked second getter on its next refresh would lose the stated post-COW guarantee. Existing entry-cache1 control remains supported when its actual profile qualifies; cached reads need no new A extent/allocation/query because they do not consume A. Connectivity-null returns known Skip after the completed first getter and must not run a second getter. No completed-frame token or span capability is retained across invocations.

## Test plan acceptance

The proposed test matrix meaningfully discriminates one reader/two getters from CP137's two uncached null-companion readers. Require versions6000/10999/11000/INT32_MAX, optional widths0/4 and metadata2/3: two COWs with second observed after first Stop, one arrays append, one Start/Read/Stop, stable actual A/cache/version, and known mesh/edge bounds. Fresh cached refreshes add only cached getter COWs; they must not query, allocate or replay timer effects.

Keep a connectivity-null edge-only case with one getter. Preserve sticky producer cohorts by explicitly seeding negative arrays counts before their snapshots, rather than deleting their unsupported assertions. Add actual nonzero main/equal-width and foreign-owner/state/MAR refusal cases retaining the first producer. Use the actual owned allocator for any main-storage fixture and restore mutated pointer/owner fields with RAII before destruction.

The second-COW cache0 mutation is essential: observe first Stop, require exactly two COWs but no second Start/query/allocation/arrays/Stop, unknown scene, then a fresh cached success after restoration. Owner/version mutation cases would strengthen the same boundary. Later dimension/numeric/connectivity refusal must occur after both getters and after proper first-frame retirement; restoration must not rerun the producer. A high arrays-timer capacity case would additionally check no-op-at-capacity semantics, without inferring a missing reader from unchanged arrays count. Those extra mutation/capacity cases are coverage recommendations, not a newly found semantic blocker.

Existing low-frame retirement, null-companion repeated-reader, cached control, equal-copy and public editor/import guards must remain unchanged. Root should inspect the released implementation and run `m2m_mdt_geometry_a_producer`, `m2m_mdt_viewer_box` and `m2m_mdt_geometry_node_width_mismatch` before the source-stable full suite. This report certifies no source patch or tests run.

## Independent qualification

All four proposed half-open native hashes independently matched:

| Range | SHA-256 |
| --- | --- |
|14082d863..14082d887 cached exit|`1db2147f23ddf03981eab6c33498068f8a70526df518ae99e02796d0626bde8c`|
|14082dc81..14082dc8b post-Stop return|`abf6edcf823af1d7ea5d32708a4c942450a09b6873f4dbe959a9963193256892`|
|14082dca0..14082e9e0 outer dispatcher|`f985e7ddbd4218a68c500775f28e6f8e231ff0b43e96f557679927a023b10454`|
|140854af0..140854b59 coordinate getter|`5457765a83e18b68d8ceda00e7c2d2a7af3517f4b28f510a2339454e7e7aed6b`|

Plan SHA-256 `4ec4138f1e9a3bd1a07a8fd5c046d61e7105fe665bf091f46ad71bb2b8f382ae`; CP135 tail qualification `resume135-geometry-next.md` SHA `e50275898a872abddb159cea7242ab4e2f327754e00d81d369f0bd9178ca914a`; CP143 implementation source review SHA `5b79191ccec07ce08edb95909be445723ea1927a699503727948f17e189e78b0`. The reviewed starting Viewer/producer-test hashes are CP143 `08211fd9e400d1224ff9b8559fae33ef6b3be2b3e8b2574c459b1dd35abbedc3` / `f96d671f0c157478bba7a70cb4543c89ae39339ff131006da4973e34ac1dbfc8`.
