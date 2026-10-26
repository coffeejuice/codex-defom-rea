# CP159 DOE empty-operation source review

The new production branch is consistent with the approved bounded scalar value prefix. I found no production blocker in the reviewed implementation. Checkpoint integration remains pending: the new test calls the private helper with a null source and does not establish the actual Type2 lazy-reader/notifier bypass or the real A,B,A header/Keyword split. Those are test-coverage blockers to claiming the plan's integration validation, rather than evidence of a production defect.

This was a source-only review. No source/test edit, build, test execution, GUI, original process, or mutable REA operation was performed. Only this report was written. Original behavior is qualified by `resume159-doe-next.md` and the independent plan review; this review does not provide a fresh original empty-operation observation, native allocation/refcount parity, parser success, or Open-to-Save acceptance.

## Production findings

- `keyword_reader_prefix.cpp` retains the exact five scalar attributes, TLOC spelling, nominal spelling/text, mode0 and local dimension3 bounds. The empty branch requires `KwOperations` with no attributes and `children.empty()`, so a raw comment, PI, CDATA, text, null child slot or unknown element cannot masquerade as an empty element list. The existing operation1 profile remains a separate selection. Empty operations are derived from that selected DOM shape, never a supplied count or parse failure. Ordinary whitespace-only XML content is removed by the existing Qt-like XML parser; the childless gate is on the parsed DOM, not the literal bytes. This is not a newly introduced parser rule.
- The empty branch executes immediately after the canonical append and throws before the subsequent `source && source->notifier` check. It therefore cannot read suppression, clear flag90, build a notifier key, obtain/populate a receiver list, or dispatch a callback. It fixes the unconditional-notifier hazard identified by the plan review without fabricating a notifier return.
- Initial lookup resolves `originalName` through owned concrete header aliases. The reused constructor helper rejects foreign/duplicate aliases, mismatched owner counts, competing owner kinds and nonempty canonical Item lists before mutation. It commits the same typed Keyword into the named owner's canonical Item list and unions only supplied IDs. Empty input therefore preserves preexisting `Var.ops`, including existing order/duplicates.
- The empty continuation re-resolves the original name on the same Study, checks that it is the original receiver, and verifies the last canonical Item still holds the returned appended record. It writes selector `-1`, false function/enum flags and the exact nominal string on that Item. It does not reuse the outer reader's last header pointer. The append helper's fresh-list requirement means the current list contains exactly the newly appended Item.
- Direct nominal-text assignment is value-equivalent to the recovered setter for the only admitted text, `TLOC    \t0.1` (with a tab): that text contains no CR/LF, so CR removal/LF-to-CRLF normalization is an identity operation. This does not recover a general setter, codec conversion or QString data lifetime.
- The explicit unsupported exception names the scalar partial prefix and `0x140910abf`, before the upper-text probe. `[[noreturn]]` remains on declaration and definition. The outer reader cannot reach later children, reindex, deferred links or outer Operations; the lazy caller cannot reach sample-table/PAR reads after the throw. The constructor's upper/lower text, data slots, defined flags, numeric nominal and save selector remain unchanged.
- The existing operation1 path still constructs `{1}`, then follows the same contextual notifier entry or source-less guard. No Study/Session interface, C ABI, Save admission, Regions resolver or public successful parser return was added. Existing public nonempty Save guards remain in `doe_study.cpp`; this branch only advances retained partial state before refusal.

Two comments should be corrected before release. `include/m2m/doe_variable_record.hpp` still documents only the B121 constructor profile, omitting the new empty scalar continuation. The private reader header says operation1 “stops before notifier,” although contextual operation1 enters the existing notifier prefix and can clear flag90 before the unresolved key guard; “retains its notifier-entry/key guard” is more precise. Neither comment changes runtime admission.

## Test assessment and required integration work

The new direct-helper tests have Testkit entry points, resolvable private relative includes, supported `CHECK`/`CHECK_EQ`/`REQUIRE` macros and no C assert. CMake automatically discovers the new `test_*.cpp` file. Their positive assertions establish the helper's nominal text, scalar flags, operation lists, preserved A ops and retained upper/lower text; operation1 checks constructor-null nominal text and its source-less guard.

