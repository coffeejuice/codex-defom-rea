# CP97 initial prepared geometry query: source release

The low-record initial invocation now acquires its synchronous native-step
borrow immediately before installing its own barrier. It installs, executes
the same closed directory query, observes postVersion and writes QueryCommitted
inside that one callback, then returns the existing pending result. This does
not create a native lease, permit later continuation or alter frame pointer
observations. Source/test files are frozen for root validation.

Root explicitly approved this scope after the two independent CP97 reviews.
Both reviews were read before editing. The current source matches their
serialized, callback-free prefix assumptions; no ordering/reentrancy conflict
requiring scope widening was found.

## Exact production change and preserved effects

Only viewer.cpp's low-record install/query/postVersion/stage block changed.
All actual owner/profile/reader/receipt/physical-index/record-version checks,
native COW/table/timer prefix, frame allocation and metadata snapshots remain
in their original positions. In particular frame->step is still assigned
outside the callback and remains historical metadata without a new read,
comparison, dereference or authorization role.

The existing withStep preflight executes while the model is unparked at the
old install position. It supplies actual nativeStep to the void callback,
whose explicit captures are model/reader/index values and &frame. The exact
order inside is:

1. Existing GeometrySyncAccess::install; failure returns from the callback,
   skips query/post-version/stage, then outer code returns the same pending
   result. No new outer success result is introduced.
2. Same step_dir_sizes call once, with nativeStep, reader, index and addresses
   of frame.hsize/marSize/arraySize/recordVersion. Store its same uint64 result
   in frame.queryResult.
3. Read nativeStep.step_version into frame.postVersion.
4. Store the same QueryCommitted stage; end callback/std::invoke scope.

The DB shared owner stays in its existing outer scope, keeping the actual
reader live through the synchronous call and releasing on the same return/
unwind. Installation writes only the private barrier; the qualified native
query uses direct field/record reads and conditional version write without
an event or unknown callback. Neither operation replaces GuiModel.step.
No nativeStep pointer is returned or saved. The ordinary high-record query
from CP95 remains unchanged. General guards and all parked retries/mutators/
reset refusal stay unchanged.

If the query throws after install, callback unwinding leaves the installed
prepared frame and already committed effects; no catch, rollback, replay or
retirement was added. The scope does not re-enter a guarded public accessor
after installation. External concurrent/raw step replacement remains outside
the existing serialized-call contract; no lock or generation is invented.

## Focused regression and limits

The existing checkGeometryDirectoryPending helper now captures the actual
model.step.get() before using the native reference, requires it nonnull, and
REQUIREs that same pointer immediately after every viewerRefresh, before
subsequent assertions use the saved native reference. This applies to existing
high/low record, bounds, owner/receipt and reload profiles, including both
initial invocation and parked retry where applicable. No new test API or
private frame diagnostic was exposed. Target count remains45.

This is an endpoint identity regression. It does not alone instrument the
query argument or prove absence of transient replacement/ABA. Static review
binds the query's first argument to the callback nativeStep, shows no owner
replacement/callback in the scope and establishes its exact lifetime. Existing
four low-record rows check first-call version mutation/pending behavior; the
actual two-View parked test preserves later-View/retry/mutator/reset refusal.
The normally unreachable install-failure branch was preserved statically;
no artificial callback or corrupted receipt was introduced to force it.

Exact focused target: m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box. Root owns
compilation, runtime tests and complete source-stable/export/ABI acceptance.
Worker performed static preflight only.

## Static binding receipt

Passed balanced/order/reversal checks:

- Exactly one install and one native query in the selected void callback;
  install precedes query, postVersion and stage. Callback ends immediately
  after the stage. Existing outer comment/pending return remains.
- All seven native arguments and four output identities are preserved; query
  result is still stored. Historical frame->step assignment stays outside.
- Reversing only this block wrapping reconstructs the complete CP95
  viewer.cpp SHA-256 7274e7772e6dc0c6dbf02a9a0811ee77e65bf9b683e474831cab54f3f9797f75.
- Reversing only the test helper identity capture/assertion reconstructs its
  complete input SHA-256 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98.
- Test quoted includes resolve against actual roots, main retains run_all,
  checks remain Release-active and no unsupported REQUIRE_EQ/C assert was
  added. No existing test or guard was removed.
- Helper/model/public C headers and ABI implementation hashes remain unchanged.
  Existing bool/void, pointer/reference rejection, native layout, GuiModel
  step type/nonpolymorphism, APIversion1 and six C signatures remain active.

Final SHA-256:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | e2287cac017f17035e7c6f172d6f2765300a7c56ff0207b489e7b07142cdbede |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 2be69b66f3e1e06585fbe608f650381dc3bb2b57269a974716da44bb6731dee9 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source qualifications remain CP71 complete native query, CP77 owner/receipt
binding, CP78/79 parking order, CP96 scope design and the two CP97 independent
reviews. No new native execution or behavior beyond that closed query is
claimed. Worker performed no build, CTest, GUI or mutable REA action.

Read qualification bindings (SHA-256):

| File | SHA-256 |
|---|---|
| analysis/resume97-prepared-query-scope-review.md | 7de426c2844955013af91816bd6787b4aa0311061e0b310c340b610a3b0c2b32 |
| analysis/resume97-geometry-preinstall-borrow-review.md | e3a461b04f5221db4ed71f8d4aebef8f7af5f33be0ae787aa9223914e1a2265b |
| analysis/resume96-geometry-access-frontier.md | 473334cfda4aa899fe45918787d82384b2d010053b2dfd5b3e71147a08066439 |
| source/port/defdbio/src/dbread/w2_objarrays.cpp | c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359 |
