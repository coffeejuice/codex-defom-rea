# Resume 47 DOE preference loader: missing-key carry-forward

## Source-qualified behavior

The immutable target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The bounded disassembly is `analysis/resume46-doe-preferences.asm`; it covers the preference-loading routine beginning at `0x1414091b0`. Its key strings are qualified by `analysis/wave2-doe-active-task-settings-literals.json`, and its helper exports are recorded in `analysis/wave2-doe-session-task-preference-getters-receipt.json`.

The call sequence forms settings keys and calls the native QSettings `contains` wrapper (`0x140808860`) with an address inside the task/prototype object as the destination. That wrapper first calls `contains`; its `value` and destination store are inside the `contains == true` block. If the key is absent, it returns false without writing through the destination pointer. The analogous bool getter at `0x140808270` follows the same structure: `contains` gates `value().toBool()` and the output assignment. The int getter at `0x1408085c0` likewise gates `value().toInt()` and its assignment. Therefore missing keys carry forward the field's entry value; they do not replace it with an implicit zero, false, or empty string.

The observed key set includes `/exeFileName`, `/arguments`, `/exeFileEnabled`, `/exeFileName2`, `/arguments2`, `/exeFileEnabled2`, `/workingDir`, `/inputFileNo`, and indexed `/inputFile/%1/desc` and `/inputFile/%1/filename`. The 17 prototype constructors and this loader disassembly establish a static ordering/field-write contract only. No runtime settings registry capture establishes actual user-specific values, and the current native factory stops at Report QObject before the settings loader, TaskInfo, and cache registration. Do not move or relax that guard based on this local contract.

## Small helper-test design

Add one focused case to the existing `tests/mdt/test_doe_task_factory.cpp` target (already registered through the `tests/*/test_*.cpp` CMake glob and labeled `m2m-mdt`, `mdt-l6`, `m2m-mdt-l7`). The case should exercise a proposed isolated settings-lookup helper with a deterministic in-memory map, without invoking `TaskFactoryRuntime::createTaskBase`, native QObject construction, or TaskInfo initialization.

Suggested contract and assertions:

1. Seed a destination string/bool/int with nondefault values representing the caller's existing prototype fields.
2. For each getter kind, ask for a key absent from the map. Assert the helper reports `present == false` and leaves the destination byte/value-for-value unchanged.
3. Add the same key and value to the map. Assert `present == true` and only then the corresponding typed destination changes.
4. Include an explicitly present false/zero/empty value. It must overwrite the seeded value, distinguishing “present with default value” from “missing”.
5. Keep key order observable in the fixture or invoke the helper in the recovered order for at least the first `/exeFileName`, `/arguments`, `/exeFileEnabled` group. Do not assign meanings to opaque object offsets beyond those established by the disassembly.

The helper should be a narrow representation of the accessor semantics (`contains` gates assignment), not a simulated full preference loader. Test names and comments should say it is a source-qualified helper contract, not proof that the original constructor ran or that preferences are wired into the active lifecycle. The current source has no implemented native preference-loader call path to test end-to-end; adding such a call or changing the public unsupported guard is outside this proposal.

## Limitations

The getter disassembly establishes missing-key non-write behavior and typed conversion on present keys. It does not establish QSettings backend selection, registry contents, default preference values, complete TaskInfo field ownership, or whole factory constructor completion. This document is design-only: no production source, test, build, app, or REA state was changed.
