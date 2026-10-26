# CP162 SPRING2 BMP Unicode decimal digit implementation

## Implementation

Added a private SPRING2 companion path for explicit, well-formed UTF-16LE/BE
and UTF-32LE/BE files. It preserves ASCII bytes and represents each accepted
non-ASCII digit internally with a distinct marker, using the 23 retained Qt
BMP category-4 blocks (220 non-ASCII code units). The marker keeps radix
selection distinct: Arabic-Indic `٠١٠` parses as decimal 10; ASCII `010` stays
on the pre-existing octal route and parses as 8.

The new route is selected only when an explicit UTF-16/32 payload contains a
non-ASCII code unit, and only for one- or two-row primaries. It accepts full
signed/unsigned digit runs terminated by ASCII whitespace or EOF, with a
signed int32 result bound. Malformed suffixes, ASCII-zero-prefixed multi-digit
tokens in this route, non-ASCII whitespace, non-category numeric characters,
surrogates/supplementary scalars and count-three use are refused. The default
ASCII and existing malformed-token routes remain selected when there are no
non-ASCII payload units. No other sidecar decoder or public interface changed.

## Focused test coverage and preflight

The focused test exhausts all 220 non-ASCII category members under all four
UTF-16/32 byte orders as count-one values, then repeats them as count-two EOF
companions to check zero-fill. It also covers Arabic-Indic zero-radix
distinction, mixed Arabic-Indic/fullwidth decimal digits, signed two-row
extraction, ASCII `010` -> 8, category-boundary negatives, malformed suffix,
and byte-for-byte source preservation. Existing ASCII/BOM/sign/q/? and range
families remain in the same focused target.

Preflight: the test is a registered `TEST_CASE` in the existing mdt source,
uses the target's existing `testkit` include path and `../support/test_paths.hpp`,
uses only supported `REQUIRE`/`CHECK` macros, and contains no C `assert`.
`git diff --check` passed for the two owned files. No build, CTest, GUI,
original-process, live REA or mutable database operation was run; root owns
the source-stable integration build, tests, archive and freeze.

## Release hashes

This is a source release after the CP161 checkpoint archive recorded in
`analysis/resume161-root-integration.md` and
`analysis/wave3-checkpoint161-validation.json`.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `1F3DF812602C0CDA75B394750FFC523A2CF938E01CDB32B01449F4A032EF5313` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `3D294B1460B39A90AD3CD72F856A11FF230A00D1D02AD995C50B68290AE0D5BB` |

The immutable qualification and independent plan review are
`analysis/resume162-spring-unicode-digit-next.md` and
`analysis/resume162-spring-unicode-digit-plan-review.md`. These establish
static PE behavior only; original-runtime differential and public lifecycle
acceptance remain pending.
