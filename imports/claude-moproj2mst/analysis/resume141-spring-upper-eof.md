# CP141 bounded SPRING2 uppercase bare-prefix EOF implementation

## Change

The candidate now admits exactly one requested companion flag with complete bytes `0X`. It returns public flag zero and advances the candidate cursor to EOF. The focused test asserts successful read, zero value, and byte-for-byte preservation of both input files.

This path does not model Qt's internal EOF conversion status 1. Exact `0X` under count two, `0X\r\n`, `0XG`, whitespace variants, and unrelated malformed prefixes remain guarded as before. The one-row `0X\r\n` case remains separately admitted with CR left unread.

## Source qualification

`analysis/resume141-spring-next.md` records the evidence chain. The independent Qt PE review `analysis/resume83-sw2sp-parser-independent-review.md` identifies QChar lowercasing before automatic base-prefix selection and records lowercase `0x` at EOF as failure, consumed prefix, offset 2/EOF, status 1. Applying the case-folded prefix branch to `0X` predicts the same EOF result; the exact uppercase spelling is a static derivation, not a separate captured table row or original-process differential.

The original SPRING2 helper `FUN_1410da690`, summarized in `analysis/resume119-spring2-normal-path.md`, enters extraction because the file initially has bytes, copies the operator's zero destination, ignores stream status and returns true after its one requested flag. Thus the public result is predicted to be zero. Original executable SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `AD1633CD791DCB541F33BB070741F72B4DF73B54B2BBD706323264832C37BB22`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `EE84FBF08D6BF6BD9E9718434E593E00543A5C696AC7BCF08D6B434122C7F444`
