# CP64 Type2 Save integration design and shared ownership

The smallest safe public-writer increment is to replace only the Type2 arm's blanket refusal with an actual owned-study binding, append the qualified SfDoeStudy node to the pending new Sub_Project, and stop at the next precise InputTable boundary. It can use already implemented studyElement values without enabling unsupported I/O or notifications. Type3 stays separately guarded. Full Type2 output is not available by calling existing FileEffects planners and discarding their writes.

This lane wrote only this report and bounded source citations. No source/shared state, build/tests, GUI, original execution or mutable REA changed. Native call order is controlled by resume63-doe-public-save-order.md and the independent CP64 verification in resume64-doe-save-effects.md, not by assumptions inferred from candidate code. The source excerpts are frozen in resume64-doe-type2-source-citations.txt with exact lines and per-file hashes.

## Candidate owner map

| Native/candidate role | Current actual owner and available binding | Required integration rule |
| --- | --- | --- |
| Save process/main window | ConversionSession created in convert.cpp188; Ctx.session bound for WRITE_SAVE | Pass this same borrowed Session to the shared writer; do not create a second Session at Save |
| Project | session.project(), checked against session.liveProject() | Check identity before dereferencing retained aliases; external I/O leases do not keep Project alive |
| Active SUB | Project.subs unique ownership; activeSubProject lookup at saveProjectFileBody512 | Resolve once for entry, validate actual membership/backlink/type2, then preserve this source-selected identity |
| Native manager SUB+20 | SubProject.ownedDoeStudy shared owner and sub.doe raw alias; makeSubProject301-307 commits both | Require ownedDoeStudy and matching alias and Study.sub==that SUB; null alias alone is not a qualified native-null manager observation |
| Study name/data | Study fields and samples; studyElement495-500 writes only SfDoeStudy/name | Borrow the actual reached values; do not duplicate a Study or treat a diagnostic shared lease as native lifetime proof |
| Existing DOE I/O | Project.doeIo; actual native/recording sink selected at convert193 | Require the already bound sink, never lazily construct/rebind one in the writer |
| Model manager | ConversionSession.modelManagerOwner shared into StudyIo at convert199 | Same owner/identity; current model/Project are separate and must be queried at reconciliation's reached native point |
| Messenger | ConversionSession notification owner bound to StudyIo at convert195 | Same actual notifier, with key-specific population; SetOprHasDoe or empty-key fixtures do not qualify DoeTableRefresh |
| File effects | SaveEffects.step2 then SaveEffects.project in recording; StudyIo also records immediate/simulated attempts | Native writes occur at the actual phase; recording writes become visible to subsequent reads immediately and remain in ordered receipts |

Retain the manager identity cached by the native Type2 arm. After any reached callback, validate the same SUB/Study is still owned before continuing; do not reroute later phases by re-reading activeSubProject. If another SUB becomes active while the original Study remains alive, native caching is not permission to switch managers. If the cached manager dies, preserve reached effects and guard. Only weak/identity observations can validate lifetime; an extra shared_ptr alone must not revive the native object.

Do not require a synthetic current Project for node creation or an empty-variable reconciliation: native0d2b60 returns immediately when variables are empty. When variables exist, it resolves the **actual current manager Project**, which is not automatically the Study's Project, then operation/base-object dimensions. A current-model substitution or unconditional same-Project requirement would change native branches. Re-query at each native source point rather than precomputing all later paths/names before callbacks.

## Exact public Type2 sequence and candidate gaps

