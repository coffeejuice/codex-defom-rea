# CP136 headless B98 full-output comparison on CP135

## Result

The frozen CP135 product CLI completed the retained B98 before-to-after comparison successfully: exit code 0, 29 comparable paths on each side, and exactly one raw content difference. The differing file is the complete `Gear_blank.MST` (2,722 candidate bytes, SHA-256 `cef0c336d31dd0e7d944564ba750b267108da83e7df19c314b0e762335c0c5d0`; retained after-state 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`).

The candidate MST contains the candidate `Gear_blank.KEY` path exactly once; the retained output contains the original historical KEY path exactly once. Replacing that one path in a separate preserved copy produced a 2,756-byte file with the retained hash and byte equality. All other comparable files match. The raw difference is retained and remains a DIFF in the unnormalized view.

## Binding and execution

The repo's `admin/accept_b98_before_after.py` ran using only the frozen CP135 artifacts and copies of retained before-state input. The exact command was:

```powershell
python admin\accept_b98_before_after.py --candidate-version CP135 --freeze-receipt build\wave3-checkpoint135-verified\receipt.json --source-tree-sha256 b276306d0a82b132e0719ea4eccd7a9685b0b3ae9ba96ffd0b94f086912cfd6c --cli build\wave3-checkpoint135-verified\moproj2mst.exe --expected-cli-sha256 35ea156657492d189691b9e993a43f5e1c18fd3bb43ec6c480ef3b832e1548b7 --expected-moproj2mst-dll-sha256 83bee15db64e13392d023923b34bbd0f1ad62d26051e83e60f597eec1c75b893 --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

- CP135 source tree SHA-256: `b276306d0a82b132e0719ea4eccd7a9685b0b3ae9ba96ffd0b94f086912cfd6c` (3,480 files).
- Freeze receipt: `build/wave3-checkpoint135-verified/receipt.json`; product CLI SHA-256 `35ea156657492d189691b9e993a43f5e1c18fd3bb43ec6c480ef3b832e1548b7`, `moproj2mst.dll` SHA-256 `83bee15db64e13392d023923b34bbd0f1ad62d26051e83e60f597eec1c75b893`, `deformdb.dll` SHA-256 `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.
- Run root: `C:\PROBLEM\b98r\r135-98c8ea0f`; paths are generated under the short-path scratch root and checked by the script. Candidate input mode was explicitly pinned to `M2M_MO_INPUT_MODE=0`; the parent value was overridden.
- Run receipt: `C:\PROBLEM\b98r\r135-98c8ea0f\receipt.json`, SHA-256 `3a2869a8edb0373821e78304dc b848e94831b07fcdf8addf72ae5e9c5e6cb687` (formatting below corrected: `3a2869a8edb0373821e78304dcb848e94831b07fcdf8addf72ae5e9c5e6cb687`). It records exit 0, `acceptance_pass=true`, 29/29 paths, one raw diff, exact normalized equality, `retained_evidence_modified=false`, and `original_executable_launched=false`.
- Separate path-qualified MST copy: `C:\PROBLEM\b98r\r135-98c8ea0f\candidate.path-qualified.MST`, SHA-256 equal to the retained MST above. Raw stdout and stderr are empty; the script's JSON summary records the raw result.

## Qualification

This exercises a full candidate output tree, beyond P07's header/main prefix and trailer, against retained B98 before/after bytes. It can detect unrelated changes in the 29 comparable files for this input and confirms that CP135's only raw mismatch is the known embedded path difference. The one-path substitution is a separate derived view and does not erase the raw DIFF.

The retained after-state is historical output, not a fresh original Save in a newly observed GUI session. The check does not explain/replay the GUI action that produced it and does not establish fresh-original parity or close the overall acceptance gate. No retained fixtures, source, candidate executable, original GUI, or mutable REA state were changed or used; only the script's new disposable run tree under `C:\PROBLEM` was created.
