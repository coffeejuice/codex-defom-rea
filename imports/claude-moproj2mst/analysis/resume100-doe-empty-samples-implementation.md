# CP100 explicit-empty sample count import

An exact direct DoeVariable with explicit empty numSamples can now create an actual Study-owned mode0 record with sample count0. Both quote forms use the same bounded profile as CP99's missing-samples branch: explicit nonempty name, canonical-positive varId and explicit dimension3. Duplicate rows ignore the incoming count and apply later reader setters to the last record. Present nonempty conversion, payload and Save guards remain.

Root verified CP99's archive freeze and authorized the exclusive implementation after `resume100-doe-empty-samples-plan.md` was frozen, SHA-256 `6EA1773DD5999A2176091068959F9DC4E50FA0FBFFF0B243D91D48FFFD99E2E8`. That report binds the exact original numSamples callsite, generic QtXml quoted-empty/DOM-found-value chain, QtCore length0 conversion and actual record/duplicate ownership. No new capture or inferred field analogy was needed. All evidence limitations in that plan remain attached to this implementation.

## Source behavior

`src/doe/variable_record.cpp` retrieves the actual sample attribute once. Missing or empty text produces an engaged optional integer0; present nonempty text still goes through the existing canonical-positive conversion admission. The explicit-header reconstruction bound now applies to both zero-default profiles. Normal positive-sample optional-header behavior is unchanged.

On a new name, the same concrete `HeaderVariableRecord` is secured on Study before its borrowed pointer append, then name, selected index, invalidation and subsequent headers/Description follow the existing qualified order. Constructor `numSamples` stores0. On an existing name, no owner/sample/name/selection/invalidation mutation is added; reader-last-record setters retain the original behavior. Coherent concrete owners, foreign/retained-prefix/mode guards and mixed LinkTo/Keyword/Geometry/other-child guards still precede mutation.

The native value result is0 because QtXml's normal explicit-empty attribute has length0 and native QString::toInt reaches the length0 failure result with a null validity pointer. This does not make the call locale-free: native default QLocale construction can initialize/publish globals before empty detection and C fallback. The candidate continues the existing String/DOM numeric value abstraction; it does not replay actual native global/ref/allocator/failure timing or qualify unknown process state. No codec/shared-null, QFileInfo, QIcon or receiver delivery guard is released.

Later guard behavior remains incremental. A later unsupported matched row leaves the earlier zero-count record with index-1 before whole-list reindex; a later top-level sibling leaves index0 after that phase. Actual lazy activation continues LHS/FF/USR writes and current DEF_DOE.PAR read on the same Study/StudyIo. Same-owner Save remains refused at nonempty-vars dimension reconciliation before XML open; no output, notification or refresh success follows from this reader advance.

## Focused tests and static preflight

New `tests/doe/test_variable_empty_samples.cpp` has six cases:

- Both quote forms create one concrete borrowed-alias owner with count0, headers/Description, index/selection and invalid flags.
- A,B,A with empty samples on the last row preserves both creator counts, selection and invalid state, while B receives later ID/Description setters.
- Empty, missing and positive creator profiles each retain their original count across later empty/positive/missing duplicate rows.
- Later matched-row and sibling guards preserve the reached zero-count record and distinct index phases.
- Other missing-header combinations, empty name, bad ID, present whitespace/negative/leading-zero/zero/overflow samples, wrong mode, foreign aliases and mixed payloads remain guarded.
- Actual internal Session/Project/Type2 SUB lazy read preserves the same owned Study, ordered sample effects/current read and unchanged nonempty-vars Save refusal with old DOM/no XML or notification delivery.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_variable_empty_samples.cpp` passed with no issues. It checked supported Testkit macros, main entrypoint and resolvable quoted includes against actual target include directories. Scoped `git diff --check` was clean. No worker build/compiler/CTest or GUI/native process/mutable database was used. Root owns build/test acceptance. Target `m2m_doe_variable_empty_samples`, CTest `m2m.doe.variable_empty_samples`, is discovered by the existing CMake glob.

Root separately removed the now-obsolete full explicit-header empty-samples guard entry from `test_variable_missing_samples.cpp`. This lane did not edit old tests, shared build, Session, reader/writer, convert or C ABI files.

## Source freeze receipt

All paths below are under `source/apps/moproj2mst`; the hashes were sent to root and the independent reviewer after static preflight, before this prose receipt was completed. The files remain held through root validation/archive.

| File | SHA-256 |
| --- | --- |
| src/doe/variable_record.cpp | A50490BE544A265575FC9A87C8DFC46C89D158516968E87A444826CA6BFE7F1F |
| include/m2m/doe_variable_record.hpp | 58336B10C14EDF9C63BF5FDE88E7F65C2A8933B7B6B8D099881AB3B8D12444CC |
| tests/doe/test_variable_empty_samples.cpp | 76AA29C8AB3C117577C42485A1EA315B0665D7CBA0956B596C82F15C92B28102 |

Next bounded dependency is present textual0 or another source-qualified numeric profile; it requires the nonempty original conversion branch rather than treating empty-value evidence as a general toInt implementation. General zero-count optional-header combinations and nonempty-variable Save reconciliation remain guarded. Public refresh still needs its actual receiver/widget/timer lifecycle, not a synthetic Study substitute.
