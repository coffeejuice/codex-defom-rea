# CP147 independent review: process-scoped QString codec owner

## Conclusion

The CP146 design is sound as an ownership-only correction if implemented with its stated constraints. It correctly separates the QtCore process field from Session/Project lifetime, codec-object lifetime, and each QString occurrence's data/ref owner. Unknown remains the default, and possession of the shared process owner does not qualify a codec read, decoder call, QString construction, callback, or native cleanup.

No immediate source blocker is present in the reviewed baseline. The strongest implementation risks are already named in CP146: the existing `QStringResourceProcessScope` constructor allocates and therefore cannot simply be added before the conversion `try`; its isolated profile must not become actual process knowledge; and Session teardown must not revoke/reset process state. The upcoming integration should preserve these as explicit tests/guards.

## Review findings

### Lifetime and binding

`ConversionSession` currently owns Project and Session-lifetime state and releases Project storage in its destructor (`include/m2m/engine/conversion_session.hpp`, `src/engine/conversion_session.cpp:146-207`). `convert.cpp` constructs it inside the conversion `try` (`src/core/convert.cpp:199`), while the catch/details path is outside that local lifetime. Therefore a process owner that must survive Project release, Session destruction, and exception/catch publication must be owned by an outer run/driver scope, not by Session. A new Session member may only be a borrowed pointer/reference to that owner.

The borrowing API should preserve actual identity: Sessions explicitly supplied the same process owner share one address/identity; unrelated process profiles cannot silently share it; existing packet Sessions with no such binding remain unavailable. Do not backfill the binding from `Study`, `Project`, a static singleton, or an implicit per-Session default. This satisfies the ownership distinction in the design without changing the public conversion ABI.

The repository's current `QStringResourceProcessScope` is not suitable as the outer default unchanged: its constructor calls `make_unique<Impl>()`, and the pimpl initializes `make_shared`, both potentially allocating (`src/doe/qstring_resource_owner.cpp:15-24`). The report explicitly requires an inert, noexcept, no-allocation, Unknown default or allocation under the existing exception boundary. Any change that puts the current allocating constructor before the try would alter exception behavior and is a blocker.

### Codec admission and source separation

CP146 keeps the three relevant facts distinct: raw `codecForCStrings` pointer state, codec registry/object liveness, and returned QString data/ref identity. That is necessary because QtCore's setter is a raw store without ownership transfer, the registry has a separate cleanup path, and the nonnull constructor branch performs a later pointer load before decoding. The design does not promote image-load zero-tail state or CP130's different-process null observations into a late callsite value.

The three C-string occurrences remain distinct by source site, stack slot, and call generation: SetOprHasDoe 21 bytes, DoeTableRefresh 24 bytes, and icon resource 36 code units. Sharing one authoritative process codec owner is appropriate; sharing a constructed QString data cell by text, key, or resource type would not be. CP146 explicitly forbids that conflation and keeps the existing source boundary refusing.

The process owner must start Unknown and must become Unknown across any unclosed writer interval. A `setIsolatedCodecProfile(Null, ...)` label, an initial image-null record, a common source string, or a Session binding cannot be used as a production Null read. At each occurrence, Unknown must stop before allocation; NonNull must stop before decoder/data publication until the actual pointer/liveness/result is qualified. A real same-occurrence read can establish only that read's selected branch, not retroactively close all intervening setters.

### Cleanup and failure behavior

The design correctly treats Qt global codec-registry cleanup and the `codecForCStrings` field as separate state. In particular, cleanup's registry-null store before deleting virtual calls is not evidence that the field was reset. Process-owner destruction or Session/Project teardown must not imply any native QString release, QTextCodec deletion, CRT callback, field-null store, or successful cleanup.

Failure ordering should remain stage-based: if an occurrence selected a branch or reached an allocation request before a later guard/allocation failure, retain those reached facts and do not roll back to Unknown/null or mark constructor return. Similarly, source failure in one occurrence cannot consume another occurrence's lease. CP146 specifies these distinctions and the current `QStringResourceProcessScope` already models some isolated partial states, but its isolated stages must remain labeled diagnostic rather than source-qualified.

### ABI and public outcomes

The proposed Session constructor/accessor change is internal and does not require a new C ABI argument. Preserve the existing constructor as unbound so current packet callers remain guarded. The owner is bookkeeping only: public conversion results, write behavior, exception mapping, and current unsupported outcomes should remain unchanged until actual source evidence supports a later stage. Catch/details publication must continue to see the same error/result and must not be bypassed by a new pre-try allocation failure.

## Implementation checks for the next source freeze

- Default process owner construction is `noexcept`, allocation-free and Unknown.
- Owner lifetime encloses Session/Project teardown and catch/details publication; Session stores only a borrowed identity.
- Multiple Sessions share only through explicit same-process owner injection; unbound legacy Sessions remain unavailable.
- No owner destructor, revoke call, Project Close, or Session destructor synthesizes a codec-null store, native data release, QTextCodec deletion, or CRT cleanup.
- Occurrence identity includes source site/slot/call generation; each null-branch construction gets a distinct data/ref identity.
- Source setter/read events are required to change production knowledge; isolated profiles remain non-admitting.
- The existing C ABI and public error/result behavior are unchanged.

This review is read-only. I did not edit source, build, run tests, use GUI control, execute the original application, or mutate REA state.
