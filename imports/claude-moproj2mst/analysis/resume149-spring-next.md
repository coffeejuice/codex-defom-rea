# CP149 next SPRING2 numeric edge: one-row octal prefix with unread `9`

## Selected bounded case

The smallest useful remaining leading-zero token cut is exact one-row companion bytes `019\r\n`, paired with a complete one-row ASCII `SPRING.INI`. Original Qt parses the octal prefix `01` as value 1, ungets the invalid first non-octal digit `9`, and returns getter success. Thus the integer operator writes flag 1, leaves the stream cursor at byte offset 2 (the `9`), and leaves fresh stream status at success. `FUN_1410da690` requests only one flag, so it does not retry at `9`; it returns success after the readable file was opened. The row's public flag is `[1]`. An unchanged Save sees a nonzero flag and the qualified writer emits decimal `1` in row order.

This is not the `08` branch. When the second digit after leading zero is 8 or 9, Qt's base selector chooses decimal; when it is 0–7, Qt chooses octal. In `019`, second digit 1 selects octal, then the later 9 is a stopping character after a valid octal digit. The already recovered `018\r\n` case demonstrates the same successful octal-prefix/unget shape with byte `8`; `019` follows the same verified loop for digit `9`.

## Evidence

- Original executable identity: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Original QtCore identity: `QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- `analysis/resume83-sw2sp-parser-independent-review.md` records the PE-verified base-zero decision: the second digit 0–7 chooses octal; 8/9 chooses decimal. Its corrected table gives `018\r\n` → 1, cursor offset 2 at `8`, successful getter/status 0.
- `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` qualifies octal dispatch at `0x670c57cc`, octal digit acceptance at `0x670c5832–5852`, and invalid digit unget at `0x670c585d–5860`; the accepted-digit path returns the accumulator successfully. Since `9` fails the same `<=7` digit test as `8`, `019` returns the same prefix value 1 with next unread offset 2 at `9`.
- `analysis/wave2-save-diffs-300-support-1410da690.c` shows one extraction per requested slot, status ignored, and unconditional index increment after the attempt. For a one-row SPRING record, no second extraction is attempted.
- `analysis/resume120-spring2-writer-order.md` establishes the downstream effect: a nonzero flag selects the companion write path and integer flags serialize as decimal in row order.

This is a static deduction from verified PE branches and caller decompile, not an original-process differential.

## Candidate gap and bounded next action

The candidate currently has a one-row exact `018` rule in `readSpringFlagInt` and explicitly refuses `019\r\n` in the one-row negative matrix. Its ordinary leading-zero guard rejects other multi-digit zero-prefixed forms. A minimal source cut is to admit only exact full bytes `019\r\n` under the existing `allowSingleRowQualifiedToken` gate; return flag 1 and set the cursor to offset 2, leaving `9` unread. Test the one-row result and byte-preservation, and retain refusals for the same token with count two, `019 0\r\n`, `0190\r\n`, leading whitespace, and other neighboring octal/malformed spellings. Do not infer a general octal parser or signed/prefix support from this one value.

A broader source-backed class could be considered later: valid one-row octal digit runs plus a first unread invalid decimal digit. The exact `019` cut is smaller and makes the cursor consequence inspectable while keeping existing count and byte-profile guards.

## Candidate bindings

- Reader: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `50BC8B56431F31E2B2D8EA005A7D6C55FF7B0D6DDFF161004E455E8CC22E3B58`
- Focused reader test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `58810845CF508ABA550EA26AA0BC8BD3359FDF89AD420A6A78E08CB902DCE27D`
- Corrected Qt table: `analysis/resume83-sw2sp-parser-independent-review.md` — `012507E626AA458EEC257D263CF1FF04BBBDF55CE2F334856BBCE3B33D539457`
- Writer ordering: `analysis/resume120-spring2-writer-order.md` — `AA90D258C00350BCF1DBDF57B37C7EF4A696D16CAFBFC4BB02A2BB5E88B5C896`

No source, test, build, GUI, original runtime, or mutable REA state changed for this read-only report. Root owns any source assignment and validation.
