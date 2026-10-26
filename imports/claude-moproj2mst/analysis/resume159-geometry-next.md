# CP159 geometry: isolate the body32 allocation-throw ordering

The smallest source-ready next cut is a **deterministic failure-order regression against the actual translated `CowVector<double>::detach`**, covering a throw from its body32 allocation. It requires one new test file and no production allocator change, FLS/PTD backend, GUI or source-payload publication. The original decrements old refs before requesting body32; if that request throws, the old handle remains installed with its already-reduced refcount, and no new storage exists for the EH action to free. This deliberately weak failure guarantee is worth locking down before adding a new ownership bridge.

This is a candidate host-lifecycle validation increment, not further execution of CP157's parked T/B occurrence. **There is still no source-ready production continuation beyond `140832adc` through the existing owners.** The next B store would expose live source-role payload to clear/destruction/grow/clone consumers whose synchronous free-zero continuation is missing. The smaller test below does not relax that guard.

Only this report was written. No source edit, configure/build/test, original execution, GUI/app control, mutable REA or analysis-engine startup occurred. Read the local AGENTS instructions, takeover/import state, CP155 plan/review, CP157 plan/implementation/review, CP158 CowVector owner/EH and FLS/PTD reports, current CowVector source and existing test-allocation replacement precedent. The reverse-engineer-anything skill's retained-evidence workflow was used; no new REA session was opened. Original DEF_GUI_MO_64 **v11.0.2**, immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, independently rehashed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This report discusses no reconstructed/deployed DEF_PRE instance.

## Original boundary independently checked

Bounded DUMPBIN 14.44.35228.0 independently confirms detach `[1401b2760,1401b27d4)`: ref-decrement call at `277a`, request32 at `277f/2784`, raw body retention at `2789`, clone call at `27a3`, and handle store only at `27cc`. A body-allocation exception propagates from `2784`, before EH state0 begins at `278e`.

CP158's decoded FH3 records were independently rehashed against that same PE. State0 covers the clone-construction interval and points to `1419f8cf0`, whose raw-body free call is `1419f8cfd -> 1414dfc68`. Consequently distinguish these cases:

| Failure site | Reached effect | Cleanup/publication contract |
| --- | --- | --- |
| body32 allocating new throws at `2784` | old refs have decremented | old handle unchanged; no new raw body, no clone ctor, no EH body free, no refs restoration |
| payload allocating new throws at `1401b28dc` | old refs decremented; raw body32 exists; clone has begun | exactly raw body32 storage free through EH action; no incomplete-body destructor, old-payload free, handle publication or refs restoration |
| source body32 cleanup HeapFree returns0 | cleanup free was entered during propagation | synchronous provider/error tail must complete before unwinding can continue; raw0 cannot be converted to a completed successful free or parked C++ destructor |

The clone ctor has flags0. Its positive-count payload allocation precedes the first pointer store; its later double-copy loop has no throwing call. The table is static EH-routing evidence under ordinary FH3 propagation, not an observed original throw or HeapFree result. In particular, do not confuse body32 allocation throw with payload-constructor throw and invoke cleanup on nonexistent body storage.

## Exact source-ready test contract

Reserve only new `source/apps/moproj2mst/tests/model/test_geometry_clone_body_failure.cpp` and its implementation receipt. The app test glob supplies target `m2m_model_geometry_clone_body_failure` and CTest `m2m.model.geometry_clone_body_failure`; no CMake change or production source seam is required. Existing `tests/model/test_project_feature_storage_epoch.cpp` already provides the suitable per-executable scalar replacement-new/delete pattern. Do not modify that existing test or share its mutable probe across executables.

Use fixed test-local probe storage and scalar global replacement `operator new(std::size_t)` plus matching unsized/sized delete. Normal unarmed allocation delegates to the paired host malloc/free mapping, as the existing precedent does. Arm only around a direct call to actual `CowVector<double>::detach`; at its first observed scalar new, record requested size and the actual still-live old body's refs, then throw `std::bad_alloc` before acquiring host storage. This is explicit **declared host allocation failure**, not forced OOM, a fabricated Win32 receipt, native operator-new returning null, or an original new-handler test. Do not hook original binaries, IATs or process heaps.

Use two actual candidate handles sharing a default-constructed empty host body. This test reaches no clone constructor or empty pointer subtraction because body allocation throws first. Require `sizeof(CowVector<double>) == 8` and `sizeof(CowVectorBody<double>) == 0x20` for this x64 qualification. Save both handle/body identities and the three null fields before arming. No Testkit call, logging, vector growth or unrelated allocating operation is allowed while armed. Disarm through a trivial scope guard before making assertions or rethrowing an unexpected exception. The probe must observe the actual replacement-new call; optimization that prevents observation fails the prerequisite rather than establishing the contract by inference.

