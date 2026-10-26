# CP157 SPRING2 BOM implementation review

## Result

No blocking source defect found. The reader opts into BOM decoding only at the SPRING2 companion callsite, uses the retained Qt signature precedence, and preserves the existing numeric parser and EOF-short zero-fill behavior. The current source/test hashes match the CP157 implementation receipt:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `8C1A89B9390E6803E38E66A7BB1B9742A6F87DF7A48433C80CFC90253D92B628`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `677005EE55152D933F5108DDBB0353F162E0B8E813DE13DA2264A913DC89A079`

## Source review

`readSprings` returns before opening/decoding the companion unless the primary `SPRING.INI` yields a positive supported count. The changed companion call is `asciiBytes(*companion, true)`; primary SPRING and other byte-reader calls omit the default-false option. B_WEIGHT still opts in only through its existing one-value `oneValueBWeight` condition. BOM-less high bytes pass through the shared `>= 128` rejection.

The decoder's recognized-signature order is UTF-32BE, UTF-32LE, UTF-16BE, UTF-16LE, UTF-8. This checks `FF FE 00 00` as UTF-32LE before the overlapping UTF-16LE prefix. UTF-16/32 payload decoding checks full code-unit alignment before reading units, computes each unit according to endianness, and rejects any decoded value above `0x7f`; UTF-8 payload bytes are also subject to the common high-byte rejection. Thus recognized BOMs do not admit malformed high bytes or a second encoded BOM as ASCII data. A truncated `FF FE` + one payload byte test confirms incomplete UTF-16 rejection. A UTF-16 non-ASCII unit test confirms the scalar guard.

After decoding, the existing SPRING2 loop receives the ASCII string, parses flags with the same count-driven route, keeps its special retry profiles keyed on decoded bytes, and stops at EOF with untouched pre-zeroed suffix values. The two-row tests exercise all five BOM forms over `1\t-2\t\r\n`, assert the expected flags, and verify the original primary and companion byte strings remain unchanged. A one-flag UTF-16LE companion confirms EOF-short `[1,0]`. The CP156 explicit-positive wrapping source and table remain present in the frozen file set; positive `+2^64`/`+2^64+1` and multi-wrap witnesses retain their expected results and byte-preservation assertions.

## Coverage note and limits

The generic decoder handles UTF-32 truncation and non-ASCII identically by width/alignment and the common code-point guard, but the new SPRING2 tests shown here directly exercise truncation and non-ASCII only for UTF-16. Adding one truncated UTF-32 payload and one non-ASCII UTF-32 unit would make those format-specific edges explicit; this is a coverage improvement, not a source blocker. UTF-8 malformed payload and second-BOM behavior are rejected by the common high-byte guard but likewise lack direct SPRING2-specific assertions in this change.

No build or CTest was run. This review establishes static source and test coverage only; malformed Qt decoder replacement/status and fresh original-process parity remain unclaimed.
