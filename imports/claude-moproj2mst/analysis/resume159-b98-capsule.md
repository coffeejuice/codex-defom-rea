# CP159 B98 same-root comparison capsule

## Purpose and gate

This is a preparation capsule for a future matched B98 comparison. It does not run the original application, create `C:\PROBLEM` files, or claim parity. Current GUI validation status is `release_unverified` / `release_ready=false`; the local GUI lifecycle assessment records no supported release operation or verifiable release status. The acceptance queue says B98 raw equality needs original and candidate saves from identical pre-save bytes at the same exact short project path. The original comparison remains blocked until the required release API and receipt exist.

## Read-only inputs observed in this checkout

| Role | Absolute path | Bytes | SHA-256 |
|---|---|---:|---|
| Candidate test executable (prepared candidate input) | `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\candidate\bin\m2m_s_b98_kfread.exe` | 4,215,808 | `922599de1ec7dbb4eb2fb5416803c734a6762e285c8c4ba5e88a02d85d5924e9` |
| Retained historical output `Gear_blank.MST` | `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\oracle_b\98_B_DS3F\Gear_blank.MST` | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |
| Retained fixture `Gear_blank.KEY` | `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\oracle_b\98_B_DS3F\Gear_blank.KEY` | 2,291 | `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290` |
| B98 test source (reference only) | `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\source\apps\moproj2mst\tests\s\test_b98_kfread.cpp` | 3,209 | `350a1ebcc463bf9d9772e6ab3a5f1283eedc3f341fa551c729a04fcaf39ca4fb` |
| Prior raw-difference audit | `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\resume149-b98-gear-raw-diff.md` | 4,490 | `ead343390c8619fef5c3e6622fbe03aa8270cea6194b58ebfd4548961ab48215` |
| Original writer path qualification | `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\resume116-b98-path-source.md` | 5,614 | `fe2bb6de606cb2b5c6b083b2b1a562b9ce65e0131756242dbae68bafb32edad1` |

The fixture tree is the retained source for two byte-identical disposable copies. Its previously recorded 31-file sorted manifest digest is `1a9066f35f83705e0ecf881ec2f426bf749c1e5a3df6e083c49beb5c1d7957d2` (CP66 before/after). Do not copy protected original application files. The original executable identity/hash and a fresh original-save receipt at a disposable root are not present in the inputs inspected for this capsule; these are missing immutable inputs for an executable paired comparison.

## Exact disposable path plan

Use the short shared project root `C:\PROBLEM\b98x\p` (17 characters). Under it, stage one fixture copy for the original save, capture its raw output, then restore the exact same fixture bytes into the same path before the candidate save. Preserve the original output outside the staged project at `C:\PROBLEM\b98x\original\Gear_blank.MST` (39 characters) and candidate output at `C:\PROBLEM\b98x\candidate\Gear_blank.MST` (40 characters); all paths are below 100 characters. The generated key path in the project is `C:\PROBLEM\b98x\p\Gear_blank.KEY` (33 characters). Do not stage both trees simultaneously at different roots: the path embedded in `KFREAD` is the variable under test.

Before either save, record the sorted per-file SHA-256/length/path manifest for the staged project and require it to match the retained fixture manifest. After original capture, save the raw MST and hash it; restore the same manifest and run candidate capture at the same project path, then hash the raw candidate MST. Compare raw bytes and retain both files and receipts. No path substitution is allowed in the acceptance comparison; the old path-normalized diagnostic remains a separately labeled historical analysis only. Include the 29-file folder comparator output in the eventual capsule receipt, as required by `resume156-acceptance-queue.md`.

These are path names and acceptance conditions only. `C:\PROBLEM` exists, but `C:\PROBLEM\b98x` does not. No directory or file was created there.

## Limits and evidence

The previous evidence establishes that historical original and candidate MSTs differed only in the project-root-dependent `KFREAD` path, and that a separate in-memory path-substitution diagnostic matched the retained output. This capsule does not establish fresh raw equality, folder parity, or original runtime behavior. It makes no source/build/fixture changes and launches no executable. The GUI restriction is independently reflected in `analysis/gui-control-lifecycle-assessment.json` (`unsupported_release_api`) and the current `python tools/gui_validation_status.py` output (`blocked`, `release_ready=false`).

Capsule SHA-256: recorded in the task handoff (hashing the file with this self-reference excluded).
