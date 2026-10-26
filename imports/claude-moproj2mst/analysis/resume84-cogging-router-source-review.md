# CP84 cogging router access review

## Result

The `router` adapter now gates through `GuiModelAccess::withStep` and only then invokes the native function. The native call still receives the same 12 arguments, in the same order: `nativeStep`, `mover`, `target`, `dir`, `tol`, `dist`, `G`, `flag`, `nc`, `list`, `tchg`, `one` (`cogging_save.cpp:160-167`). The lambda returns the native `bool`, which is an allowed synchronous result; no pointer or reference is returned from the access seam.

The barrier check precedes `w.step` borrow and all native-router effects, including writes through `nc`, `list`, or `tchg`. This applies even when callers ignore the result. `router` does not add a nullable-step fallback: it passes a null step through as before, preserving the previous call contract. The current body itself contains no branch or fallback whose order changed; all routing and output-flag behavior remains inside `def::cmd::objpos::fun_14019fa00`.

## Caller ordering and semantics

The cogging callers retain their branch structure. Die routers 2/3 remain result-ignored at `cogging_save.cpp:421,439`. The manipulator-4 branch still calls `viewerSync` only when the router returns true (`593-594`); later routers for manipulators 5-7 remain result-ignored (`604,622,640`). `tchg` and per-call `one` values are still passed by the same addresses, and the same shared `nc`/`list` state is supplied where it was before. No caller-side reset, fallback, or result use was reordered by this wrapper migration.

The router takes raw `mover` and `target` pointers from its caller, but the barrier refusal happens before the callback executes, so those inputs are not dereferenced by the native router on a parked model. The adapter cannot revoke a pointer a caller already holds; prior public object accessors and raw model fields remain outside this seam. This review therefore establishes router-entry safety, not a universal guarantee for mutations a caller may have performed before reaching the router.

No focused test directly asserts the 12-argument forwarding or `tchg` output under both success/failure results. Existing full cogging save coverage exercises its ordinary nonpending callers and output-sensitive model path. The static signature/body inspection is sufficient to verify the current forwarding statement, but a future isolated router test could protect argument order if this wrapper grows beyond a single call. No blocker found for this migration. No source/test edits, builds, or test runs were made.
