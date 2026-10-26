# CP159 internal lazy-reader test source review

The new `doe_lazy_empty_keyword_operations_commits_named_scalar_prefix_then_stops` case is reachable and correctly tests the internal `readLazyStudy` → named-source InputTable → direct DoeVariable → direct Keyword path. No source blocker was found for that bounded claim. It closes the private-helper-only gap for lazy study file reading, but does not close actual ConversionSession/non-null notifier coverage or the A,B,A integration gap identified in the main CP159 source review.

The reviewed handoff explicitly excludes project-open, public Save, sample/PAR/DOM completion and native success; those qualifications are accurate. The test was inspected only: no build, execution, GUI, original process or mutable REA action occurred. Only this report was written.

## Reachability and fixture

The fresh `Study` has mode0 and coherent empty concrete-header ownership. `readLazyStudy` first reads the study name `Existing`, then obtains the input file through ScriptIo. The row has nonempty name `TLOC row`, canonical positive varId1, exact dimension3 and positive numSamples3. The outer header admission therefore constructs its actual HeaderVariableRecord, publishes its alias and dispatches the direct Keyword child. Earlier concern that this fixture cannot reach the helper was incorrect.

The Keyword has exactly the five required scalar attributes and exact values. NominalData has one raw child, a string element whose text is `TLOC    \t0.1` with a tab. `KwOperations` is self-closing with no attributes or raw children. These are the intended empty-profile inputs. XML attribute order and quote style do not affect the existing DOM gate. The synthetic `<x>` root is accepted by the current parser; this is exploratory candidate input rather than an original captured case.

## Actual context and observed assertions

This test calls the four-argument `readLazyStudy` overload. It forwards `nullptr` as notifierContext; the implementation still creates a real, non-null `InputTableSource` containing the successfully read filename, but leaves `source.notifier` disengaged. There is no ConversionSession, bound model-manager/notification owner, flag90 sentinel, key sentinel, or callback/attempt observation in this case. It must not be presented as a test that proves notifier bypass when a real context exists.

The assertions correctly check the reached scalar diagnostic, earlier study-name/ROI8 effects, concrete Header owner and alias identity, one canonical typed Keyword, empty record/variable operation lists, selector/type flags, nominal text, and still-null upper/lower text. The exact one-item `io.order` plus read count1 and no text writes shows that the lazy reader unwinds before its sample-table writes and DEF_DOE.PAR read. A null outputDocument is consistent with this prefix, but does not establish project DOM replacement prevention because no project DOM exists in the rig.

Coverage is intentionally narrower than the full integration plan: the test has one fresh row, so “named” lookup cannot distinguish A from the outer last-row receiver; no prior ops union, queued link, selected index, reindex sentinel or sample/count sentinel is seeded. It also matches the scalar diagnostic phrase rather than asserting the explicit `0x140910abf` boundary text. These are remaining coverage limits, not a false pass implied by this test's current assertions.

## Integration disposition

Accept this case as internal lazy-reader/file-I/O prefix coverage after root's grouped compile and execution pass. Keep the broader actual ConversionSession/non-null notifier and A,B,A requirements pending unless root adds that coverage or explicitly records a narrower acceptance qualification. The existing operation1 contextual notifier tests should remain in the focused group. Raw-child and invalid-owner refusal coverage also remains as stated in `resume159-doe-source-review.md`.

Test preflight is sound on inspection: its private include resolves from tests/mdt to src/doe; doe_activation.hpp includes doe_study.hpp, which includes the complete HeaderVariableRecord declaration; the existing Testkit entry point and supported assertions remain, without C assert or REQUIRE_EQ. This review does not claim compilation.

## Reviewed SHA-256 identities

| Converter-relative path | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/tests/mdt/test_doe_activation.cpp | BF69AE11C0503AF11121226C3A6561F317E5DCE34AED013DCECD095CC7C48DDE |
| analysis/resume159-doe-lazy-test-implementation.md | 95C0EACF9DB4CD74A30C899FCA5BA0E8C566099A938EB9DF923E4121D0AD5203 |
| source/apps/moproj2mst/src/doe/activation.cpp | FFB76FCFB6C768BCF15AE77480AB781B5DCA0C9594D1F42A6822B6131EB850D1 |
| source/apps/moproj2mst/src/doe/variable_record.cpp | 829CFBCA780A0F95AD0313C75580998D8EBC31FAFB461B0CCE601A8B338F3ABF |
| source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp | 8FB6412E44EE3720C6740ABA04A8E1B63FF0AFAC9026C11EDB55644F3899A1F4 |
