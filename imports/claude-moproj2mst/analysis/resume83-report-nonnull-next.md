# CP83: Report nonnull deleting target and actual guard lifetime

The native Report document role has a concrete deleting target: the `SfSessionManager` table installed by constructor `0x140643100` has deleting virtual `0x14064f060` at `+0x18`. It invokes destructor `0x1406432f0`, then frees the allocation only when `flag&1`. This resolves the virtual target for that constructor profile, but does not qualify native deletion of a candidate opaque token. The smallest safe next owner is one canonical document allocation/lifetime, borrowed by both the actual SubProject guard and main-window QObject parent graph. Nonnull execution must remain guarded before payload effects and actual QObject teardown.

Only this report was written. No source, build, tests, GUI, original execution or mutable REA operations occurred. Analysis used retained evidence and bounded offline PE disassembly/RTTI/import parsing. Original DEF_GUI_MO_64 v11.0.2 SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. QtCore4_64.dll SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No reconstructed executable/deployment was inspected or changed.

## Actual target and destructor order

The retained constructor capture (`resume63-report-viewer-bounded-capture.txt`, constructor643100) binds a supplied main-window parent through QObject construction and installs table `0x1424e0098` at64313e. Its MSVC RTTI complete-object locator is `0x1430e6020`, whose type descriptor names `.?AVSfSessionManager@@`. Table `+0x18` is064f060. This is the native Report document's class, not the converter's `ConversionSession`, DOE task-allocation storage or a class chosen merely because its name contains Session.

The deleting wrapper calls6432f0 at064f072, tests flag bit0 only afterward, and calls scalar free14dfc68 at064f087 on the delete branch. It cannot release storage before the destructor returns.

The destructor installs its class table at64330e, then runs these phases in order:

| Source site | Effect or boundary |
| --- | --- |
| 643316 ->643400 | Explicit payload cleanup; expanded below. |
| 643325,643335,643345 | Destroy QDomElement handles at document+60,+58,+50, respectively, through IAT141e0c6b0. These are **QDomElement**, not QDomDocument. Their transitive refcount/free behavior is not closed here. |
| 643355 ->2081a0 | Destroy list at+40; body/payload obligations remain unexpanded. |
| 643364,643373 ->cbc900 | Destroy internal guard members at+38, then+20. cbc900 calls imported `QMetaObject::removeGuard(QObject**)` at IAT141e042a8. It is not cbc920/changeGuard. |
| 643382 | Destroy QString+18 through IAT141e059e0. |
| 64338e | Call QObject destructor through IAT141e042b0. Only this later boundary can perform QObject guard invalidation for the Report object. |
| 064f087, after return | Free document allocation if requested by deleting flag. |

Cleanup643400 starts with `QString::operator=(QString::Null const&)` at643419, passing document+18 and imported `QString::null` (IAT141e04190). It does this **before** any null payload check. Null child payloads therefore cannot justify bypassing the first string-assignment effect.

It next reads guard document+20 through pure getterc1aef0 at643428. Initial null goes directly to64348c without cbc920. Initial nonnull leads to a second getter at64343b, a conditional deleting virtual+18(flag1) at643469, and then cbc920(document+20,null) at643487 after normal return/second-null skip. It then tests raw document+30 at643491: initial null skips to6434ea; nonnull conditionally invokes its deleting virtual+18(flag1) at6434ca and stores raw+30=null at6434e2 afterward. Finally it calls646e90 at6434ef and64be50 at6434f9. These bodies and the live nonnull payload dynamic types remain unresolved here.

Constructor643100 initializes internal fields, but calls6433a0 and transitive64b890 and performs notifier registrations before returning. Constructor defaults cannot be replayed onto a late document. In particular, do not infer current internal+20/+30 null merely from their construction stores.

Three cells must remain distinct: **SubProject+30** is the external Report QPointer; **Report+20/+38** are internal guards; **Report+30** is a different raw payload. The path/string+18 is also unrelated to status-message/private strings or the process message cache.

## Post-delete external QPointer continuation

