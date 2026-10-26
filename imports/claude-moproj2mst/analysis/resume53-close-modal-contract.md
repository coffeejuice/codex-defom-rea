# Normal Close modal answers, schedules, and accepted-Close effects

## Evidence status

The controlling retained contracts are `analysis/wave3-normal-close-guard-contract.json`, `analysis/wave3-close-save-return-contract.json`, `analysis/wave3-close-post-answer-contract.json`, and `analysis/wave3-directory-failure-dialog-contract.json`. They bind selected call paths to original PE/Qt binaries by hash and use offline PE/DUMPBIN/static decompilation; they are not an original runtime capture. The reference-harness and closure inventories state that a complete fresh original GUI Open→Save→Close run has not been performed. No retained historical capture located for this audit establishes a timed Yes/No/Cancel answer sequence, eligible timer set, or full post-answer runtime state.

Accordingly, the answer mapping and call ordering below are source-derived facts under the recorded shipped-style/focus qualifications. The event schedule is not a deterministic list of events actually delivered. Alternative No/Cancel answers are control-flow cases for the mapping table; the selected scope remains original defaults, not user-action parity.

## Normal current-operation Close question

Guard `0x140044110` asks only when a project model exists and the application process type is 1 or 5. Successful native `.moproj` Open selects type 5 and project+0x55 true in the retained source path. The question provides Yes `0x4000`, No `0x10000`, and Cancel `0x400000` (`0x414000` total) and requests `NoButton` as explicit default. Under the captured Windows/Common, WindowsXP, or WindowsVista style path with unchanged initial focus, the helper's default selection resolves to Yes. Application startup style choice and runtime focus changes are not proven.

The `QDialog::exec` modal loop dispatches the current-thread event dispatcher until exit. Thus each answer case must be paired with its own modal interval and explicit ordered due-event profile. A modal schedule may include no eligible diagram timer, one, or more; no retained evidence selects among these for any answer. Timer IDs that are delivered are consumed before callback, and the callback uses the then-current receiver attachment/model/item. Do not use dialog answer or button order as a timer schedule.

| Question result | Source-derived caller mapping | Effect status |
|---|---|---|
| Yes (`0x4000`) | Call nested Save while project/diagram remain attached and replace the question answer with Save's returned Qt integer. If Save returns Yes (`0x4000`), remap exactly that value to Cancel (`0x400000`). A last-operation warning itself offers only Yes; nested Save returns early before current-operation and file writers. | A warning Yes therefore cancels Close, retains project attachment, and skips accepted-Close callbacks. Modal event effects before the answer remain unknown. For ordinary Save outcomes, apply the exact nested Save result; do not derive it from writer booleans. |
| No (`0x10000`) | No nested Save call in this current-operation-question branch; final answer remains No. Guard's final accepted predicate is answer != Cancel. | Static path proceeds to post-answer callback/removal and later teardown. No fresh default-path capture verifies runtime state. |
| Cancel (`0x400000`) | Final answer remains Cancel. | Static path bypasses post-answer callback/removal and Close is rejected; project remains attached. |

The current-operation-question default Yes is the selected source contract. No and Cancel rows describe the native caller mapping if those results are supplied; they are not alternatives to capture as part of this default-only stage.

## Separate Keyword-question branch

When the guard is active but does not take the current-operation Save question path, the source asks whether to save a Keyword file, using the same three standard buttons and explicit `NoButton` default. The retained guard contract does not resolve its Yes callback at main-window virtual `+0x3a0`. Therefore do not claim the Yes branch's writer, status, or side effects. Its answer still participates in the final `answer != Cancel` predicate, but full branch effects remain unsupported. This branch is not required to close the successful native `.moproj` current-operation default path unless reachability analysis places it there.

## Accepted-Close static order and limits

For a non-Cancel result, the guard first invokes main-window virtual `+0x288` and then emits `signalRemoveGraphItems`; it returns true only after those calls. The close-session integration audit binds the callback input as `d0e540(0,0,true)`, followed by RemoveGraphItems, manager Close, project destruction, and diagram null attachment. The post-answer static contract traces selection of the displayed GuiModel, StateVar variable/component stores to zero, literal `2signalChangePtrkGraph()` notification, StateVar/render updates, eligible named-model updates, and later scene/controller callbacks. It explicitly says model/display effects are not proven a no-op and leaves render/legend/notification closure partial.

The following remain unsupported as exact runtime effects: which model is selected in a concrete fresh session without a supplied display profile; whether callbacks mutate project-visible data or only view state; exact transitive notification/render effects; any state changed by modal timers before the answer; and the Keyword Yes writer branch. Do not replace accepted Close with direct detach, or assert the callback is harmless because a default answer was selected.

## Smallest isolated test seam

The smallest testable seam is the existing isolated `close_dialog::resolve` answer helper and its unit test, with no public Close binding:

1. Assert immediate guard-bypass cases need no modal schedule.
2. For a qualifying model/process type, require a distinct qualified Close-question modal schedule before resolving the shipped-style default Yes.
3. Inject nested Save. If its result records the last-operation warning, require the warning-specific schedule to be dispatched from inside the nested-Save callback before it returns Yes.
4. Assert nested Save Yes maps to Close Cancel. Separately assert No/Cancel mapping without claiming their user actions are in the selected default scope.
5. Assert Cancel does not call any accepted-Close callback; the isolated helper deliberately has no cleanup or display callback.

Do not make this test assert a specific due timer ID list, timer count at dialog entry, GUI button click, runtime focus, or accepted-Close scene mutations. Those require a source-qualified immutable modal schedule and, for runtime parity, a fresh original-process capture. The test verifies the internal integer contract and fail-closed schedule guard only.

## Schedule evidence needed for eventual full binding

For each modal interval (Close question and nested last-operation warning), a profile must bind the exact source occurrence, process/session/receiver identity, eligible timer IDs and order, explicit empty eligibility where supported, and callback effects against the then-current attachment. If it includes a timer callback, retain whether it fitted, selected an item, changed editor state, or disconnected because the receiver was destroyed. Then retain dialog result, Save return, final Close answer, post-answer callback sequence, project attachment, and file/state effects. Until such evidence exists, event delivery must be an input or fail-closed condition, not inferred from requested timer intervals or elapsed wall-clock time.

No source edits, builds, GUI sessions, or mutable REA operations were performed for this audit.
