# CP134 two-row `018` SPRING2 source review

## Result

The parser and test implement the qualified two-row cursor continuation: exact companion bytes `018\r\n` with `count == 2` cause the first read to return 1 and leave cursor at byte 2 (`8`); the second read uses the existing nonzero decimal path and returns 8. The tests assert `[1, 8]`, row count two, and byte-for-byte unchanged primary and companion inputs. The whole-file gate also excludes other counts and alternate spellings.

One stale source comment needs correction before freeze: `simcontrol300_native_io.cpp` around lines 250–251 says the `018` profile is limited to one requested flag and that a subsequent extraction at `8` remains outside the admitted reader path. CP134 now explicitly enables the second extraction for the exact two-row profile. The helper comment should distinguish the one-row token-only case from this new two-row continuation; otherwise it contradicts the implemented and tested behavior. This is comment-only; I found no logic defect.

## Gate and neighbor review

`readSprings` derives `exactTwoRowOctalContinuationProfile` only from `count == 2 && flagBytes == "018\r\n"`. That boolean is passed to each flag extraction. At the first cursor, the exact `018` branch returns 1 and sets the cursor to `start + 2`; on the second loop iteration, the token branch no longer matches and decimal parsing consumes `8`. The loop stops after the second requested flag, so no third read from the trailing CRLF is attempted.

The focused test asserts exact two-row success and unchanged bytes. It refuses `1 018`, `018 1`, repeated `018`, and leading/trailing spaces; count-three use is separately refused. Existing one-row `018` remains covered by the one-row profile. These are candidate boundaries, not claims that the original rejects alternate forms.

## Evidence binding

The CP134 receipt `analysis/resume134-spring-two-row-octal.md` binds original DEF_GUI_MO SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64 SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The retained Qt evidence supports base-zero selection of octal for `01`, unread `8` after successful first extraction, and successful decimal reparse as 8. The SPRING2 loop increments and ignores conversion status until count or `atEnd()`. This is static PE/call-path evidence, not an original-process differential.

Reviewed source SHA-256 `8c5b765ff8a05f8a393b1dd39c67fc5a670688c16750b1a5060d9c0e793457f4`; test SHA-256 `c444ac4773bf11c2347ff6272bcfc6e050108a6d77508d87f1b1d180932c2f9e`. No build or test execution was performed.
