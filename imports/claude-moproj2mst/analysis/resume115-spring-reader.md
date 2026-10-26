# CP115 bounded native SPRING reader

## Change

`NativeExtra3DIo::readSprings()` now admits one exact source-backed ASCII record only:

```text
1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n
```

It requires `SPRING2.INI` to be absent and assigns the source-qualified pre-zeroed companion flag `0`. The output vector is cleared before parsing, then receives the single `SpringRecord` in the recovered field order. It admits two exact spellings only: the bounded CRLF seed row and the exact tab/newline row produced by the source-qualified one-record Save writer. The second spelling is exercised by the frozen public component roundtrip fixture. Other positive counts, any other row bytes, and a present companion file remain explicit unsupported cases. Existing nonpositive-count and exact `x\r\n` behavior remain unchanged.

The new focused test checks the row values from both admitted spellings, absent companion and flag zero, and guards for count two, present companion, and truncated row. It is a native-reader test only; it does not enable `fillExtra3D` for nonempty rows or any public Save path.

## Integration notes

The existing `Extra3DIo::readSprings(std::vector<SpringRecord>&)` API already expresses this reader result, so no shared interface change was needed. The current header comment still describes all positive records as unsupported; root should update that documentation if integrating this increment. The new test source is not registered in CMake from this lane; root must add its target/test registration before build validation.

## Validation and freeze

Static checks only: `git diff --check` passed for the owned tracked source; the new test was manually inspected for bounded scratch-path use and no fixture mutation. No build/CTest was run, as root owns integration/build.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `C13FBF4944AF7B50AF5F6D119F163C6531F09C8BDC5060B30306D8CE2B29C0A8` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `3C5FC749FC350352F61A9D28BE866FE35A34F2B2591C6F703FE3C8D8852622CF` |

No public page/list or writer code was changed. Original runtime comparison remains pending.
