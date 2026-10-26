# Private palette ownership and ordering review

No ordering contradiction was found in `resume80-placeholder-palette-ref-design.md`. Its shared per-data native-reference state, one getter-temporary token and canonical partial widget slot preserve the retained native sequence. The zero-deref branch must remain guarded before destruction. This confirms a **design**, not an implemented private-copy path, runtime count observation or public constructor completion.

Only this report was written. No source, build, test, GUI, mutable REA or original execution changed. Coordination with editor300 confirmed the initial successor is an isolated new native-ref helper, with shared runtime/getter/placeholder integration still reserved separately.

## Independently confirmed native sequence

Read-only bounded DUMPBIN windows of the immutable QtGui/QtCore originals reconfirmed the full selected default QPalette body, getter-copy constructor and atomic leaves. No additional raw files were written. Original binary hashes match the pinned evidence:

- QtGui: `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
- QtCore: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

| Native site | Established ownership/order |
| --- | --- |
| `6504a8c3` | Getter's copy constructor stores the source data pointer into its own temporary. |
| `6504a8c6` | Ref that data once; its return is not branched upon. The later two masked word writes reconstruct the complete source word before return. |
| `6504abcb` | Default constructor stores that same returned data pointer in the private destination before acquiring a destination ref. |
| `6504abce` | Deref the getter temporary's data. This consumes the temporary reference, not the destination's not-yet-acquired ref. |
| `6504abd4..abd6` | Test the returned bool; nonzero skips the data destructor/free path. |
| `6504abfb`, `6504ac03` | Zero path destroys60 QBrush elements, stride8 from data+8, then frees the data. These are real intervening effects. |
| `6504ac10` | Only after that branch returns, set the entire private destination word+8 to0. |
| `6504ac17` | Ref destination data once; caller ignores the return. |
| `6504ac25` | Return from default QPalette constructor; QWidgetPrivate's QFont call at `6505e8de` is later. |

The data pointer at the first private store is copied from the getter result while the temporary deref uses the same data pointer from its stack temporary. A design must not substitute application default/class-cache data, a new graph, private word0 before release, or a ref on the app_pal wrapper object itself.

The selected nonzero branch contains no brush-destructor/free callback between temporary deref and the private word0/ref tail. The zero branch does. Nothing in the proposed guard justifies declaring that branch absent merely because a diagnostic app_pal wrapper exists.

## Raw arithmetic and branch fidelity

The complete atomic leaves are:

```text
67001aa0: lock add dword ptr [rcx],1
67001aa4: setne al
67001aa7: ret

