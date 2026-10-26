# Earliest public active-study Save guard

The earliest shared DOE Save guard belongs to `moproj/write.cpp::subProjectElement`, called by the actual native/recording `.moproj` writer. Releasing the Type2 arm requires an integrated **owned Study output sequence at this call site**, before the new Sub_Project replaces the old DOM node and before `.moproj` text is emitted. The Type3 arm has different outputs and should remain separately guarded.

This follow-on audit was requested after the CP63 backend-to-output report. Only this note and uniquely named bounded public-Save evidence were written. No source, build, tests, GUI, original execution or mutable REA changed.

## Actual public caller selects study effects

Native `0x1400983c0` calls `0x140099040` at **`0x140098cb6`** with actual Project, its DOM, empty Name, active SUB type and **sixth argument 0**. The flag store is `0x140098c8c: mov byte ptr [rsp+28h],0`; type is written at `0x140098c91`. This is not the snapshot/copy caller's sixth-argument-1 profile.

Inside `0x140099040`, `0x14009963c..646` tests this sixth argument. Nonzero skips Type2 study effects; the separate Type3 gate does the same. The public Save's zero flag reaches those arms. Type predicates `0x140010330` and `0x140010360` compare SUB `+0x18` to exactly 2 and 3, respectively; they are distinct cases.

Current `saveProjectFileBody` obtains `activeSubProject(p)` and calls the shared `subProjectElement`; both native and recording Save use it. The function has already built local ViewTransform/dispView/item nodes before its unconditional Type2/3 unsupported exception. That exception currently precedes all native study-node and study-file effects.

The public native caller has already opened the `.moproj` QFile WriteOnly before this point. Native and current FileDevice paths may therefore have truncated that file before reaching the DOE guard. Earlier root DOM title/unit/operation/item-list updates also precede this call. Keep these partial effects; do not move study output to a later all-or-nothing commit or restore the file on refusal.

## Exact Type2 arm

The native arm obtains the **active SUB's manager pointer at SUB `+0x20`** through `0x1400b5a90`. A qualified null manager skips the arm and continues to common Subproject_DbName output. A non-null manager is cached for the arm and used in this order:

| Order | Effect | Confirmed native call |
| --- | --- | --- |
| 1 | Create `<SfDoeStudy name=manager.name>` and append it to the local new Sub_Project | `0x1400d29d0` at `0x1400996ce`, followed by appendChild |
| 2 | Obtain actual Project DoeStudies directory and save InputTable | `0x1400d3200` at **`0x140099770`** |
| 3 | Save current selected samples to `<DoeStudies>/<name>/Control/DEF_DOE.PAR` when that helper's conditions permit | `0x1400d75a0` at **`0x1400997b2`** |
| 4 | Save design mode 0 to `Control/Samples_LHS.par` | `0x1400d7790` at **`0x140099a77`**, EDX=0 |
| 5 | Save design mode 1 to `Control/Samples_FF.par` | `0x1400d7790` at **`0x140099d7c`**, EDX=1 |
| 6 | Save design mode 2 to `Control/Samples_USR.par` | `0x1400d7790` at **`0x14009a081`**, EDX=2 |
| 7 | Append common `Subproject_DbName`, return new Sub_Project | shared suffix at `0x14009a442` |

The file helpers' ordinary Boolean results are not tested to cancel this arm. InputTable false still proceeds to current/PAR helpers. A reached unsupported callback/exception/fault is a distinct failure and retains effects already performed. The named PAR files are conditional producers; do not force their existence when the original helper skips invalid/empty state.

Only after `0x140099040` returns does the parent replace/append the new Sub_Project in the existing Project DOM. The parent then streams the full DOM to the already-open `.moproj`, closes it and queries QFile error. Thus study files precede final `.moproj` bytes, and a failure during study output can leave the old Sub_Project node in the DOM while other earlier DOM/file effects remain.

InputTable includes the CP63 chain `dimension reconciliation -> empty-variable cleanup -> QFile open/DOM serialization/close/error -> signalDoeTableRefresh`, even when the writer returns false. Both notifications and file status must retain their own native order and state; none is implied by the CP60 backend registry.

## Type3 is a different arm

