# CP149 B98 `gear_blank.mst` retained raw-difference audit

## Result and direct byte comparison

Read-only comparison used the retained historical original at `evidence/gui_mo/oracle_b/98_B_DS3F/Gear_blank.MST` and the retained CP136 candidate at `C:\PROBLEM\b98r\r136-527631d9\p\Gear_blank.MST`. Neither file was edited. The hashes and lengths match the stored CP136 comparison receipt:

| File | Bytes | SHA-256 |
|---|---:|---|
| Historical original | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |
| CP136 candidate raw | 2,722 | `e5845ff130256e919f4193ccc73b2196f00f1de3b74c8beb0ab6ba3f9c4aae12` |

Both place `KFREAD` at byte offset 1325 and its path value at offset 1338 (zero-based). The differing field is the one absolute key-file path on line 30:

- Original bytes `[1338, 1418)`: `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY` (80 bytes).
- Candidate bytes `[1338, 1384)`: `C:/PROBLEM/b98r/r136-527631d9/p/Gear_blank.KEY` (46 bytes).

The remainder of each line and file is byte-identical after replacing only the candidate path value in a separate diagnostic byte vector with the historical path value. That transformed vector is 2,756 bytes, hashes to the retained original SHA-256 above, and compares exactly equal to the original. Raw files remain different; this is a path-qualified diagnostic, not raw equality. The 34-byte total length difference is exactly the path-value length difference. No other MST fields differ.

## Source attribution and L6 causality

The path difference is causally explained by the opened project root, not by an L6 save side effect. The retained source contract in `analysis/resume116-b98-path-source.md` traces original Diestress writer VA `0x1402b5410`: for this B98 Type-0 subproject, it derives the full DB path from the project, changes `.DB` to `.KEY`, and passes it to the `KFREAD` QString writer `0x1401344a0`. The candidate implementation follows the same project-derived rule. Retained setup/run evidence reviewed in `analysis/resume143-b98-key-path.md` binds the original output to the historical opened case directory; CP136 ran against its disposable `C:\PROBLEM` project directory. The original process CWD being `_tools` is not the path source.

`source/docs/re/moproj2mst/mdt/L6/spec.md` covers operation loader slots and Save callbacks plus L6-owned files/effects. It also says oracle B cases 98–102 have L6 active first and that `test_save_l6` compares L6 Save files byte-for-byte. That packet establishes L6 is exercised for B98; it does not explain this line. The differing bytes are the top-level MST `KFREAD` value. The packet's L6 Save-file coverage is distinct from packet S's operation MST writer and does not supply a competing path rule. No evidence links an L6 mutation to the project-root-derived path value, so L6 is not a supported cause.

## Comparator and behavior qualification

CP147's `m2m.s.save_folder` row is a historical diagnostic: one differing file for `B:98_B_DS3F`, no missing/extra, and `comparator_adjustments` empty. Its raw log names `gear_blank.mst`; the comparator does not normalize this path and correctly retains the DIFF. The separate exact-path replacement above is an audit view only, not a comparator adjustment or output rewrite.

These artifacts establish a historical original/candidate payload match after one environment-root substitution and explain the length/digest delta. They do not establish a fresh original Open–Save–Close result at the CP136 root, nor should they be reported as raw parity or general public behavior beyond the source-qualified path rule. Earlier CP112/CP124/CP137 audits already reached the same conclusion, so no source, test, build, GUI or REA change is warranted. The smallest next action is to keep the raw DIFF and its path-qualified explanation in the historical ledger; any stronger raw-parity claim would require a fresh paired original and candidate save at the same opened-project path, which is outside this read-only audit.

## Evidence used

- `analysis/resume147-tests.json`, `build/test-runs/1792946148326067900/m2m.s.save_folder.log`.
- `analysis/resume116-b98-path-source.md`, `analysis/resume137-b98-path-diagnosis.md`, `analysis/resume143-b98-key-path.md`.
- `analysis/resume136-b98-cp135-full-output-comparison.md` / retained CP136 receipt `C:\PROBLEM\b98r\r136-527631d9\receipt.json` (raw manifest and hashes).
- `source/docs/re/moproj2mst/mdt/L6/spec.md`.
