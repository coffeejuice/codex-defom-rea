# CP71 implementation: exact SW2SP uint64 wrap token

Implemented the source-qualified one-int token `18446744073709551616\r\n` in `readDecimalInt`. It is enabled only when the caller requests exactly one value from logical path `SW2SP.DAT`. The branch consumes the decimal digits, leaves the CR/LF delimiter unread, and returns zero, matching the original QtCore 64-bit wrap of 2^64 followed by low-dword `int` extraction.

The change remains an exact-token exception. Neighboring `18446744073709551615` and `18446744073709551617`, a suffix-attached token, other file paths, SPRING count reads, and multi-value reads remain guarded. Existing positive/negative int32 overflow exceptions and parser guards are unchanged.

The adapter regression now covers the wrap token, adjacent positive/negative values, neighboring large magnitudes, suffix/path/count gates, SPRING counts, and multi-value requests. Static preflight reviewed the string length/boundary checks, the explicit one-value SW2SP gate, branch order, delimiter advancement, and test expectations. No build or CTest was run; root owns integration validation.

Frozen SHA-256 values:

- `src/engine/simcontrol300_native_io.cpp`: `DDFA2199522D6AB9A892C101F9444034AFB0C88425A8F3B667321BC9144578B7`
- `tests/mdt/test_simcontrol300_native_io.cpp`: `B49110442CD496B90394163448ED26070BE684DF0352BABF3FCECB38A6AD463C`

No shared files, CMake, GUI, or mutable REA state were changed.
