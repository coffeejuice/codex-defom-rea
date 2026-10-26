# CP219 ObjProperties Open path review

## Finding

The negative-ID run produced three new files at the project root named `ObjProperties00001.KEY` through `ObjProperties00003.KEY`. Their lengths and SHA-256 values exactly match the same three `Operations/Task00001/ObjProperties...` and `Task00002/ObjProperties...` files already present in the before-image. This is consistent with the original Open parser replaying each embedded `<ObjPropertiesKey><Data>` blob through the normal `WriteBlob` route after the negative operation ID left its task-directory field empty.

The observed run is not a completed-Open witness. The retained process receipt records a 5,021 ms observation window, `endpoint.reason = terminate_command`, and cleanup termination; `exit_verified` was false before cleanup. The tree delta therefore proves that these root-level files appeared during the bounded process interval, but does not prove Open completed, establish which exact instruction wrote them, or settle whether the stale diagram references affected later processing. The control/variant comparison is likewise qualified as “consistent with positive-ID path” rather than causal proof.

## Original Open route from retained static evidence

1. `WalkProject` visits top-level elements and dispatches `Operation_List` to `14008cb50`. The operation list walks each `<Operation>` in document order and invokes `14008ccf0` (`ParseOperation`). See `source/docs/re/moproj2mst/s1-01/spec.md`, sections 3 and 5.1.
2. At `<Type>`, `ParseOperation` changes an ID only when it equals zero; a negative ID remains negative. The factory creates the operation, whose constructor calls `140050820(OPI,id)`. That setter always stores the ID, but only sets `OPI+0x130` to `Operations/Task%05d` and enters the task/project-directory setup when `id >= 1`. Negative IDs therefore leave the default task-directory string empty. See `spec.md` sections 5.1–5.3.
3. Parsing an operation's `<Objects>` calls `140093c30`. For each object slot it calls `140093ab0` on its `ObjPropertiesKey` child; that derives `ObjProperties%05d.KEY` using `14005eb10`, then calls `1400937b0` for each `Data` child. `WriteBlob` hex-decodes and decompresses the bytes, creates the absolute parent directory, opens the path for truncating write, and writes the payload. This path is independent of the `id >= 1` branch: it consumes the operation's already-created object/slot data and does not test the operation ID. See `spec.md` sections 5.3–5.4.
4. With an empty task-directory string, `taskDirAbs` is the project directory after separator insertion and path cleaning. Joining the numbered ObjProperties basename to that directory predicts a project-root path. This prediction matches the three root-level files in the CP219 after-manifest. It is a source-based path derivation, not a runtime call-site trace.

The candidate converter has the same relevant shape: `setOperationId` populates `taskDir` only when `!hasNoId(op)` (`hasNoId` is `op.id < 1`); `taskDirAbs` then resolves the project directory plus that empty relative path. `objPropertiesPath` appends the slot filename. `parseObjects` calls `parseObjPropertiesKey`; `storeBlob` decodes each blob and, in its native-operation-blob mode, performs the directory and file attempts at the derived path. In ordinary recording mode it instead stores the payload in the project overlay. These paths are in `source/apps/moproj2mst/src/model/operation_rescue.cpp`, `opinfo.cpp`, and `props_keys.cpp`.

## Relationship to operation IDs and diagram items

The CP219 input mutation changed only the two direct operation IDs from `1,2` to `-1,-2`. Its retained `effect-comparison.json` records the root-level ObjProperties files as the negative-ID-only additions and `Lubricants` as the control-only addition. The original B98 tree already had task folders and task-local copies; the root-level paths are additional outputs, not replacement locations.

The retained parser specification separates diagram parsing from operation-blob parsing. `WalkProject` records sub-project metadata; the main sub-project's children are parsed on activation. `ParseDiagramItem` reads `Operation_Id` and creates a diagram model item from the class and that ID. In B98 the diagram rows still reference positive IDs 1 and 2, so the negative-ID input creates a possible stale-link condition. The available static slice shows no call from diagram parsing to `ObjPropertiesKey`/`WriteBlob`, and it does not show an operation lookup or failure gate for those IDs. Therefore the stale diagram relationship may affect later activation/binding, but it does not explain or prevent the independently documented blob path. No conclusion about successful diagram binding follows from CP219's partial run.

## Evidence limits and next discriminating increment

CP219's retained evidence is the launch receipt, executed runner, process result, before/after manifests, and effect comparison in `analysis/cp219-launch-receipt.json` and `analysis/resume217-plain-run-2026-10-26T14-38-49.491Z/`. The process identity is pinned to the original GUI PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; GUI control was not acquired. The receipt also records different SHA-256 values for the reviewed and executed runner, so the executed file is not byte-identical to the reviewed file. Its guard was restored and the transient live copy was absent afterward, but the hash discrepancy limits the runner review's transfer to the executed artifact.

The smallest source-closed candidate increment is a focused model test for negative operation IDs with embedded object-property blobs and `nativeOperationBlobs` enabled. Assert that the effective ID remains negative, the recorded `ObjProperties` path is rooted at the project directory (slot names `00001`… independent of operation ID), and the written bytes equal the decoded `Data`; separately assert that the positive-ID directory collector contributes no five-directory group. This locks the two independent effects without enabling any new public directory-creation behavior. Existing staged-directory tests cover only directory decisions and intentionally leave `FileEffects` empty; they do not cover blob materialization for negative IDs.

Alternatively, an exact original runtime witness would need to bind `ParseObjPropertiesKey`/`WriteBlob` and path arguments to the same negative-ID operation occurrence, and separately record whether diagram activation/binding completed. A second uninstrumented short run would not supply that attribution. This review performs no such run.

## Sources

- `source/docs/re/moproj2mst/s1-01/spec.md`: `WalkProject`, `ParseOperation`, operation-ID/task-directory setup, object-property blob path and writer, and deferred sub-project diagram parsing.
- `source/apps/moproj2mst/src/model/operation_rescue.cpp`: candidate ID setup and `parseObjects` route.
- `source/apps/moproj2mst/src/model/opinfo.cpp`: `hasNoId`, `taskDirAbs`, and `objPropertiesPath`.
- `source/apps/moproj2mst/src/model/props_keys.cpp`: candidate blob decode, overlay/native write branches.
- `analysis/cp219-launch-receipt.json`, `analysis/cp219-executed-runner.txt`, `analysis/resume217-plain-run-2026-10-26T14-38-49.491Z/negative_ids/result.json`, and `effect-comparison.json`: process bounds, observed paths, and hashes.
- `analysis/resume217-operation-input-review.md` and `analysis/resume218-staged-open-dir-review.md`: earlier negative-ID and staged-directory qualifications. The former already records stale diagram IDs as a validity risk; the latter explicitly does not claim original runtime creation.

Read-only static and retained-evidence review. No source, candidate, fixture, mutable REA database, original process, or GUI state was changed or run.
