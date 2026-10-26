# CP222 step identity implementation batch

The first ownership migration batch adds observation of GuiModel's owner and
storage generation. It does not establish ownership closure or permit R300 cache1
publication. The existing public native storage and all cache1 guards remain.

## Implemented boundary

`StepIdentity` stores only a weak reference to the existing GuiModel lifetime
control block and a private 64-bit generation. Only GuiModel can construct a
bound token. Callers can create an inert default, copy tokens, compare equality,
and ask `isCurrent()`. `GuiModel::stepIdentity()` issues the current observation;
`isCurrentStepIdentity()` additionally checks that exact model owner. There is no
pointer/address conversion, pointer-bearing result, resolver, or native access
grant. Equality compares weak control-block identity, including after expiration,
and generation, rather than comparing a model address.

The existing private lifetime record holds generation 1 at construction and an
observation-live flag. Tokens never lock, recover or dereference the GuiModel;
currentness reads only the independently owned lifetime record after locking its
weak pointer. Destruction retires observations before cleanup and otherwise keeps
the existing Guard/member destruction order. The last weak token cannot keep the
model alive. A new model at the same address has a distinct lifetime control block.

Reset retains its geometry check and null-step no-op. Before the existing first
destructive effect (`viewerClear`), it checks generation exhaustion, then advances
the generation. It preserves viewer-clear, mirror-clear, native `step_clear`, GUI
simulation-control initialization, DOE property reset, and state100 reset order.
The increment is not rolled back if later work throws. Exhaustion refuses before
destructive work, using the existing unsupported error mechanism; no wraparound
can revive a token. No test-only state mutation seam was added to force exhaustion.

## Qualification and remaining gaps

The behavioral ordering follows the current source's qualified native locations:
constructor `0x140c7a5b0` / `0x140c7a870`, reset `0x140c7b720` and
`0x140c82c70` / `0x140c8ba80`, native clear `0x1408246a0` twin
`0x1401cb400`, and existing destructor cleanup comments. This batch adds adapter
metadata; the original executable does not establish these new token semantics.
No new original capture, native continuation evidence, or GUI interaction occurred.

Direct public `step.reset()` or assignment still bypasses invalidation. Other
destructive loaders and raw/interior aliases are not migrated. Thus `isCurrent()`
is only an observation of the instrumented owner transitions; it cannot certify
native aggregate identity after legacy mutation, grant native admission, or
support selected-length/cache1 publication. The focused test explicitly records
this bypass so the staging limitation remains visible. The C ABI is unchanged.

## Validation

`tests/mdt/test_step_identity.cpp` links the real `m2m_core` through the existing
test glob, with no mocks or standalone doubles. The five Testkit cases cover:

- inert/default, constructor currentness, exact equality and foreign-owner refusal;
- repeated reset invalidation despite the same native step address, plus native
  nstep/state100 reset results;
- expired owner, weak Guard expiration and retained token equality;
- safe `std::optional<GuiModel>` destruction/reconstruction at the same model
  address with old-token refusal;
- the deliberately unmigrated public step replacement and null-reset no-op.

Compile-time checks reject public construction from an owner/generation and
conversion to native pointers or an integer address. The read-only
`admin/check_new_cpp_bindings.py` preflight passed for all three changed C++ files;
quoted `m2m/engine.hpp` resolves in the existing public include directory and the
test has a Testkit `main` with supported checks. `git diff --check` passed for the
tracked source edits.

The root owns the MSVC wrapper build and grouped/final integration tests to avoid
competing builds. Expected focused target: `m2m_mdt_step_identity`; CTest name:
`m2m.mdt.step_identity`. No build or runtime pass is claimed by this implementation
note until that integration evidence exists. Full closure remains a later batch.
