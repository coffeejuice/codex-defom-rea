# Per-file USN cache probe

## Result

The no-elevation scratch probe did **not** establish that this volume can
provide usable USN invalidation. `FSCTL_QUERY_USN_JOURNAL` on `\\.\C:` failed
before and after the scratch mutations with Win32 error **1** (`ERROR_INVALID_FUNCTION`,
“Incorrect function”). `FSCTL_READ_FILE_USN_DATA` returned a valid-looking
major-version-3 record for the scratch file, but its USN was **0** both before
and after a same-size overwrite whose original nanosecond mtime was restored.
The rename preserved the file ID and likewise returned USN 0. The change was
therefore **not detected** by the per-file USN value on this host.

The probe stopped at that point. No journal was created or changed, no
elevation was requested, and no fixture or runner path was opened. Its isolated
scratch directory was removed in the script's `finally` cleanup. The exact
structured probe output is reproduced below:

```json
{
  "journal_query_error": 1,
  "journal_query_message": "Incorrect function.",
  "file_record_major": 3,
  "usn_before": 0,
  "usn_after_same_size_write_with_mtime_restored": 0,
  "same_size_change_detected": false,
  "rename_old_path_exists": false,
  "rename_new_path_exists": true,
  "cleanup": "removed own scratch directory"
}
```

The probe does not identify the filesystem type, explain why the USN is zero,
or establish whether another volume or API configuration would behave
differently. Treat the NTFS USN-cache idea as **not feasible on the observed
volume under this non-elevated API path**. Keep the current full SHA-256
integrity snapshots. Do not reuse `FILE_ID_INFO` as a content-change signal;
the file ID stayed constant across the write and says nothing about the changed
bytes.

The script is [probe_usn_cache.py](../tools/probe_usn_cache.py). It only creates,
mutates, renames, and deletes its own temporary file beneath this analysis
directory.
