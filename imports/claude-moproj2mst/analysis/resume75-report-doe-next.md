# Checkpoint 75: next Report/DoeOutput lifecycle edge

## Decision

No additional stateful Report or DoeOutput lifecycle edge is currently safe to implement without a native viewer/window owner or process-global application state. Keep both public paths guarded at their existing boundaries. The smallest productive next step is static caller/dataflow recovery for DoeOutput's DBLIST admission setter; if that cannot identify a complete owner and lifecycle, the missing producer is an original application-scope trace across project Open/Close, outside this no-GUI lane.

## DoeOutput: first blocked lifecycle edge

The candidate's `DoeOutputClass::loadOperation` performs its recovered model reset, dimension selection, and show-model step, then calls `loadClassData` before the later selected-session/global-task refusal. `loadClassData` requires a Type2 subproject with owned DOE study and configured `doeIo`, reads the subproject's `.DS` through `readDoeDocument`, and invokes `finishDoeOutputClassData`. This is a useful candidate-only partial path, but it does not establish original acceptance for DBLIST/ReportList.

The retained DBLIST handler evidence distinguishes DBLIST (`0x1424e3cb0 -> 0x1406554a0`) from ReportList (`0x1424e3cb8 -> 0x140655df0`). DBLIST admission reads process-global receiver `0x143d766b8 + 0x1c` and admits only when it is zero. The sibling warning behavior also reads process type at `+0x0c`; that value is warning suppression and does not reverse a failed DBNAME registration. The B127 document has DBLIST `../../../COG_P1_B1.DB` and an empty ReportList, but does not reveal either runtime gate value.

CP66's static scan found no direct call/jump references or raw code-pointer literal for setter `0x1402cbf30`, which writes `1` to receiver `+0x1c`. It did not qualify indirect callers, all writes, a zeroing path, or receiver lifetime. A fresh-image zero-fill is not a valid per-Session default in a long-lived application. The next bounded executable action is to inspect immutable PE indirect-call/dataflow candidates for the setter and enumerate all stores to this receiver field. Acceptance requires identifying the setter's caller and concrete receiver, every write/reset to `+0x1c`, and the receiver's lifetime across native project transitions. If the static evidence still cannot bind those facts, stop and request an original runtime trace at DBLIST dispatch spanning fresh startup, first Open, Close, and second Open; do not infer zero or bind the gate to `ConversionSession`.

## Report: next independent effect

The pure `.DS` sibling path derivation from the owned `Project.path` has already been implemented and qualified in CP52. It does not allocate a document, touch a QObject, select a viewer model, or perform file I/O, and it correctly leaves the Report public guard in place.

The next Report load effect is lazy document access at `SubProject+0x30`: native getter `0x1400b5ad0` allocates the document on first use, obtains the main-window QObject parent, calls constructor `0x140643100`, and stores the result. The constructor's connection callbacks and normal-Close teardown are not qualified. On Save, the native Report path calls `0x140d0d6f0(display, model)` and iterates the returned model. Candidate `displayRoiModel` implements a distinct target (`0x140d0d670`); treating those calls as equivalent would invent model identity and behavior. Subsequent per-object helper/IAT effects remain unresolved too.

Therefore there is no stateful Report prefix independent of the missing window/document owner or viewer selection. The next static action is to resolve `0x140d0d6f0` return type/ownership, whether it creates or selects a named model, and the called per-object helper/file modes. Separately resolve document constructor callbacks and destruction order. A bounded original lifecycle capture would be required if static evidence cannot establish those contracts.

## Qualification and evidence

These findings are candidate/static evidence, not an original-vs-candidate acceptance claim. The existing candidate DoeOutput path can perform ordered partial effects before refusal; no new run or mutable project was made for this audit.

Relevant retained evidence and SHA-256:

- `analysis/resume65-report-ownership.md`: `7e62a11b87034afe74901a018b7a24a45b995b34c3b5c3192782729c295c0cad`
- `analysis/resume66-dblist-global-lifecycle.md`: `e57b855d498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e`
- `analysis/resume61-dblist-first-effect.md`: `592958b44af829135fc08ede9e4959b2815a1d10bce5ac51eaef4ecb3e66ce2089`
- `source/apps/moproj2mst/src/ops/report.cpp`: `e378d472550bbe9f6a109baa330f6612277328ac425853cf5e17ba316252b43a`
- `source/apps/moproj2mst/src/ops/noop.cpp`: `27542e5d9250eba762a39e786f59f38048997b938142a2d490e8f4ea77b79fb4`
- Original `evidence/original-binaries/DEF_GUI_MO_64.exe`: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (static analysis only; no new binary execution).
