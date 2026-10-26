# Resume 59: isolated PPV Open sequence implementation

## Added boundary

Added `simctrl_ppv::readPpvOpenSequence(project, path, operation,
hasPredecessor)` in the assigned new header/source. It composes the existing
PPV file reader and operation binding for the caller sequence after the KEY
branch. It does not modify or call public `loadSimCtrl`.

The helper reports five outcomes: `Applied`, `ParserReturnedFalse`,
`OpenFailed`, `UnsupportedInput`, and `OwnerUnavailable`.

- `Read` input is passed to `readPpvForOperation(opened=true, ...)`. The helper
  ignores neither native side effect nor result ordering: it always propagates
  the local plus optional shared point count to each present slot after the
  parser call, even when the parser returns false after partial mutation.
- `OpenFailed` calls the model binding with `opened=false`, preserving local,
  shared, and flag lists, then performs the caller's count update.
- Unsupported codecs or malformed input are refused before model or slot-count
  changes. This preserves a visible guard for file profiles whose native decode
  behavior is not yet represented.
- Missing model owners or an unrepresentable `int32` count are guarded as
  `OwnerUnavailable`.

Added `tests/mdt/test_simctrl_ppv_open_sequence.cpp` with Release-active
Testkit assertions for successful CRLF decoding and per-slot local count,
open-failure list preservation plus local+shared count, malformed-row partial
mutation plus count propagation, and unknown-codec refusal before mutation.
There is no C `assert` in the test. No public loader, shared model/session,
`readTextLines`, or CMake file was changed. The source CMake glob is expected to
discover the new source and test at configure time, but this has not been
verified by a build.

## Source basis and boundaries

Original caller `0x1400594d0` is preserved in the bounded PPV decompile receipt
`analysis/resume48-ppv-reader-receipt.txt`; parser clear/count/partial-row
ordering is summarized in `analysis/resume50-ppv-reader-model.md`. The
file-byte decoder profile and its static Qt qualification are in
`analysis/resume58-ppv-file-reader-utf16.md`. The native count alias is the
existing `StateVars::nrecvlCount` field at `+0xf0`, exposed by
`setPpvCount()`; `tests/mdt/test_ppv_count_state.cpp` verifies shared storage.

This helper receives the already selected `hasPredecessor` branch. It is not a
replacement for the original caller's KEY-existence early return, KEY import,
PPV existence check, or return of the KEY import result. It reads through the
existing `datafiles::readFileBytes` overlay/disk seam rather than a literal
QFile handle. BOM-free non-ASCII/locale decoding, non-UTF-16LE BOMs, malformed
UTF-16 effects, and exact QFile failure mappings remain unknown and guarded.

Public Open must stay guarded: current `save_current.cpp::saveSimCtrl` still
refuses a nonempty local PPV list because the paired SimCtrl.PPV Save writer is
not integrated. This isolated sequence is not a complete Open-to-Save feature.

## Freeze and validation

No build or tests were run; root owns validation. `git diff --check` passed at
freeze. SHA-256 of the three assigned new source/test files at freeze:

| File | SHA-256 |
|---|---|
| `simctrl_ppv_open_sequence.hpp` | `5ae46d68ac39dee50835074023cdd30303061e3f897426ae891ee4647e2d9c80` |
| `simctrl_ppv_open_sequence.cpp` | `59505820be62ac8de44da5da183c38b0a9ec3cdde454e73a6138f6419cdd8223` |
| `test_simctrl_ppv_open_sequence.cpp` | `8eea4f33902627e54f71fb827c6eff4ad4a3be58284357cd0f612b5cb072fa37` |
