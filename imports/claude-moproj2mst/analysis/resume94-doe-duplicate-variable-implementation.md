# CP94 duplicate names preserve the native reader-last-record behavior

The active bounded header/Description reader now accepts an existing name without creating a record, then applies its following ID/dimension/Description setters to the last concrete variable. This faithfully preserves native A,B,A targeting B. Name/sample count, selection, sample invalid flags and existing tables do not change on the duplicate helper branch. Mixed payload/foreign owner and public Save guards remain.

Root confirmed CP93 frozen before authorizing this cut and its exact DOE-only write set. Qualification is `resume94-doe-duplicate-variable-plan.md`, SHA-256 `B814F20DD4785B2DA67DD8ED53C1E06B9707CE085548353D89D8FEEC408288A8`. That report binds the complete original0x140919de0 existing-name branch, ignored helper return and separate reader count-1 lookup0x14090f0b2..0x14090f0ef, and subsequent setters. No new native capture, execution, GUI, mutable database, compiler/build or CTest occurred in this lane. Root owns grouped validation.

## Source change and boundaries

After unchanged concrete owner/alias identity/order checks and bounded mode0/header/child admission, helper name lookup now records whether a name exists. An existing name selects `headerVariableOwners.back()` for the reader continuation, bypassing every new-object effect. It does not preserve the native helper's matched pointer as the setter target: that pointer is ignored by the original reader, which independently gets the last list object. A match implies a nonempty coherent concrete owner; no raw/foreign record adoption or empty fallback is introduced.

The unique-name branch retains constructor/owner/pointer append/name/selection/invalidation behavior. The shared following reader body assigns positive ID and dimension3, then applies ordered nonempty Description text. Duplicate numSamples remains parsed for bounded admission but is not stored. No generated ID/next-number query is added to the existing-name branch. Empty descriptions preserve the last record's text. Normal completed traversal reaches existing reindex; a later guard preserves reached setters and earlier indexes/state without whole-document rollback.

String/name/text allocations retain existing host value behavior. The duplicate branch performs no concrete record allocation/push, not a claim that native Qt lookup/string temporaries allocate nothing. Native refcount/COW/raw-null/fault timing and cleanup parity remain outside the adapter. A later host text failure may leave earlier reader header/text mutations on the last record; no failure emulation or rollback is added.

Mode0, canonical positive samples/ID, exact dimension3, no LinkTo, direct Description/raw-child profile, concrete ownership, int32 count bound, and scalar/geometry/retained-owner restrictions remain. Mixed Keyword/Geometry/other direct element profiles retain the pre-append reconstruction refusal; that earlier adapter guard is not called native failure ordering. Public Save continues to stop at nonempty-variable dimension reconciliation before XML open, without notification/refresh/output completion.

## Tests and corrected obsolete assertions

New target `m2m_doe_variable_duplicate`, CTest `m2m.doe.variable_duplicate`, has seven Testkit cases:

- A,A retains one exact record/name/samples and updates its ID/Description.
- A,B,A leaves A untouched, updates only B's ID/Description, and preserves both identities/names/sample counts.
- Explicit same-owner selected index, mixed invalid flags and sample tables distinguish skipped duplicate helper stores; empty descriptions keep B's prior text.
- Duplicate reader setters survive a later unqualified matched row with indexes still-1.
- Mixed Geometry payload and foreign alias still refuse before current setters.
- Exact case-sensitive name existence controls the branch.
- Actual Session-owned Type2 lazy A,B,A input retains two actual records, ordered sample effects/current PAR, and the same Save reconciliation guard before XML with old DOM/identities/no notifier effects.

Only genuinely obsolete duplicate-refusal checks changed in existing tests. The record partial/retry test uses an unqualified ID0 row, retaining its intended preceding record/index-1 state. The Description partial test's later row now has direct Keyword payload, retaining its earlier description/index-1 intent. The explicit unique-append duplicate-guard case now asserts the supported A,B,A last-record result. All other tests are unchanged, including notifier Keyword/payload refusal.

Static preflight resolved quoted includes against source-local/target include roots, verified Testkit main and supported CHECK/CHECK_EQ/REQUIRE with no C assert/REQUIRE_EQ, and found no trailing whitespace. `array` is explicitly included for the invalid-state fixture and `qt4path.hpp` for kSep. git diff --check reported no diagnostics; direct scans cover untracked/new files. Tests are written, not worker-executed. Root should run new duplicate plus existing record/description/unique-append/notifier-binding targets, then the full source-stable launcher.

## Frozen source

All paths are beneath `source/apps/moproj2mst/`. The six files were released immediately after static preflight and are held unchanged through root build/tests/archive. No Study/reader/Session/activation/shared writer/convert/CMake/C ABI or original fixture changed.

| File | SHA-256 |
| --- | --- |
| `src/doe/variable_record.cpp` | `D874EB6C753810ACAC163016C41849F138D7EA9CAC217101A812BE3A2BF3C446` |
| `include/m2m/doe_variable_record.hpp` | `D901723301C740BD118226D0E818FEB49CA96E722FF65E401977027AEC91986F` |
| `tests/doe/test_variable_duplicate.cpp` | `9A34F917C0F1990A76C2CB6DD5BA4596E12E3B7C90B2ABF386A3313536AAB49B` |
| `tests/doe/test_variable_record.cpp` | `0CD5999D031A4408D914EBB991566F8BB47D0573C5981A6C793D6512C0F58F1E` |
| `tests/doe/test_variable_description.cpp` | `C4608B5B02E103DCCCB7C0B3E9015DA1C22AD83F9FEAB9286CB90DB5D7B4F0B3` |
| `tests/doe/test_variable_unique_append.cpp` | `97E74B2B1D443D42627747F04C707449D6A44FC3039432B8E54EF36B5668A20F` |

Next independent variable state may be a bounded default/optional header or deferred-link path; it needs its own exact order/owner proof. Keyword/Geometry, general conversion/default locale, native ref/allocation parity, output and Save reconciliation/refresh remain guarded. This is real production duplicate-row behavior, not scalar parsing or public Save completion.
