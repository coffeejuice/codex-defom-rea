# Checkpoint 67: PPV Save failure ordering after KEY-open coverage

## What CP66 establishes

`public_ppv_save_continues_when_simctrl_key_open_fails` exercises a KEY-open failure followed by a successful local PPV rewrite and later public conversion output. Its `M2M_OK` assertion is not the internal KEY writer status: candidate `save_current.cpp::saveSimCtrl` returns `r == 0`, but `saveCurrentOperation` ignores that boolean when it calls `saveSimCtrl` (see `wave3-native-simctrl-contract.json`: original `0x140057370` likewise ignores `saveSimCtrl` and proceeds to later operation writers). The CP66 file assertions are the meaningful observation: KEY stayed unchanged, PPV was rewritten, and Save continued.

Original `0x1400597d0` returns the KEY writer result while always reaching PPV after the KEY attempt when local rows exist. `analysis/wave3-native-simctrl-contract.json` further qualifies the native KEY writer: successful open assigns flags, invokes the banner/group writer and returns the group result; `fclose` is ignored. `analysis/resume48-ppv-writer-receipt.txt` qualifies the PPV writer's successful-open sequence and true return after close without testing stream/file status.

## Next uncovered failure ordering

The distinct next case is a **successful KEY open followed by a failed KEY flush/close, while PPV still writes**. It differs from CP66's failed KEY open: state updates occur after the successful open, the KEY stream can fail later, and `fclose` status is ignored. `tests/s/test_save_io.cpp::native_simctrl_failed_open_skips_model_flag_mutation` already has the same-host byte-range-lock setup for KEY: after the lock is installed, `saveSimCtrl` succeeds, the model flag reflects the successful open, and the key file is empty after the failed flush. That test has no nonempty `recoveryB`, so it never checks the PPV call that follows this failure.

The smallest acceptance extension belongs at the direct `saveSimCtrl` boundary: set a valid nonempty local `recoveryB`, keep PPV writable, and use the existing KEY byte-range lock pattern. Assert the key-writer/model-state result already covered there, plus that PPV reaches the normalized local-row output and `saveSimCtrl` keeps the KEY result. Do not claim original physical residue for the failed KEY flush beyond what the existing candidate test observes; native evidence qualifies ignored `fclose`, not this particular fault injection's on-disk bytes.

An alternate, more PPV-specific composition case is a first PPV write failure after successful PPV open. The `beforeNativeKeyWrite` callback in direct `saveSimCtrl` is invoked with PPV path immediately before FileDevice open; a test callback can open a sharing-compatible handle and acquire byte-range lock zero there. The retained `resume48-project-lock-probe.md` shows this host's FileDevice-style open/truncate succeeds under an exclusive byte-zero lock and subsequent `WriteFile` fails with `ERROR_LOCK_VIOLATION`. This would exercise the actual save helper + sink composition while leaving Open/PPV parsing unaffected. Assert the helper continues/returns the KEY result and makes the later call path; do not assert PPV residue. This alternate setup probes a different gap than CP66 and is more targeted if the goal is PPV post-open error handling rather than KEY close handling.

Existing `test_simctrl_ppv_text_device.cpp` fake-device cases already cover first text write, CRLF write, flush failure, sticky suppression of later rows, close notification ordering, and the native true writer return. They do not make another low-level sink-only fault case valuable. Neither candidate extension was implemented or run here.

No source, test, build, GUI, or mutable REA change was made for this analysis.
