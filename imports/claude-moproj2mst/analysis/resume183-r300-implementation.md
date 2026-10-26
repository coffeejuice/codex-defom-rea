# CP183 R300 base contact-pair step scope implementation

The base contact-pair builder now confines its nullable step pointer to an immediately invoked void callback. The existing single `opModel(p, op)` lookup stays outside the callback; `withOpStep` is deliberately not used because it would add another lookup. Only the scalar `ContactPair` vector leaves the callback.

## File binding

Hashes were read before editing either reserved file (SHA-256):

| File | Before | After |
| --- | --- | --- |
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `128d620cdf510e00b0fc8c77da9e061f8be7f599c4e29278626836e512f7ff72` | `eb702bd2e07ffacfdd7e49ab73488fcd1e17b77821ad5f4516dc40fcf20beedf` |
| `source/apps/moproj2mst/tests/s/test_save_current.cpp` | `53eb2307086402bd22f990f37a88158a80e143b900d47de3ae400cf609944d69` | `4f8342c0b14812ca908a843e24652c67e3d348659f37c411fc7c9674e627d5e5` |

No other production/test source was edited by this worker.

## Preserved behavior and test coverage

The null callback argument returns before effects. For a present step, `inter_obj.clear()` still precedes `objectCount()`. Both loops, null/kind-7 filtering, last-candidate assignment, kind-5 entry lookup, counters, dimension/property decisions and pair construction retain their original order. The callback is invoked directly in the same expression and is never stored. Step/object/entry pointers remain local; no geometry or Stopping guard was added.

`base_default_pairs_step_scope` requires the configured copied `46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj`, operation ID 2, the expected `SfOprHeatTransferExpress2D` class, two real loaded objects and a dimension-2 step. It sets private object IDs/kinds to make exactly one candidate and sets nonzero common friction/IHTC. It asserts both ordered pairs and all scalar fields, verifies a seeded list was cleared, and checks model/step/object identities. The synthetic object/property mutations are never saved and the private project is destroyed at case exit.

The null-step call uses a noncopyable RAII owner of the moved unique_ptr. The detached list stays populated, the returned vector is empty, and model/step/object identities and count are checked after restoration. A separate throw/catch probe exercises exceptional guard exit. While detached, the case invokes only the direct helper and inspects the detached list/owner; no Save, loader, reset, dispatch or object access occurs. FileEffects must retain no writes/deletes.

## Verification and integration status

Static preflight passed: one production model lookup, null-before-clear-before-count, direct synchronous invocation, no withOpStep double lookup, no added geometry guard, existing Testkit main, supported assertion macros and resolvable quoted includes. The internal include supplies DB_STEP_DATA/objects; the inter-object header resolves through defmodel's public include directory, as in the existing MDT L3 test. Existing CMake discovery registers this source as `m2m_s_save_current` / `m2m.s.save_current` linked to m2m_core and Testkit.

No build, runtime test, GUI session, REA database operation or shared-state mutation was performed by this worker. Root must build and actually run `m2m.s.save_current` against the copied corpus before claiming runtime closure; root also owns grouped integration, the complete source-stable suite and freeze. Files are released for integration.

These are direct helper mechanics with synthetic inputs, not public Save admission/dispatch or native equivalence. Source qualification remains the existing SfOprBase `vt+0x198` annotation and the reviewed reconstruction; original evidence identity is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No native-runtime observation, model-wide parked barrier, cache1 authority or same-occurrence reader-tail claim is added.
