# CP131: establish the real context receiver before post-map stores

The caller is source-closed from map return through the pre-string initialization prefix: four caller DWORD stores and three pure reset leaves,55 memory stores in total. The immediate missing prerequisite is receiver ownership. Current M is standalone; it cannot become C+1458 by subtracting an address or attaching a new context after construction. A safe next private cut creates an explicit isolated context receiver before the child map starts, runs the existing8/48-byte stages on its embedded M, then commits this prefix on the same C. Default Project/Close binding stays guarded.

## Exact source sequence

Retained original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Constructor12f10d0 saves C, passes C+1458 to12f1060 at12f10e4, and only after that returns performs:

| Source | Effect |
| --- | --- |
|12f10e9/eb/ed|Prepare EDI=0, EDX=0, RCX=C. These are the next reset-call inputs, not another object producer.|
|12f10f0|DWORD C+1498=-1, independent of map count atC+1490.|
|12f10fa|DWORD C+3c=0.|
|12f10fd|DWORD C+40=0.|
|12f1100|Call12e6fb0(C,DL0),18 stores listed below.|
|12f1105|Call12e6ff0 with the same C/DL0,14 stores.|
|12f110a|Call12e7020 with the same C/DL0,19 stores.|
|12f111a|DWORD C+44=0.|
|12f111d|First call146de38 with destinationC+48 and static source2146f86. Guard before this unported routine.|

The reset leaves preserve RCX and EDX. The first zero-extends DL into EAX for its DWORD store; none changes EDI, so the caller's later C+44 store still uses0. Exact store order/width within each leaf matters:

* 12e6fb0: bytes00,01,02,03,04; **DWORD34**; bytes05,06,07,08,09,0a,0b,2f,30,38,39,3a.
* 12e6ff0: bytes0c,0e,12,14,16,18,1a,1c,1e,20,22,24,26,28.
* 12e7020: bytes0d,0f,13,15,17,19,1b,1d,1f,21,23,25,27,29,2a,2b,2c,2d,2e.

Thus source initialization knows54 flag bytes:00..0f,12..30 and34..3a. It **does not write PST bytes10/11**, gap31..33 or byte3b. The four DWORD stores add16 bytes, so this caller prefix knows70 distinct bytes, plus previously initialized map fields. It is not a whole-context memset. The strict minimum is the first three scalar writes, but the three reset leaves have no additional owner, allocation, provider or callback dependency; the first unqualified callee is the natural coherent boundary at12f111d.

The string destinations later appear in order48,c48,1048,848,448. This report does not assign a host string type or allocator semantics to146de38 without its body. C+1450 is written null only later at12f116e after all five calls; at the proposed boundary it remains unknown. No constructor return, optional-owner default or cleanup eligibility follows from this prefix.

## Receiver and allocator ownership

Current FeatureMapProxyCall::Storage owns MapHeader map directly. Its A backpointer and return identity point to that actual standalone object. There is no enclosing C, and the current completed map must not be copied/moved/rebound into a new context. The existing passive Project epoch is a different owner, not permission to reinterpret it as this receiver.

Introduce a second, explicit **isolated context** preparation profile. It creates a stable private ContextEnvelope before any native child allocation. Its actual MapHeader member is at byte1458; independent signed32 cache is at1498. A possible typed envelope is opaque00[3c], DWORD3c, DWORD40, DWORD44, opaque48[1410], MapHeader map, int32 cache1498 and alignment padding. Compile-check offsets of3c/40/44/map/cache, x64 alignment and minimum backing extent149c. A host sizeof14a0 is a backing/layout gate, not a recovered whole native Project size.

Use an explicit empty constructor so unknown bytes and fields are not value-initialized as native state. Raw storage before3c and48..1457 preserves unmodeled strings, PST bytes and C1450. Do not construct host strings/providers or make a native allocation for C:12f10d0 receives C from its caller. This envelope is host backing for an isolated receiver, not the missing original Project allocation or inline Project+60 provenance.

The same runtime-retained call strongly owns that envelope; its map view refers to the actual embedded member from birth. The existing A store therefore naturally records M=&C.map=C+1458, and H belongs to that same child. Keep old standalone-map preparation unchanged. One internal implementation can select a passive standalone MapHeader versus the explicitly owned envelope before binding its map reference; it must not choose/reassign after any child effect. The runtime owns the call, the call borrows runtime weakly, and no Project or reciprocal strong cycle is added.

Allocator effects remain exactly the existing real8/48-byte child requests from the qualified canonical candidate heap. The post-map55 stores allocate nothing and call only the three bounded reset leaves. Host envelope/control-block preparation happens before those native allocations and can fail as ordinary host metadata allocation. It must not be reported as a native C constructor allocation/failure. Runtime retention keeps C/M/A/H until the bounded owned process ends; no native cleanup or default Project binding is added.

## Proposed private API and write set

Reserve existing src/model/feature_crt_runtime.hpp/.cpp, existing tests/model/test_feature_crt_runtime.cpp for standalone refusal/regression assertions, and NEW tests/model/test_feature_context_prefix.cpp. Existing globs supply target m2m_model_feature_context_prefix / m2m.model.feature_context_prefix. No public model.hpp, Project/epoch, Session, C ABI, CMake or standalone/p11 source edit is needed.

Proposed private methods, with frame/receipt types kept in the existing private header:

