# CP163 connector prototype implementation receipt

Implemented the retained sparse QObject/private-allocation prefix through the checkpoint immediately before QtCore RVA `0x131185`, the first shared QList load. The frame remains owned by the existing pending Report allocation node and carries qualified QtCore address-point RVAs, a dedicated sparse private-state object owned by `unique_ptr`, explicit prefix phase/write markers, and unavailable backlinks/completion fields. Its `privateIdentity()` is a nonowning pointer observation, so copies do not extend adapter ownership. The factory reuses this frame on reentry and refuses at the same boundary. It does not create TaskInfo, expose a task result, touch either map, or claim shared-container, TLS/thread, counter, font, or native cleanup effects.

The focused factory test now checks the frame fields, single retained allocation, stable pending-node and private-state identities checked after each reentry refusal, and adapter-scope release behavior. Test source inspection confirmed the CMake test glob includes `tests/mdt/test_*.cpp`, the test includes `testkit/check.hpp` and production headers available to the `m2m_core` test target, and it uses `CHECK`/`REQUIRE` rather than C `assert`.

No build, test execution, GUI interaction, or live REA operation was performed, as directed. Source-level verification was limited to checking updated call-site expectations and the registered test pattern. Normal non-null prefix admission is modeled. No injectable allocation-failure test seam was added; native null allocation, host allocation failure parity, and native unwind behavior remain unsupported and are not claimed. The local sparse frame is published only after its private-state allocation succeeds.

## SHA-256 receipts

- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`: `d708e5a3e204ea574fa775184669bb357c250b2395dfa0127bd15253717ab936`
- `source/apps/moproj2mst/src/doe/task_runtime.cpp`: `f573396d9d3a7c86ec4817efecbc375e8fbb6f325ceca4d04b272bf8ed137735`
- `source/apps/moproj2mst/src/doe/task_factory.cpp`: `87cd85e2fd08ac2550f89db01ba9092bf6302bf4166597d6781debe7c1a06520`
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`: `e6a5eb7d43d4e31b2e58caa497457f7597f5ab65ae27afbc134661df39059c14`

The implementation is released to the coordinating agent for integration. The owner must keep these files unchanged through the source-stable focused test and freeze checkpoint.



