# CP141 independent review: private Type3 REGIONS QString value helper

## Result

No blocking defect found. The helper reproduces the qualified successful rewrite in order: remove through the first `#`, remove through the last `/`, remove through the last backslash, then truncate before the last `.`. It searches and edits UTF-16 code units, keeps exact delimiter matching, preserves null/empty state on a no-op, and returns a non-null empty QString after an actual deletion that empties the value.

It is not integrated into the parser or public Type3 path. `REGIONS` remains `RowBodyNotImplemented`, so this helper does not relax admission or imply row/file/error semantics.

## Frozen identity

The files match `analysis/resume141-type3-region-value-implementation.md`:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/ops/type3_region_value.hpp` | `e028b5407461940980eae25f51d69e3778ed89e386746d31ddde0b44632a2cae` |
| `source/apps/moproj2mst/src/ops/type3_region_value.cpp` | `8b33a6107a9ef88e21032e304cf709e950b137c2f4d43403b66d7997651eb7cc` |
| `source/apps/moproj2mst/tests/doe/test_type3_region_value.cpp` | `8fba5e54d3abea29d5c63f9eb1a6d3b4ddad2732b28475b3d066e9c77cdbd984` |

## Findings

- The implementation's first `find('#')` is equivalent to the native case-sensitive `indexOf` and `remove(0,index+1)`. It then runs `rfind('/')`, `rfind('\\')`, and `rfind('.')` on each successive intermediate value, matching the qualified native call order and last-delimiter behavior for path separators/extensions.
- `std::u16string` preserves UTF-16 code units; only the ASCII delimiters are searched or removed, so unrelated BMP and surrogate-pair units pass through unchanged. The test covers a surrogate pair around delimiters. Delimiter matching is exact/case-sensitive, consistent with the imported QString calls and their case-sensitivity arguments.
- `changed` tracks whether any native-equivalent mutation occurs. When no delimiter exists it returns the original `rowText`, preserving null versus non-null empty. If a delimiter mutation leaves an empty buffer, constructing `qt::String(std::move(value))` creates a non-null empty string, consistent with the test for `"#"`.
- Tests cover first hash, last slash/backslash, final-dot removal, combined sequential delimiters, surrogate retention, and null/empty results. The helper does not split `.OPT` lines, tokenize, append rows, touch an optimizer state, or handle native allocation-failure behavior.
- The separate scalar parser still returns `RowBodyNotImplemented` for `REGIONS`; static search shows the new helper is referenced only by its own test/declaration/definition, not by an application parser or public loader. No public gate was changed.

## Evidence and validation limits

`analysis/resume141-type3-regions-rewrite.md` binds original PE IAT slots to `QString::indexOf`, `QString::lastIndexOf`, `QString::remove`, and `QString::truncate`, and qualifies the sequential successful transformation. This is a source-qualified private pure helper, not original runtime parity or file-level Type3 acceptance. No build/test execution, source edits, GUI/original app interaction, or mutable REA operation occurred in this review.
