# CP124 B98 candidate-only comparison receipt

## Result

The exact CP123 frozen-artifact invocation proposed in `resume124-historical-diffs-next.md` completed successfully. It ran only the candidate CLI against a disposable copy of the retained B98 before tree; it did not launch the original application. The script returned `acceptance_pass: true`, CLI exit code 0, and 29 comparable paths on each side.

The raw comparison remains one content difference, `Gear_blank.MST`:

| Artifact | Bytes | SHA-256 |
|---|---:|---|
| CP123 candidate raw MST | 2,722 | `df8a9d9ded0d67470144b12f6d9e0d5865c3af613d5ed15e0ac43f161de6762e` |
| Retained B98 after-state MST | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |
| Separate exact-path-qualified candidate MST | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |

The raw candidate contains its one generated project-local path, `C:/PROBLEM/b98r/r123-2599ffb2/p/Gear_blank.KEY`. The retained file contains its one historical path, `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`. The script replaced only the former exact value in a separate output copy. That copy is byte-identical to the retained after-state MST; its hash was independently checked against the retained file hash. The raw candidate bytes were left unchanged. The historical folder comparator's B98 row must therefore remain a raw DIFF.

## Frozen candidate and run binding

The run used CP123 frozen artifacts, not the mutable build output. The freeze receipt records source tree SHA-256 `81e35c00599eb03f0351cb452f502eb442fa48208d91cdcbf28fd40ac41c68c5` and artifacts:

- `moproj2mst.exe`: `3a0528f5f2870b13c5f2795b5fa93df8ff53a11b3c894b147877ed976003aa97`
- `moproj2mst.dll`: `012e686b9358b83dea9e159d93f61c5dd417b1a5df15e620ca214ed0b9fa8dbc`
- `deformdb.dll`: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`

The candidate was invoked with `--strict --codepage 1252 --write-save`, child `M2M_MO_INPUT_MODE=0`, and cwd/project under `C:\PROBLEM\b98r\r123-2599ffb2\p`. The longest generated project path was 75 characters. The script's unique run directory contains the copied project tree, raw candidate output, path-qualified copy, stdout/stderr, manifests, and receipt; it performs no cleanup.

- Run root: `C:\PROBLEM\b98r\r123-2599ffb2`
- Durable receipt: `C:\PROBLEM\b98r\r123-2599ffb2\receipt.json`
- Receipt SHA-256: `167d96400ebf7c827b983502abb7837c4c550c0a3a20d4306b76d89d4cd4ec34`
- Candidate raw file: `C:\PROBLEM\b98r\r123-2599ffb2\p\Gear_blank.MST`
- Separate qualified file: `C:\PROBLEM\b98r\r123-2599ffb2\candidate.path-qualified.MST`

The retained after-state MST hash remains the known `e6998030...27c1`. The acceptance receipt records `retained_evidence_modified: false` and `original_executable_launched: false`. It also explicitly limits the result to candidate output versus retained before/after artifacts; the original GUI action is not replayed or newly qualified.

## Interpretation

This run closes the specific CP124 candidate-only evidence step: the latest frozen candidate still emits the project-root-dependent path, and every other byte of this B98 MST matches the retained historical artifact after the one exact path substitution. It does not convert the raw result into equality, revise the five-row historical DIFF count, or establish fresh-original parity. No source, test, fixture, or build was changed; no GUI or mutable REA state was used.
