# CP90 — Close singleton producer and same-Project handoff

## Finding

The original static call chain closes the local handoff from accepted can-close to the true busy setter and its 0x2caa63 argument block. It does not close the candidate's producer for the original process singleton at 0x143d766b8 or the mutable current-main value used at Close entry. Current close helpers bind a ConversionSession to its actual ModelManager, but receive caller-supplied busy/main observations and a copied busy-state projection. They do not read or update one process owner shared with Open, Save, nested Save and accepted Close. This prevents public normal-Close integration.

## What the retained source closes

The key source flow is retained in evidence/gui_mo/s1/moproj_vt/v6.c and reconciled in analysis/resume89-close-caller-root.md:

1. Native normal Close enters 0x1400299a0 and calls 0x1402ca920(0x143d766b8, true) at 0x1400299cb.
2. 0x1402ca920 reads singleton busy byte +9 first. For zero it gets the current main pointer through 0x1402c8090(0x143d766b8); this getter's body is only return *param. If the pointer is nonnull, the caller asks its virtual +0x300. For the recovered main vtable, that decision includes current stacked-widget index +0x480 and another window predicate. It is mutable UI state; constructor defaults or MainViewState do not qualify its current return.
3. Only an unbusy result continues into the actual global manager's owned children and each child's virtual can-close. The Mo child consults its own Project backlink at +0x19c0 and global process type. A successful .moproj Open makes that Project nonnull and selects process type 5; this branch asks the Save question, and Yes performs a nested Save while the Project is still attached. The modal/answer/nested-Save completion must be part of this same attempt.
4. After all can-close checks allow, public caller 0x140029a54 invokes 0x1402ca9f0(global, true). That setter writes busy +9 and fields +A/+B on the same singleton in native order, then reaches its notification tail. The false Boolean passed into the TreeEnableSelection notification is built synchronously at 0x2caa63; CP89 closes its borrowed QGenericArgument constructor/argument bytes and the call into 0x2cf3c0.
5. This proves a same-process static sequence, not a runtime-bound same Project/Session occurrence. The singleton/main/manager/Project relationships are implicit in native globals and child pointers, whereas the converter's Session is a candidate owner.

## Candidate helpers are not that producer

- close_entry_prefix.hpp defines CloseEntryBusyObservations as a struct of values plus qualification labels. Its helper verifies the supplied Session owner and obtains that Session's actual ModelManager/children; the busy byte, global-main-present fact and main +0x300 result remain supplied observations.
- CloseBusyGlobalOwner takes CloseBusyNativeFields explicitly by value. Its constructor binds its copy to a Session/manager identity for lifetime safety, but comments correctly state it is not a native observation authenticator or production Session field binding. Its 1/0/0 stores mutate that copy, not storage at 0x143d766b8.
- SessionNotificationStateOwner is the candidate notifier owner. It does not establish ownership of the original global singleton or current main pointer.
- Native global getter 0x1402c8090 is a trivial dereference. It proves where a call reads the current qword; it does not produce, construct, or publish the particular main window. The retained Close contracts do not show a source-closed same-instance chain that initializes the global singleton/current main and then preserves it through the converter's Open, Save and Close attempt.

Thus correct Session/manager validation in the helpers is useful and necessary, but it does not upgrade the supplied facts or copied fields into observations of the current native process state.

## Exact missing gate

Before calling the public normal-Close coordinator, require either:

1. a source-closed production owner for the singleton and current-main pointer whose construction/publication and every relevant read/write are represented, and whose same mutable fields are consumed by both entry and accepted setter; plus the actual manager child/Project links and nested process-5 Save/modal continuation; or
2. a same-instance occurrence receipt from one accepted Open→Save→Close attempt that binds the actual singleton address/state, current main identity, ModelManager and Project child, busy/main decision at 0x1400299cb, can-close/modal/nested-Save results, and the later 0x140029a54→0x1402ca9f0(true) setter occurrence, with intervening writers accounted for.

A Session address, a manager backlink, nonblank qualification, declared test values, PE-zero startup defaults, or the mere presence of a main-window candidate is not this gate. Do not inject fixed unbusy/main facts in convert.cpp or run Close from Session destruction.

The smallest static producer query is to close the singleton's construction/publication path, enumerate relevant stores to +0, +9, +A and +B, and trace main-window publication/teardown and mutable virtual +0x300 inputs through Open and Save. Static closure alone still needs same-instance validation that accepted Close follows the qualified root for the selected Project.

## Validation boundary

The current tests for close_entry_prefix and close_busy_prefix exercise source-qualified state-machine prefixes using explicitly declared fixture values. They are appropriate isolated unit tests; they do not call the public conversion entry or manufacture a native Close result. Once the producer gate is met, the smallest integrated test must drive the actual write-save conversion/Session path, retain one Project identity across Open and explicit Save, then validate the busy read, can-close/modal+nested-Save branch, and setter on that same owner before reaching the next guarded notifier effect. Accepted Close and Report/Project cleanup remain later gates.

Read-only retained-evidence and candidate-source review. No source/build/test/GUI/original execution/mutable REA action was performed.

