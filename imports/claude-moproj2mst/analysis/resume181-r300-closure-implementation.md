# CP181 R300 Express Save SimControl implementation handoff

The Express Save object-temperature loop now borrows SimControl only inside an immediately invoked synchronous lambda with explicit `void` return. The callback captures the model pointer by value, returns on null control before `objectCount()`, and neither stores nor returns any pointer. The heat-mode check remains before the one `opModel` lookup and its missing-model return. The argument remains `m->simCtl()`, preserving its geometry refusal before callback entry and therefore before the count read.

The loop still reads count once, visits `objectAt(i + 1)` in the original order, skips null objects and kinds 0/3, reads old upper/lower, takes absolute half-width, writes upper using SimControl +0x3d0, then writes lower using that same field. Reverse-substituting the former loop reconstructs the exact CP180 production SHA-256, verifying that the remaining production file is unchanged. No public header, C ABI, cache0/A behavior or cache1 admission changed.

The existing `heat_express_3d` copied-corpus case now requires a real nonnull eligible object and nonnull model step. It sets reversed finite endpoints 83/19 and environment temperature 211, then asserts actual recentered endpoints 179/243. The reversed inputs also exercise the absolute-width calculation. A second direct call temporarily moves the actual step out, restores it on normal and exceptional exits, and checks unchanged endpoint sentinels plus cached model, step and object identities. This is synthetic internal null-step coverage; it adds no public Save route or test seam. The existing CP180 TMAX checks remain unchanged.

Static preflight verified the existing Testkit `main`, supported REQUIRE/CHECK/CHECK_EQ macros, qualified `engine::at` and `engine::DB_STEP_DATA`, and standard includes for `std::int32_t` and `std::move`. No new quoted include was introduced. The fixture retains a hard eligible-kind requirement rather than silently passing a no-op. Its corpus eligibility and execution remain to be confirmed by root's focused test run.

| File | Before SHA-256 | After SHA-256 |
| --- | --- | --- |
| `source/apps/moproj2mst/src/engine/save_l3.cpp` | `7c89166e6d2fede2b110578c332ecb690e62d94d180ae6d27be72c30759684a1` | `1a7c6de08abcb97906518549dbdaf0dc1568d444c2167dc70eb69a2f1f1befe0` |
| `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` | `c24892241818ed286c8cda68136cdae53dafa2d9e3e2a179dae1bb0c4a4982cb` | `a9a151d214960df4bfeeb4153ce673651642c3553b3290dfe8cbba0086f8931b` |

Unchanged guard hashes: `source/apps/moproj2mst/include/moproj2mst.h` is `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`; `source/apps/moproj2mst/src/engine/simcontrol300.cpp` is `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`.

This worker performed no build, test execution, source freeze, GUI/app-control acquisition, original execution, live mutable REA operation, deployment or shared-state update. Root must build these exact hashes, run `m2m.mdt.engine_l3`, `m2m.s.save_current`, `m2m.mdt.save_l2`, `m2m.mdt.simcontrol300_stopping_cached`, and `m2m.g0.abi`, confirm the extended case executes, then run the complete source-stable suite and freeze. No compile or runtime pass is claimed here. The local callback narrows only this alias lifetime; aggregate parked-model safety and other raw aliases remain unresolved, and Stopping cache1 remains blocked. Rollback is the paired CP180 files above.

Ownership of both source/test files is released to root. No further edits will occur unless reassigned.
