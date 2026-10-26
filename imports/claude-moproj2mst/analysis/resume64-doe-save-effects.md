# Type2 public Save effects and acceptance fixtures

Independent native verification confirms this order inside the actual public `0x140099040` Type2 arm: **study node -> InputTable XML -> DoeTableRefresh -> current DEF_DOE.PAR -> LHS -> FF -> USR -> Subproject_DbName -> parent DOM replacement/final .moproj stream**. XML and PAR use different failure Booleans. A valid empty sample table still produces its mode header; invalid flags, rather than row emptiness, skip PAR output.

This lane wrote only this report and bounded raw evidence/receipt. No source, CMake, build, executable test, GUI, original application execution or mutable REA changed. Candidate ownership/interface architecture is owned by `/root/doe_active` in `analysis/resume64-doe-type2-integration.md`; this report supplies native effect facts and complementary integrated fixture requirements, without duplicate edits.

## Public caller and output order

The CP63 public caller selection was rechecked: native Save `0x1400983c0` calls `0x140099040` at `0x140098cb6` with sixth argument zero, selecting study effects. Type2 is exact SUB type 2. Its cached manager is SUB `+0x20`, not an independently created Study.

Fresh `resume64-doe-public-call-receipts.asm` retains five call/argument/continuation windows. They independently confirm:

| Order | Call in `0x140099040` | Target/output |
| --- | --- | --- |
| 1 | `0x1400996ce` (CP63 gate capture) | `0x1400d29d0`, append `<SfDoeStudy name=...>` |
| 2 | `0x140099770` | `0x1400d3200`, InputTable wrapper |
| 3 | `0x1400997b2` | `0x1400d75a0`, selected-mode `Control/DEF_DOE.PAR` |
| 4 | `0x140099a77`, EDX=0 | `0x1400d7790`, `Control/Samples_LHS.par` |
| 5 | `0x140099d7c`, EDX=1 | `0x1400d7790`, `Control/Samples_FF.par` |
| 6 | `0x14009a081`, EDX=2 | `0x1400d7790`, `Control/Samples_USR.par` |

No returned AL from these file calls is tested to cancel the next phase. Returned false and an unsupported exception/refusal are different outcomes. Ordinary file false continues; a reached exception/refusal preserves earlier effects and leaves later phases unreached.

The new local Sub_Project is replaced/appended into the Project DOM only after this helper returns. The parent then serializes/closes the already-open `.moproj`. Native/candidate WriteOnly open and earlier root DOM/list changes precede the Type2 arm. Failure inside it must not roll those earlier effects back, move study files after final `.moproj` emission, or delete prior successful study outputs.

## InputTable status and reached notification

The complete CP63 wrapper capture confirms `0x1400d3200` performs dimension reconciliation first, builds `DoeStudies/<name>/Control/DoeInputTable.XML`, calls writer `0x14090a550` at `0x1400d349c`, saves its Boolean, then **unconditionally** emits `2signalDoeTableRefresh()` at `0x1400d369d` before returning that saved Boolean. Writer false does not suppress notification or later PAR calls.

The writer removes empty variables before its QFile open. Failed open returns false and sets `Failed to open file, %1` on global `0x143dd9a30`. Successful open serializes the DOM, closes QFile, checks QFile.error, and returns true only when that error is zero; nonzero returns false and sets `Error writing file, %1`. This is not QTextStream status and not the PAR open-only result. Full variable/constraint serialization remains separately unclosed.

The architecture lane identified a real DoeTableRefresh registration candidate. New bounded native evidence independently confirms **`0x1400ebc5c ->0x1402cfbe0`** in constructor `0x1400eb6f0`: flag 0, receiver=this, global notifier `0x143d76700`, key `0x14218af60 = 2signalDoeTableRefresh()`, slot `0x14218af50 = 1slotUpdateUI()`. No owner construction/lifetime or receiver effects were closed here. Existing `observeEmptySessionNotification` does not accept this key. Do not omit the message or invent an empty receiver list in public integration.

## PAR predicates and exact Boolean distinction

Fresh complete native bodies establish:

- **Current PAR `0x1400d75a0`:** initialize result false; read selected mode at manager `+0x40`, then invalid byte `manager+0x70+mode`. Nonzero returns false without building/opening a file. Zero builds the actual Control directory, selects table `+0x58+8*mode`, and calls `0x140915290`. The original does not bounds-check this selected mode; retain the candidate's explicit unsupported guard outside valid modes rather than invent a native false branch.
- **Current-path wrapper `0x140915290`:** append exact `DEF_DOE.PAR` and call `0x140915360`, returning its Boolean unchanged. It does not check row count.
- **Fixed-mode PAR `0x1400d7790`:** require mode >-1 and <3, then invalid byte zero; otherwise return false without file I/O. On admission select that mode's actual table and call `0x140915360`, returning its Boolean unchanged. In the public arm, these modes are fixed to 0,1,2.
- **PAR writer `0x140915360`:** open QFile WriteOnly|Text at `0x1409153ca`; open failure reaches false return. On success write the mode header, rows and trailing separators through QTextStream, close at `0x14091568d`, then unconditionally set true at **`0x140915693`**. There is no QFile.error or stream-status check before true. Partial write/close failure is not converted into false by this helper. This body does not assign the global error string.

There is **no empty-table exclusion**. With a valid invalid-flag byte and zero rows, the mode header still writes, e.g. mode 1 yields `1\r\n` under the qualified Windows Text/ASCII profile. This narrows the CP63 note's loose reference to helpers skipping "invalid/empty" state: the selected PAR helpers skip **invalid flags**, while a valid empty table writes. Current constructor-invalid tables remain a different case.

