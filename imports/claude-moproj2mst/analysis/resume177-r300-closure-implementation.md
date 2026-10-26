# CP177 R300 HeatTreat owner SimControl closure implementation

Owner: `/root/r300_heat_closure_impl`. Status: implementation and static preflight complete; files released to root for source-bound integration. Scope is the imported converter only.

## Changed files and hashes

| File | Pre-cut SHA-256 | Released SHA-256 |
|---|---|---|
| `source/apps/moproj2mst/src/model/props_heattreat_slots.cpp` | `8dea903f2b4ba68f055d4965891a643fdfbeb31af7cfb51db174dcdf6e88e20a` | `295c055e5d908214e8685904145368df511bdb9cacfb172a05c49b769f57f531` |
| `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` | `5b69d84666a508581a307cc28a28edbdc55f3f619b1d6ad9a2a1d305f0c24bad` | `ecc675d7711b40370efc170e8a38b76a7bb46c4a7375918791e615fa5cf0ff3e` |

The exact plan-owned production file is `src/model/props_heattreat_slots.cpp`, rather than a file under `src/engine`. No headers, C ABI, engine layout, CP163 source, cache publication, deployment, or shared workflow state were edited.

## Closure and order review

`withOwnerSimCtl` returns void and admits only callbacks with an exact void result. Missing owner/project invokes the callback once with null and does no model lookup. The other branch performs exactly one `engine::opModel` lookup and calls `m->simCtl()` before invoking the callback. Thus the preexisting `GuiModel::requireGeometrySyncAvailable()` refusal remains, including its order before any callback effect; `withOpStep` is not used.

Four immediate inline void callbacks replace the old raw-return consumers. Mode callbacks capture only offset/flag values. STEPDEF callbacks capture the existing property block by reference; they neither retain nor return the SimControl pointer and perform only the conditional field store. The helper neither stores nor queues the callback. Static source review found no raw-return/capture escape in this seam. This is a local constraint, not a whole-program borrow proof.

Mode setter still applies optional engine defaults, converts/stores the property flag, performs the control write, then resolves ownerProject again for propsRefresh. Getter still writes control before constructing the returned Variant. STEPDEF setter stores converted b.stpdef before lookup and preserves zero -> 1 / nonzero -> 3 mapping; getter writes control before assigning its integer Variant. No allocation or changed null fallback was introduced.

## Focused coverage and static preflight

Added `heattreat_property_simcontrol_writes_and_null_step` to the existing Testkit executable. It loads a corpus-backed reclassified HeatTreat owner; checks Deformation setter/getter property flags, Variant kind/value and control word; checks STEPDEF zero/nonzero setter mapping and getter write; then temporarily moves the actual model step away and checks the Mode getter plus STEPDEF setter/getter follow the null-control path without changing saved control sentinels. Normal and exceptional exits restore the step. This is a copied corpus/internal-slot witness, not original or public Open evidence. Existing HeatTreat and CP176 Distortion cases remain intact.

Static checks passed: removed_raw_helper, four_inline_callback_sites, four_explicit_void_lambdas, one_lookup_in_helper, geometry_gate_retained, no_withOpStep_substitution, exact_void_constraint. Every quoted include in the two owned files resolves from its file directory or the target's public include path. The new `m2m/props_variant.hpp` include exists under the target's exported include directory. Existing Testkit main is retained; only CHECK, CHECK_EQ and REQUIRE are used, with no C assert or unsupported REQUIRE_EQ. CMake's existing mdt test discovery links m2m_core and testkit.

## Pending integration and limits

No build, CTest, full suite, freeze, GUI control, original execution, live mutable REA, or deployment was run by this worker. This report does not claim tests pass. Root should run the plan's focused group (`m2m.mdt.l6_slots`, `m2m.p15.heattreat_tree`, `m2m.mdt.save_l6`, `m2m.mdt.simcontrol300_stopping_cached`, `m2m.g0.abi`), then source-stable full suite and freeze. Existing CP163 cache0/A/after-A refusal must remain; no cache1 publication is authorized. The public GuiModel step/simCtl seam and other raw helpers remain outside this cut.

Rollback is limited to the two files and the pre-cut hashes above. No reconstructed DEF_PRE or DMO executable was produced or deployed. Original qualification remains the next-cut plan's DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; no new original behavior is inferred from this implementation.
