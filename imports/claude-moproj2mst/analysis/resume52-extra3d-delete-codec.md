# Checkpoint 52: Extra3D delete path and codec boundary

## Finding

The Extra3D delete guard can be split from the QFile read/write admission guard, but the delete guard itself should remain. The two consumers take materially different path representations. The original scalar readers and writer pass `QString` paths to `QFile`; the cleanup helper at `0x1407fbc10` instead receives a narrow `char*` filename. Its filename encoding and directory resolution are separate questions.

## Original delete contract

`analysis/wave2-save-diffs-300-native-io-repair-contract.json` records the qualified call chain from bounded original PE evidence. The helper passes its `char*` unchanged to static CRT `fopen` with mode `"rt"`; on success it closes the file and passes the same pointer unchanged to `remove`, which reaches imported `DeleteFileA`. The `fopen` failure path reports success without attempting deletion. This is narrow-byte handling, not a QString/QFile conversion. The CRT reaches `CreateFileA` for the read probe. The original callers supply fixed ASCII relative sidecar literals.

Therefore ACP is not used to encode the directory by this helper: the literal basename is ASCII. The effective directory comes from the original process current directory. The retained evidence explicitly treats that current-directory mapping as part of lifecycle and does not establish behavior for an ASCII relative name under a Unicode or long current directory.

## Candidate boundary

Current `NativeExtra3DIo` keeps these contracts distinct:

- Constructor and QFile-equivalent read/write admission use `validateQFilePath`, which preserves ordinary path length, control-character, and namespace checks while allowing Unicode code units.
- `openRead` passes the resolved QString path to `StudyIo`; `openWrite` passes it to the wide `mst::FileDevice` path.
- `removeIfReadable` separately applies `ordinaryNarrowPath` to the resolved absolute path before `fopen` / `remove`. This deliberately refuses Unicode paths rather than silently selecting an ACP conversion or changing the original relative-name/current-directory behavior.

This is a safe static split in the sense that relaxing QFile path admission no longer disables reads/writes solely because the directory is Unicode, while the unqualified delete branch remains refused. It does **not** prove original and candidate delete behavior match for Unicode directories. Removing the delete guard, converting the absolute path through ACP, or swapping to `_wremove`/`DeleteFileW` would all change an unqualified branch. Passing only the relative basename to the candidate CRT is also not a justified fix without proving the candidate OS current directory matches the original Save-time current directory and without checking side effects from changing it.

## What static evidence cannot settle

Static evidence identifies the narrow API and fixed ASCII arguments, but it does not show a successful original delete under a non-ASCII project directory. Nor does it settle the exact process CWD at the Extra3D cleanup call for every Save path. No original application runtime was controlled for this task, so no ACP-dependent or Unicode-directory success claim is made. A future runtime capture would need to observe the process CWD and the helper's actual argument at the Save call, then test in a disposable Unicode directory; until then retain the candidate delete guard.

## Decision

Keep the read/write constructor split already described in `analysis/resume51-extra3d-path-contract.md`. Keep `ordinaryNarrowPath(resolve(path))` in `removeIfReadable` and preserve the separate refusal test. Treat the delete guard as a candidate support-profile limit, not evidence that the original rejects Unicode project directories. No source, fixture, build, GUI, or REA state was changed for this audit.