The writer has no native directory-creation, temporary replacement or rollback. Failed open cannot be masked by creating a directory at this phase. Existing StudyIo's open-success Boolean suits PAR status; InputTable needs separate post-close QFile-error observation.

## Minimal integration fixture matrix

These are integration fixtures for the **actual shared public Save body**, using a declared owner/view/effect profile, not another helper-only unit test or a fresh-original parity claim. Keep disposable native files under a short owned path such as `C:\PROBLEM\D64\A`, with all full paths under 100 characters. Actual execution belongs to root after source review; none ran here.

Common setup: actual ConversionSession and current Project/model-manager relation; SUB0 plus actual active Type2 SUB/owned Study and aliases; existing bound StudyIo/notifier; existing Project DOM containing the old matching Sub_Project; explicit DiagramViews observations; no variables or constraints. Do not fabricate sampler output or copy the historical B121 folder. Use fixed integer row values so ordering/status assertions do not depend on an unqualified decimal locale. Any explicitly declared notification driver remains diagnostic until its actual owner/effects are source bound.

| Fixture | Declared state/fault | Required ordered observation |
| --- | --- | --- |
| **P0: immediately safe public prefix** | Actual owned empty Type2 Study, output/refresh owner still unavailable | Enter real `subProjectElement`, create the owned SfDoeStudy node in its pending new Sub_Project, then stop at the first exact unavailable InputTable stage. Keep old Sub_Project replacement/final `.moproj` stream unreached; retain earlier root DOM/open effects. Type3 remains guarded. |
| **P1: complete effect order, gated future profile** | mode 1; all invalid bytes false; tables `{{1}}`, `{{2}}`, `{{3}}`; actual/declared refresh returns | XML open/write/close/status, refresh, DEF_DOE.PAR using mode1/value2, LHS mode0/value1, FF mode1/value2, USR mode2/value3, DB-name, replacement/final stream. DEF and FF bytes agree. This test must call actual public orchestration, not four writer functions independently. |
| **P2: predicate controls** | Parameterize valid empty tables; then selected invalid mode1 with only mode0 valid | Valid empty tables still open/write all admitted files with mode headers; when selected mode1 invalid, no DEF file or fallback to mode0. LHS0 may write independently; invalid FF/USR skip. No regeneration is implied. |
| **P3: XML false continues** | Parameterize XML open failure and successful-open/post-close QFile error | Save native error text; refresh still runs, then admitted PAR calls and final `.moproj` phases. Preserve partial XML bytes in the post-close-error case. Do not use PAR open-success as XML status. |
| **P4: PAR false/true distinction** | LHS open failure; separately an admitted PAR write/close fault | Open failure is false but FF/USR continue. A successfully opened PAR returns true despite later partial/failure observations. No global error substitution, rollback, retry or reordered output. |
| **P5: reached refresh refusal/release** | XML phase completes, actual refresh refuses or destroys the cached Study owner | XML/error effects remain; no current/LHS/FF/USR or final Sub_Project replacement occurs after refusal. A released owner must not be revived through a shared diagnostic lease or rerouted to another active Study. Lifetime loss is a reconstruction guard, not a proven native safe-return branch. |
| **P6: owner/regression boundary** | Missing/foreign Study aliases, changed actual current Project; plus Type0 and Type3 controls | First unqualified owner refuses before invented study output; preserve earlier public Save effects. Type0 behavior unchanged; Type3 retains its distinct OPT/owner guard. Public nonempty B121 and unrelated backend guards remain. |

Only P0 can advance while XML/refresh owners remain incomplete. P1-P5 are acceptance requirements for later actual effect wiring; an injected diagnostic profile passing them alone is not permission to enable public B121 or claim fresh-original equivalence. The architecture report owns the concrete shared signature/attempt-receipt placement for P0. The original arm caches its manager, so later callbacks cannot silently switch remaining files to a new active Study.

## Bounded receipt

Original SHA-256 was independently rechecked as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. New Microsoft dumpbin 14.44.35228.0 evidence totals **32,849 bytes**, recorded in `analysis/resume64-doe-save-effects-pe-receipt.json` (SHA-256 `23f9774237beedcfc7ffd90e85cb93a3e009aebcdc5066df2035e39008260269`).

| Complete selected body | End exclusive | Body SHA-256 |
| --- | --- | --- |
| `0x1400d75a0` | `0x1400d7785` | `d1670a0fecf123fa146225d20b6162417975508bcd5a72b15e3236ee9d1f9f23` |
| `0x1400d7790` | `0x1400d780f` | `d1e751d8fa3b68703b6148bcf0e3d7bd2eccf99a3c615fa918b10b2408e6d63c` |
| `0x140915290` | `0x140915359` | `a6b5fc1b042522caad555eaa16e46082d1afa3fc291cab4018f2ae6f46cf3d92` |
| `0x140915360` | `0x1409156d4` | `dddfc9de840deaf01e9161c507fedd4e8898de66a401b43a808288e4829afe04` |

Also retained are disjoint public call/continuation windows and the DoeTableRefresh registration window. These do not claim whole surrounding constructor/callgraph closure. CP63 complete InputTable wrapper and selected XML entry/tail capture receipts remain the qualified XML evidence. Source tests and architecture were read only; no acceptance test was authored or executed. Token measurements are unavailable.
