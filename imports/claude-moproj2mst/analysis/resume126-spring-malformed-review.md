# CP126 independent review: one-row SPRING2 `0xG`

## Finding

The frozen candidate branch is source-consistent for its declared narrow case: a one-row `SPRING.INI` with a readable ASCII `SPRING2.INI` whose first requested flag is exactly `0xG` followed by whitespace or EOF. Qt consumes `0x`, leaves `G` unread, stores zero on failed `operator>>(int&)`, and sets a conversion status; original `FUN_1410da690` ignores that status and completes its single requested extraction. The candidate returns flag zero while preserving both input files. I found no defect within this scope.

This is a static source qualification, not a fresh original-process comparison. It does not establish outcomes for a second requested extraction, other invalid letters, valid broader hexadecimal forms, or arbitrary suffixes.

## Native order and behavior

The independent PE review in `analysis/resume83-sw2sp-parser-independent-review.md` binds the Qt 4 captures to QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its verified `0xG\r\n` row says: getter failure, next unread byte offset 2 (`G`), integer extraction destination zero, conversion status 2.

The Qt trace is specific: base-zero parsing recognizes the `0x` prefix; `G` fails hex-digit classification and is ungotten without rolling back the consumed prefix. The integer extraction operator writes zero and records conversion status. The retained `FUN_1410da690` path in `analysis/wave2-save-diffs-300-support-1410da690.c` zero-initializes the output array, makes one extraction per requested item while input remains and the count permits, ignores stream status, then closes and returns true after a successful file open. With count one, the requested extraction has completed; the unread `G` does not trigger another extraction. The parent therefore receives a successful companion-reader return and the single flag remains zero.

## Candidate boundary

`source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` recognizes the spelling only when the row count is one, the current slot is zero, and the bytes are exactly `0xG` followed by ASCII whitespace or EOF. It advances the candidate cursor through `0x` but leaves the invalid `G` conceptually unread, clears its comma state, and returns zero. Since the loop has only one requested flag, the candidate then completes. For two rows, the special case is disabled and the input remains refused; this is an intentionally unresolved retry profile, not a claim about native rejection.

`source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` has an exact one-row `0xG\r\n` case asserting successful parse, flag zero, and byte-identical `SPRING.INI` and `SPRING2.INI`. Nearby refusal fixtures (`0x`, `0xH`, `0xG0`, `0xA0`, `0Xa`, and two-row `0xG`) are candidate boundary checks only. In particular, Qt's case-folded hex parser accepts `0Xa` and broader valid hex; the candidate's refusal there must not be interpreted as native invalidity.

No build/test execution, GUI, original process, source edit, or mutable REA operation was performed for this review.

## Inspected source identity

- `simcontrol300_native_io.cpp`: `5F85C75A511CABA658873838E329C2C902C3930F3ABDE2117F3C4B9173139DBD`
- `test_simcontrol300_spring_reader.cpp`: `DDD75E47879CE4736A4BABB7856F12AE83EFF154492E37D10BE1E266442F264F`
- CP126 implementation receipt: `analysis/resume126-spring-malformed-hex.md`
