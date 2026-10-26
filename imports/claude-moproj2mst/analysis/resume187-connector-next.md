# CP187 connector: private return boundary

## Decision

The next connector cut after the CP186 `pop rdi` stop is the single `ret` at
QtCore RVA `0x13123f`, byte `c3`, ending at `0x131240`. Model the transfer from
the private initializer back to the caller continuation RVA `0x13141c`, and
stop there **before executing the caller's first instruction**. A RET is not a
fallthrough to `0x131240`.

This is a symbolic control-flow and stack transition only. It does not execute
the caller's `xor esi,esi`, publish the allocation through the caller's public
object field, write the private-to-public backlink, or establish complete
QObject/base construction.

## Retained evidence and qualification

The read-only PE is `imports/claude-moproj2mst/evidence/original-binaries/QtCore4_64.dll`,
SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`,
preferred image base `0x67000000`. RVA `0x13123f` maps to VA `0x6713123f`;
the retained initializer listing ends immediately before the QObjectPrivate
destructor at `0x67131240` and records the byte `c3` at the cut (the previous
cut qualification is in `analysis/resume163-connector-qobject-static.json`).
The retained caller listing
`analysis/resume163-connector-qobject-671313d0.asm` is SHA-256
`6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`.
Its retained source range is VA `0x671313d0..0x671314a0`, equivalent to RVA
`0x1313d0..0x1314a0`. Both listings are qualified as PE unwind regions, not as
proof that either unwind interval equals a complete logical function. This
analysis uses only their bounded instruction bytes and the already retained
CP186 symbolic predecessor contract; it makes no fresh runtime claim.

The caller listing gives the call at RVA `0x131417` and its architectural
return address `0x13141c`. The callee's RET consumes that saved return target
from `[S]`. This relationship follows from the x64 CALL/RET stack semantics
and the listed instruction addresses; no numeric/native stack address or
captured stack bytes are needed.

## Symbolic return contract

Keep `S` as the private initializer's entry RSP after CALL has pushed the
return address. At the accepted CP186 terminal state, the previous POP has
restored the symbolic saved entry-RDI token and live RSP is `S`. The return
slot remains `[S] = ReturnToCaller(0x13141c)`. CP187 requires this exact
provenance-qualified slot and the CP186 phase/cursor; it must not accept a
caller-provided arbitrary target.

On the one successful RET transition:

| Before | RET effect | Terminal cursor |
|---|---|---|
| RIP `0x13123f`; RSP `S`; return slot `[S]` qualified as `0x13141c` | RIP becomes `0x13141c`; RSP becomes `S+8`; no general-purpose register, flags, or memory contents change | `0x13141c`, before caller `xor esi,esi` |

Preserve the symbolic return-slot provenance for validation even though RET
has advanced the live cursor past it; do not model RET as erasing stack memory.
Preserve CP186 restored RDI, CP184 restored task RBX, CP183 private RAX, all
historical stack relations/flags, allocation and owner/reference records, and
all private expressions/stores. Do not fabricate new arithmetic flag bits or
change either object identity. The restored RDI token still denotes the
caller's incoming RDX origin; RET does not turn it into a proven QObject parent.

Require the full accepted CP186 state and all predecessor provenance, including
owner/control-block identity, pending Report ordinal, allocation-time private
identity, module hash, initializer/call sites, saved-register tokens and
displacements, and `ReturnToCaller(0x13141c)`. Validate before mutation. Commit
RIP/cursor and live RSP as one logical transition; use nonthrowing state or
prepare allocations before mutation so a failed operation cannot leave a
partial return. At the terminal state require RIP `0x13141c`, RSP displacement
`+8` relative to `S`, and the exact CP186 restored-register results. Reentry,
including a lowered execution budget, must validate and retain this terminal
state without executing RET again. Explicit rewind to CP186 may restore RIP to
`0x13123f` and RSP to `S`, clearing only the CP187 stage marker; the retained
return-slot provenance remains required and unchanged.

## Caller relationship and hard boundary

The caller listing establishes the sequence surrounding the return:

* At `0x1313eb`, RDI is loaded from incoming RDX. At `0x131414`, RCX is loaded
  from the private allocation in RAX, then `call 0x131160` at `0x131417`
  places `0x13141c` on the stack.
* At `0x13141c`, the first caller instruction is `xor esi,esi`; at `0x13141e`
  it jumps to `0x131425`. Thus the non-null allocation return path rejoins
  there, while the null-allocation branch at `0x131420` is a separate path.
* The instruction at `0x131425` stores RAX to `[RBX+8]`. At `0x131429` the
  caller copies RAX to R12; at `0x13142c` it stores that value in a local
  slot; at `0x131431` it stores RBX to `[RAX+8]`.

CP187 stops at the address `0x13141c` before its XOR. In particular it does not
advance to `0x131425`, write the caller's `[RBX+8]` field, or perform the
private `[RAX+8] = RBX` backlink at `0x131431`. Those are caller-side effects
and remain outside this cut. The later caller `mov [rcx],rax` at `0x1313f8`
is evidence that the caller writes its initial vptr before allocation, but it
does not authorize CP187 to model public-object completion. Likewise, the
caller listing's later parent-dependent operations do not prove the concrete
incoming RDI token or justify parent publication in the return step.

## Cursor ledger

| Entry RVA | Instruction | Required effect / next boundary |
|---|---|---|
| `0x13123f` | `c3` — `ret` | Consume symbolic `[S]`; set RIP=`0x13141c`, RSP=`S+8`; stop before caller instruction |
| `0x13141c` | `33 f6` — `xor esi,esi` | Caller behavior; not part of CP187 |
| `0x131425` | `48 89 43 08` — store RAX to `[RBX+8]` | Caller public-field publication; unavailable to CP187 |
| `0x131431` | `48 89 58 08` — store RBX to `[RAX+8]` | Caller private backlink; unavailable to CP187 |

## Focused acceptance and evidence cursors

Before integration, retain the prior CP183–186 stops and verify:

1. A genuine CP186 terminal advances exactly once to `0x13141c`; RIP and live
   RSP change as specified, while RDI/RBX/RAX provenance and all other modeled
   state remain unchanged.
2. A complete before/after snapshot shows only return-stage cursor/RIP and live
   RSP differences. The symbolic return slot, ownership/reference counts,
   allocation map, private fields, flags, publication fields, and backlinks
   remain unchanged.
3. Wrong phase/cursor, absent/foreign/corrupt return-slot token, wrong slot
   displacement, missing or foreign CP186 restored registers, and every
   represented predecessor-provenance corruption refuse without mutation.
4. Reentry at the CP187 terminal cannot replay RET. Rewinds to CP186 preserve
   slot provenance and allow exactly one subsequent return. Earlier diagnostic
   stops, budget retention and rewind/resume behavior remain covered.
5. Tests assert that the caller XOR, `[RBX+8]` publication, `[RAX+8]` backlink,
   parent inference, and QObject/base completion have not occurred.

Evidence cursors for the next reviewer: `analysis/resume186-connector-next.md`
for the accepted CP186 predecessor and its saved-RDI provenance;
`analysis/resume163-connector-qobject-67131160.asm` for callee epilogue bytes;
`analysis/resume163-connector-qobject-671313d0.asm` for call/return and caller
continuation; `analysis/resume163-connector-qobject-static.json` for PE/listing
hashes and unwind-region qualification; and `analysis/wave3-checkpoint185-validation.json`
plus the CP186 implementation/validation receipts when available for lineage.
The caller listing is static evidence only: caller effects after `0x13141c`,
constructor success/failure behavior, exception unwinding, concrete stack
contents, actual parent relation, and full QObject lifecycle remain unknown.

No source, build, test, GUI, runtime, deployment or REA database operation was
performed for this read-only cut.
