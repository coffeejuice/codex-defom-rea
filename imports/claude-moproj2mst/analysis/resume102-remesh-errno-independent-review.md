# CP102 independent remesh finite-real errno review

No blocker was found. The frozen correction preserves entry errno only after an ordinary host conversion passes the existing finite/current-call ERANGE checks. It fixes integer-overflow→finite-scalar ordering without admitting another numeric profile or hiding a new range failure.

## Native evidence

Original EXE SHA-256 independently matched `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All four PE range hashes in `resume102-remesh-errno-order-contract.md` matched direct section-mapped bytes, including wrapper `14147583c..1414759b6` = `03e0e1ba872019abe31f370b68e4c6f2355da8cb31e69f8c2f7b1ff4b634ab79` and result-selection range `14147592a..14147599c` = `c713d3212435672fad4bcacb70e0135db4518c3fd22449b8a4f925a72a19cc68`.

The independently inspected wrapper has no entry errno reset. It selects ordinary finite payload at `141475980`; no-digits/invalid-result flags0x240 select zero and also bypass errno stores. Its explicit error stores belong to invalid arguments or qualified range handling, with range write0x22 at5978. The fltin wrapper returns flags/payload without an errno reset. This conclusion retains the existing normal KnownC/CRT/TLS profile; exceptional locale initialization, allocation and invalid-parameter paths are not generalized.

CP98 establishes integer overflow setting errno34 before method clamping. Following it with finite filter7.5 and scaling2.25 therefore continues with method5, those scalar values and the prior errno at the reader close boundary. CP101's successful prefix-zero branch likewise already bypasses the ordinary temporary clear. Public outer Close/API work can have separate effects; neither this review nor the new tests infer final C ABI errno from the local conversion proof.

## Frozen source and tests

| File beneath `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/engine/simcontrol300_remesh.cpp` | `39249f84faa6bef97e53504b7fb22f632a539d42cc7b5270a7bd3c553321814f` |
| New `tests/mdt/test_simcontrol300_remesh_errno_order.cpp` | `0188313ffdf40a6d5e8a7f3ab3ffa8559a3c8b5961f735d0e299ecfb48e92acc` |

Direct comparison with `build/wave3-checkpoint101-verified/source.zip` shows only saving entry errno immediately before temporary clear and restoring it after the unchanged admission check. Host strtod still starts at errno0, so a prior ERANGE cannot falsely reject finite success. A current-call ERANGE or nonfinite result returns unsupported before restoration, retaining the prior guard behavior. On accepted finite/no-digits zero, entry errno is restored unconditionally. Values, fields, reader loop, cursor, close order, profiles and public interfaces are unchanged; CP98 integer and CP99/100 range/CP101 prefix branches return before this fallback and are untouched.

Five Release-active Testkit cases cover entry0/EDOM/ERANGE across finite records; integer ERANGE followed by finite scalar stores; already-admitted no-digits zero; new unsupported overflow/underflow with previous EDOM; and actual ConversionSession::loadProjectRemesh dispatch. The range-failure case retains earlier method3 and the old filter value, leaves enabled assignment pending, closes once and sees the new ERANGE rather than restored EDOM. The Session case uses the same owned RemeshState and records errno immediately on return, before unrelated operations, while separately capturing it before fake close assertions. This is a Session dispatcher test, not a new disk/full-project Open errno claim.

The new test has supported CHECK/CHECK_EQ/REQUIRE macros, an explicit run_all entry point and resolvable includes. Its fixture is memory-backed and performs no filesystem writes. There are no permissive success alternatives or debug-only assertions. No older test amendment is needed to establish this delta.

The correction does not claim general CRT real-conversion recovery, FP-environment/temporary architecture equivalence, original TLS identity, physical fclose errno, editor Fill/palette behavior or complete Open/Save parity. Existing unsupported spelling/range behavior remains a candidate reconstruction boundary.

This report is the sole write. No source/test edits, build/tests, GUI, original execution or mutable REA occurred. Inspection used retained evidence, bounded offline DUMPBIN14.44.35228.0 views and direct immutable PE reads. Contract SHA-256: `e00d891bf18282ebbc13b97b1495bef26d1d3caf10bd3e81f7270710046c7852`. Root owns focused/full execution and final frozen source/executable binding.
