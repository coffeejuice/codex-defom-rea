# CP214 postOpen probe: independent copied-PE static review

**Static path: PASS. Original-run release: HOLD** until the exact disabled runner and its native relocation fixture are reviewed. This review used only the copied original installation v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained disassembly, and fresh bounded DUMPBIN 14.44.35228.0 views. Its PE version resource is null. No original process, GUI control, mutable REA session, runner, source, fixture, or shared state was changed. This is instruction evidence, not observed Open behavior.

## Exact caller and entry evidence

Direct section-file reads of that PE confirmed these bytes; DUMPBIN decoded the instruction boundaries. Addresses are RVAs under image base `0x140000000`.

| Site | Exact bytes and meaning | Filtered return |
|---|---|---|
| `1ca9` | `e8 32 84 02 00`: call `2a0e0`, after `1c94` stores outer RAX, `1c9c` reloads it into RDX, and `1ca4` prepares main in RCX | `1cae` |
| `2a19c` | `e8 0f 08 00 00`: call `2a9b0` on nonnull postOpen arm | `2a1a1` |
| `2aa6d` | `e8 0e ad 01 00`: call `45780` after fresh `133a0` manager lookup | `2aa72` |
| `2aa9c` | `e8 af c7 07 00`: call `a7250` with RCX from saved newly current Project | `2aaa1` |

The `45780` caller input is the manager pointer in RCX; its RAX is the current Project. There is another `45780` call at `2a9d5` for the **old** current Project. The `2aa72` return-site filter and same postOpen frame are essential. The normal null-current branch at `2aa7f` jumps to `2aad0`, bypassing activation. `2a0e0` is void; its RAX must not be treated as a success code. Its common tail continues through virtual callbacks before `2a99e` returns to `1cae`.

## Prologue and five-byte E9 review

| Entry | Exact original prefix | First five-byte write | Instruction relocation needed |
|---|---|---|---|
| `2a0e0` | `48 89 54 24 10 48 89 4c 24 08 56 57 48 81 ec 88 04 00 00` | Replaces complete five-byte `mov [rsp+10h],rdx` | At least 5 bytes |
| `45780` | `48 89 4c 24 08 48 83 ec 48` | Replaces complete five-byte `mov [rsp+8],rcx` | At least 5 bytes |
| `a7250` | `89 54 24 10 48 89 4c 24 08 48 83 ec 38` | Replaces four-byte `mov [rsp+10h],edx` **and first byte** of five-byte `mov [rsp+8],rcx` | At least 9 full bytes |

Thus a five-byte E9 is plausible at all three entries, but the activation site is not a five-byte instruction boundary. Frida must relocate both complete activation instructions into its trampoline and resume at `a7259`; a five-byte patch with an unchanged suffix at `a7255` is not by itself proof of correct execution. Require an exact-code native fixture exercising activation entry and normal leave, plus the existing loaded-image original-prefix, E9 destination, and unchanged-suffix gates before release. A different patch shape or unproven relocation is HOLD.

## Same-frame obligations and index correction

Let `S` be RSP on the filtered `2a0e0` entry. Its two pushes and `sub rsp,488h` give body RSP `S-498h`. Direct call at `2a19c` gives `2a9b0` entry `S-4a0h`. Its `sub rsp,38h` gives body `S-4d8h`; the direct calls to `45780` at `2aa6d` and `a7250` at `2aa9c` each enter with RSP `S-4e0h`. These equalities are for the selected synchronous chain and should be checked alongside primary TID, call return address, one live postOpen frame, matched entry/leave, main identity, and outer-P identity. No fixed RSP relation to the earlier outer `1ca30` frame is established or required.

The plan's phrase `a7250(currentP,0)` is a **runtime gate**, not an invariant of the static caller. At `2a153`, postOpen initializes its local `[rsp+70h]` to zero and at `2a18a` passes its address as R8 to `2a9b0`. At `2aa8f`, after several calls, `2a9b0` reads that pointer from its stack slot; `2aa94` loads EDX from the pointed word immediately before activation. Intervening code can have changed that word. The requested list ordinal is a different local: `c79ee0` returns it to `[rsp+74h]`, whose address is passed in RDX. An observed raw EDX low 32 bits of zero at the filtered `a7250` entry, paired with RCX equal to the witnessed current P and outer P and a normal matching leave, suffices to claim this occurrence called `a7250(P,0)`. It does not prove the source local remained zero throughout postOpen or that future occurrences use zero.

The probe should separately report loader P delivered to postOpen, postOpen normal return, current-P equality, activation entry/index, and activation normal return. A nonzero loader P or a zero current lookup cannot be promoted to completed activation. The CP213 independent review qualifies its prior outer-return observation despite strict capture flags, but it contains no postOpen witness. The CP214 observation remains instrumented and bounded to one occurrence; it cannot establish completed GUI binding, XML read, timers, Save, Close, or full lifecycle parity.
