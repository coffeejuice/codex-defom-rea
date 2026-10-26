# CP184 connector: saved RBX restoration contract

## Decision

The smallest next exact cut after CP183 is **`[0x131235,0x13123a)`**, `48 8b 5c 24 48`, `mov rbx,qword ptr [rsp+0x48]`. Model restoration of the saved caller RBX as an opaque, owner-qualified task identity, and stop at **RVA `0x13123a`**, before stack adjustment. RAX continues to carry CP183's private allocation identity. This cut changes neither private storage nor either backlink and does not return to the public QObject caller.

Use a distinct symbolic saved-register/stack contract. Do not reinterpret `allocatedPrivateIdentity` as the restored RBX: the initializer deliberately replaces RBX with the private pointer, then restores its caller's RBX. A single-instruction increment is sufficient; the later epilogue instructions are fully laid out below so an implementation can preserve every cursor without broadening this acceptance boundary.

## Retained evidence and qualification

The selected binary is `evidence/original-binaries/QtCore4_64.dll`, verified SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred image base `0x67000000`. PE `.text` has RVA `0x1000`, raw offset `0x400`, and raw size `0x1a6200`; these ranges map through `raw = RVA - 0xc00`. Direct read-only PE extraction reconfirmed the instruction bytes and hashes below.

