# CP124: private Spring checkbox-off / empty-list Apply cut

Implement the private Spring Save gate when the page checkbox is off or the item list is empty. Native Apply bypasses the current-item callback, record snapshot and writer on this branch, then attempts readable-delete of SPRING.INI followed by SPRING2.INI. Current candidate Apply instead refuses a populated unchecked page and an explicit checked empty list before scalar sidecar effects. This is a source-closed private page increment, independent of combo labels and numeric parser spellings. Full public Editor300 construction remains behind its palette gate.

Only this new report was written. No source/test/shared-header/CMake edits, builds/tests, GUI, original execution or mutable REA operation occurred. Existing source and retained decompile were reconciled with bounded offline DUMPBIN and PE/import reads. Original DEF_GUI_MO_64 v11.0.2 SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Exact native decision and effects

The retained `wave2-save-diffs-300-support-140ec9040.c` already contains the checkbox/count gate after NBC, SW2SP, STRETCH, B_WEIGHT and STRAIN_DST handling. Direct instruction inspection confirms its ordering and excludes payload/callback reads on the deletion branch:

| Preferred EXE address | Effect |
| --- | --- |
|`140ec96f2..96ff`|Load UI+1610 QListWidget; call IAT141e05bd8 count(); store returned int at rsp+48.|
|`140ec9712..971f`|Load UI+15f0 checkbox; call IAT141e05d68 isChecked(); zero-extend AL.|
|`140ec9724`|Unchecked jumps directly to989e.|
|`140ec972a..972f`|Otherwise compare count with0; signed JLE jumps989e.|
|`140ec974b..975c`|Only checked positive count calls currentItem through IAT141e08b60, then140ecdee0.|
|`140ec9761` onward|Only that checked positive branch computes count*0x40, allocates/copies payloads, and invokes record writer.|
|`140ec989e/98a5`|Load literal142b14fd8=SPRING.INI; call1407fbc10.|
|`140ec98aa/98b1`|Load literal142b14fe8=SPRING2.INI; call1407fbc10 regardless of first return.|
|`140ec98b7..98d5`|Common local cleanup and normal return; neither delete result is tested here.|

Imports were resolved directly from the original PE import table:141e05bd8 is QtGui4_64.dll `?count@QListWidget@@QEBAHXZ`;141e05d68 is `?isChecked@QAbstractButton@@QEBA_NXZ`;141e08b60 is `?currentItem@QListWidget@@QEBAPEAVQListWidgetItem@@XZ`. This is a checked/count decision on real native controls, not a label, itemData or model-ID decision.

Helper1407fbc10 passes the supplied narrow path and literal14385aee4=`rt` into its open routine at7fbc25. Null at7fbc2f returns1 without removal. Nonnull closes at7fbc43, then removes at7fbc4d, returning whether removal result equals0. Its caller above ignores either result and always reaches the second path. The existing NativeExtra3DIo::removeIfReadable already models this readable-open/close/remove contract. Reuse that API; do not introduce an unconditional filesystem remove, transaction, warning or rollback. Broader CRT/file encoding/error-profile equivalence is not newly claimed by this gate.

Crucially, skipping the record path skips old-row writeback too. Unchecking a populated list does not zero flags, clear records or commit edited controls first. No list item, numeric edit, checkbox-reversible, combo index or text parser is consumed after the off/empty decision. This page checkbox is distinct from each record's flag. The earlier toggled->groupBox40 enable routes do not need to be modeled or claimed for this private explicit-value Save cut.

## Minimal modern C++ implementation contract

In applyExtra3D, create a Spring snapshot only for `form.spring && form.spring->pageChecked && !form.spring->items.empty()`. Keep current checked/nonempty admission, transition/save snapshot and writer branches intact. For unchecked or empty explicit state, leave the snapshot absent and use the existing final SPRING.INI then SPRING2.INI remove calls after scalar effects.

Do not run supportedSpringItems, validate currentRow/listCurrentRow, require comboLabels or parse visibleNumbers on that deletion branch. Those values are not consumed there. A typed empty vector represents native count0; no negative count needs to be invented. The adapter's candidate limits still apply where its checked positive record path consumes payloads; they are not native validations. No schema/header field change or new persistent state is needed.

