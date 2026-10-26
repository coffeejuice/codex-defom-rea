# CP92 CP90 B98 retained-artifact acceptance

## Frozen candidate binding

The generalized `admin/accept_b98_before_after.py` accepts an explicit checkpoint label, frozen receipt, source-tree SHA-256, EXE SHA-256, and both adjacent DLL SHA-256 values. It validates the receipt's checkpoint path, source hash, artifact paths, and artifact hashes before running. Existing CP88 invocation compatibility is retained through its prior default hashes. The generated short directory carries the checkpoint label (`r90-...`).

The run used only the CP90 frozen artifacts from `build/wave3-checkpoint90-verified/receipt.json`:

- Checkpoint label: `CP90` (product CMake version remains `0.1.0`).
- Freeze receipt SHA-256: `1eea1efe2475f360e46baf3838441066bc7208ad89d280c95e2f4ff34fdeedd3`.
- Source tree SHA-256: `bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9`.
- `moproj2mst.exe`: `fcc201e988d8e467060c56610305bedf8a7291e6bc38a48b5558e779977c0256`.
- `moproj2mst.dll`: `ac401824a4da8fe220029551b131fb69372da53213f338fad1c3078efa621435`.
- `deformdb.dll`: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.

## Run and result

The exact command was:

```powershell
python admin\accept_b98_before_after.py --candidate-version CP90 --freeze-receipt build\wave3-checkpoint90-verified\receipt.json --source-tree-sha256 bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9 --cli build\wave3-checkpoint90-verified\moproj2mst.exe --expected-cli-sha256 fcc201e988d8e467060c56610305bedf8a7291e6bc38a48b5558e779977c0256 --expected-moproj2mst-dll-sha256 ac401824a4da8fe220029551b131fb69372da53213f338fad1c3078efa621435 --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

The parent process had `M2M_MO_INPUT_MODE=1`; the child process used the explicitly pinned mode `0`. Candidate exit code was 0 and acceptance passed. The retained before-to-after comparator found 29 paths in each tree and exactly one raw content difference, the path-bearing `Gear_blank.MST`:

- Candidate raw MST: 2,721 bytes, SHA-256 `dcd2dc1919bb71deeb0454ba15ed6c5856aeb26c715806d8c6a421a9b2b6a3a7`.
- Retained after-state MST: 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.
- Candidate KEY path occurs once; retained historical KEY path occurs once. Replacing exactly the candidate path in a separate file yields 2,756 bytes and hash `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`, byte-equal to retained MST.

Run root: `C:\PROBLEM\b98r\r90-5f34b6e3`; complete receipt: `C:\PROBLEM\b98r\r90-5f34b6e3\receipt.json`. Raw candidate and path-qualified MST copies, stdout/stderr, manifests, and source/binary binding are retained there. CP88 run receipts/directories were not reused or changed. Retained fixture paths were not modified. The protected original executable and GUI were not launched.

This is a CP90 candidate reproduction against retained B98 before/after artifacts. The original GUI action sequence remains unqualified; this result is not fresh original parity.
