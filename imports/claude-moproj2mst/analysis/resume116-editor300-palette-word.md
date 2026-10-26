# Palette word consumption and settings receiver producer

The selected application wrapper word is still not recoverable from retained private word 0. A tighter static result reduces its relevance: the default private constructor does **not consume that temporary word** before overwriting the destination word with 0. The next real barrier is same-data/getter origin and temporary-deref outcome/current atomic, not a guessed wrapper constant. A settings receiver's exact static allocation/parent/publication/call sites are also closed below, but its actual native bits remain uncaptured.

Only this assigned report was written. No source/build/test, GUI, original execution or mutable REA database operation occurred. Bounded direct PE/import reads and retained disassembly/capture source were used. No new helper or metadata-only implementation is proposed.

## Two different words

Use `W` for the QPalette wrapper's uint32 group/resolve word at wrapper+8 and `R` for the shared palette DATA atomic at data+0. They have different owners and uses. Private word 0, app_pal presence, brush style or QWidget counts do not determine `R`.

At `6504a8cc..6504a8f4`, the copy constructor's two masks reconstruct the **entire** source `W`, not a default 0 or just its low group nibble. With destination entry bits `D` and mask `M=0xf`, the native instructions compute:

```text
t   = W XOR ((W XOR D) AND M)
out = t XOR ((t XOR W) AND M) = W
```

The first intermediate write has source high bits/destination low bits; the second restores source low bits too. Unknown entry padding cancels. The returned getter temporary therefore preserves app_pal's full source word. This proof does not reveal its selected runtime value.

The selected default constructor `6504abb0..6504ac26` then reads temporary **data**, stores that pointer to private destination at 6504abcb, derefs DATA at 6504abce, and on its admitted continuation writes the entire destination word 0 at 6504ac10. Neither the nonzero tail nor the zero destruction branch reads temporary wrapper+8. The latter destroys brushes through data+8 and frees data. Thus source `W` is not a branch/control/value input to this private pointer/deref/word 0 slice.

Consequences are deliberately narrow:

- Private word 0 cannot be inverted to recover source `W`; many source words converge to that output.
- A specialized, correctly bound private-constructor projection could retain an unknown temporary word while using known data and preserving native effects. It need not invent a source word merely to reach the pointer-store/deref guard.
- The general QApplication getter API still promises its full value. Current `WidgetPaletteValue` has a known uint32 word, and present-slot observation requires one. That representation cannot silently reinterpret unknown as 0; a future partial/symbolic producer would need a separate reviewed contract over the **same** data authority.
- This static consumption result does not supply the selected app_pal/data owner or execute a copy. Changing a value type alone would not move the public path and is not recommended as this checkpoint's source work.

## Atomic edge is unchanged

`6504abce` calls QtCore `67001ac0`: raw uint32 minus 1, SETNE AL. The native consumer at 6504abd4 tests AL; input `R=1` alone selects the zero/destruction branch. All other raw inputs skip it. Pointer store 6504abcb already happened; destination ref 6504ac17 is later and its bool is ignored.

No retained selected-frame input `R` or actual AL was recovered here. Editor004's observed returned private palette data `0x3e3edb0`/word 0 remains same-call output evidence, not the earlier atomic/source-wrapper observation. CP114/CP102's same-frame pointer equalities and race qualification remain necessary. A bound observed AL could establish the branch only, not fabricate an exact numeric ledger word; current primitive/count admission must not be bypassed with an unqualified expected-nonzero boolean.

For the smallest next private prefix, prioritize actual same-runtime selected-frame data/getter origin plus the at-deref atomic/outcome witness. Source `W` may be recorded cheaply when the real wrapper is observed, but it cannot be recovered by seeding startup value 13893616 or private value 0. Count 1 at allocation and later splitter counts 166/167/165 remain wrong source points.

## Exact settings receiver producer

The retained setupUi body and independently matched PE imports bind the heat-mode receiver to the actual UI payload, rather than a generic enabled flag:

