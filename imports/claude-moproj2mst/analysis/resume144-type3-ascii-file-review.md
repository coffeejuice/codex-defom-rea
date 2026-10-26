# CP144 review: private Type3 ASCII file adapter

## Result

The new adapter is bounded as described: it requires an absolute path, opens before mutating optimizer `value48`, reads bytes in binary mode, rejects incomplete reads as a distinct unsupported outcome, and only parses a BOM-free seven-bit file with LF line breaks, no CR/NUL, and a final LF. For admitted files it consumes the first line separately as the discarded-trim/base-10 header probe, then dispatches subsequent lines. REGIONS rows go through the CP143 sequence helper as raw line QStrings. EOF-short region blocks preserve prior scalar and row effects. Static use search finds no application/public caller of this private parser; the ShapeOptim Type3 owner refusal remains present.

One minor source-comment mismatch: `splitAdmittedFile` says it excludes “controls other than TAB,” but the predicate admits byte `0x7f` (DEL), since it excludes C0 bytes below `0x20` except TAB and rejects only bytes above `0x7f`. The CP144 report's stated requirement is seven-bit ASCII plus the listed CR/NUL/newline rules, which does admit DEL as ASCII. This does not violate the documented admission contract, but the comment should say “C0 controls other than TAB” or the implementation should reject DEL if the stronger all-controls rule was intended. Current tests do not cover DEL.

## Ordering and state checks

- Relative paths return `UnsupportedPath` before opening or setting `state.value48`. For absolute paths, open failure returns `OpenFailed` with `value48` unchanged. Successful open sets `value48=true` before reading bytes, matching the original open-success mutation order.
- After a complete successful read and byte-profile check, the first line is removed as the separate header. `asciiTrimmed(header)` is computed and discarded; `qt::toInt(header)` is called on the original header and its result is discarded. Body parsing begins only after that probe. Empty files map to an empty header/body and naturally end; header-only files have no body records.
- REGIONS count fields are parsed from the dispatch line; the header is advanced before passing the following raw strings to `appendType3RegionRows`. The helper consumes only the declared count, preserves order and prior appends, and returns an EOF outcome if supplied lines end before the count is met. The new file test covers a two-row block with count three and asserts both previously appended rows plus the earlier OPR_NO value remain.
- NUM_CON body slices are limited to the declared/requested count and available lines. A short slice returns `EndOfFile` after prior row appends; scalar parsing remains routed through the existing sequence helper. ENDDATA terminates dispatch and the test confirms later lines do not mutate state.
- Byte-profile/read failures occur before header/body mutation, except `value48` which correctly follows successful open. This differs from a hypothetical late native read failure after parsed prefix rows; the report and outcome name this unsupported boundary rather than claiming ordinary-EOF parity.

## Raw-row and public-path boundaries

The REGIONS path passes `lines` directly to `appendType3RegionRows`; that function passes each complete `qt::String` to `deriveType3RegionValue`, not a token from the split fields. The CP141 helper's delimiter order remains first `#`, last `/`, last backslash, final dot. The sequence test in the new file checks both combined delimiter behavior and that it stops at the declared count.

`parseType3OptAsciiFile` is only referenced by its focused DOE test in the inspected tree; its header calls it a “private candidate-only reader” and explicitly says it does not activate public Type3 loading. The existing `ShapeOptimClass::studyFolder`/`loadClassData` path still refuses the Type3 optimization-study owner as unmodeled. This file adapter does not repair or bypass that missing owner.

## Limits

The audit verifies the source against the bounded ASCII/LF candidate contract, not original runtime parity. It does not establish QFile `Text` translation for CR, BOM/locale codec behavior, unterminated final lines, stream read-error partial effects, or the Type3 optimizer owner/lifetime. No source edits, build, GUI, original-process execution, or mutable REA action occurred.

## Reviewed hashes

- Admission contract: `analysis/resume144-type3-file-admission.md` — `71B7B6F0DB71620EB121B05F18463DBF7197FBFEFDDE7CC9BA58A08A23CC8C87`
- Parser source: `source/apps/moproj2mst/src/ops/type3_opt_ascii_file.cpp` — `71B7B6F0DB71620EB121B05F18463DBF7197FBFEFDDE7CC9BA58A08A23CC8C87`
- Parser header: `source/apps/moproj2mst/src/ops/type3_opt_ascii_file.hpp` — `6E59F46B8ECF738CEF7035687893FBBCFA80B90A7EB8D9BB8999E015DE80542A`
- Focused file test: `source/apps/moproj2mst/tests/doe/test_type3_opt_ascii_file.cpp` — `B997240B13C226D77AA946ED238A2440B1DFD440B4DD8001272AA772C637A0A1`
- CP143 row sequence: `source/apps/moproj2mst/src/ops/type3_region_sequence.cpp` — `FFA5C09816B02E33859D617BC8CD84CDB9838EADD86209474331D7FD7E1D5EB1`
- CP141 row value transform: `source/apps/moproj2mst/src/ops/type3_region_value.cpp` — `8B33A6107A9EF88E21032E304CF709E950B137C2F4D43403B66D7997651EB7CC`

Root owns build and integration validation.
