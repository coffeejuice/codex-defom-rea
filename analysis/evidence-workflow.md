# Retained evidence lookup pilot

The dedicated Bottleneck chat owns this bounded tooling pilot; Coordinator reviews
and integrates it. Workers own their recovery contracts and feedback. This workflow
provides bounded retained-evidence access; it does not replace REA analysis or
establish runtime equivalence. The original evidence remains authoritative.

## Select before printing

Use `rg --files --hidden --no-ignore analysis` with a filename filter when searching
local evidence intentionally, since ordinary `rg --files` omits ignored captures.
Do not print a whole snapshot, README plus ledger, or complete function dossier.
First inspect a compact inventory:

```powershell
python tools/evidence_slice.py analysis/material-import-0x140384a50.json
```

Select the field you need. Literal matching is case-sensitive and returns original
zero-based indices. It is a locator, not sufficient control-flow context:

```powershell
python tools/evidence_slice.py analysis/material-import-0x140384a50.json --pointer /normalized_result/pseudocode --contains 41bed0
```

Then read surrounding lines with `--start` and `--count` without `--contains`.
With a filter, `--start` indexes the matching sequence, not source lines.
`next_start` reports remaining pages; `omitted_items` includes both filtered and
unpaged source items. Text values retain their original line endings.
Objects yield an inventory; choose a child pointer to read its contents.

For nested bundles select the record before its field. Pin the full source hash:

```powershell
python tools/evidence_slice.py analysis/oracle/import-snapshot.json --record-pointer /evidence_bundle/records/9 --pointer /raw_result/value --count 200 --expect-sha256 2f7f66e01d4be040fb30237d78426d712e93c00a3f6225ec9c5b46b4caaa02a3
```

This index is valid only for that digest. Check the returned evidence ID, target
digest, operation and parameters before using the result. Missing fields and wrong
digests fail with exit code2. A missing field proves only that the explicit pointer
is absent from this source, not that the function is absent from all evidence.

## Limits and validation

The helper reads and parses the complete JSON file on every call. It does not build
an address index or normalize all historical REA schemas. `--count` limits items
(maximum200), not bytes; a large single item or metadata section can still be large.
Use smaller child pointers. Only selected values and enumerated qualification
paths are reproduced; consult the original source for deeper facet-specific limits.

Source identity includes the absolute path, exact byte count, SHA256 and nested
record pointer. Qualifiers include available root and normalized limitations,
relationship coverage, facet availability and analysis-profile digest. A hash binds
bytes; it does not authenticate who produced them or certify their conclusions.

Run helper checks without building or running the original app:

```powershell
python -m unittest discover -s tests -p test_evidence_slice.py -v
python tools/benchmark_evidence_slice.py analysis/material-import-0x140384a50.json --contains 41bed0 --output analysis/bottleneck-pilot-benchmark.json
```

Benchmark output volume separately from elapsed time. The first measured selection
was about83% smaller than full pseudocode but took about5ms longer to parse and bind.
Whole-file output is a weak baseline. No end-to-end recovery speedup has been measured.

Chat C's independent pilot is complete (`PASS_WITH_LIMITATIONS`). Exact identities,
pages, line endings, wrong-hash failures and missing-pointer failures passed. For
the known snapshot record9, two helper pages emitted37,881B and took0.1163s;
focused direct pseudocode emitted11,019B at median0.0371s. These have different
provenance payloads. The relevant row-availability conditional was at zero-based
lines254–263, beyond the first200-line page. Keep slices opt-in and check pagination;
do not claim they are faster or smaller than focused extraction.

## One completed improvement: repeated address discovery

`evidence_index.py` builds metadata for explicitly named files, including ignored
ones. This pilot searched only these current-checkout files:

| File | Bytes | Indexed / seen |
| --- | ---: | ---: |
| analysis/material-import-0x140384a50.json | 14,368,119 | 1 / 1 |
| analysis/oracle/fstres-static-evidence.json | 150,534,258 | 569 / 625 |
| analysis/oracle/import-snapshot.json | 1,162,308 | 89 / 103 |

