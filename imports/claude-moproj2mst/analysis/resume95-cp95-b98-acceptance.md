# CP95 B98 retained-artifact acceptance

Ran the generalized acceptance script against the frozen CP95 CLI and the retained `98_B_DS3F_before` → `98_B_DS3F` artifacts. This is candidate-only; the protected original executable and GUI were not launched, and the retained fixtures were not modified.

## Candidate binding

- Checkpoint label: `CP95` (a recovery checkpoint, not a product-version claim).
- Freeze receipt: `build/wave3-checkpoint95-verified/receipt.json`, SHA-256 `52f5f615bbf50cb2622af786a6d4ebd7392695bfd60d8f8278d4cbf267708764`.
- Source tree: SHA-256 `ec4212e45a75ad74f81f59e07f38a08811c03c1fbfc7f2b0cbe986f5b8e6dc3c` (3,386 files).
- `moproj2mst.exe`: SHA-256 `d0d770c6ee9208034516b78770127b5f3e6fd1f8af5f3b284030dead776cf9bb`.
- `moproj2mst.dll`: SHA-256 `3aaa38d2c20d1b821589d67edc9a8aca6d4d1d4c12bb076fdbb52fdf50436684`.
- `deformdb.dll`: SHA-256 `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.

The script validated the CLI and dependency hashes against both explicit parameters and the CP95 freeze receipt before allocating its run directory. The parent environment was set to `M2M_MO_INPUT_MODE=1`; the candidate process environment recorded the pinned value `0`.

## Comparison result

The run exited 0 and passed the acceptance contract. Candidate and retained after-state each had 29 comparator-visible paths. Every relative path matched, and the sole raw content difference was `Gear_blank.MST`:

- Candidate raw output: 2,721 bytes, SHA-256 `42a48184d865f1465a5d4a2f23ab935743cada14b9a8a16cf87a5170b416f947`.
- Retained after-state: 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.
- Exactly one candidate key path and one retained historical key path were present. Replacing only the candidate path in a separate copy yielded 2,756 bytes and SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`, byte-identical to retained MST.

Run root: `C:\PROBLEM\b98r\r95-49479e3d`; full run receipt: `C:\PROBLEM\b98r\r95-49479e3d\receipt.json`, SHA-256 `db36222e76a5747b0c812d44adcaedaf1d22c1d6c957c9418e07b1d4237f38c0`. It retains the raw MST under `p/Gear_blank.MST`, the separate path-qualified MST, stdout/stderr, copied-input manifest, complete output manifests and source/binary binding. Maximum path within the disposable project was 74 characters.

This confirms CP95 reproduces the retained B98 before-to-after artifact delta after one explicit path substitution. It does not replay or qualify the historical original GUI action, so it is not fresh-original parity.