Required assertions after the caught declared exception:

- Exactly one observed scalar new request of32 bytes; callback saw the original body with refs1 after its initial refs2. No new storage was obtained and zero probe delete/cleanup calls occurred inside the armed interval.
- Both real handles still contain the same pre-call old body; refs remain1 and all three actual old payload pointers remain null. No successful clone/publication is reported and no automatic ref repair occurred.
- The exception propagates to the test caller. No retry, native-handler, malloc-null, original HeapFree/errno effect or CP157 prefix advancement is claimed.

**Test-fixture teardown requires special care.** This failure leaves two real handles but refs1. Normal destruction of both would otherwise free the shared body too early and then use a dangling body. Prepare a trivial test-only refcount-repair guard before arming. After recording/asserting the native-shaped undercount, restore refs2 solely for fixture teardown through the actual host body, then allow both handles to destruct normally. The guard must also repair on assertion/unexpected-exception exit. Label this repair as test cleanup; it must never appear in `detach`, a production catch, a runtime continuation receipt or a native rollback claim. Use CHECK/REQUIRE from Testkit, no C assert/REQUIRE_EQ; include a valid Testkit entry point. No filesystem fixture or external missing-file probe is needed.

One focused regression is enough for this cut. Existing CowVector sharing/positive-clone tests remain regression context; no new fake successful body32 allocation or broad failure matrix is necessary. Root owns compiling/running the focused target and normal source-stable integration/freeze after the test owner releases its file. This report itself does not claim a test pass.

## What remains blocked, precisely

This cut validates production host code already expressing the original weak ordering (`--body_->refs; body_ = new Body(*body_)`). It adds failure coverage, not a new source-heap owner or Open-to-Save capability. Do not describe it as implementing body32 constructor cleanup or as advancing the original scalar's unique-body path, which allocates no body32.

The next constructor-throw case would need deterministic failure at the real double-payload allocator. Current `cow_detail::alloc<double>` directly calls std::malloc and exposes no bounded injection seam; replacement scalar new alone cannot force that branch. A declared transition model could document its order but would not exercise the actual constructor/new-expression cleanup. Do not substitute a throwing element assignment: native double copying does not throw. Do not provoke resource exhaustion or intercept all host malloc calls merely to manufacture this event.

Actual source-role constructor cleanup additionally requires the full synchronous body32 release owner. CP158 establishes why a success-only HeapFree backend is insufficient: on raw0, provider `141474134 -> 1414814e8` must preserve failed LastError, select actual owned PTD+0x10 or module fallback, complete mapper/store, and return. Lazy PTD ownership includes callback/default/lock/locale lifetime and registration-failure recursive free. The missing operation is not another clone address or payload recipient. No source body/slot publication, T nonnull free, whole-leaf completion, later receiver, Stop or retirement becomes admissible through the body32 allocation-throw regression.

## Revalidated immutable ranges

Half-open preferred-image VA ranges, SHA-256 of PE bytes using the existing section-aware `tools/doe_static_pe.py` reader. No original process address is candidate pointer authority.

| Range | SHA-256 |
| --- | --- |
| `1401b2760..1401b27d4` | `1d3c56085667f49d6d5f8de7bc9f6253223513fdf79ba16dca228173bafb723b` |
| `1401b2890..1401b297d` | `6f1cdbaf97acd4999d20f733ac5e4fe3719b5218420d3fe72a0c1ad7a34835b1` |
| `1419f8cf0..1419f8d08` | `d454b775089241fcc956458e1325093839de798d2d05d532e5118b6bd2426fc7` |
| `143174b08..143174b18` | `69ae6142e5cc1bb4608007b31a1ef3c1aba644714d37ba7f008d0241556902ff` |
| `143174b18..143174b30` | `2c5869ada581f19af554f2e4d69ac3716a7a78695760960308c5e963c5209fc8` |
| `1421ec2f0..1421ec318` | `0e53b52f423106794153d858379aed80646c4ff01a45d46d00426769cc52bcf0` |
| `1432cc560..1432cc568` | `3454176c5ec37409cdcb5f19b12556676a0324f60cb94b52304ad8d359dd9086` |
| `1434212c8..1434212d0` | `136991d2f5bc00abebfc21efaa20262eec8617bc5c87ff6b409959dccad91329` |

No deployment, original parity, completed lifecycle or measured efficiency improvement is claimed.