The index stores target SHA256, canonical procedure address, evidence ID, operation,
absolute source path, source SHA256, record pointer and available pseudocode field
pointers. It supports standalone records, root `records`, and
`evidence_bundle.records`. It does not infer bodies from assembly or normalize other
schemas. Seventy records lack a usable address/identity and are counted as skipped.
An unsupported container fails at build time. A miss means only no matching indexed
record in these listed files; skipped records and every other file remain unknown.

Each query checks the index against the SHA256 returned by build, then hashes every
source, including on a miss. Changed content fails even when file size and mtime are
unchanged. Missing sources and an altered index fail. Rebuild after changes and
explicitly add newly relevant files; no recursive discovery occurs automatically.
Hashes bind bytes, not authenticity. Source files can change after validation, so
pin the returned source digest again when extracting a body.

The original `1-review1` code, before the output-alias correction below, was timed.
These retained measurements do not measure the corrected helper or benchmark runner.
Five alternating CLI repeats per query measured these historical medians, including Python
startup, source hashing and serialization:

| Procedure | Focused direct scan | Indexed | Direct / indexed output |
| --- | ---: | ---: | ---: |
| 0x1401dc3f0 | 1.1832s | 0.1423s | 1,302 / 1,646B |
| 0x140384a50 | 1.1689s | 0.1404s | 1,827 / 2,171B |
| 0xffffffffffffffff (miss) | 1.1872s | 0.1425s | 783 / 1,153B |

Setup cost1.2297s and produced a348,733B local index with659 entries. It amortized
after two queries in each measured case. That revision was about8.3x faster for repeated
discovery across these files. Each query still takes one CLI command, and indexing
adds one setup command. The benchmark executed31 subprocesses: setup plus30 timed
queries. The successful measurement run was launched through one exec call; this
is not a reduction in agent tool calls. Tool roundtrip time and agent reasoning
time/tokens remain unmeasured.

The baseline independently parses the same files, filters the exact address and
target, hashes sources and prints compact matching identity/pointer metadata. It
does not print entire files or bodies. The indexed wrapper adds status/notice text,
so its output is slightly larger. Results matched for all15 comparisons. A separate
independent pointer walk verified all659 cached identities and body-field paths.
All source hashes remained unchanged. Twelve unit tests passed across both helpers.
Raw samples, source hashes and code hashes are in `bottleneck-index-benchmark.json`.

Subject to acceptance of the correction, recommend opt-in adoption for repeated
discovery across a fixed evidence set.
Known-record extraction should remain direct or use source-bound slices when their
provenance is needed. This benchmark does not include body extraction, qualification
review, cold disk, peak memory, optimized byte-prefilter search, persistent in-memory
parsers, or whole recovery tasks. Full digest validation still reads166MB per query.
Do not generalize its8.3x ratio to application recovery or all lookup strategies.

### Output-alias correction awaiting independent recheck

A's `review-index-1` verdict was `REVISE`: an existing hard-link output alias could
bypass the path-equality check and overwrite its source. Task `fix-index-alias-1`
changes build policy to reject every existing output, including unrelated caches,
source paths and links. A preflight check gives an early error; exclusive `xb`
creation prevents truncation if an output appears between the check and write.
Fresh independent output remains supported. Rebuild into a new uniquely named file;
never delete or replace source evidence to make a cache path available.

All17 tests passed after the correction, using disposable synthetic data only.
New checks cover a real hard-link alias, source-as-output, an existing independent
cache, fresh creation with a valid receipt, and deterministic check/write interleavings
where a hard link or independent file appears after preflight. The initial sandbox
run could not create hard links; the scoped elevated rerun passed all17 tests.
This exercises the output-file creation race, not a general concurrency/locking model
for changing parent directories or source contents.

The benchmark runner now chooses a unique index path and a unique report path by
default, and rejects existing reports with exclusive creation as the final guard.
It was not rerun during this correction. The historical benchmark JSON and original
`bottleneck-pilot-freeze.json` remain unchanged. The new audit sidecar is
`bottleneck-pilot-freeze-v1-review2.json`; it cites the original freeze and A's review
hashes. Rollout remains blocked until independent recheck accepts the new freeze.

### Adoption commands

