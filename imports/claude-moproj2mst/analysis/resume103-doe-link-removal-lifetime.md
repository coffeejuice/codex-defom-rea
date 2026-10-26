# CP103 DOE LinkTo deletion and prune lifetime audit

## Finding

There is a source-correctable deletion gap in the existing candidate `removeVarFiles` helper. After CP102 admits nonowning `Var::link` pointers, removal currently erases only the matching raw entry from `Study::vars`; it neither clears incoming links nor removes the corresponding `HeaderVariableRecord` owner. Retaining the concrete owner avoids immediate use-after-free, but leaves a deleted logical variable alive and its inbound links observable. It also makes the next admitted header-row parse fail its `vars.size() == headerVariableOwners.size()` consistency guard.

This helper currently has only a declaration and definition in the source tree and no production callsite (`rg removeVarFiles(`). Thus the mismatch is not a newly reachable public parse path today. It is the incomplete existing UI-removal projection identified by `source/docs/re/moproj2mst/mdt/L6/doe.md` as original `0x1400d69c0`; it should be fixed before that route is treated as integrated.

## Native removal evidence

The immutable original GUI binary hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded read-only DUMPBIN byte disassembly of manager removal body `0x14091a090` through its epilogue at `0x14091a2ce` shows:

1. Resolve the selected target record pointer from the manager list.
2. Iterate current records and compare each record's `+0x40` link with the target pointer (`0x14091a1b9–0x14091a1c7`). For every match, write null to that link field (`0x14091a1c9–0x14091a1d4`). This clears all inbound edges, including a self-link or cycle edge, before target destruction.
3. Delete the target through `0x14091db70` with flag 1 (`0x14091a244–0x14091a253`), then remove its manager-list entry through `0x1401f4660` (`0x14091a269–0x14091a279`).
4. Mark the three sample invalid bytes at manager `+0x70..+0x72` (`0x14091a27e–0x14091a2ac`).

The separate CP102 capture of record destructor `0x1408169d0` confirms it does not follow/delete the `+0x40` link. Therefore the explicit pre-delete incoming-link scan is required; relying on record destruction cannot clean backlinks. Forward links, self-links, two-node cycles, and longer cycles are handled by the same pointer-equality scan: every surviving source link to the removed target is nulled before the target is destroyed; the target's outgoing edge dies with its own record.

## Candidate locations and narrow correction

- `source/apps/moproj2mst/src/doe/doe_study.cpp:312–320`, `removeVarFiles`, currently removes the first name-equal `Study::vars` pointer and proceeds to geometry-file removal.
- `source/apps/moproj2mst/include/m2m/doe_study.hpp` declares `headerVariableOwners` as `unique_ptr<HeaderVariableRecord>` storage. `Study::vars` is the manager's borrowed `Var*` list.
- `source/apps/moproj2mst/include/m2m/mst/doe_model.hpp:73` declares `Var::link` at `+0x40`; `variable_record.cpp:129–143` resolves it as a nonowning pointer.

When a concrete header owner is removed, the narrow repair is to save the exact `Var*`, clear every surviving `s.vars` entry whose `link` equals it, erase its `s.vars` row and matching concrete `headerVariableOwners` entry, and mark `s.samples.invalid` true for all three profiles. Do the link clearing before owner erase. Keep geometry file effects after the manager mutation, matching the existing helper's stated order. Do not delete through `Var*` (it has no virtual destructor); erase only the matching concrete owner. If the matching list entry is not one of the supported concrete owners, retain the existing guard/owner policy rather than guessing a destructor.

A focused test needs no Save/refresh: construct a Study with header-owned A/B/C, set A→B, C→B, and B→A (plus a separate self-link case), invoke removal for B, then assert A and C are unlinked, B is absent from both aligned vectors, and all sample-invalid flags are set. This also prevents a stale target from poisoning the next `appendBoundedHeaderVariable` owner-alignment precondition. Because the helper has no current production callsite, keep the finding scoped to that helper and do not claim public removal integration until a caller is recovered.

Read-only source and immutable-PE analysis only. No source edit, build, test, GUI, original process, or mutable REA operation was performed.
