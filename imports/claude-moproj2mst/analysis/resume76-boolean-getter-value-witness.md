# CP76: actual +3c8 getter-value witness remains unavailable

No direct `SfMoMainWindow+3c8` value at getter read `140018477` was found in the bounded retained original probe survey. Existing constructor-return observations establish owner/source lineage, but cannot initialize the CP75 canonical cell with a current null or nonnull value. The exact missing input is a reached getter-read occurrence with its actual main-window owner/generation and field value; a constructor-return receipt or generic QWidget snapshot is insufficient.

Read-only lane: this report and one compact survey receipt only. No source/build/tests/GUI/native execution/mutable REA. CP75's five helper/test file hashes were rechecked and match its frozen preflight receipt.

## Native read and branch are exact

Retained `resume65-boolean-area-owner-140018460.asm` positively decodes:

| Site | Operand or effect |
| --- | --- |
|18460|Spill incoming RCX `this` into caller home space.|
|18465|Subtract0x48 from RSP.|
|18472|RAX = `[rsp+50]`, the saved actual parent.|
|18477|`cmp qword ptr [rax+3c8],0` — selected current-field read.|
|1847f|Nonzero ->184d2 cached path.|
|18481/18486|Zero -> request allocation0x30.|
|184a2|Only successful allocation branch calls area ctor140d14d30.|
|184cb|Store actual returned/null result into `[rcx+3c8]` after admitted selected constructor return or explicit null-allocation branch.|
|184d7|Load return pointer from `[rax+3c8]` on both paths.|
|184e2|Return after restoring RSP.|

At18477, the selected frame's main-window owner is RAX and also `[rsp+50]`; native return address is `[rsp+48]` because this prologue reserved0x48 and made no pushes. The actual captured module must rebase these sites.184d2 is a common tail reached from both branches, so observing it alone cannot classify cached versus newly constructed return. The compare's nonzero branch also supplies no area address without the actual operand/read or later return value.

CP71's setup body calls the getter at188af and188c8. Earlier initializer184fa supplies a known-null value only at its source store; area construction and intervening callbacks can change later state. The old initializer cannot replace a read at18477. Original EXE identity remains SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Bounded retained observation survey

`resume76-boolean-getter-witness-survey.json` binds hashes for22 retained scripts and12 capture/status files in12 `build/original-*` directories. The survey covers allocator001, editor001..004, splitter001..004, startup001..003 (including retained reviewed/setup/termination variants where present). No script contains an explicit0x3c8,18460,18477,184fa,184cb or d14d30 site/field access. This exact-token search excludes incidental hash substrings; it is not a full proof that every possible dynamically computed address is absent.

The selected scripts' actual snapshot subjects are editor QWidget/private state, layout, ViewManager/grid, SplitView and queue/dispatcher prefixes. No generic main-window memory span was identified as an actual18477 operand/value observation. The survey extracts two `mainwindow_return` observations, but neither records `+3c8`. Script hook absence means the getter was not directly observed there; it does **not** mean the native getter was never called.

CP75's most useful retained record remains editor004: PID27216/TID30444, outer constructor return ordinal1 owner0xafdce0, selected CLI Open ordinal2 same owner. The verified script hooks10490 and the selected Open boundary; its editor hook is eb26f0, not Boolean11e2480. Combined with the unconditional normal ctor/initializer sequence, this supports an earlier initializer store, not its global first ordinal or the current area pointer. Setup/getter source-order reachability does not recover a pointer value. No original address from this terminated process becomes a live candidate allocation identity.

## Smallest exact next witness

For one selected fresh owned original process, retain main-window constructor owner/frame identity and actual loaded EXE hash/base. At the selected18477 occurrence record:

- Process/capture identity, actual thread, unique invocation/observation ordinal and native frame/caller return address.
- RAX, saved `[rsp+50]`, equality to the selected SfMoMainWindow owner, and the exact8-byte value read from `owner+3c8`.
- The executed comparison/branch outcome (18481 allocation prefix or cached jump), retaining nested occurrences. If another thread can mutate the field, a preinstruction sample alone is insufficient: require an actual executed operand trace or a qualified suspended snapshot/step establishing the sampled value is what this instruction used. Do not infer serialization solely from class names.

That read can populate the canonical cell's knowledge at this occurrence. Null permits only the allocation-request prefix. Nonnull remains an opaque raw value until that exact area's canonical allocation/lifetime mapping is established; it does not imply live QObject, impl+28, placeholder/current editor or registration completion. The cached return can be correlated with184d7/actual onLeave; keep the initial compare and later return reads distinct if callbacks/writes intervene on the allocation branch.

To continue the new CP75 window-bound helper beyond ConstructorPending/ParentCommitPending, the same native invocation must additionally supply allocation result, the actual area constructor return and reached184cb store. The helper's opaque invocation identity/revision/occurrence checks bind declared observations; they do not collect these original facts or establish readiness. No HWND, postedWake, elapsed-time guess or timer count is needed for this field read, and none substitutes for it.

If the objective is specifically the first initializer occurrence rather than the next getter decision, the separate184fa/18505 owner/caller witness in CP75 remains required. A getter read can establish current null without proving that initializer's exact first occurrence. These are future evidence specifications only, subject to the existing app-control release gate and owned-session rules; no capture was attempted.

## Integration consequence

Keep the canonical cell unavailable until an admitted actual read/source cursor supplies value. CP75 already reconciles CP65/68 storage and leaves all native fields/lifetimes unknown initially. This lane adds no initializer, getter, area constructor, registration or first-Open Apply activation. The next safe implementation work independent of capture must remain a separately source-qualified primitive, not a fabricated observed field value. Current witness collection is the remaining dependency for selecting this real public owner branch.
