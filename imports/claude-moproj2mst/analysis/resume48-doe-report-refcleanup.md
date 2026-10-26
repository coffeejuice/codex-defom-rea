# Checkpoint 48: DOE Report refcount cleanup and source-only owner boundary

## Scope

Static-only analysis of the retained original QtCore PE assembly and existing contracts. No executable was started, no REA database was changed, and no source was edited. The question is whether the constructor's shared Qt empty-container effects, TLS-selected thread reference, and process QObject count have enough evidence for a source-only bounded owner primitive before a runtime capture.

## Constructor effects and matching cleanup evidence

The Report public base call is fixed by `analysis/wave2-doe-task-public-qobject-ownership-contract.json` to caller VA `0x141403222` and public QObject constructor `QtCore+0x001313d0`. The private initializer disassembly `analysis/wave2-doe-task-qobject-private-initializer.asm` shows four shared empty-container increments in this exact order:

1. `QtCore+0x00131190`: `LOCK ADD DWORD[QtCore+0x002f0a50],1` (QListData), private `+0x18` points to this address.
2. `QtCore+0x001311ad`: `LOCK ADD DWORD[QtCore+0x002f0b30],1` (QString), private `+0x30` points to this address.
3. `QtCore+0x001311d2`: `LOCK ADD DWORD[QtCore+0x002f0bb8],1` (QVectorData), private `+0x68` points to this address.
4. `QtCore+0x001311de`: second `LOCK ADD DWORD[QtCore+0x002f0a50],1` (QListData), private `+0x70` points to the same global.

The additions are atomic DWORD operations, so an implementation that records just `+2/+1/+1` deltas can preserve the static constructor effect ordering and 32-bit modular arithmetic. The contract `analysis/wave3-editor-shared-qobject-counter-contract.json` independently establishes the process QObject counter `QtCore+0x002f4344` is process-global and distinct from widget counters/thread notify depth. It does not cover the three container globals.

The retained cleanup candidate `analysis/wave3-boolean-timer-qobject-destructor.asm` is QtCore `0x6712f870` through `0x6712fdbe`. At `+0x0012fd59` it `LOCK XADD`s `r14d` with process QObject global `[QtCore+0x002f4344]`; at `+0x0012fd62` it adjusts the returned old value to test zero. On zero, `+0x0012fd6b` exchanges the process cleanup pointer `[QtCore+0x002f4348]` with null and conditionally calls `QtCore+0x00026d40` then frees at `QtCore+0x00178956`. It later detaches from a parent at `+0x0012fd94` when private `+0x10` is nonnull, and the destructor epilogue invokes a function through the object's private pointer vtable at `+0x0012fda3..+0x0012fda9`. These are not the constructor's shared-container decrements.

In the retained destructor body, no direct references to container globals `+0x002f0a50`, `+0x002f0b30`, or `+0x002f0bb8` occur, and no direct reads from private offsets `+0x18`, `+0x30`, `+0x68`, or `+0x70` occur. The available dump therefore does not establish the exact member-destruction/decrement path for those references. The indirect private vtable call is not enough to infer which destructor it invokes or whether/how it releases each shared ref. A second retained destructor in this collection is the Boolean timer QObject destructor, not proof of the Report path's common-base member cleanup. Do not invent a matching decrement routine or treat every owner count as a simple reversible scalar.

For `QThreadData`, the constructor path is bounded: `currentThreadData` at `QtCore+0x0012aca0` uses TLS global `+0x002f08e8`, `TlsGetValue` at `+0x0012acbc`, then on a nonnull result stores the returned pointer at private `+0x40` (`QtCore+0x0013144e`) and calls ref helper `QtCore+0x00027630` from `+0x00131453`; helper is `LOCK ADD DWORD[RCX],1`. The available destructor excerpt does not establish the corresponding `QThreadData` ref decrement or thread-data zero-count cleanup for this Report object. These remain unknown, separately from the process QObject decrement.

## Can a source-only bounded owner primitive be implemented now?

**A non-executing ledger/type seam can be implemented from source evidence; an executable native-effect primitive cannot yet be qualified.** The safe source-only slice is an owner representation in the existing root `WidgetRuntimeState` for three named shared-global identities and retained exact signed/unsigned modular deltas, with explicit unobserved base values and per-constructor once-only stage markers. It can represent the ordered constructor effects `QList +1`, `QString +1`, `QVector +1`, `QList +1` without claiming absolute counts or performing decrements. This is useful structural scaffolding only: the globals' actual runtime values, alias identity, and effect attribution on an active DOE constructor are unknown. The stage state must be attached to the already-retained stable `PendingTaskPrototype` allocation, not a new SessionTask/prototype or factory-local count.

The source-only slice must not:

- advance/clear the existing M2M unsupported guard or claim a completed QObject constructor;
- mutate a simulated count as if it were an observed process-global count, infer zero bases, or run destructor decrement/zero branches;
- commit the process QObject counter early (its native increment is only at `+0x00131488`, after thread-reference acquisition);
- synthesize TLS/current-thread identity or QThreadData refcount from notify-depth/OS TID;
- implement cleanup for the container refs or QThreadData ref until their exact destructor paths and zero-count consequences are source-qualified and the supported baseline/cleanup state is known.

The strongest safe decision before capture is therefore: source types and dormant ledger fields may be added only if the root owner API is available and the implementation leaves them inert behind the existing guard; no QObject constructor side effect is activated. If the task requires executable behavior or proof of a source-only owner primitive matching native object lifetime, static evidence is insufficient. A future capture should first establish the same-frame constructor deltas and nonnull TLS/ref identity; separate static work must qualify matching shared-container and QThreadData destructor release paths before any owner teardown is simulated. The previous static prerequisite contract (`analysis/wave3-doe-report-qobject-prerequisite-contract.json`) remains controlling: no DOE subsystem-local state changes safely execute QObject/private/thread/counter effects on their own.

## Evidence limits

This review used the retained constructor/private initializer, current-thread/TLS/ref helper assembly, the retained Qt QObject destructor excerpt, and the Report prerequisite/shared-counter contracts. It did not perform a fresh disassembly or runtime observation. The absence of direct cleanup instructions in that retained destructor excerpt is only a limit of the reviewed artifact; it is not proof that Qt never releases those references elsewhere.
