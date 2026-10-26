# Selected editor palette producer: bounded retained audit

The retained editor004 capture identifies the actual selected editor's **returned private palette data** beyond the symbol address. It does not identify the getter's actual app_pal wrapper/word or the palette-data atomic at the earlier temporary-deref edge. Native code establishes the producer chain, but the missing same-frame runtime inputs still prevent a public Session palette continuation. No metadata-only implementation is recommended.

Only this report was written. No source/build/test, GUI, original execution or mutable REA changed. Existing PE disassembly and retained captures were read in bounded views; no new native capture or raw evidence file was produced.

## Same selected native call and data identity

Re-read `build/original-editor-probe-004/probe.js` and its retained capture. The capture has161 messages,73 observations and30 states, with `messages_truncated=false`, in historical PID27216. Its selected observations bind one thread30444 and the following original identities:

| Ordinal | Selected observed boundary |
| --- | --- |
|1|MainWindow returned, owner `0xafdce0`|
|2|Native Open entered on that MainWindow for `C:\PROBLEM\m2m42\DB_deconstruction_02.moproj`|
|3-6|Project `0x3797390`, model child `0x3795990`; read returned1; Open returned the same Project; postOpen entered|
|7|Selected editor constructor `0x383e780`, Operation `0x37bdbe0`, postOpen=true|
|8|Same editor QWidget entered, parentnull and flags0|
|9|Its QWidgetPrivate entered at `0x50ade90`, version264196 (`0x40804`)|
|10|Same private returned: palette object `0x50ae000` (=private+170), data **`0x3e3edb0`**, word8=0; native threadData still null|

The hook is tied to that selected QWidget frame and actual private call return at module RVA70db5. It verifies returned private identity. This is stronger than an uncorrelated palette snapshot or a module-symbol registration.

Later observations18-21 preserve the same private palette object/data: parentless QWidget palette return19 is object `0x50ae000`, data `0x3e3edb0`, word0; group4/current-group Window role10 lookup20 returns brush slot `0x3e3ee08`, exactly data+8+10*8. That slot references BrushData `0x4618db0`, style1, alpha255. These independently tie the selected constructor's output to its later opacity consumer within the same captured process/frame history. They do not read the palette-data atomic.

Capture003 is a separate process/capture with a less direct selected-private endpoint; it cannot supply missing editor004 slot/count values merely through matching phase labels. Probe001 stopped with a callback error and probe002 is earlier, narrower selected QWidget/setup evidence. The bounded source checks of all four selected editor probes found no app_pal-RVA getter/private-deref hooks or palette-data-ref field. No whole-application absence conclusion follows from that inventory.

## Exact native producer, qualified as inference

Retained `wave3-closure-next-65070d70.asm` shows QWidget allocating private storage and calling `6505e780` at `65070db0`, with return at65070db5, matching the selected probe filter. `wave3-closure-next-6505e780.asm` supplies the complete selected private body through RET6505e9d2. It passes private+170 at `6505e8d4` to default QPalette `6504abb0`, then separately constructs font/locale and other members.

The selected ordinary native palette producer is therefore:

```text
same QWidgetPrivate+170
  <- QPalette default6504abb0
  <- QApplication palette65016f70
  <- module app_pal slot at RVA9c9690
  <- copied SAME palette data through6504a8b0
```

Joining the captured private-return data to this source producer is a **static provenance inference over the selected ordinary body**, not a runtime observation at the getter. It establishes which native operation produced the private field and its observed output identity. It does not recover the intervening getter temporary address, loaded app_pal wrapper pointer, source wrapper word or counter value at the selected call.

The getter's `65016f8a` reads the current module slot. Null and nonnull paths converge on copy `65016fc6`; copy ref is `6504a8c6`. The default constructor stores the returned pointer at6504abcb, derefs temporary data at6504abce, conditionally destroys60 brushes/frees data, sets private word0 at6504ac10 and refs destination at6504ac17. These prior CP78/80 byte qualifications remain attached. The returned private word0 cannot reveal the app wrapper's pre-zero word.

## What the retained script actually reads

Editor004 `palette(a)` (`probe.js:138`) reads only `{object, data=*(a), word8=*(a+8)}`. It does not read `data[0]`. The private-return hook (`:359-367`) calls that helper on private+170. Its globals helper (`:161-168`) reads QWidget counts/application/style/thread context, but does not read QtGui+9c9690 or a global palette value.