| Native phase | Candidate implementation state | Required reached effect |
| --- | --- | --- |
| Main .moproj WriteOnly open | saveProjectFileNative opens FileDevice before body551-559 | Failed open returns before DOM mutation; success can truncate before later DOE refusal |
| Root title/unit/DB/operation/item DOM updates, diagram view/item nodes | write.cpp495-513 and112-131 already implement these | Preserve existing view guards and earlier partial mutations |
| Study node append at996ce | studyElement exists; combined Type2/3 guard currently132-133 prevents call | Append to **local pending new SUB**, not old Project DOM; owned name, no new native-constructor claim |
| InputTable dimension reconcile0d2b60 then empty-variable cleanup91c820 | saveInputTable322-328 only guards nonempty vars; writeInputTable280 guards vars/constraints | Empty vars has no reconciliation/cleanup mutations; nonempty guards at that reached boundary until complete variable/current-owner logic exists |
| InputTable QFile open, DOM/locale text stream, close, QFile.error | Current writer appends prebuilt bytes to FileEffects and returns true | Add a distinct XML device/status seam; actual DOM serialization follows successful open and uses reached Study values |
| signalDoeTableRefresh after writer result, even false | Existing empty-notification helper excludes this key | Actual registration exists; guard before unknown delivery after preserving any XML/error/cleanup effects |
| DEF_DOE.PAR current mode; LHS0; FF1; USR2 | Standalone serializers/conditional planners137-162 exist; immediate StudyIo.writeTextFile already exists | Continue after ordinaryfalse; mode/invalid gates and row formatting are phase-local, not a prevalidated batch |
| Subproject_DbName, return new node, replace old node, stream .moproj/close/error | write.cpp134-141 and514-539 | Only after all preceding phases return; prior study files are not rolled back if later writing fails |

Correction to earlier broad 'empty' wording: valid empty sample tables **write a mode header**. Only invalid state skips the selected PAR producer (fixed-mode helper also rejects invalid index; current-mode out-of-range is separately unrepresented). Native915360 returnsfalse only on open failure; successful open returns true after writing/close without querying QFile.error. InputTable90a550 instead uses post-close QFile.error. The Type2 caller ignores ordinary helper Booleans and proceeds. An unsupported callback/owner refusal remains a reached stop with earlier effects retained.

The independent native CP64 capture qualifies an actual DoeTableRefresh registration at0xebc5c, receiverthis from owner0xeb6f0, flag0, encoded source slot1slotUpdateUI(), key2signalDoeTableRefresh(). The existing whitelist cannot add this as empty delivery. Complete actual receiver construction/death/population and slot effects are needed; no registration-candidate string is runtime completion.

## Why existing I/O and FileEffects cannot simply be reused as success

Native saveProjectFileNative passes a local unused FileEffects into the shared body and discards it. Appending study outputs there would lose them while still claiming progress. Recording Save passes SaveEffects.project, but a deferred list alone would not make a study file visible to a later same-conversion StudyIo read. Native output cannot wait for convert's later applyEffects or end-of-Save batching.

StudyIo.writeTextFile supplies PAR's open-success Boolean and records write/close observations. RecordingStudyIo immediately updates its simulated write map, which subsequent reads consult; native sink truncates/writes in place. This mechanism is useful for PAR after the correct reached phase, with no second native application of its receipt. FileEffects.project can retain the corresponding recording write/attempt in source order, but cannot be a throwaway or a second executor.

InputTable needs an error-capable XML output device/observation. Existing mst::FileDevice separates opened(), close() and error(); OutputDevice alone has no virtual error accessor. Existing writeTextMode supplies Qt Text-mode translation. Reuse those qualified mechanisms through a study-specific owner seam; do not replace post-close QFile.error with std::ofstream.good or PAR's open-success Boolean, hardcode success for recording, or prewrite UTF-8 when the original uses the reached locale profile. Preserve open-failure global error mutation and refresh even after false. Exact error literals/status ownership must remain source-qualified.

## Smallest root-owned source slice now

Wire an optional actual Type2 Save context through the **existing shared entry points**, preserving signatures/defaults for unrelated Type0 callers and the C ABI. The context should carry actual Session, already bound I/O and an ordered caller-owned report/sink; it must not perform admission or I/O at construction.

At the current combined guard in subProjectElement, separate Type3 (unchanged refusal) from Type2. For Type2, validate actual Project/SUB/Study aliases at that reached point, cache the manager identity and append studyElement(*study) to the local new SUB. Then invoke a staged InputTable prefix: nonempty vars stop at unrecovered reconciliation; empty vars proceed to the exact not-yet-bound XML-open interface boundary. Do not call the old planner as native success, produce PAR files or replace the old SUB node. A test-visible pending-node/phase receipt may retain the local prefix, without inserting it prematurely into the public DOM or claiming a completed native QObject.

This increment wires the public shared Save callsite and makes the next dependency exact; it deliberately does not claim public Type2 Save success or output files. The next real file increment can integrate the empty-vars/constraints0 XML writer through the new device seam and stop **after** its reached result at the still-unqualified refresh delivery. That requires the additional I/O/writer files below, and retains XML truncation/partial bytes/error state on refusal.

