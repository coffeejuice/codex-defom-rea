# Resume 58: root-only PPV Save gate

## Finding

The native PPV writer gate is exactly `QListData::isEmpty()` on the **local**
point list. This resolves the remaining predicate ambiguity from
`resume48-ppv-writer-contract.md` / `resume57-ppv-writer.md`.

The bounded immutable PE capture `analysis/resume58-ppv-save-gate.asm` checks
the original target SHA-256 and disassembles only `0x140307410..0x140307450`.
The thunk calls through `0x141e03f00`; the slot's file-backed RVA resolves to
the import name `?isEmpty@QListData@@QEBA_NXZ` (`QListData::isEmpty() const`).
The complete PPV writer receipt `analysis/resume48-ppv-writer-receipt.txt`
shows that a true result skips QFile construction/open and returns false;
false enters writer setup.

## Native Save call and root-only behavior

The qualified caller `0x1400597d0` is documented in
`source/docs/re/moproj2mst/s/spec.md` and retained in
`evidence/gui_mo/out_s/s1.c`. It writes SimCtrl.KEY first, then calls the PPV
writer with the operation-local list (`OPI +0x20`), path, and—when a
predecessor exists—the shared data-root list (`+0x19a8`) and flags (`OPI
+0x28`). With no predecessor it passes null shared/flags. The wrapper's return
is the KEY writer result; the PPV helper return does not change it.

For a **root-only nonempty state** (local list empty, shared root list
nonempty), the PPV writer's local `isEmpty()` gate returns true and the writer
exits before opening/truncating SimCtrl.PPV. Root size and flags do not affect
that early exit. No PPV count line or root row is emitted; any existing PPV
file is left untouched by this helper. SimCtrl.KEY has already been written.
This is the exact gate behavior; it does not claim how other Save phases alter
files.

In the current converter, `saveSimCtrl` writes SimCtrl.KEY and then throws
unsupported only when `op.recoveryB` (local list) is nonempty. A root-only
nonempty state therefore does not trigger that guard. Since the candidate does
not yet call the native PPV writer, it leaves SimCtrl.PPV untouched and
returns the KEY result. For this root-only case, that observed effect matches
the native writer's early exit. The existing local-only guard is not missing a
root-row serialization path: the native PPV writer itself does not reach the
root loop when its local list is empty.

## Isolated conditional helper

Added `include/m2m/engine/simctrl_ppv_save_gate.hpp` with
`ppvWriterGateAllows(localPointCount)`, a pure expression of the qualified
local-list gate. Added Release-active testkit coverage in
`tests/mdt/test_simctrl_ppv_save_gate.cpp` for root-only points (writer skipped)
and nonempty local points (writer entered). It has no filesystem, Project,
Operation, Save, or writer-payload side effects and is not wired into public
Save. The test was not built or run.

## Boundary

This resolves root-only **entry-gate** behavior, not a broader Save contract.
`0x140d4afe0` still has open/file/payload effects recorded in
`analysis/resume48-ppv-writer-contract.md`; the helper preserves them without
reinterpretation. No source `save_current.cpp`, loader, writer dispatch, CMake,
GUI, or mutable REA changes were made.
