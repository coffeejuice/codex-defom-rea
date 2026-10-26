# CP125: private Spring off/empty Apply gate

Private Extra3D Apply now accepts an unchecked Spring page or an explicit zero-item list without reading item controls or payloads. It preserves earlier scalar sidecar effects, then attempts readable-delete of SPRING.INI followed by SPRING2.INI. Public Session/editor construction remains guarded at its actual palette producer; this is not full Session Open->Save or a native checkbox-event acceptance result.

## Source-qualified change

The controlling contract is `analysis/resume124-editor300-next-cut.md`, SHA-256 `629707f5d681abb38305391044b24fc9a48b2fc156c37d3dd99d01b5626c0739`. Original DEF_GUI_MO_64 v11.0.2 has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Native count is read at140ec96f9; checkbox at140ec9719; unchecked at9724 or signed count<=0 at972f goes directly989e. Only checked positive count reaches currentItem974b and old-item writeback975c. Main removal98a5 and companion removal98b1 run in order without testing either result.

The implementation creates a record snapshot only for a present, checked, nonempty private page. Off/empty skips supportedSpringItems, current/list row validation, combo context, numeric parsing, selected-record flag normalization and all payload copies. It reaches the existing scalar handlers, then the existing absent-snapshot removal tail. The page is const and remains unchanged. Rechecking a positive page still invokes all existing record/transition guards and writer behavior. No reader/parser, API, lifecycle or public constructor behavior was broadened.

The source change is +6/-4 lines against CP124. Checked-positive reconstruction preflight remains before scalar effects for excluded profiles; no native partial scalar effect is claimed for those refusals. The newly admitted off/empty path does not have that early Spring refusal.

## Focused cases and qualifications

The existing page target is `m2m_s_spring_page_io_roundtrip` / CTest `m2m.s.spring_page_io_roundtrip`, now19 Release-active cases (15 retained,4 added):

- An unchecked populated page with absent combo context, invalid row indexes/text, a nonfinite payload and raw flag2 completes without consuming or changing them. Rechecking the same page is refused with no I/O. Actual disposable main/companion files are deleted in the off case.
- A checked explicit empty list with no combo/selection context deletes both files without reading its invalid controls and leaves the page state intact.
- A deterministic Extra3DIo spy denies the primary readable-delete; the companion is still attempted/deleted, the original main bytes remain, Completed is returned and no Spring warning/NBC warning is introduced. This is injected ordinary helper failure, not a newly observed host/native removal failure.
- A deterministic earlier NBC open failure is recorded before subsequent scalar removals and ordered Spring removals. Apply still completes and the actual disposable Spring files are removed. The injected failed scalar has no backing output file; no native failure residue is asserted for it.

SpringWriteTrace gained opt-in full-operation recording and two narrow failure switches; its default observation behavior is retained for earlier cases. Tests use the existing verified short C:/PROBLEM leaves and only remove the known owned Spring files/empty leaf. Diagnostic scratch and cleanup receipts retain the existing component-level qualification. No temporary project or sidecar was created by this worker because tests were not run.

## Static preflight and freeze

The assigned test was verified before editing: the real path is `tests/s/test_spring_page_io_roundtrip.cpp`; no mdt counterpart exists. Root was notified of the corrected directory. Ownership stayed within that existing test and io.cpp, plus this new receipt. No shared header/source outside the pair, CMake, build artifacts, GUI, original process or mutable REA was changed.

Static checks compared both files against `build/wave3-checkpoint124-verified/source.zip`, inspected the complete small production diff and new test paths, checked quoted include resolution, supported Testkit macros,19 unique TEST_CASE names, one main, no C assert/REQUIRE_EQ, balanced braces and whitespace. `git diff --check` produced no errors. No compiler/build/CTest execution occurred; these checks do not establish compilation or a test pass. Root owns grouped focused/full validation and archive binding.

Frozen source/test:

| File | SHA-256 | CP124 text diff |
| --- | --- | --- |
|`source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`|`720828fd1a77cef0e3b6fbb7201c40be86fb847fe603f2e2f638ef8075045ee0`|+6/-4|
|`source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp`|`7e652d40e4e9463391a469fb61e0552d6110764cc087a1866ab08b7bc24a2df2`|+95/-3|

Files are released for independent review and root build, with no further worker source edits until root releases the next window. No reconstructed executable/deployment was inspected or produced. The native palette/current-ref witness, actual settings receiver enablement and full public Editor300 constructor remain unresolved separately.
