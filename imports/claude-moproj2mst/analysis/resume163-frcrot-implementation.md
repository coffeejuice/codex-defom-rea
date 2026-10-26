# CP163 guarded FRCROT actual-reader prefix

The Type2 contextual lazy DOE reader now admits one exact synthetic exploratory FRCROT/function profile. It appends registry ID276 to the original named Header owner with empty operations and entity `[1]`, reselects that canonical record, commits parameter/function/enum and the three fixed source texts, then raises `M2M_E_UNSUPPORTED` at `0x140910b6f` before function conversion. The zero-operation branch does not reach SetOprHasDoe. Existing TLOC branches and their error messages remain unchanged.

The admission predicate requires the exact six Keyword attributes, five raw element children in order, one Text child under each of the three single-line ASCII `string` elements, raw-child-empty `KwOperations`, and one childless `KwEntity id="1"`. The entry guard rejects other identities, attributes, wrappers, comments, raw operation children, nonempty operations, other entities and different text before constructor mutation. The code does not call the private table helper, a temp-file converter, Save reconciliation, or downstream sample/link finalization.

Two new tests use an actual `ConversionSession`, Type2 `SfDoeStudy`, contextual lazy read and recording `StudyIo`. The accepted case seeds A,B, then reads A again to distinguish the original named Keyword owner A from the last Header/Description target B. It checks ID276, operations/entities, all three text fields, untouched data slots, the reached boundary, no notifier event or additional recording-I/O write, no later sample processing and no deferred-link completion. The malformed-profile loop uses the same contextual route, includes explicit comment and text children under both `Keyword` and `KwEntities`, and checks that prior Description effects remain while the Keyword constructor is not reached. These inputs are synthetic source profiles, not observed successful original parser cases.

Evidence basis: `analysis/resume163-frcrot-integration-next.md`, independent `analysis/resume163-frcrot-plan-review.md`, and qualified immutable-byte receipt `analysis/resume163-frcrot-integration-evidence.json`. The original GUI PE is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The evidence is static and conditional; it does not establish native Qt storage, a successful function conversion, or original output equality.

## Source identities and validation

| Owned file | SHA-256 after edit |
| --- | --- |
| `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` | `df1135df9b0d4410d551281845fb7e7c599b89d058b47056203242552d1a8a31` |
| `source/apps/moproj2mst/tests/doe/test_keyword_reader_prefix.cpp` | `609a75a5308456642bc9661255e3c4fd370e08ea9dcfe3331126bc211c9f72d2` |

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_keyword_reader_prefix.cpp` returned `ok: true`, with no issues. Its check covers Testkit entry, include resolution and assertion macro shape. No compile, executable test, full suite, GUI session, live REA action, original process, or deployment was run by this owner. Root's grouped build and tests remain the next validation gate.

Ownership released: `keyword_reader_prefix.cpp`, `test_keyword_reader_prefix.cpp`, and this implementation receipt. The review follow-up changed only the test and receipt; production source and its TLOC diagnostic remain at the original implementation hash. No shared interface, CMake, plan, or state file was edited.
