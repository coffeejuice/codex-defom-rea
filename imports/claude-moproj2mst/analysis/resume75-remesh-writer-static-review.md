# Private remesh writer static review

No blocking divergence was found in the released seam's qualified ordinary regular-file contract. The default path retains the same native file owner/formatter, all31 format calls and arguments in order, ignored post-open write/close results, and the prior disabled/locale/failed-open guards. The new fake tests continuation after a returned write/close error, not physical failure residue or full public Save acceptance.

Only this review report was written. No source, build, executable test, GUI, original execution or mutable REA changed.

## Source and original contract

Reviewed the three frozen files listed in `resume75-remesh-writer-seam.md` and compared their current hashes to that release. They match. Compared the remesh implementation/public header against `build/wave3-checkpoint74-verified/source.zip` without extracting or changing it.

The retained original qualification is `wave3-remesh-save-lifecycle-contract.json`: enabled writer `0x1400abcc0` opens `LOCAL_REMESH.DAT` with `wt`, issues31 ordered calls to original fprintf `0x14146da0c`, then fclose `0x14146fb28`, without checking write/close returns. Failed open reaches the first null-FILE fprintf/invalid-parameter path rather than an ordinary ignored false result. `wave3-remesh-invalid-parameter-startup-contract.json` separately qualifies the startup default handler; candidate terminal-outcome representation remains guarded. Disabled Save uses immediate QFile removal with ignored result. These are static original contracts, not a fresh physical-fault experiment.

In the released implementation:

- `simcontrol300_remesh.cpp:276-295` remains the public entry and constructs a per-call native factory. Its `NativeSaveFile` owns the existing `CoggingFile`, whose ordinary narrow path still reaches the same CreateFileA/text/4KiB-buffer/fclose implementation.
- `:284-286` forwards each `writeV` to `def::rt::crt::vfprintf`. The predecessor's `def::rt::crt::fprintf` reaches that same vfprintf through its ordinary va_start/call/va_end wrapper (`source/port/defrt/src/stdlib/legacy_printf.cpp:966-979`). The replacement variadic lambda at `:328-335` supplies equivalent argument forwarding and does not preformat through a different formatter.
- `:300-317` retains the prior ordinary directory profile, disabled immediate DeleteFileW branch and KnownC/current-C-locale gate before opening. The disabled branch does not invoke the factory. Unrecovered locale returns UnsupportedSnapshot before opening.
- `:318-325` records actual successful/failed open. Failed open remains `UnsupportedSnapshot` with `invalidParameterReached=true`; it is not newly reported Completed and the candidate does not execute host null-FILE behavior.
- `:337-366` retains all31 source format/argument calls. A line-level comparison after removing only `fprintf(f,` versus `write(` prefixes found **31 predecessor calls,31 new calls, identical argument text and order**. Constant comments, blank lines, field ordering, `%lf` formatting and absence of DONE are preserved.
- `:367` ignores the returned close status. Every write result is likewise unused; a returned failure cannot terminate the sequence or change the default Completed report. Exceptions are distinct from returned errors and are not newly caught/ignored.

The public `include/m2m/engine/simcontrol300_remesh.hpp` matches the CP74 predecessor after newline normalization. No converter or outer Save interface is part of this release; the seam is source-private and per call, with no global/environment/public fault setting.

## Lifetime and default-route limits

The factory is borrowed only during `saveRemeshAtProjectSaveWithFactory`. The returned unique device lives through all format attempts and explicit close, then is destroyed before returning to its caller. Each va_list is started and ended around one synchronous writeV call; the native adapter does not retain it. CoggingFile's idempotent close means its destructor does not repeat a completed explicit fclose. No device, callback or factory enters RemeshState or RemeshSaveReport.

The default path is **not allocation-identical** to CP74: the formerly stack CoggingFile is now a member of heap-allocated NativeSaveFile and the public wrapper creates a std::function factory. This introduces adapter allocation/resource-exhaustion possibilities before native open; it does not change the reviewed ordinary path, and the retained CoggingFile profile already excludes allocation/FILE/descriptor-exhaustion fidelity. No allocation-failure equivalence is claimed. A future requirement for an allocation-neutral default route would need a separate adjustment; it is not established by this release's test.

The native adapter returns0 from its private close method because CoggingFile intentionally discards fclose's result. Since the caller ignores it, this does not change the default native decision. It also means the fake's failed close is a declared dependency result, not a measurement of a persistent native fclose failure.

The private helper currently treats a null factory result like failed open (`:319-325`). Its supplied public factory returns an object or throws, and the new fake returns an object with opened true, so no reviewed public/test execution relies on null meaning a physical fopen outcome. An empty std::function or throwing factory is likewise private configuration/exception behavior, outside the returned-error continuation being tested.

## Test scope and evidence strength

`tests/mdt/test_simcontrol300_remesh.cpp:503-562` adds one Release-active Testkit case. Its fake reports successful open, returns-1 at write ordinal6, then allows later calls, reports failed close and has no backing file. Assertions require exactly31 write events followed by close, success immediately after the injected failed ordinal, Completed/opened true and no invalid-parameter marker. The requested path is checked. The fake path is only lexically validated by NativeIo construction; no actual path/file probe occurs through this fake.

This directly tests that the helper does not branch on returned write/close values and does not accidentally enter the failed-open guard. The fake deliberately ignores format and va_list, so its self-generated ordinal labels do **not independently verify format identity or payload order**. Payload/order coverage remains the existing default native byte test at line412, current-state/legacy numeric test at line426 and overwritten-byte test at line452, plus the source comparison above. The release report's phrase “31 ordered format attempts” should be read as attempted-call sequence/count, not a new independent formatted-byte oracle.

The separate existing failed-open and disabled-removal tests at lines487 and469 remain necessary: the new successful-open fake is not evidence for them. It also cannot prove a real filesystem's partial bytes, flush/fclose state, source-original fault output or outer public continuation. No such outcome is asserted in the test.

No compile/runtime success is claimed by this review. Root should retain the released pre-existing native byte tests and run the focused remesh target with grouped/full source-stable validation before freezing acceptance. No source correction is required for the reviewed scoped contract.

## Reviewed frozen hashes

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/engine/simcontrol300_remesh.cpp` | `5ed9068356712acd03d8643d6006f921e32e69e4d07c7719b8ba726911e0a1bf` |
| `src/engine/simcontrol300_remesh_internal.hpp` | `4b4d3df42673ad02e587a7bc52f03b41c83486369105cb820208bfcf25981936` |
| `tests/mdt/test_simcontrol300_remesh.cpp` | `24c82860089c0e2ddc49a12652ca362560eb6ca4814bfe6d90a0e69d988be769` |

Reviewed release report SHA-256: `433627af703728cca951840ebcfa1607d7898738111cc54d49b7d2f5a530487b`. Token measurements are unavailable.
