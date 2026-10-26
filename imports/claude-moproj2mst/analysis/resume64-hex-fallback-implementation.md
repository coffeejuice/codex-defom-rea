# CP64 implementation successor: API mismatch blocks requested change

## Review result

The requested uppercase/signed behavior is qualified for `QString::toInt(base 0)` in `analysis/resume64-hex-fallback-contract.md`. The current target, however, does not implement or call `QString::toInt` for the path being changed.

`src/engine/simcontrol300_native_io.cpp::readInts` (lines 185–199) calls `readDecimalInt(bytes, cursor)` on the byte stream returned by `asciiBytes`. The production read path is the reconstructed `Extra3DIo::readInts` sidecar reader. The retained original binding for this operation is `FUN_1410da690`, which constructs `QTextStream` on `QFile` and invokes `operator>>(int&)` for each requested item (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`). Its Qt getter is `QTextStream::getInteger` at `0x670C5210`, not `QString::toInt` at `0x6706C280`.

The existing candidate parser documents the distinction directly: `readDecimalInt` implements only the source-qualified lowercase stream `0x` profile, then bounded decimal/octal cases. The source comment explicitly keeps signed and uppercase forms guarded. A behavior change accepting `0X10` or `+0X10`/`-0X10` based on the QString conversion contract would silently graft the wrong Qt API's grammar onto the stream reader. That would conflict with the retained QtCore stream disassembly, which does not treat uppercase `X` as its hex prefix and routes sign-first input to decimal parsing.

## Decision

No source or test edits were made. The requested change is not applicable to this parser/call site. The earlier CP64 report establishes an isolated QString behavior but does not qualify a reachable call from this implementation. To make a legitimate implementation, first identify a live current-code call site that corresponds to the original `QString::toInt(base 0)` consumer and add the behavior/test at that separate seam. If the intended scope remains `SW2SP.DAT`/Extra3D `readInts`, retain the current guard pending original `QTextStream` locale/runtime qualification.

Current file hashes, unchanged by this review:

- `src/engine/simcontrol300_native_io.cpp`: `733D5DB9F3D8C86D2E5DDEA331C236B1838B014D9512B38274BB44090C7CDD63`
- `tests/mdt/test_simcontrol300_native_io.cpp`: `BF8C620142F0CBE6D85307EF62BA4568B7FBD75076CF70296863C9ED494D5A6E`

No build, test, GUI, or mutable REA action was performed.
