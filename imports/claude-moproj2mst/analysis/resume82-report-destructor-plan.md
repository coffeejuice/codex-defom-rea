# CP82: actual SubProject Report guard and conditional release prefix

The next implementable prefix is an isolated read of the **actual SubProject's Report guard slot**, with Unknown/KnownNull/KnownNonNull kept distinct. A known-null first read returns from the native helper without document deletion or QPointer assignment. A nonnull read must stop before the actual document's deleting virtual, because the converter has no executable Report document owner/destructor. Do not add Report deletion to the existing noexcept SubProject destructor or infer accepted normal Close.

CP81 newly qualifies explicit SubProject-driven release of a live Report document, despite its separate main-window QObject parent. The guard is not a unique owning smart pointer: SubProject can explicitly delete the target, and prior QObject destruction can independently invalidate the guard. The same canonical document/lifetime must serve both mechanisms before nonnull deletion is implemented.

This report began as read-only design while CP81 source was frozen. Root subsequently authorized the exact source lane after CP81 freeze; the final section records that implementation. No build/test execution/GUI/original execution/mutable REA activity occurred. Evidence: `resume81-report-close-pe.md`, retained earlier Close/Report evidence, current source, bounded immutable-PE disassembly and RTTI. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Current ownership map

Paths are relative to `source/apps/moproj2mst/`.

| Actual candidate location | Current behavior | Report gap |
| --- | --- | --- |
| `Project::subs`, `include/m2m/model.hpp` | Owning vector of unique SubProject allocations; Project destructor is default. | Dropping vector storage is adapter cleanup, not source-ordered native diagram/Project/SubProject teardown. |
| `makeSubProject`, `src/model/subproject.cpp:289` | Constructs the actual SubProject, binds Project/flow witness/type/id/DOE aliases/dbName. | No Report+30 guard, document construction, parent or publication is represented. |
| `SubProject`, `model.hpp:263` | Owns its FlowChart/Type2 study and candidate+48 ownDb projection. | No Report document field or lifetime owner. An absent C++ member is not a source-proven null native guard. |
| `SubProject::~SubProject`, `src/model/subproject_lifetime.cpp:7` | Records adapter flow-receiver witness death, then ordinary members destroy. | No original+48 delete, Report helper, flow deleting virtual or QPointer teardown cursor. |
| `ReportClass::loadClassData`, `src/ops/report.cpp` | Still refuses before accessing/constructing the GUI Report document. | Public Report activation remains guarded; no constructor-returned document may be invented for cleanup. |
| `ReportQObjectSharedRefLedger`, engine helper | Inert supplied-observation ledger for a pending QObject constructor prefix. | Neither a Report allocation/guard owner nor executable constructor/destructor/liveness authority. |

A `unique_ptr<ReportDocument>` on SubProject would be the wrong first representation. It would make C++ destruction perform an unconditional delete and would not model main-parent destruction invalidating the same QPointer first. Likewise, a weak_ptr expiring is not a native guard-null producer unless actual QObject invalidation was represented.

## Exact helper branches and deletion/clear ordering

CP81's helper is0b5bb0..0b5c2b, end exclusive. Bounded disassembly confirms a narrower null branch than a general “clear slot” description:

```text
0b5bc2  first pure QPointer value getter(SubProject+30)
0b5bc7  test returned pointer
0b5bca  null ->0b5c26 return        NO cbc920 on this branch
0b5bd5  second pure getter of the same slot
0b5bef  second-null ->skip deleting call
0b5c03  nonnull second result: deleting virtual+18(flag1)
0b5c21  after normal delete/skip return: cbc920(actual slot,null)
0b5c26  return
```

The first-known-null prefix therefore performs **no pointer store, guard reassignment, Qt call or document destructor**. It should not call reset “for convenience.” A first nonnull result does not permit clearing before the document virtual returns. The second getter has its own null check; if independently qualified second-null is selected, the later guard-clear helper still runs. Do not conflate that with the initial-null direct return.

After the document deleting virtual returns, cbc920 compares the **then-current slot** to null. CP80 qualifies its equal branch as no-write/no-changeGuard. If QObject teardown already nulled this guard, that branch skips. If still nonnull, the unequal branch calls QMetaObject::changeGuard; those effects remain separate. Reentrant callbacks could replace the slot: final assignment then concerns the replacement guard target, not the earlier deleted document. It does not delete that replacement again. Never clear a cached old target instead of the actual current cell, or use a supplied “delete completed” Boolean to bypass the real destructor.

The full SubProject cleanup0b58d0 first conditionally deletes+48, then calls the Report helper at0b5954, then deletes its raw flow/model+0, and continues member/guard cleanup. A production SubProject-destructor coordinator must preserve that earlier+48 phase. Direct entry to0b5bb0 is a narrower conditional seam and does not certify prior SubProject phases; CP81 also finds another helper caller1412ae046 whose context is not yet classified.

## Root-order reconciliation

CP81's direct-call scan does not by itself establish accepted-Close reachability. Earlier retained evidence does, however, provide a **conditional static Mo-child virtual bridge** that should not be discarded because it is indirect:

