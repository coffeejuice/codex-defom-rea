# CP160 RINHERIT: Report release chain and the actual Close occurrence

The immutable PE identifies the Report document's deleting target and its ordered path into QObject destruction. Together with CP143, this closes the **conditional static** accepted-Close → Project → SubProject → live document deletion chain. It does not establish the actual same-document occurrence or provide a source-owned Report object. Keep public Report Open/Save and accepted-Close guards.

CP156's claim that the SubProject destructor and setter are still missing is too broad. CP81/142 already recovered the destructor and CP50/128 identified the setter; CP143 recovered the manager's indirect SfMoModel deleting target. CP149's narrower retained-subset search did not invalidate those findings. The remaining task is owner/current-state production across callback-sensitive boundaries, plus document payload effects, rather than another search for an absent direct destructor call.

Only this report was written. No source/build/test, GUI/app-control, original execution, mutable REA/database operation, live REA retry, or deployment action occurred. The reverse-engineer-anything workflow used retained qualifications, DUMPBIN 14.44.35228.0, and section-aware offline PE reads. No new REA Evidence ID is claimed. Parent recovery status was read at entry; its GUI-release blocker was respected.

## Conditional native chain already available

CP159 distinguishes queued posting from delivery and actual Close admission. Once delivered or directly selected on the actual main M, native 1400299a0 still performs fresh busy/current-main, can-close/modal/nested-Save, accepted callbacks and null refreshes. Only afterward does a fresh M+50 read supply the manager for 140c78300. This is not a promise that those predecessors return normally or preserve earlier pointers.

CP143 binds manager's actual nonnull child deleting call at 140c784f2 (vt+18, flag1) to SfMoModel table 142112878+18=140045350. The chain is 140045350 →140044040 →1400440c0 →140044bf0. The latter first calls feature cleanup on current Mo+19c0=P, then **rereads** that slot before 1400453e0 →14008a3c0 →1400a80e0. Project's ascending +58 list loop selects SUB and calls 1400ac330(SUB,1) →1400b58d0. SUB+48 deletion/clear precedes 1400b5bb0(SUB); raw flow-model+0 deletion follows it.

Rechecked 1400b5bb0 reads SUB+30 twice. An initially null read returns without explicit deletion or assignment. Otherwise it uses the second actual read as the receiver, calls vt+18(flag1) if that read is nonnull, then calls 140cbc920(SUB+30,null). Preserve the reread and callback return; a cached document pointer cannot replace them.

Rechecked 140cbc920 compares *slot with the incoming pointer and, only if different, invokes IAT141e058b0. The actual import is QtCore4_64.dll `QMetaObject::changeGuard(QObject**,QObject*)`. This is guard bookkeeping, not an evidenced shared-reference decrement/last-owner protocol. Explicit object deletion occurs in the separate virtual call. The setter alone does not destroy the document.

## New concrete document destructor target

Report accessor 1400b5ad0 publishes the return of 140643100 into the selected SUB+30 only after construction returns. The constructor calls imported QObject(parent) at 14064312b and installs table **1424e0098** at 14064313e. Its complete-object locator 1430e6020 resolves RTTI name **`.?AVSfSessionManager@@`**. This native type name qualifies the object used as the Report document; it is not a new candidate owner type.

The table's +18 entry is **14064f060**. Its complete scalar deleting wrapper calls **1406432f0** at 14064f072 and, after ordinary destructor return, scalar-frees the same allocation at 14064f087 when flags&1. For a still-live object retaining this constructor-installed table, the SUB deleting virtual therefore resolves to this wrapper.

The ordinary destructor reinstalls the same table, calls **140643400(this)** at 140643316, then destroys QDomElement members +60/+58/+50, embedded +40, guards +38/+20, QString +18, and finally calls IAT141e042b0 at **14064338e**, verified as **QObject::~QObject**. The wrapper's scalar free is after that imported base destructor returns. These are ordered native calls, not a claim that their callback/exception behavior is fully reconstructed.

140643400 first invokes the imported +18 string assignment with the imported null QString. Then it reads guarded **document+20**, conditionally calls that actual target's vt+18(flag1), and clears its guard through 140cbc920. Next it conditionally deletes raw **document+30** through vt+18(flag1) and stores zero. It then calls 140646e90 and 14064be50 before returning. These are document fields, distinct from **SUB+30**.

The complete 140646e90 body examines document+40, obtains an entry through 140c51720, calls 1406589a0 on it, conditionally invokes entry vt+18(flag1), and repeats its native loop. Its count/empty/accessor helper semantics and entry type/lifetime are not closed here. 14064be50 conditionally calls 1402a2bc0(document+48,1), then stores document+48=0. No QString-only or empty-document assumption can bypass these predecessors to QObject destruction.

