# CP214 integration and successor

Checkpoint214 freezes the candidate B98 Open-only regression and separately
qualifies one bounded original postOpen occurrence. It does not complete the
converter lifecycle.

The original is the installed DEF_GUI_MO_64.exe v11.0.2 (SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`;
retained PE version resource null). `analysis/resume214-original-postopen-review.md`
independently checked the raw one-run receipt at
`analysis/resume214-original-postopen-2026-10-26T13-17-52-922Z`. The exact
decoded B98 path reached outer Open. The returned Project P was delivered to
postOpen, became current, and returned normally from activation with EDX low
32 bits zero; postOpen returned normally. Nine live patch shapes passed before
one resume. The held child was intentionally terminated with exit `0xe201`,
handles closed, helper exit zero and successful Windows plus Frida absence
checks. The fresh disposable clone gained only an empty `Lubricants` directory;
the source fixture remained unchanged. The observation remains instrumented.
It establishes neither every view/parser effect nor Save or accepted Close.

`source/apps/moproj2mst/tests/s/test_b98_open_only.cpp` adds a candidate-only
regression without Save. It pins the 27-file B98 project SHA, separates
`acceptedProject=true` from `openedOk=true`, and checks the entire copied tree
before and after Open. The candidate tree stayed unchanged. This difference
does not itself identify the original directory creator; the bounded static
candidate is documented in `analysis/resume214-lubricants-creator.md`.

The first full-suite attempt (`build/test-runs/1793020714239937300`) was
source-stable but had 13 test scratch creation failures from default sandbox
denials under `C:\PROBLEM`; those logs are retained and are not candidate
behavior failures. The identical source-bound suite reran with the required
filesystem access at `build/test-runs/1793020799270713800`: 362 registered,
355 clean pass, six passing with logged gaps, one passing diagnostic
difference, zero failures/skips/incomplete outputs and zero fixture changes.
The historical folder comparator remained 93 SAME/five DIFF. The full build
receipt is `analysis/cp214-b98-open-build.json`; the freeze and source archive
are `analysis/wave3-checkpoint214-validation.json` and
`build/wave3-checkpoint214-verified/`. Source fingerprint is
`beca11c78710470ced4caa78034ff98e8920a47b60eabaead468e76e32796953`.

The successor is CP215: bind original diagram/view binding and XML/parser
state after this activation chain, then compare those exact boundaries with
candidate source. Public Save, accepted Close and other recovery guards remain
unresolved. `python admin/check_continuation.py` still returns `CONTINUE` with
15 workstreams and eight completion gates.
