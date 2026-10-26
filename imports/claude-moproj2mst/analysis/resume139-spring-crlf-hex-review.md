# CP139 independent review: one-row SPRING2 `0x\r\n`

## Result

No blocker found. The candidate admits only count-one `0x\r\n` through whole-file equality, returns public zero and records cursor offset 2, where CR remains unread. Retained Qt analysis supports a failed conversion with that ungot byte and fresh status 2. The candidate intentionally models only the public flag, not QTextStream status, and the receipt is explicit that CR-specific behavior is inferred from the verified invalid-hex-following-character branch rather than a separate captured row or original-process run.

## Frozen source identity

The current files match the CP139 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `636cb27aef27e60f023676351710180c5db39fb2e91af6dbee409d234d39dea7` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `7c3b51170949ed88a759fc63e1696a18e08a989983f31b183be2e8723690c670` |

## Findings

- The special predicate is `allowSingleRowQualifiedToken && bytes == "0x\r\n"`; the read loop sets that permission only for count one/index zero. The branch leaves cursor at `start + 2`. Because the entire-byte predicate requires start zero, this is absolute offset 2 (CR) for the exact fixture.
- The surrounding bare-prefix guard rejects any other matching `0x` followed by whitespace or EOF. Thus `0x \r\n` is still refused; the EOF-only `0x` case remains a separate CP138 predicate. Two-row input cannot get this permission, and the count-two refusal list explicitly tests `0x\r\n`.
- The one-row success test checks one returned row with flag zero and byte-exact unchanged SPRING and SPRING2 contents. Neighbor tests retain `0x ` with CRLF refusal and separate `0xG\r\n` handling. No status equivalence is asserted.
- Static Qt evidence establishes that recognized `0x` is consumed, then an invalid following character is ungot; for `0xG\r\n`, the retained table records offset 2 and status 2. Applying the same path to CR yields the CP139 cursor/status inference. The original one-row helper enters extraction because input is initially nonempty, copies the operator's zero, ignores conversion status, and returns after its one slot. This supports the public zero result only.

## Validation boundary

No source/test changes, build/test execution, GUI/original application interaction, or mutable REA operation occurred. Root owns integrated validation.
