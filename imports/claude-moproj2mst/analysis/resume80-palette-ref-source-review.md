# Native palette ref helper source review

No blocking mismatch was found in the frozen **isolated reference primitive** against the CP80 native design. It preserves uint32 wrap, once-only value tokens, current-sequence late count qualification and committed zero-deref destruction guards. It does not attach to actual palette data/runtime, execute a private pointer/word store or release a public constructor guard. Those remain required integration work.

Only this report was written. No source, build, test executable, GUI, mutable REA or original execution changed. The three source hashes match editor300's released receipt.

## Native arithmetic and source sequence

The original QtCore leaves already independently confirmed in `resume80-palette-private-review.md` perform LOCK ADD dword,+1/-1 then SETNE/RET. Source `palette_native_ref.cpp:27-39` and `:43-54` uses optional uint32 bits and unsigned +/-1, preserving modular arithmetic without signed overflow or a negative/static-count special case. `nonzero` reflects updated bits. Neither numeric count nor atomic return is inferred from host shared_ptr counts.

Known ref wrappingffffffff→0 remains a successful Held token with diagnostic false; `retain():77-92` adds no false-return refusal. Known deref0→ffffffff is nonzero and does not mark destruction. Deref1→0 marks destruction pending. This matches the native branch predicate; it does not imply those raw values were observed in a valid original UI.

The private state mutation functions are accessible only to the value token. Each increments the modeled mutation sequence and appropriate ref/deref counter once and returns its before/after/result receipt. The explicit value balance counts modeled source operations rather than total native count or C++ ownership. Starting observed total2 with token balance0 is therefore valid in this bounded model.

This is serialized logical modeling of the native word and operation history, not a claim that the C++ fields themselves implement LOCK-prefixed concurrent execution. Actual runtime/thread/data association remains outside this isolated source. No public production caller uses the helper yet.

## Token admission and once-only mutation

`PaletteNativeValueRef` construction (`:56-65`) checks exact pointer/control-block identity against the state's canonical weak_from_this while the supplied object is live. A rejected equal-raw foreign block is not retained for later dereference. Valid tokens hold only a weak state owner. `validate():67-75` rechecks state availability/native admission, canonical block and nonempty qualification before any operation. Expired/released owners preserve historical stage and receipts.

`retain()` mutates only AwaitingRetain and changes to Held after its receipt commits. Repeated retain of Held is a no-op. Retain of Released refuses rather than recycling the same native value identity. `release()` (`:94-116`) refuses an unretained token; Held with unknown bits remains Held with RefCountUnavailable and no release receipt, sequence or deref mutation. With known bits, the deref receipt and Released stage commit together. A normal Released retry performs no arithmetic.

Preflight, stage and known-word checks finish before the raw mutation. The subsequent count/receipt/stage updates use only nonthrowing primitive/optional value operations; no allocation or callback lies between the committed deref and token-stage change. This avoids a later error leaving the same temporary eligible to decrement twice.

Tokens are noncopyable; host diagnostic sharing uses the same shared token. Their default destructor (`palette_native_ref.hpp:64`) performs no native release. Host report copies or abandonment cannot add a ref, subtract one, close a native unwind path or change shared numeric history. GetterTemporary/PrivateDestination kind is diagnostic edge intent; it does not certify the surrounding getter/private-store operation occurred.

## Late observations and zero destruction guard

`observeCurrent():17-24` accepts only a live qualified state at its exact current sequence and unknown-or-equal bits. Binding an unknown current word changes no sequence, ref/deref counter or value balance. A late observation therefore anchors the count **including previous unknown getter retains**, rather than replaying them. Stale sequence and conflicting known count reject unchanged.

Optional engagement is used correctly: observed0 is known and can be decremented, while nullopt is unavailable. Native ref events with unknown bits still record their one sequence/ref/balance event and keep bits/result unknown. A current-sequence observation can later admit the deref branch without retaking the getter retain.

On zero deref, state sets `destructionPending_` (`:52`) and the token becomes Released (`:108-110`) before returning DataDestructionPending. The guard is shared and sticky. Later retain/release on any token sees that pending state before another mutation. No method clears it or executes QBrush destruction/free. An equal count0 observation is allowed as an idempotent diagnostic but cannot clear the pending dependency; a different replacement count is refused.

This preserves the CP80 selected native order: temporary deref has happened once, but the zero path's60-brush destructor/free has not. The helper itself has no private slot to clear, private word to zero or destination completion to expose. A later integration must keep the already stored pointer and unknown word at this point, rather than treating DataDestructionPending as an uncommitted atomic failure or retrying release.

The normal nonzero token can be followed by a separate destination retain; it does not authorize that step until the source private word0 store and other integration prerequisites are represented. Native-owner release only invalidates callability and preserves history; it is not a data-free implementation.

## Test coverage read

The new Testkit file has15 cases and one run_all entry. Read source covers unknown initial state and exact/equal/conflicting/stale observations; one unknown getter retain; unknown deref remaining held; late count including that retain; nonzero temporary-to-destination conversion; zero release once and shared destruction blocking; raw underflow and ignored ref wrap; signed boundary/negative bits; getter wrap; two frames sharing a ledger; late count including both getter refs; host copies/destruction; canonical-block/qualification/lifetime refusal and distinct equal-qualified states.

The tests assert receipts, token stage, raw count, mutation/ref/deref counters, balance and pending state separately. The zero case verifies repeated calls do not decrement again and a destination stays AwaitingRetain without a receipt. Wrap tests verify Held despite false ref and no invented destruction guard. The rejected foreign-block test destroys the canonical owner while the foreign block remains, then checks the already rejected token without dereferencing stale state.

These are declared raw-state arithmetic/lifetime fixtures, not native numeric observations. Tests do not exercise actual palette registry/module binding, partial private slots, getter integration, complete QPalette construction, QObject/QWidget counters, GUI timing or public Save. No executable test was built/run in this review. Root must provide the focused/grouped/full stable-source validation and archive.

## Remaining integration boundary

Root must still intern exactly one state under each actual runtime palette-data/control block; qualify module/data/current-sequence observations; issue one token at each actual getter copy; retain that token through CP79's completed read; and attach a canonical partial slot before the unknown-deref guard. Existing immutable app_pal wrapper/source words and same-placeholder ownership checks must remain. A standalone helper qualification string or kind enum cannot supply that binding.

The future copy cursor must preserve pointer-store → temporary release → zero destruction guard or word0 → destination ref → font guard. No state seeding from application presence, shared_ptr counts or widget174/760 history is admitted. This isolated source requires no correction within its released scope.

## Frozen source identity

Implementation report SHA-256: `caabc4f1053b7e6f87879b7c65fc4fb4259818f6359b1f5567b04b18c3ad11af`.

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine/palette_native_ref.hpp` | `e4b3ea0aa19fcd9616593eb2d6eece522ff7ebbc9ea8aa020a107e7a602468cd` |
| `src/engine/palette_native_ref.cpp` | `188ef36821488a00a31956b44642a59476ae6a5b68eae1f97e8e1e5098be64c5` |
| `tests/mdt/test_palette_native_ref.cpp` | `1d85174f6a946c872cd694aa5add6bc2b6ff5f907dfdc5038d298fa4501d23df` |

Token measurements are unavailable.
