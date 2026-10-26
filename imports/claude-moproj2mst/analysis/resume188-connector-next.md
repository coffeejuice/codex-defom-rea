# CP188 connector: caller normalization before publication

## Decision

The next bounded caller cut after CP187 is the single instruction at QtCore
RVA `0x13141c`, bytes `33 f6` (`xor esi,esi`). Execute exactly this instruction
and stop at `0x13141e`, before the short jump to `0x131425`. It zeroes RSI/ESI
and updates the arithmetic flags as an XOR; it does not touch RAX (the private
allocation identity), RBX (the public task identity), RDI (the opaque incoming
parent value), memory, or RSP. The next reachable caller store is the separate
public publication `[RBX+8] = RAX` at `0x131425`, outside CP188.

This is an instruction-level continuation of the qualified normal non-null
private-allocation path. It does not complete QObject construction, prove a
parent relationship, or publish either backlink.

## Evidence and symbolic entry state

Use the retained QtCore PE identity from
`analysis/resume163-connector-qobject-static.json`: SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image
base `0x67000000`. The caller listing
`analysis/resume163-connector-qobject-671313d0.asm` contains the bounded bytes
at VA `0x6713141c`, equivalent to RVA `0x13141c`, followed by the jump at
`0x13141e`; the listing is qualified as an unwind region, not as proof of a
complete logical function or runtime behavior. CP187's terminal state is RIP
`0x13141c`, RSP `S+8`, with `S` the private initializer entry-RSP origin.

The caller establishes RBX as the public task identity at `0x1313ee`, copies
incoming RDX to RDI at `0x1313eb`, and invokes the private initializer with
private RAX in RCX at `0x131414..0x131417`. The normal return therefore retains
the qualified task/private identities from CP187. The incoming RDI token stays
opaque; its bits and any QObject-parent interpretation are unknown. CP188 must
require this exact CP187 predecessor/cursor and provenance rather than accept
caller-supplied identities or synthesize a return.

## Transition contract

| Before | CP188 effect | Terminal boundary |
|---|---|---|
| RIP `0x13141c`; RSP `S+8`; CP187 return completed; RAX=private token; RBX=task token | `xor esi,esi`: RSI becomes zero; XOR-defined flags are updated; other GPRs, memory, and RSP are unchanged | RIP `0x13141e`, before `jmp 0x131425` |

The exact architectural flag result for 32-bit XOR is ZF=1, SF=0, PF=1,
CF=0, OF=0; AF is undefined. Represent undefined AF as unavailable/undefined,
not a guessed bit. Since this instruction zero-extends ESI into RSI, the entire
RSI register is zero. Do not alter RAX/RBX/RDI or the symbolic `S+8` stack
cursor. Validate and commit the stage/cursor, RSI, and flag state atomically;
reentry at the terminal must not replay the XOR. Explicit rewind to the CP187
terminal may restore its predecessor RSI/flags/cursor only under the existing
validated rewind contract.

## Guards, successor, and publication boundary

Require the existing owner/control-block binding, pending Report ordinal,
allocation-time private identity, QtCore hash, CP187 terminal stage, RIP,
RSP displacement, return-slot lineage, and qualified task/private tokens. A
missing, foreign, or inconsistent predecessor must refuse without mutation.
The selected continuation is the call-return path at `0x13141c`; the separate
null-allocation branch at `0x131420` is not admitted by this cut.

At `0x13141e`, the next instruction is `jmp 0x131425` (bytes `eb 05`). It
changes control flow only. The target at `0x131425` is `mov qword ptr
[rbx+8],rax` (bytes `48 89 43 08`), the first publication of the private
allocation through the public task object. CP188 stops before both instructions,
so the public `task+8` field remains unpublished. The later `0x131431`
instruction (`mov [rax+8],rbx`) is the distinct private-to-public backlink and
also remains unavailable. No parent-dependent operation or QObject/base
completion is licensed here.

## Focused acceptance evidence

Check that a genuine CP187 terminal advances once to RIP `0x13141e`; RSI is
zero and flags match XOR semantics (AF unavailable), while RAX/RBX/RDI, RSP,
memory, allocation/reference records, and both backlink states remain
unchanged. Wrong phase/cursor, foreign task/private tokens, or corrupted
predecessor lineage must refuse without mutation. Reentry must retain the
CP188 terminal without replay. Assert that neither the caller jump nor
`[RBX+8]` / `[RAX+8]` stores have occurred.

This note uses retained static disassembly and existing source contracts only;
it makes no runtime claim and performed no source edit, build, GUI, REA database,
or deployment operation.
