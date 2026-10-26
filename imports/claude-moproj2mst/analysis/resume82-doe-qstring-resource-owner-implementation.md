# CP82: isolated resource QString codec and fresh owner

The isolated null-codec constructor now initializes a fresh resource data generation with one canonical ref cell at 1, then publishes the completed source-slot lease without incrementing it. Unknown/non-null codec states guard before allocation. Actual Session/receiver occurrence is still missing: the production entry always refuses, including when a diagnostic profile says Null. No QFileInfo, native decoder, shared-null count, notification or public Type2 capability is enabled.

## Changed source and interface

`QStringResourceProcessScope` is an explicitly caller-owned process profile, with default Unknown codec and no host singleton or automatic Session context. Codec profile updates carry a required isolated qualification, validate the enum, and advance an epoch. They are not original runtime observations. `prepareIsolatedOccurrence` retains a unique occurrence; `readIsolatedCodec` validates pre-read epoch and records the branch. A stale pre-read admission refuses. Once Null is selected, later codec profile changes do not alter that branch; subsequent occurrences use the new state.

`constructIsolatedResource` requires the selected Null branch and an empty destination. It records a native request of 104 bytes and an explicit isolated Success/ReturnNull allocation decision. ReturnNull retains allocation failure and publishes no data/lease/constructor return. Success invokes a private fixed-resource constructor in the existing `QStringPathOwner`. This method creates the fixed 36-code-unit text and ref cell initialized to1 directly; it does not call `observeInput` with a supplied count or make a second ref snapshot. The resulting origin is `ResourceNullCodecProjection`, distinct from existing diagnostic observations.

The existing owner's alias/copy/release operations share that exact cell. Only read-only owner observation is exposed by the process scope; mutation delegates are limited to alias/ref operations. Source data generation/lease is distinct from text equality and copied observers. Existing diagnostic path-owner APIs remain available on independently created diagnostic owners.

`constructSourceResource` always raises the precise actual Session/receiver source-occurrence guard without allocating or changing a destination. No isolated token can be converted into a production source admission. The assigned scope contains no Session/receiver implementation, so no sealed production admission was invented. Root retains that shared integration dependency.

## Native qualification and representation limits

The source qualification is frozen CP81 report `resume81-doe-qstring-source-owner-evidence.md` (SHA-256 `201734ffb656a8f5b39c73cd12f7aab3537bd865a032b114cc92bb8a2a78ac60`) and its raw capture manifest. GUI086d68 supplies literal14213afd8 and actual temporary slot rsp+58; QtCore67174bd0 calls fromAscii67074120 then publishes its returned data. The null codec at QtCoreRVA2f3e10 selects Latin-1 allocation6706bfa0. This fixed 36-byte path requests104 bytes, initializes ref1 and qualified length/payload fields, and returns after copying. Non-null codec dispatches an unresolved actual decoder virtual+18 and remains guarded.

This helper models a typed data owner; its host record is not claimed to be a 104-byte native allocation or original address. The 104-byte value is the represented source allocator request. The explicit allocation profile is a diagnostic decision, not a host malloc observation. Known initialized fields are separately recorded only after successful construction: length36, payload offset0x1a, low flag mask0x1f/value0 and trailing UTF-16 NUL. Other native flags/padding have no fabricated value. The host UTF-16 string is not represented as the original inline pointer.

Occurrence metadata exists before the represented allocation. A host bookkeeping exception after allocation acceptance leaves the retained stage AllocationAccepted, no normal constructor return, and no claimed native rollback. Deterministic host-allocation-failure injection is outside this bounded profile. Successful publication and subsequent scalar/weak observer assignments do not allocate. Host owner destruction reclaims adapter storage only; it does not record native cleanup. Explicit source release retains the existing zero-ref data-free guard. Shared-null stays absent; no global count is inferred from fresh input ref1.

## Focused tests and static preflight

New target: `m2m_doe_qstring_resource_owner`; CTest name: `m2m.doe.qstring_resource_owner`. CMake's existing src/test glob discovers the new files; no CMake change was made.

Eleven Testkit cases cover default Unknown, NonNull decoder refusal, Null success/fields/ref1, represented allocator-null failure, equal-text distinct generations, canonical alias/ref/free boundary, stale pre-read epoch, selected-branch timing and later invalidation, foreign/expired/revoked scopes, occupied destination/profile admission, and refusal to convert isolated success into actual Session/receiver source completion. Existing CP80 path-owner tests remain relevant for diagnostic counts and inert lease destruction.

Static checks found no missing quoted includes; the new test has a Testkit main and supported active assertions, with no C assert or REQUIRE_EQ. Existing source snapshot type use was inspected; no external aggregate construction/structured binding of it exists. Tracked diff whitespace and all assigned file trailing-whitespace checks pass. No build, compiler invocation, CTest, GUI, original app control or analysis database mutation occurred. Root owns focused/grouped validation after this freeze.

## Frozen source hashes

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_qstring_path_owner.hpp` | `61d028f5f9fd80df03c2a6dde9eb4b78c4228fe670e20f9076eabb945e39efe1` |
| `source/apps/moproj2mst/src/doe/qstring_path_owner.cpp` | `ae63e9c8e5c1b72b26a09d5d682c8e3ec1c7721c95eabc415c3923c416023bc4` |
| `source/apps/moproj2mst/include/m2m/doe_qstring_resource_owner.hpp` | `1f00cfaf4ffb8a03ed0a9d6c37fc712dd3784e86e2b5387c7e39e3620209568f` |
| `source/apps/moproj2mst/src/doe/qstring_resource_owner.cpp` | `d85a2edb00084a5daf2e14af3b93b4974abbded1585b93057beb0455178158d9` |
| `source/apps/moproj2mst/tests/doe/test_qstring_resource_owner.cpp` | `f0208ce1b1d6217cd65a2cc5aa83927b34ff14fb25be967c36c3b7aa5d0741fd` |

Only the five assigned source/test files and this report were written. Source is held through root validation/archive. Next actual dependency: one real invocation-time process codec owner and actual Session/model receiver source occurrence; unknown/non-null decoder and shared-null/custom-engine/public Type2 guards remain.
