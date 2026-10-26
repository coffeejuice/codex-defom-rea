# CP82: codec admission and source QString owner binding

The smallest sound increment is a process-scoped C-string codec state plus a source-only fresh-resource constructor entry into the existing `QStringPathOwner`. The source constructor can establish refcount 1 itself on its proven null-codec branch. It must share that one canonical record with later aliases; copying a count into a second owner would lose the native ownership relationship. Unknown and non-null codec states remain refusal boundaries before decoder invocation or resource allocation.

This is a read-only implementation plan. No source was edited while CP81 acceptance is pending. The file ownership below is a proposal for root to assign, not an instruction to modify shared files now.

## Qualified native transition

The frozen evidence is `analysis/resume81-doe-qstring-source-owner-evidence.md`, SHA-256 `201734ffb656a8f5b39c73cd12f7aab3537bd865a032b114cc92bb8a2a78ac60`, and its `resume81-doe-qstring-source-freeze.json` manifest. Original QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Source edge | Evidence and resulting contract |
| --- | --- |
| GUI `0x140086d68`, import `0x141e04480` | QString(char const*) receives the fixed 36-byte resource at `0x14213afd8`, destination `rsp+0x58`. A value-only icon request does not prove this source occurrence. |
| QtCore `0x67174bd0` / `0x67074120` | Constructor invokes fromAscii with length -1. The first helper branch reads C-string codec at RVA `0x2f3e10`. Successful helper return is stored in the source slot at `0x67174be7`, then the object pointer is returned. |
| Null codec -> `0x6706bf40`, `0x6706bfa0` | The nonempty literal gets a 104-byte allocation. Data ref is initialized to 1, length/allocation fields to 36, UTF-16 payload pointer to data+0x1a, and trailing UTF-16 NUL is written. The low five flag bits are cleared; other flag bits/padding are not proven zero. |
| Allocation failure `0x6706bfbf` -> `0x6701bfb0` | Throw occurs before slot publication. No completed QString or data receipt may escape. The selected allocator request is distinct from adapter metadata allocations. |
| Non-null codec `0x6707419f` -> `0x67072460` | Actual codec virtual+0x18 returns a QString whose data is retained and whose temporary is released. The concrete data/ref owner remains unqualified. Do not approximate it with host locale conversion or the Latin-1 branch. |
| GUI `0x1400026ee` -> QtCore `0x67177590` | The source setter writes the process global. Language setup can select Shift-JIS, Big5, KOI8-R, EUC-KR or GBK through codecForName. Image zero-fill alone does not establish invocation-time null. |
| GUI `0x140086da1` / unwind state 1 | The completed temporary releases exactly one data ref at the reached destructor phase. Adapter scope teardown is not that source event. |

For this resource the successful null-codec input does not alias shared-null. The later QFileInfo shared-null increment at RVA `0x2f0b30` still needs an independent current-count owner; the input's known initial 1 cannot supply it.

## One process state, explicit knowledge

Propose one explicitly caller-owned `QStringResourceProcessScope` for the selected reconstructed fresh-process boundary. It privately owns the canonical `QStringPathOwner`, codec state and generation/epoch. Sessions borrow the same context; a new Session or Project must not create a new process-global codec value or null counter. A host function-static singleton must not merge separate reconstructed fresh-process profiles.

Codec state has three explicit alternatives: `Unknown`, `Null`, and `NonNull`. Default is **Unknown**. Null is a known native pointer value, not absence of an observation. NonNull may retain an opaque codec owner identity for diagnostics but cannot invoke it through a fabricated host pointer. The immutable symbol identity is exact module SHA plus codec RVA `0x2f3e10`; it is distinct from the QString shared-null symbol.

