# CP87 `viewerRebuild` two-borrow review

## Decision

Approve the narrow migration. The implementation matches the frozen CP87 plan and its source-qualified `viewerRebuild` order. It uses a bool callback for the prior missing-step early return, then after `viewerOf` and optional `viewerClear`, a separate void callback copies only `last_object_number` into local `n`. No native step pointer survives either callback or crosses clearing, resizing, view binding, pending-query handling, settings, or refresh (`src/engine/viewer.cpp:1043-1067`). The second callback retains the planned unconditional native-step dereference; it introduces no fallback for a step that disappears after the first check.

## Order and semantic checks

The body remains: earliest `GeometrySyncAccess::requireAvailable`; nullable-step predicate; `viewerOf`; optional clear; tail-number read; resize; ordered object binding; pending-query early return before flags/refresh; conditional default flags; refresh; unused `b` contact-display argument. Thus the first gate still precedes lazy Viewer creation. If `viewerClear` errors, the second callback is not reached. The second borrow ends before `resizeViews` and all per-view work.

The count helper is `def::model::last_object_number`, which returns zero for no counted objects/null tail and otherwise reads the actual tail object's number (`port/defmodel/src/gui_twins/step_utils.cpp:48-56`). The migration does not substitute `step_count_objects`, so sparse/kind-7 positions continue to drive View indexing as specified by the MDT-V ledger/spec cited in the CP87 plan.

## Test comparison

The direct boundary case exercises all four `a`/`b` combinations on a two-object model with real bound views and existing Settings, checking step/Viewer/object identities, counts, and settings snapshots after each call. It then temporarily moves the same step owner out while retaining the existing Viewer and verifies all four combinations leave views/settings intact; the RAII owner restores the exact step. A fresh missing-step model verifies all combinations return without creating a Viewer. These cases protect the legacy null-step shortcut rather than weakening the parked contract.

The parked directory fixture now runs all four combinations inside the existing pre-effect snapshot interval. The expected refusal therefore cannot pass after a reset-true clear without violating the subsequent step-byte, viewer, settings, count, and timer/global-state comparisons. This covers the important `a=true` case that previously had only one argument combination.

## ABI and bounded limitations

The change adds no member, helper signature, public API, or native layout change. The `GuiModelAccess` helper and `GuiModel` layout are unchanged; the static API/step-size assertions remain in the existing test. No binary export table or `sizeof(GuiModel)` verification was run in this review. As the CP87 plan states, arbitrary reentrancy/concurrent replacement and previously retained raw aliases are not established by this migration.

No correctness blocker found. Review was read-only; no source/test edits, builds, or tests were made.
