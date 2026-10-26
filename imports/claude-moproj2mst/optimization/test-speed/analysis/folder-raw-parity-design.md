# Exact raw parity receipts for the folder sweep

## Purpose and acceptance boundary

The folder sweep's printed tally is an adjusted comparison, not a complete
record of bytes or decisions. Exact acceptance of opt-in process sharding needs
both (1) a complete per-case receipt captured before the work directory is
removed and (2) a serial/sharded replay that gives the converter the same
absolute case work paths. The ordinary serial default must stay unchanged.

Use this only in an explicit audit/replay mode. Keep the normal serial and
opt-in sharded runs free of receipt hashing unless requested. Do not treat the
audit run as the performance benchmark: hashing and serialization add I/O and
CPU work.

## Receipt contents

Write one versioned binary or JSON Lines record per attempted case beneath a
separate run-receipt directory. Write atomically to a temporary sibling and
rename only after the case record is complete. Bind every record to:

- unique run UUID, replay-pair UUID, mode (`serial` or `sharded`), worker count,
  schedule algorithm and worker assignment;
- source-tree digest, executable SHA-256, compiler/build configuration, and
  fixture/oracle root identities plus the launcher's full before/after fixture
  manifest digests;
- discovery ordinal, case name, event-stream index, terminal state, input,
  expected, selected `.moproj`, scratch root and work leaf absolute paths;
- coordinator PID, worker PID, process creation timestamp (to disambiguate PID
  reuse), worker index, and the effective `TEMP`, `TMP`, and `TMPDIR` values;
- conversion return code and full untruncated message, plus the exact captured
  case-output bytes (or their SHA-256 and a separately retained byte file).

Before `remove_all(work)`, enumerate and SHA-256 every regular file in the
produced work tree, including files the legacy comparator ignores. For each,
store original relative spelling, normalized comparison key, size, `mtime_ns`,
SHA-256 and reparse/link metadata where applicable. Separately store the full
raw inventories of the input and expected trees or bind them to the immutable
launcher's fixture manifest. Also store the comparator-visible inventories
after the current exact filters: `.png`, `.oracleA`, and `view_state.json` are
excluded; relative keys are lowercased and `/`-separated. A file can be present
in the raw inventory while excluded from comparison, so do not collapse these
two inventories.

File hashes are captured before any case cleanup, including on refusal if the
case work tree exists. Copy failures record the input identity and copy error;
they have no output-tree hash if no complete work tree exists. Capture/receipt
failure is an infrastructure failure and must fail the audit run. Preserve the
legacy behavior where normal diagnostic differences do not fail the sweep.

## Preserve each comparator stage

For every expected/actual relative-key pair, retain the full decision path and
the relevant file digests. The receipt must distinguish:

1. **Path inventory:** complete `got` and `want` key sets after the existing
   exact case-sensitive exclusions and lowercased-key mapping.
2. **Raw bytes:** exact equality result and both SHA-256 values. This is the
   first comparison and must stay first.
3. **Relocation:** if raw bytes differ, the expected bytes after the current
   whole-path-component replacement from `expected` to `work`, including both
   generic `/` and native separator forms. Record the transformed digest and
   whether it matched. Keep the existing early pass for bytes that already
   match without relocation.
4. **`.moproj` masks:** for remaining `.moproj` differences, record equality
   after the existing cycle/group `Expanded` mask, then equality after DOE-only
   removal of `ViewTransform` and `SfDispView`. Store the `doe` predicate,
   mask-enabled booleans, and each resulting digest. Do not mask other files or
   cases.
5. **Banner classifier:** for remaining non-`.moproj` differences, record the
   line-count check, whether at least one differing line was found, and the
   exact per-line classification inputs/result for `bannerOnly`. Retain the
   resulting banner count separately from file-difference vectors.
6. **Final vectors:** full untruncated `differ`, `missing`, and `extra` vectors
   in their actual order; `moprojDiffers`; and the pre-suppression state.
   Preserve the current first-eight-only human rendering separately.
