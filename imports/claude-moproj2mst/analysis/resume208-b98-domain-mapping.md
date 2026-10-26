# CP208 B98 public-domain and Open/Save boundary audit

**CP212 qualification:** The original CLI crash cited below used a literal
`-f` path, while the executable decodes that option from four hexadecimal
characters per UTF-16 code unit. The literal input was malformed and cannot
establish a valid B98 Open failure. The public converter's B98 success and
side effects remain candidate evidence, but the two runs are not a valid
behavioral comparison. See `analysis/resume212-cli-decoder-independent.md`.

## Question and conclusion

B98 (`98_B_DS3F`, `Gear_blank.moproj`) is within the converter's public input domain: it is an admitted `.moproj`, its operation types are registered and parsed, and public full-Save conversion completes on the exact project fixture. The original GUI crash is **not currently evidenced as a failure of the converter's public headless mode**. The observed GUI path includes a `postOpen` direct `2signalRefreshMainView()` callback; public `m2m` has no GUI/main-view refresh callback and proceeds from modeled Open parsing to model selection, activation, active-operation load, then Save. Therefore CP206 success does not contradict an observed failure at that omitted UI callback, but it remains an internal-fidelity gap if the research claim is that all original Open behavior, including the GUI-only refresh, must be reproduced through the adapter.

This is not evidence of original Save behavior for B98. The original process crashed before Open returned; no original Save or accepted Close was reached. Nor does public-mode success establish complete byte equivalence: CP206 records successful output and side effects, while the historical comparator has a one-line difference for B98.

## Public API and supported domain evidence

- `source/apps/moproj2mst/include/moproj2mst.h:54-68` defines `M2M_FLAG_WRITE_SAVE` as the opt-in full Save-effects mode. It documents an already-selected input path and mapping of failures to adapter result codes. `m2m_convert_w` is the stable C ABI entry point (same header, lines 80 onward); CLI `--write-save` sets the same flag (`src/cli/main.cpp:56`).
- The exact B98 fixture at `evidence/gui_mo/oracle_b/98_B_DS3F_before/Gear_blank.moproj` is a normal XML project with `Operation_List Number="2"`. Its rows are `SfOprDiestress` and `SfOprForming`. The latter is the active/default flowchart row: the project has `<Sub_Project Active="0" Number="2" Type="0" ...>` at the project tail (fixture lines 216 onward). These are not unknown/unregistered operation names. `src/model/classinit.cpp` and `src/model/registry_rescue.cpp` register `SfOprForming`; `src/model/props_rescue.cpp` includes `SfOprDiestress`; `src/model/operation_rescue.cpp:18-45` walks `<Operation>` rows, obtains `<Type>`, creates the registered operation and parses properties. No B98-specific exclusion appears in the public contract.
- The converter docs (`source/docs/re/moproj2mst/status.md`, historical corpus row `57_Gear_blank`) already record successful ordinary `SfOprForming` project coverage. This is supporting family coverage, not a substitute for exact B98 evidence.
- Exact-project public evidence is CP206: `analysis/resume206-b98-candidate-comparison.md` binds the frozen CP201 executable hash and exact fixture hash, reports `--write-save --isolated-save` return code 0, MST output creation, and `.moproj`, KEY, and `DieStress.Wzd` effects. The comparison is not an original-vs-converter paired Save result. `analysis/wave3-native-save-validation.json` and `analysis/wave3-public-open-combo-validation.json` record historical B98 comparator status `DIFF` with one differing line, zero missing and zero extra; those totals are historical, and the validation documents warn they are not fresh original-session acceptance evidence.

## C++ stage boundary

For `M2M_FLAG_WRITE_SAVE`, `src/core/convert.cpp` constructs a `ConversionSession`, prepares the project model, initializes the project/registry, then calls `openProject` (`:207-241`). `src/moproj/read.cpp:28-65` implements that modeled Open: suffix/path admission, project DB setup, XML walk (`readProjectFile`/operation parsing), current-directory update, remesh load, and an accepted-project return. Its source comment at `:45-48` explicitly identifies the original UI refresh as not modeled.

After `openProject` returns, `convert.cpp:246-249` performs the Open-success gate, commits model selection, and activates subproject 0. It then loads the active operation (`:280`), prepares Open effects, and applies those effects before entering Save. The full-Save transition is explicit at `:336`; `saveCurrent` (Save step 2) starts at `:346-349`, then the project writer (step 3) is called at `:352-360`, property Save writers follow (`:371-400`), and the MST/remesh Save work follows (`:401-447`). Thus public `--write-save` necessarily crossed the converter's modeled Open return and proceeded through activation and Save.

## What CP205 does and does not establish

CP205 qualification reports the original v11.0.2 `DEF_GUI_MO_64.exe` fault in QtCore `QListData::size` at RVA `0x32c0`, read/RCX `0x58`, natural exit `0xC0000005`, during `postOpen`'s direct `2signalRefreshMainView()` callback before native Open returned. The owned fixture stayed unchanged. Its retained caller frames establish a real original process crash, but the report itself says intermediate owner/branch interpretation remains qualified. CP206 explicitly records that the command profiles differ and no original Save or accepted Close occurred.

The evidence therefore supports this narrower mapping:

1. Same project input and operation family are accepted by the public converter.
2. Public headless flow omits the GUI refresh callback where CP205 faulted, and then succeeds through its modeled Open/activation/Save stages.
3. Current evidence does **not** establish that the fault is part of the public m2m execution contract, because m2m is a converter adapter, not the original GUI event/main-view interface.
4. Since the charter requires architectural fidelity and labels the C ABI an adapter, the omitted `postOpen` refresh is still an acknowledged reconstruction gap to assess. A future claim that B98 has no corresponding original Open failure would require call-chain evidence proving the callback is strictly UI-specific / unreachable in the selected headless boundary, or a paired original invocation through a genuinely corresponding headless entry point. Neither is present here.
5. Do not infer how original Save would behave after a hypothetical successful refresh, and do not treat CP206's successful output as proof that Save should recover from the GUI's observed crash.

## Evidence paths

- `source/apps/moproj2mst/include/moproj2mst.h`
- `source/apps/moproj2mst/src/core/convert.cpp`
- `source/apps/moproj2mst/src/moproj/read.cpp`
- `source/apps/moproj2mst/src/model/operation_rescue.cpp`
- `source/apps/moproj2mst/src/model/classinit.cpp`
- `source/apps/moproj2mst/src/model/registry_rescue.cpp`
- `source/apps/moproj2mst/src/model/props_rescue.cpp`
- `evidence/gui_mo/oracle_b/98_B_DS3F_before/Gear_blank.moproj`
- `analysis/resume205-original-crash-capture-qualification.md`
- `analysis/resume206-b98-candidate-comparison.md`
- `analysis/wave3-native-save-validation.json`
- `analysis/wave3-public-open-combo-validation.json`

