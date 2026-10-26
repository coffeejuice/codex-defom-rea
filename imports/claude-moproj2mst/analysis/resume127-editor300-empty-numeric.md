# CP127: empty private Spring numeric edit

The private Spring transition now commits positive zero when an explicitly supplied numeric visible String has zero length and the existing Qt conversion reports failure. Empty and null Strings are admitted; nonempty failed conversions, whitespace, nonfinite values and range failures remain guarded. The transition keeps old-item writeback before same-row comparison or next-row load. Public Session construction/palette remains guarded.

## Source and scope

Controlling report: `analysis/resume126-editor300-next-cut.md`, SHA-256 `5335d5232ac2acb608adc2a639909a156a6f2f83bb8d9da8dbfc1879c63c0f32`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

Five native Spring numeric calls pass EDX=null before QString::toDouble at140ece015/072/135/195/1f5 and store XMM0 unconditionally into the local record. The IAT resolves to the exact QtCore6706c340 export. Zero-length preparation6704c190 returns false; conversion6704c340 returns +0 through XORPD at6704c407; QString's fallback repeats the same empty path. Item payload memcpy140ece257 follows conversion/flag preparation and precedes the same-row comparison140ece287. The pinned half-open native ranges/import proof are in CP126.

The existing qt::toDouble adapter already returns +0 with ok=false for this case. The patch changes only transitionSpringItem's failed-conversion admission to permit isEmpty(), while retaining its finite check. It does not implement a second parser, force arbitrary invalid values to zero, alter the Qt helper, or change payload snapshot/click order. Same-row Save retains empty visible text; a later row reload formats the stored zero as0. CP125's unchecked/zero-count branch still skips all numeric controls.

This is an explicit private page/value transition. It does not instantiate native QLineEdit, prove GUI validator/event delivery, model new QString/QLocale allocation/refcount effects, release the selected palette guard or establish same-run original/public Session acceptance.

## Focused regression additions

The existing `m2m_s_spring_page_io_roundtrip` target / CTest `m2m.s.spring_page_io_roundtrip` now has24 Release-active cases (19 retained,5 added):

1. Both empty and null visible String values, in each of the five numeric fields, commit +0 bits only to that slot while other finite edits, IDs and checkbox flag writeback retain their expected values. Same-row return keeps the exact null/empty visible distinction.
2. Row0->row1->row0 commits empty-as-zero before the click switches rows. Programmatic list movement alone leaves the old record untouched. Reloading row0 displays0.
3. Actual Extra3DIo/page Apply with five empty controls writes exact zero-record main bytes and companion `1\t\r\n`; reopen and repeat Save preserves those bytes. The const caller item/control state is unchanged by Apply's local snapshot.
4. Nonempty space/tab, bad text, positive/negative extreme exponents, nan/inf and embedded NUL still refuse, even when an earlier control is empty. No item writeback or sidecar effects occur on this preflight refusal.
5. An empty old control is committed to +0 before a later next-row second-combo label refusal; the report preserves OldItemWritten/FirstComboApplied, partial first-combo index change, old stored row and empty visible text.

Tests retain the existing component qualification and bounded short C:/PROBLEM project/diagnostic cleanup scheme. No temporary project or sidecar was created by the worker because no test was executed.

## Static preflight and freeze

Compared the pair against `build/wave3-checkpoint126-verified/source.zip`, inspected the complete production diff, checked the existing quoted include targets and supported CHECK/CHECK_EQ/REQUIRE macros,24 unique TEST_CASE names, one main, no C assert/REQUIRE_EQ or raw NUL source byte, balanced braces and whitespace. `git diff --check` produced no error. No compiler/build/CTest, GUI, original execution or mutable REA operation ran. Static preflight is not a compile/test pass; root owns grouped validation and archive freeze.

Exclusive files released:

| File | SHA-256 | CP126 text diff |
| --- | --- | --- |
|`source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`|`7cccbd614ebda370eae85f313a4cb1f321be080dba827ce8976bb318d68dec69`|+3/-3|
|`source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp`|`fead4fe7e6caadc6aca22ff824b349c307cfc06e104ebadf5b70eeb4a27e22bb`|+112/-0|

No shared header, lifecycle, native parser, Qt implementation, CMake or public loader/factory was edited. No reconstructed executable/deployment was inspected or produced. The pair is frozen for independent review/root build; no further worker source edits until release.
