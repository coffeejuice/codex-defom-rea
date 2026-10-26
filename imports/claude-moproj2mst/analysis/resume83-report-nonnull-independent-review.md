# CP83 — independent Report nonnull destructor-chain review

Scope: immutable original PE and retained static receipts only. No GUI, mutable REA database, source, build, test, or project mutation was performed.

## Identity and tool receipt

- Original binary: `evidence/original-binaries/DEF_GUI_MO_64.exe`
- SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
- Bounded PE inspection used DUMPBIN 14.44.35207.0 at `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
- Import-slot names below are from the PE's DUMPBIN `/imports` listing; instruction bytes and destinations are independently visible in bounded `/disasm` output. The PE hash pins those observations.

## What is independently supported

### `SfSessionManager` vtable slot and wrapper

`evidence/gui_mo/s1/vtables.tsv` identifies `SfSessionManager +0x18 -> 0x14064f060`. Bounded disassembly at `0x14064f060..0x14064f095` establishes the deleting wrapper:

1. Preserve `edx` (the deleting flag) and call `0x1406432f0` on the object.
2. If `flag & 1`, call `0x1414dfc68` on the same object (allocation release).
3. Return the object address.

`0x1406432f0` binds the class vftable and calls the non-deleting cleanup body `0x140643400`; so the vtable entry and the cleanup body are connected by direct code, independent of RTTI naming.

The task-provided `COL 0x1430e6020` **does** identify `SfSessionManager`. My first read used the `.rdata` raw adjustment for the pointed TypeDescriptor, but its RVA `0x378cb40` belongs to `.data`; applying `.data`'s raw mapping yields raw offset `0x378ab40`, with the decorated name `.?AVSfSessionManager@@` at raw `0x378ab50` (TypeDescriptor +16). The COL itself is at `.rdata` raw `0x30e4820`; its six DWORDs are `1, 0, 0, 0x0378cb40, 0x030e6048, 0x030e6020`. This agrees with the constructor-installed table `0x1424e0098` and the independent `vtables.tsv` row.

### `0x140643400` cleanup order

The bounded body (`0x140643400..0x140643503`) and its caller (`0x1406432f0`) establish this order:

1. Invoke a call through IAT slot `0x141e058e8`, with `this+0x18` in `rcx` and the pointer loaded from `0x141e04190` in `rdx`.
2. For `this+0x20`, call getter/helper `0x140c1aef0`; when nonnull, invoke the object's deleting virtual `+0x18` with flag 1; then call `0x140cbc920(this+0x20,null)` to clear the guard.
3. For `this+0x30`, repeat the getter and conditional deleting virtual; then write null to raw field `this+0x30`.
4. Call `0x140646e90`, which iterates the list at `this+0x40`; each nonnull retrieved record receives `0x1406589a0` and then its deleting virtual `+0x18` with flag 1, until list removal/test terminates.
5. Call `0x14064be50` for the `+0x48` guarded DOM member.

Thus `+0x48` cleanup is **after** the optional `+0x20/+0x30` objects and the list-backed record cleanup. The same address sequence is consistent with `analysis/wave2-doe-all-session-record-ownership-implementation.json`, which says the constructor initializes `+0x20/+0x30` null, then `643400` handles these slots, `646e90` removes/deletes `SfPartInfo`, and `64be50` cleans the DOM member. Populated UI-member ownership and the object meanings at `+0x20/+0x30` remain separately unqualified.

### Exact imported providers and an important mismatch

The import table identifies the exact providers when original-thunk index is matched to the same zero-based IAT index. The prior sequential listing-only interpretation in this review was incorrect:

- IAT `0x141e0c6b0`: `QtXml4_64.dll`, `QDomElement::~QDomElement()` (`??1QDomElement@@QEAA@XZ`). At `0x140643325`, `0x140643335`, and `0x140643345`, it destroys the document's `+0x60`, `+0x58`, and `+0x50` QDomElement handles, in that order. This is not the Report document object's deleting virtual.
- IAT `0x141e04190`: `QtCore4_64.dll`, `QString::null` (`?null@QString@@2UNull@1@B`).
- IAT `0x141e058e8`: `QtCore4_64.dll`, `QString::operator=(QString::Null const&)` (`??4QString@@QEAAAEAV0@AEBUNull@0@@Z`). In `0x140643400`, it assigns null to the document's `+0x18` QString before the payload null checks.
- IAT `0x141e059e0`: `QtCore4_64.dll`, `QString::~QString()` (`??1QString@@QEAA@XZ`), called on `this+0x18` at `0x140643382`.
- IAT `0x141e042a8`: `QtCore4_64.dll`, `QMetaObject::removeGuard(QObject**)` (`?removeGuard@QMetaObject@@SAXPEAPEAVQObject@@@Z`), used by `0x140643379` on the internal guard at `+0x20`.
- IAT `0x141e042b0`: `QtCore4_64.dll`, `QObject::~QObject()` (`??1QObject@@UEAA@XZ`), called at `0x14064338e`. This is the Report object's QObject destruction boundary that can invalidate registered external guards; the exact registration/liveness for a particular SubProject cell remains unproven.

The exact thunk-indexed providers now support the QString assignment/destructor, QDomElement member destructors, internal `removeGuard`, and QObject destructor distinctions. The imported provider correction does not discharge the underlying runtime guard-registration or transitive destruction obligations.

## Report document and owner order

The independently retained Report cleanup evidence in `analysis/resume81-report-close-pe.md` establishes `SubProject+0x30` is a QPointer, helper `0x1400b5bb0` checks its current target, calls its deleting virtual `+0x18` with flag 1 when nonnull, then calls `0x140cbc920` on the guard cell. The constructor/RTTI binding in `analysis/resume83-report-nonnull-next.md` identifies that deleting target as the `SfSessionManager` Report document for this profile. `SubProject` cleanup calls that helper before deleting the raw model and destroying remaining fields. Project cleanup calls its SubProject-list destructor before Project member teardown. CP82's manager-tail contract places Project deletion before the manager's five Mo slots and before manager list/current reset for the selected static child path.

Keep distinct the Report document object, its `+0x20/+0x30` payload fields, its internal `+0x20/+0x38` guard members, its `+0x50/+0x58/+0x60` QDomElement members, and external `SubProject+0x30` QPointer. The same class name `SfSessionManager` does not make those object fields interchangeable.

## Accepted-Close guard remains

The recovered native sequence is a **conditional static bridge** from the manager's virtual `+0x18` deleting dispatch for an encountered `SfMoModel` into Mo cleanup, Project deletion, Project's SubProject-list cleanup, and the Report QPointer cleanup. It is not proof that an accepted Close reaches this nonempty path for a live child. It does not bind the can-close/modal/refresh guards, a concrete child/vtable at that manager encounter, or same-instance ordering against main-window QObject destruction.

Therefore retain the root guard from `analysis/resume82-report-destructor-plan.md`: require a source-qualified accepted-Close continuation that reaches the real nonempty manager/Project/SubProject chain and binds the actual Report document/main-window lifetimes. Do not treat Open-failure cleanup, a virtual-capable static path, default C++ destruction, or the separate `SfSessionManager` destructor as accepted normal-Close proof.

## Next verification

The COL and QtCore/QtXml import mappings are resolved by section-aware RTTI and zero-based original-thunk/IAT indexing as recorded in `analysis/resume83-report-nonnull-next.md`; this amendment supersedes the initial misread above. Remaining work is to bind the accepted-Close root and same-instance Report-document/main-window parent lifetime ordering. Until those are closed, the nonnull Report deletion adapter should remain guarded.
