# CP458 ShapeOptim linked property: concrete creation and operation disposal

The original has a concrete linked-property creation path; the remaining gate is the identity history at the consumer occurrence. The linked property B is constructed with the creating ShapeOptim class OB at B+40 and an independently allocated data block at B+38. Generic operation creation installs that exact factory return in OPI+10. The current-operation property P and the linked B are therefore conditionally the same object, but equality is not established for an arbitrary later Type3 load. Public Type3 admission remains guarded.

## Evidence and qualifications

`cp458-type3-linked-byte-verification.json` binds 35 bounded dumpbin exports, 1,167 addressed instructions and 6,005 bytes, with zero mismatches, to imported original `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `cp458-type3-linked-verify.py` reproduces the exports and compares every addressed byte using an independent PE section mapping. Nonleaf ranges use PE exception-function bounds; six explicit leaf bounds end at RET. Every export ends at RET. The receipt separately binds called IAT symbols, RTTI descriptors and selected vtable slots.

CP454's tail contract and its static-only/Qt/fault qualifications still apply. Retained decompiles identify useful paths but are not sufficient alone: their qualified hypotheses are checked against the original PE here. `evidence/gui_mo/strings.txt` uses raw file offsets; those apparent addresses must not be read as image RVAs. The PE-bound ShapeOptim property descriptor really is `0x1435b4c20`, whose name is `.?AVSfOprShapeOptimProperties@@`; the class descriptor is `0x1435b4bc8`.

No original executable ran, no GUI or app-control session was acquired, no REA/Ghidra project was opened, and no source/test/build/plan/state was changed. This is original static evidence, not an observed lifecycle occurrence. Qt DLL bodies and arbitrary callback/reentrant effects were not inspected.

## Exact creation, data and class source

1. Constructor `14013a9a0` sets primary vtable `1421b4d98`. Its PE-bound slots are +98=`14013be10`, +78=`14013d740`, +188=`14013af20`. It zero-initializes the guarded-pointer storage at OB+b0 and OB+b8, then calls reset `14013ab00`; reset passes those two addresses and zero to `140cbc920`.
2. Factory `14013be10` allocates 0x48 bytes. On a nonnull allocation it calls OB vt+188 and passes that result in RDX and the same OB in R8 to `140142620(B, capabilityPointer, OB)` at `14013be59`. The concrete +188 helper returns the virtual-base pointer computed from OB+30 and the signed offset at its vbtable+4. It does not fetch current-operation properties or B. Property construction uses that capability pointer's vt+58 to calculate a local flag; it does not assign it as B's data or source class.
3. `140142620` calls base property construction `1405f8bb0`, installs primary vtable `1421bfcd8` and secondary vtable `1421bfca0`, then stores the exact incoming creator OB at B+40 (`14014269f`). B+40 is a raw store, not a QPointer or retained-reference operation. This resolves its construction-time source identity. It does not prove validity after OB destruction.
4. Initializer `140142720` allocates 0x18 bytes, calls `140142d90`, and stores the returned pointer at B+38 (`140142777`). Data construction zeroes its leading qword, writes int 200 at +8, and writes double 0.001 at +10 (bound constant `1421b5050`, raw `fca9f1d24d62503f`). It then runs property registration `140142800` and secondary tree builder vt+20=`140142b00`. This records the explicit allocation/store path; transitive Qt callbacks and allocation/unwind behavior are not closed.
5. Factory `14013be10` passes `(OB+b0, B)` to `140cbc920` at `14013be89`, then calls `140c1aef0(OB+b0)` and returns its RAX unchanged. Allocation-null passes zero instead; construction failure does not reach that publication call along the normal path.

`140cbc920` compares the existing first qword with the supplied pointer; if different, it calls bound IAT `141e058b0`, `QMetaObject::changeGuard(QObject**,QObject*)`, with precisely those two arguments. `140cbc900` calls bound IAT `141e042a8`, `QMetaObject::removeGuard(QObject**)`. This establishes the guard API and arguments. The Qt implementation's update/deletion-notification timing is not a newly observed fact. The getter is the same first-qword load CP454 bound; it does not retain B.

## Operation property P versus linked B

Generic operation factory `140046cc0` obtains a class through `140047c00`, calls that class vt+98 at `140047296`, preserves its returned pointer, obtains the operation pointer, and passes both to `1400511f0` at `1400472af`. `1400511f0` stores the exact factory pointer into OPI+10 at `140051208`, then calls `1405f8d60(B,OPI)`. That helper passes `(B+20,OPI)` to `changeGuard` through `140cbc920`; it does not alter B+40.

At the CP454 consumer, current operation C comes from `1405ef2b0(OB)`, which reads guarded OB+20. `140051230(C)` loads C+10. RTTI cast `143579b60 -> 1435b4c20` selects P. Final refresh independently reloads guarded OB+b0 to obtain B and dispatches B vt+58. For the concrete constructed type, that target is PE-bound `140142ca0`, reading B+40 and writing B+38 as CP454 described.

Consequently, if C is the same OPI just assigned by this factory path, its +10 still contains that returned B, the cast succeeds and no intervening creation/reset/destruction or callback changed the relevant pointers, P and B are identical and both access the same +38. Those are explicit preconditions, not facts established for a later load. A subsequent call to the same OB's vt+98 requests a replacement OB+b0 guard; the shown factory never examines or changes the existing current-operation OB+20. Reset also clears +b0 independently. Do not replace those two pointer histories with an unconditional P=B assertion or infer equality just because both dynamic classes are ShapeOptim properties.

## Disposal boundary

The operation's concrete vtable `1421247d8` slot+18 is deleting wrapper `1400652a0`; it calls operation destructor `140050440`, whose first cleanup is `140050690`. That cleanup immediately calls `1400512c0`. If OPI+10 is nonnull, `1400512c0` dispatches that object's vt+18 with EDX=1 at `140051307`, then zeroes OPI+10 at `14005131f`.

For an intact concrete B vtable, vt+18 is deleting wrapper `140142d50`. It calls B destructor `1401426c0` and then `1414dfc68(B)` when flag bit 0 is set. B destructor runs `1401427b0`, base property destructor `1405f8c30`, and ultimately bound QObject destructor IAT `141e042b0`. This proves an ordinary operation-disposal route to deletion of its published concrete property. Qt guard invalidation itself remains inside the uninspected QObject implementation. B's raw +38 and raw +40 must not be described as owning or guarded merely from this route; the selected B destructor calls do not themselves establish the ultimate +38 data allocation's disposal policy.

OB destruction `14013aa60` calls the empty helper `14013ab70`, destroys its QString, calls removeGuard for OB+b8 and OB+b0, then destroys its bases. It contains no visible B deletion or B+40 nulling. Removing the class's guard is not evidence that B or its raw class source remains valid, nor that the property owns the class. No accepted Close or project-wide disposal order is established here.

## Exact next integration dependency

Recover or witness the same-occurrence publication/selection sequence from imported project Open to `14013d740`: identify the concrete OB instance returned by the registry, the OPI selected into OB+20, every intervening vt+98 creation and +b0 reset/replacement, and the resulting pair `(P=C+10, B=OB+b0)` with their B/P+38 and B/P+40 values. The static factory/store contracts above can be reused; repeating their discovery is unnecessary. If a static event trace closes those paths without callback uncertainty, use it; otherwise the original needs a same-instance witness after the GUI-control release gate permits runtime work. Also close actual containing-operation/class destruction order and the +38 data disposal dependency before claiming the full selected Open-Save-Close lifecycle.

The source supports retaining separate current P and linked B identities in private models, with B carrying its creator OB; it does not justify merging them for public integration. Type3 SUB+28 optimizer ownership, .OPT streaming, GEO and Save gates are unchanged.

