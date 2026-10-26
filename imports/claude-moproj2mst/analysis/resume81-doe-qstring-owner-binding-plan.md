# Checkpoint 81: Session binding for QString path owners

The smallest safe next binding is an **unknown-state Session/Project admission holder** borrowing one explicitly owned fresh-process QtCore context. It can identify the canonical QString shared-null symbol, retain the actual Session/Project/ModelManager/runtime identities and preserve current refusal. It cannot derive a native data identity or reference count from CP78's fixed resource text, instantiate a per-Session global null counter, or advance QFileInfo.

The actual input QString is constructed inside the still-guarded flow-model receiver at `0x140086d68`. There is currently no source-bound native data/slot return from that constructor. A context holder is a useful integration prefix, not a claim that the receiver, input QString or global native counters have been recovered.

This read-only lane owns only this report. No source/build/test/GUI/app/mutable REA action occurred while CP80 acceptance/source freeze is pending.

## Existing interfaces and missing provenance

`QStringPathOwner` provides isolated reconstructed data identities, optional ref observations, same-data leases and one shared-null record per declared owner scope. Its source-qualified arithmetic/cleanup rules are not a production binding:

- `observeInput(text,observation)` creates a new projection record each time. It does not inspect an actual QString slot, intern original data pointers or prove a constructor returned. Identical text is insufficient to reuse a data identity; different QStrings can share one data allocation and equal-valued QStrings can own different allocations.
- `bindSharedNull` validates module SHA/RVA and can keep count unknown. It does not observe the loaded process/global ref word. The current binding is once-only and there is no unknown-to-known update API.
- The owner creates its own atomic ref projection from a supplied count; it does not borrow an actual reconstructed native-data ref cell that other Qt actors mutate. A copied snapshot can become stale if treated as the canonical global count outside this isolated scope.
- `qt::String` remains std::u16string plus null flag. Its buffer pointer, object address or C++ use_count is not a native QString::Data identity/ref owner. The Report QObject ledger is inert and cannot supply that missing mutable owner.

`ConversionSession` has an actual Project, ModelManager, WidgetRuntime and notifier. ModelManager.ownerSession is the correct existing source relationship; Project release leaves Session/global owners alive. WidgetRuntime supports other qualified palette/brush/thread/widget observations but no QString input-data/shared-null owner. A gradient shared-null owner or a Project-scoped QString counter must not be substituted for QtCore QString::shared_null.

## Input source point and exact binding receipt

On the matched true receiver route, original `0x140086ca0` allocates/constructs QIcon, constructs QSize, then calls imported QString(char const*) at `0x140086d68` for `:/SfTool/icon/tool/iconDoe_24_24.png`. The resulting **actual temporary QString slot/data** becomes addFile's path argument. This is distinct from Study data, the variable's collected nominal text, the DOE table UI receiver, and an arbitrary caller's descriptor.

An eventual sealed input receipt must be produced by that source constructor continuation and carry:

- Exact Session admission scope, same live Project and actual flow-model witness/generation which reached this occurrence. All registered flow models can receive SetOprHasDoe; do not require that this is the active SUB merely because B121's Study is active.
- Unique source call occurrence and site `0x140086d68`, actual source QString slot identity, returned native-data allocation/generation and module/constructor provenance. Address reuse is a new generation.
- The same data/ref owner used by its source QString and later aliases, with a current known/unknown count. Constructor return is not inferred from a qualification label, text or previously created shell.
- Actual resource String construction/codec/allocation return and cleanup state. The current shell helper builds a value request only and cannot manufacture this receipt.

At invocation, resolve the original cached receiver/model owner and revalidate its generation before reading a data slot. Do not use a newly active Study or silently re-intern by text after callback/ownership changes. Unavailable slot/data or count remains a guard, even if all metadata labels match.

## Three lifetimes, without a false global

