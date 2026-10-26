# CP82: Boolean creation order and the correct Open boundary

Verified DUMPBIN establishes ordinary call-return order for UpdateData/Apply/addObject and clarifies an important phase boundary: CLI return site `140001c94` is the return from loader01ca30, **before** postOpen02a0e0. It is not whole Open completion. No retained original occurrence proves that Boolean Apply's addObject calls execute before the enclosing Open completes. Keep conditional static order separate from callback scheduling and actual branch/owner observations.

Only this report was written. No source/build/tests/GUI/native execution/mutable database. Immutable EXE SHA-256 rechecked: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Verified tool and bounded checks

Tool: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`; file version and output banner both14.44.35228.0. Its version-directory name is not its reported file version. New decodes read the immutable EXE directly without loading/executing it.

Aligned selected ranges checked: `140001c8a..1ccf` (authoritative complete instructions in the new CLI view); `1411e293f..2970` and `1411e2bb0..2be1`; `140dae349..3b0`; `1405f4516..4528`. A full native unwind interval `1400271b0..140028514` was decoded in memory and only its relevant calls/tail printed. PE exception-table bounds independently give0271b0 end-exclusive028515 and01ca30 end-exclusive01d435. Bounds identify selected code regions, not complete transitive callgraphs.

An exploratory CLI range initially started at1c7f, in an instruction, and its partial/bad prefix was discarded. The aligned1c8a re-decode and existing complete1410 body are authoritative; incomplete suffix bytes after1ccf are also unused. No raw artifact/file outside this report was created.

## Loader return, postOpen return and GUI Open return differ

The selected CLI branch decodes exactly:

| Instruction | Meaning on its normal selected path |
| --- | --- |
|140001c8f ->14001ca30|Call loader/read helper with actual main-window owner.|
|140001c94|Receive/store returned Project. This is the probe's loader-call return-address selector.|
|140001ca9 ->14002a0e0|Invoke postOpen with that Project and same main-window owner.|
|140001cae|PostOpen returned.|
|140001cbb ->14001dc00|Record recent path.|
|140001ccf ->140001e4e|Join after selected CLI path/string cleanup. This is not a separate Open function RET.|

The interactive handler0271b0 has an explicit enclosing function boundary. New complete-region decode shows busy true at028371, loader01ca30 at028383 (continuation028388), postOpen02a0e0 at02839a (continuation02839f), recent-path01dc00 at0283ac, busy false at0283ba, further selected notification/cleanup code, then RET028514. Busy false is not the handler return either.

Successful subproject binding/timer requests are in the later postOpen chain, so they cannot be assigned before the loader return solely because the probe calls its selected01ca30 interval “native Open.” A mutation after1c94 may still be inside the larger Open operation. CLI postOpen/branch completion and GUI0271b0 return are different boundaries/profiles. Any future phase classifier/receipt must carry an explicit boundary kind and source occurrence; the generic “Open return” wording in CP80 is insufficient if bound to1c94.

## What ordinary static order does prove

1. Selected non-Process Object selection5f36d0, with an actual nonzero property-index result and admitted notifications, calls UpdateData notifier1402cf3c0 at5f4524. This precedes old Leave, new factory at5f4756, Fill and Show. Applying an old current editor is different from constructing the new first Boolean editor.
2. CP64's notifier/Qt invoke body binds explicit raw type1: an admitted actual method invocation is synchronous. Notifier suppression, receiver-list order, liveness and method resolution still select whether it invokes anything.
3. Newly re-decoded140dae340 resolves its current guard+18 twice atdae352/365, casts the returned widget atdae38c, and only with nonnull cast calls its virtual+1c0 atdae3ab. The actual Boolean table142cd9b18 selects1411e2870. A registration or the first guard read alone cannot certify the eventual call target/current owner.
4. With Boolean Apply's actual workpiece text `Add New Object`, normal execution snapshots first-free at1411e2944, calls140057ba0 at1411e2955, receives its result at1411e295a and reads result+8 at1411e2964. The independent Cutter branch does the corresponding snapshot1411e2bb5, call1411e2bc6, return1411e2bcb and result+8 read1411e2bd5. Workpiece processing precedes Cutter. CP62 retains comparison/rename sites and the full body.

Thus **if** this admitted synchronous invocation occurs on the selected Open stack and those calls return normally, its addObject return precedes Apply return, UpdateData return, selection return and the enclosing Open frame's return. No timer readiness input is needed for this conditional direct-call order. It does not prove that Open reaches this branch with a current Boolean editor, that its text chooses Add New, or that creation succeeds. Apply entry may bypass both add sites for existing selections. AddObject entry is an attempt; object return and later field/name/Apply completion are separate boundaries. No null-result or exception recovery was invented.

## Where scheduling remains unresolved

PostOpen02a0e0 ->02a9b0 ->0a7250 ->0a7b90 and038380 can bind the diagram model and request separate1000ms single-shots at17085f. The selected038380 body ends by calling the binding virtual; the timer request does not call Boolean Apply synchronously. Model virtual168/restart-icon bodies previously qualified in the timer contract do not close every indirect UI descendant.

An actual timer callback requires registration/delivery, unblocked timeout activation, live receiver, actual Auto0 thread branch and modelUpdated183160. It fits before current-model/item reads, then conditional notifier/selection/current-editor paths may reach Apply. Save's098129 flags-zero poll offers a later nonblocking delivery opportunity, not a1000ms wait or proof of any delivery. A nested event loop could admit a callback before Open returns; a later loop could admit it after. The retained source does not decide between those schedules or prove absence of other reentrant paths.

Initial page/current-editor/impl/receiver state is also not supplied by CP75/77's ownership helpers. Native constructor source population and primary-address identity do not supply a live current Boolean or a current+3c8 value. The original editor004 capture observes eb26f0 rather than Boolean11e2480. Historical final files/screenshots and a Save-time Apply cannot assign an earlier creation phase.

## Exact smallest runtime witness

For positive creation timing, the narrowest useful witness is:

- One selected process/capture/profile, canonical window/Project/Operation/editor/GuiModel generations and the correct enclosing Open boundary. For GUI, retain handler0271b0 entry and actual return028514 to its caller. For CLI, name postOpen completion1cae or selected branch completion1e4e explicitly; do not relabel loader return1c94 as whole Open completion.
- An actual Apply1411e2870 invocation identity, then a normal140057ba0 return whose native caller continuation is1411e295a (Workpiece) or1411e2bcb (Cutter), with actual model/result/object number, ordered relative to that boundary. Keep attempt, successful result, rename and Apply-return events distinct.
- Gap-free coverage of the selected interval if claiming **first** such creation or absence. One qualified positive return proves an occurrence; a gapped first hit does not prove first execution. The two call sites classify Boolean Apply creation, not every object created by file loading or other engine paths.

To prove the **cause**, additionally retain actual UpdateData emitter/receiver/current guard and bounded stack; for a timer cause, add timer/connection/modelUpdated occurrence linkage and actual branch gates. These causal inputs are unnecessary for the narrower positive addObject-vs-boundary timing fact, but necessary before enabling public timer/first-Open dispatch.

The safe bounded source contract remains observation-only admission/classification with explicit LoaderReturned/PostOpenReturned/GuiHandlerReturned/CLIStartupBranchCompleted kinds, owner generations and complete mutation occurrences. Reject missing or ambiguous enclosing boundaries, foreign owners, unsupported nesting and incomplete completion; never turn classification into scheduled dispatch. Existing candidate Add New/order/refill tests verify conditional leaf behavior, not the missing original phase. No new tests or source were written here.

## Retained source qualifications

Full original bodies/contracts used: `wave3-editor-owned-open-entry-140001410.asm`, `wave3-boolean-open-inputmode-1400271b0.c`, `wave3-boolean-open-sequence-contract.json`, `wave3-boolean-external-callback-1400a7250.asm`, `wave3-boolean-open-view-binding-140038380.c`, CP62 Apply/vtable, CP64 direct notifier invocation, CP70/71 Show/receiver population, CP72 timer gate, CP75/76 owner/value gap, CP77 Qt primary identity, CP79 Save readiness and CP80 proposed mutation witness. New DUMPBIN checks above corroborate the selected instruction boundaries and reveal the loader/enclosing-Open distinction. They do not supply original runtime values or callback occurrences. Public guards remain necessary.