67001ac0: lock add dword ptr [rcx],0xffffffff
67001ac4: setne al
67001ac7: ret
```

They have no sentinel, positivity, saturation or static-data exemption. The design correctly uses uint32 modular arithmetic: deref false means the updated bits are0, not that the old count was already0. Explicit current1 selects destruction; current0 decrements toffffffff and skips it. Destination ref may wrap to0 and return false, but adding a refusal there would contradict the ignored native return. These unusual raw cases are arithmetic tests, not evidence a valid original UI supplied them.

The copy constructor's getter ref also has an ignored return. Therefore a revised integrated getter may record its native +1 with numeric bits still unknown, while preserving that unknown numeric state and advancing a separate mutation sequence. It need not invent a positive initial count to finish the getter value copy.

## Required identity and history model

The proposed ledger belongs to the exact runtime-interned WidgetPaletteDataState/control block. One raw alias number, one QWidget or one call-local counter is insufficient. Two getter frames sharing actual data must share its numeric/sequence/lifetime state while retaining separate temporary identities. Equal aliases in another runtime cannot supply that state.

Every successful integrated getter needs one actual source token/ref acquisition at the copy constructor, including when numeric bits are unknown. Host copies of a report must share that token and create no native ref. App-pal slot observation names an existing wrapper and creates no native reference. C++ shared_ptr counts/destructors must not determine or update the Qt word.

The design's current-sequence observation rule is essential. A later current-word observation **already includes prior getter +1 operations**. It may anchor unknown bits at that exact sequence but cannot replay earlier refs, reset conflicting known bits, substitute foreign data or repair omitted external history. This is a new model execution from a qualified beginning, not a retrofit that guesses a native retain for an old running CP79 frame.

Unknown-deref refusal leaves the temporary held and unconsumed. Once deref executes, its mutation/sequence/token consumption must commit together and the cursor must advance before any pending destructor boundary. A retry or diagnostic report copy must not decrement again. A known-zero result is not itself proof data has been freed; mark shared destruction-pending state until actual brush/free closure exists. That pending state must guard another frame from treating the data as an ordinary available source while this destruction remains unrepresented.

Host abandonment can drop metadata leases but cannot perform guessed native unwind, temporary release or destination release. Any unclosed source value/lifetime must remain explicit on the bounded shared history rather than silently subtracting a reference or authorizing another frame from apparent C++ liveness.

## Partial private slot and exact guard

Current WidgetState exposes only a fully known `WidgetPaletteValue`. That cannot represent a committed data pointer with an unknown destination word between `6504abcb` and `6504ac10`. The proposed canonical partial constructor slot is therefore necessary if the implementation advances past the pointer store.

The correct progression is:

1. Validate same placeholder/runtime/registered app_pal origin and completed getter's actual token/data ledger.
2. Store the data pointer once into that same widget's partial slot, leaving its word unknown and no destination native ref.
3. If bits are unknown, stop before deref, preserving the pointer and getter token. A late observation alone does not resume or complete copying.
4. If deref reaches0, retain count0, consumed temporary and stored pointer; stop before60-brush destruction/free. No private word0, destination ref, font or completed palette is admitted.
5. If deref is nonzero, write word0 then ref the destination once, ignoring its return. Only after this source sequence may the complete same-data/word0 value and destination token be exposed, then stop at the independent private-font guard.

The completed palette and construction slot must be two views of one authority, not separately mutable records that can duplicate a retain or replace data. Runtime loss at every pending stage retains source history and blocks further work. None of these stages initializes a font, locale, thread, registry or QWidget/QObject counter, opens earlier public SplitView gates or proves startup/Open/Save completion.

An isolated ref primitive is a coherent first implementation split, provided it cannot execute private copying by itself. A pointer-store-plus-unknown-ref integration is also honest if it uses the actual canonical widget partial slot. A complete nonzero branch needs the full shared runtime/getter/slot integration; helper tests alone cannot establish it.

## Acceptance checks required at implementation

The design specifies the decisive cases: unknown getter refs with current-sequence late count3; known pre-getter2 producing3 then temporary3→2/destination2→3; two getter frames on one data ledger; committed zero destruction guard; raw0→ffffffff→0 with ignored final false; stale/conflicting/foreign observations; report copies sharing one token; runtime loss and abandonment without replay/rollback.

Review of the future source should additionally check that all count mutation and token-consumption preconditions finish before mutation; no partial failure consumes a token without its count/sequence effect; zero-destruction pending guards new sources; and exposing a completed palette cannot duplicate a destination retain. Tests must distinguish native-token identity from C++ metadata retention and must not seed counts from app_pal presence or historical QWidget counts174/760.

No such tests were authored or run in this review. The report's proposed tests are candidate contracts, not original count observations. No source correction to the frozen design is required.

## Evidence and design identity

Reviewed design report SHA-256: `64f78ddad2e3b29a7e3e83058b142bee3b542e60ca4a125d8f6b7f269a11671a`.

| Independently rehashed native range, end exclusive | SHA-256 |
| --- | --- |
| QtGui `6504abb0..6504ac26` | `704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399` |
| QtGui `6504a8b0..6504a8fd` | `a0c82e2e8037dc55d2034c9b82ad3ae911f3c4af2e740438ecfbe4e9970cc03b` |
| QtCore `67001aa0..67001aa8` | `7b57a50acb800ca3e7d326b624b14349c569ea06f18e594273c33a92fe1256de` |
| QtCore `67001ac0..67001ac8` | `06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6` |

Microsoft DUMPBIN14.44.35228.0 was used offline; RET-bounded leaf/code hashes exclude alignment/neighbor code. No numeric original ref count was acquired. Token measurements are unavailable.
