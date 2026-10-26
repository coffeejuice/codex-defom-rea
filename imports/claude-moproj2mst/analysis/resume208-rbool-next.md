# CP208 Boolean lane: constructor and first-Open timing remain unqualified

## Finding

CP164's zero-slot `postLoad` behavior is a separate, already implemented synchronous branch. `baseLoad` invokes `postLoad` before `propsLoad` and `postLoadViews`; when the OPI slot-record count is zero, the selected native body requests one kind-5 object (`11df0e9`, normal continuation `11df0ee`). Candidate `l5_slots.cpp` and `test_boolean_postload_creation.cpp` model and distinguish this case. This does not establish a Boolean constructor notifier or an Apply/add during the same first Open.

No implementation increment is supported by current evidence. In particular, do not turn the zero-slot branch into a constructor callback, mark an editor as constructed from its later fill, or infer an Apply add from the postLoad add. The selected native constructor route requires the actual Object-factory E allocation (`11e065b`, size `0x50`), constructor entry, QWidget return (`377990`), guard/model setup returns, and notifier call with caller continuation `130ad6`, all joined to the same E allocation generation and constructor invocation. Retained Editor004 is a Forming capture and cannot qualify that Boolean path. The retained checker result is a coverage miss, not evidence the path never runs.

Apply timing remains a second, independent gap. A postLoad add originates at `11df0e9`; the Apply adds originate after Boolean Apply `11e2870` resolves E+38 to OPI O, with normal add return `11e295a` (Workpiece) or `11e2bcb` (Cutter). Even a valid Apply/add occurrence does not prove it happened during the first Open without complete Open interval coverage.

## Smallest next original capture

When app-control release is verifiably supported and a fresh owned original run is permitted, capture one naturally reached Boolean Open from allocation through completion, with a single process/module profile, TID, matched normal-return ordinals, object identities, and allocation lifetimes. In that same invocation, retain:

1. Object-factory branch, E allocation and `11e2480(E, OPI)`; `377950`/`130740` through QWidget normal return `377990`.
2. `130830(E, OPI)` through its `cbc920(E+38, OPI)` normal return, resolved OPI, conditional model return if taken, and `377b20(E,S)` normal return. Preserve nullable S and distinguish E+28 from E+38.
3. `130ad1 -> 2cf3c0` notifier entry and return at caller `130ad6`, tied to the same constructor frame/E generation. This is the minimum constructor-timing witness.
4. For first-Open Apply specifically, retain the complete fresh Open interval (GUI handler `271b0..28514`, or the qualified CLI startup completion through `1e4e`) and any nested current-widget UpdateData -> `11e2870` -> E+38/OPI resolution -> `057ba0` normal return at `11e295a` or `11e2bcb`. Tag `11df0ee` separately as postLoad origin. A gap-free interval is needed to assert first-Open placement; otherwise report only the witnessed occurrence.

Retain hook/read-error/overflow and coverage receipts, plus original module, input and probe hashes. The offline Boolean Open witness checker is a structural preflight only; its exit status alone is not capture acceptance. No GUI, original-process, or source changes were made in this lane.
