# CP137 headless retained-output comparison using the CP136 freeze

## Comparison and result

Ran the opt-in B98 before/after comparator on a disposable copy of the retained `98_B_DS3F_before` project. This exercises the full comparable output set, including KEY side files and the complete MST, so it covers output bytes beyond the P07 header/main-block/trailer prefix. The frozen CP136 CLI returned exit code 0; all 29 comparable paths were present on both sides. There was exactly one raw difference: `Gear_blank.MST`.

| View | Bytes | SHA-256 |
|---|---:|---|
| Candidate raw MST | 2722 | `e5845ff130256e919f4193ccc73b2196f00f1de3b74c8beb0ab6ba3f9c4aae12` |
| Retained after MST | 2756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |
| Candidate with only the recorded candidate KEY path replaced by the retained historical KEY path | 2756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |

Each path occurs once in its respective MST, and the separately retained path-qualified copy is byte-identical to the retained MST. The raw difference remains visible and is not counted as raw parity.

## Binding and command

- CP136 source tree SHA-256: `c000b41b213b211b630134aad66390cc74fe211a29ea6d68c1787059d886bb3e` (3,485 files).
- Frozen source archive SHA-256: `6eb13b11193f5e6acb26602ca6d6d7d0051541c6811022004cb1343048f0dde5`.
- CLI SHA-256: `9c18675c7dbafacacd838eb659774f3163a72268f8a13a31645c9c6c5726732f`.
- `moproj2mst.dll` SHA-256: `1129ad201d35eac801cb78379e84d73f2a58c3f8905269f6bea21d64677eefc5`.
- `deformdb.dll` SHA-256: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfbae66b1fc1f419077e9577aa3`.
- Retained before project `Gear_blank.moproj` SHA-256: `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.
- Retained after `Gear_blank.MST` SHA-256: `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.

Command:

```powershell
python admin\accept_b98_before_after.py --candidate-version CP136 --freeze-receipt build\wave3-checkpoint136-verified\receipt.json --source-tree-sha256 c000b41b213b211b630134aad66390cc74fe211a29ea6d68c1787059d886bb3e --cli build\wave3-checkpoint136-verified\moproj2mst.exe --expected-cli-sha256 9c18675c7dbafacacd838eb659774f3163a72268f8a13a31645c9c6c5726732f --expected-moproj2mst-dll-sha256 1129ad201d35eac801cb78379e84d73f2a58c3f8905269f6bea21d64677eefc5 --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

The disposable run is `C:\PROBLEM\b98r\r136-527631d9`; no cleanup was performed. Receipt SHA-256: `1b17a4a8b28ef6773fc342e65eb99bf02059873e56fdf1f232d479bbd3efa486`. The receipt binds the executable/dependencies and source, records `--strict --codepage 1252 --write-save`, pins child `M2M_MO_INPUT_MODE=0`, confirms no original executable was launched, and records retained evidence unchanged. Candidate stdout/stderr were empty.

## Qualification

This is a candidate run against retained before/after bytes. It reproduces the known single path-bearing MST difference with the CP136 freeze; it is not a fresh original-session capture and does not establish the historical GUI action sequence. It does not clear any of the remaining historical DIFF rows or complete a public Open/Save/Close workflow. The six logged gaps and five other historical comparisons remain independently qualified.
