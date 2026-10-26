# Checkpoint 65: PPV Save failure acceptance case

## Qualified native behavior and candidate path

The retained PPV writer body `0x140d4afe0` in `analysis/resume48-ppv-writer-receipt.txt` and summary `analysis/resume59-ppv-save-next.md` establish the relevant sequence: local QList empty gate; QFile WriteOnly|Text open; failed open returns false before QTextStream/payload; successful open emits count and rows, each terminated by `endl`; close then true return without checking stream/write/close status. The caller `0x1400597d0` runs PPV after SimCtrl.KEY regardless of the KEY result and ignores the PPV helper bool; its return is the KEY result.

Candidate `save_current.cpp::saveSimCtrl` follows this sequence at lines 286-332. It attempts SimCtrl.KEY first and preserves `r`; local empty skips PPV. For nonempty local recovery state it validates the bounded Operation/path/codec profile, invokes the path callback, opens `FileDevice`, then generates payload and streams lines through `writePpvTextSegments`. If the PPV open fails, `writePpvForOperation(...Failed)` yields `OpenFailed`, so no stream is entered; the caller ignores it and returns `r == 0`. Existing text-device tests already inject first text-write failure, CRLF-write failure, and flush failure and assert per-line suppression/close order plus successful writer return. They deliberately do not assert physical residue. Those tests exercise the sink, not `saveSimCtrl`'s KEY-before-PPV continuation.

## Smallest independent acceptance case

Add a public-boundary test around `engine::saveSimCtrl` (or a minimal Save harness) with a nonempty current local recovery list and a valid step/model:

1. Seed a preexisting `Operations/Task.../SimCtrl.PPV` file with known bytes and hold it with a Windows handle that allows read but denies write/delete sharing (`FILE_SHARE_READ`).
2. Leave SimCtrl.KEY writable and call `saveSimCtrl` with `nativeSimCtrl=true`.
3. In `beforeNativeKeyWrite`, record the callback target and confirm SimCtrl.KEY has already been written. The callback should receive the operation's `SimCtrl.PPV` path after the local gate.
4. Assert `saveSimCtrl` returns the KEY writer's success (`true`), the KEY reflects its successful write, and the PPV file retains its exact prior bytes because QFile-like open failed before stream creation.

This distinguishes public continuation from the existing isolated open-failure tests and proves the exact helper-result boundary. It should not require a fault seam: a same-host Windows sharing-denied handle is the established deterministic FileDevice open-failure fixture style in `test_save_io.cpp`. The assertion is about failed open preserving existing bytes, not post-open residue.

To exercise the less common “KEY failed but PPV still attempted” branch separately, lock SimCtrl.KEY against write while leaving PPV writable and assert the callback/open still occurs and the public result is false. That is useful but not needed if only one case is admitted; it can share the same harness with lock direction reversed. The primary acceptance case above demonstrates the most important open-failure ignore result while keeping KEY success visible.

## What remains unknown

No retained evidence or existing deterministic seam identifies a physical byte residue after PPV post-open short/failed writes or close-time errors. Existing fake-device tests establish call order and ignored status only. Keep exact residue unasserted; any request to validate the actual Windows file after a byte-range lock is a host/device property experiment, not a qualified original QFile guarantee. The static evidence does qualify that such errors do not change the original writer's true return after successful open.

No source, tests, CMake, build, GUI, or mutable REA change was made for this audit.
