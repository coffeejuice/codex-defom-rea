# CP141 Type3 `REGIONS` rewrite resolution

## Finding

The CP138 concern is resolved: the four IAT calls in the rewrite are **QString operations**, not QStringList operations. The parser edits a QString in place, then passes that derived QString with integer zero to the existing region append call. This closes a narrow private helper over an already-decoded row string; it does not close the file/codec owner, row error behavior, or public Type3 admission.

## Import and ABI binding

The immutable `DEF_GUI_MO_64.exe` hash is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The executable's PE import table maps these exact IAT slots to QtCore4_64.dll symbols:

| IAT VA | Imported symbol | Call-site effect |
|---|---|---|
| `0x141e05498` | `QString::indexOf(QString const&, int, Qt::CaseSensitivity) const` | Search for `#`, start index 0, case-sensitive (`r9d=1`). |
| `0x141e046a0` | `QString::lastIndexOf(QString const&, int, Qt::CaseSensitivity) const` | Search for `/`, `\\`, or `.`, start `-1`, case-sensitive. |
| `0x141e054e8` | `QString::remove(int, int)` | Remove a prefix from position zero through the selected delimiter. |
| `0x141e046a8` | `QString::truncate(int)` | Keep the prefix ending immediately before the last dot. |

These names come from parsing the original PE import descriptors/thunks, not from inferred mangling or the Qt export list. Register setup in `analysis/resume127-type3-opt-parser.asm` matches each imported signature.

## Exact successful transformation

The constants are separately NUL-terminated strings in the immutable PE: addresses `0x1427ba0a4` and `0x1427ba0b4` each produce `"#"`; `0x1427ba120`/`124` produce `"/"`; `0x1427ba128`/`12c` produce `"\\"`; and `0x1427ba130`/`134` produce `"."`. The duplicate literals are search-term and mutation-term temporaries, not distinct strings.

For each row line, the sequence at `0x140908ed2–0x140909228` is:

1. If the current string contains `#`, find the first `#` and call `remove(0, index+1)`, discarding the prefix through that marker.
2. Find the last `/`; if present, call `remove(0, index+1)`, retaining the suffix after the last forward slash.
3. Find the last `\\`; if present, remove the prefix through it, retaining the suffix after the last backslash.
4. Find the last `.`; if present, call `truncate(index)`, removing the dot and everything after it.
5. Call optimizer virtual `+0xa8` with `edx=0` and the resulting QString (`0x140909248–0x14090925a`).

Each step searches the string resulting from the previous step. The dot check and truncate are conditional on a dot being found. Therefore the normal-path pure string transform is closed for a supplied QString; it has no token-list insertion or join step. The prepared region append shape remains as documented in `analysis/resume131-type3-prepared-rows.md`.

## Smallest source cut

Add a private pure `deriveType3RegionValue(const qt::String&)` helper and tests for: no delimiter; first `#` removal; repeated `/` and `\\` selecting the last; final-dot truncation; and combined delimiters proving sequential order. The parser currently has a QString line and the code copies that value to the mutable receiver used by these searches, so this helper should take that row text directly. The returned value can then be passed to `appendType3RegionPreparedRow(state, derived)` in an already-tokenized sequence cut.

Do not infer malformed/short-row or allocation failure behavior from this success-path projection. The parser creates a split result before the rewrite, and the exact contribution of that result to other paths should remain as currently modeled until a file-level parser is considered. QFile `ReadOnly|Text`, QTextStream header/read order, encoding/codec profile, native optimizer ownership, and public ShapeOptim consumers remain separate gates. No public Type3 guard should be removed on this finding alone.

## Qualification

Read-only evidence: retained parser disassembly, original PE import table and string bytes, and existing prepared-row evidence. No source, build, test, GUI, original execution, or mutable REA database was touched. This report adds no native runtime parity claim.
