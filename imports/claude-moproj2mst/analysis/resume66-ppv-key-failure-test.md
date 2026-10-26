# Checkpoint 66: public PPV continuation after KEY open failure

## Added acceptance case

Added `public_ppv_save_continues_when_simctrl_key_open_fails` to `tests/mdt/test_ppv_public_lifecycle.cpp`. It reuses the source-qualified Oracle-B fixture/copy and selects Task00002 as the active operation. The disposable copy receives a nonempty local PPV row. A Windows handle is then opened on SimCtrl.KEY with `GENERIC_READ | GENERIC_WRITE` and `FILE_SHARE_READ`: the Open phase can read the file, while a new write open is denied. SimCtrl.PPV remains writable.

The case invokes the public conversion API and checks that it reaches `M2M_OK` and creates the MST, the SimCtrl.KEY hash remains unchanged, and PPV is rewritten to its qualified normalized local-row bytes. It also verifies the source fixture manifest is unchanged. This distinguishes the branch from the existing `public_ppv_readonly_save_open_failure_keeps_key_save_success` case, which covers the inverse condition (KEY Save succeeds, PPV open fails).

The test asserts only candidate behavior against the retained original order: native caller `0x1400597d0` attempts KEY first, then calls PPV regardless of the KEY result, ignores the PPV result, and returns the KEY result. Candidate `save_current.cpp`'s `saveCurrentOperation` ignores the internal `saveSimCtrl` boolean, so the public API result remains `M2M_OK` even when the KEY open fails. No fresh original execution or parity claim is made.

## Validation boundary

No build or CTest was run; root owns validation. `git diff --check` passed for the owned test file. The test relies on the existing supported same-host Windows sharing semantics; root should validate the actual run before accepting it. No production source, CMake, GUI, mutable REA, or original binary was touched.

## Freeze hash

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/mdt/test_ppv_public_lifecycle.cpp` | `cae17de6c7ffb5102ee84df0ac01ad59c21721b284c49ba8b15d6c008ad7823e` |
