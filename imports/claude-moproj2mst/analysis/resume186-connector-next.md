# CP186 connector: saved RDI restoration contract

## Decision

The next exact cut after frozen CP185 is **`[0x13123e,0x13123f)`**, byte `5f`, `pop rdi`. Restore an opaque, provenance-qualified saved entry RDI token from the symbolic slot `[S-8]`, advance live symbolic RSP from `S-8` to `S`, and stop at **RVA `0x13123f`**, before `ret`. This is one instruction with two register effects. It neither returns to the public caller nor completes QObject construction.

Use the accepted CP185 terminal `BeforePrivateRdiRestore` at `0x13123e` as the sole predecessor. A suitable new phase is `BeforePrivateReturn` at `0x13123f`. Default instruction ordinal may advance from 7 to 8, while preserving explicit stops 0 through 7 and treating 9 as the first invalid ordinal. Names are proposed source integration choices; the instruction boundary and effects are fixed by retained bytes.

## Evidence and scope

Read-only PE extraction reconfirmed `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred image base `0x67000000`. Its `.text` RVA `0x1000` / raw offset `0x400` maps RVA `0x13123e` to raw offset `0x13063e`. The cut byte is `5f`, SHA-256 `d2e2adf7177b7a8afddbc12d1634cf23ea1a71020f6a1308070a16400fb68fde`; the immediately following byte at RVA `0x13123f` is `c3` (`ret`).

The unchanged retained initializer listing `analysis/resume163-connector-qobject-67131160.asm` hashes to `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad`; caller listing `analysis/resume163-connector-qobject-671313d0.asm` hashes to `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. Their source qualification in `analysis/resume163-connector-qobject-static.json` is an unwind region, which alone does not prove the full logical function. CP183/CP184 plans establish the return-register and saved-register derivations; CP185 plan, implementation, source review and `analysis/wave3-checkpoint185-validation.json` establish the retained predecessor contract and its acceptance limitations. This preparation does not independently rerun CP185 validation.

These are static instruction and symbolic provenance facts. No live stack contents, concrete incoming-parent value, full constructor occurrence, exception path, allocator contents or complete application behavior is established.

## Symbolic stack and RDI identity

Retain `S` as RSP on entry to the private initializer at RVA `0x131160`, after the selected caller's CALL has placed its return target on the stack. It is a frame-relative symbolic origin, never a native numeric address. The prologue's `push rdi` at `0x131165` saves entry RDI at `[S-8]`; `sub rsp,0x30` then makes live RSP `S-0x38`. CP184 restores task RBX from `[S+0x10]` without changing RSP. CP185 adds `0x30`, leaving live RSP exactly `S-8`, pointing at the saved RDI slot.

The selected caller sets RDI from incoming RDX at `0x1313eb`, passes the private allocation in RCX at `0x131414`, and calls this initializer at `0x131417`. Therefore the saved RDI token represents that caller's incoming-parent register value. Its concrete value is unknown. It must not be assumed null, equated with task RBX or private RAX, treated as a captured host pointer, dereferenced, or converted into a published parent/backlink relation. The initializer's live RDI zeroed at `0x1311b5` is a different value from the saved entry RDI.

If the frozen frame represents only the saved-slot displacement, introduce the smallest distinct opaque saved-RDI provenance record and restored-RDI result. Derive the record from the qualified caller/prologue and bind it to the current frame/ledger owner and control block, pending Report ordinal, allocation-time private identity, Qt module hash, initializer entry `0x131160`, save instruction `0x131165`, incoming-register origin (`RDX` to caller RDI at `0x1313eb`), call `0x131417` and return target `0x13141c`. The record describes a symbolic value origin; it supplies no native bits or proof of parent ownership. Use a tagged token or equivalent explicit validity representation so that absence is distinct from a possibly null concrete value.

Do not accept an arbitrary caller-supplied saved register value. Establish this deterministic symbolic contract at the appropriate frame-creation/prologue stage; retain and validate it through CP185. Never repair a corrupted persisted record by replacing it with a newly derived valid one. Preserve the historical saved-RBX MOV relation independently: it remains `[S+0x10] = [historical RSP+0x48]` with historical displacement `-0x38`, even after live RSP advances.

POP reads the token at current RSP `[S-8]`, assigns that token to the live restored-RDI abstraction, and advances RSP by 8. Reading/popping the symbolic slot does not zero or erase saved stack contents. Preserve the saved-RDI provenance/slot for validation; phase and live RSP express that the POP has occurred. POP changes no arithmetic flags. Keep CP185's documented ADD flag effect and historical CP182 `flags20Eax` expression separate from this register effect.

## Guard and publication

