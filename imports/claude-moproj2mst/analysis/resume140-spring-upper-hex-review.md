# CP140 independent review: one-row SPRING2 uppercase `0X\r\n`

## Result

No blocker found. The candidate has a count-one/first-flag whole-byte gate for `0X\r\n`, returns public zero and sets the cursor to offset 2, leaving CR unread. Retained Qt evidence supports case-folded prefix recognition and the invalid-following-character failure behavior. The CR-specific cursor/status conclusion is a static deduction from that verified branch, not a separate captured Qt table row. Candidate internal status remains unmodeled.

## Frozen hashes

The current source and test match the CP140 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `c4d45f3faee3f95dad0a2afa3c3ec9e0c295ca944660050b8b564c6129e77c1f` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `581dc8004b40aff72b1acbb13d3eed04ad9f6e57b6a310bddccadc6d3fdcb04` |

## Findings

- `exactUpperBareHexCrLfZero` requires `allowSingleRowQualifiedToken && bytes == "0X\r\n"`. The caller sets that permission only for count one/index zero. It returns zero and sets cursor to `start + 2`; whole-file equality ensures start is zero, so CR remains unread at offset 2.
- Qt's retained PE analysis calls `QChar::toLower` before prefix selection and hex validation. Thus uppercase `X` is recognized as the hexadecimal prefix. The verified lowercase `0xG\r\n` path consumes the prefix, ungets the following invalid character and leaves data remaining; applying that same path to CR supports offset 2 and status 2. This is explicitly a static inference, not a separate recorded Qt row or original-process differential.
- The candidate's prior lowercase `0x\r\n` path and EOF-only `0x` path remain separate. The one-row refusal list includes `0X` at EOF, `0XG\r\n`, and spaced forms; the count-two list includes exact `0X\r\n`. The focused success test asserts one row with flag zero and byte-exact preservation of both source files.
- No internal QTextStream status is represented or asserted. The evidence predicts status 2 because bytes remain, while the helper copies zero, ignores status and returns true for the one requested slot. This supports only the public flag result.

## Validation boundary

No source/test edits, build/test execution, GUI/original application interaction, or mutable REA operation occurred. Root owns integrated validation.
