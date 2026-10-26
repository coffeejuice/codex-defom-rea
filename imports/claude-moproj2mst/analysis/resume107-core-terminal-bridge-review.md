# CP107 private remesh core bridge: static review

**Result: no blocker found in the forwarding path.** The new bridge takes a
snapshot at the remesh dispatch boundary and reuses it on the writer's
return-failure callback. Public conversion entrypoints keep their null
service argument and no public result or C ABI declaration was changed.

## Source order and snapshot lifetime

In `core/convert.cpp`, the project writer's result and
`projectWriteReturned` are recorded before the step-4/property and MST work.
The native MST branch records `mstWriteAttempted`, `mstWriteReturned`, and its
success status before the remesh save call. The local optional forwarder is
constructed immediately before that call only when
`services->remeshTerminal` is non-null. It holds a reference to the live
`saved.call`, whose scope encloses the synchronous writer call.

`RemeshTerminalForwarder::dispatch` copies that live value into `snapshot_`
before forwarding. If the terminal dispatch unexpectedly returns, the
remesh writer calls `failClosed` after dispatch; the forwarder passes the same
captured value to the target. The test mutates the source `SaveCallState`
between these callbacks and checks both target records retain the dispatch
snapshot, including project-failure and MST-success flags. This matches the
leaf writer's actual callback order.

One narrow defensive-path caveat: `failClosed` copies `live_` itself when
`captured_` is false. The writer currently cannot reach this branch because
it calls `failClosed` only after `dispatch` returns. The fallback does mean a
misordered future direct caller could sample a later state rather than fail
closed with the dispatch snapshot; the current valid core path and test do
not take that route.

## Compatibility and test boundary

`RemeshTerminalCallService`, `RemeshTerminalForwarder`, and
`ConversionCallServices` live in `src/core`. The new service pointer is
appended after the existing `projectOutput` field, so existing aggregate
initializers such as `{&factory}` retain their meaning. `convert.hpp`,
`ConversionDetails`, the C ABI declarations, and public result layout expose
no new service parameter or status. `convertDetailedWithServices` is declared
only in the internal source header. Existing public wrappers call `run`
with a null services pointer; the normal `ConversionCallServices{}` state
also has a null remesh service.

The new one-case Testkit source has a `run_all` main, resolvable relative
includes to the two private headers, supported `CHECK`/`CHECK_EQ` macros, and
no `assert` or unsupported `REQUIRE_EQ`. Its case tests the forwarder
directly; it does not invoke the complete conversion/Open/Save path or the
leaf writer's actual admitted failed-open dispatch. The CP106 writer test
still deliberately avoids FreshDefault failed-open dispatch, which parks.
No compile or runtime result is claimed here; root owns build validation.

The bridge transports a snapshot but does not implement a terminal endpoint,
worker, protocol, or supervisor. Public conversion/C ABI callers remain
unconnected to the private service. Any later activated service must still
prove that it owns an isolated opted-in child and preserve the fixed-endpoint
qualification.

No source, build, GUI, or original application changes were made for this
review.
