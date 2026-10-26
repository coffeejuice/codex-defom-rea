# Checkpoint 47: DOE TLS and QThreadData capture contract

## Purpose

This contract refines the TLS/current-thread portion of `analysis/resume46-doe-report-static.md` from retained disassembly only. It defines the exact runtime observations needed to qualify the existing-TLS, nonnull `QThreadData` fast path for the first DOE Report `QObject(parent=null)`. No process was launched or inspected for this note, so all runtime values remain unknown and the Report guard remains required.

## Address and identity basis

Use the actually loaded `QtCore4_64.dll` base `Bqt` for the capture process. The static preferred image base represented by the retained assembly is `0x67000000`; all listed RVAs below are preferred VA minus that base. Capture the module path, base, image size, hash, PID, and thread ID first; resolve each hook as `Bqt + RVA`. Never treat the example base from probe004 (`0x53b00000`) as this run's base.

The public Report QObject path is caller `DEF_GUI_MO_64.exe+0x01403222`, public constructor `QtCore+0x001313d0`, and current-thread routine `QtCore+0x0012aca0`. For one selected constructor ordinal, the identity chain to retain is:

`Report object RCX` → `QObject private pointer = object+8` / reciprocal `private+8 = object` → `currentThreadData` return pointer → `private+0x40` → ref helper RCX and its pointed-to refcount DWORD.

Every edge must be observed or verified by bounded reads during this same invocation, on the same TID. A pointer observed only in an unrelated startup/editor constructor cannot qualify the DOE frame.

## Static branch map and exact hook inputs

All instruction offsets are relative to `QtCore4_64.dll` load base:

| Routine/event | Offset | Static operation and capture fields |
|---|---:|---|
| `QObject` calls `currentThreadData` | `+0x00131440` call target `+0x0012aca0` | On currentThreadData entry/return retain caller return address, TID, nesting/call ordinal, and returned RAX. The caller stores RAX in private `+0x40` shortly afterward. |
| currentThreadData entry | `+0x0012aca0` | Entry RSP/return address and TID; this identifies the invocation nested under the selected QObject constructor. |
| call TLS initializer | `+0x0012acb1` call target `+0x00129600` | Pair initializer entry/return with this currentThreadData invocation. |
| TLS index read | `+0x0012acb6` | `DWORD[ Bqt+0x002f08e8 ]` is passed as ECX. Record its exact value immediately before the read/call. `0xffffffff` means initializer's allocation path is entered; any other value skips that initializer body. |
| `TlsGetValue` call | `+0x0012acbc` | IAT slot is preferred VA `0x671a80a8`, RVA `+0x001a80a8`. At call entry record ECX index; at return record RAX. This RAX is copied to RSI and tested at `+0x0012acc5`. Nonzero branches directly to `+0x0012ae42`, the fast return. |
| null branch after TLS read | `+0x0012acce` onward | If RAX was zero, do not call it the fast path. This route calls a native helper at `+0x0012acdb`, then either obtains/adopts an existing thread-data object (`+0x0012ace4..+0x0012ad49`) or allocates and initializes a new one (`+0x0012ad4b..+0x0012ae3d`). Record as an unqualified branch; no synthetic `QThreadData` may be substituted. |
| TLS initializer entry | `+0x00129600` | Record TID, entry return address, and TLS index global before execution. At `+0x00129614`, it compares the global with `0xffffffff`; any other value branches to return at `+0x001296c0`. |
| TLS initialization guard | `+0x00129621..+0x0012968c` | On the `0xffffffff` branch, record only observed bounded globals/calls: initialization flag at `+0x002f3d28`, once object at `+0x002f3d20`, and actual result of TlsAlloc call through IAT slot preferred VA `0x671a8378` (RVA `+0x001a8378`). Store at `+0x002f08e8` occurs at `+0x0012968c`. Do not invent an index value or assume the one-time guard branch. |
| TLS initializer return | `+0x001296c0` | Record post-call TLS index global and verify whether it was retained or newly written. Pair this return with the caller's same invocation. |
| selected pointer stored | `+0x0013144e` (confirmed store instruction in retained public-constructor assembly) | Read private `+0x40` after the store and require exact equality to the nonnull TLS return RAX for the selected constructor. Static caller flow stores RAX there before ref helper. Do not place an execution breakpoint at a guessed instruction boundary; use disassembly-confirmed instruction bytes/offset in a future script. |
| thread ref helper call | `+0x00131453` | Record helper target `+0x00027630` and its RCX at helper entry. Require RCX equals captured selected QThreadData pointer. Capture `DWORD[RCX]` immediately before and after the helper; retain exact uint32 values and verify delta modulo 2^32 is `+1`. |
| helper body | `+0x00027630` | `LOCK ADD DWORD [RCX],1; RET` at preferred VA `0x67027630`. Record object pointer and refcount address. The static helper proves the operation, not its runtime input or starting count. |
| QObject counter follows | `+0x00131488` | Distinct process-global increment at `Bqt+0x002f4344`; capture before/after separately and verify it follows the thread-ref helper on this same call. It is not a QThreadData reference count. |

