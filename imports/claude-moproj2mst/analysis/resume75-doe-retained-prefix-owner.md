# Checkpoint 75: retained B121 scalar-prefix data owner

Implemented the isolated actual-Session scalar-prefix entry and Study-owned concrete data retention qualified in `resume74-doe-retained-prefix-owner.md`. The stack driver can now unwind at the existing SetOprHasDoe guard while the actual Study keeps the early variable/keyword mutations and alias valid. No public reader/activation/Save integration was changed.

Root released implementation only after CP74's frozen receipt existed. This lane changed exactly four source/test files plus this report; it ran no build, CTest, original application, GUI or mutable REA operation. Root owns compilation/executable verification and the source archive.

## Interface and ownership

`Study::retainedScalarPrefix()` returns `shared_ptr<const B121ScalarPrefixState>`. The mutable owner is private and has no public reset. State is forward-declared in the Study header, so no Study constructor/destructor or Var polymorphism change was required.

`src/doe/variable_prefix.hpp` defines the complete state. It owns `unique_ptr<ScalarVariablePrefixRecord>`, pending signal values, selected index and started bit, with const observation accessors only. It has no Study, Project, Session, I/O, notifier, callback or native handle. Concrete deletion never passes through Var. An escaped const observer keeps only diagnostics/allocation alive, not the original runtime owners.

New internal entry:

```cpp
[[noreturn]] void readOwnedB121ScalarPrefix(
    engine::ConversionSession& session, Project& project,
    SubProject& sub, const xml::Node& doeVariables);
```

It verifies Session.liveProject and owned SUB membership before following backlinks, then exact active Type2 Study/raw alias/backlink, existing actual StudyIo, Session ModelManager owner and noncreating shared notifier binding. It does not create/rebind any of those owners. Fresh-state admission rejects existing projection owners, variables, rows, nonzero sample mode, incompatible root or retained attempts before a new state is attached.

After admission, the entry stores the driver's data state on the actual Study before allocation/append. The existing read path owns the concrete record before appending its borrowed Var alias, then retains the original source mutation order. It still stops with M2M_E_UNSUPPORTED at:

```text
DOE variable prefix 0x14090ee20: 2signalSetOprHasDoe() delivery is unresolved after committed keyword/operation prefix
```

No actual notifier dispatch/reset/population/UI effect is performed by this ownership slice. Pending nominal text is separate from the keyword's still-empty committed text. No root Operations read, post-message parameter/selector/text setter, variable reindex, samples, temp.KWD, successful parse or public Save is claimed.

An earlier guard can retain an appended but unfinished record and no pending signal. A host allocation exception can also leave an attached started attempt with no record or no appended alias; State existence is not an append/constructor-completion status. Re-entry refuses instead of resetting/replaying the attempted prefix.

The existing standalone B121ScalarVariablePrefix constructor/accessors are preserved. Detached mode still erases its own alias on driver destruction. Retained mode skips that erase because Study owns the same data. No driver is stored inside Study, so the driver's bounded strong Study/I/O leases cannot form a cycle. State destruction follows no runtime backlinks and only releases its concrete allocation/value fields. Ordinary Model.vars destruction does not dereference its pointers; no half-destroyed Study traversal is added. No native Close/delete protocol is inferred from adapter cleanup.

## Focused tests and static preflight

New `tests/doe/test_retained_scalar_prefix.cpp` has eight Testkit cases and `testkit::run_all()`:

- Actual Session/Type2 owner and exact pending prefix survive the driver call's guard; nominal text remains uncommitted and I/O/notifier counters/flag are unchanged.
- Retry keeps the exact state, allocation and single alias.
- Foreign Session, inactive SUB, bad alias and unbound replacement I/O refuse before state/model mutation.
- Wrong root, mode1 and preexisting rows refuse before owner publication.
- A varId0 guard retains the earlier appended record without inventing a keyword/pending result.
- Releasing actual Project storage destroys Study while an explicit const observer may retain diagnostic data; releasing that observer permits State reclamation.
- Clearing SUB storage with only weak observations expires both Study and State, checking the no-cycle design.
- Detached cleanup remains independent; public nonempty reader and Save reconciliation guards still refuse.

These are written checks, not executed results. Existing `tests/doe/test_variable_prefix.cpp` was left unchanged and should run alongside the new packet to catch mutation/accessor regressions.

Static preflight completed:

- `git diff --check` on assigned existing files reported no diagnostics; a direct whitespace scan also covered the new file and reported none.
- All quoted includes in the four assigned files resolve through source-local paths or actual target include roots. Existing testkit macros/main were checked; no C assert or unsupported REQUIRE_EQ is used.
- Source was inspected for typed ownership, state-only retention, unchanged detached destructor behavior, admission-before-publish and the unchanged behavioral guard. No source file outside this lane's write set was edited.

CMake's existing test glob discovers the new file. Root targets: `m2m_doe_retained_scalar_prefix` and existing `m2m_doe_variable_prefix`; CTest names `m2m.doe.retained_scalar_prefix` and `m2m.doe.variable_prefix`. No CMake edit is necessary.

## Frozen files

Paths are beneath `source/apps/moproj2mst/`.

| File | SHA-256 |
| --- | --- |
| include/m2m/doe_study.hpp | 80E61D48A5C3588816D4463634AEF86B5E34C39558F34478F42513E7813B2EE4 |
| src/doe/variable_prefix.hpp | DDC049B0E824F544AE16063937C24791C6EB0F6DB20493065BF64371DE917F45 |
| src/doe/variable_prefix.cpp | 8E4F95D1E48561C81116363F5F0010C903FE31A94BB08285F590AB875E73F95F |
| tests/doe/test_retained_scalar_prefix.cpp | AAB790DF0AC00E32D7FCA24AFCB7A0771C09CAC2BAADDC18D761486664451693 |

All four source files are held unchanged for root's grouped validation/archive. Native early-mutation order remains qualified by CP50/51 retained PE evidence; this change adds adapter ownership, not original cleanup evidence.

## Next dependency

The entry is intentionally not called by public readInputTable/readLazyStudy. Root must separately assign an actual Session-aware nested-reader integration interface before that source binding. The next behavioral boundary remains actual SetOprHasDoe receiver population/backend delivery, followed by scalar selector/text/reindex/root operation processing. The direct final B121 model projection/writer remains separate and cannot serialize this interrupted native prefix as complete. Public Type2 variable reconciliation, XML/refresh/downstream output and Type3/OPT guards remain.
