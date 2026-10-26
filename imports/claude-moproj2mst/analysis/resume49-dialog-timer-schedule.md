# Open-to-warning diagram timer schedule

## Static chain and deterministic subpath

For the selected successful fresh `.moproj` Open path, retained `analysis/wave3-boolean-open-sequence-contract.json` and `analysis/wave3-boolean-timer-contract.json` establish this bounded order:

1. Open parses the project, then post-open activation selects subproject 0 through `0x1400a7250` / `0x1400a7b90`, which binds the diagram model through `0x1400a89e0`.
2. Native view refresh `0x140038380` uses the same lazy diagram view and binds the operation model through virtual `+0x1b8` (`0x140170430`).
3. On each nonnull attachment, `0x140170430` registers a distinct `QTimer::singleShot(1000, view, modelUpdated())`. It has no equality early return, timer ID retention, cancellation, or coalescing. For the selected successful active-subproject path, the two attachment calls therefore deterministically request two independent timers. This is a conditional registration count for that path, not a global count for all Open variants.
4. Open has earlier `processEvents(flags=0)` and cursor-pump points before these later subproject-binding timers on the normal path; those earlier pumps cannot deliver timers not yet registered. Save later has another `processEvents(flags=0)` at `0x140098129`, before the per-subproject warning check. Static QtCore evidence says this pump polls/dispatches once and does not wait for a not-yet-due 1000 ms timer.
5. If one of the registered timers remains eligible and pending when `0x1400b8fb0` enters the Yes-only warning's `QDialog::exec`, the nested event loop can dispatch it. Delivery consumes the timer and invokes `modelUpdated`; the callback fits first, then resolves the current model and current item. With a live model/item, downstream selection/editor effects are possible. Null attachment retains the timer but skips selection after fit.

## What can be modeled without GUI capture

The registration subpath can be modeled from the current static contract: for this exact successful Open route, allocate two distinct logical timer records at the two nonnull attachment sites, each with the recovered 1000 ms request and same receiver identity. The source-predicted callback body and then-current attachment semantics can also be represented as a conditional transition when an explicit due timer is supplied.

The timer delivery set during the warning cannot be selected deterministically from retained evidence alone. No elapsed time, OS readiness, exact Save duration, or modal-entry schedule is known. Earlier Save's flags=0 pump can deliver timers that are due at that point, but it neither proves all timers were delivered nor that all were still pending. The warning's nested event loop can dispatch eligible pending timers, but the static source does not establish which are eligible at modal entry or the answer latency. Thus no callback / an empty timer set is not a valid default assumption; forcing one or both callbacks is also unsupported.

## Required adapter guard

Keep the modal warning unsupported unless its pump point receives a source-qualified schedule that names the exact modal occurrence and an ordered due-timer set, including evidence for an empty set when appropriate. Timer IDs must correspond to the session's two registrations for this route; each delivery must consume before callback and resolve the then-current receiver attachment. Do not derive readiness by sleeping, comparing a synthetic clock to 1000 ms, or assuming the timer interval is the modal duration. Do not promote this deterministic registration count into an original observed delivery count.

## Qualification

The source artifacts are bounded PE/QtCore/static analyses; they report no original runtime, GUI, or timer observations. They do not exhaust every Open descendant event loop or establish all possible callback types in the warning's nested dispatcher. The deterministic result here is limited to timer registration on the selected successful active-subproject Open chain.

No source changes, builds, GUI sessions, or mutable REA operations were performed.