```text
manager c78300: encountered child deleting virtual+18(flag1)
RTTI SfMoModel table142112878+18 ->045350
045350 ->044040 ->0440c0 ->044bf0
044bf0 ->Project delete0453e0 ->08a3c0
08a403 ->0a80e0 ->SubProject delete0ac330 ->0b58d0 ->0b5954/0b5bb0
```

This lane independently decoded142112878's RTTI as SfMoModel and its+18 target045350. `resume46-close-manager-tail.md` and retained wave3 contracts qualify the enclosing order. This closes a selected **static** path for an encountered actual Mo child; it is not an observed accepted-Close root, completed can-close/modal/refresh phase, proven live child/vtable at that occurrence or same-instance ordering against main-window destruction.

Keep the root guard: accepted Close must reach the real nonempty manager/Project/SubProject coordinator through prior guards and effects, with actual child/document/main lifetimes. Existing `releaseProjectAfterCloseEffects` and default C++ destructors cannot stand in for it. A null Report guard could result from its constructor default, explicit publication/clear history or earlier parent-driven QObject invalidation; distinguish those producers instead of assuming which occurred first.

## Smallest source task after freeze

Proposed exclusive files:

- **Shared, reserve first:** `include/m2m/model.hpp`, to put one actual Report guard-state component on SubProject.
- **New:** `include/m2m/engine/report_document_slot.hpp`, `src/engine/report_document_slot.cpp`, `tests/mdt/test_report_document_slot.cpp`.
- **Receipt:** a new implementation report when authorized. No existing Report operation, QObject ledger, Project/SubProject destructor, CMake or public Close source edit in the first increment.

The small header must forward-declare Project/SubProject and avoid including model.hpp back into itself. A value state component avoids an inline-default-constructor/incomplete-unique_ptr hazard. Before implementation, audit SubProject copy/move/value call sites because a noncopyable lifetime-bearing slot makes those special members unavailable. Do not silently add global identity interning or a second actual SubProject/document owner.

Use Unknown as the slot's initial knowledge. Explicit observations may record the actual slot's first known null or nonnull opaque document identity; labels are provenance, not original capture authentication. The first increment should neither initialize a late slot from constructor defaults nor create a ReportDocument object. Nonnull identity can remain opaque at a guard; actual allocation/lifetime/vtable resolution is mandatory only when progressing to deletion. Expose no dereferenceable historical raw document pointer.

Bind progress to this actual SubProject's member and a private adapter lifetime/control-block identity. The helper obtains the slot internally rather than accepting a detached `hasReport` Boolean or another owner's slot. SubProject can be in teardown; do not require a global current Project lookup, or infer native liveness from the adapter scope. Scope expiry must retain unavailable/non-null distinctions and refuse reuse on a new allocation at the same address.

The helper `attemptReportDocumentReleasePrefix(SubProject&, progress)` can then:

1. Validate actual SubProject/slot/scope and the exact0b5bb0 occurrence; construction itself does not prove root entry.
2. Unknown stops before0b5bc2. First known null latches `NullGuardReturnReached` at0b5c26, with no delete/clear/Qt effect.
3. First nonnull retains that value/identity and stops before0b5c03's actual deleting virtual (or before an unresolved second read/target when needed). Never turn missing/expired metadata into the initial-null branch.
4. Retry retains the consumed guard branch and refuses incompatible later observations. It must not delete a later replacement under the old occurrence, replay a completed read or mark SubProject/normal Close completed.

Actual document deletion and the post-virtual QPointer-clear continuation are a later increment, requiring the real Report deleting target/payload/QObject guard effects and then-current cell. A success Boolean/mock callback is not that producer. No call from the existing noexcept SubProject destructor is authorized by this first isolated helper.

## Focused tests

Proposed target `m2m_mdt_report_document_slot` should use an actual Project-owned SubProject, for example a normal makeSubProject/p.subs fixture, while marking supplied guard observations as declared candidate data:

- Actual slot starts Unknown despite empty C++ metadata; no automatic native-null inference or Report object allocation.
- Explicit known-null first read returns with **zero delete and zero guard-clear**; repeat does not replay it.
- Known nonnull/opaque or unavailable document stays guarded, retaining the same identity and all actual SubProject ownDb/flow/DOE/Project/operation storage.
- First-nonnull progress cannot be bypassed by externally observing null, replacing the document, dropping metadata or reusing a SubProject address/scope.
- Foreign SubProject/slot/scope and unqualified occurrence refuse before effects; historical identity access is opaque, and expired scope cannot authorize a replacement.
- Creating/destroying the adapter component invokes no Report virtual, QObject ledger completion, main-parent teardown, witness death, public Report activation or native Close replay.

Future nonnull-executor tests must verify delete-before-clear, guard auto-null during QObject death versus still-nonnull changeGuard, replacement-on-return affecting only final guard assignment, and failure at the deleting virtual preserving reached phase without rollback or early clear. Root-order and original one-project acceptance are distinct integration tests. The first-prefix tests were subsequently created as recorded below; none were executed by this worker.

## Evidence receipt and remaining work

