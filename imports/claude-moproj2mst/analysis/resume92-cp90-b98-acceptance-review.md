# CP92 review of generalized CP90 B98 acceptance

## CP90 run is correctly bound

The reported run root `C:\PROBLEM\b98r\r90-5f34b6e3\receipt.json` was inspected read-only. It records `candidate_version=CP90`, freeze receipt path `build/wave3-checkpoint90-verified/receipt.json`, receipt SHA-256 `1eea1efe2475f360e46baf3838441066bc7208ad89d280c95e2f4ff34fdeedd3`, and source SHA-256 `bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9`. Rehashing the referenced freeze receipt produced the same hash. The recorded EXE, converter DLL and deformdb DLL hashes match the CP90 receipt entries and the report's explicit values.

For non-CP88 versions, the script requires all four expected hashes and a freeze receipt. It checks the receipt path against `build/wave3-checkpoint<N>-verified/receipt.json`, validates its source hash, and requires the receipt artifact paths and hashes to match the supplied CLI and adjacent DLLs. The CP90 receipt's effective mode is `M2M_MO_INPUT_MODE=0` despite parent value `1`; exit code is 0, acceptance is true, and it records retained-evidence-unmodified/original-not-launched. It compares 29 comparable paths per side, one raw `Gear_blank.MST` difference, one occurrence of each KEY path, and a separate normalized MST that is byte-equal to retained output. The run root is unique and no delete/cleanup path is present. The report correctly limits this to candidate-vs-retained-artifact evidence.

The old CP88 invocation form remains accepted by parser defaults, with the CP88 source/artifact hashes used when those options are omitted. However, a compatibility/security regression remains in that default branch.

## CP88 default hash override is not constrained

For `--candidate-version CP88` with no `--freeze-receipt`, `required_values` does not require explicit hashes. But if an explicit expected artifact hash is supplied, `main()` overwrites the corresponding `CP88_ARTIFACTS` entry before calling `validate_cli()`. Since the no-receipt branch has no later equality check against the CP88 constants, any executable and adjacent DLL set whose hashes match the caller-supplied values can be accepted under the CP88 label. The earlier CP90 script's explicit CLI check rejected a supplied hash that differed from the CP88 frozen hash. The generalized report's statement that the prior default hashes remain in effect is true only when these optional override arguments are omitted or carry the CP88 values.

This does not invalidate the reported CP90 run: CP90 requires a receipt and every supplied hash/path was checked. It does mean the CP88 fallback can mislabel an arbitrary candidate as CP88. Repair by rejecting any CP88 no-receipt override that differs from the CP88 constants, or require a CP88 freeze receipt whenever any artifact/source override is supplied. No rerun is needed to establish the branch behavior; this is a static control-flow finding. No source, build, test or C:\PROBLEM artifact was changed during this review.
