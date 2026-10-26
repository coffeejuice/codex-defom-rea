# CP153 signed-leading-zero SPRING2 review

## Result

No blocking defect found in the frozen implementation. The new admission is scoped to a complete, explicitly signed, count-one token with at least two digits after the sign and a leading zero. It preserves unsigned leading-zero parsing and the existing refusal profiles.

## Source and test review

The frozen reader and test hashes match the implementation receipt:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `E1BD637DBE55A875C9BBED604663ECACD3599647AF77DBADCF0D365479B4AD8E`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `4C946D622384E5BD30DC9599D17998D98287167BA3A87D35D6A78749F7502508`

At the reader's numeric-prefix guard, `completeSingleRowSignedLeadingZero` requires `allowSingleRowQualifiedToken`, sign presence (`digitStart == 1`), more than one digit after the sign, leading `0`, and immediate whitespace or EOF after the digit run. Only that case bypasses the general multi-digit-leading-zero refusal. Conversion remains `std::from_chars(..., int, 10)`, with `+` removed first; therefore `-010` and `+010` yield decimal -10 and 10, while signed `int` range errors do not get routed into CP151/152's nonzero-leading overflow cases (their subsequent fallback predicates require `[1-9]` after the sign).

The unsigned `010` path remains separately covered as octal 8, alongside `02`, `017`, EOF termination, `017777777777` at `INT32_MAX`, and tab termination. The new tests cover `-010\r\n` -> -10, `+010\t` -> 10, leading-whitespace/EOF ` -0012` -> -12, and `+000\r\n` -> 0; they assert both source files remain byte-for-byte unchanged. One-row `-010x` and `+010x` remain unsupported. The count-two refusal matrix retains both signed forms, confirming the count-one gate. Existing unsigned-octal and partial-radix branches are not altered by this cut.

## Evidence boundary

This review checks source ordering and focused test intent only; no build or CTest was run. The parser behavior is consistent with the supplied retained Qt evidence that a recognized sign chooses decimal before unsigned leading-zero base selection. The implementation's ASCII whitespace/EOF boundary matches that bounded contract. No conclusion is made for locale-specific signs/spaces, non-ASCII digits, BOMs, malformed suffix recovery beyond the exercised cases, or multi-row signed-leading-zero values.
