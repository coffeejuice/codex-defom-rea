# CP91 CP90 B98 rerun design

## Current freeze binding

The CP90 freeze is now present at `build/wave3-checkpoint90-verified/receipt.json`. It binds source tree SHA-256 `bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9` (3,379 files) and source archive SHA-256 `d74ef171f751e23814e492ad48c9c505d347a4fd691ae8da1eefde3c9b115f1a` to these verified artifacts:

| Artifact | SHA-256 | Bytes |
|---|---|---:|
| `moproj2mst.exe` | `fcc201e988d8e467060c56610305bedf8a7291e6bc38a48b5558e779977c0256` | 4,134,912 |
| `moproj2mst.dll` | `ac401824a4da8fe220029551b131fb69372da53213f338fad1c3078efa621435` | 4,140,032 |
| `deformdb.dll` | `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3` | 216,576 |

The frozen binary files currently match those receipt hashes. The frozen CP88 receipt and prior CP88 run receipts remain separate and untouched. `moproj2mst` has CMake project version `0.1.0`; `CP90` is the recovery checkpoint/build label, not a different product semantic version.

## Required harness change

`admin/accept_b98_before_after.py` is still CP88-specific: it embeds CP88 source and artifact hashes, and its CLI validation rejects any executable hash other than CP88. It cannot currently run CP90. Before a CP90 run, generalize only this admin script to accept an explicit checkpoint label, frozen receipt, source-tree hash, executable hash, and both adjacent DLL hashes. Validate that the supplied frozen receipt is CP90's receipt, its source hash equals the supplied source hash, and its three artifact records and files match the supplied hashes. Require the CLI path to resolve to the frozen `moproj2mst.exe` entry, not the mutable `build/candidate/bin` path. Record every supplied version/hash and the receipt path in the output receipt. Keep `M2M_MO_INPUT_MODE=0` pinned and recorded.

Suggested invocation after that change, using the current CP90 frozen values:

```powershell
python admin\accept_b98_before_after.py `
  --candidate-version CP90 `
  --freeze-receipt build\wave3-checkpoint90-verified\receipt.json `
  --source-tree-sha256 bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9 `
  --cli build\wave3-checkpoint90-verified\moproj2mst.exe `
  --expected-cli-sha256 fcc201e988d8e4670605c5ae23cb315cea88a1b14dc907f97c4c7e6897c0256 `
  --expected-moproj2mst-dll-sha256 ac401824a4da8fe220029551b131fb69372da53213f338fad1c3078efa621435 `
  --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

The executable hash above is transcribed from the verified CP90 freeze receipt as `fcc201e988d8e467060c56610305bedf8a7291e6bc38a48b5558e779977c0256`; the command form should be checked against the implemented parser before execution. If product version is also recorded, use the CMake version `0.1.0` separately from checkpoint label `CP90`.

## Preservation and acceptance behavior

Keep the existing CP88 receipts under `C:\PROBLEM\b98r\r-5957c441` and `r-10a8e762`; do not reuse their directories or rewrite their reports. Allocate another unique short disposable path for CP90, preferably labeled `r90-<8 hex>` while keeping generated project paths below 100 characters. The current script's no-cleanup behavior, source-copy verification, 29-path raw comparison, exact one-path normalization, and action-contract qualification should remain unchanged. A CP90 raw result may differ in size/hash due to its fresh unique path; acceptance should still require exactly the single `Gear_blank.MST` difference and byte equality only after the exact key-path replacement. Any other tree difference should be reported as a failed comparison, not normalized.

This is a static plan only. No candidate CLI was run and no source or binary was changed. Run only after root authorizes execution with the frozen artifact binding above.
