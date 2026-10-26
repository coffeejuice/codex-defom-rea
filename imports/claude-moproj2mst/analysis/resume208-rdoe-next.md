# CP208 RDOE next action: preserve Save prefix and qualify notifier codec

## Decision

Do not change DOE production source or tests in this cut. The smallest justified result is an exact evidence action for the nonempty `SetOprHasDoe` key construction. The empty-variable Type2 Save prefix is already implemented through InputTable XML open/write/close/status and stops at Session-owned refresh delivery. Do not extend it from the B120 after-state or synthesize a receiver result. The nonempty-key path is independent and cannot authorize Save continuation.

## Reconciliation of the two DOE boundaries

- `analysis/takeover-state.json` RDOE next_action records the existing empty Type2 Save prefix and requests separate same-occurrence evidence for study creation and codec publication.
- `analysis/resume203-b120-doe-state-static.md` verifies B120 before has no study, while after has a newly added Type2 Id1 Active5 and seven study files. That pair does not identify the add action, selected-study transition, sample generation, or which action wrote the files. It cannot justify automatic study creation or further serialization.
- `analysis/resume203-doe-publication-static.md` confirms no source-ready nonempty notifier state prefix. Existing static bytes leave the QtCore codec CMP outcome, possible decoder return/output, and natural QString publication data dependent.
- Existing populated-variable Save guards remain in place. The empty-variable prefix is not proof of refresh delivery, DBName/DOM replacement, later sample writes, full Save, or Open–Save–Close parity.

## Exact next evidence action

Acquire one natural nonempty Type2 `SetOprHasDoe` occurrence, without seeding the codec or calling the decoder manually. Freeze and hash the existing-study input. Bind process start/generation, thread, loaded GUI/QtCore identities and bases, caller stack `S`, actual notifier and imported-call target, and event order. Capture:

1. GUI caller `+0x0d2995 -> +0x2cf3c0`, dispatcher `+0x2cf41f`, return `+0x0d299a`, the exact NUL-terminated key `2signalSetOprHasDoe()` and destination `S+0x98`.
2. The same helper occurrence's consumed CMP flags immediately after QtCore `+0x74126` (`+0x7412e`) or before JE `+0x74133`, and the naturally taken edge. A standalone read of global `+0x2f3e10` is insufficient.
3. On the nonnull edge, the same invocation's codec reload at `+0x74184`, virtual target at `+0x7247e`, and natural decoder output/return. On the null edge, follow the natural constructor path; static fresh-allocation shape is not an allocator receipt.
4. Natural data publication at QtCore `+0x174be7` and caller return at GUI `+0x2cf425`; record bounded QString data header/refcount/length/payload and alias/ownership, or the actual exception/fault. Stop before map/list lookup at `+0x2cf454`.

An unrelated constructor occurrence, missed event, unreadable output, timeout, or failed return is incomplete evidence; preserve it as such. Compare the joined trace against the retained QtCore and GUI hashes documented in CP203. Then independently review whether the observed prefix supports only a bounded null or nonnull key-construction test. Keep map/list/receiver dispatch, normal Type2 Open, Save continuation, and full lifecycle acceptance separate.

## Separate study-creation action

If continuing the B120 question, use the actual no-study before input and obtain one same-process/action trace of natural study addition, new SUB/Study identity and id/name, current-selection/diagram transition, sample-generation state, and subsequent Save. Do not reorder subprojects or inject a Type2 record. This action is independent of the key-construction witness and empty-variable Save prefix.

## Scope and evidence

Read-only bounded reconciliation of `analysis/takeover-state.json`, CP160/171/196/200/203 DOE reports, current DOE source-frontier notes, and B120 retained-file audit. No source/tests/build/original/GUI/REA activity or shared state edit. This file is the CP208 resume cursor; no claim of exhaustive absence beyond the named bounded evidence.
