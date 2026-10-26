# CP159 independent plan review

No blocker was found to the proposed single test-only regression of actual `CowVector<double>::detach` body32 allocation failure. It is a useful host-lifecycle/failure-order check, with no production continuation or original allocation-failure claim. Safe fixture teardown and a confirmed observed allocation call are required implementation details.

Reviewed plan `analysis/resume159-geometry-next.md` SHA-256 `d6fd24815edbe079d6c559b0cc9a08ba55ec58f50c7d0a445e71fb3e2d25c6d0`. Independently verified its8 native/EH interval hashes against pinned original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Current CowVector header is `6928fd1bfeb9001e0ad31e560b527a8f26f8621c92c4f3345b1ecf764ab0a983`; the existing per-executable replacement-new precedent is `test_project_feature_storage_epoch.cpp` SHA-256 `03b798bfa44a3e61c412af8bec75e66cc4352d39de78fbe16c07db051e3381a0`.

## Fidelity

Native detach calls the ref decrement at1401b277a before body32 new at2784. Its handle store is later27cc. FH3 state0 begins278e, after allocation returns, and protects clone construction/raw-body cleanup. Therefore a throw from allocating body32 leaves old refs reduced and the old handle installed, with no raw new body to free and no clone constructor or ref restoration. The payload-constructor-throw cleanup is a different case and is correctly excluded.

Current translated `CowVector::detach` has the same weak ordering: decrement refs, then `body_ = new Body(*body_)`. For two actual handles sharing refs2, throwing replacement scalar new observes refs1 and prevents the constructor/publication. Empty storage is appropriate because the intended throw occurs before clone count/pointer arithmetic; it does not pretend the positive-payload constructor path was exercised.

## Test seam and teardown requirements

A separate executable's global scalar replacement new/delete is supported by the existing epoch test precedent. Use fixed probe fields; the only armed operation is the direct real detach call. No Testkit/logging/dynamic probe work may occur while armed. The callback must observe requested size32 and actual old refs1, then throw before acquiring storage. Match unsized/sized delete with one accounting path. Exact one observed allocation, caught bad_alloc and zero armed delete calls are prerequisites. An optimized-away/unobserved call fails the test prerequisite; it cannot be counted as proof of ordering.

The repair guard must be created after both real handles and before arming, so it runs before their destructors on all exits. Disarm before assertions/rethrow. Assert/snapshot the real undercount before repair. Restore refs2 only while **both current handles still equal the saved old body**; an unexpected successful detach must not blindly overcount the old body. If a stronger rollback leaves refs2, the assertion should fail and cleanup may leave that safe count intact. The repair is test fixture cleanup only, not a production catch, native rollback or allocator receipt.

## Value and limits

The probe is discriminating: moving decrement after allocation makes the in-allocation observation see refs2; adding rollback makes final refs2; changing request size/count, publishing a new handle or cleaning nonexistent body storage fails the independent identity/allocation/delete checks. The test calls actual production detach rather than reimplementing its transition or throwing from an element operation. It therefore exposes meaningful ref-order/exception-guarantee regressions rather than merely mirroring a model.

Its value is deliberately narrow. It does not validate original CRT new-handler behavior, payload allocation failure/EH body release, source HeapFree-zero/errno/FLS ownership, or CP157 unique-body scalar advancement. It keeps the before-body-publication guard and all Open-to-Save/public completion limits. One focused regression is sufficient; no wider failure matrix or production allocator seam is justified for this cut.

Disposition: source-ready test plan, no required plan revision or production change. Implement the conditional repair/disarm details above, then root owns compiler/CTest/full validation. Only this report was written; no source edits, build/test, GUI/original execution or mutable REA occurred.
