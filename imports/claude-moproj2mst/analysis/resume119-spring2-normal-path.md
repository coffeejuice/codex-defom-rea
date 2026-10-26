# CP119 `SPRING2.INI` normal-path contract

## Finding

The original companion reader is a count-limited integer stream into a pre-zeroed flag array. A readable file with complete canonical values supplies one flag per row in order. A missing or unopenable companion supplies zero for every row because the caller resets the array after the helper returns false. On a successful open, EOF before `count` leaves the remaining entries zero and the helper still returns true. The helper does not inspect stream conversion status.

The Save writer treats flags as a separate list. It attempts `SPRING.INI` first. If all flags are zero, it attempts readable-file deletion of `SPRING2.INI` regardless of the primary writer result. If any flag is nonzero, it writes `SPRING2.INI` only after the primary writer reports success; a companion open failure returns failure without rolling back the already written primary file.

## Reader path: `FUN_1410da690`

The retained decompile is `analysis/wave2-save-diffs-300-support-1410da690.c`, also embedded in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`. It belongs to original `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The helper first writes zero to every one of the `count` output integers. It constructs a `QFile` for the supplied `SPRING2.INI` path and checks `QFile::exists()`. If the file does not exist, it returns false. If it exists, the helper attempts ReadOnly open; open failure also returns false. After successful open, it constructs one default `QTextStream` and loops while `!atEnd()` and the loop index is less than `count`. Each iteration extracts one `int` into the current pre-zeroed slot and increments the index. It then closes and returns true. It does not inspect QTextStream status or require `count` successful extractions.

The parent `FUN_1410dab40` in `analysis/wave2-save-diffs-300-support-1410dab40.c` calls this helper after allocating the auxiliary array. If the helper returns false—whether because the file is absent or open failed—the parent explicitly zeroes all `count` entries and continues parsing `SPRING.INI`. If either the auxiliary or record allocation fails, the parent returns false before this companion read and before row parsing. Allocator overflow and cleanup behavior remain unqualified.

For a source-closed normal profile, use a present ASCII file containing canonical signed decimal int32 tokens (writer-normalized values contain no leading zeroes other than `0`), with whitespace separators, up to the main row count. Full `count` tokens map to flags in row order. A readable empty or EOF-short file leaves the untouched suffix of the array at zero. Any token after the requested `count` is ignored because the loop exits on its count condition. The stream's numeric locale is C by default (`analysis/resume67-sw2sp-locale.md`); no `setLocale` call occurs here. Keep malformed tokens, radix-prefixed spellings, grouping separators, overflow, non-ASCII bytes, and uncertain codec cases outside this admission.

The EOF-short behavior follows directly from the array pre-zero loop, the `atEnd()` loop condition, and the helper's unconditional true return after successful open. It is distinct from malformed extraction: QTextStream conversion status/cursor effects are ignored by the helper but are not fully qualified for arbitrary malformed tokens. Do not treat a parse failure as equivalent to EOF without additional evidence.

## Writer path: `FUN_1410db030` and `FUN_1410da9e0`

Writer source: `analysis/wave2-save-diffs-300-support-1410db030.c`; its nested QFile/TextStream writer is `analysis/wave2-save-diffs-300-support-1410da9e0.c`; companion names and separators are in `analysis/wave2-save-diffs-300-extra3d-defaults.json`. These are tied to the same original executable SHA above.

`FUN_1410db030` scans every record flag at record offset `+0x38` and sets a Boolean if any flag is nonzero. It formats every flag as a base-10 integer into a separate QStringList, preserving row order. It constructs the primary `SPRING.INI` list and calls `FUN_1410da9e0` for that file first. The string data at file offsets `0x2c12bb0`, `0x2c12bb4`, and `0x2c12bb8` are respectively LF (`0A`), TAB (`09`), and CRLF (`0D 0A`). The nested writer streams every list entry, appends TAB when an entry is not exactly LF, then appends CRLF. Since companion entries are decimal flag strings, a bounded all-integer companion output is:

```text
flag0<TAB>flag1<TAB>...<TAB><CR><LF>
```

For example, flags `[1, 0]` serialize as `1\t0\t\r\n`. This is ASCII for these tokens and is independent of the process's non-ASCII codec. The reader's whitespace extraction accepts the writer's tab-separated form.

After the primary attempt:

- If every flag is zero, it calls `FUN_1407fbc10("SPRING2.INI")` and returns the primary writer's Boolean. The existing readable-delete evidence (`analysis/resume116-spring-count-two-writer.md`) says this attempts `fopen("rt")`, then closes/removes a readable existing file; absent files remain absent. The deletion return is ignored. Thus cleanup is attempted even when primary open failed.
- If any flag is nonzero and the primary writer returned false, it returns false without attempting companion write. An older companion may remain because no rollback or cleanup branch is shown.
- If any flag is nonzero and the primary writer returned true, it writes the companion list. Its Boolean becomes the writer result. If companion open fails, the primary has already been written and is not rolled back.

`FUN_1410da9e0` opens the target QFile in the observed WriteOnly mode, constructs a default QTextStream, streams the QStringList, appends CRLF, closes, and returns true after successful open. It does not inspect QTextStream status after writes. Therefore source evidence qualifies open-failure ordering and returned booleans, not storage-device short-write behavior or atomicity.

Count zero returns false from `FUN_1410db030` before either list is written or companion removal is attempted. The normal caller supplies a nonnegative QListWidget count; do not generalize negative count or invalid pointer behavior.

## Narrow candidate scope and tests

For a first candidate extension beyond absent-companion rows, keep the same bounded `SPRING.INI` count/row grammar and count ceiling, and add only a present readable ASCII companion with zero to `count` canonical signed decimal int32 tokens. Initialize all candidate flags to zero, overwrite the prefix supplied by complete tokens, and preserve record order. This models complete files and the source-qualified EOF-short zero-fill path while avoiding malformed conversion behavior. Exclude non-ASCII, BOMs, grouping/radix forms, malformed tokens, overflow, and inaccessible-file simulation.

Focused reader tests should cover: present `count` flags, mixed-sign decimal values, empty present file, one-value-short file for count two (second flag remains zero), trailing extra token ignored, malformed token refused with public output left empty, count/path gates, and retained absent-file zero behavior. An independent candidate writer test should assert `[1,0]` bytes `1\t0\t\r\n`; public Open→Save tests must additionally cover primary-write failure followed by flag-zero deletion, primary failure suppressing nonzero companion write, companion-open failure after successful primary write, and the no-rollback result. Those write-failure cases should use an isolated injected I/O seam, not destructive original files.

## Evidence limits

Static source establishes the open/read loop, zero initialization, EOF-short remaining zeros, parent fallback to zeros, writer field order, decimal flag formatting, file names, separators, and primary/companion attempt ordering. Static evidence does not qualify malformed-token cursor/status outcomes, the actual default codec in a fresh original process, non-ASCII companion content, allocator failure cleanup, write errors after successful QFile open, or matched original runtime behavior. The proposed canonical ASCII profile is an implementation admission subset; it is not a claim that the original rejects other forms.

No source, test, build, GUI, original-process, or mutable REA state changed for this report.