Type3 requires **both** optimization pointer SUB `+0x28` and manager pointer SUB `+0x20`. If either is a qualified null, it skips these study/optimization effects. Otherwise it creates/appends the same study element (`0x14009a167`), computes the `.OPT` target from the SUB path, Project base name and literal `.OPT`, saves InputTable (`0x14009a3a1`), then invokes optimization writer `0x140907a10` at `0x14009a3ec`. It does not use Type2's current/LHS/FF/USR sequence here. Its return is stored without an aborting Boolean branch, then common Subproject_DbName output follows.

Do not lift the current combined guard for Type3 merely because Type2 ownership is available. Optimization document construction, path and serializer need their own owner/effect qualification.

## Smallest integrated source action

For Type2, bind the existing Project-owned `SubProject::ownedDoeStudy`, `sub.doe` alias, `Study::sub`, existing `Project::doeIo`, and the actual ConversionSession/current-model-manager/notifier identities at the current shared call site. The native arm caches the original manager; later callback changes must not silently reroute remaining writes to a different active Study. Lost native lifetime/ownership should stop at that reached boundary after preserving prior effects, rather than using an extra diagnostic shared lease as proof that the original object survived.

Root coordination is required for `moproj/write.cpp`, its shared interface/callers and actual DOE writer/notification entry points. Make this an integrated public orchestration increment, not another independently asserted backend profile. Keep existing prior DiagramViews requirements and Type3 guard.

The existing standalone `doe::saveInputTable`, `saveCurrentSamples` and `saveSampleTable` cannot simply be appended into a local FileEffects bucket and discarded: `saveProjectFileNative` currently passes a local `unused` bucket to the body. Native study writes must occur at their reached source phase, survive later refusal and be visible to subsequent readers. Recording Save needs the corresponding ordered simulation in `SaveEffects.project`, not synthetic native success.

`StudyIo::writeTextFile` currently returns **open success**, with separate write/close observations, matching PAR contracts. InputTable instead returns QFile error after close. Do not reuse that open-only Boolean as the XML result. Existing native OutputDevice/Text-mode/error semantics or an explicitly qualified study-file interface must retain that distinction; no temporary replacement, prevalidation rollback or deferred bulk write should be introduced.

A first integrated Type2 increment may append the qualified owned study node and reach the exact next unavailable InputTable/notification boundary, retaining public nonempty-variable and unresolved callback guards. Complete active DOE success additionally requires actual variable finalization/serialization, sample state/output, backend/population ownership and the later main Save/DOE-output producers. No general B121 success follows from the study-node prefix.

## Required integrated checks

- Exercise the real shared `subProjectElement` through native/recording project Save with an actual owned Type2 Study, explicit view inputs and current Session identities; do not test only standalone writers.
- Trace `.moproj` open before DOM/study effects, study-node append, InputTable cleanup/open/close/notification, current/PAR modes 0/1/2, DB-name append, old-node replacement and final `.moproj` stream/close.
- File-open false for one study writer must retain error/partial state and continue when the native caller ignores false; a later unsupported message/owner failure must retain all earlier effects and omit only unreached phases.
- Verify missing/foreign Study ownership refuses before an invented node/output; source-qualified null native manager remains a separate skip branch. Release during notification must not reroute to another SUB or revive a dead manager.
- Keep Type3 guarded and preserve existing Type0 behavior. Public nonempty DoeVariables, scalar delivery and SetOprHasDoe Population guards remain until the actual dependent paths are closed.

## Evidence receipt

Original PE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `analysis/resume63-doe-public-save-receipt.json` hashes `287341c8e6d4a04cebde69c2b24584c1719d506c08d4764f930a1fbdc3e6d4f9` and identifies five new bounded Microsoft dumpbin 14.44.35228.0 selections totaling 26,534 bytes: public caller arguments, Type2 gate/first effects, Type3 arm, exact type predicates and three disjoint PAR call/argument windows. These windows do not claim whole surrounding callgraph or runtime closure.

Retained `evidence/gui_mo/out_scope/save2.c` (SHA-256 `b0ff7f6d90ddb4636eba8b447d8649e6a109857b66f24ce53eb81dc508820290`) supplies provisional complete arm structure; the decisive gates/calls/mode arguments are independently confirmed in the new native selections. Current `moproj/write.cpp` SHA-256 is `6df9a21b4dd9847b50ad17ddc3af48252ed49ce11369b36267abbc6f614d3299`.

No fresh-original acceptance, complete dynamic notifier population, sample regeneration, complete XML/OPT serialization or end-to-end active-study Save is established. Token measurements are unavailable.
