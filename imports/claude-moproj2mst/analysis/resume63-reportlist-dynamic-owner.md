# Checkpoint 63: ReportList dynamic task owner

## Finding

The ReportList handler does **not** statically establish that its returned `SfTaskBase*` has an `SfTaskReportGenerate` vtable. That class is the strongest selector-based candidate, but the dynamic owner remains unresolved. Therefore slot `+0x218` at `0x1406561a4` cannot yet be treated as the ReportGenerate implementation for active Open/Save.

## Signal and return path

`analysis/resume63-reportlist-owner.md` traces the ReportList tag (`0x1424e3cb8`) to `FUN_140655df0`. At `0x1406556151`, that handler invokes the main-window global `0x143d76700` using `2signalGetTaskBase()` and supplies selector data `SfTaskReportGenerate` as a `QString`. The generic return target is initialized null and statically typed `SfTaskBase*`. If non-null, the handler immediately calls its virtual slot `+0x218` at `0x1406561a4`.

The retained constructor connection evidence in `analysis/resume60-dblist-signal-next.md` maps `0x1400111c0`'s connection setup from global sender `0x143d76700`, with `2signalGetTaskBase()` and encoded receiver member `1slotGetTaskBase()`, to the initializing main-window object. The class string inventory contains `SfTaskBase*` and `slotGetTaskBase(QString)` (`evidence/gui_mo/strings.txt`, offsets `0x0216dd36` and `0x0216dd4e`). This supports a main-window getter slot whose metadata declares the base-pointer result and class-name argument. The retained evidence does not include its implementation body, final connection flags/semantics, or a runtime result tying the selector to a particular object.

The encoded connection's `1slotGetTaskBase()` spelling and the metadata's `slotGetTaskBase(QString)` are not explained by a retained metacall body. This mismatch is a remaining static gap; it should not be papered over by assuming ordinary synchronous return propagation.

## Candidate vtable versus factory construction

The selector `SfTaskReportGenerate` and ReportList handler's subsequent `+0x218` call make `SfTaskReportGenerate` a plausible expected subtype. The retained RTTI/vtable evidence in `analysis/resume59-dblist-vtable-bytes.txt` identifies its slot 67 (`0x218 / 8`) as `0x1407a0150`; the bounded code is `analysis/resume63-reportgenerate-slot67.asm`. That routine inspects ReportList children for `OutputList` and delegates a matching child through slot `+0x240`. This tells us what slot 67 would do **if** the returned object has that vtable; it does not identify the getter's returned object.

The separate native TaskFactory constructor evidence does not close the gap. `analysis/wave2-doe-task-factory-prefix-api-proposal.json` identifies the first global prototype constructor at `0x1407b89e0` as plain `SfTaskReport` (`Report` metadata), with registration/cache insertion only after its constructor returns. The 17-entry constructor receipt `analysis/wave2-doe-task-prototype-constructors-static.json` is explicitly qualified as direct prototype-constructor entries and partial unwind regions, not a complete class-to-return lookup. The plain `SfTaskReport` prototype is not evidence that `slotGetTaskBase("SfTaskReportGenerate")` returns an `SfTaskReportGenerate` instance; nor does its distinct name prove the getter cannot find another instance.

## Open/Save consequence

For active Open/Save, the ReportList branch's first virtual dispatch is controlled by the concrete getter result. Current evidence proves only the selector, generic base-pointer type, non-null guard, and the candidate ReportGenerate slot behavior. It does not prove whether the getter returns a factory prototype, a separately created task, another `SfTaskBase` subtype, or null. No retained capture observes this getter on the active path. Keep public ReportList dispatch guarded until either the native `slotGetTaskBase(QString)` lookup/return path is closed statically (including cache and construction identity) or a source-qualified original observation provides the returned object's dynamic vtable.

## Evidence boundary

Evidence used: retained original-PE decompilation and bounded assembly/RTTI notes tied to SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, plus the retained GUI string inventory and TaskFactory receipts. No source, build, GUI, runtime, or mutable REA access was used. No new binary capture was needed; the unresolved item is missing owner/lookup evidence, not the slot body.
