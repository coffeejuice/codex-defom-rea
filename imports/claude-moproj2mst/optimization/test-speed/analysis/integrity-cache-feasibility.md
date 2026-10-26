# Persistent fixture-integrity cache feasibility

## Decision

An NTFS USN journal can support a fail-closed incremental SHA-256 cache, but a
volume-wide journal cursor alone is too coarse to promise useful savings: any
unrelated write anywhere on the volume would force a full rehash. A per-file
change token is a more promising candidate. Do not replace the current
uncached before/after SHA-256 snapshots until a prototype matches them exactly
on controlled changed-file cases and survives the invalidation tests below.

The current manifest maps resolved evidence-relative paths to
`[size, mtime_ns, sha256]`. The root enumerator's resolved-path behavior,
duplicate roots, output ordering, and failure behavior are part of the
comparison contract. A cache must return that same mapping and must still
notice directory additions, deletions, and renames.

## Candidate design

1. Keep the current full hash as the bootstrap and fail-safe path. Persist each
   exact digest together with the volume serial, file ID, file USN, size, and
   `mtime_ns` observed for that resolved file. Persist a journal ID for each
   volume and bind the cache to the canonical fixture roots and manifest
   format version.
2. On a later snapshot, enumerate the roots exactly as the current code does.
   For every occurrence, obtain current path, file ID, volume serial, size,
   `mtime_ns`, and per-file USN. Reuse a digest only when the full identity and
   change token match the cached row. Hash every new, changed, unqueryable, or
   otherwise ambiguous file. Rebuild the returned manifest in legacy order.
3. Treat missing cache data, parse/version errors, changed roots, a volume or
   journal-ID change, unsupported filesystem, unreadable per-file USN, failed
   metadata query, and any uncertain condition as a cache miss. Invalidate and
   perform the complete SHA-256 snapshot. Never emit a partial snapshot.
4. Persist new cache state atomically only after the complete snapshot succeeds.
   Do not skip the after-test integrity snapshot or fixture-change comparison.
   If the fixture tree is not quiescent, use the existing full snapshot and
   retain its documented race limitation.

`FSCTL_READ_FILE_USN_DATA` returns the most recent USN record for a file or
directory; `FILE_ID_INFO` supplies a volume serial and 128-bit file ID. Together
with path-set enumeration these are candidate invalidation signals. USN is a
change marker, not content, so the cached SHA remains the integrity value and
all cases where the marker's continuity is uncertain must fall back to reading
bytes. Microsoft documents that journals can be absent, recreated, stamped
with a new ID, or lose old records, and that records can aggregate reasons
rather than record every repeated same-kind write. Therefore never use
timestamps or file size alone as a cache key.

References: [FSCTL_READ_FILE_USN_DATA](https://learn.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_read_file_usn_data),
[FILE_ID_INFO](https://learn.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-file_id_info),
[USN journal data and valid-USN bounds](https://learn.microsoft.com/en-us/windows/win32/api/winioctl/ns-winioctl-usn_journal_data_v0),
[change-journal records](https://learn.microsoft.com/en-us/windows/win32/fileio/change-journal-records),
and [reading the change journal](https://learn.microsoft.com/en-us/windows/win32/fileio/using-the-change-journal).

## Required invalidation behavior

- **First use:** no trusted digest cache exists, so hash every file and create
  the baseline. No journal creation or mutation should be attempted.
- **Journal wrap/truncation:** query the current journal ID and valid-USN
  bounds. If the saved cursor predates `FirstUsn`/`LowestValidUsn`, a journal
  read fails, or the continuation cursor is discontinuous, discard affected
  cache state and fully hash. Per-file USNs may avoid depending on a saved
  cursor, but must still be invalidated when the journal ID changes.
- **Same-size write with restored mtime:** must be detected by the USN change
  token and rehashed. This is a mandatory experiment, not an assumption based
  on file size or timestamps.
- **Rename/move:** root enumeration must remove the old relative-path entry
  and add the new one. Rehash unless the cache proves the resolved file identity
  and content digest under the new path; conservative rehash is acceptable.
- **Create/delete/directory rename:** path-set comparison catches changes in
  the selected roots. A directory move across a root boundary must be treated
  as removals/additions. Do not assume a file-only journal record is sufficient
  to reconstruct path ancestry.
- **Volume boundaries:** maintain a separate identity per volume, keyed by
  volume serial and journal ID. Any unsupported/non-NTFS volume or inability to
  query its file USNs means full hashing for its files. Never apply a cursor
  from one volume to another.
- **Permissions and unsupported operations:** if volume or file handles cannot
  be opened, `DeviceIoControl` is denied, the journal is absent, or metadata is
  unavailable, fall back to full SHA-256 without requesting elevation or
  creating/changing journals. A fallback error must not silently become a
  successful cache hit.
- **File identity reuse, links, reparses:** include volume serial and complete
  file ID, compare resolved paths as the current implementation does, and
  preserve duplicate-root behavior. Treat reparse points, hard-link ambiguity,
  ID changes, or mismatched enumeration metadata conservatively as misses.
- **Atomic cache persistence:** interrupted writes, partial state, stale roots,
  and version changes invalidate the cache. Cache write failure may leave the
  normal snapshot successful, but the next run must bootstrap again.

## Safe experiment

Prototype outside the test runner, using only disposable synthetic directories
under the checkout's writable optimization area. Leave evidence fixtures and
the runner unchanged. The experiment should compare (a) the existing full
snapshot, (b) the cached snapshot result, and (c) the cached snapshot's path
and metadata set for exact equality. Keep SHA-256 as the oracle and run a
quiescent unchanged repeat plus one-change-at-a-time cases: same-size overwrite
with mtime restored, rename, create, delete, directory rename, attribute/time
change, and cache truncation. Use mocked journal-ID changes and invalid bounds
to verify fallback without altering the machine journal. Exercise missing
journal/API access and cross-volume roots when available; skip with a recorded
reason if the host cannot provide them.

After synthetic correctness passes, measure separate warm unchanged and
changed-file costs against full hashing. Only then propose a read-only
full-fixture comparison: exact equality of the cached and uncached complete
manifest, no test-runner integration, no fixture writes, and a full SHA oracle
on both sides. Record enumeration, USN-query, hash, and external wall times.
Reject the cache if metadata-query cost approaches the current hash duration,
if any required fallback cannot be proven, or if any manifest differs. A
successful one-host NTFS experiment would establish feasibility only for that
volume/filesystem configuration, not for other volumes or remote filesystems.

No prototype, journal query, fixture access, fixture mutation, or timing run was
performed for this assessment.
