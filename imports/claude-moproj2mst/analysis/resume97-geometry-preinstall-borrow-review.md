# CP97 — independent review of the prepared geometry borrow scope

## Review result

The proposed same-invocation scope is source-compatible with the current access seam, provided its callback stays exactly bounded to install, `step_dir_sizes`, and recording the existing frame fields. The early `withStep` preflight runs while the model is unparked; its callback then installs the barrier and continues directly without re-entering a public guarded accessor. `install` only assigns the barrier, and the directory query is a closed field-read/write routine with no callback dispatch. There is no source-visible reentrancy between installation and scope exit.

I found no reason to change the decision to install before native version mutation. I would qualify the safety claim with an explicit serialized-call assumption: neither `GuiModelAccess::withStep` nor the public `GuiModel::step` field synchronizes a concurrent external replacement. Under the existing single-call execution model, the active `shared_ptr<GuiModel>` owns its `unique_ptr` step throughout the callback. The retained source does not establish safety against an unrelated thread directly mutating public fields.

The proposed same-instance assertion is useful but limited. Checking `model.step.get()` before and after refresh shows endpoint identity; it does not, alone, prove which pointer the query received or rule out replacement and same-address reuse during the call. The static callsite review proves the proposed call passes the callback's `nativeStep` to `step_dir_sizes`, and the callback has no step replacement. Treat the test as an end-state regression, with source review as the argument-identity evidence; do not describe the test alone as proving pointer identity throughout the interval.

No source or tests were changed and no build/test was run.

## Owner and pointer lifetime

- The outer `dbOwner` is a strong local copied from `project->dataDb` and remains live across the prepared call. The `reader` raw pointer was validated against that owner before the call. The installed frame stores only a weak `dbOwner`; it does not prolong the reader after the current invocation returns. This is appropriate for a parked barrier that refuses later continuation, and the callback's captured raw reader is used only synchronously while the strong local remains in scope.
- The model is passed by reference to `withStep`, but the refresh path's `model` is a strong shared owner for the duration of this synchronous function. Its `step` unique_ptr therefore remains owned by that model unless some other actor mutates the public field concurrently. No lock or borrow count prevents such a concurrent write.
- `frame->step` is assigned before installation and remains a historical raw identity only. The proposed lambda does not capture or return `nativeStep`; it passes it immediately to `step_dir_sizes`, reads `step_version`, and ends. The frame/DB receipt remains parked metadata, not authority for a later dereference.
- The `withStep` template accepts only void or bool-returning callbacks, which prevents a direct pointer return. It cannot prevent storing the pointer into a captured variable or global. The exact proposed lambda does not do so, so non-escape is a callsite invariant enforced by review, not by the helper type.

## Barrier and exception order

`withStep` calls `model.requireGeometrySyncAvailable()` before reading `model.step.get()`. `GeometrySyncAccess::install` then refuses if a frame is already present and otherwise stores it. Therefore an already-parked entry is refused before acquiring the pointer, while this initial invocation can proceed across its own install because it does not call `withStep` again.

If install returns false, the callback must return without querying; the outer function preserves the pending result. If the native query throws, stack unwinding releases the borrowed scope but leaves the already-installed barrier and any committed partial writes intact. No cleanup or replay should be added. The proposed callback invokes no public mutator/reset path, so it does not encounter the newly installed barrier through a second admission check.

The implementation must keep the exact argument sequence: `nativeStep`, the already-qualified reader, the same index, and addresses of the four existing frame output fields. It must retain the query return, post-version read, and stage assignment before callback exit. No event pump, owner reload/replacement, or unknown callback may be introduced in the scope.

## Regression claim and source evidence

The exact same-instance test proposed in CP96 is a good focused guard against an accidental net replacement on the exercised path. It is not an interval or argument instrumentation test. The existing standalone `step_dir_sizes_and_version` regression covers the directory helper's outputs/conditional version behavior but does not bind the GUI callsite's borrowed pointer. The strongest combined evidence is therefore:

1. a candidate runtime assertion that `model.step.get()` has the same value immediately before and after the refresh;
2. static review that the callback passes its `nativeStep` parameter to `step_dir_sizes` and does not save it;
3. existing barrier tests that another View, retry, mutator, and reset remain blocked after installation.

The native routine in `source/port/defdbio/src/dbread/w2_objarrays.cpp` reads the supplied step's directory index and conditionally writes its version; it performs direct memory accesses and calls only the local stored-version/setter path. No user callback or Qt event dispatch appears in that body. Source hashes at review: `viewer.cpp` `7274e7772e6dc0c6dbf02a9a0811ee77e65bf9b683e474831cab54f3f9797f75`, `gui_model_access.hpp` `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26`, `gui_model.cpp` `26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45`, `engine.hpp` `77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f`, `w2_objarrays.cpp` `c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359`, and `test_viewer_box.cpp` `58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98`.

The review does not qualify any later continuation. Once this callback returns, the weak receipt/step identities do not authorize reuse, and public raw-field writes remain outside the adapter barrier's interception.
