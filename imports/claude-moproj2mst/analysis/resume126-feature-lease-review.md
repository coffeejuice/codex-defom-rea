# CP126 independent review: feature CRT retained lease

## Result

The frozen private runtime implements the requested ownership check without invoking the producer or adding a default Project hook. `matchesRetainedLease` requires (1) a nonnull supplied lease whose stored pointer is this runtime, (2) a live canonical lease recovered from the stored weak pointer whose pointer is also this runtime, and (3) equivalent shared ownership as shown by both `owner_before` directions returning false. This rejects an aliasing-null lease and a new control block that happens to wrap the same raw pointer.

## Canonical owner and matching behavior

The `FeatureCrtRuntime` constructor is private. The only construction path, `prepareOwnedTestProcess`, is an explicit factory for the bounded test-process experiment. Its function-local static allocates one retained `shared_ptr` owner, assigns a `weak_ptr` copy to `Storage::retainedLease`, then publishes/returns the owner. The retained strong reference makes `lock()` remain live for the owned test process without a strong-reference cycle. Repeated factory calls return copies with the same stored pointer and control block.

`matchesRetainedLease` performs only shared/weak pointer operations and pointer/control-block comparisons. `weak_ptr::lock()` increments the host shared-owner count; it is not a Windows handle operation. The matcher does not call the factory, `attemptProducer`, `HeapCreate`, `HeapSetInformation`, `GetModuleHandleExW`, or any other Win32 function. It does not acquire the runtime SRW lock or alter report/effect counters. The tests check that matching a genuine copy and rejecting empty/null-alias/same-address foreign-owner leases leaves the full report unchanged both before producer attempt and after the single actual producer attempt.

## Project/default path boundary

The runtime type lives under private `src/model/feature_crt_runtime.*` and is not a member of `Project` or `MoModelSlots`. `model.hpp` declares no runtime owner, binding method, constructor parameter, or default-project hook for it; the Project constructors and implementation have no reference to this runtime. A repository search of `source/apps/moproj2mst/src` finds the runtime only in its private source and its focused model test. Thus a normal Project construction does not trigger this factory, pin a module, create a heap, or retain a runtime.

The public/static report constants also keep `projectBindingAdmitted`, `proxyAllocationAdmitted`, `nativeTeardownCompleted`, and `completeForPublicClose` false. This lease evidence concerns candidate process/module ownership only; it does not identify the original executable's global heap slot or establish that a Project/Close lifecycle is supported.

## Limits

The source owner froze these files and reported static preflight only; this review did not compile or execute the tests. It establishes code-path absence of producer calls in matching and absence of Project integration, not race-stress behavior or any broader CRT startup/teardown parity. The factory itself intentionally calls `GetModuleHandleExW(...PIN)` during explicit preparation and intentionally retains the runtime until process exit; those effects are separate from matching and are visible in the producer report.

## Frozen files

Reviewed against the CP126 source-owner freeze:

- `source/apps/moproj2mst/src/model/feature_crt_runtime.hpp`: `25b1cc1059fa225017bec07dd5289b9084f92c12fdd78f0bade562d3ce65071c`
- `source/apps/moproj2mst/src/model/feature_crt_runtime.cpp`: `89989f0d68c6bf7657b8d86e587ebfe550ca95a457c2f385efd5279a01911be8`
- `source/apps/moproj2mst/tests/model/test_feature_crt_runtime.cpp`: `1529ac9ef022c4e0c0d11b51e94f59832d789f4ea6d941b1054a2e1a5e564c61`
