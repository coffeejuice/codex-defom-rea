# CP103: Save prune, LinkTo cleanup, and the pre-prune owner blocker

## Finding

The native prune effect is value-local to the Study once entered, and CP102's actual Study-owned LinkTo records expose its incoming-link cleanup obligation. However, no source-qualified **public Save** increment can insert that prune before the existing refresh guard without also closing the earlier operation-number reconciliation owner. Native order is: `0x1400d2b60` reconciliation, then `0x1400d3200` calls `0x14090a550`, which prunes eligible variables in `0x14091c820` before the XML QFile open, then later dispatches `DoeTableRefresh`. Candidate `appendType2SaveNodePrefix` currently stops nonempty-variable Save before XML; moving prune before/around it would skip or reorder `0x1400d2b60`.

## Native prune and delete contract

On the hash-verified original GUI PE (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), the retained `0x14091c820` body first scans the manager's variables and collects names whose keyword count is zero **and** geometry count is zero. Sample count is not part of the visible decision. It then processes the collected names: find the first matching variable, get its pointer, call manager vtable `+0x100` (native `0x14091a090`) to remove it, and continue. Thus eligible header-only records can be pruned even when their `numSamples` is positive or zero. The helper returns only after this prune pass; at `0x1400d3200`, its caller then enters `0x14090a550`, whose QFile open is later than the prune. A file-open failure therefore follows already-applied model removals in the native sequence.

The complete retained `0x14091a090` body shows incoming-link cleanup before destruction: it finds the target, iterates the current variable list, compares each variable's `+0x40` link with the target pointer, and zeroes matching incoming links. It then finds the target again, deletes it, removes its pointer from the manager list, and sets the three sample invalid flags. Link pointers are raw/nonowning; deleting the target without the scan would leave stale incoming pointers. CP102 now models `Var.link` as a Study-owned pointer and resolves deferred names onto those actual `Var` records, so this cleanup is a real obligation, not dead metadata.

## Earlier Save dependency that prevents a public increment

The bounded `0x1400d2b60` caller capture establishes it runs first at `0x1400d3227`, before path/variable serializer call `0x14090a550` at `0x1400d349c`. For an empty variable list it has an early return. For nonempty variables it resolves process/main-window state, current MO/Project and the lazy operation manager before looping variables. The active evidence audit reports this as the missing `0x140044c80` lazy operation manager owner. Even a CP102 header-only variable with empty `ops` reaches this outer resolution path before its first operation getter (`0x140816ae0`) returns the empty-list result and operation lookup skips it. Therefore empty operation mappings do not justify bypassing the native manager construction/lookup stage.

Current candidate source has two earlier stops: `source/apps/moproj2mst/src/doe/type2_save_node_prefix.cpp:41-45` chooses `VariableDimensionReconciliation` whenever `Study::vars` is nonempty; independently, legacy `saveInputTable` refuses a nonempty variable list at `doe_study.cpp:400-406`. Public Save never reaches a prune helper. Inserting a Study prune before either stop would mutate variables and could reach XML while omitting native `0x1400d2b60`; that would reverse the required order and misrepresent failure/partial effects. An isolated Study helper could test the local prune/cleanup semantics, but it would not be a public Save continuation or prove the native stage was entered.

`removeVarFiles` at `doe_study.cpp:312-327` is not a substitute: it erases only a borrowed pointer from `Study::vars`, leaves the `headerVariableOwners` unique_ptr alive, does not clear `Var.link` incoming pointers, and does not reset invalid flags. It is a geometry-file bookkeeping path, not native variable ownership deletion.

## Narrow future seam

The next public Save work item is the real operation-reconciliation owner before prune, with ordered results for global/window/current Project lookup, lazy manager creation and per-variable operation mapping. Only after that stage has a source-owned return/failure contract can `0x14091c820` be wired as an ordered Study mutation. For the bounded all-header-record case, a candidate prune must first collect eligibility from the actual records, then for each selected target clear inbound links among current Study vars, remove the borrowed alias and its matching unique_ptr owner without invalidating unrelated records, and reset the three sample invalid flags. It must preserve all earlier prune deletions if a later operation or QFile open fails. Empty keyword/geometry fields qualify these records; do not use `numSamples` as a keep predicate. No new source/test seam is proposed here because it would remain unreachable from public Save without the preceding manager owner.

## Evidence and limits

- `analysis/resume103-doe-prune-14091c820.asm`, SHA-256 `15d8f45477d68837c3c663679c3c6b4e9dbf095e3dcd23b780a360647c6281af`.
- `analysis/resume103-doe-prune-fields-14091a090.asm`, SHA-256 `31046a65e407ac72cc15e0fe5346262fb824fae356eaf5801170641b46c0ae8e`.
- `analysis/resume103-doe-save-reconcile-1400d2b60.asm`, SHA-256 `4e5a81664574951b2a327a00723bc13c1ff0153292be67613f21c1f0598a59a2`; `resume103-doe-save-reconcile-1400d3200.asm`, SHA-256 `73f96c7d1b7861059fef9cd1a71dd7fc0283221c272a145b4b7ee027db318e08`; receipt `resume103-doe-save-reconcile-static.json`, SHA-256 `fac23b7acac4a5eaf95f79174575ffe6f8c48a4624bd2196ccd53ffcdcd7d127`.
- Candidate `doe_study.cpp` SHA-256 `c6eaa2581e23d4bce83301190667d038f4bf2045dca35bc7bc0ec7c0250e83eb`; `variable_record.cpp` `0a802cbbe0b46a80f01a3955cb48c7a346f20f2d9f1519e6d41da2e6d0a78813`; `test_variable_deferred_link.cpp` `c7250c0d37ee6ff4ab06f0e925dea5c0873894d5420164b62dc4ece4d85919ba`; `doe_study.hpp` `ec320a75461def6fc872a123824183301fa518ccbda5c1b6367b178e04005f7d`.

This is bounded retained PE/source review only; no source/test/build, GUI/original execution, or REA database was used.

