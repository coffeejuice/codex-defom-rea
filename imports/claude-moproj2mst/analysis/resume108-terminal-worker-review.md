# CP108 terminal remesh worker independent review

## Result and binding

The retained CP108 full-run receipt reports source-tree SHA-256 `d4af6a0dd5175467c7369bb724eb79dae1320f027990e98ea4a3c9fc30550e89`, CTest exit 0, 292 registered tests with 285 clean, 6 logged-gap, 1 logged-difference, and no skips/failures/incomplete outputs. The relevant worker log reports 0 of 4 cases failed. The worker executable currently present hashes to `138f392584da57cbf47145fccad4ba2d573fe58560921487a1b1f6d151597c0f`, matching the case receipts.

The CP108 source-to-run binding is verified against the retained archive rather than today’s live checkout. `build/wave3-checkpoint108-verified/source.zip` hashes to `2bffd39f4d8fcf66c1f46c694cd0ae0f07f13bef261c450252aac10aa244839`; its `apps/moproj2mst/tests/s/test_terminal_remesh_worker.cpp` entry hashes to `263d287e13f1ed8ca51cd02c70bfcb92e7656067c26c6d935ea568e80a1faf19`, matching the CP108 implementation receipt and the source-tree hash `d4af6a0dd5175467c7369bb724eb79dae1320f027990e98ea4a3c9fc30550e89` in the repair1 build/test receipts. The worker executable SHA in the raw case receipts also matches the executable recorded by the run. Thus the four logged outcomes are bound to the frozen CP108 worker source. The current live test file is a later CP109 revision: SHA-256 `dcae996e49c84d8159bf178e975c826cce36ee0a80b31b610caa9eb64f6b110d`, 719 lines, eight TEST_CASE declarations. It is not the source under review here; its separate coverage is governed by CP109 receipts.

## Four retained case outcomes

Raw evidence is under `build/candidate/t/87115a3f/terminal-remesh-41728-50193015/case-{17,18,21,22}`. All receipts report the same executable SHA before/after, `created=1`, `resumed=1`, `stopped=1`, `supervisor_stop=0`, `prepared=1`, `inspected=1`, `lease_retained=0`, empty cleanup error, two protocol frames, and the live save snapshot `save_flags=1015`, checked subprojects `1`, answer `65536` (No), pending default `0` (NoButton).

- **Normal (17):** worker disposition `returned`, OS status 0, Boundary 0, Returned 1, adapter code 0. The worker log says the enabled writer returns normally. The primary output reflects the successful remesh write; fallback remains unchanged.
- **Denied (18):** parent’s write-denial lease remains active through stopped-child inspection (the test checks `lockHeldAtInspection`). Receipt has `candidate-process-terminal`, OS status `3221226519` (`0xC0000417`), Started+Boundary, no Returned, no supervisor stop, profile/dispatch/failure counts `1/1/0`; primary remains the exact initial `DONE\n` and fallback is unchanged. No adapter-return marker is present. This is the candidate’s fixed owned-worker endpoint, not the native CRT invalid-parameter handler path.
- **Unknown (21):** denied writer setup, but no Boundary; worker returns with OS status 0 and adapter code 7 (`M2M_E_UNSUPPORTED`), profile/dispatch/failure `1/0/0`. The retained adapter receipt gives the existing remesh guard message.
- **Custom (22):** same expected fail-closed mapping as Unknown: Returned 7/OS 0, profile/dispatch/failure `1/0/0`, no Boundary.

Each case preserves raw difference paths in `raw-differences.txt`; they include the project XML and the five prior Save outputs. In each `actual-prefix` directory, the five outputs are byte-size/hash identical across all four cases and match retained B133 values: MST 7,196 bytes `9bad363c…5ed75ef1`; Object KEY 2,411 bytes `7adeee16…02f94e1`; Boolean BWZ 182 bytes `0d696c7c…eb00ca8`; ObjProperties KEY 169 bytes `2dc78ce7…6fa0f4a`; SimCtrl KEY 2,111 bytes `6f0dc482…897fe15`. The full hashes and bytes are checked by the worker test according to its implementation receipt. `LOCAL_REMESH.DAT` appears only in normal’s differences; denied/Unknown/Custom preserve it under the active lock. No normalization is implied by these raw lists.

## Source/protocol review and limits

The worker implementation receipt describes a separate explicitly opted-in RemeshConversion child role, RemeshV2 profile, own-image identity, suspended/job-supervised launch, deadline/capture bounds, and validated argv/case selection. The supervisor’s frozen header/source hashes remain `186c3e7b…6730bce3` and `20a18180…791a8b3b`. The child records the actual `saved.call` snapshot at remesh dispatch; the test compares it with `expectedSnapshot(mode)`. For the terminal case, receipt `returned_code=0` is a default field because there is no Returned frame and must not be read as adapter status.

The corrected classifier source hash is `1d359121…c481db2`. Its correction properly keeps RemeshV2 adapter Returned code separate from OS exit: Returned codes 0–7 require OS 0, while SafetyV1 keeps its legacy equality rule. The four retained outcomes are consistent with that mapping.

Qualification remains `constructed-remesh-overlay;retained-B133-raw-prefix;candidate-fixed-endpoint;native-default-handler-prelude-unimplemented;no-native-Close-claim`. These cases do not establish a fresh original enabled-remesh occurrence, native handler prelude, native Close, public CLI/C ABI behavior, or deferred project-false/MST-false/publication-failure/endpoint-return conversions. The corrected protocol classifier’s tests are separate evidence and do not expand this worker’s four retained case results.

## Required closeout

CP108 evidence is sufficient for the CP108 frozen source and four retained cases. Do not transfer that pass to the later CP109 worker source without using CP109’s own source-bound receipts. No source, build, test, GUI, or original-app action was performed for this review.

