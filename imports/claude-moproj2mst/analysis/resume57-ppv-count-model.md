# Resume 57: PPV count is the existing per-slot `+0xf0` field

## Reconciliation

No duplicate count field was added. The native PPV caller writes each
operation slot's subrecord `+0xf0` (`0x14004df00`), and that exact offset is
already represented as `ObjEntry::sv.nrecvlCount`. The native KEY emitter reads
the same field at `+0xf0` for NRECVL output (`emit_b_lines.cpp` and
`evidence/gui_mo/s1/s1-02/spec.md` §6.4). A separate `ppvCount` data member
would diverge from the original shared storage and could make KEY and PPV
counts disagree.

The model now exposes `StateVars::ppvCount()` and `setPpvCount()` as semantic
aliases over `nrecvlCount`. They add no data and do not change `StateVars`,
`RecoveryEntry`, or their storage layout. Existing KEY reads and writes remain
on `nrecvlCount`.

## Source-qualified initialization and updates

The retained native OPI/SlotRec evidence in
`evidence/gui_mo/s1/s1-01/spec.md` §9.4 identifies the per-slot subrecord as
default-zero. Current `StateVars` also initializes `nrecvlCount` to zero.
Therefore the mapped initial value is zero.

`source/docs/re/moproj2mst/mdt/L0/spec_A.md` §2 and the retained decompile of
`0x1400594d0` establish this update sequence when `SimCtrl.PPV` exists after a
`SimCtrl.KEY` exists:

1. Invoke `ReadPpv` with operation-local points and either shared root/flags
   (predecessor case) or null shared/flags (no-predecessor case).
2. Ignore `ReadPpv`'s returned bool.
3. Compute the post-call list size: local plus root in the predecessor case,
   local only otherwise.
4. Assign that total to `+0xf0` for each present slot subrecord.

This means a successful-open parse failure still assigns a count based on the
partial mutations retained by the reader. If QFile open fails, the list sizes
are unchanged but the caller still reaches the count update. Missing PPV skips
the update. Missing `SimCtrl.KEY` returns through inheritance before the PPV
path. The focused test covers the state alias and per-slot ownership; it does
not simulate a loader update or claim loader integration.

## Scope and remaining integration

Added `tests/mdt/test_ppv_count_state.cpp` with Release-active Testkit checks
for zero initialization, aliasing with the existing KEY NRECVL field, setter
behavior, and distinct count state per slot. The test was not built or run.

No loader code now updates this state; the public PPV Open guard remains. The
alias resolves the missing-model-field concern from `resume57-ppv-open-binding.md`,
but exact QFile/QTextStream line decoding and the predecessor shared-root Save
coverage remain open. No Save code, CMake, GUI, or mutable REA changes were
made. `RecoveryEntry` semantics and KEY serialization were not changed.
