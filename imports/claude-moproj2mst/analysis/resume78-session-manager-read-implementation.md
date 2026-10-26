# CP78: actual Session existing-manager Open read

Session now creates a private candidate-only owner/read receipt after the fresh-Open precondition and before model-child allocation. It binds the same existing ModelManager/control block and actual ownerSession backlink. Open's manager, child registration and later Project/read/selection behavior remain in place. No second manager, native factory/class completion or main-window readiness is introduced.

Root reserved the two shared Session files and released the source lane before edits. Only those files, the three new helper/test files below and this receipt were written. ModelManager, CP77 helper, CMake, plan, public C ABI and public Close were untouched. No compilation/build/CTest/test executable/GUI/original execution/mutable REA ran. All five code files are frozen and their ownership released to root.

## Integration boundary and lifetime

`modelManagerOpenReadReceipt()` is absent before the actual Open read edge. `prepareProjectModel` retains its existing fresh empty/unbound guard, then creates the receipt from its actual manager before `make_shared<DbHandle>`. The private constructor validates Session's actual accessor/control block and `manager->ownerSession()==&session`, recording candidate population/connection counts at that read. The original child construction, path assignment, append, two registrations and Project.dataDb assignment follow unchanged.

The receipt's origin is `CandidateExistingOwnerRead`; its source correspondence is01d078/c9b7ca. Native factory completion, concrete Mo class observation, wrapper qualification, main-window readiness and public Close are all explicit false constants. It calls no CP77 constructor/class/factory-store observation. Its historical counts are not current population or native QList evidence; `requireManagerFor` checks only the same actual live adapter Session/manager identity.

Session owns one receipt and exposes a const shared view. It revokes the receipt's adapter admission at destructor entry before Project storage/event/view teardown. Escaped receipt/ModelManager C++ leases cannot authorize a replacement Session. The receipt retains a weak manager, a private comparison-only Session identity and an opaque integer manager token captured while the candidate manager is live. It exposes no historical raw ModelManager pointer. Live manager access requires scope/Session/control-block validation. It creates no main/QWidget allocation lease or native-address claim. Revocation is not a native QObject death assertion. Normal Project-only storage release does not revoke the surviving Session owner receipt.

Rejected second Open retains the original receipt/child. A later Project-binding guard failure does not fabricate factory evidence. An exception allocating the receipt itself occurs before child allocation and leaves manager/Project binding unchanged; this is an added adapter metadata allocation, not reconstructed original failure behavior.

## Source qualification

`resume78-session-manager-open-integration.md` separates native factory creation from Open use. Original01d078 calls c9b7c0, whose leaf loads main+50 atc9b7ca, then01d085 invokes c79390 on that existing manager to create a child. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The native factory sequence still includes QObject/list/guard construction, manager+10 store, explicit current resetc782c4, concrete Mo class and main+50 publication. No constructor-order refactor was made or certified. This increment records actual candidate storage/read identity and preserves those original constructor/provider/readiness dependencies.

## Review repair and separate suffix-order limit

Independent review identified that a historical raw pointer could escape after the manager's C++ lifetime ended. The public accessor now returns only `std::uintptr_t observedManagerIdentityToken()`, with no pointer-returning historical accessor. The token is formed while the candidate manager is live and remains diagnostic comparison data; it is not a live object/native-address capability. The new expired-manager test keeps only a receipt and weak manager across Session destruction, verifies actual weak expiry plus retained opaque token, and confirms replacement-Session authorization still refuses before historical-owner access.

The separate **existing rejected-suffix ordering limit remains**: full-Save `convert.cpp` calls prepareProjectModel before openProject classifies the accepted suffix. Retained original01ca30 rejects an unsupported suffix before the c9b7ca read. Therefore a candidate receipt may exist for an input where the original never reached this read. `sourceSite` expresses static correspondence, not native-path acceptance or a captured source hit. The header now states that limit explicitly. This repair does not modify convert.cpp, public Open order or suffix handling; resolving that broader ordering gap is separate work.

## Focused tests and static preflight

Target: `m2m_mdt_session_model_manager_read`. CTest: `m2m.mdt.session_model_manager_read`.

Ten Release-active Testkit cases cover: absence before Open; actual same-manager read before child registration with historical zero counts versus subsequent one child/two connections; later Project/StudyIo/selection binding using that same manager; rejected second prepare; actual selection-before-Project guard retaining state; foreign Session refusal; revoked escaped receipt despite retained C++ manager; opaque identity after actual manager destruction without a retained strong lease; historical read not proving current population/Project absence; and unchanged unrelated Session owners/notifications/remesh/pending timers/readiness guards.

Static preflight passed for quoted includes, Testkit main and supported CHECK/REQUIRE/TEST_CASE macros, no C assert/NDEBUG-dependent checks, trailing whitespace and `git diff --check`. Ordering checks confirm receipt capture after the fresh guard and before child allocation, and revocation before Project teardown. Direct inspection confirms the helper creates no ModelManager, executes no factory observation and does not clear/select/query current or call Qt. Test paths stay under C:/PROBLEM below100 characters; the path setter used by prepare does not write files.

No executable test result is claimed. Root may run:

```text
cmake --build <combined-build-directory> --config Release --target m2m_mdt_session_model_manager_read
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.session_model_manager_read$ --output-on-failure
```

## Frozen files

| File relative to source/apps/moproj2mst | SHA-256 |
| --- | --- |
| include/m2m/engine/conversion_session.hpp | `46d4aaaba8128fc78d3d19b51158b319ec5959009eba48184194958e25a32e31` |
| src/engine/conversion_session.cpp | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |
| include/m2m/engine/session_model_manager_read.hpp | `f9d8c014b7f6fc4bd14e0795cb7b9c3ad1e4317d3a7bdf6d08771dbcca698750` |
| src/engine/session_model_manager_read.cpp | `ef86f587106679a6c33a6abe724c0a6e25ea51c72eddeb3d65db48203fa51725` |
| tests/mdt/test_session_model_manager_read.cpp | `9e23eb8ad347b765299b400ea635d5ee3ef529733be0e644fc57b488777d186a` |

Root's source-stable focused/full validation and archive remain pending. Actual native main/factory/wrapper, Qt construction, accepted-Close/modal/providers, main+3c0/013d40 and late status/private/cache bindings remain guarded.
