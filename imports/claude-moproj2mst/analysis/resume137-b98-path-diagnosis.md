# CP137 B98 KEY-path difference diagnosis

## Result

The single `Gear_blank.MST` difference in CP136 is explained by the two project roots. It is not evidence of a candidate path-construction defect. Keep the raw difference visible; do not rewrite the candidate path or normalize the acceptance comparison.

The original and candidate both serialize the absolute `.KEY` path derived from the opened project's subproject DB path. CP116's source trace identifies original `SfOprDiestress::writeMst` at VA `0x1402b5410`: for this B98 `Type=0` subproject, the writer obtains the full DB path from the project, replaces `.DB` with `.KEY`, and passes that QString to the `KFREAD` QString-stream helper. Candidate `src/ops/diestress.cpp` and `src/model/paths.cpp` implement the same project-owned derivation. Neither writer substitutes a fixed checkout root.

## Invocation roots

The retained original run scripts distinguish process working directory from opened-project directory:

- `_tools/mk2.py` sets the corpus/case `ROOT` to `C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b`; `_tools/mkl6.py` builds `98_B_DS3F` there and copies it to `98_B_DS3F_before`.
- `_tools/run.py::fresh` starts `DEF_GUI_MO_64.exe` with process CWD set to the `_tools` directory. `run()` separately opens the `.moproj` at `ROOT\<case>` and observes/saves `ROOT\<case>\<case>.MST`. The retained log records `98_B_DS3F saved 2756` and GUI exit code 0.
- CP136 ran the frozen candidate on its disposable project under `C:\PROBLEM\b98r\r136-527631d9\p` (receipt `cwd` is this project directory). Its output therefore contains that current project root.

Thus the differing strings are the old opened-project root (`C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`) and the CP136 disposable opened-project root (`C:/PROBLEM/b98r/r136-527631d9/p/Gear_blank.KEY`). The original process CWD being `_tools` does not explain the path: the writer's qualified source uses the opened project path.

## Raw evidence and scope

CP136 reports 29 comparable output paths, with exactly one raw difference. Candidate `Gear_blank.MST` is 2,722 bytes, SHA-256 `e5845ff130256e919f4193ccc73b2196f00f1de3b74c8beb0ab6ba3f9c4aae12`; retained original is 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. Each path occurs once. Replacing only the candidate path in a separate diagnostic copy yields 2,756 bytes and the retained original hash. This supports a location-qualified explanation, but the unmodified files remain different and must continue to be recorded as raw DIFF.

The retained run scripts/log bind an earlier original GUI save and its case root; CP136 itself is candidate-only, not a fresh paired run. No production fix is supported. The smallest additional raw-parity witness would be a fresh original and candidate Save using the same absolute opened-project path, with both raw outputs preserved. No such run is needed to justify the current source contract, and no GUI action was performed for this audit.

## Decision

Status: **source contract settled; raw historical path difference explained by opened-project root; fresh same-root raw parity unobserved.** Leave production and comparator unchanged. No source, build, test, GUI, or mutable REA action was taken.
