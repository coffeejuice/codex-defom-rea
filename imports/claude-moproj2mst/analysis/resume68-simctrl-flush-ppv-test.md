# Checkpoint 68: KEY flush failure with PPV continuation

## Added focused case

Added `native_simctrl_key_flush_failure_still_attempts_local_ppv` to
`tests/s/test_save_io.cpp`. It builds a disposable Project/Boolean Operation
with a valid engine step and one local recovery entry. The test installs the
same read/write-shared byte-zero `LockFileEx` pattern used by the existing
`native_simctrl_failed_open_skips_model_flag_mutation` case, then directly
calls `engine::saveSimCtrl` with native I/O enabled.

The test observes three independent results:

- `saveSimCtrl` returns the KEY writer result (`true`), not the PPV helper
  result;
- the KEY open effect (`step.u1744 == 0xf`) is visible when the callback
  receives the PPV path, and the callback proves the PPV boundary was reached;
- PPV is rewritten to the expected local-only row after the KEY close-flush
  fault setup.

It deliberately does not inspect the KEY bytes or assert any durable residue
from the injected flush failure. The existing adjacent test already exercises
the same host lock pattern and observes a failed candidate flush; this case adds
nonempty local PPV state to cover the subsequent writer call.

## Qualification

The source-qualified original order is that SimCtrl.KEY is attempted first,
then the PPV writer is called regardless of the KEY writer result. The native
KEY wrapper returns the group-writer result and discards `fclose`; the PPV
writer's return does not replace the KEY result. `analysis/wave3-native-simctrl-contract.json`
records that outer `0x140057370` ignores this boolean and proceeds to later
operation-file writers. The retained PPV writer receipt covers its own
successful-open return/order. This test is candidate evidence only; it does not
run the original GUI or claim fresh original parity.

`git diff --check` passed for the owned test file. No build or CTest was run;
root owns validation. No production source, CMake, GUI, mutable REA, or original
binary was modified.

## Freeze hash

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/s/test_save_io.cpp` | `229102e09dfbcdbb9a4f24efd19e45eaddc13637779ebf86b991b034d1c2a3e4` |
