# CP59 isolated receiver population projection freeze

The new helper projects actual Session-owned flow receiver constructor history into ordered weak registration entries. It is explicitly diagnostic and incomplete: completeNativePopulation() always returns false, and no SetOprHasDoeRegistrationList or qualification string is produced or installed. Public Population/B121/DBLIST/Report guards remain unchanged.

## Files and interface

Only three new source/test files and this report were created. Existing model, Session, notifier, witness registry, shared interfaces and CMake files were not edited. Root owns build integration.

The interface is projectFlowReceiverPopulationPrefix(ConversionSession&), returning FlowReceiverPopulationPrefix with entries(), owner(), completeNativePopulation(), and validateAgainst(ConversionSession&). Each entry stores constructor ordinal, NormalSub/Copy source, original registration site, diagnostic identity token and a weak SetOprHasDoeRegistration with bare method slotSetOprHasDoe.

The helper validates the same live Project, Session registry and notifier shared ownership, Project registry binding, and registry notifier owner. If Project.doeIo already exists, it must already observe the actual Session notifier; no lazy I/O/notifier construction occurs. The lease revalidates these identities when validateAgainst is called. Project identity is checked before dereferencing its saved raw identity. Prefix entries are snapshots of actual registry history, never Project.subs/current-subproject enumeration.

Entries preserve dead generations, inactive receivers, copy-source distinction and registration order. Later constructor events require another projection. No model context, backend pointer, Task, Session selection, flags, map keys or population is fabricated or changed. The result retains only weak registry/notifier/receiver references; holding it does not extend those owners' lifetimes.

## Evidence and qualifications

The source contract is analysis/resume59-doe-active-next.md plus the CP56 native registration audit and CP58 actual constructor binding. Native normal/copy sites are `140071166`/`140071a7f`. The 348 direct-site inventory is fixed direct-call evidence, not whole-program dynamic registration closure. The helper does not advance a production guard or imply native QObject completion.

One precise existing-interface limit remains: FlowReceiverWitnessRegistry::releaseAdapterScope marks all retained witnesses dead but leaves owner() and orderedWitnesses() observable. There is no scope-live accessor, so released scope and valid all-dead history cannot be distinguished. Root explicitly authorized preserving that diagnostic history without claiming released-registry refusal. The test documents observable dead state and incomplete status. Strict registry-scope admission needs a const scope-state accessor or equivalent root-owned interface extension before production use.

## Seven focused Testkit cases

- Pre-SUB0/pre-I/O history is empty and incomplete, with no notifier or factory effects.
- Actual SUB0, Type2 and later normal SUBs preserve construction order and model identity, including inactive models; all backend witnesses remain unavailable.
- SUB death stays in history; explicit isolated copy event and identity reuse retain distinct source sites/generations; prior snapshots stay unchanged.
- Foreign Session, unbound existing I/O, foreign notifier and released Project refuse without lazy owner creation.
- Escaped projection keeps no Session registry/notifier/receiver lifetime alive.
- Released registry exposes dead diagnostic history only, with no unreleased-scope assertion.
- Real SetOprHasDoe session binding remains without registrations; unsuppressed dispatch still resets flag90 then stops at Population before a receiver snapshot.

Copy/identity-reuse events in the dedicated fixture are explicit isolated registry events, not a new production copy-constructor binding. No original execution or acceptance comparison was performed.

## Static verification and root integration

All 13 quoted includes resolve against the current file/include roots. The test has seven unique TEST_CASE names, Testkit main, only supported CHECK/REQUIRE assertions, and no C assert. Testkit macro definitions were read. New files have final newlines and no trailing whitespace. These are static checks only; no compiler, build or executable test was run by this lane.

Suggested root test target: m2m_doe_flow_receiver_population_prefix; suggested CTest name: m2m.doe.flow_receiver_population_prefix. Add the new cpp to the same core linkage as flow_receiver_witness.cpp and link the test like test_flow_receiver_constructor_binding. Root must compile/run the focused test and grouped candidate checks before claiming validation.

Next dependency: complete source qualification of the chosen fresh constructor receiver population before any production registration install; then actual current backend ownership remains unavailable. Separately, strict released-registry admission needs the root-owned interface noted above. The first Report QObject/TaskInfo boundary is independent and remains guarded.

Freeze UTC: 2026-10-24T20:53:45.634092+00:00.

| New source/test | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/flow_receiver_population_prefix.hpp` | `da29d8fe925e5d5744d2750e639b9459eccb30a9eca133c426a68d24b1dfe369` |
| `source/apps/moproj2mst/src/doe/flow_receiver_population_prefix.cpp` | `f0865a8c8bca08806dbe2135fd62b0ffd24da0e1892fed9f2b86c805a996b9c7` |
| `source/apps/moproj2mst/tests/doe/test_flow_receiver_population_prefix.cpp` | `5a10847e9e01ae687a38e164d0f6603efa755491e8ecce03afa1901f726bfdf0` |

| Controlling retained evidence/current interface | SHA-256 |
| --- | --- |
| `analysis/resume59-doe-active-next.md` | `4a457e654b69b4d6ec91d170a227bfcd466e19a9117a8f75bf76c3087d4e7dcb` |
| `analysis/resume56-doe-receiver-population.md` | `69cc4fd68264c1328fb668bd80a5e8c9f7e94d46c947f2ef4b7c4d7bd0101232` |
| `analysis/resume58-doe-constructor-wiring.md` | `f52b7e6967fae16c99228e39a25f820a0330297cfe7720d5668c73b69d6d61c2` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.hpp` | `2b5f788308d2f41dc9dcaa70d8c5557d1a65a6931ee6563d5eb8f79683459152` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.cpp` | `53ca78012fb718c7818d334cec1138306387e9d3a6461c4dfbe9f66707decab1` |
| `source/apps/moproj2mst/src/doe/variable_notify.cpp` | `592ddf00afa584c67584ea01f2e3c8fd426b3e49e381cb5ec298b314637b6c83` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `09feedab6ca1ac321deee04eecf5ff51bd6c1b2302c396ee4de0a3af5acfcebe` |