`+0x0013144e` above is a derived candidate continuation, not an address qualified by this note's input excerpt. Since offsets in the contract/capture plan list `+0x00131453` and `+0x00131488` as fixed points, the future operator should use the exact instruction bytes from `wave2-doe-task-public-qobject-constructor.asm` before installing any hook at `+0x0013144e`; if not independently confirmed, use constructor return and a bounded read of private `+0x40` instead.

## Qualification rules by branch

**Fast path accepted** only if all of the following occur in one DOE constructor invocation: TLS initializer returns with a valid index; the index passed to `TlsGetValue` equals the recorded TLS global; `TlsGetValue` returns nonnull pointer `P`; currentThreadData returns that same `P` through its direct nonnull branch; QObject writes `P` to private `+0x40`; helper RCX is `P`; and the pointed-to DWORD shows the helper's exact `+1` effect before the separate QObject counter increment. Capture must establish a same-TID / call-ordinal / object-private association throughout. The TLS index's numeric value and `P` are run-specific observations, not constants from this static evidence.

**Initialized index does not imply initialized thread value.** If the TLS index starts non-`0xffffffff` but `TlsGetValue` returns null, currentThreadData enters the allocation/adoption branch. Preserve the trace and stop normal-prefix qualification; that branch includes TLS set, private state, and possible process-level `+0x2f4230` effects. Do not claim the fast path from index alone.

**Missing index is a distinct case.** If the index starts `0xffffffff`, the TLS initializer can execute its once/allocation path and call TlsAlloc; capture the actual returned index and post-store global. Even if the subsequent TlsGetValue returns nonnull, this run is not evidence for the requested already-initialized TLS-index fast path. It may be useful separate evidence, but the contract requires a different qualification.

**Ambiguous/nested invocation fails attribution.** If a nested QObject constructor, TID change, missing hook, read failure, reentrancy, or capture truncation breaks the pointer/ordinal chain, label the chain inconclusive. Preserve partial events and do not combine values from another constructor or process.

## Bounded passive capture recipe

Within the single serialized Report capture already specified by the prior note, add hooks at `Bqt+0x0012aca0` entry/return, `Bqt+0x00129600` entry/return, the IAT target called from `Bqt+0x0012acbc` for `TlsGetValue` entry/return (using the actual resolved target at runtime), the instruction at `Bqt+0x00131453` before the ref-helper call, helper `Bqt+0x00027630` entry/return, and counter instruction `Bqt+0x00131488`. Also record TLS global `Bqt+0x002f08e8` at initializer/currentThreadData boundaries and, only if the initializer branch is actually taken, the observed TlsAlloc return and store. Keep hooks read-only; no target function invocation, memory writes, API calls, event pumping, or GUI input.

For each event, serialize: process/module identity, TID, monotonic timestamp/order, selected DOE constructor ordinal, routine RVA, entry/return phase, caller return address, object/private/P pointer where established, TLS index, returned pointer, refcount address/value, and read status. Keep `P` as an opaque identity token in the report if pointer disclosure is restricted by the evidence policy; within the raw capture retain enough consistent identity to verify equality.

Stop once the selected QObject returns or a required branch is found unqualified. Do not continue into TaskInfo, font, or report metadata for this contract. No values from probe004 establish TLS index, TlsGetValue result, or QThreadData identity for a later DOE constructor.

## Implementation consequence

A reconstructed `WidgetThreadState` observation must carry the actual selected thread-data identity and qualified refcount seam in addition to its current notify depth. Counter-only `WidgetRuntimeState` commit cannot stand in for it. Preserve one-time resumability on the same pending Report allocation: until an observed and supported fast-path profile is bound to that existing owner, stop before the first QObject call as the current guard does.

