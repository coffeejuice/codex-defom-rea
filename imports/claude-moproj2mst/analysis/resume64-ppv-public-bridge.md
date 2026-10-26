# Checkpoint 64: public PPV Save bridge audit

## Finding

The public native Save path is already active in `save_current.cpp`; CP63 is an isolated duplicate writer seam, not a missing production path. `saveSimCtrl` writes/truncates `SimCtrl.KEY` first and records its return in `r` (`save_current.cpp:286-300`). It then gates PPV on nonempty `op.recoveryB` (`302`), performs the Operation/path/codec/owner preflight, calls `beforeNativeKeyWrite` with the PPV path, and opens `mst::FileDevice` (`304-317`). After open it calls `writePpvForOperation` for payload, splits payload CRLF rows, and passes those rows to `writePpvTextSegments` (`318-329`). That sink streams per line/endl, closes with the stream callback alive, and returns its post-open writer result without promoting stream errors. `saveSimCtrl` discards that result and returns only `r == 0` (`330-332`).

This order matches the source-qualified original boundary: `analysis/resume57-ppv-writer.md` identifies `0x140d4afe0` as the local-list gate, WriteOnly|Text open, count then shared rows then local rows, `endl` per record, and unchecked successful-open close/write status. The caller `0x1400597d0` runs after SimCtrl.KEY and ignores the PPV bool. `analysis/resume60-ppv-text-device-implementation.md` qualifies the existing text sink’s line/write/CRLF/flush/close behavior. The public code preserves KEY-before-PPV open, root-only local-empty no-open, failed-open/no-stream, and continued return of the KEY result after post-open stream failure.

## CP63 bridge is not a drop-in replacement

`writePpvTextWriterFile` takes `Point` vectors and itself opens a path; `writePpvTextWriterAfterOpen` also takes `Point` vectors and reserializes them (`simctrl_ppv_text_writer.hpp/.cpp`). Public Save already snapshots `Operation` owners and produces its qualified payload via `writePpvForOperation`. Replacing the current call with either CP63 entry point would require exposing/repeating the private `RecoveryEntry`→`Point` mapping and would either reopen the file or duplicate payload construction. That expands the patch and risks moving work across the original open boundary.

The exact public bridge is therefore already the correct minimal architecture: keep its Operation adapter preflight and payload creation, and keep the current post-open `writePpvTextSegments` call. If root wants to remove the duplicated CRLF splitting, the safe helper should accept the adapter’s already-generated payload (or prepared line spans) and only stream those lines to the already-open device. It must not open the file, reserialize `Point`s, alter the preflight/open order, or surface post-open write/flush/close errors. That is an optional deduplication, not a behavioral fix.

## Output alias guard

There is no direct PPV entry in the `sameFile` list at `core/convert.cpp:263-266`, but active PPV truncation is already guarded at the actual write boundary. `rejectNativeKeyAlias` compares any supplied target path to `outPath` (`convert.cpp:231-233`); `save_current.cpp:315` calls `fx.beforeNativeKeyWrite(beforeOpen.path)` with the derived PPV path immediately before constructing/opening `FileDevice`. Thus an active PPV/output alias throws before truncating PPV. The callback name is narrower than its use, but its comparison is generic. The guard happens after the preceding KEY attempt, consistent with Save order; it is a converter safety rejection, not native application behavior. No separate PPV alias patch is required for this active write path.

## Supported profile and limits

The operation adapter qualifies task-folder `SimCtrl.PPV`, nonzero caller-supplied ASCII-compatible codepage, signed-int count bounds, valid local/shared owners, and adequate flags length. Do not report the passed codepage as the original process locale. The writer preserves shared count/emission asymmetry when shared exists but flags are null. Public Open’s codec restrictions remain separately documented in `analysis/resume60-ppv-public-integration.md`; no full locale codec is implied.

Post-open partial physical bytes remain unqualified. Existing device fault tests can establish event order and that the helper result is ignored, but cannot support a deterministic residue assertion. No source/test/build/GUI/REA change was made for this audit.
