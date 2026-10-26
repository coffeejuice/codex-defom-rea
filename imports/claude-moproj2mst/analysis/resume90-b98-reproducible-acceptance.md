# CP90 reproducible B98 retained-artifact acceptance

Added the opt-in script `admin/accept_b98_before_after.py`. It is not registered in or invoked by the fast full suite. The script requires an explicit CLI path and expected executable SHA-256, then independently checks the frozen CP88 executable and adjacent dependency hashes. It does not search for or launch `DEF_GUI_MO_64.exe`.

The script validates the retained before/after fixtures and their link-free tree, allocates a unique never-reused short directory below `C:\PROBLEM\b98r`, copies the before fixture, and verifies the copied input manifest before conversion. The previous CP90 invocation left `M2M_MO_INPUT_MODE` untouched; the candidate adapter contract maps an unset variable to mode 0. The script now pins child `M2M_MO_INPUT_MODE=0` for every run, so inherited overrides cannot change the conversion. The effective value and prior parent value are recorded in `receipt.json`. It records raw stdout/stderr, full input/output manifests, the retained after-state manifest, exit status, argv, cwd, and hashes. It compares the 29 `.png`/`.oracleA`-excluded paths, requires the one raw content difference to be `Gear_blank.MST`, and preserves a separate one-path-qualified MST in the run root. Run artifacts are retained; there is no cleanup or delete path.

## Execution receipt

Command:

```powershell
python admin\accept_b98_before_after.py --cli build\wave3-checkpoint88-verified\moproj2mst.exe --expected-cli-sha256 eef23401ece560ca6405c5ae23cb315cea88a1b14dc907f97c4c7e689a4be373
```

Result: exit code 0; acceptance passed. Run root: `C:\PROBLEM\b98r\r-10a8e762`; receipt: `C:\PROBLEM\b98r\r-10a8e762\receipt.json`. For this run the parent process was deliberately given `M2M_MO_INPUT_MODE=1`; the recorded child environment was explicitly `0`, proving an inherited override does not affect the comparator. Both sides had 29 comparable paths. The only raw difference was `Gear_blank.MST`: candidate 2,719 bytes, SHA-256 `fdfb6d4eebd558b531341545bab54c7da26a4bc15fcf4905b647883866ebc013`; retained 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. Exactly one candidate key path and one retained key path were found. Replacing only the candidate path produced a separate 2,756-byte file byte-identical to the retained MST, with hash `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.

The no-execute check `python admin/accept_b98_before_after.py --self-check` passed. It injects an inherited sentinel value in an isolated mapping and confirms the child value is fixed to `0`; it does not launch the converter.

This repeats the retained-artifact comparison with a different unique run path, which changes the raw path-bearing MST size/hash while preserving the one-path-normalized equality. It remains a candidate-only reproduction; the exact historical GUI action sequence remains unqualified. The protected original was neither run nor modified, and retained evidence was not modified.
