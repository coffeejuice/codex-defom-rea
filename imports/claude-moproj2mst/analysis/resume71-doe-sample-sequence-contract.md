# Checkpoint 71: Type2 current/LHS/FF/USR sequence

An isolated four-call sequence can use the actual owned Study and its Project's StudyIo without enabling public Type2 completion. The selected mode must come from that cached Study after the InputTable wrapper and synchronous refresh have returned. The public writer remains guarded before this sequence while actual refresh delivery is incomplete. Ordinary XML/PAR false is not a reason to skip the subsequent calls.

This lane is read only apart from this report. The current sequence helper is root's in-flight source; its presence is not a frozen build or acceptance result.

## Exact retained native order

The original image SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Existing bounded CP63/64 captures suffice; no new capture or mutable REA is needed.

At `0x14009969a`, the Type2 arm calls `0x1400b5a90` on the active SUB. It caches the returned manager in stack slot `[rsp+0x90]` at `0x14009969f`. A qualified null skips the study arm. A nonnull manager remains the receiver of the following calls; the code does not reselect another active manager after notification.

| Order | Native call site | Effect/continuation |
| --- | --- | --- |
| 1 | `0x1400996ce -> 0x1400d29d0` | Study element appended to the pending Sub_Project. |
| 2 | `0x140099770 -> 0x1400d3200` | InputTable wrapper: reconciliation/cleanup, XML writer, unconditional refresh, then return the saved XML Boolean. |
| 3 | `0x1400997b2 -> 0x1400d75a0` | Selected current mode to `Control/DEF_DOE.PAR`. |
| 4 | `0x140099a77 -> 0x1400d7790`, EDX=0 | `Control/Samples_LHS.par`. |
| 5 | `0x140099d7c -> 0x1400d7790`, EDX=1 | `Control/Samples_FF.par`. |
| 6 | `0x14009a081 -> 0x1400d7790`, EDX=2 | `Control/Samples_USR.par`. |

The call/continuation windows do not test returned AL to cancel the next step. The InputTable wrapper saves XML's Boolean, sends no-argument `2signalDoeTableRefresh()` at `0x1400d369d`, then returns that saved value. A failed open or post-close XML error still reaches refresh; if refresh returns normally, the caller proceeds to the selected PAR. A guard/exception during refresh is distinct: later PAR calls are unreached, while earlier XML/error/open effects remain.

After these calls return, common Subproject_DbName construction follows. The parent installs/replaces the pending Sub_Project only after the helper returns, then completes the already-open `.moproj` stream. Four PAR calls alone cannot prove that tail or full Save completed. Type3 has its own OPT sequence and must not acquire Type2's four-call behavior.

## Selected mode and table predicates

The complete current helper `0x1400d75a0` supplies the precise selection contract:

1. At `0x1400d75c8`, read manager `+0x40` as the current signed mode. At `0x1400d75d4`, inspect `manager+0x70+mode`. Nonzero returns ordinary false without path construction or file open. There is no fallback to another valid mode, row-count check, or sampler invocation.
2. When admitted, build the actual Control directory using the manager name. At `0x1400d772c`, reread `+0x40` for the table address `manager+0x58+8*mode` (instruction `0x1400d7738`), then reread mode into ECX at `0x1400d774a` and call `0x140915290` at `0x1400d774d`.
3. `0x140915290` appends exact `DEF_DOE.PAR` and delegates to `0x140915360`, propagating its Boolean. No selected-mode assignment occurs here; it observes the manager state already established by preceding work.

The current mode is therefore not a pre-refresh snapshot or a constant zero. Source `Study::samples.mode` represents native `+0x40`; the invalid array represents `+0x70`, and the three tables represent `+0x58`. In the admitted, nonreentrant isolated profile, sampling mode once at entry to the current stage is sufficient. A future implementation that permits state-changing callbacks between the native reads must preserve the reread points or guard that unsupported profile. Original current mode is unchecked before indexing; the candidate's explicit unsupported guard outside 0..2 preserves an unrecovered/undefined boundary, not a claimed native false return.

