# CP461 ShapeOptim scalar initializer: ordered stores and list resets

The stable private value projection of `14013f5a0` can be source-closed. Its normal result includes three ordered list clears, not only scalar defaults. Preserve the existing linked-property and current-operation identities; no selected helper directly clears OB+20, OB+b0 or OB+b8. Native Qt ownership, invalid-memory failures and exceptional cleanup remain outside that prepared value contract.

## Evidence

The new original-EXE exports cover 19 complete PE exception-function ranges, 1,766 bytes and 421 addressed instructions with zero mismatches, bound to `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The five selected original QtCore exports cover 256 bytes and 76 instructions with zero mismatches, bound to `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The Qt receipt also binds the imported shared-null data and five export names. `cp461-shapeoptim-init-verify.py` and `cp461-shapeoptim-init-qt-verify.py` reproduce these bounded exports and byte comparisons.

CP460 already binds the complete parent initializer `14013f5a0` and its two calls from concrete selected wrapper `14013beb0`. Its evidence and CP454/CP458 qualifications remain in force. Retained decompiles in L5/v4.c, L7/d7.c and out_p07/lists.c guide names and body selection; PE bytes establish the new claims. No original was executed, GUI control acquired, mutable native REA opened, source/build/deployment/plan/state changed. Only new `analysis/cp461-shapeoptim-init-*` artifacts were written.

## Exact normal effect order

The parent first writes OB+6c=1, +c4=1, +c0=1022, +c8=0, +cc=200, double +e0=0.001, bytes +e8=0 and +e9=0, then +d0=1. Its helper `1405e0d50(OB+60)` then performs these additional stores in this order:

1. double positive zero to OB+88 and OB+90;
2. byte zero separately to OB+98,+99,+9a,+9b,+9c,+9d;
3. int 1 separately to OB+70,+74,+78;
4. int 0 separately to OB+7c,+80,+84;
5. reset the list object at OB+a0 through `1400424b0 -> 140042500`;
6. reset the list object at OB+a8 through `1405e0e40 -> 1405e0e90`.

Only after that helper returns does the parent call `1400aced0(OB+f0)`, which resets the list object at OB+f0 through `140088ac0`. Both nested list clears finish before the +f0 reset starts. Parent return follows temporary-list destruction. The three reset wrappers each construct a temporary with `14018f9e0`, assign it, then destroy the temporary.

`14018f9e0` loads the address from imported `QListData::shared_null`, stores that pointer in the temporary and calls `QBasicAtomicInt::ref`. In the imported original QtCore PE, shared_null at 672f0a50 initially contains dwords `(1,0,0,0,1,0)`: refcount=1, allocation=0, begin=end=0 and the low sharable flag at +10 is set. Thus the stable normal assignment yields an empty list backed by shared_null and skips its nonsharable detach branch. The receipt proves initial module data, not the runtime refcount or which DLL a process loaded.

All three assignment bodies use the same order: compare old and incoming backing pointers; if equal skip assignment work; otherwise ref incoming, deref old, release old if deref returned false, publish incoming into the destination qword, then test incoming+10 bit 0 and detach only if clear. Pointer publication sites are 140042569 (OB+a0), 1405e0ef9 (OB+a8), and 140088b29 (OB+f0). The temporary destructor always follows a normally returning assignment. The atomic ref/deref exports are respectively locked add +1/-1 followed by setne; ref's bool is ignored. Deref=false is the final-owner branch, not an error return.

The OB+a0/+a8 old-storage release goes through `140025650`: node destroy `140025840` takes a constant no-op path, then qFree releases backing storage. It does not delete pointer-valued elements. The OB+f0 old-storage release goes through `140004e90`: `140005240` traverses from end toward begin, eight bytes per inline slot, and calls `140002f80(slot,0)`; that calls imported QString destructor and does not call object delete because EDX=0. qFree of the list backing occurs only after every visited QString destructor returns. These facts do not require naming the +a0/+a8 element types.

Qt QString destructor `67020c70` atomically decrements the QString backing-data refcount. On zero it calls `670700d0`; when data+18 bit 3 is clear, that helper reaches qFree directly. If bit 3 is set, it first enters additional data-cache helpers, whose transitive behavior is deliberately not expanded. qFree `670203b0` is a tail jump to imported MSVCR90 `free`. No selected normal shareable-list reset path directly invokes QObject notifications, changeGuard or a virtual application callback. This is narrower than proving that allocator/cache hooks or concurrent writes cannot affect application state.

## Failure and partial effects

No selected reset returns a checked success flag, validates OB/list/backing pointers, or implements rollback. All preceding completed parent/helper scalar stores remain ordered before the first list reset. A fault or nonreturn during OB+a0 reset prevents its normal completion, the OB+a8 reset, OB+f0 reset and parent return. An OB+a8 failure follows a completed +a0 reset. A +f0 failure follows both earlier list resets. The second `14013f5a0` invocation in CP460 remains separated from the first by external engine work, whose effects are not closed here.

Within a differing-backing assignment, incoming ref occurs before old deref; old backing release occurs before publication. Failure during old release therefore can leave the destination qword pointing at backing whose refcount already reached zero or whose elements/storage have been partly destroyed. With +f0, later-slot QString destructions precede earlier slots, and a later failure does not undo those completed destructions. If publication has completed but a detach or subsequent temporary cleanup fails, the destination pointer may already be shared_null (or a subsequent detached backing); do not model such failure as an unchanged destination. There is no native allocation-null success/failure branch in these selected wrappers.

The shared-null flag being initially sharable makes detach unreachable under the stated stable-module profile. The selected detach entry wrappers are retained to identify the boundary, but transitive detach/copy/allocation paths are not closed for a mutated shared-null flag. QString data-cache helpers, MSVCR90 free, asynchronous access, malformed begin/end, memory faults, SEH/C++ unwind and exceptional temporary cleanup likewise remain explicit unknowns. This is an ordered normal-path/failure-prefix contract, not a claim that standard-container exceptions reproduce native behavior.

## Prepared integration boundary

A private helper may take valid prepared OB scalar values and three ordered list-value destinations, prohibit callbacks/concurrent mutation, and apply the parent stores, helper stores, then `clear(a0)`, `clear(a8)`, `clear(f0)` in that exact order. It must preserve unmentioned fields and all independent OB/OPI/property identities. It can expose ordered effect events and inject a refusal at a selected list boundary while retaining preceding effects. Such a refusal is a safe private model event, not a discovered native return value or full Qt failure simulation.

For the full selected wrapper, invoke this initializer twice around an explicit external engine effect; re-read values at actual native effect points if aliasing or mutable callbacks are ever admitted. The source evidence closes that stable private value projection now. Exact heap/refcount/lifetime behavior requires the native backing graph and remaining Qt/cache/CRT dependencies; arbitrary imported Open and public Type3 admission remain governed by CP460's identity and lifecycle gates.
