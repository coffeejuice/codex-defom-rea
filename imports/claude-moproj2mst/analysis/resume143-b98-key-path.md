# CP143 B98 retained KEY-path discrepancy

## Finding

The historical path is bound to the original opened-project root by retained run/setup artifacts, without another GUI session. This resolves the source of the raw difference: both original and candidate writers derive `KFREAD` from the opened project's Type 0 subproject DB path and change `.DB` to `.KEY`. It does **not** make the candidate's short-path run raw-equal to the retained MST, nor establish that a fresh original Save at a new root would be identical.

`evidence/gui_mo/oracle_b/_tools/mk2.py` sets `ROOT` to `C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b`; `mkl6.py` builds `98_B_DS3F` there and copies it to `98_B_DS3F_before`. `run.py` independently binds the Save target: `run(case)` constructs `d = ROOT\case`, opens the `.moproj` from that directory, and observes/saves `d\<project-base>.MST` (lines 51–83). `fresh()` sets the GUI process CWD to `_tools`, but invokes `run(case)` separately (lines 90–103). The retained `run_l6.log` records `98_B_DS3F saved 2756` and GUI exit code 0. Thus CWD is not the path source; the opened case directory is. The retained `Gear_blank.moproj` has `Type="0"` and `Subproject_DbName DbName="Gear_blank.DB"` (lines 216 and 327), and the original writer contract derives the full DB path from the project before emitting the KEY path.

The retained `Gear_blank.MST` contains exactly:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

Candidate receipts instead contain the actual copied project root, such as `C:/PROBLEM/b98r/.../Gear_blank.KEY`. The latest CP136 full-output comparison (see `analysis/resume137-b98-path-diagnosis.md`) preserves both raw artifacts: 29 comparable files each, one raw differing file (`Gear_blank.MST`), no missing or extra files. Substituting the one counted candidate path in a separate diagnostic copy yields the retained MST bytes; the raw output remains different.

## Qualification and next action

- **Original-root decision:** Bound for the retained historical capture by the root constant, setup scripts, Open/Save path construction, and saved output location. This is enough to attribute the historical absolute path to the opened case directory.
- **Same-root raw normalization:** Not appropriate. The path is an output value derived from the actual project location, not incidental formatting. Preserve the raw `DIFF`; the one exact path-substitution view is diagnostic only and must remain separately named and hashed.
- **Candidate output location:** The required disposable candidate root is `C:\PROBLEM`, so its generated absolute KEY path should differ from the retained path. Moving candidate writes under the old evidence root would violate disposable isolation and is unnecessary for the candidate path contract.
- **Fresh raw parity:** Still unobserved. A raw same-root witness would require a paired original and candidate Save on the same project path. No GUI control or original execution was used or acquired here; without that paired capture, do not claim new original parity.

Evidence hashes checked in this read-only audit: `mk2.py` `072502D0EABBEC650DDF2DD2FAA18AD3846A90114F1B50C6E54FAA3F7F567BBB`, `mkl6.py` `4A5093E5666049BEC61A2CB9281E71608F0B1DFCA2F681B61F1C170AF2566BC0`, `run.py` `3465BADB0E37E9FC9CD37F4069B78AE0303295B82CC490AAAB487A71C1472F2E`, and `run_l6.log` `E9CFD7618FDC2309493416B6CDAD28D6CAD2C472611F0911CC94C13E8DC2B56D`. No source, fixture, build, GUI, or mutable REA state was changed or exercised.
