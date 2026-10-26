# Fresh BILLET capture preflight from case 004

## Decision and provenance

Case 004 is an original **failed-prefix** observation plus a separate successful candidate observation. It does not contain an original Save or Close result and must not be described as a completed original-vs-candidate lifecycle comparison. Existing raw receipt `comparisons/raw.json` says `original=crashed`, `candidate=completed`, `raw_content_equal=false`; the only byte-changed project file is `project/BILLET.moproj` (both 5,201 bytes, first difference at byte 3,428). It also records metadata changes across cwd and project files. Those facts are final filesystem comparison only; they do not imply candidate parity or explain the original crash.

Original identity in case 004: `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, size 67,611,136. Original session was PID 13316, launched 2026-10-23 11:21:14.014Z from `build/reference-harness/billet-training-004/sandboxes/original/cwd`, fresh process and no previous project, but existing settings were retained. The GUI first rejected a forward-slash filename in its native chooser; after dismissal, a backslash path selected the project. This is a documented training protocol deviation. The first captured fault was `c0000005` at `QtCore4_64.dll+0x12a0e4`, followed by `c000041d`; process/window disappeared naturally. No registers or call stack were captured, so cause/caller remain unknown. Save key was attempted after the first fault and Close could not act on the absent process. Do not attribute the fault to Save or Close.

The candidate is a different event and identity: case 004 frozen `moproj2mst.exe` SHA-256 `dea5f846e2117e02ab8917ddb08b2331feca10b644b73232502de270334fa89d` (3,562,496 bytes), DLL `facb8b34cc8e2d5f94cd41b0307c6f9f9f93788e786bbd14f8209c055ecaf980` (3,567,104), `deformdb.dll` `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3` (216,576). It ran later, 11:25:01.815Z, with `--write-save`, exit 0. This adapter completion cannot stand in for original evidence, nor prove internal cleanup by itself.

## Frozen inputs and hashes

Case root: `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\reference-harness\billet-training-004`.

- Fixture source: `evidence\gui_mo\s1\corpus\06_BILLET`; project `BILLET.moproj`; source fixture manifest `fixture-before.json` was captured 2026-10-23 11:20:35.592Z and the prepared copy root is `inputs\`.
- Exact project input: `BILLET.moproj`, SHA-256 `650b448be5e765c0eaf1e32ef70bf4f7280173b10c8767b023b12e11762a29c1`, 5,201 bytes.
- Companion `BILLET.MST`: SHA-256 `62829836c3b9c8c1e710c066a8dea0d14872874546e46ba2580d19a4cf99e3de`, 2,184 bytes.
- Other inputs: `Equipments/Equipment00001.KEY` `6fa08567ca10794a3f145e9297cd215367a03fc9b24b5c0954d154a5a4233f51` (824 bytes); `Materials/Material00001.KEY` `9a59dc725b43f13bbd8939cbf8e485b00d06d906218c0635f0246214639ac343` (11,966); `Objects/Object00001.KEY` `4a658de4a37973bcce5e6a73aa0a7f2639e6afe97a41ddcb3ef75bb1f9160d0f` (486,412); `Operations/Task00001/InterObject.KEY` `c61c21876c76d7439af50478e52810fc1a41bda0c326e142d81a791f32fbbd8f` (63); `ObjProperties00001.KEY` `16edca97bc62d6979c2db3af30917c13264bb54af7e2d0529aacecc734a458dd` (169); `SimCtrl.KEY` `f0152aaa70d71d72c9066ebe70efe8246e000a39996b5bcefcdf0f9e153e1591` (2,225).
- Case 004 source snapshot was `build/wave2-geometry-source`, 2,998 files, source digest `3d9024b16b39a6893b4f325e60db6e680a0ccac86b67e654a2677e860dec2fb1`, source content digest `90f4ede815960f8a1cb412f6dc865e948b82ffb65b87b94b39492cefc189e2fe`. Its build receipt `analysis/wave2-geometry-v1-build.json` is locally consistent with these artifacts, not independent compiler attestation.
- `case.json` binds original executable and binaries above. Frozen receipt SHA-256 is `811266d91e917af74733de0b5cde73806ae29838f96a565ca1a1f40cb50f6354`.

These hashes are historical case-004 receipts. A new capture must rerun harness preflight and validate current files/manifests; do not treat recorded hashes as a present-time identity check.

## Exact disposable layout and preflight sequence

Keep a new attempt append-only under `build/reference-harness/billet-training-005`; the intended runtime paths are:

- Original cwd: `...\billet-training-005\sandboxes\original\cwd`
- Original project: `...\billet-training-005\sandboxes\original\project\BILLET.moproj`
- Candidate cwd: `...\billet-training-005\sandboxes\candidate\cwd`
- Candidate project: `...\billet-training-005\sandboxes\candidate\project\BILLET.moproj`
- Runtime outputs/evidence: `...\billet-training-005\evidence\` (retain harness-generated snapshots and receipts in the case tree).

Before any original launch, copy/prepare from the pinned fixture into a new case, run `preflight`, and confirm exact input set and hashes; no absolute/escape/missing selected XML references; original EXE hash and loaded module identities; no existing `DEF_GUI_MO_64` process; license/startup availability; observed current locale, ANSI code page, settings and startup unit; target process cwd and exact backslash project path; no project already open. Fresh process means no project history, not pristine user settings. Record settings as found; do not clear them as part of this preflight. Since the harness receipt's path is long, measure the actual project path before launch and preserve the exact path used; changing to a shortened alias would change the tested environment and must be recorded if used.

For the original run, record phase evidence for actual File>Open, Save and Close and the actually displayed default answer for any dialog. Select only the observed default. Capture screenshot/message, PID, argv, cwd, code page/locale, settings snapshot, loaded module identities/hashes, process exit/close state, and post-run project + cwd manifests. Record observable external writes such as recent-project settings and license logs; explicitly mark unobserved registry/filesystem channels and intermediate write order unknown. No retry in the same case. If the original crashes, preserve the last action/phase and exception evidence, write a failed-prefix original receipt, and stop the original attempt; do not label later attempted key presses as successful lifecycle actions.

Only after an original receipt exists may the frozen candidate run in its separate candidate project/cwd with full `--write-save`, followed by its own receipt. Candidate `started_utc` must be later than the original receipt. If original crashed, candidate execution is optional diagnostic output and must remain explicitly separate; compare may show raw filesystem deltas but cannot make an acceptance pass. Never infer the original's missing Save/Close results from candidate exit 0.

## Expected outcomes and decision labels

1. **Complete comparison eligible:** original demonstrably reaches Open, Save, Close using observed defaults; candidate runs afterward in isolation; receipts and raw manifests exist. This is one training case result, still not broad acceptance.
2. **Original crash/refusal/timeout:** save original failed-prefix receipt, exact phase and partial effects; no parity verdict. A candidate result, if separately retained, remains candidate-only evidence.
3. **Preflight failure:** no original launch. Preserve input/hash/path/module/licensing discrepancy and repair preparation in a new case ID.

Case 004 currently matches outcome 2, not outcome 1. Its original `save-observation.json` and later key-attempt trace must not be merged into a successful Save stage because the process had already faulted.

## Capture risks

- The immediate post-Open QtCore fault is reproducible evidence of a risk but has no captured registers/stack and no known caller. Static export proximity to `QObject::metaObject` does not prove which application path called it.
- The native chooser rejected `/` path syntax once. Start with the exact Windows backslash path and retain chooser evidence; do not silently count the earlier failed selection as project Open.
- Current settings persisted through case 004; a fresh process is insufficient to claim fresh settings. Record them and treat them as a possible confounder.
- GUI recent-project and license-manager writes were observed historically; project/cwd-only manifests are not an external-write audit.
- Harness freeze/build receipts are local consistency checks. Recheck hashes and case freeze before execution. Supplied dependency hashes do not prove full loaded-module closure.
- If a supported app-control release cannot be established with a receipt/status, do not begin another controlled GUI interaction. This document is a preflight only; it acquires no control and starts no process.

No original app launch/control, candidate execution, build, source edit or REA database mutation was performed for this preflight.