Production state may become known only at a source-qualified read/write edge: an actual reconstructed codec setter updates the same process state; a qualified invocation read can supply the current state. A future module-load initializer can establish null only if the selected execution models initialization and all intervening writers. The PE zero-filled tail, a chosen language, omission of a setter in one module, or a free-form qualification string is insufficient. Unknown activity that can change this global invalidates its knownness. No current candidate startup path supplies that closed state, so production default remains guarded.

Admission captures the selected branch at the helper's actual codec read, with the process/codec epoch validated at that instant. Do not reread/reject merely because the codec changes after branch selection: the native null-codec path does not reread it during allocation or copying. A pre-created admission that has not yet reached its modeled read must be rejected if stale. These two times must not be conflated.

## Source-only constructor admission

Proposed type names and interface, to settle with root before implementation:

```cpp
enum class QStringCStringCodecKnowledge { Unknown, Null, NonNull };
enum class QStringResourceNextBoundary { CodecState, CodecDecoder, AddFile };
class QStringResourceProcessScope;  // private canonical QStringPathOwner
class QStringResourceCtorAdmission; // sealed, single source occurrence

// New entry on the existing owner, invoked only by the owning process scope.
// The admission is consumed; destination must be an unconstructed source slot.
QStringPathData QStringPathOwner::constructResourceCString(
    QStringResourceCtorAdmission&&, QStringPathLease& destination);
```

The admission is privately constructed at the actual receiver's `0x140086d68` continuation. It binds process identity, actual Session/live Project, ModelManager/notifier/model witness generation, source call occurrence and a logical source-slot generation. The logical slot models the native stack temporary; it is not the original process's physical address. Every actual delivered model may reach the call; active SUB identity alone is insufficient. A diagnostic caller passing `DoeIconAddFileRequest::resource`, source address constants or refcount 1 cannot mint this admission.

For the first implementation, a clearly labeled test profile may exercise the isolated transition. It must have no conversion into the production sealed admission. Until the real receiver and process-state producer are assigned and wired, this remains a source-qualified internal capability with no public delivery claim.

At entry, validate process/same-Project/source occurrence and empty destination, then perform the source codec read. Unknown returns/refuses at `CodecState`; NonNull at `CodecDecoder`. Both leave the destination, canonical input records and native allocation request history unchanged. Refusal may retain the reached diagnostic stage, but not a fabricated data owner.

For Null, the helper performs the qualified fixed-resource branch. The allocator decision must be an actual modeled 104-byte source request with a selected success/failure result, not an unrelated `make_unique` succeeding earlier. On success, create one canonical fresh-data generation with source-initialized refcount 1 and the qualified text/fields; publish it to the actual destination slot only after the data is initialized, then publish constructor return. Adoption does not increment the new count. Repeated calls allocate distinct generations even for identical text or reused host addresses.

The current `observeInput(text, profile)` remains diagnostic. A new source entry must not implement production admission merely by calling that public profile method with a persuasive string. It should share the existing owner's private record creation mechanics while marking provenance separately and retaining one canonical counter. `QStringPathData` observation copies and constructor-result object-pointer spills do not increment it. Later `acquirePathAlias`, `copyAlias` and explicit `release` use this same record/cell.

## Representation and partial failure

The minimum owner representation is a typed reconstructed data record with the known initialized fields, unique allocation generation and its single ref cell. Record the native allocation request size separately from the host representation size; do not label `sizeof(Record)` as native 104-byte storage. If an implementation instead models the raw native block, it must preserve unknown flag bits/padding and safe typed access; a zero-filled `std::array` cannot claim to reproduce the native mask on previously uninitialized malloc memory.

Adapter metadata reservations should precede the modeled native allocator call where possible. They are host bookkeeping, not extra native malloc events. Any host bookkeeping failure after modeled allocation must retain an explicit incomplete allocation observation rather than publish a completed source slot or silently claim native rollback. This is a review point for the concrete allocator interface before implementation; no general allocator simulator is needed.

