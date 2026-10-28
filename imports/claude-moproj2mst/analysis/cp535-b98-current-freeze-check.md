# CP535 current-freeze B98 harness check

The updated B98 harness accepted the immutable CP535 receipt at
`build/cp535-verified/receipt.json` (SHA-256
`a6105c65fbd4e70355bcd56dd6f7096d17505f51a135ce7168a7c7907a414c49`)
and verified its source plus three frozen executable/library hashes before
launching the converter candidate. The self-check passed without launching it.

The bounded run completed with exit code 0 and `acceptance_pass=true`. Its raw
receipt is `C:\PROBLEM\b98r\r535-82dceaaa\receipt.json`, SHA-256
`da33ce8d84dd193c4e656d0c926712e37de78c90a916865e7395aa1d4c72e080`.
Candidate and retained after manifests each contain 29 paths. The raw
comparison retains one content difference, `Gear_blank.MST` (candidate 2,722
bytes versus retained 2,756 bytes); the harness's explicit single project-path
normalization makes that file equal. It preserves the raw difference and the
normalized copy separately.

This validates the new current-receipt binding and a candidate comparison to
retained before/after artifacts. It did not launch the original executable,
modify retained evidence, observe a fresh Save, or establish accepted Close.