### Exact shared ownership root must coordinate

| Increment | Existing files root must own |
| --- | --- |
| Entry context and actual shared Type2 node/prefix | include/m2m/moproj_write.hpp; src/moproj/write.cpp; src/engine/save_project.cpp; src/core/convert.cpp |
| Durable caller-owned trace/result, if added to existing state | include/m2m/engine.hpp (SaveEffects/FileEffects) and/or include/m2m/common.hpp (Ctx), only for the chosen explicit owner; avoid duplicate ledgers |
| First XML file/status + staged InputTable orchestration | include/m2m/doe_io.hpp; src/doe/io.cpp; include/m2m/doe_study.hpp; src/doe/doe_study.cpp |
| Actual DoeTableRefresh dispatch after XML | include/m2m/doe_session_notifications.hpp; src/doe/session_notifications.cpp plus whichever exact receiver/constructor owner evidence assigns |
| Later immediate sample/PAR wiring | same DOE Study/I/O files; source-qualified serializers can be separated into new exclusive helper/test files, but public caller remains root-owned |

New isolated Type2 context/stage/report helper files and one focused test can be delegated separately after root fixes the public interface. No model.hpp or conversion_session.hpp edit is required merely to create another study/manager/notifier owner; existing owners should be reused. Native lifetime witnesses may require an explicitly assigned later interface if current identity checks cannot represent callback death.

## Integrated fixture boundary

Use real saveProjectFileNative/saveProjectFile through the shared body with an actual ConversionSession-owned Project/Type2 SUB/Study/I/O and explicit DiagramViews fixture. Match ModelManager/notifier owners to that Session. Use empty vars/constraints for the bounded file-producing profile; select current sample mode1 and all three tables valid with distinct integer rows, plus a separate valid-empty table case. The full future sequence is XML,refresh,DEF_PAR(mode1),LHS0,FF1,USR2,DBname,DOM replacement,.moproj bytes/close. Check failed XML open still reaches refresh; failed PAR open proceeds to later modes; callback refusal preserves prior files and old SUB DOM; owner loss does not reroute.

A root writer fixture can test the initial stage today: real .moproj open/truncation and earlier DOM updates occur; pending node carries actual Study.name; precise InputTable refusal leaves old SUB DOM in place; no study file/PAR or later main-file bytes are fabricated. Type3 and Type0 behavior need separate regressions. Missing/foreign Study alias or I/O rejects at its reached point, and DiagramViews failure remains earlier.

There is a separate public-selection limit: read.cpp141 keeps constructor SUB0 type0/id0, and convert.cpp222-223 selects/activates SUB0. B120/B127 Type2 SUB1 tests require explicit internal selection and are **shared-writer integration**, not proof of public C ABI Type2 selection. Do not change default selection to1 to make a fixture pass. Public Type2 Open-to-Save success additionally needs a source-qualified way to reach the originally selected Type2 within the permitted lifecycle and the existing B121/DBLIST/lazy-study dependencies; preserve the C ABI as thin compatibility adapter.

Freeze UTC: 2026-10-24T21:58:50.924809+00:00. No executable validation occurred.

| Evidence/source snapshot | SHA-256 |
| --- | --- |
| `analysis/resume63-doe-public-save-order.md` | `354b25eb69b39d8aa0896fbe2ffd3d12d84f0f6de824599c7da1483ddf432335` |
| `analysis/resume63-doe-public-save-receipt.json` | `287341c8e6d4a04cebde69c2b24584c1719d506c08d4764f930a1fbdc3e6d4f9` |
| `analysis/resume64-doe-save-effects.md` | `68c8a8f29fa6d19631d3db7ad9a3d114dcf28f0c369285877c040a996380d18c` |
| `analysis/resume63-doe-backend-next.md` | `2d3235fa49f86498463db8898247b0264f344cbdcbc968af48766efa36d87889` |
| `analysis\resume64-doe-type2-source-citations.txt` | `23f5a831d5bc33958930226f92d207fd31cb0741febd15b84707db07c9884625` |
| `analysis/wave2-doe-session-notifier-direct-key-candidates.json` | `1a947bb2daf487a9e9d8aee1999d09675f2abb6202b3e526ef1847fcbc6cf0d9` |
