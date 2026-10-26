# CP64: admitted synchronous Boolean UpdateData projection

The isolated Boolean lifecycle helper now has a synchronous receiver-invocation seam. `BooleanSynchronousUpdateData::dispatch` calls the existing operation class's real `updateDataInWidgetArea` hook before returning, only after an explicitly admitted emission, receiver registration, native method resolution and current-widget guard/cast. It does not post a candidate event, manufacture a due timer, or enable a public Open/Save dispatcher.

The source basis is `analysis/resume64-boolean-event-route.md`: native notifier `1402cf3c0` passes raw invocation type1 at `1402cf6bd..1402cf6c6`; Qt's type1 path `67120685` calls the receiver metacall directly instead of reaching queued-event branches. The current widget impl `140dae340` calls vtable `+1c0` at `140dae3ab`, which Boolean's installed vtable resolves to Apply `1411e2870`.

## Scope and ownership

Only these three announced existing files were changed, plus this receipt. Parent confirmed exclusive ownership. No CMake, ConversionSession, serial gate, trace binder, GUI, original application, mutable REA or shared recovery-state files were changed. The prior `reselectBooleanObjectEditor` implementation is unchanged.

| Frozen file | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/boolean_lifecycle.hpp` | `976cd0c9b2388d3e14de53c906ebfa027f09dbe2c78699247198d289fabce69a` |
| `source/apps/moproj2mst/src/engine/boolean_lifecycle.cpp` | `7340522bfce32301d5cbbfc970575aca0ce2fc0534d43a5bb43c4ebfadcfdd76` |
| `source/apps/moproj2mst/tests/mdt/test_boolean_editor.cpp` | `1a095f88b9dfe7322b5ba1e11a39366c2643363f48da28b219bf40dc4c53a796` |

The noncopyable, non-rebindable scope borrows the actual Project/Operation and records their existing class/model binding. Its receiver and current-editor identities are weak aliases to owner control blocks supplied by the admitting layer, not fabricated native addresses or another scheduler. Dispatch checks the same Project/Operation/class and the model's exact pointer **and shared owner control block** immediately before effects. Each received alias must likewise match its bound pointer and control block, so equal raw addresses from independent lifetimes are rejected. Project/Operation must outlive the borrowed scope.

A changed current editor requires release of the prior admission and a new qualified scope; `releaseAdmissionScope` makes a retained scope unusable even if external references keep its owner allocations alive. The canonical owner/lifetime mapping is an explicit caller responsibility; arbitrary nonempty evidence text does not authenticate a capture.

## Represented behavior

- An absent occurrence, missing emission admission, missing gate observation or missing qualification returns incomplete without mutation. Missing receiver-registration/method/current-widget observations likewise remain incomplete; no absent input becomes a successful default.
- Explicit suppression completes without Apply. A dead receiver, failed method resolution, failed current-widget match or expired editor completes the selected suffix without Apply. This does not introduce failure propagation into the native notifier's ignored invoke bool.
- A fully admitted suffix calls the existing real Boolean hook immediately. The caller sees its Workpiece/Cutter effects on return. The seam performs no replacement Fill, current-object-zero write, Leave callback, Save propsLoad, timer registration or dispatcher operation.
- The occurrence is consumed before the effect hook. A throw after partial mutation does not allow a second dispatch of that occurrence to replay Add New Object. No rollback is introduced.
- Nested dispatch into the same scope is explicitly refused. The retained native direct path permits nesting, but its receiver-list/callback ordering has not been admitted to this bounded candidate projection. No assertion of nested parity is made.

This is a **selected receiver suffix**, not the whole custom notifier. The actual notifier's `+90` clear, key-map lookup, receiver-list snapshot and other receivers stay with the admitting caller. Its runtime `+141` value must be explicitly supplied. The helper does not allocate or copy a native notifier/queue/dispatcher, and its internal consumed-occurrence set is admission bookkeeping rather than native posted-event state.

## Focused test additions and preflight

Seven new Testkit cases were added to the existing `m2m_mdt_boolean_editor` target:

1. Binding itself leaves the model unchanged; admitted dispatch mutates before returning, with no implicit Fill/current-object reset. Explicit replacement Fill plus a newly qualified scope avoids another cutter creation.
2. Emission, suppression observation, registration, method resolution and current-widget evidence are each mandatory; missing evidence can be supplied before the occurrence is consumed.
3. Explicit suppression, failed method resolution and absent current widget skip Apply.
4. Same-pointer/different-control-block receiver/editor aliases, a second Project/Operation with colliding logical IDs and a rebound model are rejected before mutation.
5. Expired weak receiver/editor lifetime skips Apply.
6. A test hook performs the real Apply then throws; retry cannot create a second object because consumption precedes effects.
7. Reentrant dispatch is refused; released scope admission remains unusable.

The fixtures explicitly identify their owner mappings and admissions as synthetic candidate contracts, not original runtime observations. They exercise real Boolean Apply hooks and object counts rather than duplicating the implementation's internal bookkeeping.

Preflight completed by source inspection and bounded mechanical checks: Testkit entry point exists; new assertions use supported `CHECK`, `CHECK_EQ` and `REQUIRE`; no C `assert` or unsupported `REQUIRE_EQ` appears; quoted includes resolve under the test source directory or configured include roots; new code uses existing includes/targets; no trailing whitespace was found in the three owned files. CMake already recursively includes the lifecycle source and discovers `tests/mdt/test_boolean_editor.cpp`; no build configuration change is needed.

**No compiler, build, CTest or test executable was run by this worker**, as assigned. Root owns the focused target and coordinated integration. These checks do not claim compile or behavioral test success.

## Public integration remains guarded

Actual Session-owned widget-area receiver/current-editor lifetime mapping and admitted emission binding are absent. No Session caller constructs or dispatches this new scope yet. A future bridge must use the actual owners, invalidate the old scope on replacement/release, bind the already admitted notifier prefix, and compose remaining native receiver order before enabling a public path. The isolated scope cannot establish the first editor on fresh Open or the first-ever emission by itself.

`WidgetConstructorTail` still stops at PostPending with `Constructor raw0x4a posting requires the actual shared queue/dispatcher continuation`. Raw constructor0x4a, native HWND/serial/postedWake, modelUpdated timer delivery, nested callbacks and first-Open timing retain their existing guards. No runtime readiness is inferred from timer counts, requested intervals, elapsed time or this helper's existence.