| Original EXE site | Source owner/effect |
| --- | --- |
|`140edc55c`|Request 0x28 bytes for the `heatCmBx` receiver.|
|`140edc561`|Call native allocator 14146eabc. Null is a separate source branch; it is not an ordinary successful control.|
|`140edc581`|Load parent pointer from the same UI payload+0x80.|
|`140edc590`|Call IAT141e06d80, exactly `QComboBox(QWidget*)`, on that allocation/parent.|
|after ctor return|Install the retained derived primary/secondary vtables 14218c7a8 / 14218c770.|
|`140edc5f9`|Publish the completed normal-path pointer to the same UI payload+0xb8.|
|`140edc64a`|Set its object name through actual UI+0xb8 receiver.|
|`140edc65f`|Set enable argument DL=0.|
|`140edc669/670`|Reload actual UI+0xb8 and call IAT141e0ba40, exactly QWidget::setEnabled(bool).|

This matches source identities `modeHeatChckBx` UI+0xa8 and `heatCmBx` UI+0xb8, plus their later toggled→setEnabled route. The parent is UI+0x80, not assumed to be the editor, MainView or Session token. Constructor return/publication are distinct from allocation; no native QObject lifetime is manufactured from an opaque map entry. The null allocation path does not license a fabricated enabled/null success.

The static control producer is useful, but retained editor004 stops at first-grid publication `140ed9a11`. Its hook validates `ui==*(editor+0x140)` and `*ui==firstGrid`, then completes the capture. The receiver above is allocated later at 140edc561; no actual UI+0x80/+0xb8 control/private/data/disabled/raw attribute 32 snapshot exists at that endpoint. Existing requested-enable projection tests do not supply those physical identities or bits.

## Smallest settings witness that could consume CP114

For one real frame at 140edc670, record SAME selected editor/UI payload, receiver RCX==`*(ui+0xb8)`, actual receiver private/q_ptr/QWidgetData relationships and parent, raw attribute 32 at private+1c8 bit0 and disabled bit at QWidgetData+8 bit0. Do not infer them from a setter argument or an uncaptured completed constructor.

If actual disabled bit 1 is established, CP114 closes the whole false request: raw attribute 32 is already 1 or is set to 1, then helper 65069f00 skips parent checks and returns 65069f6e→6506a215. Its verified attribute 32 switch adds no callback. If disabled bit 0, actual state-changing/focus/child/event work remains; the branch must not be chosen just because the source asks for false. Earlier raw attribute 32 effect is retained on a later refusal.

That control point is later than the public palette barrier, so it does not bypass it. There is no fully bound settings input here from which to build a useful new source helper. The report recovers an exact owner-producing edge and reduces unnecessary word assumptions; the current actual Session still needs its selected palette producer rather than another symbolic record.

## Byte/source qualification

Original hashes remain EXE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, QtGui `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` and QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

| Exact range, end exclusive | SHA-256 |
| --- | --- |
|QtGui 6504a8cc..6504a8f4, full selected word algebra|`043c9e16d3b77d7a8fdc8582ee8e3fe68db60a031ed73d70fd2f538a404a6d3c`|
|QtGui 6504abc3..6504ac1d, selected pointer/deref/word/ref window|`e90c0f69ec1adf898f52f32e5b7b9349ddcbada999e2b2f0eb89aa73ab6a138e`|
|EXE 140edc55c..140edc676, receiver allocation/publication/disable window|`a3f283652dffe093d9551a006d9fe02451f31aed3d7b3b82b86040fa885e89b9`|

Import reads matched 141e06d80 to `??0QComboBox@@QEAA@PEAVQWidget@@@Z` and 141e0ba40 to `?setEnabled@QWidget@@QEAAX_N@Z`, both QtGui4_64.dll. Direct static reads used `tools/doe_static_pe.py` and bounded offline DUMPBIN, without loading the DLL or opening REA. Selected capture/script identities remain CP90/98/102; no new original runtime value or token measurement was acquired.
