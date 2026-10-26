# CP90 B98 before-state candidate comparison

## Purpose and scope

This is a GUI-free candidate-only conversion of the retained `98_B_DS3F_before` project, followed by a complete tree comparison against the retained `98_B_DS3F` after-state. The protected original executable was not launched, and neither retained folder was written. This does not establish what exact GUI commands created the historical after-state.

## Inputs and identity

- Retained before fixture: `evidence/gui_mo/oracle_b/98_B_DS3F_before`.
- Disposable copy: `C:\PROBLEM\b98r\before-run`; the pre-run file inventory and hashes are in `C:\PROBLEM\b98r\before-input-manifest.json`. The retained fixture itself was not modified.
- Candidate project: `C:\PROBLEM\b98r\before-run\Gear_blank.moproj` (length 41); candidate-generated key path: `C:/PROBLEM/b98r/before-run/Gear_blank.KEY` (length 41). Generated project paths are under 100 characters.
- Frozen CP88 CLI: `build/wave3-checkpoint88-verified/moproj2mst.exe`, SHA-256 `eef23401ece560ca6405c5ae23cb315cea88a1b14dc907f97c4c7e689a4be373` (4,126,720 bytes). Adjacent DLL hashes: `moproj2mst.dll` `eed0f155561e8c95c46a77a046fcd3deb2ef23ac1770b767c05c3d76ab8401cd`; `deformdb.dll` `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.
- CP88 build source-tree SHA-256: `eacfe403abee631861befe6f9f35b274c9be1928af0e52f95e6e1e671c54cfef`, from `build/wave3-checkpoint88-verified/receipt.json` and `analysis/resume88-integrated-order-repair-build.json`. The mutable `build/candidate/bin` executable was not used.
- CLI arguments: `--strict --codepage 1252 --write-save C:\PROBLEM\b98r\before-run\Gear_blank.moproj`; working directory was the converter checkout. The command generated `Gear_blank.MST` and updated the copied `.moproj`; captured stdout/stderr file is empty. The tool wrapper did not return a separately inspectable numeric exit-code receipt, so success is inferred from the generated output and completed project rewrite rather than asserted from an exit status.

## Whole-tree result

Comparison used relative paths and SHA-256, excluding only `.png` and `.oracleA` sidecars as the existing folder comparator does. The temporary `candidate.stdout.txt` capture was also excluded. Before-state copy and retained after-state each have 29 comparable files. All relative paths are present on both sides. There is exactly one raw content difference:

| Relative path | Candidate | Retained after-state |
|---|---|---|
| `Gear_blank.MST` | 2,717 bytes, SHA-256 `4d8d4d2059ee4c5644769175a0c3b4c1134b94f892b251f814cad4e14f219c48` | 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` |

Every other comparable file is byte-identical. In particular the candidate-rewritten `Gear_blank.moproj` is 11,229 bytes with SHA-256 `dddc325a6d6eb34c1625a904eb1bc055a6403c450c312bbe1703bb26af5f9da5`, identical to retained after-state `Gear_blank.moproj`. The retained before `.moproj` was 10,932 bytes; the retained after-state includes `Gear_blank.KEY` and `Operations/Task00001/DieStress.Wzd` as well as the generated MST. Candidate output creates the corresponding KEY, Wzd, and MST paths. No other file content or path differs after the declared sidecar exclusions.

## Raw MST and path-only check

The raw candidate MST contains one occurrence of `C:/PROBLEM/b98r/before-run/Gear_blank.KEY`. The retained historical MST contains one occurrence of `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`. Replacing exactly that one candidate byte string in a separate copy yields 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`, byte-for-byte equal to the retained after-state MST.

The untouched raw candidate is `C:\PROBLEM\b98r\before-run\Gear_blank.MST`. The separate path-qualified copy is `C:\PROBLEM\b98r\candidate.path-qualified-before.MST`; its hash equals the retained MST. No normalization was applied to the raw candidate or retained evidence.

## Qualification

This provides strong retained-artifact evidence that the CP88 candidate conversion from the supplied before-state produces the same non-MST folder effects and the same MST payload after one exact project-path substitution. It is not fresh original parity: no original GUI action was replayed. The retained before/after folders show the net historical delta but do not establish the exact UI sequence, save timing, or whether any intervening action occurred. Therefore the result should be described as a candidate reproduction of the retained before-to-after artifact delta, not proof that it matches every original Save action.

No source or build files were changed. The original executable and GUI were not run.
