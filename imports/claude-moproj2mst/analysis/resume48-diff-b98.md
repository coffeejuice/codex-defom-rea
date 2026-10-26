# B:98_B_DS3F historical diff audit

## Comparator result and raw-diff limit

`analysis/wave3-checkpoint47-validation.json` records B:98_B_DS3F as `DIFF`, one differing line, zero missing files, zero extra files, and no comparator adjustments. The same result appears in `analysis/resume47-combined-full-tests.json`. This is a historical folder-comparator result, not a fresh original-session acceptance result. The validation qualification says the folder comparator applies declared profiles and that raw differences must be inspected.

The retained original file is `evidence/gui_mo/oracle_b/98_B_DS3F/Gear_blank.MST`, 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. `analysis/wave2-save-diffs-reference-inspection.json` identifies its first KFREAD at line 29 / byte offset 1325; the following line at byte offset 1338 is exactly:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

The inspection receipt says its candidate is null, and the diagnosis notes the B98 sweep deleted candidate bytes. Therefore the exact *candidate-side* bytes at the one differing line are not currently available in the retained evidence. The one-line count plus the historical path explanation is consistent with an absolute case-prefix difference, but cannot be promoted to a complete raw-byte explanation or equality after path mapping. Preserve this uncertainty explicitly.

## First source-qualified writer and codec path

The original operation-specific writer is PE function `0x1402b5410` (Diestress). The retained original decompile `evidence/gui_mo/s1/s1-05/d1s.c` shows it obtains the subproject DB path, replaces `.DB` with `.KEY`, and emits the KFREAD path. The source port `source/apps/moproj2mst/src/ops/diestress.cpp` implements the same source-qualified sequence. In the original, the resulting QString is passed to `0x1401344a0`, which appends `KFREAD` and streams the QString using `QTextStream::operator<<(QString)`.

For this reported diff, the changing text is the absolute historical root in that line, not an established ACP encoding discrepancy: both the original path and fixture path are ASCII. Still, the writer's `QTextStream` serializes QString through Qt's locale codec unless a codec was set; the retained MST format note (`source/docs/re/moproj2mst/s1-02/spec.md`) says this writer closure does not set a QTextStream codec and uses `QTextCodec::codecForLocale()`. The specific `QString::toLocal8Bit` -> `CP_ACP` path-to-CRT conversion evidence applies to native file opens such as SimCtrl.KEY, not this MST text line. Do not mislabel B98's current one-line diff as proof of CP1252 behavior.

## Original action qualification

The available B98 MST is a retained historical oracle. The checkpoint 47 full tests do not launch the original executable; `wave3-checkpoint47-validation.json` explicitly records “No original executable is launched.” The `wave2-save-diffs-reference-inspection.json` is a read-only inspection of retained bytes and source evidence, not an action capture. Thus the case's exact original action/session/environment is not newly bound at checkpoint 47, and its path embeds the earlier `C:/Users/alext/Projects/deform-rea-work/...` root. Treat it as historical comparison evidence only.

## Concrete next action

When the existing original-first validation queue next runs an already-authorized B98-equivalent Save, capture the candidate output folder before cleanup. Preserve candidate `Gear_blank.MST`, hashes, and the full one-line context; record the project absolute path and action/session identity. Compare the original and candidate bytes at line 30 before any mapping. If the only delta is the case root, show exact prefix-bounded mapping and then recompare all remaining bytes while retaining both raw artifacts. Do not regenerate or alter the historical original, tune the comparator globally, or claim B98 raw parity from the line count alone.

This audit was read-only: no source, build, fixture, application, or mutable REA state was changed.
