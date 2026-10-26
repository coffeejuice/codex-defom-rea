# CP161 FRCROT table repair: independent review

Disposition: **accepted for root's grouped build/test gate; no remaining source-review blocker found.** Both findings in `resume161-frcrot-table-source-review.md` are closed at the hashes below. This acceptance covers only the private prepared-observation projection, not actual text parsing, native allocation/lifetime, public owner admission, mirror payload or Save.

The mode0 test now references the real `destinations` and `scanReturnCodes` members and checks that the trailing-line boundary is absent. The nonexistent result-member compilation defect is removed. Testkit entry point, quoted private-header resolution, explicit standard includes and supported CHECK/CHECK_EQ/CHECK_BITS usage remain valid by static inspection; there is no C assert.

The helper now rejects `consumed != input.observations.size()` immediately after its scan loop. An observation after an X or Y EOF therefore cannot be silently admitted. Earlier shape gates still reject too many observations and codes outside {-1,0,1}; an observation shortage before a non-EOF completion returns Unsupported. The new exact-consumption check leaves valid EOF prefixes intact and does not manufacture unread-stream semantics.

The accepted observation matrix is now coherent:

| Entry/outcome | Observation/boundary requirement | Supported result |
| --- | --- | --- |
| Mode0 or unknown | no observations; trailing-line false | dimension0, no scans, status0; supplied constant only for mode0 |
| Mode1/2, negative or zero count | qualified allocation/zero-fill; zero observations; trailing-line false | count0, empty arrays, status0 |
| Mode1/2, count1..7, all scans non-EOF | qualified allocation/zero-fill; exactly 2n valid observations; trailing-line true | ordered X_i,Y_i effects, status0 |
| Mode1/2, count1..7, first -1 | observations end exactly at that failing call; trailing-line false | earlier writes retained, unassigned slots zero, status1 |
| Count>7, invalid code, missing/extra observations, unqualified allocation, inconsistent boundary | candidate refusal | Unsupported |

Code1 assigns immediately; code0 leaves the current zero-filled slot and continues. X EOF skips that row's Y, while Y EOF retains the same row's X. Mode1/2 codes remain1/4. Positive completion reaches only the represented ignored-result line-read boundary; no result value is injected into table status. Count conversion remains prepared and negative normalization has no arithmetic overflow path.

Tests now cover count0, count7, count8 refusal, missing observations with allocation qualified, positive complete scans without the required line-read boundary, explicit Supported before Y-EOF inspection, and both recorded destinations and codes for return0. The X-EOF trailing-observation case directly exercises the new exact-consumption guard. The added Y-EOF trailing-observation case is also correctly refused, although its count1/three-observation shape reaches the earlier maximum-length guard. Using count2 for that one case would isolate the new guard for Y EOF too; this is a nonblocking coverage refinement because the guard is unconditional and the X case already catches its omission.

Source remains disconnected from production conversion. No public header, pair helper, CMake dispatch or owner path was changed by this repair. Existing CMake CONFIGURE_DEPENDS discovery includes this new helper and test without a manual registration edit. Root must still compile and run the focused/grouped checks; this read-only review did not execute a build or tests.

## Reviewed source SHA-256

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/frcrot_table_prefix.hpp` (unchanged) | `a155a0e468dea82bbe1148afb51ce491c0c0c042a25db096ae2448361f6e216c` |
| `source/apps/moproj2mst/src/doe/frcrot_table_prefix.cpp` | `b20c309704cf7e512e2bc0c57afaa8adaeadb550f4b90ddb24ea24663aa664a9` |
| `source/apps/moproj2mst/tests/doe/test_frcrot_table_prefix.cpp` | `27b1dab1b68e0393af64ef4cff652dfd3fb559e92da79309ff14c6417e31ac5f` |

Hashes match `analysis/resume161-frcrot-table-repair.md`. Native qualification remains the immutable original GUI PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and the accepted CP160/161 table-reader evidence/plan. No native behavior was reacquired, source edited, build/tests executed, GUI/app control acquired, live REA retried or mutable database accessed. Only this report was written.
