# CP161 FRCROT table projection: independent source review

Disposition: **blocked by one test compilation defect and one observation-shape admission defect**. The ordinary supported table-state/order branches match the accepted CP160/161 prepared-observation contract. Repair the two findings below before grouped build/tests. No public FRCROT, Type3, owner or Save admission follows from this helper.

This was a read-only source/evidence review. No build, test execution, source edits, GUI/app-control session, original process, live REA call or mutable database was used. Only this review report was written.

## Blocking findings

1. **Test refers to a nonexistent result member.** `source/apps/moproj2mst/tests/doe/test_frcrot_table_prefix.cpp:20` calls `CHECK(result.observations.empty())`, but `FrcrotTablePrefixResult` has no `observations` member. Its observation-consumption outputs are `destinations` and `scanReturnCodes`. This is a definite compile defect, independent of running a compiler. Replace the assertion with checks that both consumption vectors are empty; retain the mode0 constant/dimension/status and no-line-read checks.

2. **Observations after an EOF stop are silently admitted.** `source/apps/moproj2mst/src/doe/frcrot_table_prefix.cpp:58..64` stops at the first returnCode -1, but the final return at line74 does not require `consumed == input.observations.size()`. For example mode1, count1, allocation/zero-fill true, observations `{{-1,0.0},{1,2.0}}`, trailing-line false is admitted as Supported after consuming only X0. The Y0 observation cannot correspond to an original scan: native X0 EOF returns before Y0. The accepted plan requires each observation to correspond to exactly the next original call and malformed sequences to be unsupported. Add a post-loop consumed-size check before publishing Supported, and test both X-EOF and Y-EOF with otherwise valid trailing observations. Do not reinterpret those extra entries as unread stream content; the API names them scanner observations, with no unconsumed-tail representation.

## Supported behavior verified statically

- Modes0/unknown take the dimension0/default branch before the allocation qualification gate. They reject supplied scan observations or a supplied trailing-line boundary, perform no scans, and return table status0. Mode0 copies only the supplied constant; unknown mode retains reset0.0. Ignoring irrelevant preparedCount/allocation flags in this branch agrees with the native early return.
- Modes1/2 select dimension1 and codes1/4, clamp negative prepared counts to0, and refuse normalized counts above7. Allocation/zero-fill must be explicitly qualified. Two zero vectors are established before observation application; native allocation failure is not replaced with an invented native status.
- Only prepared scanner codes -1/0/1 are admitted. Destination order is X0,Y0,X1,Y1. Code1 immediately assigns the supplied double, code0 preserves zero and continues, and code-1 returns table status1 while retaining earlier assignments. X failure skips its Y; Y failure retains its row's X. No text/locale/CRT conversion behavior is implemented.
- A complete positive-count scan requires the qualified trailing-line boundary and exposes it as reached. EOF/count0 reject that boundary. There is no line-read-result input that could alter table status: the helper models the ignored-result boundary only, not an actual file read or proof that a particular failed read occurred.
- No production source references `applyFrcrotTablePrefix`; references are its declaration, definition and focused tests. The helper has no Study/Keyword/SubProject owner, stream, registry, mirror payload, public conversion or Save callsite. Existing pair-prefix and public guards remain separate.

These facts are bound to the original immutable GUI PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, especially table-reader `140af73f0..140af77fa` byte SHA-256 `a9a7c9faf99726022edd96b06e427fed146093d41162bff2b557a30f91cc25a9`, and the accepted `analysis/resume161-ops-type3-plan-review.md`. This review does not repeat native acquisition or claim runtime validation.

## Test/build preflight and focused repair checks

The quoted private-header include resolves relative to the test file; `<testkit/check.hpp>` and its CHECK/CHECK_EQ/CHECK_BITS macros exist. The test has `main(){ return testkit::run_all(); }` and no C assert. Existing cases meaningfully assert mode selection, negative normalization, X/Y ordering, X EOF, Y EOF and return0 continuation. Their vector comparison macros are syntactically protected by constructor parentheses.

Besides the two mandatory repairs, complete the finite-profile boundary coverage with count0, count7 success and count8 refusal, a missing-observation case with allocation/zero-fill **true**, and a positive-count complete scan with trailing-line false. The current final test's missing-observation input sets allocation false first, so it only proves the allocation guard. The existing no-trailing-boundary case already exercises that boundary guard but should remain explicit when refactoring. Assert Supported before inspecting the Y-EOF result, and assert the recorded scanReturnCodes as well as destinations in a return0 case.

No manual CMake registration is required: `source/apps/moproj2mst/CMakeLists.txt:90` auto-globs `src/*.cpp` recursively with CONFIGURE_DEPENDS, and lines129/135 discover the existing doe directory and `test_*.cpp` with CONFIGURE_DEPENDS. Root's focused/grouped build remains required after source repair; this read-only review did not run it.

## Reviewed immutable source identities

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/frcrot_table_prefix.hpp` | `a155a0e468dea82bbe1148afb51ce491c0c0c042a25db096ae2448361f6e216c` |
| `source/apps/moproj2mst/src/doe/frcrot_table_prefix.cpp` | `9c324bdeb2eb1ec183c672d2c669566f35a8a5ea9bd919f1f60d2cba4a2f470c` |
| `source/apps/moproj2mst/tests/doe/test_frcrot_table_prefix.cpp` | `ff88abf52467a350f1b65c39365886987b97b15929faaa07acd4480d6e6b444d` |

The implementation receipt matches these three hashes. A repair requires fresh source hashes and a bounded re-review of the changed branches/tests before the normal root integration gate.
