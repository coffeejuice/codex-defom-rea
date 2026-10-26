# CP105 — candidate entrypoints and earliest application/main owner boundary

## Result

The earliest native owner boundary is application startup, before any project Open: the native application constructor initializes the global application-state object, then creates and publishes the canonical main window later in its startup sequence. The earliest common candidate code reached by both the CLI and DLL is `m2m::convert` → `core::run`, but that is a conversion request, not an application constructor. `run()` creates a stack `ConversionSession`; it has no application/main owner or normal-Close producer.

Therefore no current candidate entrypoint is a source-qualified point to construct and mark a **live native-equivalent** app/main owner. A future owner could be allocated in an explicitly chosen host/application lifetime and start with unknown facts, but `wmain`, the DLL module, `m2m_convert_*`, `run()`, and `ConversionSession` are not interchangeable lifetimes. Do not apply the native constructor stores at `run()` entry or Session construction.

## Candidate entrypoint trace

### CLI

`src/cli/main.cpp::wmain` sets Win32 error mode, parses command arguments, optionally calls the configuration setters, and calls `m2m_convert_w` once for the selected project. It does not create a `QApplication`, main window, main-window manager, or application state object. Process termination follows the C ABI return. This makes `wmain` the CLI process entry, not a shared library/host entry that can control later API calls.

### DLL / C ABI

`src/dll/abi.cpp::m2m_convert_w` and `m2m_convert_utf8` validate/decode request values and call `m2m::convert`; setters for codepage, GUI unit, and deform directory update separate adapter configuration globals. They do not create an owner or expose an app lifecycle callback. A DLL host can call conversion repeatedly; the ABI has no start/end-app or current-main handle. The existing C ABI provides no new owner parameter and must remain unchanged unless a separately versioned API is chosen.

### Common core

`core/convert.cpp::run` is the common conversion path. It serializes the process-wide engine with a function-static mutex, initializes conversion context, and constructs a local `ConversionSession` before Project setup. The mutex synchronizes engine work; it stores no application/main state and its lifetime is not the native app object. The Session constructor creates its own per-call `ModelManager` and `MainViewState`; its destructor releases these adapter owners and expressly does not replay native normal Close. `run()` has no Close entry/cursor after Save.

Consequently, placing an `ApplicationState` in `ConversionSession` would make its lifetime per conversion. Placing one as another local in `run()` would have the same problem. A new process-static object could span calls, but no source path would initialize it from native app startup, bind the same main-window allocation, or clear it at the native teardown boundary. A static empty holder is not yet useful live state.

## Native startup and owner order

Retained static evidence on the immutable original `DEF_GUI_MO_64.exe` (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) supports this selected normal startup path, conditional on reaching each call:

1. The retained native application entry/frame function begins at `0x140001410`; it reaches the application-global state initializer at `0x141ca0bb0`, which passes storage `0x143d766b8` to `0x1402c7ee0`. Constructor member setup precedes the recovered `0x1402c7f80` prefix. That prefix's qualified stores initialize process `+0x0c`, global main `+0`, byte `+0x18`, busy `+9`, and bytes `+0xA/+0xB` in source order. This is an initializer prefix, not proof of full application construction. The unresolved DBLIST `+0x1c` is not included.
2. The native application later creates the primary main window through constructor body `0x140010490`. The application frame's same stack `this` is forwarded into that constructor. QCoreApplication/base and other constructor effects precede publication.
3. The main-window constructor's predecessor chain reaches `0x140c99530`; at `0x140c9957b` it publishes that exact `this` through `0x1402c8070(0x143d766b8, this)`. Only afterward, at `0x140c99588`, it initializes `main+0x50` to null. The concrete ModelManager factory occurs later. Publishing a candidate Session/MainView pointer at the earlier app-state initialization point would not match this native order or identity.
4. Native Open, explicit Save, and Close use the same global state and main lineage. After `QApplication::exec` returns and main cleanup proceeds, the selected normal teardown frees main private storage and clears the global main pointer at `0x140c99245`. Project Close does not clear this application-main link.

For an eventual combined app/main owner, the storage shell may be created at the candidate's true host-application startup if one is introduced. The `+0x0c` and busy initializer facts should advance only after their corresponding constructor prefix. The main link should become known only at a candidate source point that corresponds to completed main construction/publication; manager-known must remain later still. The current converter has none of those producer points.

## Proven and missing

| Question | Proven | Missing |
|---|---|---|
| Native app-state storage | Global receiver identity and the selected initializer prefix/store order. | Full constructor completion and runtime callback effects. |
| Native canonical main | Same app object lineage, publish-before-main+0x50 order, and selected teardown clear. | Runtime occurrence and all main/window effects in candidate source. |
| DBLIST admission | Gate reads global `+0x1c`; helper body writes `1`. | Setter caller/`rcx` receiver proving same global; complete writes/reset lifecycle. Fresh image zero-fill is startup-only. |
| Candidate host scope | CLI and DLL converge on `m2m::convert`/`run`; C ABI configuration globals exist; engine calls serialize. | Application lifetime handle/startup/shutdown, canonical main allocation, shared manager owner, and repeated Open/Close orchestration. |
| Candidate Session | Actual per-call Session/Project/manager lifecycle and Save work. | Native main alias and normal Close producer. Destructor is adapter cleanup. |

No source-closed native leaf is currently callable from a candidate owner transition without first inventing the entry occurrence. The native initializer leaves and main publication assignment are exact, but their candidate prerequisites are absent. The next concrete dependency is to define/locate a genuine host-application root with a lifetime contract and a main-window construction/publication producer, then bind its exact identity through Open/Save/Close and teardown. If the intended profile is instead one fresh application per `run()`, that profile must be explicit at the public boundary and supply startup/teardown phases; current code does not declare that equivalence.

## Evidence receipts

| Artifact | SHA-256 |
|---|---|
| `analysis/resume101-close-live-owner-chain.md` | `8d893447af81cedd885b112609f7a7de709c1e2ab9b9f54a456746c4e2c7f793` |
| `analysis/resume101-close-pe-writer-chain.json` | `38ae40f0a8d56641940bfd3af1e2b31ab7ef43acb484f420a10b8f930534c9a1` |
| `analysis/resume103-application-state-close-design.md` | `75decaa6e8004e99031830ac3a1b8818100ae7c033887ad77023da9b2317c3a7` |
| `source/apps/moproj2mst/src/cli/main.cpp` | `b382032ec09bd7ed910a87e48f823924efc1ced57fda847db4d9cf2fc5476c5e` |
| `source/apps/moproj2mst/src/dll/abi.cpp` | `6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `46d4aaaba8128fc78d3d19b51158b319ec5959009eba48184194958e25a32e31` |

This is a static source and retained-evidence trace. No REA retry, GUI, original application execution, source edit, or build occurred.
