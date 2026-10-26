# CP139 bounded SPRING2 `0x` with CRLF implementation

## Change

The candidate admits exactly one requested companion flag with entire bytes `0x\r\n`. It returns public flag zero and leaves the candidate cursor at CR (offset 2). The focused test asserts successful read, zero flag, and byte-for-byte preservation of both input files. A subsequent focused CTest log identified a stale one-row refusal-list entry for this newly admitted exact fixture; that entry was removed. The separate count-two `0x\r\n` guard remains. The failing suite was not rerun in this lane.

The implementation explicitly does not model the original Qt internal conversion status. Retained PE analysis predicts status 2 for this case because the prefix is consumed, CR is ungot, and data remains. The distinct no-newline `0x` case retains its CP138 EOF-only path. One-row `0x \r\n` and count-two `0x\r\n` remain guarded, along with other nearby malformed spellings.

## Source qualification

`analysis/resume139-spring-next.md` gives the retained Qt basis. The independent PE-verified review `analysis/resume83-sw2sp-parser-independent-review.md` records that a bad post-hex-prefix character is ungot with the prefix consumed (`0xG\r\n` leaves offset 2/status 2), while bare prefix at EOF leaves offset 2/status 1. It shows the same getInteger invalid-following-character path; CR in `0x\r\n` takes that failure path and is ungot. Original helper `FUN_1410da690` is documented in `analysis/resume119-spring2-normal-path.md`: it enters extraction for initially nonempty input, copies the resulting zero and ignores stream status. The public flag result is static inference, not an original-process differential.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `636CB27AEF27E60F023676351710180C5DB39FB2E91AF6DBEE409D234D39DEA7`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `73CE76F149D14F3C8A988207AAAB840FA5A33D7194DAA72A987401FFC6A7E1F4`

