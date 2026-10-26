# Synthetic folder-pipeline failure validation

## Scope and run coordination

`tools/test_folder_pipeline_failures.py` prepares a disposable corpus and
oracle tree below a unique `build/test-speed-candidate/pipeline-failure-runs/`
directory. It runs nothing unless called with `--execute`. Do not execute while
the coordinator's full suite is active. The test binary must be an existing
isolated candidate executable under `build/test-speed-candidate`; the script
does not configure or build it.

After the coordinator confirms that the full suite is idle, run:

```powershell
python imports/claude-moproj2mst/optimization/test-speed/tools/test_folder_pipeline_failures.py --execute `
  --test-exe imports/claude-moproj2mst/build/test-speed-candidate/bin/m2m_s_save_folder.exe
```

The tool creates all fixture and scratch paths under one run-specific candidate
build directory. It leaves the run artifacts in place for inspection. It does
not read or mutate imported evidence fixtures, use the original application,
or change source files. `--execute` invokes the focused `m2m_s_save_folder.exe`
directly, whose test translation unit registers only `save_folder_sweep`.

## Synthetic cases

The corpus and oracle are deliberately small, created from scratch per run:

- A fixed B input with a malformed `.moproj` and an existing expected folder
  should produce a normal `REFUSED` diagnostic.
- A second malformed `.moproj` under a generic `*_before` folder provides an
  adjacent refusal case.
- A copy-failure case holds one input file open with Windows share mode zero
  while the test runs. Recursive copy should fail on that owned sentinel and
  produce `SKIP` with the OS error.
- Inputs with no exact lowercase `.moproj`, including a fixed B case, should
  be silent preflight omissions. An input with a `.moproj` but no expected
  folder should also be omitted before its ordinal is allocated.
- The fixed 52 B case has an input `.moproj` and intentionally absent expected
  folder. The generic `missingexpected_before` case covers the same rule.

The script invokes serial default first, then `M2M_FOLDER_PIPELINE=1`, keeping
the copy-failure lock held across each run. It checks for the expected two
refusals, one copy skip, three counted eligible cases, silent omissions,
successful testkit status, and byte-for-byte equality of the serial and
pipeline stdout/stderr files. The structured receipt includes return codes and
stream SHA-256 values. If malformed-project bytes do not lead to the expected
converter refusal in this build, the check fails; it must not reinterpret a
different result as success.

The tool also calls the executable's private `--folder-post-worker` entry point
with a deliberately truncated length frame. It expects a nonzero exit, no
stdout, and a `Truncated folder IPC` diagnostic in the helper's private stderr
file. This checks local malformed-input rejection, but it does not prove that
the coordinator suppresses partial output after a helper crash.

## Injection limit

The inspected optimization source has no deterministic helper crash,
incomplete-result, or delayed-helper injection setting. Do not race a watcher
against IPC files or kill processes by PID to simulate those faults. Parent
coordinator behavior under helper crash/incomplete IPC remains **not tested**
until a source-owned deterministic seam is available. The direct malformed
helper check is intentionally narrower. Normal serial/pipeline log equality
does not substitute for that failure-path proof.

## Completed focused run

The full suite was confirmed idle before execution. The isolated executable
`build/test-speed-candidate/bin/m2m_s_save_folder.exe` (SHA-256
`6BA60A57A59F508D97C6E2A1748B56F6505F0F21C23C6F50FCF21A86C406EBD4`) ran the
synthetic serial and pipeline sweeps successfully. The run artifacts are under
[`pipeline-failure-runs/b1b940b84d`](../../../build/test-speed-candidate/pipeline-failure-runs/b1b940b84d),
with the machine-readable result in `receipt.json`.

Both modes exited 0 and emitted byte-identical stdout and stderr. Each emitted
two refusals with converter code 3 (`Error on reading project file!`), one
copy-failure skip with Windows' “file is being used by another process”
message, and the same `oracle B: 3 cases ... 2 refused` summary. The fixed
missing-moproj case, fixed missing-expected case, and generic missing-moproj /
missing-expected cases stayed silent. Both testkit runs reported `0 of 1 cases
failed`. The held-open sentinel was in the script's disposable fixture tree
and its handle was closed after each run.

The direct malformed private-helper case exited **2**, emitted no stdout, and
captured `Folder post helper infrastructure failure: Truncated folder IPC`.
This validates that one helper rejects a truncated frame without printing a
success result. It does **not** validate coordinator behavior after a helper
crash or missing result: the source has no deterministic parent/helper fault
injection seam, and no race-based kill or IPC edit was attempted. Therefore
parent no-partial-output behavior under helper failure remains unproven.
