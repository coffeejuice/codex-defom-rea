# CP99 independent canonical real-overflow review

No blocker was found. The frozen change admits only the new source-qualified nonfinite class: after existing D/d-to-E normalization, a whole token matching `[+-]?[1-9][eE][+]?[0-9]+` with exponent strictly above5200. Other host nonfinite/range outcomes remain guarded.

## Original chain independently checked

Original EXE SHA-256 independently matched `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All seven PE range hashes published in `resume99-remesh-real-overflow-contract.md` matched direct section-mapped bytes, including lexer `141486ea4..14148779d` = `89b29985132ef432b85a6f949f1837b73ea2ab4b9b8baf14ba7c5f38aea1f4f5` and wrapper `14147583c..1414759c0` = `42baa7021035b346b0695f2dc5020381da562f042e63bcd0892979ff76bf188e`.

Retained token-dispatch code and independently decoded kind10 table entry bind the route to `140906d75`: exponent normalization, call `1414759b8`, then store XMM0 to the destination. Bounded offline DUMPBIN inspection confirms the latter wrapper jumps to `14147583c`, whose call at5916 enters `141486ccc` and lexer `141486ea4`.

For this one nonzero integral digit, normalization offset remains zero: one digit is stored, the fractional path is absent, and trailing-significand-zero adjustment is absent. Optional initial sign and exponent plus follow the retained states; leading exponent zeros are skipped. At `141487294..72ae` a positive exponent above0x1450 is capped to0x1451. The strict comparison at7359/735f reaches7724, constructs extended infinity and returns lexer flag2. This bypasses the power-table branch.

The fltin layer still calls its extended-to-double converter, then independently maps lexer bit2 to flag0x80 at `141486d42..6d52`. The outer strtod wrapper tests flag0x81 at `141475940`, selects HUGE_VAL bits `7ff0000000000000`, applies leading-minus sign mask `8000000000000000`, and writes errno0x22 through `141474134` at5973/5978. Its overflow result therefore does not depend on a host conversion result. Endptr progress is unused by the reader after complete token selection. These are static original-code conclusions, not original-process observations.

## Frozen source/test comparison

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp` | `a3f361099a8d0686825b191e8516220505f16c3228c1044e5cd4becdde970779` |
| New `tests/mdt/test_simcontrol300_remesh_real_overflow.cpp` beneath that app | `ebf581ebb0f60e4d18488b7dbb39140c0107bb9f6f9952883666697381f87f90` |
| Root-amended `tests/mdt/test_simcontrol300_remesh.cpp` beneath that app | `9e5fbd0251eb5dcd8c99761603b7bc006d62a72759d86c605c9f134b9e0ae5e2` |

Direct comparison with `build/wave3-checkpoint98-verified/source.zip` shows the production delta is only the private classifier and its call after D/d normalization, before the existing host strtod path. CP98 integer saturation remains unchanged. The classifier requires one nonzero mantissa digit immediately followed by e/E, an optional exponent plus, at least one exponent digit, and no trailing characters. It scans remaining digits even after saturation, so `1e9999x` cannot slip through. Multiplication occurs only while exponent<=5200; its largest temporary is52009, safely within unsigned range, then capped to5201. All indexing is bounded under the existing chunk/token profile.

A match sets ERANGE and returns signed infinity directly. A miss retains the old hex guard, errno reset, finite/range checks and ordinary conversion; no general host-overflow admission was added. Unknown locale, ASCII/token-size/path restrictions, streaming ownership, real underflow, INF/NaN spellings, fractional/multidigit overflow mantissas, trailing junk and exponent5200 range profiles remain guarded. Finite decimal inputs continue through the existing parser. Root's old-test amendment is exactly `1e9999` to still-refused `1.5e9999`.

## Test and continuation assessment

The five new Testkit cases use supported release-active assertions and an explicit run_all main; includes resolve as in the CP98 test. They check exact signed IEEE infinity bits and errno0x22 at the memory-file close boundary, optional signs/plus, representative mantissas including range endpoints1/9, D/d normalization, leading exponent zeros and long exponent strings. They require later integer/boolean records and enabled assignment, plus scaling clamps10 for positive infinity and1 for negative infinity. Guard cases preserve earlier method3 while refusing later unsupported real forms. Finite cases exercise the unchanged fallback.

The disk case reaches actual `openProject -> ConversionSession::loadProjectRemesh` using a disposable minimal project/fallback file beneath M2M_TEST_SCRATCH. It checks openedOk, unchanged Session owner identity, signed infinity, enabled and later method4 before editor creation. This is public C++ Open continuation, not solely helper coverage. There is no optional success branch, fixture-output bypass or corpus mutation.

The increment does not reproduce the legacy extended temporary/converter architecture or claim floating-environment flags, original CRT TLS identity, physical fclose errno, C ABI errno visibility, arbitrary real overflow/underflow support, editor palette/Fill parity, or complete Open/Save equivalence. Candidate errno is observed before fake close effects; native status/cursor internals remain source qualifications.

This report is the sole write. No build, tests, source edits, GUI, original execution or mutable REA occurred. Offline DUMPBIN14.44.35228.0 was used only with bounded `/disasm:bytes /range` views of the immutable imported EXE. Evidence contract SHA-256 is `0b8e94a08ef219bc7a2e9571f4d1c7ce22eb2bdc5e6da1f14f6f39eb0848a9cb`. Root retains focused/full validation and final frozen source/executable binding.
