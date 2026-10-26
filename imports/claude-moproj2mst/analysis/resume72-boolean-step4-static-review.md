# CP72: static review of Boolean Save step-4 dispatch

## Scope

Read-only review of the current `convert.cpp` Boolean properties dispatch, the existing `booleanSideFiles` helper and step-5 call, the updated `test_save_io.cpp` failure checks, and the recovered Save order. No source, test, build, fixture, GUI, or mutable REA state was changed or run.

## Review result

The native `writeFile && WRITE_SAVE` path is aligned with the recovered ordering for the ordinary valid Boolean operation: after project Save, the step-4 loop calls the existing Boolean side-file slot before attempting to open the MST. The `FileDevice` path writes each file directly and silently continues if one file cannot be opened. Thus a failed MST open does not suppress earlier Boolean output, as the revised two failure tests require. The existing step-5 call remains and causes the expected second Boolean side-file invocation on an MST generation path that reaches the Boolean writer.

I found one API-level qualification for the recording path and one malformed-owner boundary to retain explicitly; neither invalidates the ordinary native-path change.

## Findings

### 1. Recording mode captures the same Boolean path twice

When `withSave` is true but `writeFile` is false (the `convertToBytes` path), `nativeSave` is false and the step-4 callback uses the recording branch of `booleanSideFiles`, adding both files to `step4` and `ctx.pendingWrites`. The non-native MST writer also receives `&step5` because `withSave` is true; the Boolean writer invokes the same helper again and stages the same paths in `step5`. The final `SideBytes` construction iterates both effect sets and appends each entry without coalescing. Consequently, a valid Boolean Save through `convertToBytes(..., M2M_FLAG_WRITE_SAVE, ...)` can return two entries per Boolean path.

Native evidence supports two write attempts during File > Save, but `SideBytes` is documented as a collection of files written, and this static audit did not find a contract saying repeated same-path entries are intended. Treat this as an open return-shape issue: CP72's native filesystem assertions do not cover it. If `SideBytes` should represent final file state, a future check should assert one final entry per path with the last bytes; if it is an ordered attempt journal, document and test the duplicates. Do not silently remove the step-5 invocation, because the original writer calls the helper there too.

### 2. Owner and data guards are only equivalent for normally constructed project props

The step-4 dispatch checks `BooleanProps` and non-null `data`, then passes the `Operation` from `p.operations` to `booleanSideFiles`. The helper returns only if `op.project` or `op.props` is null; it does not check `BooleanProps::owner`. The native Boolean side-file routine checks the BooleanData owner's property and its owner item before deriving the task directory (`s1-04 §1.2`). In the parser/factory path `setOprProps` binds `Props::owner` to that operation, and the factory constructs BooleanData, so the normal candidate project path satisfies the intended owner relation. A manually assembled, detached, or inconsistent model could produce files when the original would return early or would use a different owner item. Keep the implementation's claim qualified to the ordinary bound operation/property state; do not generalize this patch to malformed owner graphs.

The new `boolean && boolean->data` check is a defensive omission guard. The recovered constructor gives BooleanData its default block and the regular parser owner-binding path installs both relationships; static evidence here does not establish native behavior for a Boolean properties object whose data pointer is null.

## Checks against requested concerns

- **Native path:** `ctx.nativeMstSideFiles = nativeSave` is set before the step-4 loop. For the native Save path it selects direct per-file opens in `booleanSideFiles`; open failure for BWZ returns from that individual emitter, then the INI emitter is still attempted. The later MST target open remains after step 4.
- **Recording path:** side effects are staged in `step4` and later staged again in `step5` for the same Boolean files, as described in finding 1. `convertToBytes` does not write them during the conversion; it returns recorded entries after MST serialization succeeds. Its collection order is step4 before step5.
- **Duplicate step-5 call:** retained and source-supported. Native Boolean `vt+0x280` calls `0x1411e1820` after `KFWRIT/Step.KEY` and before `EXECMD`; the property Save loop separately calls Boolean `vt+0xb8` before MST generation. The helper serializes BooleanData and operation dimension; the MST writer's preceding ICB changes do not mutate BooleanData in the inspected code. Therefore identical bytes on both calls are expected for normal unchanged inputs, though no fresh original run was made.
- **Slot/data prerequisites:** the registry installs `g_l5Slots.booleanSideFiles`; the step-4 candidate gates on dynamic `BooleanProps` plus non-null data. Normal `setOprProps` binds the owner. The helper still uses the supplied operation rather than checking/deriving from `Props::owner`.
- **Failure continuation:** each side-file open failure is silent and independent; the property loop then proceeds to L6/Express dispatch and the later MST step. The existing `save_failed_mst_open...` and `save_missing_mst_parent...` tests assert file contents only after the call returns `M2M_E_WRITE`, and do not claim a successful Save. The stream-failure test separately observes the existing step-5 side-file path.
- **Alias behavior:** for native Save, the earlier output-alias preflight checks both `Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI` under every BooleanProps operation before step 4. `checkOutputAlias(current)` after the direct helper call cannot inspect files the helper wrote directly, but the preflight covers those two targets for the normal task-directory derivation. For recording mode, staged `step4` and `step5` entries reach the later generic alias scan before returned side entries or deferred writes are applied.

## Source references

- Native Save order, operation-list traversal and per-properties `vt+0xb8`: `source/docs/re/moproj2mst/mdt/spec.md` §1.
- Native Boolean `vt+0x280` call and writer position; `0x1411e1820` owner guard/output semantics: `source/docs/re/moproj2mst/s1-04/spec.md` §1 / §1.2.
- Candidate step-4 call and native/recording selection: `source/apps/moproj2mst/src/core/convert.cpp`, lines 350–380 and 425–445.
- Direct-versus-recorded file behavior: `source/apps/moproj2mst/src/engine/boolean_editor.cpp`, `booleanSideFiles`.
- Independent step-5 call: `source/apps/moproj2mst/src/ops/boolean.cpp`, around lines 136–143.
- Owner binding: `source/apps/moproj2mst/src/model/opinfo.cpp::setOprProps` / `setPropsOwner`.
- Current failure assertions: `source/apps/moproj2mst/tests/s/test_save_io.cpp`, tests `save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects` and `save_missing_mst_parent_keeps_step4_boolean_files`.

## Evidence hashes at review time

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/core/convert.cpp` | `C1B946637405BE3D4991DA4DF2DE1CC83BC81AE0993D7C48367845BA90031F47` |
| `source/apps/moproj2mst/src/engine/boolean_editor.cpp` | `5AAB83CB71D5EEDC69AD1476C4E5FF2ADC566687655694E59E3C3D949FF9A44F` |
| `source/apps/moproj2mst/src/ops/boolean.cpp` | `4D881B968CA13EB9E78FB57114B4E062358F870E2A075F7C9CE7740CECB2AB8E` |
| `source/apps/moproj2mst/tests/s/test_save_io.cpp` | `49203AE0CF44811DC62F61551216E07FEA4058174F79F517E36DBCD3D21D4798` |
| `source/apps/moproj2mst/src/model/opinfo.cpp` | read-only owner-binding evidence; hash not captured for this review |
| `source/docs/re/moproj2mst/mdt/spec.md` | `607731629D7382770222FC223E5174FB14D82B9F7D7010C0126566CCEDBF7A4C` |
| `source/docs/re/moproj2mst/s1-04/spec.md` | `2E1F9F7B75F49B4E3BCEC811755C4C0666E217CCBA8864CF2128E58268B93790` |

Static-only review; no build/test execution and no original runtime comparison.