The fixed helper `0x1400d7790` is different: bounds checks at `0x1400d77b0/77b7` admit only 0..2, then `0x1400d77c8` checks that exact mode's invalid byte. Invalid/out-of-range returns false without I/O. Valid selects that exact table at `0x1400d77db` and calls the writer at `0x1400d77e9`. The public fixed arguments are 0, 1, 2 regardless of selected mode.

Valid empty tables still write their mode header. For example current mode 1 with all three valid empty tables yields DEF header `1`, LHS `0`, FF `1`, USR `2`, each followed by Windows CRLF under the qualified ASCII/Text profile. Invalid current mode 1 skips DEF and FF, but does not prevent a valid LHS0 or USR2. This explicitly retains CP64's correction to CP63's loose "invalid/empty" wording.

## PAR Boolean and immediate effects

Native writer `0x140915360` opens QFile WriteOnly|Text at `0x1409153ca`, before stream formatting. Failed open returns false. Successful open writes a mode header, sequential one-based row IDs, tabs after IDs and each value, and line endings; it closes at `0x14091568d`, then sets true unconditionally at `0x140915693`. It does not gate true on stream status or post-close QFile.error, and does not assign XML's global error string. The sequence ignores that ordinary Boolean for continuation.

Do not reuse XML's post-close Boolean rule for PAR. Do not create missing directories, stage temporary replacements, retry short writes, roll back previous files, regenerate samples, clear invalid flags, or alter counts in this Save sequence. Successful opens can truncate at their reached phases. A later failure retains earlier output. Neither requested PAR bytes nor an open-success Boolean establishes native durable content.

## Isolated helper binding and boundaries

The inspected in-flight root helper is `saveType2SampleSequenceImmediate(const Study&, const String& doeStudiesDir, StudyIo&)`, declared in `include/m2m/doe_study.hpp` and defined in `src/doe/doe_study.cpp:164–191`. Its separate states SkippedInvalid/OpenFailed/Opened and ordered four result slots correctly avoid conflating invalid skips with failed opens. A false result does not cancel later calls. It reads selected mode at the current stage and uses fixed 0..2 afterwards. It deliberately performs no refresh delivery or public Save.

The direct reference interface alone cannot prove the Study or I/O belongs to a live Session. An isolated test should supply an actual ConversionSession Project, its owned Type2 SUB/Study and the Project's real StudyIo, then call the helper as an explicitly isolated stage. No diagnostic "refresh succeeded" Boolean is evidence for public continuation.

Future shared binding must occur only after actual refresh returns normally. It should revalidate the cached manager's lifetime and existing identity relations, rather than obtain a new Study from the then-active SUB:

- Session's live Project is the original Project, that Project owns the original SUB, SUB.project agrees, and the original owned Study still exists with `sub.doe == ownedDoeStudy.get()` and `Study.sub == &sub`.
- Use the weak cached Study identity from the earlier Type2 prefix, not an extra shared lease held across notification as proof of native survival. A released/replaced manager refuses at this reached boundary after earlier effects; no safe native release continuation has been established.
- Project's actual StudyIo remains the same intended owner, its bound ModelManager is Session's actual owner, and any changed Project/SUB/manager identity is handled before dereferencing stale state. Do not synthesize another sink or callback-driven replacement Study.
- Obtain the Study's mode/table state after refresh, not before it. Preserve the native cached-manager semantics even if actual global current SUB changed during refresh. Native path-root/name acquisition occurs at its source phases; the isolated helper's single Control path computation assumes unchanged path/name over its bounded call, and is not evidence for a reentrant mutation profile.

Current `src/moproj/write.cpp` reaches the real Session DOE-table owner refresh guard and then retains a Type2 guard. Do not append this isolated helper past that refusal or catch the refusal to continue. Neither B121/Type2 completion nor B127/Type3 completion is enabled by these tests.

## Existing StudyIo seam limitations

