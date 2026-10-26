# CP120 SPRING2 writer ordering and failure continuation

## Source qualification

The evidence is bound to original `evidence/gui_mo/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The branch analysis is in `analysis/wave2-save-diffs-300-support-1410db030.c` (`FUN_1410db030`) and the nested file writer in `analysis/wave2-save-diffs-300-support-1410da9e0.c` (`FUN_1410da9e0`). The consolidated qualification and reader context is `analysis/resume119-spring2-normal-path.md`.

## Branch order

`FUN_1410db030` scans all `count` records' integer flags at offset `+0x38`, setting one Boolean if any flag is nonzero. It constructs the primary `SPRING.INI` list and a separate `SPRING2.INI` list of the flags in row order. It attempts the primary write first.

| Flags | Primary write result | Companion action | Function result / retained old companion |
|---|---|---|---|
| all zero | success | Call `FUN_1407fbc10("SPRING2.INI")` after primary attempt; its result is ignored | Return primary result; readable old file removal is attempted |
| all zero | failure | Same readable-delete call still occurs | Return false; a readable old companion is still subject to deletion |
| at least one nonzero | failure | Skip companion write entirely | Return false; no cleanup/rollback branch is shown, so an older companion is left as-is |
| at least one nonzero | success | Call `FUN_1410da9e0` for the new companion list | Return companion writer result; primary remains written if companion open fails |

`FUN_1407fbc10` is the qualified readable-only cleanup path: it attempts `fopen("rt")`, then closes and removes the filename only if that open succeeded. Missing or unreadable files are not removed. Its return is not consumed by the all-zero branch. Consequently, zero flags can remove a stale readable companion even when the primary open failed; nonzero flags preserve a stale companion when the primary open fails because the second writer is gated on primary success.

## QFile/TextStream completion boundary

`FUN_1410da9e0` opens the target QFile in observed WriteOnly mode. If open fails, it returns false before constructing a QTextStream or issuing stream writes. If open succeeds, it writes every QStringList item, appends CRLF, calls `QFile::close()`, sets its return value to true, then destroys the QTextStream and QFile. It does not check QTextStream status, QFile error state, or any close result. The close call returns void at the observed signature. Thus its Boolean distinguishes open success from open failure; it does not report short writes, flush/storage errors, or close errors. This statement describes the decompiled return path and does not infer Qt's handling of an old file's bytes on a failed or partial write.

Failure continuation is limited to the branches above. A primary open failure with nonzero flags stops before companion open. A companion open failure follows a successful primary write and returns false without rollback. A failure after successful open is not represented in the writer's Boolean, so no further continuation can be inferred from it.

Count zero returns false in `FUN_1410db030` before either sidecar writer or the `SPRING2.INI` cleanup call. The count is normally supplied by the UI list count; invalid pointer/count states are outside this qualification. No original runtime or candidate test was run for this read-only report.
