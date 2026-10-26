# CP65: staged widget-area lazy getter prefix

The new isolated helper projects original `140018460` through its parent `+3c8` return/store boundary. It distinguishes unavailable, null and cached fields; requests allocation0x30 only for known null; retains the actual allocation alias while constructor work is unresolved; and commits the field only after an admitted constructor return or the original explicit null-allocation path. Widget impl/registration, actual Session ownership, current Boolean editor and public PostPending remain guarded.

Only these announced new source/test files and this receipt were written. Root confirmed exclusive ownership. No existing source, CMake, Session, Boolean helper, GUI, mutable REA or shared recovery state was edited. No compiler, build, CTest or test executable ran in this worker.

| Frozen new file | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/widget_area_lazy.hpp` | `d5795e6a8aa6046ae4ebfca78d03bfef3a226b5eca5c7e4787cfcf5475199e5b` |
| `source/apps/moproj2mst/src/engine/widget_area_lazy.cpp` | `c86118ca4d81d5a8fca70d3e371b27f74d07d75cbf40e00afaf3cfb0af4135f6` |
| `source/apps/moproj2mst/tests/mdt/test_widget_area_lazy.cpp` | `d10cbefde1b0fa69240183ccc976f9ba95345cc7833b1756e1ad185ccccf6df1` |

## Native source qualification

`analysis/resume65-boolean-session-binding.md`, `resume65-boolean-area-owner-static.json` and its `140018460` disassembly qualify the original getter. The immutable EXE hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Relevant source order is:

- `140018477`: read parent `+3c8`; nonnull skips allocation/constructor/store and returns the field.
- `140018481..140018486`: known-null path requests0x30 bytes.
- `140018490..140018496`: null allocation bypasses the constructor; `1400184ae` sets the result null.
- `140018498..1400184a2`: nonnull allocation calls `SfWidgetArea` constructor `140d14d30` with parent argument0 and flag1.
- `1400184a7`: collect the returned constructor pointer. The selected constructor returns its original `this` at `140d14dee..140d14df7`.
- `1400184cb`: only afterward store the result into the parent `+3c8` field; `1400184d7` returns that field.

The separate retained area constructor contains QWidget work, impl0x48 allocation/constructor `140dad210`, area `+28` store, registration `140d14e90` and initial widget selection. Those effects are not implemented by observing this getter's constructor return. No constructor success is inferred by the helper from an allocation or a cached C++ object.

## API and staged behavior

`WidgetAreaLazyParent::createQualified` creates one canonical diagnostic projection for an explicit actual parent owner and initial optional field observation. An absent optional means unavailable; a present null shared alias means known null. `observeInitialField` can resolve unavailable state once, with qualification, and cannot replace an admitted field or active construction. Initial cached aliases are caller-qualified observations, not native constructors run by this helper.

One noncopyable `WidgetAreaLazyGetter` represents one invocation. `resume` crosses at most one boundary at a time:

1. `FieldRead`: remain pending on unavailable field. A cached nonnull field returns immediately with `cacheHit`, no allocation/constructor request and no field store. Known null enters `AllocationPending` and records size0x30.
2. `AllocationPending`: unavailable allocator outcome stays pending. Explicit failure advances to `ParentCommitPending` with null result and no constructor request. Explicit success requires an actual nonnull owner alias, retains it, records parent-null/flag1 constructor arguments and enters `ConstructorPending`.
3. `ConstructorPending`: absent constructor observation stays guarded. Explicit `Interrupted` is terminal for that invocation, retains the allocation and never reaches the parent store. Explicit `Returned` requires the same pointer and shared owner control block as the allocation, then enters `ParentCommitPending` without storing yet.
4. `ParentCommitPending`: store the retained returned allocation or explicit null into the same parent field, increment its store count and complete. A second resume of this completed invocation does not repeat the store. A **new** getter invocation after committed null retries allocation, matching the raw field check.

The helper performs no allocator call, native constructor, Qt callback, receiver registration or OS dispatch. Observations must be supplied by a source-qualified capture/integration layer; nonempty strings are a record of caller admission, not authentication of evidence or a fabricated constructor success.

Exact pointer **and owner control block** checks bind canonical state, parent and constructor result. A frame cannot rebind to another parent even if logical IDs or raw addresses collide. Parent lifetime expiry/released admission refuses further work. Simultaneous/nested construction frames on the same null parent are guarded because their native ordering has not been recovered here. An interrupted frame continues to hold that diagnostic active-construction guard until abandoned/destructed; no post-exception reentry parity is claimed.

Shared aliases retain diagnostic allocation records. They do not assert QObject parenting, native heap release, accepted Close or recovered destructor ownership. `requireImplRegistration()` always refuses, including after a getter cache hit or admitted constructor return. No impl pointer/current widget/receiver population is fabricated from the returned area alias.

## Focused tests and preflight

The new automatically discovered target is `m2m_mdt_widget_area_lazy`, containing ten Testkit cases:

1. Unknown field remains distinct from null and cannot request allocation until observed.
2. Cached nonnull returns without allocation, constructor or store.
3. Successful allocation/constructor return retains staged order; parent field/store count stay unchanged until the later commit; the next getter reuses the cache.
4. Unresolved constructor remains pending through repeated resumes and cannot expose impl/registration.
5. Interrupted constructor retains allocation and performs no parent store; a later fabricated return cannot revive that invocation.
6. Null allocation commits null without constructing; a new invocation retries.
7. Wrong parent, same-address/different-control-block parent alias, state alias and cross-parent rebind are refused before store.
8. Constructor return must have the canonical allocation pointer/lifetime.
9. Null allocation rejects contradictory constructor effects.
10. Nested construction, released scope and expired actual parent remain guarded.

Fixtures explicitly declare synthetic source-stage observations. They do not claim a native area was constructed or an original allocator returned any value.

Source preflight passed: Testkit entry point exists; only supported Release-active CHECK/CHECK_EQ/REQUIRE-style assertions are used, with no C `assert` or unsupported REQUIRE_EQ; all new quoted includes resolve under configured source/include roots; files have no trailing whitespace. Existing CMake recursively globs core `.cpp` files and discovers `tests/mdt/test_*.cpp`, so no shared CMake edit is needed. These checks are not compiler/test success. Root owns the coordinated focused build/tests and full validation.

## Remaining integration dependency

No Session callsite constructs this parent projection and no actual Session `SfWidgetArea`/impl owner is bound. Original parent construction/callsite admission, QWidget base continuation, actual impl/registration and current-widget attachment/deferred deletion must be composed under shared ownership before CP64's synchronous UpdateData suffix can use this allocation. A successful isolated getter cannot establish first fresh-Open Apply or timer readiness.

The public constructor guard remains `Constructor raw0x4a posting requires the actual shared queue/dispatcher continuation`. Source inspection confirmed it unchanged. Public PostPending, native event/dispatcher values and nested Boolean callback timing are not enabled by this prefix.
