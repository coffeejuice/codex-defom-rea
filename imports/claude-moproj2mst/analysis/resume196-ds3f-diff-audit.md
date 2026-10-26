# CP196 B:98_B_DS3F raw-difference audit

## Disposition

No evidence-backed source or test fix is ready. CP195's frozen `m2m.s.save_folder` output records `DIFF B:98_B_DS3F: 1 differ, 0 missing, 0 extra`, naming only `gear_blank.mst`; the comparator applies no adjustment. Its diagnostic sweep deletes generated scratch projects, so CP195 does not preserve the candidate bytes needed for a new byte-level attribution. The preserved CP162 candidate artifact was therefore used for the bounded byte inspection below and is identified separately; it is not represented as CP195 output.

The preserved raw difference is exactly the project-root-derived absolute `KFREAD` value. The original source-qualified writer and candidate implementation both derive the Type-0 subproject DB path from the opened project, replace `.DB` with `.KEY`, and serialize it. Rewriting that path or changing the comparator would contradict the established path rule and erase a valid raw difference. The existing B98 test already checks the project-owned path rule and keeps path substitution diagnostic-only.

## Byte evidence

CP195 frozen output is `build/test-runs/1792979918197143000/m2m.s.save_folder.log`; its frozen validation is `analysis/wave3-checkpoint195-validation.json`. The preserved comparison bytes inspected here are CP162's candidate `C:\PROBLEM\b98r\r162-a5684b28\p\Gear_blank.MST` and retained historical original `evidence/gui_mo/oracle_b/98_B_DS3F/Gear_blank.MST`:

| Artifact | Bytes | SHA-256 |
|---|---:|---|
| CP162 candidate raw MST | 2,722 | `384de848b4b6d5e5863ba451647d0a6dfe670874d35a94c87c40021ac7c7fc7a` |
| Retained historical MST | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |

Both contain `KFREAD` at line 29 (`KFREAD    \t1\n`). The single path value starts at byte offset 1338 in each file. The candidate's 46-byte value is `C:/PROBLEM/b98r/r162-a5684b28/p/Gear_blank.KEY`; the historical 80-byte value is `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`. The first differing byte is offset 1341. Each value occurs once; the 34-byte value-length delta exactly accounts for the 34-byte file-length delta. Replacing only the candidate path in a separate in-memory diagnostic vector yields 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`, byte-identical to the historical file. Neither raw file was changed.

## Source and capture qualification

`analysis/resume116-b98-path-source.md` documents the original writer chain: B98's Type-0 `SfOprDiestress` subproject resolves its DB path through the project, changes `.DB` to `.KEY`, then emits `KFREAD`. The candidate source follows that same rule. `analysis/resume143-b98-key-path.md` binds the historical output to the old opened case directory from the retained setup/run evidence; the process CWD `_tools` is not the path source. `analysis/resume149-b98-gear-raw-diff.md` and `analysis/resume164-b98-cp162-comparison.md` preserve the same path-only diagnostic qualification. The CP162 artifact's identity and path substitution are also recorded in `analysis/resume164-b98-cp162-receipt.json`.

The capture evidence qualifies the retained historical run and its old project root, but it does not provide a fresh original Save from identical pre-save bytes at the candidate's short `C:\PROBLEM` root, or establish all original UI/session state for a same-root replay. `analysis/resume159-b98-capsule.md` explicitly leaves that paired original comparison gated on a supported GUI-control release operation and verifiable receipt. Therefore a fresh raw-parity claim remains blocked by the original-session capture gate; this does not make a source/test correction appropriate. Keep the CP195 raw DIFF and existing path-qualified diagnosis. Do not tune the comparator or start GUI control for this item.

## Scope

Read-only audit. No source, tests, build, GUI, original executable, mutable REA database, fixture, parent state, DMO, or Claude files were changed or used. This report is the only file written.
