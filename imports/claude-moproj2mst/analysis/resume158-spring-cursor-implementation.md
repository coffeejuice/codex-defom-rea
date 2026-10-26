# CP158 bounded SPRING2 `q` / `?` cursor implementation

## Change

Added an exact-byte-gated malformed-character cursor profile to the private SPRING2 reader. It is enabled only for count-one `1q\r\n` / `1?\r\n` or count-two `1q\r\n` / `1?\r\n` / `q 2\r\n` companion inputs. After a valid decimal prefix, the reader returns that value and leaves the marker unread. If another requested extraction starts on the same `q` or `?`, it returns zero without advancing, modeling the original ignored-status retry. Initial `q 2\r\n` under the exact two-row profile likewise yields zero twice without reaching `2`.

All other malformed marker sequences remain guarded. The implementation does not model QTextStream status bits, change other suffix/radix rules, or change CP157 BOM decoding or negative-overflow arithmetic.

## Focused tests

Added a dedicated test for one-row `1q` and `1?` -> `[1]`; two-row `1q` and `1?` -> `[1,0]`; and two-row `q 2` -> `[0,0]`. Neighboring `1z`, `1q 2`, and `1??` remain refused under count two, while `q 2` remains refused under count one. Accepted and refused cases assert the primary and companion files remain byte-for-byte unchanged.

## Static preflight and evidence

- The bounded mapping follows independent review `analysis/resume158-spring-cursor-plan-review.md` and the retained Qt/caller analysis in `analysis/resume120-spring2-malformed-flags.md`.
- The profile is selected from the decoded companion bytes and gated by row count plus exact complete input spelling; unrelated count-two parsing remains unchanged.
- Confirmed the sole `readSpringFlagInt` callsite passes the new profile bit and checked all owned files for trailing whitespace.
- No build or CTest was run. No GUI or mutable REA state was used; root owns integration validation. This is a static value/cursor model, not an original-process differential.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `A90ACFA898C7B0AAAE8915D0131FD1ECACC8C53EB27E38771796AFBD1B17BBB3`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `EB1312CAE15E2870C01E320605EC78D18627F73DC720F67A1E23B4CF80381807`
