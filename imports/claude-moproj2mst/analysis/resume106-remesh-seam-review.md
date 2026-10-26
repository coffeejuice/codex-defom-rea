# CP106 private remesh terminal seam: independent static review

**Result: no blocker for the isolated private seam.** The public save wrapper
and public header remain unchanged in signature/layout, the admitted path does
not rely on undefined behavior or a `noreturn` backend contract, and no
production caller activates the new terminal service.

## Public boundary and frozen receipt

`include/m2m/engine/simcontrol300_remesh.hpp` is unchanged. The public
`saveRemeshAtProjectSave` keeps its existing parameters and delegates to the
private service sibling with `nullptr`. The only new declaration is under
`src/engine/simcontrol300_remesh_services.hpp` in `simcontrol300::detail`;
neither the public include tree, C ABI, shared conversion services, nor
production `core/convert.cpp` gains a service field or route. The private
service is borrowed synchronously and is not retained in Session or results.

The implementation receipt records these frozen hashes, which match the
current files:

- `simcontrol300_remesh.cpp`: `a865bf7aead8ce3b74f7d670340125538cf7903381b9335d851f55dd2bf4c228`
- `simcontrol300_remesh_services.hpp`: `1245df3d36fe7d7e16a40d201f459adaa9a74914eac71d5e826bee6fb619b585`
- `test_simcontrol300_remesh_terminal_contract.cpp`: `c5daadea68b713b171395cc5ab4deaa7584b43d754f8490a38abf95508e3e807`

## Enabled failed-open order and fail-closed return behavior

The save path first validates the ordinary narrow absolute directory. Disabled
state still attempts `DeleteFileW` and returns before locale or service
admission. Enabled state validates the recovered C numeric locale before
constructing the fixed-name `CoggingFile`. Only an enabled, qualified
`LOCAL_REMESH.DAT` open failure records `opened=false` and consults a non-null
service profile. Unknown, Custom, invalid profiles, and the public null-service
path retain the guarded `UnsupportedSnapshot`/`invalidParameterReached`
report; they do not invoke endpoint or fail-closed hooks. The tests exercise
these guards using a real read-sharing handle that denies the writer's open,
and check the seeded bytes remain unchanged for that specific denial fixture.

On admitted FreshDefault profile, the writer sets the request's EINVAL value
and calls `dispatch`. `dispatch` is **not** declared `[[noreturn]]`; its
`noexcept` contract prevents a throwing C++ endpoint from unwinding. If it
returns, the writer preserves `PublicationFailed` or `EndpointReturned`,
maps other tags to `InvalidFailureCode`, calls `failClosed` once, then enters
an unconditional `Sleep(INFINITE)` loop. The `[[noreturn]]` annotation is on
the writer helper that itself guarantees that loop, not on a backend virtual
function. Thus the implemented return branch does not rely on control-flow
UB, ordinary report return, destructor unwind, or successful Save
continuation. A real supervisor must still impose a bounded external deadline
and stop only its owned worker.

The seven Testkit cases verify request constants and errno preservation,
unqualified-profile rejection, enabled failed-open guard behavior,
public/null-service equivalence, disabled removal order, locale refusal
before open, and equality of successful output with the public writer. No
case dispatches FreshDefault on failed open: none parks, terminates, or runs a
host invalid-parameter handler. This is correct for an isolated seam test,
not evidence that the child protocol or terminal path works end to end.

## Admission and status qualification

`FreshDefault` is a caller-supplied private profile value; the leaf does not
independently authenticate the process's startup CRT handler or prove the
`NativeFreshDefaultStartup` condition. The request's fixed `0xC0000417` status
and source address are evidence-bound descriptor fields, not an observed
candidate child exit. The implementation receipt accurately says no core
caller, CLI, C ABI, worker, or terminal harness is wired to this seam. Public
conversion therefore retains its prior guarded behavior and does not expose
a terminal status. Future activation needs a source-owned wrapper that proves
the saved `SaveCallState` and fresh-startup condition, an isolated opted-in
worker, and a deadline-bound supervisor; it must preserve the
`candidate-fixed-endpoint; native-default-handler-prelude-unimplemented`
qualification. Do not treat the private enum alone as that proof.

The CP106 receipt reports static preflight only, with no build or test run.
This review was also read-only; no source, build, GUI, or original runtime
action occurred.
