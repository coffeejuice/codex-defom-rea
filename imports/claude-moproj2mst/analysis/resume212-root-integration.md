# CP212 original CLI contract correction and continuation

## Retained observation

The one-shot six-function owned original run at
`analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z/`
passed independent raw review. All six live patches were E9 redirects over
exactly the first five bytes with unchanged qualified suffixes before the
only resume. Its same-invocation returns were duplicate lookup zero,
factory child nonzero, `SfMoModel` cast nonzero, Project nonzero, acceptance
AL zero, and outer loader zero. It then reproduced the earlier QtCore read of
`0x58`; the original v11.0.2 child exited naturally with `0xC0000005`,
exact handles closed, Windows and Frida found the PID absent, and the
27-file disposable fixture was unchanged. The three temporary release
assignments were restored to the disabled runner hash immediately afterward.
This is a complete capture of that **literal-argument** command, not a
completed Open→Save→Close workflow.

## Corrected `-f` contract

Independent exact-PE analysis of main's `-f` converter at RVA `11a0`
confirms four hexadecimal input characters per UTF-16 QChar. It scans each
group with `%x`, constructs a QChar and stores it in the output QString.
The earlier command passed the 34-character literal
`C:\PROBLEM\p202a\Gear_blank.moproj`. The converter allocated eight
QChars (`34/4`), then attempted nine loop stores because the loop condition
uses `4*i < 34`; its last group also reads past the logical input end.
Consequently the exact decoded runtime QString cannot be inferred from that
malformed argument. The acceptance-zero and crash are real for the command
that was run, but **do not establish** a valid B98 Open failure, a public
converter discrepancy, an original Save outcome, or an accepted Close.

The previous CP205/206/208/209 reports now carry explicit later
qualification notes. Source recovery and historical candidate tests remain
preserved. The CP211 frozen C++ candidate still passes 361 tests with six
logged gaps, one diagnostic difference and historical 93 SAME/five DIFF;
no public behavior change was made from the malformed CLI result.

## Selected successor

A fresh disposable 27-file copy at `C:\PROBLEM\p213a` was verified byte
for byte against the immutable B98 source (`analysis/resume213-fixture-preparation.md`).
The correctly encoded path is 136 hexadecimal characters, each four-digit
group representing one UTF-16 code unit. Prepare a new fixed owned CLI Open
observation with the encoded argument, bounded runtime QString proof, outer
Open outcome and exact-child cleanup. A GUI event loop may persist; an
intentional termination of only that owned child would qualify cleanup but
not Save or Close. The original-run guard stays disabled until the new
contract, harmless fixture and independent review pass. User GUI control
remains gated by the unavailable release/status API.

At this safe boundary, `analysis/resume212-efficiency-review.md` measured
CP209 and CP211 full validation launches at 34.906 and 33.906 seconds;
their CTest portions were 17.782 and 16.953 seconds. Those single runs
support no claimed stable speedup. Token and active agent effort telemetry
were unavailable. Reuse source-bound passing receipts when no source changes,
and run one focused plus one full suite after an integrated source change.

`python admin/check_continuation.py` still reports 15 unfinished workstreams
and eight pending completion gates. The next checkpoint is an encoded-path
Open observation, not objective completion.
