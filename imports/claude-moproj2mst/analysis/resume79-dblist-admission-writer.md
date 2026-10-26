# CP79: DBLIST admission DWORD `+0x1c` writer and active gate

## Result

The original PE proves the initial value and the active predicate, but does
not reveal the first runtime writer or the field's owner lifecycle. On image
load, `0x143d766b8+0x1c` is zero because the containing `.data` location is in
the section's virtual zero-fill tail. The one recovered writer helper,
`FUN_1402cbf30`, stores DWORD 1 through its receiver at `+0x1c`; its caller and
receiver occurrence remain unknown. No reset-to-zero writer is qualified.
Therefore initial zero is not a DBLIST admission contract for a later Open in
the same process.

## Active DoeOutput gate

On the retained Type2 DoeOutput load route, `FUN_140650e60` receives a
sixth-argument byte of literal zero. At `0x140651eaa`, nonzero skips the
DBLIST branch. When it is zero, code at `0x140651eba` passes the address
`0x143d766b8` to `FUN_1402cbec0`; only true continues to the corrected DBLIST
handler `FUN_1406554a0`. The helper body compares receiver `+0x1c` with zero
and returns true only on equality. This is the first active runtime gate for
DBLIST and precedes DBLIST parsing, filesystem existence checks, and manager
lookup/create effects. It is independent of process type `+0x0c` and its
warning-only role.

The sibling helper `FUN_1402cbf00` reports nonzero at the same field. The
setter `FUN_1402cbf30` writes the constant 1. Its bounded body has no other
observable store or lifecycle logic. Neither this body nor the gate identifies
the receiver's concrete class or establishes a call event.

## Initialization and search boundary

The PE section map retained in `resume65-dblist-global-owner.md` gives
`.data` VA `0x354c000`, virtual size `0x1b09344`, raw size `0x82a400`, and raw
pointer `0x354a000`. Global RVA `0x3d766b8` is at section offset `0x82a6b8`,
`0x2b8` beyond raw data but inside virtual size. This establishes loader
zero-fill at initial image load; it says nothing about later initialization or
runtime state.

The existing full `.text` raw-byte scan searched direct `E8` calls and `E9`
jumps to `0x1402cbf30`; neither was found. A raw search for the exact code
pointer also found no match. These are bounded negative results, not proof
against indirect calls, constructed function pointers, virtual dispatch, or
other stores. The available artifacts do not enumerate all possible writes
to the global field. In particular, there is no qualified writer that clears
it back to zero and no evidence that native Open or Close resets it.

Thus the first *known* write after loader initialization is the setter's
`+0x1c=1` store, but the first *actual runtime occurrence* cannot be ordered
or tied to an owner. Calling the helper the first runtime writer would exceed
the evidence.

## Owner and blocker

The gate's callsite passes the application-global object at
`0x143d766b8`. Other DoeOutput handler evidence resolves the object's first
qword to the native main-window pointer, then obtains its ModelManager from
`mainwindow+0x50`. This supports application scope for the gate receiver;
it does not identify the field's C++ class, setter caller, or exact transition
producer. The existing candidate application-state projection deliberately
leaves DBLIST admission unavailable and has no setter API. A conversion-scoped
Session or the native main-window ModelManager's candidate analogue cannot
stand in for this unresolved process-global state.

The next evidence needed is either a recovered indirect callsite/dataflow
that invokes `FUN_1402cbf30` with the actual receiver, plus enumeration of all
stores/resets to this instance, or an original application-scope trace that
records the DWORD and owner identity from fresh startup through initialization,
Open, DBLIST dispatch, Close, and a later Open. Until that closes the state
transition, keep the public B127 DBLIST guard and the candidate `+0x1c`
observation unknown.

## Receipts and limits

Controlling original PE SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

* `analysis/resume65-dblist-global-owner.asm` — bounded helper bodies;
  SHA-256 `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e`.
* `analysis/resume62-dblist-register-and-caller.asm` — active caller gate at
  `0x140651eaa..0x140651ed1`; SHA-256
  `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1`.
* `analysis/resume66-dblist-global-lifecycle.md` — full-range direct call/jump
  and pointer search result plus Close/Open limits; SHA-256
  `e57b855a498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e`.
* `analysis/resume65-dblist-admission.md` — literal caller-byte provenance and
  separation from both process-global DWORDs.

This is a read-only static audit of retained PE evidence. No source, build,
GUI, application, or mutable REA state was changed or run. Public DBLIST
admission remains guarded.
