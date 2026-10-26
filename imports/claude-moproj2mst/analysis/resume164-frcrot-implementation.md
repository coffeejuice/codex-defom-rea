# CP164 FRCROT selector/text handoff implementation

Implemented the bounded Type2 reader increment described by `analysis/resume164-frcrot-file-next.md` and its independent plan review. The exact admitted FRCROT shape still appends registry ID 276 with empty operations and entity `[1]`, then reselects the original named owner and records function=true, enum=false, parameter=-1, and the three exact text values. From that retained entity, the invocation-local selector is formed as `[1,1]`; the persistent constructor record remains entity `[1]`.

The reader now terminates with `M2M_E_UNSUPPORTED` explicitly before converter call `0x14091111d`. It performs no file creation/write, scanner or parser projection, table helper, setter/data population, notifier, DYNARY update, sample continuation, or deferred-link completion. Admission remains before constructor mutation. The contextual A,B,A test checks the named A owner, value-only selector, diagnostic/RVA, B's reached header/description effects, preserved selection and samples, unresolved link, and absence of new I/O/notifications. The existing malformed-profile matrix already includes wrong/multiple entities, populated operations, and raw/comment/text child shapes.

## Files and SHA-256

- `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` — `2F16A73723BF400121E66D66E671A5FC258C480BBB920E5DFE4FC03D8DF8DA2E`
- `source/apps/moproj2mst/tests/doe/test_keyword_reader_prefix.cpp` — `AA31C4563E562E0557C12D104E8BCA2F93582D9AEE3446F31D0CFB205923A788`

Both test/source changes are owned by this increment. Test preflight: the file retains its existing Testkit `main()` entry point, includes the project check header and existing relative support include, and uses supported `CHECK`/`REQUIRE` macros (no C `assert`). No new include was added. Tests/build/full suite were not run, as directed. No shared plan/state/CMake files, GUI, original app, or live REA were touched. GUI-control release was not applicable because no GUI-control session was acquired; no release claim is made.
