# Single-project reference evidence harness

This tool prepares and compares evidence for the authorized fresh-state Open > Save > Close boundary. It launches no program, attaches to no session, sends no GUI input and changes no converter behavior. Python's standard library is sufficient.

All generated files go below this checkout's `build/reference-harness/`. There is no cleanup or overwrite operation. Use a new case id after a failed preparation or a changed candidate. Input snapshots, source snapshots, candidate binaries, raw manifests, runtime artifacts and receipts remain available.

Current prepared training case: `build/reference-harness/billet-training-003`, bound to `analysis/wave1-connector-final-v2-build.json`. Preflight passed with `verified_receipt_consistency`, 2,998 source files and all three supplied binaries matched; its selected XML references are present. No original/candidate runtime was executed. Cases 001 and 002 remain preserved preparation history; 001 has unverified source/build correspondence and 002 is superseded. See `analysis/reference-harness-status.json` for hashes and qualifications.

## Prepare and preflight

From the imported converter root, a first **training** case is `evidence/gui_mo/s1/corpus/06_BILLET/BILLET.moproj`: one HeatTransfer operation, one workpiece, and no missing selected XML references in the historical corpus index. The entire fixture is retained. Smaller DB_deconstruction fixtures exist, but their database/open dependencies need additional qualification. This is a small first end-to-end case, not a complete branch or failure suite.

```powershell
python -B tools/reference_harness/harness.py prepare `
  --case-id billet-training-001 --split training `
  --fixture evidence/gui_mo/s1/corpus/06_BILLET --project BILLET.moproj `
  --original-binary 'C:/Program Files/SFTC/DEFORM/v11.0.2/DEF_GUI_MO_64.exe' `
  --candidate-binary build/baseline/bin/moproj2mst.exe `
  --candidate-binary build/baseline/bin/moproj2mst.dll `
  --candidate-binary build/baseline/bin/deformdb.dll `
  --historical-oracle 'Oracle A, corpus cases.tsv row 06_BILLET; historical state unknown'
python -B tools/reference_harness/harness.py preflight --case build/reference-harness/billet-training-001
```

`prepare` copies the complete fixture, verifies content hashes before/after copying, retains the source tree and supplied candidate binaries, then freezes SHA-256 manifests before any reference observation. The first `--candidate-binary` is the executable; subsequent entries are its supplied dependencies. The candidate runs later from `frozen-candidate/`. A source or frozen-artifact change invalidates the case. The declared runtime dependency list does not prove complete loaded-module coverage.

Source and binary hashes alone do not establish that the supplied source built those binaries. After the coordinator identifies the final matching full build, use its `build/candidate/bin` artifacts and pass `--build-receipt analysis/<final-label>-build.json`. Do not prepare a runtime case from an intermediate build. The preserved `build/baseline` contains the earlier untouched baseline while current source may contain later relocation edits; the command above illustrates preparation syntax only, with an explicitly unverified absent build receipt.

Supported build receipts have schema `m2m-candidate-build-v1`, produced by `admin/build_candidate.py`. The harness reproduces its exact algorithm: iterate files in native `sorted(Path.rglob('*'))` order, concatenate each relative POSIX path encoded as UTF-8, a NUL byte, and that file's raw SHA-256 digest, then SHA-256 the concatenation. It checks the file count, pre-build digest/count, after-build digest/count and `source_stable_during_build=true` against the prepared source manifest. It requires successful configure and build steps, `targets=null`, no targeted build command, and a receipt artifact matching the path, size and SHA-256 of every supplied executable/DLL. Mismatches, failed builds, partial builds and missing stability fields in v1 receipts reject preparation before any copy is created.

The receipt is copied into the case and hash-bound. `source_binary_binding.status` is `verified_receipt_consistency` only after those checks. Missing receipts yield `unverified_absent`; legacy/unsupported receipts yield `unverified_unsupported`. Matching endpoint hashes cannot exclude an intermediate change followed by reversion, and a local receipt does not independently attest compiler execution or establish a reproducible build. Preflight reports the binding explicitly and rechecks its frozen receipt and snapshot identities.

`preflight` checks the freeze and scans selected XML path fields. Absolute paths, escape paths and missing references are reported. It cannot establish containment: compressed KEY data, DBs, wizard files, registry/settings and transitive dependencies need separate review. The sandbox is a disposable directory layout, not an OS security boundary. Preserve missing-input cases as intentional failures when that is their purpose; never repair fixtures silently.

The installed original must match the imported original SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Verify installed runtime availability, licensing, startup settings and absence of an earlier project before starting. No original GUI has been launched by this harness's implementation or tests.

## Runtime evidence protocol

The authorized operator or supported runtime driver supplies the actual execution and evidence. Do not run the historical `oracle_b/run.py` unchanged: it contains hardcoded upstream paths, session modes and guessed coordinate dialog responses. The historical script's `No` answer on close is not evidence of the default button.

1. Record the installed original's executable/module identities, launch argv, PID, environment, locale/code page, GUI startup unit, settings and initial window state. Start fresh with no previously opened project. One fresh process is a sufficient isolation mechanism, but fresh settings/environment still require observation. The original reportedly ignores a project command-line argument; discover the actual current File > Open control through supported inspection.
2. Use `sandboxes/original/cwd` as working directory. Open only `sandboxes/original/project/<project>`, invoke Save, then Close. Take phase observations/screenshots and retain messages. If a dialog appears, observe and record the actual default control, caption/text and selected answer, and select that default only. Do not infer defaults from old screenshots, screen fractions or a preferred answer. Record failure/refusal/crash/timeout and all partial writes, rather than retrying or rolling back within the case. Do not start simulation, DEF_PRE or DOE execution.
3. Finish observation before recording: process writes must be quiescent. On a hang or crash, document the process state and any separately authorized intervention; this harness never terminates a process. Retain external-write observations (settings/recent files/licence logs included) under `evidence/`; project manifests alone cannot establish absence of writes elsewhere or intermediate write order. These side effects are observations, not a blanket waiver.
4. Put evidence artifacts under the case's `evidence/` folder and fill a copy of `observation.template.json`, including explicit unknown settings where recovery is incomplete. Each action and each dialog references retained evidence. Record:

```powershell
python -B tools/reference_harness/harness.py record --case build/reference-harness/billet-training-001 `
  --role original --observation build/reference-harness/billet-training-001/evidence/original-observation.json
```

