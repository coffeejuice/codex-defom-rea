# CP88 GUI-free B98 comparison design

## What is safe to run

The existing candidate `build/candidate/bin/moproj2mst.exe` is a command-line converter. Its source usage is `moproj2mst <project.moproj> [out.MST] [--strict] [--codepage N] [--write-save]`; `--write-save --strict --codepage 1252` is the prior B98 reproduction profile. A fresh candidate run on a disposable copy of the retained B98 input requires no original-app launch or GUI-control session. Bind the candidate executable SHA-256 and exact command before running.

Do **not** execute `evidence/gui_mo/DEF_GUI_MO_64.exe` as a CLI. The checked-in CMake cache contract describes `M2M_REFERENCE_EXE` as “Read-only keyword-table reference, never executed”; evidence notes characterize it as the original GUI/static-analysis target. It remains a read-only file for static inspection. Running it, even with command-line arguments or headless Windows flags, would launch the protected application and has no qualified CLI contract. Its 67,611,136-byte size and retained SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` are identity metadata, not permission to execute it.

## Minimal candidate-versus-retained-original run

1. Keep the historical fixture untouched at `evidence/gui_mo/oracle_b/98_B_DS3F/`. Record a before manifest and hashes. The historical `Gear_blank.MST` is 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; its line 30 KFREAD path is `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`.
2. Create a unique disposable copy such as `C:\PROBLEM\b98r\run\` and verify the copied project/key/output paths are all shorter than 100 characters before executing. Copy the full fixture so the candidate sees the same `.moproj`, `.KEY`, and other retained inputs. Use the copied project `C:\PROBLEM\b98r\run\Gear_blank.moproj` and send output MST to `C:\PROBLEM\b98r\run\candidate.MST` (or another explicitly recorded file in the same short tree). No output path may point into `evidence`.
3. Run only the candidate CLI, with the project tree as cwd if needed for the recorded command: `moproj2mst.exe <absolute-project> <absolute-candidate-MST> --strict --codepage 1252 --write-save`. Record exact argv, cwd, environment, return code and stdout/stderr. This profiles candidate Save behavior, not original execution.
4. Preserve the raw candidate MST byte vector and SHA-256 before any comparison transformation. Verify its single B98 Diestress KFREAD path is exactly `C:/PROBLEM/b98r/run/Gear_blank.KEY` and its line marker/count agrees with the historical fixture. Record the actual generated path rather than calculating it from the historical root.
5. Create an **in-memory/separate comparison copy** of candidate bytes and replace only that one exact candidate path value with the one exact historical path value. Do not modify either file. Record the transformed byte count/hash and require complete byte equality with the 2,756-byte historical MST; equality implies transformed hash `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. Report both results explicitly: raw comparison is one path-line difference; path-qualified whole-file comparison is equal. Any second path occurrence, additional byte difference, missing marker, wrong key basename or candidate failure invalidates the bounded match.
6. Record a before/after manifest of the disposable tree, including `.moproj`, `.MST`, `.KEY`, and every other file created/rewritten/deleted by `M2M_FLAG_WRITE_SAVE`. Retain the candidate tree and logs. Verify historical fixture manifests/hashes are unchanged.

The expected raw candidate byte count is location-dependent; do not hardcode a hash or assume the old 2,786-byte reproduction repeats. Only the exact path string is predicted. The expected transformed comparison is exact complete-file equality, based on the source-qualified Diestress path rule and the retained old MST. If that fails, retain both raw inputs and the first differing offsets without broadening normalization.

## Scope of conclusion

This is the shortest independent comparison possible without GUI control: a fresh run of the candidate against the retained original B98 artifact. It can confirm current candidate behavior for this operation and whether the only retained mismatch is the project-root-derived path. It does not prove a fresh original Save, matched-root raw parity, original runtime behavior, non-ASCII path encoding, or any other KFREAD writer. Fresh original comparison remains unavailable while the GUI release gate is closed, and the protected original binary must remain unexecuted.

No candidate command was run and no source, build, fixture or comparison file was changed for this design.
