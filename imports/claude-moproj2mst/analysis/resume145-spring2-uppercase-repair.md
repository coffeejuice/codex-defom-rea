# CP145 repair: restore the intended companion fixture

## Diagnosis

The focused CTest failure came from fixture state in `native_spring_reader_maps_present_readable_ascii_companion_flags`, not from the source-gated `1X` parser path. The test correctly admitted count-two `1X\r\n` and correctly refused the same bytes under count one and count three. After those rejection checks it restored the two-row `SPRING.INI` bytes but left `SPRING2.INI` as `1X\r\n`. The pre-existing following one-row fixture then called `readSprings` without replacing that companion, so the exact count-two-only uppercase gate correctly refused it with `SPRING2 suffix outside bounded malformed-prefix profile`.

## Repair

The test now restores its earlier lowercase companion fixture `1x\r\n` after the count-one/count-three negative cases and before the existing one-row check. This keeps the intended lower-case test setup and preserves the new uppercase scope: only count two with complete bytes exactly `1X\r\n` is admitted. No production source change was needed.

## Static preflight

Verified the regression's fixture state is restored before the next reader invocation, the exact production predicate remains `count == 2 && flagBytes == "1X\r\n"`, and both owned files contain no trailing whitespace. No build or CTest was run; root owns rerun and integration validation. No GUI, remesh, shared interface, or mutable REA state changed.

## Updated frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `35B209095822E0FCC4692AC9D7B8E484813311473790B451FD439106777C2536` (unchanged)
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `E7ED56CA27CD3D80ABAF4DBC65FBDC1CA0E3AB4C8A3EB397D00C4976C9E947F4`
