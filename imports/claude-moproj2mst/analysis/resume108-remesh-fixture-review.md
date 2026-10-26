# CP108 B133 remesh worker-fixture review

## Fixture validity and Open behavior

The CP107 plan's constructed overlay is coherent for a candidate Open/Save worker: B133_before is a retained single-project tree with one Sub_Project; seeding root-level `LOCAL_REMESH.DAT` as `DONE\n` plus `LOC_DEN_CTRL.DAT` as `FILTER_DISTANCE 7\nDONE\n` follows the current reader's source order. The reader scans the unmarked primary, rewinds/closes it, then loads the fallback. A completed read returns `loaded=true`; `loadRemeshAtProjectOpen` copies that to the same Session state's `enabled`, so a later Save reaches the enabled writer. This is candidate-path qualification, not proof that the original ever had remesh enabled for B133.

The four-case owner confirmed each run makes a fresh unique leaf from immutable B133_before and reseeds both files. This prevents the normal case's successful writer from contaminating the locked cases by replacing the unmarked primary with the candidate KEYWORD_DRIVEN save output. Locked cases retain a case-local handle through the child's stopped/terminal observation and manifest capture.

## Sharing lock and timing

The proposed handle opens the primary with `GENERIC_READ` and `FILE_SHARE_READ` before the conversion/Open starts. Candidate `NativeReadFile` requests `GENERIC_READ` with `FILE_SHARE_READ | FILE_SHARE_WRITE`, which is compatible with the held read handle, allowing the scan/rewind and Open-time fallback parse. Candidate remesh Save uses `CoggingFile`, which requests `GENERIC_WRITE` with read/write sharing and `CREATE_ALWAYS`; the existing handle's share mask excludes write, so the Save open is denied before truncation. The fallback is not locked and is only read during Open. This supports a deterministic candidate sharing-denial setup, with raw primary/fallback hashes checked before and after. It does not establish native runtime termination behavior.

Case 1 (ConversionNormal17) is a useful unlocked candidate control: open succeeds and the enabled remesh writer is expected to replace primary. Cases 2–4 use the locked primary. The receipt should explicitly record the lock acquisition before launching, case identity/path, `CreateFileW` desired access/share/disposition, and release after owned child retirement plus post-manifest capture.

## Path and manifest

The unique `C:/PROBLEM/r108_<pid>_<tick>_<sequence>` case leaf is the right containment shape. CP107's example leaf plus the longest fixture-relative path leaves comfortable margin under 100 characters; nevertheless, the worker should check **all** planned absolute input and generated paths in the selected leaf before copying/creating any files, not only the project and known outputs. The given CP107 path examples and existing task suffixes appear bounded, but the runtime path assertion is the reviewable proof for the concrete invocation.

Capture the complete 66-file overlay manifest after copying and seeding, before acquiring/launching the child; retain raw before/after entries plus explicit missing/error markers. The three terminal cases may leave different prefixes: project/MST output attempts precede remesh; the locked primary should retain its input bytes, while other files may already have changed. Never compare only the five historical outputs or infer that all unlisted files stayed unchanged. Each case's manifest must bind its own fresh source copy and overlay hashes. Parent diagnostics/protocol receipts in separate supervisor case leaves avoid mixing them with project bytes.

## Four-case interpretation and evidence boundary

1. FreshDefault, unlocked: expected normal candidate completion, no terminal request because file open succeeds, and enabled remesh output. It is a control, not a native parity case.
2. FreshDefault, locked: expected boundary publication and candidate fixed terminal endpoint. A `0xC0000417` child status is the harness's fixed candidate endpoint result; it is not execution of the original CRT invalid-parameter/default-handler path. Require terminal-publication and child-status receipts; no returned conversion marker is expected.
3. Unknown, locked: profile is queried, no terminal dispatch is authorized, and candidate returns the guarded unsupported result after the all-success project/MST prefix. Primary remains locked/unchanged.
4. Custom, locked: same no-dispatch boundary as Unknown, but a distinct profile receipt; returned unsupported is candidate policy, not an original status claim.

The normal case should capture whether the service profile was actually queried (expected zero); cases 3/4 should record profile query once and dispatch zero times. Case 2 should record one authorized request and publication. Cases 3/4 are useful guard tests; they do not prove alternate CRT behavior.

## Finding and limits

No fixture invalidity found in the bounded static review, provided the worker keeps four cases on independent fresh copies and performs all-path length checks before writes. The primary lock is compatible with the candidate reader and blocks the writer as intended. Historical original B133 lacked these two controls; this is a deliberately constructed candidate overlay. The fresh-default terminal endpoint only emulates the selected candidate endpoint after publishing the live request. It does not validate original `TerminateProcess` behavior, original Save return/status, or original enabled-remesh history.

No source/build/test/GUI/original execution or mutable REA operation was performed. Inputs and native/candidate paths were inspected read-only.