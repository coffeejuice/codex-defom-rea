# CP240 DOE Add Study static audit review

## Result: HOLD for call-site verification; core creation-body claims corroborate

The bounded review found the cited GUI binary hash and both B120 project hashes match their retained receipts. The cited `sub0_s101.c` hash also matches; its `FUN_1400a6a00` body corroborates the reported ordering: housekeeping, current index set from `FUN_1400a7360`, ID/name and DOM-node construction, GUI SUB construction and list append, `FUN_1400a7b90`, then a type-2 `FUN_1400a63c0` request for `SfOprDoeOutput`. There is no direct sample-generation call in that retained function body. The review therefore finds no sample-generation overclaim in CP240.

The current converter comparison is also sound: `convert.cpp` activates subproject 0; `makeSubProject` and `activateSubProject` build/open an existing Type2 record, and the source has no GUI Add Study transition, current-index choice, or initial DOE Output creation request. CP215's reader-I/O identity check is a separate later guard. These facts do not justify injecting Add Study into ordinary conversion.

The review cannot independently verify the earlier action/slot/wrapper/guard call addresses or the precise instruction-site claims in `0x14002AB20`: CP240 cites a DUMPBIN version and says the instructions were checked, but the bounded retained materials reviewed here contain the decompile and qualification, not the DUMPBIN output/command receipt. The decompile begins at `0x1400a6a00` and establishes the function body/order, but cannot independently substantiate the preceding edge addresses. This is an evidence-retention gap, not evidence that the addresses are wrong.

## B120 qualification and next action

The before/after hashes and XML facts match `resume203-b120-doe-state-static.md`: the after project has a Type2 study and seven DOE files. The retained L6 case note, summarized in `resume218-doe-historical-diffs.md`, calls the case “Add DOE Study” and says the action creates project/Control effects before Save. That is useful historical action attribution, but neither it nor the file pair binds the B120 click to the exact guarded success edge, runtime return values, sample-row producer, or per-file writer order. CP240 appropriately leaves those unresolved.

Smallest next action: retain a compact, reproducible DUMPBIN excerpt (or equivalent locally reproducible static disassembly) for the QAction connection, meta-call/wrapper, guarded success branch, and call to `0x1400a6a00`, with PE identity and address mapping. Then a reviewer can verify the upstream address chain without an original run. Keep dynamic same-occurrence B120 Add Study → selection/activation → generation → Save evidence as a separate later requirement. The native Report/QObject dependency and nonempty-key receiver/callback gaps remain independent blockers.

No source, test, build, shared state, original process, GUI, or mutable REA state was changed. Review scope is limited to CP240's cited decompile/qualification, retained B120 reports/files, and current DOE source; no exhaustive search is claimed.
