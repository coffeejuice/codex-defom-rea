# CP110 opt-in CLI Open→Save and qualified terminal relay

Add one explicit research CLI route for a selected project's existing Open→Save conversion in an owned fresh worker. Preserve the existing six-export ABI1 and default CLI route, including their returned7 remesh guard. A validated worker terminal outcome may be relayed by terminating only the owned CLI process withC0000417, after a durable receipt. This matches that external status value; it does not recover the original handler prelude, accepted normal Close or complete external parity.

## Evidence and scope

CP108 established normal/guarded/terminal worker separation; CP109 adds prior project/MST returned failures and fail-closed backend cases. The retained CP109 run is `build/test-runs/1792909048264141700`, linked from `analysis/resume109-worker-failure-grouped-tests.json`; its actual worker target passed in2.12317s. The source freeze is tree `2af75283ba49105d5b145609126edc8c40dcf669d3029f4a30b6fbd0636ec39a`, archive `c6552c464404c0ca9822e24d70c95bd912c31359904c0f6a724d83ed6a4f2b69`. Eight worker cases use a constructed B133 remesh overlay and retained historical raw expectations; they are not fresh original enabled-remesh acceptance.

The selected native status is source-qualifiedC0000417; this lane has not observed a fresh original crash. CP110's separate native-handler model keeps StaticFreshDefaultAssumption distinct from a same-original-module witness and stops at CaptureContextUnimplemented. Consume that qualification at the actual leaf request, never promote a FreshDefault enum or matching process code to an observed original handler.

## Exact public CLI contract

Proposed explicit invocation:

    moproj2mst <absolute-project.moproj> [absolute-out.MST] --write-save --isolated-save --isolated-receipt <new-absolute-receipt.json> [--strict] [--codepage N] [--write-mdt] [--deform-dir absolute-dir]

This is a design, not a command executed here. `--isolated-save` requires existing `--write-save` and a new receipt path. First-cut paths are absolute; omitted output retains the core's default MST calculation. Deform directory is absolute or omitted. Reject invalid combinations before launch. This narrower explicit mode avoids silently changing the legacy relative-path behavior. Default argv without the new mode still calls m2m_convert_w exactly as today; existing --version/usage behavior must stay unchanged.

The parent snapshots codepage(default1252), guiUnit0, raw supported flags, deform directory and M2M_MO_INPUT_MODE once, with the same signed-int32/error contract as the existing ABI settings snapshot. Pass OriginalSettings explicitly to the worker; do not reread mutable environment settings in the child or force mode0. Nonzero mode retains its existing editor300 guards. Capture the initial caller CWD and launch the child there; core Open then selects its ordinary project/current directory. Do not pre-open/save the project in the parent or change output path semantics inside the worker.

No ABI header/enum/API version/export changes are needed. The preserved six names are m2m_api_version, m2m_set_codepage, m2m_set_gui_unit, m2m_set_deform_dir_w, m2m_convert_w and m2m_convert_utf8. No public C call installs a terminal service, and no product user can select the test fault cases17..24.

## External outcomes and status rationale

| Validated outcome | Worker facts | CLI action |
| --- | --- | --- |
|Returned|complete v2 Returned, adapter code0..7, worker OS0, no supervisor failure|commit receipt; present normal diagnostics; return adapter code0..7 |
|CandidateProcessTerminal|complete qualified Boundary, actual natural workerC0000417, no Returned/backend/protocol/timeout/parent-stop failure|commit durable receipt, close controller-only resources, then TerminateProcess(GetCurrentProcess(),C0000417) |
|Isolation/backend/protocol/receipt failure|no complete trusted outcome, or a failed endpoint/commit|ordinary CLI-only exit65; never relay native status |
|Timeout|deadline expired, owned child stopped by supervisor|ordinary CLI-only exit66; never relay native status |
|Argument rejection before child|invalid new-mode parameters/settings|existing argument code1 |

An alternative reporting exit64 plus a typed receipt is safer than pretending it is native status, but it is **not** external OS-status parity. The recommended first cut uses the explicit relay row instead. CLI-only65/66 do not extend the M2M C enum. In particular, Testkit harness exit0 for a matched expected terminal case must never become the product CLI result.

The terminal receipt records worker termination **observed**, source site/qualification, prior SaveSnapshot and parent relay **requested**. It cannot claim its own future termination was observed. A surviving external test controller must record the final CLI status, normalized to32 bits: unsigned3221226519/signed-1073740777/hexC0000417. The relay itself omits native context/filter/debugger/WER processing and is not a claim that the original handler ran in either candidate process.

If receipt commit or relay execution fails, there is no successful external-parity claim. Return65 where control remains available, retaining the accurate worker observation and relay-request qualification. Do not rewrite the immutable record to claim an unobserved parent outcome.

## Production worker/job contract