| Owner | Scope and responsibility | What release must mean |
| --- | --- | --- |
| Fresh-process QtCore context | One explicitly supplied model of the selected original fresh-process boundary; module identity, canonical QString shared-null record/current ref state and later process-global string actors. | Ending adapter process admission invalidates native-use receipts; diagnostic snapshots may remain. It does not replay native process exit. |
| Actual Session binding | Borrows that context; captures actual Session, ModelManager/runtime identities and one admitted Project storage identity. | Session/project admission can end without deleting/resetting process-global shared-null. |
| Input QString occurrence | Temporary source slot/data plus its lease, keyed by actual source constructor occurrence. | Reached QString destructor/deallocation stages consume leases; adapter object lifetime is not a native destructor. |

The context must be created/retained outside the Session whose Project it serves, then supplied explicitly. A function-static host-process singleton would conflate reconstructed fresh-process profiles; a new per-Session null record would conflate global and Project lifetime. For the selected one-project/fresh-original boundary, the context can admit one actual Session/Project and refuse a second/foreign project. Do not claim multi-project or cross-host-process fidelity from that selected scope.

Current ConversionSession.project() refuses after storage release, so no same-Session Project recreation is admitted. Binding validation can compare `session.liveProject()` to its saved Project identity before any dereference, then check the same ModelManager/runtime shared pointers/control blocks and manager.ownerSession. Capturing weak existing owners plus explicit admission revocation avoids retaining Project or dereferencing an escaped old Session. New valid Session objects at reused addresses must fail owner/control-block generation checks.

## Smallest implementation-ready guarded prefix

Proposed new helper types, not implemented here:

```cpp
class QStringPathProcessScope;   // owns one canonical QStringPathOwner privately
class QStringPathSessionBinding; // actual Session/same-Project admission only

std::unique_ptr<QStringPathSessionBinding> bindQStringPathSessionPrefix(
    engine::ConversionSession& actualSession,
    const std::shared_ptr<QStringPathProcessScope>& actualProcessScope);
```

Process scope initially knows the immutable QtCore hash and QString shared-null RVA0x2f0b30, with **count unknown**. It can use the current isolated owner to bind that canonical unknown-count record once; repeated reads reuse the same handle. This establishes only source-global symbol identity inside the declared fresh-process context. No counter1, input record, QString lease, source constructor return or native allocation is invented.

The Session binding checks its actual live Project plus exact ModelManager/runtime/notifier and process-scope identities, and records an `InputSourceUnavailable`/equivalent guarded stage. It does not call observeInput on `DoeIconAddFileRequest::resource`, and does not expose the mutable canonical owner as a diagnostic escape hatch for injecting arbitrary known counts. A noncreating Session accessor may return the binding; absent binding remains absent, not an implicit per-call scope.

The only immediately safe source integration is to construct/store this admission holder once and consume it as an explicit guard before the first native data acquisition. The existing partial QFileInfo state must remain ref0/only4 known bytes, outer wrapper unpublished, QIcon.d null and next guard QStringPathOwnership. No native counter is changed by the holder's construction/query/refusal.

Session teardown should revoke the binding before Project storage is released; normal Project release also makes validation refuse through liveProject without touching process scope. Revocation is an adapter admission event, not QString destruction. The caller retains process context until the selected fresh-process boundary ends. Neither Project Close nor Session pointer reuse resets/rebinds shared-null.

## Later source owner integration, kept separate

To advance a reference rather than bind unknown metadata, the owner interface needs a real source-data/ref cell, interned by module/allocation generation and shared by all admitted actors. It must not copy a count observation into a second atomic counter while other source owners keep mutating the first. Native shared-null and input counters need an explicit unknown/known epoch; unqualified write intervals invalidate knownness without erasing previous observations.

The current once-only unknown shared-null binding cannot simply be rebound with a later known profile; replacement would lose canonical identity/history. A future source-assigned change may add a one-time unknown-to-known observation for the *same* data/ref cell before any acquired lease, with source scope/epoch validation. It must refuse replacement of an already-used known counter and must not turn a diagnostic string into proof. This extension is unnecessary for the first guarded holder and should not be smuggled into it.

Once the actual input constructor supplies its owner and current counts, bind its existing source lease without increment, then execute the CP80 same-pointer temporary acquisition. Preserve destination publication before the atomic increment, filename/shared-null acquisition order, temporary release, native-path-before-filename cleanup, data-free guard and ctor-unwind raw cleanup. After those qualified owners/effects, the next separate guard is the actual custom file-engine handler registry, not a synthetic null/default resource engine.

