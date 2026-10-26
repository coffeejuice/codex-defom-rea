# CP126: empty private Spring numeric edit becomes positive zero

The next source-closed private 3D transition is an empty Spring numeric line edit on item click or Save. The native callback passes a null success pointer to QString::toDouble and commits its returned positive zero for an empty QString. Current transitionSpringItem refuses that result solely because the adapter reports ok=false. Admit only the zero-length text case; leave nonempty failed conversions, nonfinite values, unchecked/empty-list behavior and the public Editor300 constructor gate unchanged.

This is a caller/decision integration of an already implemented Qt value adapter, not another SPRING2 numeric spelling or a new public native constructor claim. Only this report was written. No source/test/shared-header/CMake changes, build/test, GUI, original process or mutable REA occurred.

## Survey and chosen boundary

The bounded survey reconciled CP112's rotation/remembered/curve/validation paths, the current Advanced Fill/Apply frequency admissions, CP119's explicit private item-click/Save body, and CP125's unchecked/count-zero gate. The old remembered/rotation projections are already implemented. Generic QWidget enablement still needs actual receivers and inherited/native state, so another requested-enabled helper is not recommended. The selected empty-edit branch instead already has its private owner and an explicit reached callback: SpringSelectionState owns ordered typed item copies, visible numbers, stored/list row and combo context; transitionSpringItem is called explicitly for ItemClicked or SaveCurrentItem.

The behavior is observable inside this API: committing a previously nonzero numeric control that is now empty changes that item's number to positive zero. On a same-row callback, the visible QString stays empty because the native comparison returns before reloading formatted text. Switching away and back later displays0. This is different from rejecting Save, silently retaining the previous value, converting the text to an explicit0 before callback, or skipping old-item writeback.

## Exact original caller and value path

Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The EXE's IAT141e05810 resolves directly to QtCore4_64.dll `?toDouble@QString@@QEBANPEA_N@Z`; the shipped DLL export maps that symbol to6706c340. QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

All five controls in original140ecdee0 pass EDX=0 before the conversion call, then unconditionally store XMM0 into the local record. There is no ok variable or acceptable-input test on this path:

| UI slot / local record field | Clear EDX / toDouble call / result store |
| --- | --- |
|1638 / firstValue(+8)|140ece00b /140ece015 /140ece01b|
|16a0 / secondValue(+10)|140ece068 /140ece072 /140ece078|
|1648 / direction[0](+20)|140ece12b /140ece135 /140ece13b|
|1690 / direction[1](+28)|140ece18b /140ece195 /140ece19b|
|1680 / direction[2](+30)|140ece1eb /140ece1f5 /140ece1fb|

After checkbox-to-flag0/1,140ece244..ece257 copies the whole0x40 local record into the OLD item's+30. Only then140ece277..ece28d gets requested item's row and compares the stored row. Same row jumps to final cleanup; different row loads the requested payload/combos/numeric texts. Therefore zero conversion must precede old writeback and the existing same/different-row decision.

The shipped Qt empty path is independent of decimal spelling:

1. QString::toDouble at6706c340 first invokes the locale conversion through6704c340. It uses a local success byte; if false it takes the fallback at6706c391..c3b8, passing the caller's actual success pointer (null here).
2. Conversion preparation6704c190 loads QString data length into EBP at6704c1a9 and initializes EBX=0. Length0 skips scanning at6704c1b7..b9, reaches equal0 at6704c1d7 and returns AL=false at6704c1db..e9 without consuming characters. Null and empty QString wrappers both have zero length; their content result is the same.
3.6704c340 tests that false result;6704c3e6..c3f0 writes ok=false only if a pointer exists. At6704c407 it executes XORPD XMM0,XMM0 and returns the positive-zero double. The QString fallback takes the same zero-length preparation and same zero return. No qstrtod/nonempty digit branch is needed to establish the result.

This closes the value/decision used by the existing private adapter. It does not newly model full QLocale/QString allocation/refcount side effects or guarantee arbitrary process translator/default-locale state. The existing adapter qualification remains. Validators are attached by140eb9090, but the callback itself never checks them. The proposed private API accepts an explicitly supplied empty control value; actual GUI user-edit/validator delivery and public setup completion remain separate, unclaimed work.

## Existing candidate and minimal patch

`src/qt4/number.cpp::toDouble` already returns +0 and ok=false for a zero-length String. The existing p04 test_numbers `to_double` case includes empty input in its failure-to-zero matrix. No Qt helper or test primitive should be duplicated or changed.

`transitionSpringItem` currently calls that adapter for each visible control, then rejects `!ok || !isfinite(value)`. Change only its admission so an unsuccessful conversion is allowed when that exact visible String isEmpty(), while retaining the finite check and all other current guards. Continue using the actual adapter result; do not add a separate parse implementation or change every invalid conversion into0.

