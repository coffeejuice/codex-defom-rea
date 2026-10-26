# Checkpoint 63: isolated PPV payload-to-text-device writer

## Decision and scope

A narrow isolated bridge is ready for root validation. The original PPV writer’s post-open record and return behavior is already qualified; the missing seam was combining the structured `Point` payload serializer with the existing text-mode physical device adapter. This increment adds that bridge without wiring PPV into public Save.

The finite supported profile is explicit: caller-supplied locale codepage must be nonzero and marked ASCII-compatible; local/shared counts must fit the signed native count; when both shared rows and flags exist, flags must cover the shared rows. These are guards for this isolated adapter, not claims about malformed native state or the original process locale.

## Qualified ordering

The controlling original writer is `0x140d4afe0`, qualified in `analysis/resume57-ppv-writer.md` and `analysis/resume48-ppv-writer-receipt.txt` against the SHA-256-qualified `DEF_GUI_MO_64.exe`. It tests the local-list predicate before QFile construction/open. Once reached, it opens WriteOnly|Text, emits the combined count, then shared rows only when both shared and flags exist, then local rows with indices restarting at zero. Each record ends with `QTextStream::endl`. On a successful open, native closes the file and returns true without checking subsequent stream/write/flush/close errors; the caller ignores the PPV helper result.

`writePpvTextWriterFile` preserves that local-list short circuit before path/device construction and open. It delegates record generation to the existing `writePpvPayload` formatting path, splits its CRLF payload into rows, and delegates each row to `writePpvTextSegments`; that existing text-device sink emits the row and endl through `mst::writeTextMode`, closes while the stream callback remains attached, and reports a successful writer return after successful open even if a post-open write failed. No partial on-disk residue is claimed.

## Tests and limits

The new Release-active Testkit cases check byte-exact shared-before-local output and CRLF, local-empty no-open behavior, open failure without writer return, and an injected post-open first-write failure. The injected case checks the actual event sequence: first count-text write fails, then close flush and close notification still occur; writer return remains true and stream failure is recorded. It does not assert physical residue.

This does not establish the original runtime locale/codepage, general Unicode output, malformed codec behavior, exact physical partial-file bytes, or public PPV Save integration. The separate original call-order evidence remains controlling. No production public loader/Save, CMake/shared interface, GUI, mutable REA, build, or test run was touched. Root owns compilation and validation.

## Freeze hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/engine/simctrl_ppv_text_writer.hpp` | `17f2569036a70270f26d1878ddc65fe20dd87b0ff3c25c7291f5677428dc6cfa` |
| `source/apps/moproj2mst/src/engine/simctrl_ppv_text_writer.cpp` | `35c02727fb54de20cacc29e9918aaE78fe093fa116d818a53e28156c732db798` |
| `source/apps/moproj2mst/tests/mdt/test_simctrl_ppv_text_writer.cpp` | `273bde10506f38153522be1cde9892e583d0854538e8ef824c4742e8f4bce12f` |

Root integration repaired two Testkit/Windows test issues after the worker
freeze: a vexing-parse iterator string, a vector `CHECK_EQ` that Testkit
cannot stream, and closing the input handle before removing its scratch
directory. The integrated test SHA-256 is
`51d9e4ad94522a247e27d8da0945feeddb261ac8170fef8a062e607f54114037`.
The focused `m2m.mdt.simctrl_ppv_text_writer` target then passed all four
Release-active cases. The worker hashes above remain a historical handoff.