`analysis/resume163-connector-qobject-static.json` records the dumpbin selections and qualification: the ranges are unwind regions or bounded candidates, and neither alone proves a complete logical function. The retained initializer listing `analysis/resume163-connector-qobject-67131160.asm` hashes to `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad`; the public caller listing `analysis/resume163-connector-qobject-671313d0.asm` hashes to `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. Both were verified unchanged. These static artifacts establish the selected instruction sequence and symbolic relations, not live stack contents, runtime constructor occurrences, exception handling, allocator contents, or complete application behavior.

## Symbolic stack and register derivation

Let `S` denote RSP immediately upon entry to the private initializer at RVA `0x131160`, after the caller's `call` placed the return address on the stack. `S` is a frame-relative symbolic origin, never an integer host address. Bind its provenance to the current allocation ledger/control block, pending Report ordinal, retained private allocation identity, Qt module hash, initializer entry `0x131160`, and selected call/return RVAs `0x131417`/`0x13141c`.

The retained prologue supplies these exact relations:

- `0x131160`: save entry RCX at `[S+8]`.
- `0x131165`: `push rdi` saves entry RDI at `[S-8]` and sets RSP to `S-8`.
- `0x131166`: `sub rsp,0x30` sets RSP to `S-0x38`.
- `0x131173`: save entry RBX at `[RSP+0x48] = [S+0x10]`.
- `0x131178`: `mov rbx,rcx` makes live RBX the private allocation identity.

No selected later instruction overwrites these two saved-register locations or adjusts RSP before the epilogue. Object stores through RBX are separate from this symbolic stack. On the qualified equal-version path the error-reporting call at `0x131206` is skipped. RDI is zeroed at `0x1311b5`, so its pre-epilogue live value is zero; the saved RDI is still the entry value.

The selected public caller establishes RBX as its task/`this` identity at `0x1313ee`, sets RDI from its incoming RDX at `0x1313eb`, passes the private allocation in RCX at `0x131414`, and calls the initializer at `0x131417`. Thus saved RBX is the qualified pending task identity. Saved RDI is an opaque incoming-parent register value; this evidence alone does not make it null or supply a concrete pointer. `[S]` is the selected return target, module-qualified RVA `0x13141c`.

Keep separate fields or tagged symbolic expressions for the stack origin, saved RBX task identity, current RBX role, saved RDI origin, RSP displacement, and return target when those effects become modeled. A minimal CP184 representation needs only the qualified save-slot relation and the restored task identity; the remaining derived relations can stay a documented contract until their instruction cuts. It must not claim to have captured native stack memory. Construct/derive provenance from the already qualified caller and prologue; do not accept caller-supplied arbitrary stack values or silently repair malformed persisted provenance.

CP183's `returnPrivateIdentity` is the live RAX abstraction. `flags20Eax` remains historical provenance of the prior dword read/transform/store after `mov rax,rbx`; it cannot also describe the live EAX subregister once RAX is a pointer identity. Preserve that historical expression and `private20` exactly, without asserting simultaneous incompatible register values.

## Every remaining cursor

All ranges are half-open, and displacements are relative to the same `S`.

| Entry RVA | Instruction / byte hash | Post-state / next cursor |
|---|---|---|
| `0x131235` | `mov rbx,[rsp+0x48]`; `8b9fda10f757f3019b481c09d8cf241a230614f1344dd5513a53273c7917adb5` | RBX = saved entry RBX = qualified task identity; RSP stays `S-0x38`; stop **`0x13123a`**. CP184 boundary. |
| `0x13123a` | `48 83 c4 30`, `add rsp,0x30`; `60eb65657d43c4929eba7fce677695fd345baf50ffd9b5c56ef59b6125664f19` | RSP = `S-8`; cursor `0x13123e`. ADD also updates arithmetic flags; if modeled later, retain their symbolic dependence on S rather than inventing concrete flag bits. |
| `0x13123e` | `5f`, `pop rdi`; `d2e2adf7177b7a8afddbc12d1634cf23ea1a71020f6a1308070a16400fb68fde` | RDI = saved entry RDI, RSP = S; cursor `0x13123f`. |
| `0x13123f` | `c3`, `ret`; `ae3f4619b0413d70d3004b9131c3752153074e45725be13b9a148978895e359e` | RIP = module-qualified return RVA `0x13141c`, RSP = `S+8`, RAX unchanged. This is a caller cursor, not executable fallthrough at `0x131240`. |

RAX/private identity remains unchanged throughout these four instructions; only the later RET consumes the return slot. No instruction here publishes the task/private backlink. The caller resumes with `xor esi,esi` at `0x13141c`, then jumps at `0x13141e` to `0x131425`. The task `+0x08` publication at `0x131425` and private backlink at `0x131431` must remain separate subsequent effects. Even a future complete epilogue batch would complete only this private initializer's normal return, not public QObject construction.

## Source-ready CP184 contract and guards

Accept only a genuine CP183 terminal frame (`BeforePrivateEpilogue`, RVA `0x131235`), with the same qualified owner/root control blocks, pending Report ordinal, native task size `0xc0`, private size `0x88`, stable allocation-time private identity, task/private address points, all committed shared-null bindings and reference deltas, equal-version provenance (`0x40804`, argument load `0x13140f`, call `0x131417`), and exact sparse-zero prefix. Require the historical EAX/private20 expression `(B & 0xfffffcf0) | 0x60` with its unchanged allocation-bound unknown origin and stage, and `returnPrivateIdentity == allocatedPrivateIdentity`.

Require the saved-RBX relation to name `[S+0x10]`, equivalently `[current RSP+0x48]` at displacement `-0x38`, with the same frame/caller provenance and owner-qualified pending task identity. A restored-RBX result must be absent before this instruction; at the new terminal cursor it must equal that saved identity. A pointer equality alone is insufficient where the existing code additionally qualifies shared ownership/control blocks. No dereference of an opaque task or stack token is authorized by this cut.

After validation, publish only the symbolic restored RBX result and a new phase such as `BeforePrivateStackAdjustment` at `0x13123a`. Pair the result and phase in a nonthrowing transition. RAX, RSP, RDI, stored stack relations, sparse private state, expressions, root references, counts, maps, allocation identity, completion flags and backlink availability remain unchanged. The native MOV changes no arithmetic flags.

The existing CP183 pre-terminal guard currently forbids `returnPrivateIdentity` whenever phase differs from `BeforePrivateEpilogue`. An implementation must extend that guard to the new legitimate successor without allowing the slot at earlier cursors. Likewise keep the ordinal-five CP183 stop available when adding the successor; do not replace an old diagnostic boundary. Validate the terminal state before refusing reentry, without replay or normalization. Earlier diagnostic rewinds clear the new restored-RBX result, preserve the old rewind semantics, and deterministically retain or rederive only valid symbolic prologue provenance. A corrupted retained stack contract must refuse, not be overwritten into validity.

## Acceptance before integration

1. From a valid CP183 predecessor, verify one MOV transition reaches `0x13123a`, restored RBX equals the owner-qualified pending task identity, RAX still equals the private identity, and all other state/reference deltas match a full predecessor snapshot.
2. Keep separate task/private identity roles in assertions. Confirm successful restore does not change the stored return identity, historical EAX expression, private20, or private allocation identity.
3. Refuse without mutation for wrong/earlier cursor; premature restored result; missing/foreign saved RBX identity; foreign ledger/control block/ordinal/private origin; wrong save-slot displacement or current RSP stage when represented; wrong module/entry/call/return provenance; missing/foreign RAX token; and every existing malformed owner/root/version/sparse/expression case. Do not invent unrepresented mutable fields solely for rejection tests.
4. At `0x13123a`, missing/foreign restored identity and corrupted predecessor provenance refuse unchanged. Both same-key and different-key reentry refuse with no second transition, new allocation, root increment or cache/map effect.
5. Preserve every CP182/CP183 diagnostic stop and rewind/resume case. Extend full-frame snapshot/equality helpers and verify rewinds clear only stage-dependent results. Earlier cursor tests must still reject a premature return token.
6. Verify stack adjustment, RDI restoration, RET/caller advancement, QObject/base completion and both backlinks remain unavailable. No numeric native addresses or assumed saved-RDI value appear in the model.

Retain the established source-bound original comparison, candidate-freeze chronology and required regression gates before integration. This preparation did not run an original/candidate process, build, tests, GUI operation or REA database operation and does not establish their acceptance results. Only this planning document was written.
