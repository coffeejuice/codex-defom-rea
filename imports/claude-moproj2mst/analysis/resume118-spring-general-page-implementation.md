# CP118: generic bounded Spring no-click page

Replaced count/value fixture whitelists in the internal page layer with a reusable typed-record profile. It admits1..256 finite flag0 records, keeps selected row0/no-click/no-edit controls, preserves ordered later payloads and the existing native writer formatting/failure order. The256 ceiling is explicit candidate resource admission, not an original format maximum. Public Session/palette/UI gates remain unchanged.

## Source changes

Owned `src/engine/simcontrol300_io.cpp` now validates every row's five doubles as finite and its flag as0, without constraining IDs/directions/scalars to CP115/116 fixtures. It keeps checked bounded count-to-writer conversion and rejects empty/over-cap typed lists. Fill checks combo membership only for selected row0. Unselected rows do not execute the original selection callback's lookup on the scoped no-click path, so foreign/repeated IDs are retained and serialized rather than repaired or rejected.

Apply still requires page checked, currentRow0, selected IDs equal the first payload, flagfalse and visible texts exactly equal the admitted first-row display. It copies the vector, parses only row0's visible fields into that local Save payload, leaves later payloads unchanged and runs the existing generic list writer. The first row thus passes display general6 ->parse ->writer general6; later rows pass directly from payload to writer general6. No item-click/edit/reselection producer was added.

The const projection API still materializes row0 writeback in the local Save snapshot. It is not a claim of mutated native item memory after Save or support for subsequent arbitrary UI events. Existing deferred combo scan after scalar reads, all other sidecars and writer main-first/flag0-companion-delete behavior are unchanged. Source comments explicitly state that excluded-profile preflight happens before sidecar writes and does not reproduce original partial effects for excluded inputs.

## Focused tests

Owned `tests/s/test_spring_page_io_roundtrip.cpp` grows from6 to9 cases while retaining the one/two-row native IO roundtrips and physical denied-primary-open cleanup case.

New generic3-row input has selected IDs17/19 and more than6 significant digits; unselected row1 has foreign signed IDs-7/111, and row2 repeats selected IDs. The test verifies selected controls, ordered payloads, exact literal writer bytes, preserved unselected field bits in the supplied projection, normalized3-row reread and idempotent Save. A trailing ASCII suffix is ignored according to the reader's count stop. No expected bytes are obtained by invoking the candidate writer. Input153-byte SHA231c71c9fcf6fe9b3e5fc63e07b6ccd9ba6d013414a1753a01b46815b09ae2f2; expected120-byte SHA7f2649ef0eedeedbc4c9fe876a795d7ea9d697f47efe8ab81fc948c44e439e7f.

A dynamic256-row case uses repeated ordinary records and independently constructs expected stream grammar from a literal row. It asserts exact full output and selected row0, then verifies a257-item typed page and257-count file remain unsupported without rewriting their current bytes. This exercises a count-generic algorithm rather than another literal admitted fixture.

An injected post-open primary device returns write failure, records its Close attempt and has no backing primary output. The wrapper delegates actual native companion deletion. The case requires Apply completion and ordered open-success ->Close ->SPRING2 removal, with an observed failed write and one Close. It deliberately makes no native partial-file/residue claim. The existing physical sharing-denial case remains separate evidence for failed open before truncation.

Existing guard expectations were reconciled: the missing-ID case now excludes row0's ID20 rather than unselected ID40; changed row1 finite values are no longer a fixture-only refusal, so that negative case now uses NaN. A count3 file containing only two rows remains a partial-read refusal, not a generic count3 ban. Nonzero flags, currentRow1, incoherent swapped selected data and edited visible text remain refused. Test diagnostics/short C:/PROBLEM/known-file-only cleanup behavior are unchanged.

## Evidence and limits

CP117 page contract SHAd4dea5c8f4c76eabf59df0355f23e3490559f0f1be4aef10b599267884246895 and peer parse contract SHAc4fce5d18aeae36f8fcdc972d4392857ed92b435f285035bf2fc985d0ef9e4f6 control this slice. Retained original item/callback/writer bodies establish count-driven payload order, itemClicked connection, explicit row0 load and same-row Save writeback. Original SHAfd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. Parser changes are owned separately by format_audit; this lane changes no input grammar.

No malformed/partial/allocation/native-Qt-codec/range case is silently admitted by page finite checks. Reader admission remains responsible for complete typed records. Present SPRING2 and nonzero flags remain outside this input path. Arbitrary edits/selection changes, native painting/item identity, runtime original acceptance and public full Session construction remain unclosed. The finite/cap/early refusal checks are candidate admission policy with the qualifications above, not new original validation behavior.

## Frozen hashes and preflight

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
|src/engine/simcontrol300_io.cpp|da5400ef4fdc262d8621f9ead3ad94ff84447aa9a686582a208a39bc9c8e8057|
|tests/s/test_spring_page_io_roundtrip.cpp|2cdb44515ad1ba37d26b97f50ea43328565c235dcac3dad382addcf44931dcf9|

Compared CP117 verified archive baseline sourcea15181d2976cb601063cee32e0b932d12fdeef9d48b0d0a1714f9bfbf59b83bd and test544b5589c597bb87f8f3d3d29e7d299725bd76ff2d8b1727adc1921653a2076b. The source diff replaces only Spring fixture predicates, selected membership admission and comments; deferred scalar/IO ordering and writer byte construction are untouched. All quoted includes resolve; Testkit main/run_all, supported CHECK/CHECK_EQ/REQUIRE, nine cases, balanced delimiters and whitespace were verified; no C assert/REQUIRE_EQ. No compiler/build/test executable was invoked.

Root must run `m2m_s_spring_page_io_roundtrip` / `m2m.s.spring_page_io_roundtrip` with the peer generic reader, existing native IO/page/lifecycle/order cases and source-stable full regression before acceptance. Only the two assigned files and this receipt were written. No shared header/CMake/reader, build/test, GUI, original execution or mutable REA change occurred. Source is frozen/released; header wording remains root-owned and no source ownership is held after release.
