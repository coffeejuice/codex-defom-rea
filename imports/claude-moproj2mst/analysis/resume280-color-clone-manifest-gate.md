# CP280 color clone manifest gate

## Scope and result

Implemented a static, read-only validator in `admin/color_clone_manifest_gate.py` for the fixed CP214 B98 source fixture and a separately nominated existing clone directory. It never copies a tree, creates a destination, launches the original, starts GUI control, invokes REA/Frida, or writes into either fixture. The exact CP214 source receipt is pinned by SHA-256 and its 33-entry/27-file digest and path hashes are checked against a current scan. The clone must be an existing tree below `C:\PROBLEM`, contain exactly those relative directory/file entries, match every byte count and SHA-256, contain no reparse points, remain stable while hashed, and satisfy the sub-100-character full project-path requirement.

The `-f` argument parser consumes groups of exactly four lowercase hex digits as UTF-16 code units, decodes strict UTF-16LE bytes, and requires canonical re-encoding. Its decoded text must equal `<proposed clone>\Gear_blank.moproj`; cwd must equal that clone root; the argv vector is mandatory and must exactly equal `[DEF_GUI_MO_64.exe, '-m', 'open', '-f', HEX]`.

## Identity and explicit HOLD

CP214's source root, `C:\PROBLEM\p214a` clone, before digest `55e2a58c…cedbd`, after digest `37ae6374…19c8a`, and sole empty `Lubricants` delta are reported as historical identities. Reusing `p214a` is labeled historical. A distinct clone path is labeled proposed, but a static matching manifest does **not** prove it was freshly reserved or created; freshness remains `not established` and `readyForIndependentReview` is always false. The receipt leaves original PE/callsite bytes, observer coverage/overlap, and lifecycle/ownership/policy review as `unknown`. No matching manifest or endpoint is promoted to successful Open.

## Validation performed

Focused unit tests: `python -m unittest admin.test_color_clone_manifest_gate`.

The scan compares directory identity around enumeration. For every file it checks `lstat`, opened-handle `fstat` identity/size/mtime before and after hashing, then path `lstat` and reparse status again. Path ctime is compared across the path-level checks; opened-handle ctime is excluded because Windows reports different ctime semantics for path stat and handle stat. This catches ordinary substitution and mutation during the bounded scan, but it is not an atomic filesystem snapshot: a hostile actor could race between checks or change a tree after validation. Fresh exclusive destination reservation/creation evidence remains required.

Result: **10 tests passed**. Tests cover the retained source receipt and full 27-file hash list, exact CP214 UTF-16 encoding and malformed forms, non-BMP surrogate pairs, reparse detection, containment and length boundaries, simulated source/clone change between scans, omitted/wrong argv, and overlong project paths. The gate was not run against any clone, and no fixture bytes or shared plan/state were changed.