No helper or metadata receipt should produce a ReadyQFileInfo/receiver-delivered/public Type2-success token. A valid Session admission and canonical module symbol do not meet the source preconditions for those outcomes.

## Concrete source/test ownership proposal

After CP80 freeze and root assignment:

- New `include/m2m/doe_qstring_session_binding.hpp`: process scope and Session binding declarations/guarded read-only reports; no loaded-native-count defaults.
- New `src/doe/qstring_session_binding.cpp`: actual Session/Project/control-block checks, canonical unknown-count symbol owner, one-project admission/revocation and source-input-unavailable guard.
- New `tests/doe/test_qstring_session_binding.cpp`: real Session owner/lifetime tests with explicitly declared process profiles.
- Root-owned `include/m2m/engine/conversion_session.hpp` and `src/engine/conversion_session.cpp`: one nonlazy binding field/accessor, explicit process-scope binding and revocation before release. Preserve existing constructors/callers or add an explicit binding method; do not automatically construct a new global scope in a getter.
- Root-owned fresh-process/caller lifetime interface (possibly private convert.cpp scope) only if the process context is actually supplied there. It may remain absent in production for this first slice; existing conversion signature/C ABI need no change.

No edit to CP80 QString owner is necessary for unknown-state admission. Native cell/count resolution would need a separately assigned owner header/source extension after evidence closes its inputs. No public reader/writer/variable/backend source file is needed merely to install the guard holder. No source work is authorized/performed by this read-only plan.

## Focused tests and refusal boundary

1. Real Session live Project/ModelManager/runtime with explicitly supplied fresh-process context: one binding; same canonical shared-null handle and unknown count on repeated observation; no input data/lease is fabricated.
2. Require source input before `0x140086d68` returns: exact unavailable-data/count guard, unchanged existing icon/FI ref0 prefix and no shared-null ref change. Passing equal resource text or a qualification/count label cannot satisfy it.
3. Foreign Session, wrong Project, other process scope/module, replaced data-generation witness and duplicate/replacement binding refuse before mutation. Do not call through dangling references; compare live owners before following backlinks.
4. Actual Project storage release: binding refuses; process canonical-null identity/count remains unchanged. Do not report Project release as global/null/table death.
5. Session teardown/revocation with escaped diagnostic holders: no dispatch/admission after revocation, no native release/deallocation inferred, no Session/Project cycle. Process context stays independently inspectable until its own adapter admission ends.
6. Missing process context or unknown count never defaults to a newly created scope/count1. Supplying a second Session/Project to the one-project context refuses. A new separately declared fresh-process context is a distinct profile, not evidence that native globals reset.
7. Existing CP80 owner tests remain isolated; their diagnostic profiles cannot be passed to public/file-engine APIs as source constructor receipts. Existing QFileInfo/shared-null/receiver/public Type2 guards remain active.

The exact first active blocker remains the actual resource QString constructor/slot/data owner, followed by current input/shared-null reference state. The holder can make that dependency concrete without falsely advancing the path.

## Inspection receipt

This design uses CP78/79/80 frozen evidence and source; no new PE capture was needed.

| Inspected source | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/include/m2m/doe_qstring_path_owner.hpp | A6B453D7E816B19DE243F6B4ACE6678B9E707680D34EE803DFBD242F51039C99 |
| source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp | 46D4AAABA8128FC78D3D19B51158B319EC5959009EBA48184194958E25A32E31 |
| source/apps/moproj2mst/src/engine/conversion_session.cpp | F1FF5D1C3934B1DAA769F58C233B397A48C6EACB7CFDF790DBA36EDFEA820604 |
| source/apps/moproj2mst/include/m2m/engine/view_manager.hpp | B7EC2CA09E342A2FD5760B7F97C236FC23B159D54A1A9094DADB614D4793ABA5 |

Shared files were read during root's acceptance work; hashes identify this inspection snapshot, not a new frozen source candidate. Only `analysis/resume81-doe-qstring-owner-binding-plan.md` was written. No compile/runtime acceptance or native/public feature completion is claimed.