From `C:\Users\alext\Projects\codex-deform-rea` in PowerShell, rebuild the disposable
index locally rather than sharing its absolute paths across worktrees:

```powershell
& 'C:\Program Files\Python312\python.exe' -m unittest discover -s tests -p 'test_evidence_*.py' -v
New-Item -ItemType Directory -Force build/evidence-slice-tests | Out-Null
$bottleneckIndexPath = "build/evidence-slice-tests/bottleneck-evidence-index-$([guid]::NewGuid().ToString('N')).json"
$bottleneckReceipt = & 'C:\Program Files\Python312\python.exe' tools/evidence_index.py build analysis/material-import-0x140384a50.json analysis/oracle/fstres-static-evidence.json analysis/oracle/import-snapshot.json --output $bottleneckIndexPath | ConvertFrom-Json
& 'C:\Program Files\Python312\python.exe' tools/evidence_index.py query $bottleneckReceipt.index --expect-index-sha256 $bottleneckReceipt.index_sha256 --procedure 0x1401dc3f0 --target-sha256 f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794 --operation procedure_pseudo_code
```

Read the returned source path/hash, record pointer, operation and body pointers.
For the pilot's unchanged snapshot, the relevant lines can then be read explicitly:

```powershell
& 'C:\Program Files\Python312\python.exe' tools/evidence_slice.py analysis/oracle/import-snapshot.json --record-pointer /evidence_bundle/records/9 --pointer /raw_result/value --start 254 --count 10 --expect-sha256 2f7f66e01d4be040fb30237d78426d712e93c00a3f6225ec9c5b46b4caaa02a3
& 'C:\Program Files\Python312\python.exe' tools/benchmark_evidence_index.py
```

### Explicit review and integration file list

The following10 files are the complete proposed rollout. The six inherited files
were preserved; only their register and workflow documentation were revised here.

- `tools/evidence_slice.py` — inherited, unchanged.
- `tools/benchmark_evidence_slice.py` — inherited, unchanged.
- `tests/test_evidence_slice.py` — inherited, unchanged.
- `analysis/bottleneck-pilot-benchmark.json` — inherited, unchanged.
- `analysis/bottleneck-register.json` — updated pilot status and recommendation.
- `analysis/evidence-workflow.md` — updated measurements, scope and adoption steps.
- `tools/evidence_index.py` — bounded discovery helper with exclusive fresh output.
- `tools/benchmark_evidence_index.py` — independent baseline/runner, unique output paths.
- `tests/test_evidence_index.py` — cache, identity and source-preservation checks.
- `analysis/bottleneck-index-benchmark.json` — unchanged historical pre-correction results.

`build/evidence-slice-tests/bottleneck-evidence-index-*.json` files are ignored disposable
local output, not rollout files. The historical unversioned cache is left intact.
Freeze receipts are audit sidecars outside the ten-file rollout. No production files, central ledger, worker
contracts, reference binaries or REA sessions were changed. No app build,
deployment, stage, commit, push or merge was performed. Coordinator's next decision
is to request independent recheck, then commit this explicit set and distribute
opt-in usage only after acceptance.

## Coordination and adoption

The coordinator commits shared tooling separately from app recovery and sends the
exact revision, touched paths, examples and limitations. Each worker adopts at a
natural checkpoint, preserves local work, and acknowledges in its own chat. A
worker may use the coordinator's absolute helper path read-only for a pilot, but
must record the helper digest. Do not edit the shared helper from a worker chat.

Resolve current REA ownership from `analysis/coordination-state.json`; do not rely
on a fixed owner recorded here. Follow `analysis/coordination-workflow.md` for
confirmed release and reassignment. Workers without ownership reuse retained
evidence and record missing queries. No worker should close another worker's
session. The Bottleneck subtask and these helpers do not access any REA session.

Use apply_patch for large artifacts rather than embedding them in shell command
arguments. Keep temporary test files in the checkout build directory. Do not change
permissions, Git transport, or REA configuration to work around unrelated failures.

Rollout status and remaining bottlenecks are recorded in `bottleneck-register.json`;
the independent pilot result lives in Chat C's isolated analysis directory.
