# CP73 B121 TLOC writer static review

## Finding

The isolated writer is tightly admitted to the retained B121 one-record profile. It checks study header defaults, empty regions/constraints, a single owned scalar variable, exact Transfer Time/TLOC metadata, nominal text, and both distinct operation maps before adding one `FileEffects` write. It keeps the public reader/writer refusal guards separate. Its manually composed UTF-8/CRLF XML and fixed attribute order are explicitly a fixture candidate, not general Qt serialization behavior. The `FileEffects` return means planning succeeded; it does not establish QFile open/write/close behavior. These boundaries are sound for an isolated candidate seam.

A material test/report mismatch remains: `tests/doe/test_b121_tloc_model_prefix.cpp` does not compare against the retained 707-byte fixture. `kInput` is a second in-source XML literal. The test reads that literal into the model and then compares output with `toCrLf(kInput)`, so the byte assertion is self-referential; it proves consistency with its own duplicate, not equality with the retained original. The retained artifact is `evidence/gui_mo/oracle_b/121_B_DOE_VAR/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`, exactly 707 bytes, SHA-256 `18EC0C482730C82ADE12E91771CA67E53E4604573032624B99F3F4829A86B069`. The writer-evidence note calls the assertion an exact retained-schema regression, which is not established by this test as written. The fixture should be the source for expected bytes (or a checked-in immutable byte fixture/hash with a verified relationship to that artifact), with no host-path probe; subtree instructions require missing-file probes to remain under `M2M_TEST_SCRATCH`.

## Scope review

Static inspection found no evident unsupported field generalization in the implementation: the writer rejects nonfixture common headers, nonempty regions/constraints, variable geometry payloads/link, any different variable/keyword fields, and mismatched operation maps before appending its one write. Some internal-only fields such as `index`/`geoType` are not checked, but are not serialized by this fixed layout and are not part of the exposed fixture contract. The writer emits literal `nominalValue="0.1"`; the exact-field gate makes that bounded rather than a generic numeric formatter.

The implementation intentionally does not model native I/O failure order or refresh, does not emit temp.KWD, and is not bound into public `writeInputTable` / `saveInputTable`. No public DOE support claim follows. No build or test was run in this read-only review.

## Recommendation

Before treating the byte check as evidence for this implementation, replace the duplicated-literal expectation with expected bytes sourced from the verified 707-byte retained artifact (or a repository fixture whose content/hash is checked against it). Keep the writer/helper scope and public guards unchanged. No source edits were made.

## Addendum: fixture-backed byte assertion verification

Root updated the test after this review. Read-only inspection confirms `retainedInput()` opens the configured `M2M_ORACLE_B` path in binary mode at `121_B_DOE_VAR/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`; the writer test parses that actual input, independently reads the retained file as `expected`, checks its size is 707, and compares the planned write bytes directly to it without newline normalization. The retained source file currently measures 707 bytes and has SHA-256 `18EC0C482730C82ADE12E91771CA67E53E4604573032624B99F3F4829A86B069`, matching the previously recorded receipt. The helper writes only to the in-memory `FileEffects`; the test contains no fixture write operation. This resolves the self-referential golden issue. No build/test was run, so runtime pass remains unverified.
