# CP78: B98 short-path headless rerun plan and sandbox boundary

## Current evidence already narrows B98

The retained CP66 headless receipt at `analysis/resume66-b98-headless-comparison.md` used the existing focused `m2m.s.b98_kfread` test against an immutable copy of `98_B_DS3F`. It ran the candidate's `m2m::convert` Save path without launching the original GUI. Raw candidate MST bytes differed from retained `Gear_blank.MST` only at the candidate-local `Gear_blank.KEY` path. Replacing exactly that one path occurrence in a separate in-memory buffer yielded the retained 2,756-byte file exactly (SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`). The raw candidate remained unmodified. A 31-file source-fixture manifest digest matched before/after (`1a9066f35f83705e0ecf881ec2f426bf749c1e5a3df6e083c49beb5c1d7957d2`).

This is enough to classify the historical B98 row as a path-qualified candidate-versus-retained-output difference. It does not create a new original session, prove whole-folder lifecycle parity, or convert the retained historical output into a fresh oracle.

## Disposition under the current write boundary

The proposed short scratch root `C:\PROBLEM\cp78b98` does not currently exist. `C:\PROBLEM` itself exists and its Windows ACL allows authenticated-user modification, but this task's sandbox writable roots are only the repository and its configured visualization directory. `C:\PROBLEM` is outside those roots. Therefore a new disposable copied test run cannot be performed under the present sandbox, even though the OS ACL appears permissive. I did not create the root, write into an existing `C:\PROBLEM` folder, launch the executable, or request escalation.

The already-built candidate binary is available at `build/candidate/bin/m2m_s_b98_kfread.exe` (CP76 receipt SHA-256 `68b824275c4d68b4aa32a25fe5c5175193110a4fa95fd180235d0e20b69d4cfa`). The existing test source copies the immutable fixture to a unique child of `M2M_TEST_SCRATCH`, converts that child, and compares a path-qualified copy of the output. It does not edit the retained input fixture. A rerun would be headless and require no source/build/app action, but it would write its child project and MST to the configured scratch root, which is precisely the denied outside-workspace mutation.

## Exact ready-to-run plan if the writable boundary includes C:\PROBLEM

Use a new short scratch root of the same path length as the prior CP66 root, and the existing CP76 executable/source unchanged:

```powershell
$env:M2M_ORACLE_B = 'C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\oracle_b'
$env:M2M_TEST_SCRATCH = 'C:\PROBLEM\cp78b98'
& 'C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\candidate\bin\m2m_s_b98_kfread.exe'
```

Before the run, record the executable hash and a sorted manifest of all 31 retained fixture files; after the run, recheck the same fixture manifest and record the actual generated child path and its project/KEY/MST path lengths. The prior CP66 project path was 56 characters and candidate KEY path 53; this root has the same component length as `C:\PROBLEM\cp66b98`. Require every generated project/KEY path to remain under 100 characters. The test should pass with exit 0; preserve raw candidate bytes and report raw hash separately. Replace only the exact single candidate KEY-path occurrence in a separate comparison buffer, then require that normalized buffer to equal retained `Gear_blank.MST` hash `e6998030...27c1`. No source fixture is ever used as an output location.

The planned run can reconfirm candidate `KFREAD` path selection and exact output parity modulo that one environment path. It cannot establish original GUI execution or resolve other historical DIFFs.

## Conclusion

No new run is currently allowed because its required output scratch lies outside the sandbox writable roots. The existing CP66 `C:\PROBLEM\cp66b98` comparison already provides the relevant B98 path-qualified evidence. A future repeat only needs the existing binary and a scratch-root write grant; it does not need source or fixture changes. This report did not create paths or mutate candidate/build/fixture/GUI/REA state.