At source allocation failure, no record/lease/constructor-return observation is published. At a later guard, the completed source slot remains live. Its eventual reached destructor consumes one lease; current `QStringPathOwner::release` already records zero, marks data deallocation pending and guards before unqualified Qt data-free. Do not run that release automatically from C++ lease destruction. Adapter destruction may reclaim host storage without claiming a reached native destructor/free event. The QIcon shell has separate retained ownership and must not be deleted as a side effect of these QString guards.

After success, the next integration boundary is addFile's actual continuation. This helper does not combine QFileInfo's ref0 prefix, resolve custom-handler population, choose a default resource engine, assign a graphic icon or deliver another receiver. The owner may support already-qualified alias arithmetic internally, but success here is not ReadyQFileInfo or public Type2 success.

## Source ownership proposal

Assign one implementation lane the following files together, avoiding two owners editing the same counter implementation:

- Existing `source/apps/moproj2mst/include/m2m/doe_qstring_path_owner.hpp` and `src/doe/qstring_path_owner.cpp`: source provenance and private fresh-data creation/publication using the existing canonical ref cell; keep current diagnostic API compatible.
- New `include/m2m/doe_qstring_resource_owner.hpp` and `src/doe/qstring_resource_owner.cpp`: one explicit process codec state, sealed branch/occurrence admission and fixed resource constructor adapter; no static global and no Session default context creation.
- New `tests/doe/test_qstring_resource_owner.cpp`: focused isolated transition tests with explicit test provenance. Expected discovery target `m2m_doe_qstring_resource_owner`, CTest `m2m.doe.qstring_resource_owner`; root verifies discovery and builds.

Root retains exclusive shared ownership of `engine/conversion_session.hpp/.cpp` and the eventual actual flow-model receiver delivery source. Root would supply the external process context, store a revocable same-Project binding, and mint/consume source admissions only at the qualified receiver edge. That integration is a later assigned slice; this plan does not assume it already exists. The existing `qt4.hpp` value-string API, `variable_notify`, public writer, C ABI and current QFileInfo helper need no edits for the isolated constructor slice.

## Focused tests and refusal criteria

1. Default Unknown: precise CodecState refusal, zero native allocation requests, no input record or destination mutation. A null optional observation is not converted to known Null.
2. Known NonNull: precise CodecDecoder refusal before allocation/virtual dispatch; changing only diagnostic text to the fixed resource cannot bypass it.
3. Explicit isolated Null profile: one request for 104 bytes, one new input generation, 36 UTF-16 code units plus trailing NUL, source refcount 1 and active source slot. Copied observation handles and returned-slot spills leave refcount 1.
4. Source allocator failure: request recorded, no completed data/slot/return publication. Host allocation failure must remain distinguishable from the modeled native failure.
5. Two successful occurrences: equal text but distinct data generations. Duplicate admission consumption or reuse of a constructed destination refuses without additional allocation/ref change.
6. Codec epoch: stale pre-read admission refuses; a branch already selected Null is not retroactively changed by a later setter. A future invocation observes the updated NonNull/Unknown state and guards.
7. Canonical alias/ref path: adopting constructor result adds no ref; no-backslash path alias yields the same data/ref 2; alias release restores 1; source release consumes the final lease and reaches the existing data-free guard at 0. No second atomic profile is created.
8. Process/scope lifetime: foreign context, revoked Session/live Project, wrong receiver generation and expired owner refuse before dereference/allocation. Project release does not reset codec or process shared-null identity. No process/Session/Project cycle is introduced.
9. Unresolved shared-null: after constructor success, shared-null stays absent or unknown exactly as before. Its later increment still refuses; no counter 1 is copied from the fresh input.
10. Public guard: no new production constructor call is reachable from diagnostic test profiles; QIcon addFile/engine/graphic transfer and Type2 Save remain guarded.

Tests should use Testkit's active checks and a main entry, with explicit test-profile labels. Root runs focused tests/builds after source freeze. No new static capture, build, CTest, GUI or database operation was required for this plan.
