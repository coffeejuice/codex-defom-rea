# Selected palette deref and settings-enable edges

The palette decision is exactly a raw32 decrement followed by AL's zero/nonzero test; no retained selected postOpen observation supplies that current word or return. A separate **complete conditional settings branch** is now more tightly closed: `setEnabled(false)` on an already disabled actual QWidget may first set raw attribute32, then returns with no further widget/event work. The raw32 switch-table target is verified below. Actual selected control owner/bits are not bound, so this lane emits no helper or source edit.

Only this report was written. No source/build/test, GUI, original launch, REA import/database operation or shared edit occurred. Immutable PE bytes and bounded retained disassembly/source views were used; no new raw capture file was created.

## Palette input and return edge

For the selected ordinary default-QPalette frame, the exact preferred-address chain is:

| Site | Input/effect/next edge |
| --- | --- |
|`6504abc3`|RCX = data pointer held by getter temporary at `[RSP+20]`.|
|`6504abc8`|RDX = data pointer from getter output addressed by RAX; same source value.|
|`6504abcb`|Store RDX to destination `[RBX]` (selected private+170); no destination native ref yet.|
|`6504abce`|Call IAT`65696978`, bound to QtCore deref `67001ac0`; RCX identifies the DATA atomic at offset0.|
|`67001ac0..1ac8`|LOCK ADD dword,-1; SETNE AL; RET.|
|`6504abd4..abd6`|TEST AL; JNE`6504ac0d` for nonzero, otherwise enter`6504abd8`.|
|`6504abd8..ac08`|Zero path destroys60 brushes at data+8 and frees data; remains an unsupported continuation.|
|`6504ac0d..ac17`|Load destination data, store entire word+8=0, ref destination through`656968c0`; ref bool is ignored.|

For current raw bits `n`, updated bits are `(n-1) mod 2^32`. AL is0 exactly for input1; every other input produces AL1. In particular input0 becomesffffffff and skips destruction; ffffffff becomesfffffffe. This predicate is not positivity, a special immortal sentinel or C++ ownership count.

The selected current-data word is still unavailable. Existing private-return data/word0 or later opacity success cannot stand in for an observed input/AL at this edge. The final word0 store erases the source wrapper word; the native return edge also does not reconstruct the exact prior numeric word from AL. A properly bound observed AL would establish that branch decision only; it is not by itself a current numeric ledger anchor or permission to fabricate one.

Required frame equality remains RCX==`*(getterTemporary)`==`*(RBX)`, RBX==actual selected private+170, with same module/runtime/Operation/editor/frame and current mutation sequence. Getter ref`6504a8c6` is already included. CP102's capture recipe supplies the missing observation sites/race qualification; no new count/AL witness was acquired here. The existing isolated CP80 raw32 primitive already covers this arithmetic, so another raw decrement helper would duplicate it.

## New bounded settings closure: raw attribute32

The complete `QWidget::setEnabled(bool)` wrapper at`6506f080..6506f0b7` calls `setAttribute(raw32,!b)` at`6506f09c`, then tail-jumps at`6506f0b2` to `QWidgetPrivate::setEnabled_helper(b)` at`65069f00`. Raw32 is read from private+1c8 bit0; disabled bit0 is separately read from QWidgetData+8. These are different fields.

The attribute setter already had an equality return at`6506904d→650696f1`. The previously unclosed **changed raw32** path is now resolved from the actual PE switch data:

```text
attribute32 => dispatch index32-2=30
selector byte[6506974c+30] = byte[6506976a] = 18 (0x12)
target dword[65069700+4*18] = dword[65069748] = 0x000696f1
target = QtGuiBase65000000 +0x696f1 =650696f1 (plain return)
```

For raw32, the special attribute8 branch is skipped. The generic write at`650690fa` ORs bit0 into private+1c8 when wanted=true; `6506911b` clears bit0 when wanted=false. The verified dispatch then goes directly to the plain epilogue through RET`650696fd`. Therefore raw32's changed path has **no attribute-specific callback** after the bit write. Other extended bits are preserved. This conclusion is limited to this exact attribute and selected module, not all setAttribute cases.

