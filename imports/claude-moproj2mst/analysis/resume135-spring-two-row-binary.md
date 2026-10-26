# CP135 bounded two-row SPRING2 binary continuation implementation

## Change and exact profile

The reader now admits exactly two primary rows with the entire companion byte string `0b12\r\n`. The first extraction reads binary prefix plus digit `1`, succeeds with value 1, and leaves `2` unread. The second extraction parses the unread nonzero digit as decimal 2. The resulting flags are `[1, 2]`, with CR unread after the second extraction; both input files remain byte-for-byte unchanged.

This is gated separately from the existing one-row `0b12` witness by exact count and full-byte equality. Tests verify the exact result and refuse mixed/repeated tokens, leading/trailing whitespace, and count-three use. Other companion spellings and counts remain outside this slice.

## Source qualification

`analysis/resume135-spring-next.md` cites the independently recaptured, PE-byte-verified Qt integer and SPRING2 helper evidence in `analysis/resume83-sw2sp-parser-independent-review.md`, `analysis/resume119-spring2-normal-path.md`, and `analysis/wave2-save-diffs-300-support-1410da690.c`. The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

Qt case-folds the binary prefix before base selection. The retained numeric table states `0b12\r\n` produces 1, succeeds, and leaves byte offset 3 at `2` with fresh status zero. With a second requested extraction, the same stream now begins with nonzero digit `2`; the decimal path returns 2 and leaves CR at offset 4, again with status zero. The original helper loops while not atEnd and index<count, incrementing after each extraction, so it uses the unread suffix for row two and returns after count two. This is static PE/control-flow inference, not an original-process differential run. The values are small ASCII integers; no ACP, overflow or narrowing behavior is involved.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `0FB02C8A9F5857076F57AB2E921949A35F5C7DC741466AB480CCD938E27CBD7A`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `C671A56A72EB45D6644E2035C8149131A6F7E75AC2CB4852A5B2FA4E8DA290B4`