CP82's helper0b5bb0 retains its exact branch contract:

1. First getter0b5bc2 of actual SubProject+30 is null: return0b5c26; no delete and no cbc920.
2. First nonnull: second getter0b5bd5; nonnull invokes the current target's deleting virtual+18(flag1) at0b5c03. Second-null skips that call but still proceeds to the final guard assignment.
3. After normal virtual return/second-null skip,0b5c21 calls cbc920 on the **actual then-current SubProject+30 cell**, requesting null. Equal value skips without a store or Qt call; unequal calls `QMetaObject::changeGuard` atcbc947. The unequal provider effects are not discharged by this report.

The native QObject destructor provides a genuine guard-null mechanism. In retained QtCore QObject::~QObject6712f870, current private flags are updated, then bit9 set and bit0 clear selects6712f8bd ->6712de20. In that guard traversal,6712de40 zeroes r14;6712df75 loads a registered pointer-cell address from record+18;6712df79 writes zero into that cell. This is a direct null producer for a selected registered guard, not an inference from C++ weak_ptr expiry. Its registry lookup, synchronization, traversal and transitive effects are not claimed fully implemented. The native Report QObject profile alone does not prove this particular registration/current private state or that it has reached the boundary.

If that traversal nulls the actual SubProject guard while this exact Report dies, final cbc920 takes its equality skip. If a callback replaces the guard before return, the final assignment addresses that replacement cell and may require changeGuard; it does not delete the replacement again. If the deleting virtual has not returned, or payload effects remain refused, final clear must not run. Preserve the reached phase rather than rolling back or reporting delete completion. Continued access also needs the actual SubProject cell to remain valid through callbacks.

Internal cbc900 calls on Report+20/+38 unregister those members from their targets. They do not substitute for external guard invalidation when the Report QObject dies. Likewise, metadata destruction, C++ scope expiry or an arbitrary supplied “delete completed” flag is not this producer.

## Smallest safe owner representation and executable boundary

Current `source/apps/moproj2mst/include/m2m/engine/report_document_slot.hpp` stores Unknown/KnownNull/KnownNonNull with an opaque identity, immutable initial observations and lazily allocated C++ diagnostic scope. It has no actual native document allocation, class profile, parent graph, mutable registered pointer cell or destructor provider. The scope protects adapter progress from stale address reuse; it does not prove native liveness or null the native slot. Ordinary SubProject construction must continue to add no allocation/failure point.

A later authorized increment can add a separate lazy `ReportDocumentOwner` representation, with these boundaries:

- One canonical allocation identity and distinct native lifetime knowledge; Unknown stays Unknown. Bind the643100-returned class/parent/public-private aliases only from actual producer evidence, not an opaque token or nonblank label.
- The actual SubProject slot borrows that identity; the main-window QObject child graph references the same allocation. Do not add unconditional `unique_ptr` deletion in SubProject or a second independently allocated document. Metadata destructors must perform no native effects.
- Source-ordered progress starts at the actual deleting virtual occurrence. First payload state needed is QString+18 with enough actual backing/refcount/nullness knowledge for643419. Internal guard+20, raw+30, later guard+38/list+40 and three QDomElement handles remain separate owners/unknown boundaries. Source-qualified null branches can advance only when their preceding string effect has completed.
- Guard registration/liveness must bind actual SubProject+30 to that canonical QObject and its provider boundary. Add mutable lifecycle transitions separately from CP82's initial-observation API; observing initial null cannot simulate destruction.

Proposed exclusive new files for such a subsequent task are `include/m2m/engine/report_document_owner.hpp`, `src/engine/report_document_owner.cpp`, and `tests/mdt/test_report_document_owner.cpp`, under `source/apps/moproj2mst/`. Any typed linkage change to existing report_document_slot.hpp/.cpp needs root reservation first. This report makes no source edit.