The retained QtCore export identifies QObject::~QObject as 6712f870. Its selected tail 6712fd40..6712fdbf conditionally calls exported QObjectPrivate::deleteChildren (6712d300) at 6712fd54, then later calls 6712d3f0 with null parent at 6712fd94 when private+10 is nonnull, before private destruction. Its earlier body also has signal/hook/connection work. This supplies conditional base-destructor ordering only. It does not identify this document in any main-window child list, establish the actual accepted parent, or prove which owner first destroyed it.

## Exact unavailable owner and next action

There are two independent barriers. First, the candidate does not produce the actual application/main M, its Session manager in M+50, current-main W and phase/busy/modal state for the command occurrence (CP159). Second, Report's document constructor, current guarded payload and callback-sensitive owners are absent from the faithful candidate lifecycle. A supplied identity token, null defaults receipt or adapter destructor does not supply either producer.

The minimal same-instance evidence must bind the selected Open/Save Project P, active SUB S, S+30 document D and actual accepted QObject parent H; then follow every admitted rebind/death through Save and accepted Close's fresh manager/Mo/Project reads. At 1400b5bc2/1400b5bd5 record current guard values, not the historical pointer. If D is live, bind its actual vtable and payload +20/+30/+40/+48 through 140643400, and order its QObject destructor against H's child deletion. If the guard is null, identify the earlier destruction/invalidation occurrence. Parent-constructor argument alone is insufficient because parent acceptance and callbacks remain conditional.

For further immutable analysis, start at the first actual nonnull payload producer for SfSessionManager+20/+30 and the list+40 entry's 1406589a0/virtual call; preserve their relation to the published D. Do not repeat the now-known SUB destructor, setter, SfMoModel target or scalar document wrapper. A future same-instance original trace is an alternative only after the supported GUI-control release gate changes; no such capture is authorized by this report. Save's 140645030 effects remain an independent contract. No source-ready public admission or new private owner helper follows from these static bytes alone.

## Immutable receipts

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Half-open VA ranges below are hashed by `tools/doe_static_pe.py` section mapping. DUMPBIN syntax was `/disasm:bytes /range:0xBEGIN,0xLAST_INCLUDED target`; complete functions end at RET, with .pdata used to identify bounds. Partial selections do not claim whole-function closure.

| Range | SHA-256 |
| --- | --- |
|140cbc920..140cbc957 setter|`fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8`|
|1400b5bb0..1400b5c2b SUB Report cleanup|`2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9`|
|140643100..1406432e6 document constructor|`740045663cc135068e07e76fd105375b878596671f29d843b708fb3d56adf37e`|
|1424e0098..1424e00b8 first four vtable entries|`3bdbe559ce44aa72da09e24261287e3a860f9dd742204465d8accae3a3399fca`|
|1430e6020..1430e6038 complete-object locator|`52a36a5a2f6e75d6d7fdf00d881bb34d39dc67ee676f611e1db91463c3d2ce04`|
|14064f060..14064f096 document deleting wrapper|`18e1627d1092136d445b51dc291a7400527c0fb5c824791765e3ae5b185e0b53`|
|1406432f0..140643399 document ordinary destructor|`ae23368312937058d61d2fbc62d89954912196373c865e79260155071aff17cc`|
|140643400..140643503 document payload cleanup|`f011b8599bb96ce2259ef36b927067af8c28f6bce1bb22b8bd2dd477dd7ec650`|
|140646e90..140646f3d document+40 loop|`9f0ed9dafdf1d3a4db14c72e5a59932847743061edbcd6a6dbe82f80f3e220fa`|
|14064be50..14064beb6 document+48 cleanup|`39b3c6de27ef2744bf20268583a4cffce2a3f13eb25188be5179ad8797d3d42d`|
|QtCore6712fd40..6712fdbf QObject selected tail|`73a01cc1b182cfe7e80b51e829d252e6d71644382e0afffc5ce240e1bf419a55`|

Read receipts: CP156 inheritance `813a3ff02f16b9343f186bbe94ebe8862cd21b625f936223d6cd6260d9d4198a`; CP159 Close `1427b9ef3535541a65689c50896a1658cc535d7958cbe7fb245e9098292b7076`; CP143 indirect-owner `9d7186d66aafd6b33954a96238b7cb5a43392d39765de25cf8829eff6bcb185b`; CP142 SUB disposer `711f8a8940a89713c23f5825b192a24a2bbf53d5093529823ef6ef7eeb765533`; CP128 setter/guard `245379dd9f020a3a650f48ebfe3971a07d4369e93fa3e620f6ee3f07d07e6fb1`. These current file hashes supersede hashes quoted by older reports if those report bytes have since changed.
