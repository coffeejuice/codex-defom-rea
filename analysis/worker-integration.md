# Integrated recovery checkpoint

The A, B and C research deliverables are now collected under `analysis/` on main. See `worker-integration-manifest.json` for the original worker commits, exact file hashes and integration verification.

A contributes GRNDAT contracts and fixtures plus independent reviews. B contributes keyword export and metadata contracts and fixtures plus independent reviews. C contributes DB controls/materials mappings and keyword reviews. The correction and recheck receipts distinguish historical findings from accepted revisions.

These are research artifacts, fixture specifications and audit scripts, not a completed C++ preprocessor. The 123 GRNDAT cases and 17 keyword action cases remain unexecuted. Production recovery and both coordination schedules remain paused.

Historical absolute paths identify the original worktrees. Preserve them when checking receipt hashes; locate the integrated counterpart using the same repository-relative `analysis/` path. Some scripts retain machine-specific paths and are not portable test commands. Raw `analysis/oracle/` bundles, databases and build outputs remain local-only. A fresh clone therefore contains the contracts and compact evidence, but cannot reproduce every native-evidence audit without those omitted inputs.

No loader API or production source changed in this integration. Verification covers JSON parsing, Python syntax and exact worker-to-main artifact hashes; it does not substitute for future runtime comparisons.
