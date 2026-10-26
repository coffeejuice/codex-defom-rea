# CP200 active DOE source-readiness audit

## Decision

No smallest source-ready active-DOE integration increment beyond the CP199 QtCore CMP is justified by retained evidence. Keep public nonempty Type2 Open and populated DOE Save guarded. The least speculative next work is evidence acquisition for the exact same-occurrence `SetOprHasDoe` QString construction and publication already selected by CP196, not source integration. This audit excludes CP199's TLS/JNE proposal: it concerns a different QtCore initializer path and has no demonstrated DOE value or decision relevance.

## Current source boundary and ownership

The active converter implementation is under `optimization/test-speed/source/apps/moproj2mst/` (the audit's converter subtree). `src/doe/keyword_reader_prefix.cpp` owns the nonempty Type2 keyword prefix: it appends the canonical Study-owned Keyword, then reaches the actual notifier admission; unsuppressed notification clears flag90 and returns `KeyConstructionRequired`, stopping before the signal-key construction. `src/doe/variable_notify.cpp` owns that notifier entry and its actual Session/Project/SUB/Study/I/O/manager/notifier membership checks. `include/m2m/engine/process_codec_owner.hpp` remains Unknown and has no production codec setter or read-admission contract. These are the relevant future source owners; the notifier test owner is `tests/doe/test_keyword_notifier_entry.cpp`.

The public Save route remains separately incomplete. `src/engine/save_current.cpp:779-792` reaches owned Regions directory creation and operation-model lookup; nonnull operation enumeration is guarded. `src/doe/doe_study.cpp:411,413,452` retains populated variable/constraint writer guards. The sample sequence, geometry mapping, and DOE file emitters do not establish that public Open completed or that public Save reaches them. The DOE emitter itself identifies DOE output as unsupported at this stage (`src/mst/emit_b_doe.cpp` header comment).

## Retained evidence and exact missing witness

The controlling bounded reconciliation is `analysis/resume196-doe-integration-next.md`, supported by `analysis/resume160-doe-codec-next.md`, CP171 reconciliation, and the retained original-image qualification. It records that the real nonempty Type2 notifier branch's source is the exact NUL-terminated `2signalSetOprHasDoe()` key. No retained record joins the natural constructor call, selected QtCore null/non-null branch, natural decoder output (if any), and constructor publication in one process/thread/caller occurrence. A separately sampled codec value, ASCII source, conditional null allocation contract, static instructions, or unit fixture is not the consumed operand/output witness.

The smallest required witness binds the natural GUI `+0x0d2995 -> +0x2cf3c0` invocation to constructor `+0x2cf41f`, return `+0x0d299a`, actual notifier, source bytes and QString destination `S+0x98`; observes branch flags at QtCore `+0x7412e` or before `+0x74133`; and follows the selected natural path through QtCore `+0x174be7` to GUI `+0x2cf425`, capturing bounded header/ref/length/payload and aliasing or the actual exception. The nonnull arm additionally requires the actual codec reload at `+0x74184`, virtual target at `+0x7247e`, and natural output/return. Stop before GUI map lookup `+0x2cf454`. Tie all events to frozen input hashes, process start/generation, thread/order, loaded module identities/bases, caller and destination. The retained `resume160` report records module and bounded code-range hashes; it is the qualification source, not a substitute for that live same-occurrence observation.

The independent Regions alternative is not a source-ready shortcut: `resume196-doe-integration-next.md` requires the exact QDir resolver return and publication into the same private object, followed by a separate fresh QFileInfo engine/metadata result. A successful mkdir or path string does not supply those observations.

## Public Open→Save relevance and acceptance

The QString witness would qualify only the reader's key-construction prefix. It would not prove receiver/map lookup, normal Type2 Open completion, later study ownership, or any public Save output. Retained historical DOE comparisons also start from project inputs lacking the active study; after-state files cannot be used to fabricate the missing pre-Save study/action owner. Therefore neither this witness nor an isolated serializer can close the public Open→Save requirement.

After evidence qualification and source review, the bounded notifier acceptance pair should extend the actual `ConversionSession -> parseSubProject -> Type2 reader` route in `src/doe/variable_notify.cpp` and `tests/doe/test_keyword_notifier_entry.cpp`. It must assert the exact canonical Keyword and owners, flag90/partial-effect ordering, observed QString value/header/data/ref/alias at publication, then preserve the guard before map/list/receiver dispatch. Cover null and nonnull arms independently, wrong/foreign/expired contexts, suppression-before-key, absent Session and unknown codec; no fabricated values, task publication, sample mutation, or extra writes. This is a prefix regression only. Public acceptance additionally requires fresh original and frozen-candidate Open–Save–Close comparison for the same owned active-study input, ordered partial effects and reachable DOE variables/output/optimization, with all previously guarded boundaries resolved.

## Scope

Read-only source/evidence review only. No source, tests, build, GUI, original process, mutable REA database, parent DMO or Claude state was changed; no tests were run. The report is written to `analysis/resume200-doe-source-ready.md`.
