# CP141 private Type3 REGIONS value helper

Added `deriveType3RegionValue(const qt::String&)` as a private supplied-value helper with a focused test file. It preserves native operation order: remove from the start through the first `#`, then through the last `/`, then through the last `\\`, then truncate before the last `.`. It uses UTF-16 code-unit search, as QString does; the delimiter comparisons are exact and case-sensitive, and non-delimiter Unicode code units are copied unchanged. A no-op returns the original value to preserve QString null-versus-empty state; a mutation constructs a non-null result, including when the result is empty.

Tests cover no delimiter, first-hash behavior, last forward/backslash selection, extension removal, combined ordering, surrogate-pair preservation around an ASCII delimiter, and null/empty behavior. The helper does not tokenize or read `.OPT`, append optimizer rows, alter parser/public guards, or claim native allocation-failure behavior. Root retains integration/build ownership.

Static preflight: `git diff --check` passed on the owned files. CMake source collection recursively includes `src/*.cpp`, and test discovery includes `tests/doe/test_*.cpp`; no CMake change is needed. No build or test was run, and no GUI, original process, or mutable REA action occurred.

SHA-256:

- `source/apps/moproj2mst/src/ops/type3_region_value.hpp`: `E028B5407461940980EAE25F51D69E3778ED89E386746D31DDDE0B44632A2CAE`
- `source/apps/moproj2mst/src/ops/type3_region_value.cpp`: `8B33A6107A9EF88E21032E304CF709E950B137C2F4D43403B66D7997651EB7CC`
- `source/apps/moproj2mst/tests/doe/test_type3_region_value.cpp`: `8FBA5E54D3ABEA29D5C63F9EB1A6D3B4DDAD2732B28475B3D066E9C77CDBD984`
