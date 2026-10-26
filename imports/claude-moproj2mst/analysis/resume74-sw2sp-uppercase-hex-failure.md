# CP74 SW2SP uppercase-hex conversion failure

## Implementation

The candidate now accepts only exact bytes `0xA\r\n` when `readInts` is called for one value on `SW2SP.DAT`, returning zero. This models the original caller's observable result: Qt's lowercase `0x` prefix selects the hexadecimal getter, uppercase `A` contributes no accepted digit, `operator>>(int&)` writes zero and marks stream status failed, and the sidecar helper ignores that status and returns success after QFile opens.

The branch is guarded by the existing `allowSw2spOneIntProfile` gate and requires the exact four-byte suffix from the current cursor through EOF. It does not change the valid lowercase hex path (`0xa` -> 10), uppercase prefix fallback (`0X...` -> decimal-prefix zero), or valid lowercase-hex delimiter behavior (`0x10x` -> 16 with `x` left unread). No general uppercase digit support was added.

## Evidence

Static qualifications are in `analysis/resume74-sw2sp-next.md`, `analysis/resume69-sidecar-int-next.md`, and the retained binary captures:

- Original QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`: `QTextStream::getInteger` recognizes lowercase `0x`; the base-16 loop's character tests accept digits and lowercase `a` through `f`, so uppercase `A` gives a zero-digit failure.
- `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`: the conversion-failure arm writes zero and sets stream status; the successful arm copies the parsed integer.
- `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`: `FUN_1410da690` initializes the destination, extracts requested values, ignores stream status, and returns true once QFile open succeeded. The Extra3D Open call requests one SW2SP integer; the binding is summarized in `analysis/resume67-sw2sp-locale.md`.

This is static control-flow evidence, not an original-process runtime comparison. Multi-value continuation after sticky stream failure remains unqualified and guarded.

## Focused coverage and preflight

The native-adapter test contrasts existing lowercase `0xa` -> 10, the new exact `0xA\r\n` -> 0, existing uppercase `0X10` -> 0, and existing `0x10x` -> 16 delimiter behavior. It also checks that the new case is not admitted for another path or a two-value request; bare `0x`, `0xg`, and bare `0xA` remain unsupported.

Static preflight reviewed the profile gate, exact byte/EOF check, and test scopes. `git diff --check` completed without diagnostics. No build or CTest was run; root owns integration validation.

## Frozen file hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `66DB74FDD9FD05A682AFFE012282644478F8D29C3C33A1AB4820F6483C7028C4`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `52A9C86334CD96956059455D0BA2FE39B54186C15545B34DFC139E6EC8EECFAB`