New disassembly/RTTI facts are retained inline here; no raw file was added. DUMPBIN14.44.35228.0 supplied the bounded helper body. CP81's inclusive0b5bb0..0b5c2a code hash `2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9` maps to this report's end-exclusive0b5c2b. Its SubProject/Project/destructor-wrapper receipts remain attached. CP80 supplies cbc920 equal/unequal guard-call qualification.

Remaining guards are actual Report constructor-return/publication and canonical document ownership, its deleting target/payload/QObject effects, post-delete current guard reassignment, preceding+48/Project/manager phases, actual accepted-Close/modal/root lifetime ordering versus main-parent teardown, and original same-instance lifecycle acceptance. The proposed isolated null/nonnull prefix does not close them.

## CP82 implementation receipt

The actual `SubProject` now contains a `ReportDocumentSlot` value component. It starts Unknown; the C++ scope/empty token does not imply native null or Report construction. Explicit first observations bind known null or a nonzero opaque document identity, with first receipt retention and conflicting rebindings rejected. No historical document pointer is exposed or converted from the token. Future actual constructor-publication/clear effects require distinct qualified producers; the initial-observation API cannot manufacture them.

`ReportDocumentReleaseProgress` binds the actual SubProject/member and its private weak adapter scope. Unknown waits; known-null first read returns at0b5c26 with no document deletion or pointer assignment; nonnull preserves the same immutable observation and stops before document-vtable/deleting-virtual effects. Retry preserves the consumed branch. Foreign member/scope or same-address replacement refuses. Adapter component destruction only releases metadata; no native lifetime/callback is inferred.

No Project/SubProject destructor, Report operation/ledger, Session, CMake, public ABI or public Close code was changed. Slot metadata construction/destruction is inline to preserve standalone packet linking: their existing source lists do not include the engine helper. Following review, the default slot constructor is noexcept/defaulted and leaves the private scope empty; ordinary SubProject construction adds no scope allocation/failure point. A bounded source/test audit found26 smart-pointer SubProject constructions, all default, and no actual value-copy construction site; the one textual value-initialization match was a comment. No SubProject byte-layout/sizeof contract was introduced.

Twelve Release-active Testkit cases cover actual Project-owned slot Unknown; no scope on ordinary/default/factory SubProjects; explicit null/no-delete/no-clear and retry; later first observation after an unavailable read; nonnull opaque guard/retry; blocked nonnull cannot become null/replacement; idempotent receipt retention/conflict; detached/foreign slot refusal; expired scope with deterministic same-address SubProject reuse; progress binding before its first attempt still detecting replacement; preservation of ownDb/flow/Project/slot storage; and qualification/source/zero-token rejection without scope allocation. No Report document/fake destructor callback is constructed to bypass the nonnull guard.

Static preflight passed: quoted includes resolve, Testkit main and supported CHECK/REQUIRE/TEST_CASE macros are present, no C assert/NDEBUG checks or trailing whitespace exist, and `git diff --check` returned zero. Comment/string-stripped helper inspection finds no document allocation/delete, reset, changeGuard/cbc920 or QObject-ledger operation. Default metadata constructor/destructor are inline. No build/CTest/test executable ran; root owns validation.

### Allocation review repair

The first version eagerly allocated a shared scope for every SubProject. That added a failure point to unrelated construction and was repaired before integration. The scope is now allocated only at the first valid explicit observation or diagnostic progress binding. Binding captures lifetime immediately, even before an attempt, so old progress cannot silently attach to a replacement at the same address. Invalid source/qualification/zero-token observations leave the scope absent. Allocation failure during explicit diagnostic use leaves ordinary slot value knowledge unchanged; it does not assert native behavior.

`hasDiagnosticScope()` reports only this metadata allocation state. Tests check the actual normal factory/default members keep it false, plus a nothrow-default-construction static assertion. They do not assert that the entire preexisting SubProject has zero allocations: its unrelated containers may already allocate. Unknown remains unavailable after scope creation; scope presence is not native null/liveness/readiness. Model.hpp and all public/cleanup behavior were unchanged by this repair.

```text
cmake --build <combined-build-directory> --config Release --target m2m_mdt_report_document_slot
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.report_document_slot$ --output-on-failure
```

Only these four source/test files and this report changed. Ownership is released and code held frozen:

| File relative to source/apps/moproj2mst | SHA-256 |
| --- | --- |
| include/m2m/model.hpp | `452b668137811aaecca378a8cef78cbbc2019137ee279de1a5a6925d1e502806` |
| include/m2m/engine/report_document_slot.hpp | `3deb74db8676602d9e242a7d6cb8c679efd0be8dd8a14fb301f855b014f8832d` |
| src/engine/report_document_slot.cpp | `1f004f5f797fc0abeed34b0a90a0a58059c7763e9ea61576eaa0a7c4fb7251b4` |
| tests/mdt/test_report_document_slot.cpp | `1d25149f4d85cf6c09e145d055a73452dba1c2460eee4c1ebb1c4d153cfdf63a` |

Root's source-stable build/full validation/archive and native lifecycle acceptance remain pending. Known-null helper return is the only executable conditional cleanup leaf added; document destruction, post-delete QPointer clear and accepted-Close ordering remain guarded.
