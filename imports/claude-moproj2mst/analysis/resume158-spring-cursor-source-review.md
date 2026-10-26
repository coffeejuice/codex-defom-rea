# CP158 SPRING2 `q`/`?` cursor source review

## Result

The released code matches the four exact acceptance rows in the CP158 plan, and no defect was found in those rows' cursor/result implementation. Frozen hashes match `analysis/resume158-spring-cursor-implementation.md`:

- Reader: `A90ACFA898C7B0AAAE8915D0131FD1ECACC8C53EB27E38771796AFBD1B17BBB3`
- Test: `EB1312CAE15E2870C01E320605EC78D18627F73DC720F67A1E23B4CF80381807`

## Verified behavior

The implementation sets `exactQQuestionMarkerProfile` only for these decoded companion spellings: count one `1q\r\n`/`1?\r\n`; count two those same spellings plus `q 2\r\n`. Under that profile, the no-digit branch returns zero without moving `cursor` when its current byte is q/? and the following byte is whitespace/EOF. After a parsed decimal prefix, the suffix branch leaves the cursor at q/? and returns the prefix. This reproduces `[1]`, `[1,0]`, and `[0,0]` for the exact planned table. The comparison occurs after BOM decoding, leaving the CP157 decoder and raw-file bytes untouched.

The focused test asserts byte-for-byte input preservation for accepted and refused cases. It keeps `1z`, `1??`, and `1q 2` refused, as well as `q 2` on a one-row primary. CP157 BOM and negative-overflow test cases remain in the frozen test file; no changes to their parser/decoder source branches were observed. No build or CTest was run.

## Contract boundary to resolve

The code implements an exact-whole-file witness profile rather than a reusable rule for “q/? is the first unconsumed invalid character.” This matters because the retained Qt behavior makes all bytes after an ungot q unreachable for the requested count: two-row `1q 2\r\n` is statically predicted to return `[1,0]`, just like `1q\r\n`, but the new test explicitly refuses it because the full byte string differs from the whitelist. Similarly, a one-row initial `q 2\r\n` would write zero and return success after open under the same no-digit/unget logic, but the implementation's exact profile excludes it and the test expects refusal.

If CP158's intended contract is only the four literal cases in its table, these refusals are an explicit safe scope reduction and not a defect in the selected cases. If it intends the documented character/cursor class, these are blockers: enable the first-invalid-character transition independent of an unreachable tail (still only for q/? and counts one/two), and add tests for the predicted outcomes. The receipt should not describe a generic q/? cursor branch until this distinction is settled.

## Limits

Read-only source/test review. The receipt evidence is static Qt/caller composition, not fresh original execution. This review makes no claim about status-bit parity or arbitrary malformed characters.