An unchecked explicit page can hold invalid numeric text, unknown/missing combo context or a nonfinite record payload without this branch reading them. This is not admission of those profiles to the record serializer. Rechecking the same populated page must still reach the existing guard. The const page parameter must remain unchanged: disabling a checkbox does not mutate stored item payloads or clear the list during Save.

The source still performs scalar sidecar effects before it reads list count and checkbox. The small adapter keeps existing reconstruction preflight for excluded checked-positive profiles, which does not claim native partial effects for them. For the newly admitted off/empty profiles, no early Spring refusal should suppress those prior scalar effects.

## Proposed exclusive write set and focused tests

Request only:

- `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`.
- `source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp`.
- A new CP125 implementation receipt.

No shared header, lifecycle, public loader/factory, native reader, CMake or build changes are required. Root builds/integrates after release. The existing focused target uses Release-active Testkit checks and short verified C:/PROBLEM leaves.

Meaningful regression cases:

1. Fill a populated private page, retain a nonzero companion, set pageChecked=false, then poison visible text/current indexes or remove combo context. Apply completes, writes no Spring main/companion, and attempts deletion of main then companion after earlier scalar actions. Preserve the full input page/list/flags. Physical disposable files verify both are removed when readable.
2. Explicit items.empty() with pageChecked=true and missing combo context follows exactly the same removal path. This distinguishes an actual empty-list value from absent optional page while exercising the newly reached branch.
3. First Spring readable-delete returns false (spy or a read-share/no-delete-sharing handle), but second is attempted and succeeds. Earlier scalar actions and normal Completed outcome remain; no new NBC warning is caused by Spring deletion failure and no rollback occurs. A spy records order independently of host deletion behavior.
4. Recheck the poisoned positive-count page and require the existing UnsupportedSnapshot before Spring writes. Checked valid no-edit/edited/flagged writer tests remain unchanged.
5. If a scalar write is enabled, record its attempted write before the two Spring removals, proving that this branch does not return early at the checkbox gate. Continue after an ordinary prior write failure under the existing helper contract.

A failure to read a Spring file on a future public Open is separate; these tests exercise an already supplied private page and actual Extra3DIo. They must not be described as full Session Open->Save or native GUI checkbox events. The public PrivatePaletteUnavailable /6504abb0 guard, selected owner/atomic witness, font/counter/setup and actual enabled-widget effects remain unchanged.

## Static receipt

Direct PE ranges, end exclusive:

| Range | SHA-256 |
| --- | --- |
|`140ec96e3..140ec9761` gate and reached current-item call|`9c340268356018b42be30d064b5c541c8588ce36602c02abbd22af42c819f2e2`|
|`140ec989e..140ec98b7` ordered deletions|`cecbb0b5b26c6a7e6a3a79b69caebd47e000e9dbe42ee89d1e4770a699c2e79d`|
|`1407fbc10..1407fbc6d` complete readable-delete helper|`49fba24e4c861dbafec4e6778c25a3725cc0ba7a36133a2adbd344bae0b0fdab`|

Verified DUMPBIN path: `C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50` (banner14.44.35228.0). Reproducible `/DISASM:BYTES /RANGE:<start>,<inclusive-end>` selections are96e3..9760,989e..98b6 and7fbc10..7fbc6c on the original EXE. Only the analysis tool executed; no target function/process was executed. No additional raw capture file was created under this report-only assignment.

Current io.cpp SHA-256 `493f6e2bf21b62c377be13a8829ee1acb849da36a639f5715258ed7eee9b33dd`; page test SHA-256 `8c7b177283317028998c9952251a13dfff5d1a827e0d04908c526b5871c41c9e`. Retained decompile140ec9040 SHA-256 `d08b47a4529b3d50e7c1cf4a8247ba98c09f3f4d78efee8212b44f7dcc6e4b32`; keyword map SHA-256 `d6f9b6468cf0c6ffcbdead738371c0b1ae7378f6b2a97a81e9a081340bd660f2`. Older CP98/102 positive-item refusal reports are superseded by CP114-119; their correctly identified off/empty outer gate had not yet been integrated for an explicit populated/empty SpringSelectionState.
