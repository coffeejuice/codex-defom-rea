# CP132: independent ContextEnvelope contract review

No blocking error found in the proposed embedded receiver, child-map identity or55-store prefix. The contract is source-closed through the boundary before1412f111d, provided the implementation keeps the envelope passive, constructs the actual embedded map from birth, preserves exact store widths/order and leaves C+1450 unknown. This is a review of `analysis/resume131-feature-context-next.md` (SHA-256 `73ce08f1fe41b53574d332dc02f549905e6af25f376ffe681bc56836d384ec6f`), not approval of an unfrozen source implementation or executable.

Only this report was written. No runtime/source/model/CMake edits, builds/tests, GUI/original execution or mutable REA database. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Native addresses below are full VAs with the common14 prefix omitted when convenient.

## Receiver layout and identity

Native12f10d0 saves receiver C, passes **C+1458** to12f1060 at12f10e4, then initializes independent context fields after that child returns. Proposed opaque00[3c], DWORD3c/40/44, opaque48[1410], MapHeader map and int32 cache1498 is consistent with this sequence:

- opaque48 ends exactly at1458; it includes uninitialized C+1448 and optional C+1450 without inventing their types/defaults.
- Current MapHeader has proxy+0, opaque+8..2f, head+30 and64-bit count+38, giving a0x40-byte extent. Embedded M therefore occupies1458..1497; M.count is atC+1490, separate from the signed DWORD cache atC+1498.
- Minimum written receiver extent is149c, rounded to14a0 for x64 alignment. That is a host backing/layout check, not proof of a whole native Project size or Project+60 provenance.

Actual offset/size/alignment static_asserts belong in the implementation; arithmetic alone is not a compiler layout test. A MapHeader reference must bind once to the genuine embedded member before A/H kernel requests. The old standalone map remains a separate profile and must not be copied, moved, rebound or used to infer C by subtraction. A->M, map-return identity, head/count and later prefix must all name that same member. Stable host ownership must outlive every inspection lease and partial child stage.

An explicit user-provided empty envelope constructor, with the existing empty MapHeader constructor, can leave scalar/opaque backing uninitialized. Value-zeroing, a context memset, construction of guessed host strings/providers or observing unknown bytes would violate the proposed knownness boundary. Host backing allocation is ordinary preparation before native child effects; it is not a recovered native C allocation/failure.

## Independent store ledger check

Read the full retained context constructor and all three reset leaves. An independent bounded parser counted only instructions inside each leaf's semantic span, excluding padding and adjacent functions. Results match the proposal:

| Phase | Stores | Exact qualification |
| --- | ---: | --- |
|12f10f0,12f10fa,12f10fd|3|DWORD cache1498=-1, DWORD3c=0, DWORD40=0, in that order.|
|12e6fb0|18|Bytes00..04, **DWORD34**, then bytes05..0b,2f,30,38,39,3a.|
|12e6ff0|14|Bytes0c,0e,12,14,16,18,1a,1c,1e,20,22,24,26,28.|
|12e7020|19|Bytes0d,0f,13,15,17,19,1b,1d,1f,21,23,25,27,29,2a..2e.|
|12f111a|1|DWORD44=0, after all three leaves and before first string call12f111d.|

Total55 stores. The51 leaf stores initialize54 distinct bytes because DWORD34 covers four bytes; adding the four disjoint caller DWORDs yields70 distinct initialized bytes. Under3c, exactly10,11,31,32,33,3b remain unwritten. Thus PST10/11 must remain unknown. Cache-1 requires signed32 interpretation, rather than a count alias or pointer sentinel. The DWORD34 event must stay one four-byte write after byte04, not a sorted byte loop. These leaves preserve RCX/EDX and do not disturb EDI=0, so the caller's final DWORD44 store remains qualified.

The source prepares first string-call RCX/RDX at12f110f/1113 before storing DWORD44; those register/address preparations do not add memory initialization. Stopping before12f111d must not read the first buffer/literal or claim completion of that callee. Later destination ordering and optional-null write are outside the prefix.

## Destructor and failure interactions

The full context destructor12f0d20 first tests its feature gate and then **reads C+1450**. Constructor12f10d0 does not write that field until12f116e, after five string calls. It is therefore invalid to invoke12f0d20 on the proposed pre-string checkpoint or infer optional-null cleanup eligibility from completed child-map construction.

Host destruction of an envelope/metadata frame must stay passive. A pending8/48-byte allocation, guard at the first string call, dropped inspection lease or a host preparation failure is not a native constructor throw/EH event. Preserve M/A/H; do not call map/context destructor, free sentinel/proxy, unregister providers or activate Project/Close cleanup automatically. Actual native exception cleanup is a separately qualified occurrence and cannot be manufactured by a staged report.

The independent CP131 empty-map destructor proposal is compatible only as a separately admitted isolated action. It must not interleave with this context-constructor cursor or be used to complete it. Root must coordinate any later shared runtime edits/admission-state handoff. The context constructor's post-map stores neither execute map clear nor grant accepted Close authority.

## Review gates for root integration

Before build, check the frozen implementation against the proposed profile selection and fixed55-event ledger: no implicit child driving; no outer knownness/stores before actual child return; no host allocation/callback during the store phase; same embedded M and immutable standalone behavior; signed cache; unknown masks without raw unknown reads; and repeated calls not replaying stores/allocations. Use the new separate context test executable because the retained standalone test consumes the runtime's one-map capacity. Do not enlarge capacity to hide an owner conflict.

The report does not replace source review, compilation or actual kernel/test results. No implementation finding is asserted against a source file that this lane did not freeze/review.

## Evidence receipt

Retained constructor `analysis/resume112-feature-constructor-1412f10d0.asm` SHA-256 `38e87349cb26103cb4f4d593fa0ac441ff4b5706283645e6cfdf00255cbe6e62`; reset-leaf ASM12e6fb0 `198375e4e1a1a89249a0e96cb29a54a86661e88c264dd1aea6e5986192ecb262`,12e6ff0 `4d1a4a310102a697c44fda7a47431af41f8023344a80598fffeba614fb27f577`,12e7020 `57ad77652ec2ac0305084bb87e63fe53df32f0a67563a389a3e48bc70dd2cbfe`. Parsed leaf endpoints are12e6fe9/12e701b/12e705a, end-exclusive. Context destructor capture `resume116-feature-lifetime-1412f0d20.asm` SHA `7e96bac9d096e27d7a4901141496305e0de0c63e6954bcd3c12ff54d31210212`. CP130 sentinel receipt and CP131 destructor report retain child-map/heap/free qualifications.

No reconstructed executable/deployment was inspected or changed. Default Project binding, complete native context construction, provider registration, EH and accepted public Close remain unclaimed.