The first possible isolated executable seam is binding that real owner/profile and stopping before643419's unqualified QString effect. It may record a reached destructor entry but cannot claim payload cleanup, QObject expiry, free or final guard clear. A materially advancing string/null-payload prefix additionally requires its actual QString provider and backing ownership; current evidence does not permit a shortcut directly to QObject destruction. Public Report loading remains guarded before actual document construction, so an integrated owner cannot yet be fabricated on Open.

## Exact normal Close dependency

CP82 reconciles the conditional static bridge absent from CP81's direct-call scan:

```text
accepted Close0299a0 -> qualified earlier busy/refresh effects
manager c78300 -> encountered actual child deleting virtual+18(flag1)
SfMoModel table142112878+18 ->045350 ->044040 ->0440c0 ->044bf0
044bf0 ->Project delete0453e0 ->08a3c0
08a403 ->0a80e0 ->SubProject delete0ac330 ->0b58d0
0b58d0: conditional+48 delete BEFORE0b5954 ->0b5bb0 Report release
```

That is a conditional static path, not an observed accepted one-project root or permission to bypass prior phases. In044bf0, live moProject+19c0 additionally selects0982c0 ->0aab30 feature cleanup before Project deletion. Native Project destruction performs its diagram null rebind before the subproject list; the list's encounter order and SubProject+48 predecessor remain dependencies. Public Close must establish the real accepted/modal/busy/refresh path, actual manager child/vtable and Project/SubProject identities, feature cleanup and same-instance main-window/document parent lifetime ordering. Existing default C++ destructors or `releaseProjectAfterCloseEffects` do not certify this coordinator.

If parent-driven QObject deletion occurred first, the Report guard can already be null and CP82's no-delete/no-clear branch is correct. If the actual live Report remains registered when SubProject release runs, the nonnull destructor path applies. Open-failure cleanup and eventual application teardown cannot choose accepted normal Close's branch. Queued/reentrant receivers and the second helper caller1412ae046 remain separately unclassified.

## Focused future validation

The first owner tests should reject missing/wrong class, foreign canonical identity and expired adapter scope without inferring native null. A real owner must remain independent of C++ metadata destruction; default SubProject construction must retain no-allocation behavior. Source progress must stop at QString+18 before all child checks, including when supplied child facts are null; no successful-delete flag may advance it.

When native providers are subsequently implemented, meaningful continuation tests must exercise: first-null direct return with zero clear; first-nonnull/second-null still reaching final assignment; payload refusal preserving phase with no early clear/free; exact QObject registration invalidating the real SubProject cell; equality skip after that invalidation; still-nonnull/replaced cell reaching guarded changeGuard without a second delete; parent-first invalidation; and callbacks that invalidate the continued SubProject owner. Accepted one-project Close remains a distinct root-order integration test with public activation still guarded.

## Bounded evidence receipt

DUMPBIN14.44.35228.0 and PE section mapping were used; ranges below are **end exclusive**, hashed from the immutable files. Import names were resolved from the DEF_GUI import table. No mutable REA session or new raw evidence file was needed.

| Image / byte range | SHA-256 |
| --- | --- |
| DEF_GUI064f060..064f096 deleting wrapper | `18e1627d1092136d445b51dc291a7400527c0fb5c824791765e3ae5b185e0b53` |
| DEF_GUI6432f0..643399 destructor | `ae23368312937058d61d2fbc62d89954912196373c865e79260155071aff17cc` |
| DEF_GUI643400..643503 explicit cleanup | `f011b8599bb96ce2259ef36b927067af8c28f6bce1bb22b8bd2dd477dd7ec650` |
| DEF_GUIcbc900..cbc919 removeGuard wrapper | `13e827f0805e0c97ff985371040318551bf7f8a5ce980fb5a83de5ceca316e4f` |
| DEF_GUIcbc920..cbc957 guard assignment wrapper | `fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8` |
| QtCore6712de20..6712e02a guard traversal | `314a81ae67a7f93581c5e8ec9104820ee9763371ce0ac2545142e194d8abee25` |

