# CP137: bound Advanced frequency text-edit event

The next source-closed private state transition is a delivered Advanced frequency textEdited(QString) event. Its six native handlers immediately write one live FRQNCY value; Save does not read the frequency edits. This is a new event/model transition, independent of the previously recovered exact-number Fill profile or the combo callbacks' actual widget-enable tail.

Only this report was written. No source/build/GUI/original execution/mutable REA change occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained QtCore4_64.dll4.8.4 SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Evidence is retained source/receipts and bounded immutable-PE DUMPBIN, not observed native event delivery.

## Exact native call/order

Slot-map indices58..63 identify on_lineEdit_freqCNT/TMP/DBW/MXW/VEW/YLD_textEdited(QString). Their complete handlers are:

| Row | Half-open handler span | SHA-256 |
| --- | --- | --- |
| 0 CNT | 140ecfeb0..140ecff29 | `d487541d8ccf335c3c282808b0c2c124ef4aac1484dbc88bcf535a4d3a502125` |
| 1 TMP | 140ecff30..140ecffa6 | `573b918cb5016fb1050d1b96263ddf86572f4ab52c20832ea685c2c3645a9a52` |
| 2 DBW | 140ecffb0..140ed0028 | `3c156b667dfeec5e6246d73e24140011a62639f5215a01b9de23abfcb90cd677` |
| 3 MXW | 140ed0030..140ed00a9 | `d7a599427fca8d32ce050a10a36095dcd08f067094592ab842400dc84c0a01a8` |
| 4 VEW | 140ed00b0..140ed0129 | `62005e693defb77e928a4a3903a3c9061616f54a6c9c05a4cf1f8a71c98ec870` |
| 5 YLD | 140ed0130..140ed01a9 | `4a1bb0db00209eafd0fd32dbd8c95afe7b54ed9b78a6874a55ab21fc5714149e` |

All six perform this sequence:

1. Resolve actual current SC via130f90.
2. Read SC+32c (FRQNCY column stride), compute row×stride as signed32 and retain that index. Row2 uses SHL1; row4 uses SHL2; rows0/3/5 use IMUL constants and row1 retains the loaded stride.
3. Convert the delivered QString with QString::toInt(ok=nullptr,base10). Exact IAT141e04090 is `?toInt@QString@@QEBAHPEA_NH@Z`. No validator/enable/combo check occurs in the handler.
4. MOVD/CVTDQ2PD converts the signed32 result exactly to double, then the handler reads SC+378 (live FRQNCY values pointer) and stores that double at the retained index.
5. Destroy the by-value local QString via IAT141e059e0 and return. No widget-enable setter, model refresh, file operation or Apply tail follows.

For row0 specifically: SC call140ecfecc, stride readfedb/IMUL0fee1, conversionfef5, CVTDQ2PDfeff, values-pointer readff0d, scalar storeff14, QString destructionff1e. Thus conversion follows current-SC/stride resolution; do not reuse the title callback's conversion-first order.

## Bounded numeric value profile

Admit only canonical positive ASCII decimal1..INT32_MAX, plus truly zero-length null/empty text, with an explicit NumericLocale::KnownC value qualification. Positive text has no sign, leading zeros, whitespace or suffix. The source Fill installs QIntValidator bottom1 on all six edits, but the handler itself ignores validator state; the admission is a candidate first-cut profile, not native validation. Do not infer textEdited from a programmatic setText, constructor Fill, manual cache assignment or Save.

The existing qt::toInt adapter models QString::toInt base10 signed range/default-locale then C fallback. Do not add another numeric parser as replacement semantics. Canonical-positive lexical/range preflight may use an independent integer gate, but perform the actual existing qt::toInt value conversion after current SC/stride resolution. Reuse the existing value boundary, without claiming literal QLocale global initialization, actual QString refcounts, allocations or exceptional provider order.

Empty produces integer0 despite ignored validity, then exact double positive zero. This is independently closed by retained CP99/100 QtCore evidence:6706c280→6706c120 default/C conversion→6704c460→6704c190 length0 failure/zero. Default QLocale initialization may occur first; empty is not claimed locale-free. Hashes are listed below. Negative/zero-spelled/plus/leading-zero, whitespace/nonempty malformed, non-ASCII, overflow and other locale profiles remain guarded in this increment, even though some have known native numeric results. The event boundary, not more spellings, is the intended recovery.

## Bound private API proposal

