# CP99 absent numSamples on an actual owned Variable

An exact direct DoeVariable with absent numSamples can now create an actual Study-owned mode0 record with sample count0. This increment requires an explicit nonempty name, canonical-positive int32 varId and explicit dimension3. Those conditions bound reconstruction admission; they are not native validation. Existing positive-sample profiles keep their earlier optional-header behavior. Present empty/zero/malformed/overflow sample text remains guarded.

Root released CP98's archive boundary before these source writes. The exclusive write set is `source/apps/moproj2mst/src/doe/variable_record.cpp`, `source/apps/moproj2mst/include/m2m/doe_variable_record.hpp`, and new `source/apps/moproj2mst/tests/doe/test_variable_missing_samples.cpp`. No shared/CMake, old tests, public writer, Session, C ABI, GUI or mutable analysis database was changed. Source/test hashes were sent to root and the independent reviewer immediately after static preflight; all three files are held for root's grouped build/test/archive.

## Original value conversion and owner sequence

Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The complete original reader0x14090ee20 is retained by `resume50-doe-variable-body-pe-receipt.json`, code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08`. The native caller default-constructs the numSamples attribute default around0x14090efae..efb6, retrieves the attribute at0x14090eff1, then passes base10 and a null validity pointer at0x14090f00f..f01f. It does not require attribute presence or a positive conversion result. CP97's exact QtXml attribute/default-copy evidence establishes that absence returns the default-null QString; its data has length0.

New read-only QtCore captures bind DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`:

- QString::toInt0x6706c280 calls toLongLong0x6706c120 and checks signed int32 range. Zero is returned unchanged.
- toLongLong constructs a default QLocale at0x6706c13d **before** converting any text. It gets locale data through0x67049340 and invokes0x6704c460. On failure it constructs QLocale(language1,country0) at0x6706c192 and retries. The caller's null validity pointer is passed on that fallback.
-0x6704c460 initializes its result to0. Its locale separator0xa0 branch first calls QString::trimmed0x670744a0. The retained first unwind fragment0x670744a0..744cd proves length0 immediately copies the same data identity, increments its reference and returns; no nonempty trim branch is needed for this claim.
- The text-normalization fragment0x6704c190..4c1e9 reads QString length at data+8. With length0 it skips the whitespace loop and returns false at0x6704c1db..1e9 before inspecting a character or a locale-dependent digit.0x6704c460 then sets validity false when supplied and returns integer0 at0x6704c525. Both default-locale and C-locale attempts therefore have the same zero value for this absent/null input.

The native default QLocale constructor0x6704d4f0 may call0x6704cd30 and publish the default data at0x672f3d40, depending on the existing globals0x672f3d38/3d40/3d48. This work does **not** claim a locale-free or side-effect-free conversion, actual process-global qualification, native Qt reference/allocator/fault parity, or replay that initialization in the candidate. It extends the existing String numeric **value** abstraction only. The empty conversion result is source-closed independently of the selected locale; native initialization/failure timing remains a separate runtime/lifecycle limitation, common to existing positive numeric header abstractions.

The reader passes the converted int to manager vt+f8 at0x14090f0ac before the optional varId/dimension setters and child dispatch. CP91/94 independently bind vt+f8 to0x140919de0, fresh record constructor0x140816900, append0x140919ec7, name assignment0x140919efe, selection0x140919f8b and invalidation0x140919fb8. Constructor+10 stores the argument directly, without a positivity check. Mode0 skips mode2 design acquisition. A fresh absent-samples record therefore stores0, then follows the same append/name/select/invalidate/header/Description sequence.

An existing-name helper branch returns the match without construction, sample assignment, selection or invalidation. The reader ignores that returned pointer and obtains count-1 before its later setters. Thus A,B,A with missing samples on the third row preserves both first-created sample counts and updates B's ID/dimension/Description. The focused test binds this behavior rather than changing sample count on the matching A or inventing another allocation. Missing-samples duplicate input is admitted only within the same explicit-header reconstruction bound.

## Candidate behavior, partial effects and guards

The existing concrete `HeaderVariableRecord` lifetime and borrowed `Study.vars` aliases remain unchanged. Missing samples selects an engaged optional integer0; no optional truthiness confusion turns the value into failure. The explicit-header bound is checked before allocation/append. Existing coherent-owner/mode0/retained-prefix, LinkTo and mixed child guards still precede mutation.

Successful new records retain sample0 through header/Description assignment, complete variable-list reindexing and outer ID finalization. A later same-list matched payload refusal retains the first appended record at index-1; a later top-level sibling refusal retains its completed index0. The new tests distinguish those phases. They also preserve wrong-mode/foreign-owner and present invalid-sample refusal before any new owner append.

The actual internal Session/Project/Type2 SUB lazy-read fixture uses the same owned Study and StudyIo, writes LHS/FF/USR in order and reads DEF_DOE.PAR after XML parsing. A nonempty current table in this fixture is independent of the record's sample-count field; no generation/execution consumer is enabled. Saving that same owner still refuses at nonempty-vars dimension reconciliation **before XML open**, preserving the old DOM, record identity and write counts. No QIcon, SetOprHasDoe, refresh delivery or public Save completion is claimed. Bare DoeVariable, absent-samples null/empty-name and other missing-header combinations deliberately remain guarded.

## Static validation and source freeze

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_variable_missing_samples.cpp` passed with no issues: supported Testkit macros, main entrypoint and direct quoted includes resolve against actual target include paths. The new test has five cases: concrete zero-count owner, duplicate A/B/A behavior, later-guard partial state, retained admission/owner guards, and actual lazy owner/sample continuation followed by unchanged Save refusal. Scoped `git diff --check` passed. No worker compiler, build or CTest was run. Root owns executable validation.

Target: `m2m_doe_variable_missing_samples`; CTest: `m2m.doe.variable_missing_samples`. CMake's existing test glob discovers it without shared-file changes. Source freeze SHA-256:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/doe/variable_record.cpp | 85C56BFE0C7207D133B693F975B092C3D2E8B59AC7FA353EFBDC773F8334CE58 |
| include/m2m/doe_variable_record.hpp | 594C557EDE968D873FC320774D1AFDEFCAF9F293C472386528357319743E1E30 |
| tests/doe/test_variable_missing_samples.cpp | 6EB202F8B799F83314090BF9A8242ACC8BC0C0E253A819E4DE4A2D3C3C23A80B |

New evidence receipts contain exact DUMPBIN commands, bounds, original hashes and individual capture hashes: `resume99-doe-missing-samples-convert-static.json`, `resume99-doe-sample-locale2-static.json`, `resume99-doe-empty-locale-text-static.json`, and `resume99-doe-empty-trimmed-static.json`. The text-normalizer/trim captures are only their first unwind fragments, sufficient for their complete length0 return paths; no nonempty logical body claim follows. The49340 getter capture is an explicitly bounded leaf candidate, and instructions after its RET are not attributed to that getter. Initial standalone `resume99-doe-sample-locale-6704d4f0.asm` is a historical duplicate from a capture attempt that stopped at a leaf entry; the completed locale2 receipt is authoritative for the selected group.

Next source/evidence dependency: present empty sample text may share zero conversion but requires its parsed-empty attribute provenance to be bound explicitly to this numeric path; nonempty signed/locale/failure conversions remain guarded. General optional-header combinations are not automatically admitted by this cut. Native locale-global/allocator parity and actual nonempty-variable Save reconciliation/refresh delivery remain separate unclosed lifecycle dependencies.
