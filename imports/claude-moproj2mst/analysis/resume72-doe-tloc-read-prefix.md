# CP72 B121 TLOC model/read prefix implementation

## Scope and result

Added an isolated candidate-only lower-level reader/model projection for the single retained B121 `Transfer Time` / TLOC row. It parses the observed variable and keyword attributes, nominal text, nested `KwOperations` id, and reciprocal top-level `Operations` binding into a typed optional scalar payload on `Var`. The writer's existing nonempty-variable refusal, `readInputTable`'s public nested-variable refusal, Type 2 activation guards, and notification boundary are unchanged. The new helper does not emit `2signalSetOprHasDoe()` and does not claim full parsing or original parity.

One evidence precision correction: the retained B121 `NominalData/string` and `temp.KWD` text is exactly `TLOC` + **four spaces** + tab + `0.1` (12 code units before newline in `temp.KWD`), not five spaces. The helper/test preserve the actual four-space bytes.

## Frozen files and SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/mst/doe_model.hpp` | `1706ae4db922b1e6cda879f62494f21f996132c2470530abb90996c6d3a40429` |
| `source/apps/moproj2mst/include/m2m/doe_study.hpp` | `0afcbac9ef309bb5c40982e859d9178ebd0a65b03e24c7c9c0f0db4a06913035` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `6b00fc9c8595b09dd3a86f1240c7adf3618bf3eaf34fae0fdc6bbba84def2bfe` |
| `source/apps/moproj2mst/tests/doe/test_b121_tloc_model_prefix.cpp` | `c8552472721e3565986ab872fa5e3915c709bedf72bc483dfed7fecf35ef8503` |

The test packet is auto-discovered by `source/apps/moproj2mst/CMakeLists.txt`'s `tests/*` / `test_*.cpp` glob. Four active test cases cover exact scalar fields and both maps, `Model::varsForOperation` behavior, rejection of a mismatched top-level variable name before model mutation, and preservation of the public reader guard.

## Static preflight

- `git diff --check` on the four source/test files completed without whitespace diagnostics.
- Reviewed Testkit macros already used by this packet; cases use active `REQUIRE`/`CHECK` calls and have a nonzero case count.
- Confirmed the direct reader stores owned `Var` instances and the public parser is not wired to it.
- No build or test executable was run, per the static-preflight instruction. Therefore compile/runtime success remains unverified and these files are frozen for root integration, not acceptance.
- No GUI/app control or mutable REA action occurred.

The helper is deliberately limited to the exact observed B121 fields: id 1, dimension 3, sample-count attribute 3, TLOC/double/unit 0/parameter -1/nominal 0.1, nested operation id 1, and top-level variable binding id 1. It does not conflate XML `numSamples=3` with the 100 rows in the mode-0 PAR tables, implement variable serialization, touch `temp.KWD`, or enable B121 public Open/Save.
