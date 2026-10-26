# CP107 — startup-prefix integration review

## Decision

CP106 rules out constructing a native-equivalent application owner inside `run()` or `ConversionSession`: the original application-state object is initialized from the CRT global-initializer table before native `main`, before the pre-application helper, and before `SfApplication` constructs its `QApplication` base. Repeating an owner per conversion would be the wrong occurrence and lifetime.

A **strictly bounded application-state initializer prefix** can be represented without constructing `QApplication`. The original global initializer reaches the `ApplicationState` constructor before native `main`; its `QString` member uses QtCore shared-null storage, not QApplication. The six scalar/pointer initializer stores from CP101 can follow that QString member prerequisite. Candidate `qt::String` currently models null versus empty by value and does not perform the native shared-null reference acquire, so the initializer cannot yet be claimed as an exact source effect with the existing string field alone.

Thus the exact missing dependency for this bounded prefix is a source-owned representation of the QtCore shared-null `QString` acquire on the actual app-state field, including lifetime/refcount delta. It does not require a QApplication/event loop. A separate, larger dependency remains for any current-main/Close behavior: the candidate has no native-equivalent `SfApplication`/QApplication startup, main-window construction/publication, or manager factory. Startup-null cannot be used as a later Close current-main value.

## Earliest candidate point and scope

The current common CLI/DLL call path reaches `m2m::convert` and `core::run` only after process or module startup. `run()` takes the engine mutex, starts `GuiStartupUnitScope`, and creates a per-call `ConversionSession`; none is the original pre-main producer. The proper candidate hook for a startup-only projection would be a process/module static initializer that runs before `wmain` or before exported conversion calls, not a value constructed per run. For the CLI target this corresponds to executable CRT initialization. For the DLL it corresponds only to module-load initialization; the C ABI exposes no host application start/end callback, so do not claim the DLL module lifetime is automatically the host's native GUI lifetime.

If that startup hook is implemented, it may record only the reached global initializer prefix and must remain distinct from a live Close cursor:

- process-type `+0x0c = 0`, current-main `+0 = null`, byte `+0x18 = 0`, busy `+9 = 0`, and bytes `+0xA/+0xB = 1` after the source-ordered constructor prefix;
- warning counter remains unknown/untouched;
- DBLIST admission `+0x1c` remains unavailable because its setter caller/reset lifecycle is unbound;
- the main value represents startup-null history only. A command-time main read remains unavailable until a candidate main is actually constructed and published.

The static owner must be initialized exactly once per selected executable/module lifetime and shared by later Sessions; it must not be recreated or reset in `run()`. If the project intentionally treats each conversion API call as a fresh application, that equivalence needs a separate explicit public invocation/lifecycle contract. Current CLI/DLL code does not state it, and a run-local owner would still occur after CRT startup.

## What the prefix does not unlock

CP106 proves further native steps which the current converter does not implement: pre-application executable/environment processing at `0x14007fc450`; `SfApplication` construction at `0x1400021c0`, including `QApplication(int&, char**, 0x01040804)` at `0x1400021f1`; subsequent application message-handler/provider effects; then `SfMoMainWindow` construction and publication of the canonical main at `0x140c9957b`, with its manager initialization later. These are distinct phases and owners. No candidate `MainViewState`, `ModelManager`, configuration setter, or Session pointer can stand in for them.

Accordingly, a bounded startup initializer can improve initial global-state fidelity while public Close and DBLIST remain guarded. It cannot mark `BeforeMainPublication` complete, make startup null current for a later command, create the native ModelManager, or make `+0x1c` known. Any first Close implementation still needs current owner, live stack predicate, modal/nested Save callbacks, and the same app/main instance across Open/Save/Close.

## Safe next implementation slice

After root reserves source ownership, the smallest faithful source slice is a new app-global state implementation tied to CRT/module initialization, plus a QtCore shared-null lease for the one default `QString` field. Run a focused static/test preflight to establish one initialization per executable/module, source order, no QApplication assumption, and no repeated initialization across conversion calls. Keep the later native app/main phases and command reads unavailable. If no shared-null representation is added, stop before the six `ApplicationState` initializer stores rather than silently treating `qt::String()` as the native member constructor.

This is not an implementation authorization in this review; no source file was edited. Root must also decide whether module-load lifetime is the intended DLL profile before a production initializer is added. CP106 itself explicitly rejects run-local owner insertion and fixed literal resets without a produced startup root.

## Evidence

The CP106 initializer-array receipt resolves slot `0x141e0c950` (index 31) to `0x141ca0bb0`, whose constructor reaches app-state initializer `0x1402c7ee0`/`0x1402c7f80`, before native main `0x140001410`. Native `SfApplication` and `QApplication` follow later. The earlier CP104 receipt binds QtCore's default QString constructor at `0x67003030..0x67003046`, storing shared-null data identity `0x672f0b30` and incrementing its reference at `0x6700303a`; this is the missing QtCore member prerequisite. Candidate `qt::String` is value-only and does not model that acquire.

| Artifact | SHA-256 |
|---|---|
| Original `DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `analysis/resume106-close-producer-minimal-evidence.md` | `f69a3499b8d456c8be2dc6e48f80120e7be82e0c9d7708c1907a7acf0e43fed2` |
| `analysis/resume104-application-state-insertion.md` | `e76477e6f8a07774e2faf6306365f0e6bf9053784cf5a92e32b2d0adb363fce0` |
| `analysis/resume105-close-producer-trace.md` | `b38da3b7ca608c961adc252ffa9c2ecdaaadb0ccc22e50d4ca8bfef460058e9f` |
| `source/apps/moproj2mst/include/m2m/qt4.hpp` | `72ad25fd2ae6412c2c7043dd73a43f53398a9de9373a7834ad1faab2061e6261` |
| `source/apps/moproj2mst/src/cli/main.cpp` | `b382032ec09bd7ed910a87e48f823924efc1ced57fda847db4d9cf2fc5476c5e` |
| `source/apps/moproj2mst/src/dll/abi.cpp` | `6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632` |

Read-only source/retained-PE review. No source edits, builds, GUI, original execution, or mutable REA database operations were performed.