7. **Older-view exception:** record `oldViewProfile(name)`, the exact
   preconditions, whether the one `.moproj` difference was cleared, and the
   final `expectedView` flag. Keep pre- and post-suppression vectors so the
   expected older-window exception cannot hide a changed raw result.
8. **Tally delta:** all six integer fields (`cases`, `identical`, `refused`,
   `files`, `banner`, `viewProfile`), terminal state (`copy skip`, `refused`,
   or `compared`), and exact per-case diagnostic bytes.

Also serialize the coordinator's full event stream in discovery order: case
records, summary positions and values, text/NOTE rows, missing-re-save count,
silent preflight omissions, and the exact eligible ordinals. This makes it
possible to prove that sharding kept the same coverage and printed output, not
only the same attempted cases. Preserve the actual directory-iterator order;
do not sort it during receipt generation.

## Same-path replay

Different process IDs currently appear in the sweep scratch root and can leak
through saved absolute paths. Comparing hashes from separate default launches
therefore cannot establish byte equality when each launch uses a different
work path. Use a validation-only stable replay root, explicitly beneath the
configured `M2M_TEST_SCRATCH`, for both runs. For example, both modes can use
`<M2M_TEST_SCRATCH>\folder-raw-replay\<pair-uuid>\<ordinal>` as the work leaf.
The parent test must own and clean that exact root; run serial and sharded
sequentially, never concurrently, and remove/recreate each leaf from its
unchanged input before each attempt. Save the first receipt outside the work
root before cleanup/replay.

Keep actual coordinator and worker PIDs in the receipt even though the stable
path token is independent of PID. Record effective temp paths and any
PID-derived files. Sharded workers need isolated temporary directories to
avoid collisions, so their temp paths may differ from serial. Do not silently
rewrite such outputs or exclude them: if a temp path or process-specific value
appears in a saved output, retain the raw difference and investigate. Same-path
replay controls the path written into project outputs; it does not make
different PIDs or temp environments equal by fiat.

The stable replay setting must be opt-in and validated with a containment check
against canonical `M2M_TEST_SCRATCH` before any deletion. It must not change the
default PID-based serial scratch path. A pair is invalid if the canonical input,
expected, selected `.moproj`, source, executable, or fixture-manifest identity
differs between its runs.

## Overhead and interpretation

Hashing all output files before cleanup adds one extra full read of the produced
tree; retaining expected/input inventories may add more. Hashing files during
the existing comparator reads can reduce duplicate reads for comparator-visible
files, but cannot cover excluded files or files with no expected counterpart.
Receipt serialization, tree enumeration, and extra SHA work must be timed as
separate audit phases. Report uninstrumented runtime separately from receipt
generation time. Never use receipt-enabled timings to claim sharding speedup.

For the parity gate, run the same receipt instrumentation in serial and
sharded modes, compare exact event streams and all per-case fields, and compare
complete file-path/size/hash maps under the controlled replay root. Differences
are evidence to investigate, not candidates for new masks. If instrumentation
changes conversion settings, file ordering, output paths, or comparator branch
order, it is not a valid parity oracle.

## Proof required before accepting sharding

- Exact same eligible ordinals, case names, input/expected paths, selected
  `.moproj` paths, event positions, NOTE rows, skips, and summary positions.
- Exact equality of conversion status/code/message, complete captured diagnostic
  bytes, full comparator-stage trace, raw and final vectors, and all tally
  deltas for every case.
- With same-path replay, byte-identical full output manifests (relative path,
  size, SHA-256), including ignored files. Any PID/temp-dependent differences
  remain explicit failures of raw parity until explained by source evidence.
- Identical full launcher fixture manifests before and after both runs, and
  unchanged source/executable/fixture identities. Both runs complete; no worker
  crash, malformed receipt, missing event, partial output, or cleanup residue.
- A separate uninstrumented performance comparison with unchanged validation
  settings, serial default, corpus, and output semantics. Receipt overhead must
  not be subtracted from or conflated with the production run.

No tests, builds, or fixture operations were performed for this design.
