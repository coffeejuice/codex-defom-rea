# Independent exact-hash equivalence review

Read-only source review found no introduced difference in a successfully completed snapshot of quiescent fixture files. The helper preserves full SHA-256 content reads, metadata fields, key spelling, overwrite behavior and insertion order. Threading changes observation and failure timing; it does not provide an atomic filesystem snapshot.

Reviewed on Python 3.12.10, 2026-10-24:

| File | SHA-256 |
| --- | --- |
| `admin/fixture_hash_fast.py` | `2D85151BCF8449537B5BA7539D28505B5B7A2E1F07BE2BE3998C7304CA67B87F` |
| `admin/run_converter_tests.py` | `5C0BF4C356A02A8E1E443E68414E9EF46755FC727EE847E8F1E3126062017750` |

The comparison is against the unchanged legacy `snapshot` at runner line 73, not a reconstructed approximation. The helper's `_paths` at line 22 repeats its sorted root traversal, per-root sorted `rglob("*")`/`is_file()` selection, and resolved containment operation. `_entry` at line 31 repeats its stat-before-read operation and 1 MiB streaming SHA-256. Both build the relative key from the resolved path beneath the same evidence path.

## Coverage and ordering

Nested roots and overlapping file roots are retained. A file can be read more than once; neither implementation deduplicates occurrences. If several occurrences resolve to the same relative key, assigning the key again replaces its value without moving its first insertion position. Under the quiescence requirement those values match.

The threaded helper submits work in traversal order and consumes `pending.popleft().result()`, rather than completion order. Faster later reads cannot reorder mapping insertion. Its one-worker branch executes the same enumeration/resolve/stat/read sequence synchronously. Worker count changes introduce no persistent cache or metadata shortcut.

A missing or broken-link root can produce no files through the legacy `is_file`/`rglob` path. The helper preserves this empty contribution; it does not introduce a missing-root error. The launcher's preceding inventory is a separate guard that rejects missing configured fixture roots. Missing files discovered after enumeration can still raise during resolution/stat/open, and both implementations propagate applicable errors without returning a partial mapping.

## Symlinks and junctions

The local Python 3.12 `Path.is_file` source follows a file symlink via `stat`, returning false for errors covered by pathlib's missing-path handling. Both algorithms then call `resolve` before containment, metadata capture and opening. A file link resolving beneath evidence is hashed using its resolved relative key. A selected file resolving outside evidence, or exactly to the evidence root, is rejected by the same check. Several aliases resolving to one key retain the overwrite behavior above. A link loop can raise during resolution; neither engine adds loop handling.

The local `Path.rglob` implementation uses `_RecursiveWildcardSelector` and `Path.walk`, whose default is `follow_symlinks=False`. This establishes inherited directory-symlink traversal behavior for this interpreter. A directory link explicitly supplied as a root is a separate case: traversal starts from that root path, and selected descendant files are subsequently resolved and checked in both engines.

No junction/reparse-point fixtures were created in this review. Whether a junction is traversed depends on the installed pathlib/Windows `DirEntry` behavior, which both engines invoke identically. The helper comment saying it does not follow directory links should be read narrowly as preservation of the legacy traversal, not as a separately verified exclusion of every Windows reparse-point type. In particular, this review makes no claim that junction descendants are excluded. No new link handling was introduced by the optimization.

## Error and race qualifications

Threaded execution intentionally changes error ordering when multiple failures coexist. The helper can enumerate and resolve up to twice the worker count before awaiting the first result. For example, an earlier file's read denial and a later file's outside-evidence containment failure can coexist: serial reaches the read denial first, while threaded prefill can raise the containment failure first. Enumeration of another root can likewise run ahead of earlier pending reads. This is not identical exception selection for failing trees.

Future results are consumed in order, so among already submitted read/stat errors the earlier future is normally observed first. A synchronous traversal/resolve exception can preempt that observation. Pending futures are cancelled where possible; the executor waits for running tasks during teardown. Already running reads are not immediately cancelled, and failure latency can exceed the serial failure path. No partial mapping is returned in either engine.

Both use a stat taken before hashing. A concurrent modification can therefore produce metadata from one instant and bytes from another. Threading additionally changes relative read times, including overlapping occurrences. Neither engine detects all mutation races; content/metadata equivalence is conditional on quiescent inputs. The existing synthetic unchanged-size/restored-mtime test establishes that neither engine replaces SHA-256 with a timestamp shortcut, not that capture is atomic.

Directory enumeration errors suppressed by pathlib remain suppressed in both implementations. This review does not claim that every unreadable descendant raises: failures already omitted by `rglob` cannot be recovered by either hashing engine.

## Scope

This pass inspected source and the installed standard-library implementation and hashed only the two small Python source files identified above. It ran no fixture-heavy benchmark, build, full suite, real evidence-root snapshot, original application or GUI session, and made no source/helper changes. Prior synthetic self-test results remain in `runner-integration.md`; they were not rerun for this review. The default launcher engine remains serial. The noted failure-order difference and junction qualification were reported to the coordinator before this report was written.
