# CP227 editor title owner-command review: PASS

The bounded change preserves the supported editor title event and moves its final native write into `GuiModel`. No blocking defect found. This is a source review, not a build, test, freeze, original-runtime parity, or ownership-closure result. Root owns integration validation.

## Reviewed contract

Read subtree `AGENTS.md`, takeover/receipt/state context, corrected `resume227-editor-leaf-design.md`, implementation report, and the relevant CP222 alias/closure qualifications. Reviewed the four assigned file diffs, their surrounding implementations, the complete focused title test, Testkit definitions, and bounded existing geometry/Stopping test and configured-target metadata. CP225/226 hunks in the shared model files are pre-existing and outside this change.

- `simcontrol300_owner.cpp:280-299` retains binding/state/page/model/remesh/delivered-text checks, then the 80 UTF-16-unit bound, printable-ASCII admission, then `qt::toAscii`, then the model operation. `toAscii` in `src/qt4/string.cpp:98` maps every admitted unit to exactly one identical byte. The command's repeated size/ASCII checks cannot reject an event admitted by this path or move its reached geometry refusal earlier than conversion.
- `gui_model.cpp:215-226` rejects invalid direct input before accessing model state. For admitted input it calls `requireGeometrySyncAvailable()` before freshly resolving `step ? &step->sim_ctrl : nullptr`, before null/identity rejection, and before writing. This matches the old `simCtl()` barrier/lookup position. A parked geometry barrier still throws even with an absent step or mismatched expected identity when admitted title input reaches this command. The expected pointer is compared only; it is never dereferenced. False retains `UnsupportedSnapshot`; true retains `Completed`.
- The native title is 81 bytes (`cls_1401e7b10.hpp:38`). The implementation copies `ascii.size()` bytes and explicitly stores one NUL at that size. This is not literally the previous single `memcpy(size + 1)`, but gives the identical NUL-inclusive write footprint and order for the admitted editor path: no padding or write after the terminator. It also avoids reading one byte beyond an arbitrary non-NUL-terminated `string_view`, and skips the zero-length copy for an empty view. The 80-byte maximum keeps the final NUL in bounds. Direct invalid ASCII, including embedded NUL/control bytes, and oversized input cannot cause a store.
- The declaration returns only a boolean, with an explicitly documented legacy identity token. It adds no getter/native alias result. It does not establish opaque identities, remove public legacy aliases, or complete the CP222 closure gate.
- No CP227 change reaches the C ABI/header/layout/version, `simcontrol300.cpp`, Stopping predicates, cache1 storage/publication, widget construction, or event delivery order. Existing cached-Stopping tests assert `u26c4[11] == 0` after covered refusals and retries; those assertions remain intact and require root's regression run.

## Test assessment and limits

The new direct-command case checks successful copy, null expected identity, 81-byte refusal, high-byte refusal and unchanged title storage across refused calls. Existing event tests meaningfully cover 80-byte success with adjacent-field sentinel, 81-byte failure, trailing-byte preservation after a shorter copy, null/empty text, repeated events, invalid text and event binding, missing current control, replaced control with both old/new titles preserved, and expired/partial editor state. They use the existing Testkit entry point and supported macros; no C `assert` or `REQUIRE_EQ` is introduced. Required includes resolve in the source/target include paths. Configured CTest metadata registers `m2m.mdt.simcontrol300_title_changed` and the cached-Stopping executable.

There is no new dynamic parked-title test. The simple title fixture does not install the private parked barrier; the existing viewer test constructs a much larger geometry fixture and verifies `simCtl()` refusal. It does not directly exercise the new command. Barrier placement for this leaf is therefore established by source inspection, with existing geometry and cached-Stopping runs serving as regression guards rather than direct proof of the new title event under parking. This limitation is accurately reported and is not evidence for opening cache1 continuation. No build or tests were run by this reviewer.

Reviewed SHA-256 values match the implementation report:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/engine.hpp` | `8AEC169587DF42D3AD2BD5617EDB6BACE793FC4CAFD6C5DE1554099645AD75A5` |
| `src/engine/gui_model.cpp` | `28C842A0F6DCDE166C5DA129F5A8A095D99F6AD398F889A0A0B4687126ABBE82` |
| `src/engine/simcontrol300_owner.cpp` | `D7D865527B8252AD1C4BD6D272F51D55002FBDBCFAAB5FDCBDEDE958FF2A534F` |
| `tests/mdt/test_simcontrol300_title_changed.cpp` | `74E3289EDADE355DFEE8F7BEBB29B2A4A07742134EABC074297ADCE69B19CE1A` |

Only this review report was written. No source, test, build, shared-state, original process, GUI, or REA mutation was performed.