The selected opacity palette hook reads the same helper result, and the brush helper reads brush style/color fields on a different BrushData allocation. No selected getter65016f70, private default6504abb0/temporary-deref6504abce or QPaletteData atomic hook is installed in these editor scripts. A bounded structured-key inspection of editor004 also found no palette-data-ref or application-palette-slot observation. The raw same-process selected output is known; its earlier slot/count is not in that capture.

The static QPaletteData constructor does initialize a newly allocated atomic to1, as CP82 establishes at6504999e. That is an allocation-time producer, not a current word for the already shared data `0x3e3edb0`. The missing interval includes palette initialization/caching/copies/releases. Normal constructor return, later opacity success or the presence of an app wrapper cannot replace a current numeric witness or prove the temporary-deref branch from this evidence.

CP82's splitter003 capture does directly observe an app_pal slot/data and later atomic words166/167/165, but in another process and later parent/resolve contexts. Its pointer `0x46cedb0` is not editor004's `0x3e3edb0`; equal logical/default palette content would not make those native identities interchangeable. Choosing one later number, or subtracting a guessed ref count, would move its source point.

## Candidate Session gap and precise missing witness

Current `beginEditor300Factory` validates and binds the actual candidate Project/Operation/GuiModel/runtime, then retains EditorBase PrivatePaletteUnavailable. Loader's original-facing route still refuses at `not_ported(0x6504abb0)`. Startup application palette/default/class-cache observations and the isolated CP81 placeholder producer are not a same-process/current selected editor grant. The CP87 symbol-only proposal was correctly deferred; this audit finds no new value/count evidence that changes that decision.

Two distinct admissions remain:

1. **Getter origin/data for the selected editor:** the actual same-runtime/module app_pal wrapper/value at65016f8a/65016fc6, joined to that live editor/private call and candidate interned data. The captured returned field can qualify the expected original output, but cannot silently refine a different candidate startup alias/current owner.
2. **Current count for temporary release:** that SAME data's raw first dword immediately before6504abce (or equivalent point after getter ref and before deref), with actual getter temporary/private destination identity and the current modeled mutation sequence. The observed count already includes6504a8c6's +1 and must never replay it.

A complete same-process ref ledger from actual allocation through the selected edge could replace the latter witness, but retained captures do not supply that ledger. The selected raw field/data identity above is useful original evidence; it is not an executable numeric producer for the actual Session.

Keep unknown state and the existing public palette refusal. If the origin/data admission is eventually closed, the first honest original-facing increment is one retained getter token plus partial pointer store, still guarding count/deref. Even after count admission, zero destruction and nonzero word0/destination ref must preserve their source order, with font/count/timing gates separate. No new helper, symbol-only source change or GUI acquisition is authorized by this report.

## Evidence identity

| Retained artifact | Bytes | SHA-256 |
| --- | ---: | --- |
| `build/original-editor-probe-004/probe.js` |40015|`0251ee49c0378af50e0778c8a584a5243402b10f26ab64fc315466bf10c68203`|
| `build/original-editor-probe-004/capture-evidence.json` |145151|`e4d0a277c4bbb074fa1e038ce3870e64ebb87ec884665cb1bb0ddd0221f99ced`|
| `build/original-editor-probe-003/probe.js` |37209|`e860e7ca70b0ea24a5846b6d2f8a04f0e47cd55a8b83d37bdb7ea9dd4bc9ee41`|
| `build/original-editor-probe-003/capture-evidence.json` |229339|`42fe8c13cfafe22d9beeac4cf87c95f48635e8f91af399241366f836e7bc6a2b`|
| `analysis/wave3-editor-probe004-palette-binding-contract.json` |5147|`983794323ce1c2142f623c05f8e3f43502e06aa71d792a30c5203ccaa1b3ad00`|
| `analysis/wave3-closure-next-6505e780.asm` |10250|`76ea1255e2b25c691ec26bf3198d1ad53e991632b268a9d5c77e0b44c706bd81`|
| `analysis/wave3-closure-next-65016f70.asm` |2105|`632a5d4727fec292c646769f91acee102a871b4dddbb1f202c94da6670117c39`|
| `analysis/resume82-palette-count-source.md` |10838|`2faf5e57e0f7a4e5687a4a4aeae9d31b9f19dd24c19e94ffc292b9c09532b253`|

Reviewed source snapshots: conversion_session.cpp `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`; editor_base.cpp `20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817`; loader_l3.cpp `5899cd9e9903a1061e46c50d07ff0c43943b184cb85fe20085901f8751d4d939` (all under `source/apps/moproj2mst/src/engine`). Historical capture status is evidence data, not a currently running-process claim. Token measurements are unavailable.
