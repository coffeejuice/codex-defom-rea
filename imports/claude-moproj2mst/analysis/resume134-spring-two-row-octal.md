# CP134 bounded two-row SPRING2 octal continuation implementation

## Change and exact gate

The reader now admits the exact companion byte string `018\r\n` only when the primary file contains two rows. The first extraction reads octal `01` as 1 and leaves `8` unread; the second parses that nonzero character as decimal 8. The resulting flags are `[1, 8]`, and both input files remain byte-for-byte unchanged.

Admission uses a whole-file predicate requiring `count == 2 && flagBytes == "018\\r\\n"` plus the token branch at the initial cursor. Tests cover this exact profile and refuse count-two mixed spellings, repeated tokens, leading/trailing whitespace, and count-three use. The existing one-row `018` path is preserved. These are candidate scope boundaries, not original rejection claims for all alternate forms.

## Source qualification

The behavior is documented in `analysis/resume134-spring-next.md`, based on retained Qt 4 PE evidence in `analysis/resume83-sw2sp-parser-independent-review.md` and the original `FUN_1410da690` call path in `analysis/resume119-spring2-normal-path.md` / `analysis/wave2-save-diffs-300-support-1410da690.c`. The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The `getInteger` trace shows base-zero selection by the digit after a leading zero (`0x670c5417–0x670c5448`): `1` selects octal, so `01` is consumed as value 1; the following `8` is not octal and is ungot with successful extraction. At offset 2, `8` begins a fresh nonzero decimal extraction and yields 8, leaving CR at offset 3. Both extractions succeed with status zero. The SPRING2 helper loops until count or `atEnd()`, increments after each extraction, and does not inspect status. These facts predict `[1,8]` for this exact two-row profile. This is static evidence, not an original-process differential run.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `34CA7BB60BB44FCF3A0FA580BFFF28A24D02C4B3F991B0B6086D136EE95B38D8`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `C444AC4773BF11C2347FF6272BCFC6E050108A6D77508D87F1B1D180932C2F9E`