5. Only after the original receipt exists, execute the frozen candidate on `sandboxes/candidate/project/<project>` with cwd `sandboxes/candidate/cwd`, using **`--write-save`** and its default project-side MST output. Supply full runtime dependencies as needed and record them. MST-only mode does not cover the agreed full Save scope. Record the candidate observation with `--role candidate`. Candidate `started_utc` must be after the original receipt time. The receipt records this ordering and binds the original receipt digest.
6. Compare:

```powershell
python -B tools/reference_harness/harness.py compare --case build/reference-harness/billet-training-001
```

`record` verifies freeze identities, retains immutable-by-convention copies of final project/cwd files and an operator-declared observation, records added/removed/changed paths and metadata changes, and prints a compact receipt. Evidence files remain hash-bound in their existing case-local location. Receipts are local integrity checks; they do not independently authenticate the operator's statement or prevent deliberate rewriting of an entire evidence chain.

Successful completion requires all three lifecycle stages. Failed runs may record a prefix; failures are not converted to passes. A candidate CLI call may stand for the adapter's internal Open/Save/Close lifetime only with an explicit qualification in the observation; CLI success alone does not prove internal cleanup. Original GUI statuses/messages and adapter return values have no automatic equivalence mapping. Runtime field validation checks consistency of declarations, not independent process attestation.

## Raw and qualified comparisons

The raw comparison covers every file and directory in project and cwd, including `.moproj`, MST, KEY, MDT, wizard/DOE files, deletions and new side files. It preserves exact byte comparisons and first-byte windows. Timestamps and attributes are listed separately because byte equality does not imply metadata equality. Matching final file state never establishes write order, original architecture, complete dependency recovery or complete application acceptance.

No legacy-express, GENCTC, numerical, path, XML reordering or whitespace waiver is enabled automatically. To inspect one justified relocation difference, supply a JSON array through `--qualifications`. Each rule must select one raw-changed file, bind both full raw SHA-256 values, supply a written reason and case-local evidence, then list explicit byte slices for each role:

```json
[{
  "path": "project/Example.MST",
  "original_sha256": "<full raw digest>",
  "candidate_sha256": "<full raw digest>",
  "reason": "Specific path bytes are derived from the two disposable project directories; recovered writer evidence attached",
  "evidence": ["evidence/path-writer-evidence.json"],
  "original": [{"offset": 123, "old_hex": "433a2f6f6c64", "canonical_hex": "3c524f4f543e"}],
  "candidate": [{"offset": 123, "old_hex": "443a2f6e6577", "canonical_hex": "3c524f4f543e"}]
}]
```

Slices must match exactly and cannot overlap; empty source slices are rejected. Qualified views report separate transformed digests/equality. They never replace the raw result or create an overall pass. A reviewer must judge whether each qualification is scientifically warranted.

Historical oracle bytes remain in the fixture snapshot and provenance field. They are never used as the fresh reference. The Oracle B README explicitly documents session-seeded MultiBlow cases, while its initial plan is stale about authorization. Fresh-process historical notes remain historical evidence until bound to exact inputs, binary, environment, sequence and evidence. A declared `evaluation` label preserves intent but does not prove cases were unseen; genuinely held-out preparation/selection and candidate freezing must be controlled and reported by the research owner.

## Focused verification

```powershell
python -B -m unittest discover -s tools/reference_harness -p test_harness.py -v
```

Tests create synthetic nonexecutable bytes below `build/reference-harness/synthetic-*` and retain them. They exercise copy integrity, source/binary freezes, chronology, dialogs, failed/partial writes, tamper detection and separate raw/qualified results. They establish harness behavior only, not original or converter fidelity.
