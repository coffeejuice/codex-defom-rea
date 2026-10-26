# CP137 next SPRING2 continuation: one token plus trailing EOF extraction

## Selected exact profile

After the count-one `09` decimal fallback, the next bounded profile is a two-row primary file with the entire companion byte string exactly `09\r\n`. The first extraction follows the source-qualified decimal fallback and returns 9, leaving CR at byte offset 2 with status zero. Since CR/LF are still unread, the SPRING2 helper's `!atEnd()` loop enters a second extraction. Qt skips CR/LF, reaches EOF without finding a digit, stores zero and reports the EOF-side extraction status (internal status 1 when no previous error exists). The helper increments the second slot, reaches count two, ignores stream status and returns true. The expected flags are `[9,0]`; the second getter's terminal position is EOF after consuming CRLF.

Candidate currently admits `09` only under count one. A minimal test would use two complete ASCII primary rows and exact companion `09\r\n`, assert `[9,0]` and byte-for-byte input preservation. Keep mixed-token companions and count-three use outside this exact profile.

## Retained evidence and qualification limits

The first extraction is directly in the corrected `getInteger` table in `analysis/resume83-sw2sp-parser-independent-review.md`: `09\r\n` -> 9, successful, next unread offset 2 (CR), status zero. The same report source-qualifies the base-selection branch (`0x670c5417–0x670c5448`) that sends a second digit 9 to decimal base 10.

The companion helper `FUN_1410da690`, retained in `analysis/wave2-save-diffs-300-support-1410da690.c` and summarized in `analysis/resume119-spring2-normal-path.md`, prezeros all slots and extracts while `!atEnd()` and index<count, incrementing after each extraction without checking QTextStream status. The operator/getter evidence also documents whitespace skipping, EOF-side failure status 1, zero assignment on failure, and atEnd based on remaining buffer/device content. Therefore the second attempted extraction consumes the trailing CRLF whitespace, reaches EOF, stores zero, and completes the requested count. This combines retained method/caller evidence; it is not an exact two-call original-process capture and should remain described as static inference until such a runtime comparison exists.

The retained `resume119` EOF-short contract already establishes remaining pre-zeroed slots for EOF, but this fixture distinguishes that result from a loop iteration actually attempted because trailing whitespace remains when `atEnd()` is first checked. Do not extend this to arbitrary whitespace, malformed token failures, multi-value parsing generally, codec/ACP input, or other counts. Input is ASCII; no numeric overflow or ACP uncertainty is involved.

Source/test scope if assigned: existing `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` and `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`, with an exact count-two whole-file gate for `09\r\n`. No source/test edits, build, GUI, original process or mutable REA database activity occurred in this read-only selection.
