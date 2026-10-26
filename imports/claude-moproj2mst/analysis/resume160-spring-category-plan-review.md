# CP160 SPRING2 sign-category plan review

## Result

The recognized ASCII sign/no-digit category is source-ready for the proposed one/two-row flag-value slice. The ordered sign-pair outputs, consumed-sign cursor, sticky status after a later successful extraction, and EOF-short second-row behavior match the retained QtCore control flow and the original companion loop. One exclusion should be made explicit at implementation: comma is a C-locale grouping character and does not follow the generic invalid-character transition. The plan already says comma/grouping remains outside scope; the source guard and tests must preserve that boundary.

This is a static review; no source, test, build, GUI, original-process, or mutable REA changes were made.

## Source and cursor verification

Bound original identities: `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The PE-qualified `getInteger` capture establishes the relevant transitions:

- The sign parser recognizes the C-locale `+` or `-`, chooses decimal, then the decimal extractor consumes the sign before counting digits (`0x670C545F–5497`, `0x670C54D0`, `0x670C5609–56AA`).
- With no digit, a second sign is not a digit and is ungot. Zero accumulated digits then produce getter failure (`0x670C56C0–56E7`, `0x670C5780–5793`, failure target `0x670C59A0`). A sign at EOF is consumed before the next getChar reaches EOF, with the same zero-digit failure.
- `operator>>(int&)` writes zero on failure, chooses status 1 at end or 2 while input remains, and only installs a nonzero status if the existing stream status was zero (`read-int` capture `0x670C6125–6152`). It still calls the getter on a later extraction when input remains (`0x670C60F4–6120`); a successful second value is stored low-dword without clearing the prior failure status (`0x670C6166–616A`).
- `atEnd()` observes unread buffered/device input (`atend` capture `0x670C2660–26CD`). The original `FUN_1410da690` companion loop requests another flag while not at end and below the row count, increments the row index regardless of extraction status, and returns true after open (`analysis/wave2-save-diffs-300-support-1410da690.c`).

This directly supports the core examples: `++2` with two rows gives `[0,2]`; `+-2` gives `[0,-2]`; `-+2` gives `[0,2]`; `--2` gives `[0,-2]`. The first failed extraction leaves the second sign unread and sets status 2. The next extraction consumes that sign, parses the digit, and succeeds while status remains sticky at 2. Count one ends after the first zero. A lone `+`/`-` at EOF yields a zero first flag and the outer loop sees end before another extraction. For `+\t2`, the first extraction leaves tab unread; the next getter skips it and reads 2. The candidate's caller-side whitespace skip may avoid modeling the final whitespace-only failed attempt for `+\t`, but public flag values remain zero; the plan correctly avoids claiming exact attempt-count parity there.

## Scope caveat: comma is not an ordinary non-digit

At `0x670C5706–575C`, the getter compares a non-digit to the active locale's group separator before ungetting it. For this stream the default numeric locale is C (`analysis/resume67-sw2sp-locale.md`), so comma takes a special grouping branch. The proposed rule “next character is EOF or a non-digit” would include `+,2`/`-,2`, even though the prose also says not to broaden comma behavior. Do not route comma through the sign/no-digit unget case. Keep `+,2` and `-,2` explicitly refused by this increment (or separately source-qualify the grouping transition); do not claim the original rejects them.

The test matrix should include comma as a negative candidate control alongside the existing sign/radix and three-row guards. The source rule should be stated as recognized ASCII sign followed by EOF or a non-digit **other than comma**, unless a separate grouping path is intentionally qualified. That keeps the transition from overlapping the distinct comma prefix behavior already present for `1,234`.

## Test-matrix assessment

The proposed 2x2x9 sign-pair matrix covers all ordered pairs and signed digit values. Count-one cases assert only the zero flag after the first consumed sign; count-two cases prove the second extraction is attempted despite sticky failure status. EOF and CRLF termination exercise both final-buffer states. Lone signs and sign/tab cases cover EOF status and caller whitespace handling. `+++2`/`---2` verify one sign is consumed per failed extraction; their count-two projection is `[0,0]` with the third sign still unread. Existing CP158 q/? no-advance behavior remains a useful contrasting control and must not be merged with sign consumption.

The plan's suggestion to move one-row `++1` from refusal to accepted `[0]` is correct. `+z`/`-z` under count one is also consistent with consumed-sign plus ignored unread marker, but is optional and must not enable initial z or count-two z retries. Count three, Unicode signs/digits, non-ASCII codec bytes, malformed signed radix prefixes, and arbitrary malformed markers remain out of scope.

## Reviewed artifacts

- CP160 proposal: `analysis/resume160-spring-category-next.md`.
- Qt evidence: `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, and `analysis/wave2-save-diffs-qt-textstream-atend-complete.asm`.
- Caller/locale evidence: `analysis/resume119-spring2-normal-path.md`, `analysis/resume120-spring2-malformed-flags.md`, and `analysis/resume67-sw2sp-locale.md`.
- CP158 cursor implementation: `analysis/resume158-spring-cursor-repair.md`.

No other blocking discrepancy was found in the CP160 flag-value projection. A standalone original-process extraction witness for `++2` would be useful differential corroboration, but it is not required by the retained immutable function evidence for the static claim.
