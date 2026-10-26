# CP183 connector: private initializer return-register setup

## Decision

After the accepted CP182 symbolic flags write, the next exact interval is **`[0x131232,0x131235)`**, end exclusive. It is one instruction, `mov rax,rbx`, which copies the retained private allocation pointer into the native return register. The smallest source-ready reconstruction is a symbolic return-register identity tied to the already qualified allocation, followed by a stop at **RVA `0x131235`** before the epilogue restores RBX.

Do not publish a host pointer or mark QObject construction complete. The binary establishes register identity (`RAX=RBX`), while this model uses qualified allocation identities rather than native addresses. The next three instructions restore RBX, adjust RSP, and restore RDI; `ret` then returns the pointer. Those require a separate stack/register continuation contract if modeling the complete native initializer return. Public QObject initialization continues only after the call returns and remains outside this cut.

## Bytes and cursor sequence

The retained QtCore DLL is `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred image base `0x67000000`. The retained disassembly `analysis/resume163-connector-qobject-67131160.asm` covers the complete private initializer. Its `.text` mapping gives raw offset `RVA - 0xc00` for these bytes.

| Preferred VA / RVA | Bytes | Effect / next cursor |
|---|---|---|
| `0x67131232..0x67131235` / `[0x131232,0x131235)` | `48 8b c3` | `RAX=RBX`, the same private allocation pointer; stop at `0x131235`. |
| `0x67131235..0x6713123a` / `[0x131235,0x13123a)` | `48 8b 5c 24 48` | Restore saved RBX from `[RSP+0x48]`. |
| `0x6713123a..0x6713123e` / `[0x13123a,0x13123e)` | `48 83 c4 30` | `RSP += 0x30`. |
| `0x6713123e..0x6713123f` / `[0x13123e,0x13123f)` | `5f` | Restore RDI from stack. |
| `0x6713123f..0x67131240` / `[0x13123f,0x131240)` | `c3` | Return to caller with RAX containing the private pointer. |

The cut bytes hash to `88de8531189088e84aea757958f351c870c008322498ae012d5c1e52fc597037`. Adjacent instruction hashes, useful for boundary checking, are `8b9fda10f757f3019b481c09d8cf241a230614f1344dd5513a53273c7917adb5` for `[0x131235,0x13123a)`, `60eb65657d43c4929eba7fce677695fd345baf50ffd9b5c56ef59b6125664f19` for `[0x13123a,0x13123e)`, `d2e2adf7177b7a8afddbc12d1634cf23ea1a71020f6a1308070a16400fb68fde` for `[0x13123e,0x13123f)`, and `ae3f4619b0413d70d3004b9131c3752153074e45725be13b9a148978895e359e` for `ret`.

## Guards and unknowns

Entry requires the genuine CP182 terminal frame at `BeforePrivateReturnSetup` / `0x131232`: the pending Report ordinal and allocation-time private identity remain bound; private allocation size is `0x88` under the `0xc0` task allocation; the qualified task/private address points and QObject initializer version/equal branch remain valid; all previously accepted sparse private stores remain intact; CP182 EAX and `private20` carry the same stable unknown-origin symbolic expression `(B & 0xfffffcf0) | 0x60`; and QObject/base completion plus both public backlinks remain unavailable.

`RBX` is the private pointer established at RVA `0x131178` and is not overwritten between then and this instruction on the selected equal-version path. There is no call or branch in the intervening initializer tail. The move itself does not dereference or mutate private state. Model it as a symbolic `returnPrivateIdentity = allocatedPrivateIdentity`, with a new cursor such as `BeforePrivateEpilogue` / `0x131235`. Validate the CP182 expression and owner prefix before publication. Do not treat the sparse symbolic private20 value as a concrete integer.

The binary supplies no stack contents for saved RBX/RDI, no modeled RSP value, and no native host pointer suitable for publishing as a real address. The exact ABI return is evident after the epilogue and `ret`, but the selected source model has not established a portable symbolic stack/return mechanism. The public constructor's later task `+0x08` publication and private backlink are at `0x131425` and `0x131431` in the retained caller disassembly; they are subsequent operations, not effects of this initializer tail.

## Smallest source-ready cut and acceptance

Add a symbolic return-register slot containing the allocation identity, not a pointer-sized integer, and a phase/cursor for `0x131235`. CP182's `BeforePrivateReturnSetup` denotes the pre-instruction state; after one accepted transition, only this slot and phase should change. This preserves the native register fact while keeping the source model's address abstraction intact.

Focused acceptance should verify: (1) a valid CP182 frame advances to `0x131235` with the identical allocation identity in the return slot; (2) every CP182 expression, private sparse field, root binding/reference delta, map/count, and incomplete/backlink flag is unchanged; (3) missing or foreign return identity, invalid owner/root/version/predecessor state, mismatched EAX/private20 origin or stage, and wrong cursor refuse without mutation; (4) same-key and different-key reentry at the terminal cursor does not replay the move; and (5) earlier CP182 diagnostic rewinds clear the new slot, while terminal-state snapshots include it. Preserve CP182's original comparison/freeze gates before integrating.

Do not implement saved-register restoration, stack adjustment, `ret`, QObject completion, task publication, or backlinks in this cut. If the next increment is to model the initializer's actual return, first define a symbolic stack/register contract for `[RSP+0x48]`, `RSP`, and RDI, then compare it against the exact epilogue sequence shown above.

## Evidence qualification

Evidence is static disassembly from the shipped QtCore binary, qualified by module SHA-256 and RVA. The retained complete initializer listing is `analysis/resume163-connector-qobject-67131160.asm`; the nearby public QObject caller is `analysis/resume163-connector-qobject-671313d0.asm`. This establishes the selected instruction and its static register relation, not a live constructor occurrence, concrete allocator contents, all caller states, exception behavior, cleanup, or complete application semantics. No mutable REA database, source, plan, state, GUI session, original app, build, or test run was used or changed for this investigation.