Root owner.hpp declaration:

```cpp
ApplyResult StagedEditor300::frequencyTextEdited(Editor300Binding, int row,
    const qt::String& deliveredText, NumericLocale);
```

Use same live Project/OPI generation/GuiModel/remesh, completed private pages/source-model/remesh tokens, row0..5, KnownC and existing `pages.advanced.populatedFrequencyText[row]` engaged and equal to delivered text. The caller must first represent the new cached line-edit value, as with title delivery; optional disengaged remains unknown/unpopulated. Root may clarify the field comment to include latest explicitly delivered text; no new page array is needed.

Then query current SC (no pageState call), require original sourceControl identity, and retain the live column stride. Keep the existing six-row/one-column/ncomp6/non-null live values profile; do not allocate/repair or substitute another table. Perform conversion and store exactly one double to live row×stride. Do not read the old targeted value, sanitize other rows, alter combo index/known text/enablement, call Fill/Apply or wait for Save. An explicitly delivered event has no new enabled/custom-combo predicate because the handler contains none. Missing/replaced SC/table, unknown context or excluded input returns UnsupportedSnapshot without frequency/model/file stores; this guard is not a native graceful-null/failure claim.

All native by-value QString lifetime and QLocale side effects remain outside the existing private scalar adapter. No public palette/constructor guard is released. actual widget/validator acceptance and occurrence of the event are explicit prerequisites, not synthesized by this method.

## Exclusive files and focused tests

Worker: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_frequency_edit.cpp`, implementation receipt. Root owns method declaration and any small cache-field comment clarification. No simcontrol300.cpp, lifecycle, CMake or public Session change.

Expected target `m2m_mdt_simcontrol300_frequency_edit`, CTest `m2m.mdt.simcontrol300_frequency_edit`. Test actual bound completed private pages and model-owned FRQNCY storage:

- All six rows: delivered positive values commit immediately to the addressed row; other rows, controls/text, unrelated SC/OPI/remesh and I/O remain unchanged. Include INT32_MAX exact double and repeat same-text event after external model change.
- Empty/null edits overwrite old nonzero or negative-zero targeted values with positive-zero bits; cached nullness is retained. Leave unrelated nonfinite row bits untouched rather than imposing Fill-wide numeric validation.
- Demonstrate cache assignment alone has no model effect and existing Apply alone does not consume edited text; only explicit textEdited performs this store.
- Wrong/missing cache text, row-1/6, KnownC missing, excluded spelling/range, wrong binding, partial/unconstructed owner and expired OPI generation refuse before stores.
- Missing/replaced SC, missing values and wrong row/column/component shape refuse without allocation, table repair, stale-source access or replacement mutation. Restore test-owned array fields before teardown and allocate any fixture arrays through actual DYNARY ownership.

Current fillAdvanced3D preserves exact signed-int32 model values and projects the initial combo/optional text; it does not expose post-Open events. Existing Apply intentionally ignores frequency line edits. Neither function should be rewritten for this cut. Combo currentIndexChanged callbacks, TIMEID reconstruction/DB-step property tail, dialogs/picking and the selected constructor palette witness remain independent dependencies.

## Retained evidence reuse

Advanced Fill decompile `wave2-save-diffs-300-fill-140ec5bc0.c` SHA `34fc6d60edcb5cc4c50e99db08b2a4d2b265ef578d26fa65860df072279710ea`; current qt4/number.cpp SHA `bf90da77dc90b3adce1f688b897207686b430ab56bf6a149725a8e3f28d51ee6`. CP99 empty conversion receipts: `resume99-doe-missing-samples-convert-static.json` SHA `050c419afdd80a6fb18648cc23b773cd83787de85c88cf36e22707a21c76530e`; locale2 `d506edd7b0666066326fdb9678d523d55765c18ed4fa6aa7142e887419a32207`; empty-locale-text `92a8196631f26abba8fbb3b69160d6342b7cf23971593680558356503b1bb4d5`; trimmed `4772221cc30d595a5f142fe9d9eab3ce78f1d88770ba744b7a30dcbd3aaa9972`. Per-receipt native bounds and qualifications remain attached.

Bounded DUMPBIN used pinned BuildTools14.44.35207 SHA `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`, banner14.44.35228.0. Native function ends are confirmed by unwind bounds and returns. No new raw capture outside this report was written; no reconstructed executable/deployment was inspected or produced.
