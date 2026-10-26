# CP247 Forming success-order implementation

The recorded-effects successful-add path now stores the selected slot ID and operation ID before reacquiring the model step, querying the manager path a second time and attempting the final equipment KEY. The original creation gate and its preceding model/control/object/slot lookups are unchanged. No geometry admission, additional `simCtl()`/object lookup, catch, rollback or new throw seam was added.

`beginEquipmentCreation` retains the existing name, first serialization, recorded manager add, movement-name copy and first manager-path lookup, returning only an ID and path value. `appendFinalEquipmentKey` retains the existing nonempty-path test and write expression. `createEquipment` wraps prefix, final append and ID return in the existing order, so `equipmentSync` remains unchanged. The first manager path is intentionally retained even though Forming uses the second lookup after its stores.

## Provenance and limits

Implemented the bounded contract in `analysis/resume246-forming-success-order-repair.md` and its PASS review, with the unresolved failure qualifications in `analysis/resume247-forming-failure-next-evidence.md` and its PASS review. These consume retained static original decompilation, not a fresh original execution. Original GUI identity: DEFORM v11.0.2, `evidence/gui_mo/DEF_GUI_MO_64.exe`, retained SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Rehashed `d0.c` and `d2.c` match the reviewed identities below.

Native manager add/copy failure, concrete `vt+0x208(-1)` results, the initial fixed temporary source's physical failure/lifetime effects, and final writer filesystem results remain unresolved. The converter still supplies non-null serialized bytes to `addEquipment`; this change does not make native `-1` behavior representable. The existing nonempty final-path gate remains inherited converter behavior. The existing equipment/absent-object/absent-slot branch conditions and `objEntryN` lazy lookup effects are untouched. No GUI control, original process or mutable REA session was acquired.

The final-state test cannot prove ID-before-serialization interruption prefixes. That order is established by direct inspection of the changed call sequence; no production observer or synthetic serializer bypass was introduced. This internal extraction does not close the wider public step/native alias boundary.

## Validation

- `python admin/build_candidate.py --label cp247-forming-order-focused --target m2m_mdt_engine_l3`: PASS; configure 1.781 s, build 8.328 s, total launcher 12.802 s. Receipt: `analysis/cp247-forming-order-focused-build.json`; logs under `build/logs/cp247-forming-order-focused-*`. No `--then-full` or full suite/freeze was run.
- Focused `ctest --test-dir build/candidate -R '^m2m[.]mdt[.]engine_l3$' --output-on-failure -V`: PASS, one registered test, eight cases, 0 failed; 0.12 s total. Retained log: `build/logs/cp247-forming-order-focused-ctest.log`.
- `python admin/check_new_cpp_bindings.py source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp`: PASS. Existing Testkit entrypoint and supported `CHECK_EQ`/`REQUIRE` assertions retained; includes resolve. No C `assert` added.
- `git diff --check`: PASS.

The extended `forming_without_equipment_creates_one` fixture runs actual `loadOp` on the copied corpus. It checks the selected PDIE slot and operation IDs, one added row with the expected smallest-free ID, name and path, exactly one matching row, and exactly two writes selected by equipment destination in vector order. The first KEY has the corpus's blank PRSNAM; the final KEY and live movement have the new name. Unrelated writes are permitted. CTest redirects TEMP/TMP/TMPDIR to its `M2M_TEST_SCRATCH` child; no missing-file probe or effect replay was added.

Build source fingerprint before/after: `1cc5ccfe0db5a8f918652e59a2a967de488843b520f51a9952713baaf8af0ec6` (3,563 files, stable). This focused result does not replace root's integration/full regression/freeze. Token measurements unavailable; no token/time savings claim.

## SHA-256 at release

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/engine_internal.hpp` | `adfffeeb620a9dc34f0a6a409b6df2be47c17f07032e98e1a90f7b9046c0b33b` |
| `source/apps/moproj2mst/src/engine/loader_l3.cpp` | `b5197a54a6126a13c733a0b2943821f81756976553deee79ed52a4734aa39bbf` |
| `source/apps/moproj2mst/src/engine/postload_views.cpp` | `3c70c7edf82d9c7f6aac8d7e5298d6463e36132cb386341f02d4a0ce6ad33a2b` |
| `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` | `8c8c9aebe5cbefda0179f367a8d2e31c98cfcb61447e8ebde56c3dcd3ab8850b` |
| `evidence/gui_mo/s1/mdt/L3/d0.c` | `6d3703619843fb9ae1760f91e6633fb982c35cdc39d9dd2d89bee7435477fbff` |
| `evidence/gui_mo/s1/mdt/L3/d2.c` | `cf3f99af220cb92ec9ce8a19328640113333df4c0194faac2b20b475ba2c6f57` |

Ownership of the four source/test files is released to root; implementation is held unchanged for independent review and integration.
