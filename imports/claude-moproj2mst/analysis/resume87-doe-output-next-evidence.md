# CP87: next DOE output evidence and DBLIST blocker

## Result

The CP81 DUMPBIN pass did not locate a direct callsite or owner-bound reset
for the DBLIST admission writer. The only newly owner-qualified `+0x1c=0`
store belongs to a different object: initializer `0x1402cc690` receives the
global base `0x143d763a0`, while DBLIST tests `0x143d766b8+0x1c`. The gap is
not a missing interpretation of that store; it is the absent producer for the
actual gate receiver.

For public DOE Save, the next native output branch is already source-qualified
after a **normal** `signalDoeTableRefresh()` return: current selection writes
`Control/DEF_DOE.PAR` at `0x1400d75a0`, then mode 0/1/2 sample writers are
called in order at `0x1400d7790`. That branch remains unreachable in the
candidate because refresh delivery itself is still refused. These are
separate blockers: DBLIST admission state does not supply the Save callback
receiver, and the callback's return does not resolve the DBLIST global.

## Compact native offsets

| Offset | Qualified fact |
|---|---|
| `0x1402cbf30` | Known helper stores DWORD 1 to receiver `+0x1c`; direct call not found by the verified full DUMPBIN disassembly search. |
| `0x140651eba..0x140651ed1` | DoeOutput caller passes `0x143d766b8` to `0x1402cbec0`; true is required before `0x1406554a0`. This gate is before DBLIST parse/path/manager effects. |
| `0x1402cc6b6` | A different initializer stores zero to `this+0x1c`; its constructor call passes `0x143d763a0`, so its destination is `0x143d763bc`, not `0x143d766d4`. |
| `0x1400997b2` | First post-refresh public Type2 Save call to selected current-table `.PAR` helper `0x1400d75a0`. Requires normal callback return. |
| `0x140099a77`, `0x140099d7c`, `0x14009a081` | Subsequent fixed mode 0, 1, 2 calls to `0x1400d7790`; the first writes `Samples_LHS.par`. |

The literal caller-byte and global admission gate are distinct conditions.
`0x143d766b8+0x0c` process type is a later warning-only input; it does not
change the `+0x1c` predicate. Static initialization yields zero only at fresh
image load, not a per-open or per-Session value.

## Candidate source boundary

`source/apps/moproj2mst/src/moproj/write.cpp:143-161` writes the Type2 input
table prefix, records the refresh boundary, then calls
`table->requireRefreshDelivery(*session)`. The current implementation at
`source/apps/moproj2mst/src/doe/table_owner_state.cpp:232-236` ends in an
unconditional refusal that timer/UI receiver delivery remains unsupported.
Therefore the first candidate guard before `.PAR` output is actual
`DoeTableRefresh` receiver delivery. The next productive output task is to
bind/validate the real refresh sender and receiver/return path; wiring sample
writers before that would invent reachability. Native call order and ordinary
writer-false continuation are already recorded in
`analysis/resume70-doe-par-first-output.md`.

For DBLIST, the next concrete action is an original application-scope capture
at the call to `0x1402cbf30` (if it occurs), recording caller, `rcx` receiver,
and the target DWORD before and after. Pair it with reads at the DoeOutput
gate across fresh startup, first Open, Close, and subsequent Open. If that
setter is never reached in the observed route, trace the actual store to the
gate receiver instead. This is runtime-only with presently available local
tools: the shared REA session has no native PE provider, and DUMPBIN provides
bounded disassembly rather than complete indirect xrefs. Do not lift the
public DBLIST guard or infer candidate admission from initial zero-fill.

## Evidence qualification

Original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
DBLIST gate/helper source captures and CP81 owner comparison are in
`analysis/resume81-dblist-setter-pe.md`. Native post-refresh output order is
qualified in `analysis/resume70-doe-par-first-output.md`. Candidate source
was read only to locate the existing first guard. No GUI, source edit, build,
test, or mutable REA database was used or changed.