Before mutation require the genuine CP185 phase/cursor and live RSP displacement `-8`; saved-RDI slot displacement `-8`; return-slot displacement `0`; absent CP186 restored-RDI result; and consistent saved-RDI provenance. Validate every retained predecessor condition: owner/root control blocks, pending Report ordinal, task size `0xc0`, private size `0x88`, allocation-time identity, task/private address points, shared-null bindings/reference deltas, qualified equal-version argument `0x40804` with load `0x13140f` and call `0x131417`, exact sparse prefix, historical allocation-bound `(B & 0xfffffcf0) | 0x60` expression and stage, CP183 RAX/private identity, CP184 restored task RBX and historical save relation, and incomplete/unpublished state. Pointer equality alone does not replace control-block provenance where the existing model requires it.

After successful validation, publish restored RDI equal to the saved entry RDI token, live RSP displacement `0`, and phase/cursor `BeforePrivateReturn` / `0x13123f` as one nonthrowing logical transition. Prepare any allocating token representation before mutation, or use nonthrowing scalar/tagged state so a partial POP cannot remain after failure. Preserve RAX/private identity, RBX/task identity, all stored stack relations, return target `[S]`, private expressions and stores, roots/references, allocations, counts/maps, and completion/backlink flags. No stack-memory capture or native dereference is required.

Extend predecessor-result guards for the legitimate successor: private return identity and restored RBX remain valid at CP186; the restored RDI result is forbidden at all earlier phases and required with the correct token at CP186. Live RSP is `-0x38` before ADD, `-8` at CP185, and `0` at CP186. Validate the whole terminal frame before same-key, different-key or unknown-key reentry refuses; never replay POP or normalize malformed terminal state.

Preserve earlier diagnostic boundaries. Lowering a budget at a retained CP186 terminal must leave that validated terminal unchanged. Explicit test rewind to CP185 clears only the stage-dependent restored-RDI result and restores live displacement `-8`; resumption executes POP once. Earlier rewinds additionally preserve CP185/CP184 rules for live displacement and restored RBX. Retain valid saved-RDI provenance across rewinds, with corruption rejected rather than repaired.

## Cursor ledger and exclusions

| Entry RVA | Effect | Next cursor |
|---|---|---|
| `0x13123e` | `5f`: RDI = symbolic saved entry RDI at `[S-8]`; RSP = `S`; no flags or memory change | **`0x13123f`**, CP186 stop before RET |
| `0x13123f` | `c3`: later RET would consume `[S]`, select caller RVA `0x13141c`, and set RSP = `S+8` | Outside CP186; never model as executable fallthrough at `0x131240` |

The public caller continuation at `0x13141c`, task `+0x08` publication at `0x131425`, private backlink at `0x131431`, and QObject/base completion remain unavailable. Restoring RDI does not authorize any of these effects or establish a parent relationship.

## Focused acceptance before integration

1. A genuine CP185 predecessor advances once to `0x13123f`, restored RDI carries the exact qualified saved token, and live RSP changes from `-8` to `0`. RAX/private and RBX/task identities remain distinct and unchanged.
2. Full predecessor/terminal snapshots differ only in restored-RDI result, live RSP and phase/cursor. Include new provenance and result fields in snapshot/equality helpers; verify saved slot contents/provenance, return target, historical MOV relation, private expressions/stores, ownership/reference deltas, allocations, maps/counts and completion/backlinks remain identical. No architectural flag value is fabricated.
3. Refuse without mutation at wrong/earlier cursor; for premature restored RDI; wrong entry/live RSP or saved/return-slot displacement; absent/foreign saved token or provenance (owner/control block, ordinal, allocation identity, module, initializer, save site, incoming-register origin, caller/call/return target); and all existing malformed predecessor cases. Test represented fields rather than inventing mutable fields solely for corruption tests.
4. At CP186, missing or foreign restored-RDI result, wrong live displacement, or corrupt saved/progenitor provenance refuses unchanged. Repeated same-key, different-key and unknown-key reentry does not replay POP, allocate, adjust references or mutate maps.
5. Retain explicit CP183/CP184/CP185 stops, all earlier flag-budget rewinds and resume cases. Add CP186-to-CP185 rewind/resume, lowered-budget terminal retention and premature-result refusal at every earlier phase. Update obsolete invalid-budget fixtures when admitting ordinal 8, retaining the maximum-size invalid case.
6. Assert no RET/caller advancement, task/private publication, QObject/base completion, native pointer publication or assumed saved-parent value. Preserve the original comparison, focused execution, full regression, source-stable candidate freeze and retained receipts required by integration; retained historical oracles and synthetic helper tests do not establish fresh native GUI lifecycle acceptance.

Only this planning document was written. No source edit, build, test execution, GUI operation, original/candidate execution, deployment or REA database operation was performed.
