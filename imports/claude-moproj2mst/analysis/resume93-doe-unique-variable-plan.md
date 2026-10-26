# CP93 unique-name variable append on the actual owner

The next smallest native-evidenced production advance is **additional distinct-name header/Description variables on the existing concrete Study owner**. Constructor, pointer append, name lookup, selected index, invalidation and reader-last-variable order are independently recovered. A Keyword path would additionally need concrete keyword ownership plus unresolved SetOprHasDoe delivery before later scalar setters. It is not the smaller source-closed branch.

This report is read-only while CP92 validation/archive is pending. No source, build, CTest, GUI, native process or mutable analysis database was changed. No new capture was necessary; the complete original bodies retained in CP50 and CP91 close this cut. Source edits require CP92 archive and root's explicit assignment.

## Independent original evidence

Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The completed reader0x14090ee20/code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08` is bound by `resume50-doe-variable-body-pe-receipt.json`. Complete append/constructor/reindex bodies are in `resume91-doe-variable-lifetime2-static.json`, SHA-256 `548F133506F32B57E0BAE1130F1CD5A62DD9506E9BACE0030216A34E450A0FAE`.

This lane reread the full same-name existence helper0x14091d970 in `resume50-doe-variable-record-pe.asm`, SHA-256 `C048978836AE3939E3C4645484AFA981D36FD2E671630327E61A2B84A5CE40C6`; name-to-index0x14091b5d0 and record-at-index0x14091b950 in `resume50-doe-variable-object-pe-compact.asm`, SHA-256 `8DE9917F84D10D1FEE4F450C31E60D0408DFE531A5F840D428489DC7A4667D4E`; and the actual append, current false-assignment flag, next-number scan and final reindex captures. This is variable-specific evidence, not an analogy to CP90 Constraint ownership.

## Exact decision, owner and normal order

Installed manager vtable0x1421671b8 slots are +f8=0x140919de0 append/create, +130=0x14091d970 exists, +180=0x14091b5d0 name-index, +1a0=0x14091b950 at-index, +1d8=0x1400d7f20 pointer-list snapshot, +1e0=0x14091c7a0 reindex, +1e8=0x14091da70 next-number. The snapshot helper copies manager+38's pointer list; it does not reindex or create new records.

Same-name existence helper scans current manager+38 variables in list order, obtains each name through0x1403047c0 and tests imported QString operator==(QString) at IAT0x141e03f08. It stops after a match. Name-index scans from0, returns the first exact matching index or-1. This is content equality with case sensitivity; no case folding or codec conversion is invoked. The current QString value seam compares UTF16 content and may be used at that established abstraction level without claiming temporary/ref allocation parity.

For a **new distinct name**, the append helper's sequence is:

1. No existing name -> allocate0x48 and construct0x140816900(samples).
2. Append its pointer to manager+38 through0x14172d7e0 before assigning name.
3. Assign name through0x140dfbd00.
4. Invoke vt+1e8 next-number scan. The reader supplied false at0x14090f091, so the helper does not store its result as a record number.
5. Look up the new name and store that index at manager+74. For a unique append to an ordered list this is its newly appended position, not a hard-coded0.
6. Mark all three sample-invalid bytes true; mode0 skips0x14091d110 design acquisition. Existing tables and prior records are not cleared.
7. Return to the reader. It ignores the helper's returned pointer, calls count-1 then at-index and uses the last record for varId/dimension, LinkTo and Description children.
8. On complete DoeVariables traversal, reindex every record to its current position, then resolve deferred links. The bounded no-link profile has no deferred effect. Later top-level children and the separate outer ID pass remain in their existing positions.

The next-number helper scans **all current variables including the newly appended constructor-number0 record**, with signed comparisons and wrapping32-bit add. Its returned value may differ from1 for later rows and may wrap. It is still discarded because the false assignment flag controls the setter. Do not assign sequential/generated IDs or add a lowest-free-ID pass: parsed positive varId remains the reader's following store, and the existing outer pass skips positive IDs. At the current value abstraction the discarded query adds no persistent field mutation; do not invent query journal events to simulate Qt temporary/ref behavior.

## Duplicate names remain a distinct guard

On an existing name,0x140919de0 skips allocation/name/selection/invalidation and returns the existing matched object. Nevertheless0x14090ee20 ignores that pointer and reads **the last manager object** for subsequent headers/children. For example existing A,B followed by a row named A would direct its later setter body at B, not A. Correct behavior cannot be obtained by overwriting A, deduplicating rows, or appending another A.

The proposed cut keeps any duplicate name unsupported before current-row mutation. That is a reconstruction admission guard, not native input rejection or a native failure-order assertion. Duplicate rows' broader parser effects are not released merely because their branch is visible. Existing tests that use the same fixed name twice still exercise this guard and require no expectation rewrite.

## Minimal safe candidate transition

Retain mode0, canonical positive samples/varId, exact dimension3, no LinkTo, exact direct Description-only/raw-child profile and all mixed child guards. Continue rejecting scalar/geometry projection owners and retained scalar prefix. Replace the fresh-empty list test only with a concrete-owner consistency test:

- `Study.vars.size()==Study.headerVariableOwners.size()` and the existing count fits signed32;
- every concrete unique_ptr is nonnull and `vars[i]` equals that exact owner's borrowed Var address in the same order;
- new name is distinct from every existing concrete record name; refuse before allocating on any match;
- guard append when the resulting count cannot be represented by the existing int count/index surface.

This admits repeated source-qualified append on the actual owner; it does not adopt foreign/raw Var objects or assume identity from matching fields. An empty owner still satisfies the original first-row path. A source-created record retained after a later-row refusal can remain an explicit partial state; never reset existing indexes, descriptions, samples or selection merely to make a new admission pass. The helper must use the newly appended index for selected state.

Host ownership remains concrete unique_ptr<HeaderVariableRecord>, not deletion through Var. Secure its host owner before publishing the alias. If current alias append fails, remove only that uncommitted owner; preserve earlier records and all earlier effects. After successful alias append, name/text/header failures retain the reached current record and preceding values; no whole-list rollback. Original Qt COW/reference/raw-null allocation/fault timing and native Close ordering remain outside this value adapter. Existing manager destructor evidence, rather than pointer-node release, establishes native record ownership.

## Exclusive files and focused tests after CP92 freeze

Proposed write set: `src/doe/variable_record.cpp`, helper profile comment in `include/m2m/doe_variable_record.hpp`, and new `tests/doe/test_variable_unique_append.cpp`. No Study/reader/Session/shared writer/CMake change is needed. Existing second-row tests duplicate the same name and remain unsupported, so there is no planned old test edit.

Expected target `m2m_doe_variable_unique_append`, CTest `m2m.doe.variable_unique_append`. Focused cases should check:

- Two/three unique header/Description rows produce distinct stable owned pointers, correct ordered headers/sample counts/descriptions, final indexes0..N-1 and selectedN-1; explicit positive duplicate numeric IDs need not be renamed because name, not ID, drives this helper decision.
- Case-sensitive distinct names are admitted; exact duplicates refuse without another allocation, header overwrite or index finalization. Do not emit native existing-name semantics as fake success.
- Different canonical IDs including int32 maximum are preserved; discarded next-number query does not become generated ID assignment.
- A later unqualified row keeps earlier appended records/descriptions/selection and index-1 for rows not yet reindexed. A subsequent top-level guard after completed variable traversal keeps completed indexes.
- Missing/null/reordered/foreign aliases or owner count mismatch refuse before any current append; do not dereference an unowned Var to compare fields.
- Actual Session-owned lazy read with two distinct variables retains one actual Study, ordered samples/current PAR and no notifications. Same-owner Save continues to refuse reconciliation before XML open, retaining old DOM/record identities and no refresh/output claim.

Root owns compilation/tests after source freeze. This is actual ordered production record import, not Keyword/Geometry/deferred-link completion, native allocator/ref parity, C ABI Type2 selection parity or public Save success.
