# Isolated staged OPI Props-first cleanup attempt

Implemented only new exclusive helper/tests after root confirmed checkpoint51 frozen. Existing destructors, OpClass.current, public Close, CMake/build and shared plans remain unchanged. No build or test was run by this worker.

Files under `source/apps/moproj2mst/`:

- `include/m2m/engine/operation_info_cleanup.hpp`
- `src/model/operation_info_cleanup.cpp`
- `tests/mdt/test_operation_info_cleanup.cpp`

## Recovered branch and implemented continuation

Original reference DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The Props entry is native050690@0506a1 ->0512c0. At0512ce/0512d3, null OPI+10 skips to051327 and returns to the next0506ae engine-model stage. Nonnull OPI+10 reaches its deleting virtual+18(flag1) at051307, followed by owner+10=null at05131f. Exact bounds/code hash for0512c0..05132c are retained in resume50-close-destructor-next.md: `57471f6e75d1c985a5f199541e467501ca51da8583ab7a9ee01fe64f31ab38d6`.

`OperationInfoCleanupProgress` captures nonowning guard metadata for one actual live Operation OPI identity. Copy/move are deleted. No shared_ptr to Operation or Props is retained. `attemptOperationInfoPropsCleanup` accepts only that live identity and implements three narrow stages:

1. Inspect the actual Props pointer. A null pointer records `NullPropsSkipReached` and moves only to `EngineModelBoundary`.
2. A nonnull pointer records its identity and stops as `PropsVirtualDeleteBlocked`, returning `NonNullPropsVirtualDeleteUnavailable`. There is no bypass or caller-populated mark-complete API and no virtual destructor dispatch.
3. Retry at the engine-model boundary returns `EngineModelCleanupUnavailable`; it does not infer that a null modern engineModel proves every later native cleanup absent.

Retries preserve the reached native source boundary. Externally changing/deleting Props after a nonnull observation is not reinterpreted as the original null skip. Props appearance after the null branch, a mismatched live Operation, or expired OPI identity refuses with M2M_E_UNSUPPORTED before object mutation. Stored Props addresses are diagnostic identity values only and are never dereferenced.

Outcomes are adapter continuation states, not original native success/error returns or completed OPI cleanup. They must not be silently mapped to accepted public Close. The helper does not reset/delete Props, engine models, lists or rows, emit callbacks, pump events or commit QObject expiry. Actual OpClass.current and all matching guards remain live until an external explicit qualified boundary or ordinary separately classified adapter lifetime expiry.

## Focused tests and limits

Seven tests cover: source null skip ending at the next guarded boundary; nonnull virtual payload guard with unchanged actual pointee/use_count and no destructor callback; refusal to reinterpret external deletion as null skip; refusal on later Props appearance; progress/Operation identity mismatch; current remains bound until an explicitly declared external fixture expiry; and progress metadata not retaining actual Operation/Props allocation after fallback deletion.

Static assertions ensure progress cannot be copied/moved. Worker static checks verify no trailing whitespace in the exact three files, seven TEST_CASEs, and no Props/model reset or QObject-boundary call in the implementation.

No tests were run. Root's main source/test glob should create:

```powershell
cmake --build build/candidate --target m2m_mdt_operation_info_cleanup
ctest --test-dir build/candidate -R '^m2m\.mdt\.operation_info_cleanup$' --output-on-failure
```

Use root's actual active converter build directory. Existing tests/production do not depend on this new helper, so it does not add symbols to standalone packet targets unless root later integrates it there.

The next still-guarded work is the actual nonnull Props subtype destructor, then guarded model/list/recovery/+160/member effects in native order. Current implicit merged cleanup still differs from that sequence. This increment does not claim native OPI QObject-boundary expiry or original fresh Open/Save/Close acceptance.

## Applied hashes

- Header: `ec07f31be6e9354787e55a68362f795b4f70aa02fbb842f40ddc8fc858348a48`.
- Source: `a56f34eb1aee5dfc48ca0fe68c68ce0a55bedb69c8f492dfcc0092d8e2ae87dc`.
- Tests: `2a733f3a6c1d96b0bd232e7f35fbad4263c835fd37847f62e4666f2f5084be56`.
