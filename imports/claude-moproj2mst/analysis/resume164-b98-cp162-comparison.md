# CP162 frozen B98 candidate-only comparison

The immutable CP162 executable and adjacent DLLs passed `admin/accept_b98_before_after.py` against the retained B98 before/after artifact. The script verified the freeze receipt and all three artifact hashes before execution. It used unique disposable project `C:\PROBLEM\b98r\r162-a5684b28` and returned exit 0.

Both trees have 29 comparable paths. The raw comparison has exactly one differing file, `Gear_blank.MST`: candidate 2,722 bytes and retained after-state 2,756 bytes. The only identified difference is one absolute `Gear_blank.KEY` path occurrence in each file. Replacing that one path in a separate candidate copy yields 2,756 bytes with the retained file's SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. Raw files were preserved. The complete tool receipt is `analysis/resume164-b98-cp162-receipt.json` (SHA-256 `81c6ce6f2cf4b34aefa5936c83c45d3173a53e422864ee6e6e503518be9b6421`).

This is a refreshed candidate-versus-retained-artifact result, not a fresh original-process comparison. The historical B98 DIFF remains recorded, and path normalization is qualified to this exact occurrence. The other four historical DIFFs remain unresolved under `analysis/resume164-historical-next.md`.
