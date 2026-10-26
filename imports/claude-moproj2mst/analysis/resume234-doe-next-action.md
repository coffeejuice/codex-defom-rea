# CP234 DOE next action

## Decision

No new production edit is justified by the retained CP215-and-later evidence. The smallest new source/static-evidence increment is a bounded call-chain audit of the original Add Study command: identify the QAction/command handler, the first study/SUB creation call, and the first selection or sample-generation call, then compare those exact effects with current converter entry points. This refines the missing B120 edge without repeating the already completed before/after file-state audit. Do not claim DOE closure.

## Reconciled frontier

CP215 already added and reviewed the lazy-reader `StudyIo*` identity check (`src/doe/activation.cpp`, `src/doe/input_table_reader.hpp`, `src/doe/keyword_reader_prefix.cpp`; regression `tests/doe/test_keyword_notifier_entry.cpp`). Its focused test and 362-test freeze passed. It neither creates a nonempty key nor witnesses receiver dispatch.

For nonempty Type2 keys, `resume203-doe-publication-static.md` remains controlling: the helper's codec CMP branch and natural decoder output must be observed in the same occurrence before QString publication can be admitted. `resume215` does not change that gap. Keep the keyword and public Type2 continuation guarded; do not reimplement the reader-I/O prefix.

For B120, `resume203-b120-doe-state-static.md` verifies a no-study before tree versus an after tree containing Type2 Id1/Active5, `SfDoeStudy`, and seven DOE files. It explicitly does not assign those outputs to Add Study versus Save. CP218 and CP223 additionally establish the public converter selects subproject 0 and that current existing-study construction/sample/serialization prefixes do not replay Add Study. Therefore the first missing static link is the original UI command call chain, not another parse of the same XML or hashes.

## Exact audit and decision gate

Use only retained, qualified original GUI disassembly/decompilation. Locate the Add Study QAction/slot or command dispatch and trace its direct calls through (1) SUB/Study allocation and ID/name assignment, (2) active/current selection mutation, and (3) any sample/design generation call before Save. Record address, caller/callee, argument sources, writes and static-vs-runtime limits. Compare each effect against `source/apps/moproj2mst/src/model/subproject.cpp`, `src/doe/doe_study.cpp`, `src/doe/activation.cpp`, and `src/core/convert.cpp` (including its fixed subproject-0 activation). Check whether the chain reaches an existing owned test seam before proposing any change.

If this call chain is absent from retained bytes or its decisive branch depends on runtime state, the audit should stop with that precise witness requirement. The later dynamic proof would still need one same-process Add Study → resulting identities/selection → sample generation → explicit Save trace. Do not infer the 100 empty sample rows' producer from the saved files.

## Tests and blockers

This evidence-only increment requires no new test. If—and only if—the static chain proves a distinct source-ready behavior, add a focused Testkit case at the owning source seam, with a Testkit entry point and supported assertions; then run that target, the changed-target group, and the complete source-stable launcher before freeze. No production behavior should be enabled from static call shape alone where a runtime-selected branch or receiver result is required.

Current blockers remain the same-occurrence nonempty-key codec/output and ordered live weak-receiver/backend/callback witness, plus the B120 Add Study/selection/sample-generation/Save witness. `analysis/wave3-doe-next-active-study-dependency.json` also retains the native Report/QObject prerequisite. This recommendation is bounded to CP215 and the later reports named above; it does not assert exhaustive coverage of all retained artifacts.