Use a distinct private product role, not the test worker's case-driven entry:

| argv | Meaning |
| --- | --- |
|0|verified absolute own CLI image|
|1|`--m2m-isolated-save-worker-v1`|
|2|`--allow-owned-isolated-save`|
|3|protocol-write HANDLE as uintptr decimal|
|4|read-only job-mapping HANDLE as uintptr decimal|
|5|nonzero uint64 nonce decimal|
|6|bounded job byte count decimal|

There is no arbitrary executable argument, custom-handler selector or fault-injection case. Child requires exact argc/role, valid inherited pipe/mapping, matching nonce/schema/size and a valid parent-ownership context before conversion. Root owns only this own-image launch; neither original DEFORM binaries nor unrelated processes are selected.

The immutable bounded job(max64KiB) contains schema/version/total bytes/nonce, parent PID plus inherited query/synchronize-only parent handle, expected own image identity, codepage/guiUnit/flags/moInputMode and length-prefixed UTF16LE project/output/deform/initial-CWD values. Reject overflow, inconsistent lengths, embedded path NUL, trailing bytes, invalid flags/settings and non-WRITE_SAVE. Parent creates the mapping without a filesystem backing file, fills it before launch and passes a read-only child handle. Do not synchronously fill a small job pipe while the child is suspended. Paths remain data, never shell text.

The worker uses the existing private core service/forwarder and the same Session owner. Its production backend selects only StaticFreshDefaultAssumption, validates the complete leaf request at dispatch, copies the forwarded SaveCallState to v2, publishes Boundary and calls the fixed own-process endpoint. It does not import test fixtures/oracles/ledger logic or scripted project/MST errors. The separate native model can label the decision/prediction, but model evaluation cannot satisfy a process-terminal classifier or advance past its prelude guard.

PublicationFailed/EndpointReturned map explicitly to failure tags1/2. failClosed publishes best-effort once, then the released writer parks; the external supervisor stops it. Backend failure never invokes the expected terminal status to disguise failure. A normal return publishes code0..7 and optional returned Save state, then exits OS0. The worker renders normal Result.message with the existing CLI message/truncation convention onto its bounded captured diagnostic channel before final completion. Do not silently drop warnings/errors merely because FrameV2 contains no message string.

## Process, capture and ownership policy

Do not directly expose the test supervisor as a product runtime: it requires M2M_TEST_SCRATCH and a build-directory ancestor and implements test cases/fixtures. Preserve those guards. Reuse the pure v2 protocol, and extract only policy-free owned-process mechanics into a private shared runner, retaining the test wrapper's exact defaults. Product policy supplies immutable job/settings and actual selected project paths; it has no prepareOwnedCase copying or hidden remesh overlay.

Launch suspended, restrict inherited handles to protocol/job/parent ownership/std streams, assign kill-on-close job, then resume. No fixture locks are inherited from an outer test. Parent holds explicit job/process handles and one deadline(default15000ms for the first qualified cut, plus at most2000ms stop cleanup). That limit is an adapter boundary, not native cancellation. Do not kill by image name or retry/resume conversion after timeout. Parent death closes its owned job; all surviving-child uncertainty is a reported failure.

For production, use bounded in-memory stdout/stderr capture through separately drained pipes rather than a test scratch directory. Poll fairly across protocol/stdout/stderr in bounded chunks, reading only available bytes and checking the deadline between chunks. Cap each capture at1MiB and classify cap/partial/out-of-order data as supervision failure. A flood cannot starve protocol/deadline handling. Test this production transport; the passing file-capture test is not proof for the new memory-pipe arrangement. Pin the own CLI and relevant candidate dependency identities(actual loaded paths/hashes, e.g. deformdb), not a guessed system-wide installation.

Normal return unwinds candidate adapter scopes as it does today. It is **not** normal native Close. Terminal dispatch occurs before writer/core unwinding, earlier writer-error mapping and any later Close entry. Parent owns no Project/Session and must not run normal Close or regenerate/rollback project outputs. Original accepted Close remains unrecovered in either outcome; report Open→Save only. OS process-handle release is not fclose or source destructor equivalence.

## Durable receipt and project-write safety

The selected project and ordinary Save destinations are written by the child at their existing native source points. There is no copied product project, transactional wrapper or restoration. Earlier effects survive terminal/timeout/failure according to actual reached writes. The parent must never turn a diagnostic commit into a project write.

Require a new absolute receipt file, an existing local regular directory, no reparse/namespace ambiguity, canonical separation from the selected project tree and explicit output target, and no existing file/alias. Reject protected evidence/original/install paths in the research integration. Validation reports belong in this checkout; DEFORM test projects and generated files stay in owned short C:/PROBLEM leaves. Do not add C:/PROBLEM or this checkout as hardcoded product project destinations; these are the validation locations.

