# CP125 bounded SPRING2 hexadecimal flag implementation

## Change

The companion flag reader now admits exactly the two qualified one-row spellings `0xA` and `0XA`, returning 10. It consumes the token only when followed by ASCII whitespace or EOF; the existing bounded parser handles all other inputs. The focused test checks both spellings, decoded value, and byte-for-byte immutability of `SPRING.INI` and `SPRING2.INI`.

Adjacent candidate-only refusals include malformed `0xG`, a broader valid value `0xA0`, and mixed-case `0Xa`. These are deliberately outside this exact-literal admission. The original Qt case-folding evidence predicts `0Xa` is valid too; its refusal here is a candidate coverage limit, not an original behavior claim. Overflow and unrelated malformed cases remain guarded.

## Source qualification

The contract is recorded in [resume125-spring-stream-next.md](resume125-spring-stream-next.md). It traces the original `FUN_1410da690` SPRING2 path to Qt 4 `QTextStream::operator>>(int&)` and `getInteger`. The retained QtCore evidence shows lowercasing before automatic `0x` recognition and hexadecimal digit classification. Therefore `0xA` and `0XA` both extract 10; the helper copies the integer and ignores stream status. The ASCII input and statically established C stream locale avoid ACP dependence. This is static PE qualification; no original-process comparison was performed.

## Validation and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `4CADDEB4AFAF65E64DCC8C879F2E6301A08023B40A6125F7B5165012BA1293A0`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `73A2FACEEB196B9E6B131C12179057DC0A7A502EC7B756F77EBCE4D361923E7D`
