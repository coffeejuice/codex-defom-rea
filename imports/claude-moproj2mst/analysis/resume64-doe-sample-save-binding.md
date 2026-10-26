# Checkpoint 64: Type2 current-sample Save binding

## Native phase and owner

The active public native Save reaches the Type2 arm through `0x1400983c0 -> 0x140099040` at `0x140098cb6`, passing the real Project, active SUB type and sixth argument zero. `analysis/resume63-doe-public-save-order.md` orders that arm after new `SfDoeStudy` creation and InputTable saving. The current-samples call is `0x1400d75a0` at `0x1400997b2`; it precedes the three separate `0x1400d7790` sample-table calls for modes 0, 1 and 2 at `0x140099a77`, `0x140099d7c` and `0x14009a081`. `EDX` is respectively 0, 1 and 2. The final shared `Subproject_DbName` suffix follows at `0x14009a442`.

The manager operand is the active SUB's `+0x20` pointer, first checked/cached by the Type2 arm. A qualified null manager skips the study arm and reaches the common DB-name output. For a non-null manager, the same cached manager supplies the name and all later sample writes. The project-side binding represented in current source is `SubProject::ownedDoeStudy` with `SubProject::doe` as its alias; `Study::sub` points back to that SUB. Before invoking the writer, the shared public call site must qualify `sub.type == 2`, `sub.project == &p`, an actual owned Study, `sub.doe == sub.ownedDoeStudy.get()`, `study.sub == &sub`, and the existing Project-owned `p.doeIo`. After the preceding InputTable/notification phase, continue only against this same selected Study and same manager identity; do not look up a newly active SUB/Study and reroute the remaining writes.

## Current-sample conditions and bytes

The source port in `source/apps/moproj2mst/src/doe/doe_study.cpp:150-162` reflects the helper contract:

1. `saveCurrentSamples` reads `Study::samples.mode` (`+0x40` in the native manager). If the mode is outside 0..2, it refuses at the unsupported boundary associated with `0x1400d75a0` before any current-table file is planned.
2. It builds `<DoeStudies>/<Study.name>/Control/DEF_DOE.PAR`, then delegates using that selected mode.
3. `saveSampleTable` skips only an out-of-range explicit mode or a table whose `invalid[mode]` state is true. **A valid empty table still writes**: the file contains the mode number plus CRLF and no data rows. Do not skip it as “empty.”
4. `writeSampleTable` emits the mode header, then one-based row numbers, a tab after each value, and CRLF per row. This is a serialization of retained rows; this helper does not regenerate samples or mutate the sample state.

The three later design files use that same per-mode validity condition and corresponding table. The native helper's ordinary return is not used to cancel the Type2 arm. Static CP63 evidence qualifies invalid-table omission and mode arguments; current repository source supplies the adapter's byte construction. No new original binary disassembly or original runtime sample capture was made for this task.

## Effects and required public wiring

Native public Save has already opened the `.moproj` file WriteOnly before entering `subProjectElement`; root Project DOM updates and local ViewTransform/display/item construction also precede the DOE arm. The Type2 source sequence creates/appends the study node, attempts InputTable, then current samples, then mode 0/1/2 files, and only afterward appends DB name and returns the new Sub_Project. The parent replaces/appends that Sub_Project and serializes the existing Project DOM only after the child function returns. Therefore:

- Each reached study file must take effect at its native phase. A later refusal/fault preserves the opened/truncated `.moproj` and all earlier study writes; it does not replace the old Sub_Project node or emit final Project DOM bytes.
- Ordinary helper false results do not stop the arm. In particular, an InputTable false still reaches current-sample and mode 0/1/2 helpers. An invalid sample table skips only that one output; it does not cancel subsequent mode attempts.
- A PAR open failure is an attempted write with its own failed-open observation and the caller continues; after successful open, in-place truncation can leave a partial file if writing fails. The native `0x140915360` helper returns false on open failure, but after successful open it performs its write/close and returns true without post-close error conversion. Do not report close errors as an ordinary false result for PAR. InputTable has different QFile post-close error behavior and must keep its own result semantics.
- Recording Save must expose equivalent ordered simulated writes through the public `SaveEffects.project` stream. It cannot rely on `saveProjectFileNative`'s local `unused` FileEffects value, which is discarded after writing the Project DOM.

The current `doe::saveCurrentSamples` / `saveSampleTable` APIs only append complete payloads to `engine::FileEffects`; `saveProjectFileNative` calls `saveProjectFileBody` with a local `unused` bucket. Thus calling those helpers at the public phase alone would lose the file effects and collapse failure/ordering semantics. Integrate at the shared `subProjectElement` orchestration boundary with the active `ConversionSession`, retained `SaveEffects` output and Project/Study ownership checks. The existing `StudyIo::writeTextFile` contract is open-success (write and close observations remain separate), suitable for preserving the PAR distinction only if its write path is wired at this point. The InputTable path needs its separate QFile post-close error result and the ordered refresh notifier. Keep the native manager cached across both.

A guarded integration can close only this writer slice: exact active-owner checks, append the already-qualified study node, then reach the first still-unresolved InputTable/notification boundary. Do not enable full Type2 Save solely because sample-table serialization is source-backed; public variable finalization/serialization, InputTable/refresh ownership, backend/population ownership, and later DOE outputs remain dependencies. Type3 remains separately guarded.

## Evidence scope

Native ordering and call arguments: `analysis/resume63-doe-public-save-order.md`, bound to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; bounded windows and receipt are `analysis/resume63-doe-public-save-call.asm` and `analysis/resume63-doe-public-save-receipt.json`. Source qualifications inspected: `source/apps/moproj2mst/src/doe/doe_study.cpp`, `include/m2m/doe_study.hpp`, `include/m2m/doe_io.hpp`, `src/doe/io.cpp`, `include/m2m/model.hpp`, and `src/moproj/write.cpp`. No source/build/test/GUI/runtime/REA changes were made. No fresh-original Save acceptance or complete Type2 ownership closure is established.