They cannot detect the central notifier regression because both calls pass `nullptr`. The A/B test manually assigns B's description, so it does not test actual A,B,A parser order, deferred links or the original-name lookup producer. No new test rejects raw-child/nonempty/malformed empty-operation inputs or foreign/nonempty owners on this new branch. Existing `test_keyword_reader_prefix.cpp` covers several old profile/owner refusals, and `test_keyword_notifier_entry.cpp` already covers contextual operation1 flag reset/key guard and suppression. Run those existing tests in the focused group; they do not substitute for the new contextual empty branch.

Before treating CP159 as integration-validated, add an exploratory XML case through actual `ConversionSession`/Type2 lazy parsing with recording StudyIo bound to the session's model manager and notification owner:

1. Use exact childless `KwOperations`, same owned Study, and actual A,B,A rows (or preseed coherent A/B followed by duplicate A). Verify A's canonical Keyword receives nominal text, B receives the preceding header/Description effects, queued links stay unresolved, indices/selection retain the reached prefix, and A's prior operation union is unchanged.
2. Seed actual notification flag90=true, a key sentinel and known attempt count; verify unchanged state and no delivery. Verify only the InputTable read occurs, samples/counts and later PAR/sample data are unchanged, no writes/DOM replacement occur, and the diagnostic is the `0x140910abf` scalar stop.
3. Probe raw comment/nonempty text/CDATA/unknown child and unexpected attributes under `KwOperations`, plus nonempty named Item list/foreign alias refusal before append. Keep exact operation1 regression coverage and the public nonempty Save guard in the focused group.

An explicitly accepted narrower qualification could retain direct-reader-only validation, but must not describe CP159 as actual lazy-reader/notifier integration coverage. The current implementation handoff correctly states that limitation; this review does not silently accept the broader claim.

## Reviewed SHA-256 identities

Paths are relative to the converter subtree. Hashes bind the versions reviewed here, not later integration edits.

| Path | SHA-256 |
| --- | --- |
| analysis/resume159-doe-next.md | BEA5EAAEA22BFB5D3456DF982B802A2849DA57D8BD0CAC7AE9560EBF60F9827B |
| analysis/resume159-doe-plan-review.md | FA3C3E16FE3FE3E622D56D1825695CC48FC494719893372FB7BB048119753E2B |
| analysis/resume159-doe-empty-implementation.md | CBC09FBB0F82C371FFDC5BA6E2E1848FD8D64514763768B6CCBFEF883DC3E293 |
| source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp | 8FB6412E44EE3720C6740ABA04A8E1B63FF0AFAC9026C11EDB55644F3899A1F4 |
| source/apps/moproj2mst/src/doe/keyword_reader_prefix.hpp | C14E6FA1DE5EC009F89271FA47B61B4D23C80271976F56CEE28CC9C6A431AC58 |
| source/apps/moproj2mst/src/doe/keyword_constructor_record.hpp | 039F049DD4E7C39560BB0313BA88C8ACF0F5ACDAAFCEFB16A11C612BF5B3593C |
| source/apps/moproj2mst/src/doe/keyword_constructor_prefix.cpp | 799F0B0FB17226091828F125810838BEA2A1F27E1F562FBFB5DF3D28E28D1C3A |
| source/apps/moproj2mst/src/doe/variable_record.cpp | 829CFBCA780A0F95AD0313C75580998D8EBC31FAFB461B0CCE601A8B338F3ABF |
| source/apps/moproj2mst/include/m2m/doe_variable_record.hpp | 3E1976E79B4205D3A5003F6ADC0D439EFC1C70ED285461E04348587ADEFF4AE8 |
| source/apps/moproj2mst/tests/doe/test_keyword_reader_empty_operations_prefix.cpp | A46F991FAF0C98E099A74C4FFBFE910F77D4C40405E876BD11A35B003FED6C2B |
| source/apps/moproj2mst/tests/doe/test_keyword_reader_prefix.cpp | 4839425CC6775E978B40017381C4BAD4E52685212D9772872741FDDEE764C9AD |
| source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp | 7458A06DDDDF2B80D090DA5C45BC1A4E5AE994FC2E7EE6A2F0BD6C1F0B6E4B6A |
