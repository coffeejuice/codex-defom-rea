# CP218 B98 historical Save DIFF: raw-byte qualification

## Finding

The retained `B:98_B_DS3F` Save comparison is a one-file raw DIFF: `Gear_blank.MST`. Its payload is byte-identical after replacing one project-root-derived `Gear_blank.KEY` path occurrence in a separate diagnostic copy. The original and candidate raw files remain distinct. The implementation already follows the original source-qualified path rule, so this DIFF identifies no unimplemented B98 writer branch and supports no production change.

## Raw bytes

The immutable original `evidence/gui_mo/oracle_b/98_B_DS3F/Gear_blank.MST` is 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. The retained CP136 candidate `C:\PROBLEM\b98r\r136-527631d9\p\Gear_blank.MST` is 2,722 bytes, SHA-256 `e5845ff130256e919f4193ccc73b2196f00f1de3b74c8beb0ab6ba3f9c4aae12`. Both contain the sole KFREAD path value at byte offset 1338 (zero-based), after the marker at offset 1325.

| Bytes | Retained original | CP136 candidate |
|---|---|---|
| `[1338, 1418)` / `[1338, 1384)` | `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY` (80 bytes) | `C:/PROBLEM/b98r/r136-527631d9/p/Gear_blank.KEY` (46 bytes) |

The 34-byte file-size difference equals the 34-byte path-length difference. Replacing only the candidate path value in a separate byte vector yields the retained original's exact 2,756 bytes and SHA-256. This is a path-qualified diagnostic equality, not raw equality; the historical comparator correctly records one DIFF and applies no normalization. CP136 compared all 29 expected project paths with no missing or extra paths. `analysis/resume137-cp136-b98-full-output-comparison.md` binds those results to the frozen candidate and retained after-state.

## Source qualification and prior audits

The retained original contract in `source/docs/re/moproj2mst/s1-05/spec.md:76-95` traces Diestress writer `0x1402b5410`: it resolves the active subproject DB path, converts `.DB` to `.KEY` case-insensitively, then emits that value as KFREAD. The B98 input has a Type-0 `Gear_blank.DB` subproject. The Type-0 path goes through `0x1400a4f60`, which uses the opened project's path (`source/docs/re/moproj2mst/p17/spec.md:38-45`); the Type-4 special naming branch does not apply. Candidate `src/ops/diestress.cpp` and `src/model/paths.cpp` implement the corresponding current-project-path rule. Existing `tests/s/test_b98_kfread.cpp` checks the exact candidate key path and preserves raw output separately from its one-path diagnostic comparison.

This agrees with CP116 (`analysis/resume116-b98-path-source.md`), CP137's full-output comparison, and CP149's raw-difference audit and path-contract decision (`analysis/resume149-b98-gear-raw-diff.md`, `analysis/resume149-b98-path-contract.md`). Those qualifications explicitly do not establish a fresh same-root original/candidate Save pair, nor a new original session's exact Open–Save–Close lifecycle. They do establish why the two retained outputs differ at this path and why pinning output to the historical Oracle-B root would violate the original project-relative behavior.

## Smallest remaining decision and next action

There is no B98 source decision left to implement from this DIFF: the only differing field is already explained by a source-backed branch and existing candidate regression. The remaining evidence gap is fresh raw parity at the same project path. The smallest executable action that could close it is a matched original/candidate Save pair using the identical pre-save B98 tree at one short disposable path, preserving each raw MST. That action depends on a supported GUI-control release operation and verifiable release receipt; the prior qualification in `analysis/resume149-b98-path-contract.md` says that gate is unverified. Until that capability is established, retain the raw DIFF and its bounded path explanation; do not change the writer or comparator.

The CP217 negative-operation-ID directory experiment is unrelated to this Save-time KFREAD path decision. No fixture, source, build, original process, or GUI was touched for this audit.