Do **not** create/reserve a receipt file in the project or before worker execution, and never overwrite an existing destination. After confirmed child stop, recheck parent-directory identity/containment and target absence. If the child or another actor created that target, fail65; do not overwrite a possible project effect. Create an exclusive random sibling temp, write bounded versioned UTF8 JSON, FlushFileBuffers, close it and install the final new name without replacement and with write-through semantics. Failure leaves an accurately labeled diagnostic/temp where safe and prevents relay. This also avoids making a previously absent project input available before Open. Disk/OS failure remains qualified; there is no hard real-time guarantee for failed local storage.

Terminal path performs no potentially blocking stdout/stderr forwarding after durable commit: stop the already verified CLI process directly. The requested file is the authoritative external receipt. Normal/failure paths may render bounded diagnostics with normal CLI conventions; do not interleave child stdout with machine-readable receipt bytes. Report fields separate modeled handler evidence/stage/next(CaptureContextUnimplemented), actual worker facts, optional adapter result and parent relay request. No single success Boolean may conflate them.

## Smallest safe implementation and ownership

First freeze a private runner/job/receipt contract, then integrate one production route with no test fault selectors. Proposed exclusive sets:

- Root: src/cli/main.cpp parser/dispatch, CMake target source registration and CLI documentation. Main routes exact private worker role before normal parser; old default path/ABI code remains unchanged.
- Process owner: NEW src/core/owned_cli_process.hpp/.cpp, factoring launch/job/available-byte deadline mechanics from terminal_harness_supervisor.cpp only under explicit exclusive ownership. Existing test wrapper keeps scratch/profile policy; rerun v1/v2/role suites after extraction. No concurrent edits to that supervisor.
- Job/receipt owner: NEW src/cli/isolated_save_job.hpp/.cpp, bounded mapping schema/settings parser and durable non-overwriting receipt commit. Root agrees same environment snapshot semantics without altering public ABI behavior.
- Worker owner: NEW src/cli/isolated_save_worker.hpp/.cpp, private role validation, core service backend, normal diagnostic/result transport and qualified native decision projection.
- CLI supervisor owner: NEW src/cli/isolated_save_supervisor.hpp/.cpp, user request policy, common runner invocation, typed outcome and post-commit relay decision. Root and this owner agree all function signatures before parallel edits.
- Acceptance owner: NEW tests/g0/test_cli_isolated_save.cpp and unique receipts; no production/test fixture sharing with worker fault selectors.

The minimum product vertical cut is: default-route exact guard, isolated normal return, and isolated denied remesh with durable receipt then externally observed CLI relay. It also requires adverse transport/timeout/receipt safety and export/settings checks below before activation. A type, mode flag, copied harness or working private backend alone is not product integration.

## Required acceptance

1. Hash-bound real CLI normal control on a fresh copied fixture: normal adapter0, worker OS0, correct returned Save state, expected raw outputs, diagnostics, receipt, no terminal relay. Preserve the constructed-overlay/historical qualification where used.
2. Same input with outer test's read-compatible primary write denial: valid actual v2 Boundary, all-success snapshot1015, worker naturalC0000417, durable receipt complete before external controller observes CLI C0000417; unchanged denied primary/fallback and five pinned raw prefix outputs. No adapter return marker or normal Close claim. Inspect only after both owned processes stop, then release outer locks/cleanup.
3. Without --isolated-save, the same denied route must still return7 normally. Exercise both public C exports wide/UTF8 on that fixture and verify the exact six-export list/API version remain unchanged. Private product worker token must not become a new DLL export.
4. Invalid mode combinations/settings/argv/job/nonce/role/length cannot start conversion. Default and opt-in settings snapshots match; nonzero MoInputMode retains its existing refusal. Unicode/spaces remain length-prefixed data; absolute-path requirements are explicit new-mode guards.
5. New capture runner rejects half-frame stalls, protocol mixing/conflict, flood/cap, unexpected status and failure frames. Timeout/backend failure exits65/66 with own-stop status in receipt, neverC0000417; previous project effects remain, no rollback/Close. A terminal-looking status without Boundary cannot relay.
6. Receipt aliases to project/output/existing files, changed/reparse parent, failed flush/final install, or a child-created target cause65 and no terminal relay/overwriting. Parent cannot label relay observed before killing itself. A self-relay failure must not be reported as a normal native outcome.
7. Retain CP109 earlier project/MST false and declared backend-return matrix, v1 safety11/v2 protocol10/role5, native model guards, writer/reader/errno regressions, required full launcher and source-stable freeze. Product dynamic dependency/binary hashes are bound before/after; matching exit alone is insufficient.

Only this design report was written. No source/test edits, build/tests, GUI, original/native target execution or REA mutation occurred. Native default-handler context/filter/debug effects, same-original-run handler witness and accepted normal Close remain explicit gaps after this proposed route.
