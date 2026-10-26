# CP203 converter RDOE publication static audit

## Decision

No source-ready DOE instruction/state prefix beyond CP201 is established by retained static evidence. The CP199–201 Report/TLS work is a different QtCore initializer path and contributes no value to the nonempty `SetOprHasDoe` conversion. Keep the converter's nonempty-key guard. The exact missing input is a same-occurrence runtime witness joining the selected QtCore codec branch to the natural QString data publication.

This is a bounded read-only audit of retained disassembly and the CP160, CP164, CP196, CP199–201 reports. It makes no claim that all retained evidence has been exhaustively searched. No source, tests, build, GUI, original process, mutable REA database, executable, or deployment was changed or run.

## Static evidence and its boundary

The retained original QtCore disassembly `analysis/resume81-doe-qstring-source-67074120.asm` shows the constructor helper at preferred base `0x67000000`:

| RVA | Static instruction/effect | Unresolved runtime state |
|---|---|---|
| `0x74126` | `cmp qword [QtCore+0x2f3e10], 0` | The loaded value and the flags from this exact execution. |
| `0x74133` | `je 0x741cb` | Which edge this constructor invocation takes. |
| `0x74184` | Reloads `[QtCore+0x2f3e10]` into RCX on the nonnull path. | The pointer actually reloaded and its codec/vtable identity. |
| `0x7419f` | Calls wrapper `0x72460` with the codec, source, length and output slot. | The natural decoder's return, output length, data, aliasing and ownership. |
| `0x741a4` onward | Reads output slot, adjusts reference counts and chooses a return path. | Which return path and concrete object/data state are reached. |

The retained constructor-helper disassembly `analysis/resume81-doe-qstring-source-67174bd0.asm` shows `0x174be7` storing the helper return into `[RBX]`, returning the destination QString object in RAX. These bytes fix instruction order and operand relationships; they do not recover the data read by CMP, the natural branch, or the constructed QString contents.

The DOE caller qualification in `analysis/resume160-doe-codec-next.md` ties the required event chain to GUI `0x0d2995 -> 0x2cf3c0`, constructor `0x2cf41f`, caller return `0x0d299a`, and destination `S+0x98`. It establishes the key source spelling `2signalSetOprHasDoe()` (21 bytes plus NUL) and the later map/list boundary at `0x2cf454`. For the actual nonempty Type2 occurrence, a source string and a conditional null-codec allocation path do not select the helper branch. A separate global snapshot also cannot prove the value consumed by the CMP; it can change between sampling and use. On the nonnull path, static call shape does not determine the virtual decoder result. The later publication store at QtCore `0x174be7` therefore cannot be populated with a qualified value from static bytes alone.

The Report/TLS sequence is expressly orthogonal. CP199's Session-owned symbolic CMP is QtCore RVA `0x29614`, reads the TLS-index cell at `0x2f08e8`, and leaves its operand unknown before JNE `0x2961b`. CP200 retains both JNE successors as unresolved; CP201 adds inert descriptions of their first possible effects without selecting either. That root/thread/capability provenance belongs to the Report constructor frame. It is neither the `0x2f3e10` codec operand nor the DOE dispatcher/QString destination, and cannot authorize a codec read or QString publication. The JNE static audit also establishes no common executed prefix across its two first instructions.

The converter readiness checks agree: CP200 states that no source-ready active-DOE integration follows from retained evidence, while CP196/CP164 identify the selected same-occurrence codec branch and publication as the next evidence requirement. The current `ProcessCodecOwner` is identity with `Unknown` knowledge and has no production codec setter/read-admission API. Inventing a value or treating unknown as null would silently choose behavior unsupported by evidence.

## Smallest missing witness

One natural nonempty Type2 occurrence must correlate all of the following under the same frozen input, process generation, thread, module identities/loaded bases, dispatcher stack S, and event order:

1. Actual caller/dispatcher/constructor: GUI `+0x0d2995 -> +0x2cf3c0 -> +0x2cf41f`, caller return `+0x0d299a`, exact NUL-terminated key source, actual notifier identity, destination `S+0x98`, and actual imported-call target.
2. The flags consumed by the same helper's CMP at QtCore `+0x74126`, observed immediately after it at `+0x7412e` or before JE `+0x74133`, plus the taken edge. Do not substitute a separate read of `+0x2f3e10`.
3. If nonnull, the same invocation's reload at `+0x74184`, actual virtual target at `+0x7247e`, and natural decoder return/output. If null, follow that selected path naturally; its static fresh-allocation contract is not an allocator receipt.
4. Natural publication at QtCore `+0x174be7` and constructor return at GUI `+0x2cf425`. At the former RAX is the returned data pointer; at the latter RAX is the QString object pointer and `[S+0x98]` contains the data pointer. Retain bounded header/ref/length/payload and alias/ownership, or the actual exception/fault. Stop before map/list lookup at `+0x2cf454`.

A missed event, unrelated helper occurrence, timeout, unreadable output, or failed natural return leaves the witness incomplete. Do not seed the codec, invoke a decoder manually, or infer Latin1/fresh allocation/refcount from the ASCII key. Even a complete witness advances only key construction/publication; receiver lookup, normal Type2 Open completion, and public Open–Save–Close remain separate boundaries.

## Result

The static artifact supplies no additional executable instruction/state prefix for this DOE occurrence. It supplies the precise capture frontier and proves why a runtime same-occurrence witness is required: the branch predicate is data-dependent, the nonnull output is virtual-call-dependent, and both must be joined to the caller's actual publication slot. CP201's symbolic TLS edge records cannot fill any of these values. Maintain current production guards and defer source integration until that witness is qualified and reviewed.

Evidence references are retained under `imports/claude-moproj2mst/analysis/`: `resume160-doe-codec-next.md`, `resume164-doe-next.md`, `resume196-doe-integration-next.md`, `resume199-tls-cmp-next.md`, `resume199-tls-cmp-implementation.md`, `resume200-doe-source-ready.md`, `resume200-root-integration.md`, `resume201-tls-edge-static-next.md`, and `resume201-root-integration.md`. Original QtCore bytes are qualified by SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; GUI bytes by `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. These identities are inherited from retained checks, not claimed as fresh runtime evidence.
