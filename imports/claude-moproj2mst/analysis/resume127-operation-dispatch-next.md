# CP127 operation/property dispatch next boundary

## Finding

There is no new source-closed public operation/property branch after the currently integrated property-table step borrow. The strongest reachable positive branch from the recent inventory—the empty-variable Type2 Study Constraint XML serializer—is already integrated and regression-tested (CP124). The actual Type2 Save then reaches a Session-owned `DoeTableRefresh` notifier prefix; CP125 integrates the unsuppressed `flag90=false` store. After that store, the next native operation is a runtime-dependent signal-key QString construction, followed by runtime map/list lookup and guarded receiver dispatch. The required process/codec, registration, QObject/timer, and callback-return owners are not bound to the Save occurrence. Keep the public refresh guard; do not wire later output writers or declare Save completion.

## Dispatch inventory reconciliation

- Boolean `vt+0xb8` step-4 side-file Save is already integrated in `source/apps/moproj2mst/src/core/convert.cpp` (CP72/117). Its distinct step-5 MST write remains separate and covered.
- Owned Constraint serialization for the zero-variable Type2 XML path was integrated in CP124 at `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp`. It appends rows in the qualified native order after Operations, before XML serialization, and preserves actual file/open/close effects. It is not a detached writer and is reached through the actual Study-owned Save prefix.
- `source/apps/moproj2mst/src/doe/table_owner_state.cpp` now models the source-ordered Session notifier suppression read and, only when unsuppressed, `flag90=false` store before key construction. CP125 tests ordinary success, open failure, post-close failure, suppression, interruption, foreign scope and release boundaries.
- Active `SfOprDoeOutput` Save/Load remains separate. The active B120 `.DS` read reaches the first direct DBLIST/ReportList guard before the later task/session prefix; no evidence supports skipping or reordering that guard.

The native Type2 Study Save path thus has a concrete progression—XML file effects, owned Constraint prefix, then notifier scalar store—but the currently next operation is not a pure property field or source-independent conversion that can be safely advanced in code.

## Exact owner/callback dependency

The Save wrapper calls dispatcher `0x1402cf3c0` with global notifier `0x143d76700` and key literal `2signalDoeTableRefresh()` after the XML writer's normal return. The unsuppressed dispatcher clears `flag90` before it constructs `QString(char const*)` at QtCore import `0x141e04480` / constructor `0x67174bd0`; a normal return then proceeds to map/list lookup, local snapshot and guarded receiver traversal. CP126 closes the branch mechanics, but not the invocation-time process codec read (`codecForCStrings` at QtCore global `0x67074126`) or its actual data/ref owner for this Session. A fresh-null diagnostic profile is not same-occurrence evidence.

Even a completed key would not close the subsequent edge. The actual Save-occurrence notifier map value (hit versus miss), QMap copy-on-write/ref state, ordered receiver list, guarded object liveness, and actual `SfMoDoeTable` instance must be bound. The retained registration in the table constructor is only a potential producer. Main-window `+0x458` publication occurs after construction; the actual child `QTimer` at table `+0xb0` must be live for native `slotUpdateUI()` (`0x1400f4e10`) to stop it. Its refresh body `0x1400f47e0` must return normally, and then the dispatcher/wrapper must return before public Save reaches the following PAR sequence.

Therefore the exact missing witness is a **single Save occurrence** binding: process/module generation and C-string codec/data state; live Session/notifier and actual key/map/list registration snapshot; published same-generation table receiver and nonnull timer; invocation of its actual update slot; normal slot and dispatcher return. Static producer connections alone do not establish that occurrence. This is the receiver/callback boundary, not a missing converter-side key literal helper.

## CP125 implementation ownership

No CP125 production edit is recommended from current evidence. The source-closed notifier scalar prefix is already in `src/doe/table_owner_state.cpp`, and its tests are in `tests/doe/test_refresh_notifier_prefix.cpp`. Another fixed-key constructor/value helper would duplicate a value already known from static analysis while leaving the real process codec, map/list owner, and callback untouched. Do not edit the shared writer, public API, `viewer.cpp`, or property-table interface to force progress.

When a same-occurrence owner/callback witness exists, the next implementation needs a root-owned canonical process/Session dispatch context and must preserve the actual dispatcher order. Only then can a focused DOE source lane own a callsite plus dedicated test. The regression must distinguish key construction failure after the retained flag store, map hit/miss and snapshot state, dead versus live guards, timer stop, callback false/normal-return semantics, and interrupted versus normal dispatcher return. Tests that seed Null codec or fabricate receiver success cannot substitute for those admission facts.

## Scope

This bounded comparison uses `analysis/resume123-doe-active-next.md`, `analysis/resume124-doe-constraint-xml-implementation.md`, `analysis/resume125-doe-notifier-failure-order.md`, `analysis/resume126-doe-refresh-key.md`, current continuation output, and current converter source. No source edit, build, GUI, original-process execution, or mutable REA operation was performed.