Retained qualifications: `resume82-report-destructor-plan.md` (current slot/frozen implementation and root reconciliation), `resume81-report-close-pe.md` (SubProject/Project release bytes), `resume46-close-manager-tail.md` (manager static virtual bridge), `resume63-report-viewer-bounded-capture.txt` (Report constructor), and `resume70-close-private-owner.md` (QObject public/private destructor separation). Unknown payload/provider and actual-instance lifetime edges above remain explicit; a resolved deleting function address does not close them.

## Independent-review reconciliation: exact RVA and thunk mapping

`resume83-report-nonnull-independent-review.md` disputed the COL type and imported-provider labels. A fresh byte-level parse of this same SHA-pinned PE confirms the labels above. The mismatch is not resolved by accepting a symbol that conflicts with a destructor callsite: it is resolved by mapping each section and import-thunk index explicitly. No behavioral scope is expanded by this reconciliation.

The relevant PE section records are:

| Section | VirtualAddress (RVA) | PointerToRawData | RVA-to-raw adjustment |
| --- | --- | --- | --- |
| .rdata | `0x1e03000` | `0x1e01800` | `-0x1800` |
| .data | `0x354c000` | `0x354a000` | `-0x2000` |

ImageBase is `0x140000000`. Table1424e0098 is at raw24de898; the qword at table-8 is1430e6020, mapped to raw30e4820. Its six little-endian DWORDs are `1,0,0,0x378cb40,0x30e6048,0x30e6020`. TypeDescriptor RVA378cb40 lies in **.data**, so its raw location is378ab40, and the decorated name at raw378ab50 (descriptor+16) is `.?AVSfSessionManager@@`. Thus the independent review's COL DWORDs actually agree, but its reported type-name string does not match the correctly mapped descriptor bytes. Do not apply .rdata's raw adjustment to the type descriptor in .data.

For imports, map `OriginalFirstThunk` to the image-name RVA entries and use the same **zero-based** index in `FirstThunk`; a sequential name listing without its correct slot index cannot label a callsite. QtCore's descriptor is raw350a2a0, OriginalFirstThunk RVA350c768, FirstThunk RVA1e03b60. QtXml's descriptor is raw350a2c8, OriginalFirstThunk RVA3515030, FirstThunk RVA1e0c428. All are mapped through their own containing sections. Each import-by-name string below starts two bytes after its entry's hint.

| DLL / zero-based thunk index | Actual IAT VA | Import-by-name RVA | Exact decorated symbol |
| --- | --- | --- | --- |
| QtCore /198 | `0x141e04190` | `0x35164d4` | `?null@QString@@2UNull@1@B` |
| QtCore /233 | `0x141e042a8` | `0x3515f2a` | `?removeGuard@QMetaObject@@SAXPEAPEAVQObject@@@Z` |
| QtCore /234 | `0x141e042b0` | `0x3515f14` | `??1QObject@@UEAA@XZ` |
| QtCore /945 | `0x141e058e8` | `0x3517cd6` | `??4QString@@QEAAAEAV0@AEBUNull@0@@Z` |
| QtCore /976 | `0x141e059e0` | `0x3515460` | `??1QString@@QEAA@XZ` |
| QtXml /81 | `0x141e0c6b0` | `0x3526d64` | `??1QDomElement@@QEAA@XZ` |

For example, QtCore index234's original-thunk qword is at raw350b6b8 and contains3515f14; the symbol bytes start at raw3514716 and spell `??1QObject@@UEAA@XZ`. The previous index233 is removeGuard. QtXml index81's qword is at raw3513ab8 and contains3526d64; the symbol starts at raw3525566 and spells `??1QDomElement@@QEAA@XZ`. Accordingly, the review's QObject/removeGuard and QDomElement/QDomNode substitutions are not supported for these exact IAT slots. The QString providers likewise resolve directly rather than remaining callsite/name mismatches.

The review additionally expands646e90 and64be50. This lane has not independently closed their full payload/provider contracts, so they remain explicit guard dependencies here rather than new executable claims. Accepted normal Close, actual-instance registration, constructor-to-owner publication, transitive Qt destruction and all public activation guards remain unchanged.
