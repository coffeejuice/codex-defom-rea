# CP129: canonical cache, Type3 and SPRING2 static preflight

Three probable build issues were reported to root before the broad build: invalid Type3 TEST_CASE arguments, unavailable unqualified String aliases, and missing cache implementation linkage for standalone/p11 targets. The latest source read contains all three repairs. No additional probable compile/link blocker was found in the final SPRING2 cut or canonical cache include/member wiring. No compiler, build or test executable ran in this audit.

## Issues found and source recheck

1. The initial Type3 test used seven `TEST_CASE("descriptive string")` invocations. Testkit's actual macro declares `static void name()` and token-pastes `name##_registrar`; it requires identifier tokens. The current test uses seven unique identifier names. CHECK_BITS is supported by this Testkit and was not a defect.
2. The initial private Type3 header/source/test used unqualified String while including only common.hpp/qt4.hpp. Those headers define `m2m::qt::String`, not model.hpp's `m2m::String` alias. The current files consistently use `qt::String`, preserving a narrow include boundary rather than adding model.hpp solely for an alias. `qt::toInt`/`qt::toDouble` are already declared with the used default ok argument.
3. Adding `OperationInfoModelCache operationInfoModelCache_{*this}` to inline-constructed Operation introduces an out-of-line constructor dependency for every Operation consumer. Initially, ordinary core linkage included the cpp, but the CMake standalone branch and p11 textual-source list omitted it. Current CMake explicitly adds `src/model/operation_info_model_cache.cpp` to standalone sources, and p11 includes it under the existing `m2m`→`m2m_xc` macro. The helper now has its specific `unavailableItemModelCache` name, avoiding generic textual-source helper naming conflicts. Core's recursive source glob still provides its normal definition.

These repairs were made by root/source owners, not this read-only worker. The repaired source identities below supersede the preliminary reads, including the cache cpp's earlier `0f095c7d...` identity before its helper rename.

## Remaining include/access/target checks

- model.hpp includes the complete cache header before embedding the member. That header forward-declares Operation rather than including model.hpp, so the inspected include relation introduces no recursive complete-type requirement. OperationInfoIdentity is declared before its dependent cache; the cache constructor's guard source exists before it runs. Both accessors expose const cache references, and the constructor/member access used by tests is declared publicly as intended.
- The canonical cache test's fifteen cases use existing public Operation/default/init/identity APIs. No private Session lifecycle method or unsupported filesystem conversion is added. The header's noncopyable cache is compatible with Operation's already deleted copy/move operations.
- The SPRING2 invalid-binary witness changes existing native reader/test files only. It reuses the existing Rig, path conversion and public IO interfaces. No new header, external library or test target is required; source/test hashes match the final worker receipt.
- New Type3 scalar cpp and test are discovered by existing recursive core and packet test globs. The helper signatures match their definitions and test argument types. It introduces no new linker library or custom target rule.
- The standalone/p11 constructor dependency warrants focused linkage coverage in addition to ordinary core tests; an ordinary m2m_core-linked test alone cannot prove that separate route links. The repaired CMake branch supplies the definition and the repaired p11 list preserves its renamed namespace convention.

| Build target | CTest name | Cases |
| --- | --- | ---: |
| `m2m_mdt_operation_info_model_cache` | `m2m.mdt.operation_info_model_cache` |15 |
| `m2m_doe_type3_optimizer_scalar_prefix` | `m2m.doe.type3_optimizer_scalar_prefix` |7 |
| `m2m_mdt_simcontrol300_spring_reader` | `m2m.mdt.simcontrol300_spring_reader` |2 |

Standalone target coverage such as `m2m_p09_keywords` and textual target `m2m_p11_xcheck` checks the distinct new constructor-definition routes. Their ordinary full-suite registration remains unchanged.

## Static tool result and limitation

`admin/check_test_source_preflight.py` passed for all three C++ tests, both before and after repair. It resolves quoted includes and checks entrypoints/assertion names, but does not validate TEST_CASE argument syntax; its initial pass therefore did not rule out issue1. An additional read-only scan confirms that current arguments are unique valid identifiers. This illustrates the tool's boundary, not a passing compilation claim.

## Expected public-header invalidation

The model.hpp edit deliberately adds a complete member/constructor dependency to Operation and changes a widely included public model header. Broad core/test recompilation is expected. The last controlled CP123 dependency measurement found451 C++ edges on model.hpp in that earlier graph; this audit does not claim that exact count for the expanded CP129 graph. The new standalone source additions also legitimately create compile work. A large action count is not itself a compile failure, duplicate preflight work or evidence of configure overhead.

Do not suppress this rebuild or reuse objects from the prior Operation definition. The private feature-anchor boundary remains separate; it cannot prevent necessary invalidation from an actual public model shell change. This lane performs no new graph/timing benchmark and claims no time saving from identifying the defects.

## Latest source binding

All paths below are under `source/apps/moproj2mst`. Hashes bind the latest repaired reads; source owners/root own their final freeze and build binding.

| File | SHA-256 |
| --- | --- |
| `include/m2m/model.hpp` | `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d` |
| `include/m2m/engine/operation_info_model_cache.hpp` | `bd87dc12903ff0a5ca1fd9fd8c80a62f7899ea65680309655574aeabe2199510` |
| `src/model/operation_info_model_cache.cpp` | `d316e3dec25a4ed97a389654d2a8d9de6d69a36858b2a512483cf48ab3305a06` |
| `tests/mdt/test_operation_info_model_cache.cpp` | `8d718646279446a5e720cd35878091fe13f9afbceb336308838e9d9e985a5bd6` |
| `src/ops/type3_optimizer_scalar_state.hpp` | `4e102b761db5ac32f1c4d46d11b6e5dd2d988abb095b769111a2343a92b85b13` |
| `src/ops/type3_optimizer_scalar_state.cpp` | `1b38421ede88830349465d1e8a1dbc17d4034951d240094e8ff1d6ab1707d6e1` |
| `tests/doe/test_type3_optimizer_scalar_prefix.cpp` | `46fb358e8f5032b9ac81210ddc45601a49097f74a18fe360ac56249e534d129c` |
| `src/engine/simcontrol300_native_io.cpp` | `286be2d4f1a77dab737d315410f9dc012c695824cf5e55e59d9fbd1aabde6040` |
| `tests/mdt/test_simcontrol300_spring_reader.cpp` | `13d305842209cb7ad533b86cd5925e92bc0d7aaf4d9b811d270c90e203d29b7e` |
| `CMakeLists.txt` | `2d4e0d5ef2addce9011df45db49304e127d20ffd929ee7177ac96916e1d89cb5` |
| `tests/p11/test_xcheck.cpp` | `0da6511f62388a36cc9ddec08b352577e54ea7af9cac434af91dca538e8d02a5` |

Only this new report was written. No source/test/CMake/build/fixture edit, compiler/linker, CTest/full suite, GUI, original execution or mutable REA operation occurred. This audit does not replace compiled linkage, runtime/source-fidelity review, full fixture/binary binding or source-stable archive. Token usage was unavailable.