`saveSampleTableImmediate` currently serializes the entire body before invoking `StudyIo::writeTextFile` (`doe_study.cpp:158–161`). NativeStudyIo's implementation (`src/doe/io.cpp:191–201`) opens/truncates a binary ofstream, writes the supplied already-translated bytes, flushes/closes and records distinct observations; `recordTextWrite` returns opened even if later observations are false. RecordingStudyIo (`io.cpp:231–236`) writes the canonical simulated store and currently reports ordinary success. That is a useful immediate open-success seam for bounded sequence/order fixtures, but not native device/partial-write equivalence:

- Native formats incrementally after QFile open; current helper prepares bytes before open. Serialization failure/open ordering differs, so whole-body preparation must not qualify native partial effects in that case.
- Whole-body ofstream output does not establish native QTextStream locale/format/flush or per-line partial-write fidelity. Use simple exact integer rows/ASCII headers for bounded fixture claims; do not generalize them to every floating-point locale.
- XML fault profiles apply to XML devices only. They do not inject PAR open/short-write/post-close results. Custom test StudyIo can return false or record observations for sequence continuation, but that is a declared diagnostic driver, not original QFile proof.
- `StudyIo::textWrites()` is owned by StudyIo and separate from the shared XML event sequence. Its order establishes the admitted PAR attempt order during the isolated call, not one globally ordered native XML/refresh/PAR journal. The CP70 shared XML alias alone will not preserve PAR text-attempt diagnostics after StudyIo teardown. Broader public outcome retention needs a separate source-owned value observation design, without replaying requested bytes.

These limitations do not block an isolated helper that is labeled with them. The next native fidelity increment would be a source-qualified PAR output-device/stream seam preserving open before formatting and open-only return; it must keep normal true independent of write/close errors and use actual accepted recording spans if partial bytes are exposed.

## Focused tests and next dependency

Use mode1 and distinct integer rows in tables0/1/2. Assert DEF1, LHS0, FF1, USR2 paths, modes, exact bodies and invocation order through the actual supplied StudyIo. Cover valid empty headers, selected invalid with other fixed valid, all invalid/no attempts, unsupported selected mode before I/O, and an open-false result at each ordinal followed by later admitted attempts. A throwing diagnostic sink should show earlier effects retained and later attempts unreached; no normal aggregate result should be invented after interruption.

Separately verify actual Study/SUB/I/O identity and no public writer integration was enabled. Keep the exact upstream refresh guard and unchanged old DOM/public Type2 selection controls. Root owns builds and CTest; this lane ran neither.

Smallest ready action: freeze/test the isolated sequence helper with the above bounded StudyIo assumptions. Smallest missing public dependency: actual synchronous DoeTableRefresh owner/timer/UI completion followed by validation of the original cached manager. A rich PAR device and survivor journal are additional dependencies before claiming native partial-output parity, not permission to bypass refresh.

## Inspection receipts

Retained full capture files (not the selected-body hashes in CP64's receipt):

| File | SHA-256 |
| --- | --- |
| analysis/resume64-doe-current-samples.asm | 0564DB43B03C9A23DE7857D8E35DEFA39E081E2E6BFD768FF95BDC9B7CC7B74F |
| analysis/resume64-doe-mode-samples.asm | 30505E423D9DDBDC2A6C4F741929939707AD84B336E502C1620DD1C25C54DA9D |
| analysis/resume64-doe-sample-writer.asm | ECAAF324B126B1EA214130B721575D49F23E97FCD9910A4C4094FA1AAD4F608B |
| analysis/resume64-doe-public-call-receipts.asm | ABD3739C556BC3E8C908641BF922E30FA4192F09105E22B41E684F32555A1A1E |
| source/apps/moproj2mst/src/doe/doe_study.cpp (in-flight inspection) | 0EB68D61A8B0565E8F5EE08B5EC07C9A29425677903EA18C0DF83EA62DBBBDFF |
| source/apps/moproj2mst/src/doe/io.cpp | B8C71B9A5BE6FD13B444AF8D998DBAFA5A71A38E31ADCC72894145D5655E87F1 |

These reconcile `resume63-doe-public-save-order.md`, corrected `resume64-doe-save-effects.md`, `resume65-doe-refresh-owner.md`, and the current Type2 node/XML/caller implementation. No source, shared state, build, test, GUI, app, or mutable REA operation was performed. Only this report was written.
