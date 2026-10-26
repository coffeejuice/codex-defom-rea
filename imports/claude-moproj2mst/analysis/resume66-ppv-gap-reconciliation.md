# Checkpoint 66: PPV Save failure-gap reconciliation

## Correction to CP65

The proposed “successful KEY plus denied PPV open” public acceptance case is already covered by `tests/mdt/test_ppv_public_lifecycle.cpp::public_ppv_readonly_save_open_failure_keeps_key_save_success` (third case). It creates a disposable copy, selects the operation owning Task00002, supplies a nonempty local PPV row, sets the PPV file read-only, and invokes the public conversion API. Assertions establish `M2M_OK`, MST creation, exact PPV byte preservation, retained read-only attribute, and an updated SimCtrl.KEY last-write time. This demonstrates PPV open failure after the local gate does not undo or replace successful KEY Save. The CP65 suggestion is redundant; no additional sharing-denied variant is warranted for that contract.

## Source-qualified remaining branch

Original caller `0x1400597d0` invokes PPV after attempting SimCtrl.KEY regardless of the KEY writer result; it discards PPV return and returns the KEY result. In current candidate `save_current.cpp::saveSimCtrl`, `r` begins as failure, changes only when the KEY file opens and writer runs, and PPV processing follows the KEY block unconditionally when local recovery rows are nonempty (`lines 286-332`). Yet the current public lifecycle cases cover successful KEY + failed PPV open, successful KEY + successful PPV rewrite, and root-only local gate; they do not combine KEY open failure with nonempty local PPV and a writable PPV target. `tests/s/test_save_io.cpp::native_save_continues_after_simctrl_open_failure` covers failed KEY open and later MST, but with no PPV input.

The one meaningful independent candidate acceptance case is therefore **KEY open failure must not suppress PPV writer attempt**:

- Use the existing public PPV lifecycle fixture/copy and a valid nonempty local PPV row.
- Hold the active SimCtrl.KEY with the test's established Windows sharing-denied-for-write handle while allowing the Open/read path to consume it; keep SimCtrl.PPV writable.
- Invoke the public Save conversion. Verify the KEY bytes remain unchanged, PPV is rewritten to the qualified normalized output, and the conversion still reaches MST output. Preserve the original fixture manifest.

This targets the explicit branch not covered by CP63 public tests and is reachable through the existing handle mechanism. It should avoid claiming the public API exposes the internal KEY boolean: `saveCurrentOperation` currently ignores `saveSimCtrl`'s return, so the test should assert the observable file effects and continuation. If the existing public fixture cannot hold KEY open in a read-compatible/write-denying sharing mode without blocking Open, use the direct `saveSimCtrl` harness with a nonempty `recoveryB` and an instrumented `beforeNativeKeyWrite` callback; the callback receives the PPV path after the failed KEY open and before PPV open. Do not add a broad fault seam unless both host-level setups prove unreliable.

## Failure branches already settled / still unqualified

The sink tests at `tests/mdt/test_simctrl_ppv_text_device.cpp` cover failed-open/no stream, injected first text write failure, CRLF write failure, and flush failure. They check writer result and event order, with no deterministic physical residue claim. `analysis/resume57-ppv-writer.md` and `analysis/resume59-ppv-save-next.md` qualify the original true return after successful open despite ignored stream/write/close status. There is no independent acceptance value in adding another candidate-only fake-device fault at those same boundaries. Exact post-open bytes on actual Windows failure remain unqualified and should not be asserted.

The only work in this checkpoint is this analysis correction. No source, test, CMake, build, GUI, or mutable REA change was made.
