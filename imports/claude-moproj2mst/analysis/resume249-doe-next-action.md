# CP249 active DOE next action

## Decision

No finite internal production helper is independently ready. The only newly recovered path is still one coupled Add Study transition: it appends project DOM state, constructs and attaches a GUI SUB, changes current selection, activates it, then requests an initial DoeOutput operation. Splitting out a builder, selector setter, or `createOperation` call would preempt that sequence; invoking it from conversion would invent a public action absent from the converter contract. The pure `"DOE Study " + id` label fragment is not a useful production increment: it has no supported caller without the same transition, and existing records preserve their serialized/custom names.

## Queue and existing closure

The active RDOE item in `analysis/takeover-state.json` remains `active` and explicitly prohibits repeating CP215's `InputTableSource.readerIo` identity guard. It still names the unresolved nonempty key/ordered live weak receiver/backend/callback witness and the separate B120 Add Study/selection/sample-generation/Save witness. `analysis/wave3-doe-next-active-study-dependency.json` still records no source-qualified active-study variable/output/optimization branch and keeps the native Report/QObject constructor guard. CP240 now independently qualifies the static Add Study call chain and creator order, but adds no fresh execution evidence.

Current source continues to confirm the boundary: `src/core/convert.cpp` selects index 0; `src/model/subproject.cpp::makeSubProject` constructs an existing Type2 record through converter ownership; `src/doe/activation.cpp` and `keyword_reader_prefix.cpp` handle later lazy-reader/notifier prefixes. None supplies the GUI Add Study action or the `0x1400A6A00 → 0x1400A7B90 → 0x1400A63C0` chain. CP241–248's current migration queue does not close this DOE dependency; CP248 is a separate editor meshing-object owner command.

## Smallest non-redundant witness

Use one fresh original process and disposable copy of the retained B120 **before** project. Capture a staged trace, not another static/decompile or before/after hash-only audit:

1. Before clicking Add Study, record process/project identity, project DOM/SUB count/current index, existing DOE/session/sample state, and directory manifest.
2. Trigger Add Study once and bind that action to the actual slot and the same `0x14002AB20` invocation. Record the compared predicate inputs and `[rsp+0x70]` value/edge, then observe the same `0x1400A6A00` call through normal slot return: generated id/name, DOM append, constructed SUB identity and model alias, `P+0x50`/list state, activation outcome, `0x1400A63C0` operation result, and sample-table state **before Save**. Preserve exceptions/faults and partial effects.
3. Explicitly Save in that same process/project, recording ordered DOE file writes and resulting bytes/hashes. This separates Add Study effects and any pre-Save sample generation from Save-only effects without treating the retained B120 final files as per-call attribution.

This single occurrence is the smallest evidence that can qualify the coupled transition end-to-end while keeping ordinary Open/Save out of scope for study creation. If the supported app-control lifecycle prevents the capture, retain guards; do not replace it with the saved B120 after-state or a candidate-only helper test. The first implementation can then be limited to the earliest fully observed prefix, with its Testkit case, without claiming sample or Save behavior beyond the captured boundary.

Bounded scope: current RDOE item/dependency record, CP240 audit and disassembly receipt, and current DOE source paths above. No source, test, build, shared state, GUI, original process, or mutable REA state changed. No full DOE-closure claim is made.