## Complete conditional disable result

For canonical bool b=false, after raw32 handling the helper takes`65069f2b→65069f60`, skipping enable-only window/parent queries. It reads actual QWidgetData+8 bit0, compares0 to that disabled bit at`65069f6b`, and if the bit is1 returns via`65069f6e→6506a215..6506a21e`.

| Actual input state | Complete selected `setEnabled(false)` result |
| --- | --- |
|disabled bit1, raw32 bit1|Raw32 equality return; private helper returns. No persistent widget/event change.|
|disabled bit1, raw32 bit0|Set only raw32 bit0 to1 at650690fa; attribute setter returns; private helper returns. No disabled-bit/focus/child/platform/event effect is reached.|
|disabled bit0|Raw32 processing still precedes helper's later state-changing path. The helper reaches its disabled-attribute operation at65069f8a; full mutation/event closure remains required.|
|unknown actual native state/owner|No claim that a projected requested false is either native branch.|

Thus the source-closed disable branch is stronger than CP66's already-unchanged case: it permits a real raw32 store when the actual widget is already disabled. It does not require parent state because b=false bypasses those calls. For b=true, additional window/parent decisions remain and are not generalized here.

## Why this does not yet justify an implementation

`SetupControlState::requestedEnabled` is a setter argument/projection; its optional `enabled` has a separate bounded parent-tree meaning. Neither supplies the actual selected settings QWidget/private/q_ptr/QWidgetData identities or the physical raw32/disabled bits for this call. The selected Session still stops at its palette producer before settings controls exist. A source table showing toggled(bool)→setEnabled(bool) does not instantiate those receiver owners.

The precise future input is one actual live same-runtime settings control with its canonical widget/private/data relation, exact bool occurrence and qualified current disabled/raw32 fields. With that binding the branch above can be consumed as a genuine callback-free native return, preserving the earlier raw32 store. Unknown/changing-state cases must stop at their correct later effect, not roll back the committed raw32 prefix. At present those inputs are unavailable; an isolated Boolean/bit helper would add metadata rather than original-facing behavior, so no code/test files were created.

Public Editor300's palette refusal, CP81's pre-deref stop, private font/count/UI gates and actual Qt receiver/enablement effects remain intact. This report does not repeat or relax CP105/112's broad no-increment decision; it records two exact address/decision edges and the newly closed attribute32 dispatch.

## Byte qualification

Pinned QtGui SHA-256 is `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Reads used local `tools/doe_static_pe.py` on immutable images and existing bounded disassembly (`wave3-closure-next-65068fe0.asm`, CP66 attribute/helper tails). No provider/database was opened.

| QtGui exact range, end exclusive | SHA-256 |
| --- | --- |
|`6504abc3..6504abd8`|`5d6f4bc81ec1f76bb801ef0839325725c5ee705bf2aa8dfeb74991b8fc3a926e`|
|`6506f080..6506f0b7`|`7e0693d88f50edd8f6476a47c944948a1e0564b60173ec8809cbce57e27ee49a`|
|`650690d1..65069103`|`0f2cf5d8a5a1c69bdb630781fda5e7a36274d4d78d6db0da67370b3ff913bc84`|
|`65069103..65069122`|`02e10108a23ca3f04349c117b15c7a414f5edbbc32376bb94745ac20edd63428`|
|`65069122..6506914e`|`ae46c0c34a2299486230d935eb1a11c8c16c69ecd1e0dce5e195fc2d5afc941a`|
|`650696f1..650696fe`|`b0d004761b3a90df09ca51b13a0488db2b1156657025730961964439ee0b47e3`|
|`65069f60..65069f74`|`561d20ad3d89a8ba2f28a2022e7b2cd39ed35538e95e8986a67bf4d03f9b11b6`|
|`6506a215..6506a21e`|`45a945a546a9f0d2434231fb9a4ca2d697f01a3e481cc98f74bffa6ab53ad219`|

Exact QtCore deref8-byte hash remains `06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6`. No new runtime field or token measurement was acquired.
