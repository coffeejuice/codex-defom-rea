# CP78: actual Session manager binding at Open

The first executable integration edge is Open's **read of the already-owned manager**, bound to the actual Session/ModelManager and its existing owner backlink. It must not allocate a second manager or replay the native factory during `prepareProjectModel`. CP77 can record the candidate backlink as diagnostic evidence, while concrete native class/factory completion and main-window readiness remain unavailable. A separate Open read receipt must not be relabeled as CP77's constructor-return/main+50 store observation.

The native constructor order contains a step CP77 intentionally does not execute: after manager+10 is assigned, base initialization explicitly resets current before the concrete Mo class is installed and the manager is published. A future executable factory cursor must preserve that order. Chaining CP77 observation methods after a C++ constructor returns does not implement those native effects.

Only this report was written. No source/build/tests/GUI/original execution/mutable REA operation occurred. CP77 source stayed frozen. Method: current source, retained CP77 evidence and bounded offline PE/DUMPBIN. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Native creation versus Open use

CP77 closes the fixed SfMoMainWindow table1420fb108+2e0 ->0133f0 factory and same-owner backlink/class/field relationship. A validated indirect call to virtual+2e0 exists at01fe17 inside01e910..024b58. The surrounding dispatcher/event predicate is **not** closed here; do not call this a fully reconstructed startup event or claim it ran on a current native instance.

The constructor order is:

1.013407 allocates0x28;013423 calls045420 with manager allocation and the same main owner.
2.c781ee constructs QObject(parent=null);c78201 installs base class; c7820d constructs owning list+18; c7821c constructs current guard+20.
3.c7822c stores manager+10=main.
4.c78235 calls c782b0. That initializer explicitly invokes cbc920(manager+20,null) atc782c4, after the owner store. It is not the child-list-deleting `clear()` routine.
5.045449 installs SfMoModelManager's concrete class;04544c returns the same manager;01344c stores it at same main+50.

Open01ca30 instead calls c9b7c0 at01d078, then c79390 at01d085 to construct/register a **model child** on that existing manager. c9b7c0 is a direct leaf whose relevant load is c9b7ca: `[main+50]`. It does not allocate the manager or dynamically cast it. Open's manager lookup and its child factory must not be confused with0133f0's manager factory.

## Current actual source and the safe first producer

`ConversionSession` currently constructs `modelManager_ = make_shared<ModelManager>(this)` before `mainView_`. `ModelManager` declares owner_, heap lease, models_, current_, connections_; its constructor initializes owner_ in the initializer list and obtains the Session heap in its body. The list/current defaults are C++ members, not an observed native QObject/list/guard constructor sequence. No callback currently exposes the partly constructed object, but a future constructor cursor must not certify native order from this C++ declaration order.

`prepareProjectModel` uses this same manager, checks fresh empty/unbound state, constructs the Mo child, adopts slots, sets path, appends/registers the child and assigns Project.dataDb. The core calls it before `initProject`/`commitProjectModelConstruction`; current selection is later. Existing tests already verify child registration before read and no selection until commit.

The first new executable adapter edge should therefore be placed **after the fresh Open precondition and before child allocation** in `prepareProjectModel`:

```text
actual = this->modelManagerOwner()
require actual exists and actual->ownerSession() == this
bind/retain this same canonical manager as the Open c9b7ca field-read projection
record actual candidate owner backlink, without native constructor/class completion
continue existing model-child prefix unchanged
```

This is a pure existing-owner/read integration. It must not run `clear`, select a model, release Project, change a Qt field or infer native main readiness. It should not replace an existing supported Open with a new mandatory native-factory guard; the added receipt documents the current partial producer and preserves the unresolved native boundaries. Repeated/failed Open retains the exact receipt/owner identity reached, rather than issuing a fresh factory-completion claim.

## Concrete storage/interface design

Use one private Session-owned main-manager observation scope and one binding record, shared with consumers of the same actual manager. Its main identity should denote this **actual candidate Session ownership context**, not MainViewState, a made-up native address or a second QMainWindow/private allocation. A separately allocated lease/control block may delimit diagnostic scope only; its address is not a native main pointer. Revocation/Session checks must prevent escaped leases from authorizing a replacement Session. Leases must not imply native QObject liveness.

Keep two facts explicitly separate:

- **Candidate Open owner read:** internally generated from Session's actual manager member/accessor and `ownerSession()==this`, with c9b7ca source correspondence and a candidate-only origin. Callers cannot supply an arbitrary manager or readiness Boolean. This receipt may hold weak manager identity and Session scope, never an external current-Project assumption.
- **Native factory observation/execution:** CP77's ordered manager backlink/class/main-field facts remain Unknown except facts actually supplied by an original capture or an implemented corresponding producer. Nonblank source labels and a successful existing C++ constructor cannot complete the native factory or cast wrapper.

The CP77 API currently provides ordered **store observations**, not an existing-field-read API. Do not call `observeManagerStoreAt01344c` simply because Open reads modelManager_. The smallest bridge can attach CP77's backlink-only diagnostic projection to the same actual candidate manager and retain a separate Open-read receipt. If a common representation later supports direct existing-main+50 reads, it must allow known field identity with **unknown concrete type/construction**, and inspection must not label that state ConcreteMoIdentityObserved. Do not weaken CP77's current class-before-factory-store invariant as an accidental shortcut.

No shared pointer control block exists for a make_shared object until its constructor has completed. Do not call CP77 with a fabricated `shared_ptr(this)` from inside ModelManager construction. If a future executable factory records interior events, store an internal non-owning construction receipt on the actual manager during construction, then attach its canonical shared owner only after return. A native-order adapter constructor would initialize list/current storage before the owner assignment, perform the distinct current reset after it, then record concrete class and main publication. This is future work, not the proposed first pure-read edge or a claim of QObject parity.

Future file boundaries: Session's private scope/receipt and Open hook in `conversion_session.hpp/.cpp`; a small new binding helper for owner/read provenance; CP77's helper unchanged for its existing semantics. Any actual constructor-order refactor belongs to `model_manager.hpp/.cpp` under root coordination and should be a separate increment, with existing clear/current/Open tests preserved. No public C ABI, status-owner readiness or public Close switch is needed.

## First focused test

Extend the actual Session Open model test rather than constructing unrelated detached identities:

1. Create one Session and retain its actual manager. Before Open, inspect that no Open read has been committed and no native factory/main-ready fact is inferred.
2. Call actual `prepareProjectModel` on a disposable declared path. Verify the recorded Open manager is that same canonical owner; its receiver backlink is this Session. The receipt's origin is candidate storage/read, native factory/class/wrapper/public Close remain unqualified.
3. Verify the ordinary result still has exactly one Mo child, ID1, two registrations targeting this Session, child.moProject=null and no current selection. Project construction/read/selection remain the existing later commits. No extra manager was created and StudyIo binding later uses the same manager.
4. Exercise a rejected second prepare or a declared failure after registration. The original receipt and manager identity remain; no clear, replacement, selection or synthesized factory completion occurs. Foreign/expired/replacement Session receipt use must refuse without altering either manager.
5. Preserve main/view/runtime/notification/remesh/pending event state and existing unsupported currentViewManager readiness guard. A known model-manager identity must not make View_Default/Primary aliases, global main, QStatusBar or its private cache ready.

These are proposed tests; none were created/run. The first edge proves an actual candidate owner/read relationship, not native event dispatch, constructor completion, modal timing or end-to-end lifecycle parity.

## Bounded evidence receipt

New views are retained inline in this report; no raw evidence file was added. DUMPBIN14.44.35228.0 used PE/.pdata bounds. c9b7c0 has no unwind record in the inspected table; decoded leaf ends atc9b7cf, excluding alignment. Large dispatcher/Open region hashes bind source bytes without claiming transitive closure or the unexpanded dispatcher predicate.

| Region, end exclusive | Code SHA-256 |
| --- | --- |
| 140c782b0..140c782ce | `f41abe6e674a81fb7f030384883440e03a0d0bc04f60efafa1cc66b614c60752` |
| 14001e910..140024b58 | `d5e3363f68f2617b094c823de0d5ca19875eb5bff35f181c30a2d1a6be285c67` |
| 14001ca30..14001d435 | `93bfe124459aee248a64adf4b580b05dfc4c12c701117ff58923823ba6cbf9d4` |

CP77 supplies factory/class/RTTI hashes. Native factory invocation/return evidence, actual canonical main allocation and+50 read, complete QObject/list/guard construction, later main+3c0/013d40 providers, fresh global status ownership and private/cache lifetime remain unresolved. Their guards must survive the proposed first Session read integration.
