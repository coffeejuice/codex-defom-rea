# CP80: DBLIST `+0x1c` writer origin

## Conclusion

Retained evidence still does not identify the first runtime writer, the
setter's caller, or a reset path for the original DBLIST admission DWORD. It
does identify the field's initial storage state and the active gate precisely:
the loader zero-fills `0x143d766b8+0x1c` on image load, while the DoeOutput
path later admits DBLIST only when `FUN_1402cbec0(0x143d766b8)` observes zero.
The known setter `FUN_1402cbf30` stores 1 through its receiver at `+0x1c`,
but neither the function identity nor its body supplies a caller or event.

Consequently “first writer” has two distinct answers. The earliest established
initialization is PE loader zero-fill. The first post-load runtime write is
unknown. The setter is only the first *known write mechanism*, not a proven
first runtime occurrence.

## Gate placement and state origin

On the retained Type2 DoeOutput route, the per-call sixth byte is zero. At
`0x140651eaa`, a nonzero byte branches around DBLIST. With zero, the caller
passes the application-global address `0x143d766b8` to the predicate at
`0x140651ec1`; only predicate true reaches `FUN_1406554a0`. The predicate
compares the receiver DWORD at `+0x1c` with zero. It is evaluated before
DBLIST parsing and before the handler's path-existence and application
ModelManager lookup/create behavior. The caller byte is independent of this
global state.

The global RVA `0x3d766b8` lies `0x2b8` bytes into `.data`'s virtual
zero-fill tail past raw data. This supports initial zero on fresh image load,
not zero after static/dynamic initialization, native Open/Close, or earlier
DoeOutput calls. The application global's first qword is separately used as
the native main-window pointer; the handler obtains its manager from
`mainwindow+0x50`. That establishes the gate receiver's process/application
scope but not the field's owning class or setter occurrence.

## Why setter and reset remain unbound

The retained helper body at `0x1402cbf30` is a receiver-relative store of 1
to `+0x1c`. The direct whole-`.text` raw scan documented in CP66 found no
`E8` call or `E9` jump to the helper and no exact raw code-pointer literal.
Those negative searches do not eliminate indirect calls or constructed
function pointers. The retained Open/Close lifecycle evidence identifies no
call to this setter, no zeroing setter, and no other qualified write to this
DWORD. The sibling `0x1402cbf00` reads the same value for its nonzero
predicate; it is not a writer.

No new disassembly or search receipt was produced for CP80. The result is a
reconciliation of the retained artifacts, not a new exhaustive all-store
proof. In particular, absent xrefs cannot be promoted to “setter is never
called,” and the unknown setter receiver cannot yet be equated to the gate's
global receiver just because both are compatible with the recovered helper
signature.

## Minimal missing dependency

The minimal dependency for a source-qualified state transition is one
occurrence showing the actual call to `FUN_1402cbf30`, the receiver argument,
and the controlling lifecycle event, followed by evidence for every reset or
other write that can change the same receiver's `+0x1c` over the relevant
process lifetime. If static recovery cannot bind this, obtain an original
application-scope trace that records the DWORD and receiver/call stack at
startup initialization, first Open, DBLIST dispatch, Close, and a later Open.
Without that producer/transition contract, do not initialize a candidate
application projection to “admitted” based on fresh Session creation or
initial-image zero-fill. Keep the existing public DBLIST refusal.

## Evidence boundary

Controlling PE: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Relevant retained artifacts:

* `analysis/resume65-dblist-global-owner.asm` — helper bodies, SHA-256
  `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e`.
* `analysis/resume62-dblist-register-and-caller.asm` — active gate callsite,
  SHA-256 `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1`.
* `analysis/resume66-dblist-global-lifecycle.md` — zero-fill mapping and
  bounded call/jump/pointer search result, SHA-256
  `e57b855a498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e`.
* `analysis/resume79-dblist-admission-writer.md` — preceding focused audit,
  SHA-256 `d53d4bc87812e242d669e2ad24f6f444bd646e04a19d20d638e60b7c065bd07b`.

This was read-only retained-evidence review. No GUI, mutable REA database,
source, build, or tests were used or changed.
