# CP266: Boolean timing has no new source-promotable leaf

The CP164 timing question remains blocked on an original occurrence. The already implemented zero-slot `postLoad` branch is a separate load-time creation effect, not a new timing leaf and not evidence of Apply during Open. This audit found no additional bounded source-evidenced decision/effect leaf that can be promoted independently of the missing first-Open occurrence.

Read-only audit of the converter checkout. Only this note was written. No source, tests, shared state, plan, build, GUI, original process, or mutable REA state was touched. Retained native evidence targets DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; disassembly and PE scans establish selected static facts, not runtime timing.

## Reconciliation and examined path

CP164's zero-record branch is already fully bounded by CP102/203/214 and candidate regression: native Boolean `postLoad` reads the OPI slot-record count at `11df0cb`; zero calls `addObject(OPI)` at `11df0e9` (return `11df0ee`) and then requests slot 0 kind 5. Normal base-load order places that call before propsLoad and postLoadViews. Current `source/apps/moproj2mst/src/engine/l5_slots.cpp:booleanPostLoad` implements this exact predicate and order. The focused `test_boolean_postload_creation.cpp` distinguishes zero slot records from a nonzero record count even when engine object count is zero, and verifies unchanged Boolean values. This branch is neither the Apply add nor a decision awaiting implementation.

The distinct Apply route is also already source-bounded: the Boolean Object editor's `updateDataInWidgetArea` dispatches to `booleanEditorApply`; its workpiece and cutter selections are handled in order, and “Add New Object” requests `engine::addObject` from those respective branches. Native source selectors are Apply `11e2870`, Workpiece add return `11e295a`, and Cutter add return `11e2bcb`. The current lifecycle adapter requires an admitted emission, notifier gate, receiver registration, resolved method, current-widget match, and owner identity before invoking the hook. These guards accurately preserve the missing causal input; source inspection cannot supply it.

The retained constructor/notification path and static UI metadata do not reveal a further independent side effect that promotes timing. The native constructor's notifier stage is downstream of the same-E QWidget and setup/guard returns; ordinary `connectSlotsByName` has no Boolean Apply slot. Static tree-refresh and timer analyses bound possible routes but neither prove a runtime callback or bind it to a complete Open interval. CP196's retained evidence audit covers 12 original probe directories and 39 message-bearing streams with no qualifying same-invocation Boolean constructor chain or Apply/add-return nested in a complete first Open. It explicitly treats this as missing retained coverage, not proof the event never occurred.

## Exact runtime witness still required

To promote an Apply-origin effect, retain one selected complete fresh Open interval (GUI handler through `28514` or the selected CLI startup branch through `1e4e`) and, nested within it on the same process/thread and owner lifetimes: widget-area UpdateData resolving the actual current Boolean editor E; Apply `11e2870` on E; E+38 resolving to OPI O; and normal return from `addObject(O)` at `11e295a` or `11e2bcb`, including the returned object/number. Independently label any zero-slot load-time add at `11df0ee`; never count it as Apply. A positive chain establishes one Apply-origin effect before that observed Open completion. Claiming that it was the first occurrence, proving absence, or attributing it to a timer additionally requires gap-free interval coverage, reentrancy accounting, and timer/connection ownership evidence.

For the distinct constructor claim, require one same-E factory allocation generation and constructor frame with incoming OPI, normal QWidget return `377990`, wrapper/setup and guard returns through `130830`, actual E+38 resolution (and conditional model return if that branch runs), E+28 bind return, then notifier call `130ad1` identified by caller continuation `130ad6`, with canonical notifier identity and suppression state. Neither a later Fill nor a Forming editor observation substitutes.

The current release assessment still reports no supported GUI release method plus verifiable receipt/status. Thus the remaining witness is not available through the current authorized GUI-control path. No static branch or candidate test can promote it. Preserve current public Apply/PostPending guards and unknown timing until the correctly correlated original witness exists.

## Evidence consulted

- `analysis/resume102-boolean-postload-creation-bound.md` and `analysis/resume102-boolean-postload-test-implementation.md`: native predicate/order and candidate test.
- `analysis/resume164-boolean-next.md`, `analysis/resume203-boolean-open-constructor-static.md`, and `analysis/resume214-boolean-timing.md`: distinction among postLoad, constructor notification, and Apply timing.
- `analysis/resume63-boolean-apply-first.md`: native Apply entry, editor-area dispatch and static Qt metadata limits.
- `analysis/resume112-boolean-postload-direct-edge.md`: bounded postLoad-to-refresh/tree edge; it does not establish Apply.
- `analysis/resume196-bool-witness-audit.md` and `analysis/resume124-boolean-evidence-index.md`: retained-capture coverage and missing joins.
- Current candidate: `source/apps/moproj2mst/src/engine/l5_slots.cpp:booleanPostLoad`, `source/apps/moproj2mst/src/ops/boolean.cpp:BooleanClass` hooks, `source/apps/moproj2mst/src/engine/boolean_editor.cpp:booleanEditorApply`, and `source/apps/moproj2mst/src/engine/boolean_lifecycle.cpp:BooleanSynchronousUpdateData::dispatch`.

No additional source mutation is justified by this audit.