The narrow admitted text set is truly zero-length (nonnull empty or null String), not whitespace-only, punctuation, arbitrary letters, overflow/underflow spellings or embedded NUL. Those remain explicit exclusions even though other native branches may also produce0. Nonfinite values remain guarded. CP125's unchecked/zero-item path still skips all these reads. This patch needs no new owner, shared field or interface.

Request after root source freeze: exclusive `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`, existing `source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp`, and an implementation receipt. Do not edit shared headers, the native sidecar parser, lifecycle, Qt number implementation or public factory/loader.

## Focused tests

- For each of five visible-number slots, start with a live supplied private selection and finite nonzero visible values, replace exactly that slot by empty/null text, then invoke SaveCurrentItem on the same row. Require oldItemWritten, sameRow, positive-zero bits only in the selected numeric slot, and unchanged empty visible text. Other numbers and flag normalization remain ordered.
- Row0->row1->row0 with an empty old control must commit zero before switching, load row1 normally, and display0 only when row0 is reloaded. Programmatic setSpringListCurrentRow alone must still do no callback/writeback.
- Actual Extra3DIo/page Apply with all five visible texts empty should write exact main bytes `1\t\n10\t\n0\t0\t20\t0\t\n0\t0\t\n\r\n`; reopening confirms zero payloads. A selected checked record flag can separately verify the existing companion1/TAB/CRLF behavior; do not confuse it with the page checkbox.
- Retain nonempty bad/whitespace/nonfinite/range refusal checks, with no sidecar effects on that reconstruction-preflight rejection. A later unsupported target combo must still preserve the already committed zero old item and reached-stage report.

No test was executed by this investigation. These are private page/value tests, not a same-run original comparison or proof that the selected Session passes its palette constructor.

## Evidence receipt

Instruction views: retained `analysis/resume114-spring-selection-140ecdee0.asm` SHA-256 `fae61eecdc45c7e99e6d4ed9a19f8dae7c4864eee4b69ef934f1eb4493edc6c2`; retained `analysis/wave2-doe-qstring-double.asm` SHA-256 `4bb7ae643bf40f62c0f5d82973d13e9655cccf079b42b10bdd6c7762bf5b2ead`. The two lower QtCore regions were examined with bounded offline DUMPBIN from the same pinned binary; no raw file was created outside this report-only lane.

| Half-open native range | SHA-256 |
| --- | --- |
|EXE140ece00b..140ece021|`881616d4b5a06f203d337139fbed497deb127e6ee09dd14910fe0bce16a07510`|
|EXE140ece068..140ece07e|`79c65a26f40ab5d176ed929b03360e2b9e765cd9b0fd0f124f71729086c9762e`|
|EXE140ece12b..140ece141|`ed7a25dff909aeb74e9d50c48ae994d30e980b81bc457e607cd3d87d52d70c1a`|
|EXE140ece18b..140ece1a1|`a60d591686bac4c44a0143767f2a03bf3ca74282478896d599381dac75250cb8`|
|EXE140ece1eb..140ece201|`f0648eb3cadb2bae55b228ce9bd5082de98c82904078b6ceef5f61d2ff9f6ee9`|
|EXE140ece244..140ece29f|`f127a39c7f10dc9d89ef6190275359a57c1bc988645fef61b343c9175cc43fb4`|
|QtCore6706c340..6706c3c8|`498a54bf8e862ea8567a7af961ff9f2a8cdcfc1d8b7240c18be976ad71cdd382`|
|QtCore6704c190..6704c1ea|`a482185b7300b2c3b37d49fa63aabfdfc5007a20417a93a728e87adb82d0f453`|
|QtCore6704c3b2..6704c40d|`85945aa0de8a4c0e0774a85ab59c5241a6292675faca6f32c48e921734683aeb`|

DUMPBIN was the previously pinned BuildTools14.44.35207 path, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`; selected lower reads used `/DISASM:BYTES /RANGE:0x6704c190,0x6704c2cf` and `/RANGE:0x6704c340,0x6704c43f` on QtCore4_64.dll. Only the analysis tool executed.

Read-time candidate identities: io.cpp `720828fd1a77cef0e3b6fbb7201c40be86fb847fe603f2e2f638ef8075045ee0`; page test `7e652d40e4e9463391a469fb61e0552d6110764cc087a1866ab08b7bc24a2df2`; Qt number.cpp `bf90da77dc90b3adce1f688b897207686b430ab56bf6a149725a8e3f28d51ee6`. No reconstructed executable/deployment was inspected or produced.