```cpp
// FeatureCrtRuntime: explicit profile; no caller-supplied C pointer or completion Boolean.
FeatureMapProxyPrepareResult prepareOwnedTestContextPrefix(
    const std::shared_ptr<FeatureCrtRuntime>& canonicalLease);
// FeatureMapProxyCall: same staged child frame, now with an optional genuine C owner.
FeatureMapProxyReport resumeContextPrefix() noexcept;
```

An internal typed receiver profile selects the context envelope in its private constructor. Factory entry authenticates canonical lease and actual produced heap, then applies the existing one-frame experiment-capacity guard. It must not invoke a kernel producer lazily. Public diagnostic profile/context identity is not construction authority. A standalone frame's resumeContextPrefix returns ContextOwnerUnavailable without stores; it cannot infer C from M or accept a detached parent report.

resumeContextPrefix performs no child driving implicitly. Until the existing explicit map resume/resumeSentinel genuinely reaches candidateMapConstructorReturned for this envelope's embedded M, return ChildMapPending with no outer writes. On success, check C/M address relationship and actual member-owner identity, then write the55 selected stores once under the existing owner/call lock order. Stop at first string-call boundary with candidateContextConstructorReturned=false. A fixed ledger can store sourcePC/offset/width/observed value; knownness is per byte/word. The DWORD34 event must remain one four-byte source store inserted after byte04, not four reordered byte stores or an eager aggregate zero.

Expose only initialized scalar/flag observations. Cache is a signed-1; byte masks leave PST10/11, gaps and string/optional-owner storage unknown. Do not read uninitialized backing simply to demonstrate unknownness. Reports remain candidate identities/source-role execution, never original-global observations or completed Project provenance.

## Failure and test contract

This prefix contains no native allocation or external call beyond the closed leaves and no legitimate failure branch once valid same-owner backing exists. Host preparation failure occurs before child effects. A pending/failed8/48-byte child leaves all outer scalar/flag knownness false; do not reset fields after a failed child or call a native context/map destructor from the guard. Repeated completed prefix calls do not rewrite/reset or enter146de38. Slot/owner mismatch cannot retarget the frame. First string routine, subsequent C1450 store, registration and Close remain unadmitted.

Use a **separate new test executable** for the real context profile: the existing runtime test intentionally consumes the one-map capacity with its standalone receiver. Do not increase capacity or silently change that fixture to construct a different owner merely to run both profiles in one process.

Focused new test gates:

* Actual canonical runtime producer then explicit context preparation; genuine C exists before8/48 requests, and actual M-C offset is1458. Field knownness is unavailable before the map, with no constructor/default-zero claim.
* Premature outer resume performs zero stores and zero implicit kernel calls. Run actual child8 and48 stages, checking A->M, M.head/count and returnedM are that same embedded member. No rebinding/move occurs.
* Commit exact55-event prefix and verify actual signed cache-1, DWORD3c/40/44=0, correct byte sets/order and DWORD34 width. Map/proxy/head identities and allocation counts remain unchanged; first string call is not entered.
* PST10/11, gap31..33/3b, strings, fallback field1448 and optional-owner1450 remain unknown. Constructor/context/Project/Close completion stays false. Fixture byte seeding, if added, is explicitly host setup and never a native producer/default.
* Repeated outer/child resumes do not replay fields or allocations. Dropping local leases retains genuine C/M/A/H, with no cleanup. Foreign leases refuse and second frame preparation preserves the existing explicit capacity guard.
* Existing standalone-map test verifies no context identity is supplied and outer resume refuses without modifying the successful map. Declared child failure must never become a successful outer completion witness; do not force kernel OOM or inject a fake native throw.

If root prefers not to approve this explicit receiver-profile migration, the exact blocker is the absent C owner corresponding to current M; a detached three-word helper would not close it. All field operations themselves are already source-closed, so further provider/allocator discovery is unnecessary before this pre-string cut. The next native investigation after it is146de38/literal/buffer semantics, not defaulting PST bytes.

## Read-only receipt

Read source hashes: CRT header `053ee825f92374cd7a61b88d83381b2ebefb3fcc2bdcb5aca657c998b8b72903`; cpp `cc9c9a72ca0302148615ba51f384a51a291f40bedabda218b6a51dfe9f43e3d7`. CP130 receipt `42a047df675841bf0734a5e7b2feeb53e14a50cfb4b397318f9667319e3e4aa7` records its frozen source; this lane makes no claim about root's still-running freeze/test outcome.

Retained context constructor ASM SHA `38e87349cb26103cb4f4d593fa0ac441ff4b5706283645e6cfdf00255cbe6e62`. Reset-leaf ASM hashes:12e6fb0 `198375e4e1a1a89249a0e96cb29a54a86661e88c264dd1aea6e5986192ecb262`;12e6ff0 `4d1a4a310102a697c44fda7a47431af41f8023344a80598fffeba614fb27f577`;12e7020 `57ad77652ec2ac0305084bb87e63fe53df32f0a67563a389a3e48bc70dd2cbfe`. Semantic leaf spans end immediately after RET at12e6fe9,12e701b and12e705a; padding/next-function fragments in broad retained captures are excluded. CP114/125/127 retain map/heap/owner/Close distinctions.

Only this NEW report was written. No source, test, build, kernel, GUI, original application or mutable REA work occurred.
