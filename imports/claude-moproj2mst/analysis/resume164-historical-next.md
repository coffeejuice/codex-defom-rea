# CP164 historical DIFF qualification and smallest headless action

## Conclusion

CP162's five DIFF rows remain historical comparator results; its SPRING2 Unicode reader change does not resolve or explain any of them. Four rows depend on missing original-session/action evidence (A55 and B120–B122). B98 is the only row with an independently executable headless candidate-versus-retained-artifact comparison. The smallest useful CP164 action is to rerun that existing bounded B98 acceptance against the immutable CP162 freeze, retaining the raw output and requiring its one-path-qualified copy to match. This checks the current frozen converter against the retained B98 before/after artifacts; it is not fresh-original acceptance.

## Five rows against CP162

The CP162 validation reports 93 SAME and five DIFF, all with `differ=1`, no extras, and no comparator adjustments:

| Historical case | Qualification retained before CP162 | Effect of CP162 source | Fresh-original evidence still needed |
|---|---|---|---|
| `A:RS_55_Multiple_Operations_Example1` | One `SimCtrl.KEY` difference. The A55 process's effective `MoInputMode`, resolved INI path, and INI hash are not retained. The generic `MoInputMode=1` backup is not process-bound. The existing candidate mode-1 route stops before the editor300 setup/iteration fill where the conditional CG signal path occurs; mode 0 does not explain the historical difference. | No connection to the CP162 SPRING2-only edit. It neither binds mode nor changes the SimCtrl writer/GUI callback. | A process-specific mode/settings receipt and a same-profile original Open→Save capture; any candidate comparison must then cover the same reachable editor300 path. No normalization is justified now. |
| `B:120_B_DOE` | One differing path plus 7 missing paths. The after-state contains GUI-created Type 2 study/output state absent from the before folder. | No connection to CP162's SPRING2 companion reader. | A fresh original action capture or a qualified complete before-state/action recipe that creates the same study state. A CLI run from the retained before folder is not comparable. |
| `B:121_B_DOE_VAR` | One differing path plus 8 missing paths. In addition to GUI-created study state, the after-state adds a scalar TLOC variable and `temp.KWD`. | No connection to CP162's SPRING2 companion reader. | The corresponding fresh original action capture or complete, qualified input state including the scalar variable. |
| `B:122_B_DOE_GEO` | One differing path plus 10 missing paths. The after-state includes GUI-created study state, a Workpiece-Geometry variable, and three GEO payloads. | No connection to CP162's SPRING2 companion reader. | The corresponding fresh original action capture or complete, qualified geometry-variable and GEO input state. |
| `B:98_B_DS3F` | One differing MST path, no missing/extra paths. CP95 already produced 29 comparable paths per tree; the only raw difference was the generated absolute `Gear_blank.KEY` path. Replacing exactly that one path in a separate copy made the MST byte-identical to the retained 2,756-byte original artifact. | CP162 changed `simcontrol300_native_io.cpp` for the bounded SPRING2 path, not the Diestress writer/path-owner code. The CP95 candidate comparison is strong historical evidence, but it does not test the CP162 frozen binary. | A fresh original run at the same path/profile is needed to qualify original execution and action replay. The headless CP164 rerun below only refreshes candidate-versus-retained evidence. |

## CP164 headless action

Run the existing `admin/accept_b98_before_after.py` against the **frozen CP162** executable and its adjacent DLLs, not a mutable build output. It copies the retained B98 before fixture to a unique `C:\PROBLEM\b98r\` path, runs the candidate only, retains raw output and receipts, compares the 29 comparable paths, then checks the exact single KEY-path substitution in a separate copy. Keep the raw comparison and normalized comparison distinct. Require the recorded freeze receipt and artifact hashes to match these identities:

- CP162 frozen source tree: `e3f1abbf87f8d577de4b915fb78f96c8b8d3a920b16060011e013fe92ba03c7e`.
- Frozen `moproj2mst.exe`: `0b4e14f3cfd0b3b2b3fe58443bb987c4b5a0bc3d3266d747ad14ef0cb8f592a6`.
- Frozen `moproj2mst.dll`: `4edd3639ed0c0383ded8cf07847a9981e4db1229340c2bf097bac3d6ac89f89b`.
- Frozen `deformdb.dll`: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.

Exact invocation from the converter checkout (the script sets and receipts `M2M_MO_INPUT_MODE=0` itself):

```powershell
python admin\accept_b98_before_after.py --candidate-version CP162 --freeze-receipt build\wave3-checkpoint162-verified\receipt.json --source-tree-sha256 e3f1abbf87f8d577de4b915fb78f96c8b8d3a920b16060011e013fe92ba03c7e --cli build\wave3-checkpoint162-verified\moproj2mst.exe --expected-cli-sha256 0b4e14f3cfd0b3b2b3fe58443bb987c4b5a0bc3d3266d747ad14ef0cb8f592a6 --expected-moproj2mst-dll-sha256 4edd3639ed0c0383ded8cf07847a9981e4db1229340c2bf097bac3d6ac89f89b --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

Acceptance remains narrowly defined: exit 0, 29 comparable paths on both sides, exactly one raw content difference at `Gear_blank.MST`, one candidate and one retained KEY path, and exact whole-file equality after replacing only the candidate KEY path in the separate comparison copy. The script preserves a unique run directory and does not clean up. Do not describe success as a fresh-original pass or as closure of the historical B98 row.

## Evidence identities and limits

- `analysis/wave3-checkpoint162-validation.json` SHA-256 `9168996ebc82d9fa7d49b2c8da6936abc7e3ff04c6b1a09c240de6aec50c39e7`; records the five DIFF rows and says historical oracles are not fresh original-session evidence.
- `build/wave3-checkpoint162-verified/receipt.json` SHA-256 `dc7abf4341bff269c447f25c70e1631f1c188e92b90c492651f0714c40af3e97`; binds the CP162 source tree and three deployed artifacts listed above.
- CP162's production source change `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` SHA-256 `1f3df812602c0cda75b394750ffc523a2cf938e01cdb32b01449f4a032ef5313`. Its implementation contract confines the change to the SPRING2 companion reader's explicit UTF-16/32 BMP decimal-digit profile; the writer/path behavior used by B98 is not in that source change.
- `analysis/resume98-retained-mode-binding-audit.md` SHA-256 `3b474a1f9aba45f79093d9e0a07a97eafc126cf6c2fdea5816412e3a86063321`; records why A55 and DOE are not source-ready paired comparisons and why their DIFFs should be preserved.
- `analysis/resume95-cp95-b98-acceptance.md` SHA-256 `759481f000110cfc864e47759b64daed554af5311b7c48489b5b1720ad1beff4`; records the CP95 candidate-only B98 result and its limitations.

No source, test, fixture, shared plan/state, GUI, application, or live REA state was changed or run for this audit. The CP164 B98 action is a recommendation, not an executed result. Fresh original evidence for B98 still requires an original run; A55 additionally requires process-bound mode provenance; B120–B122 require the GUI-created study/variable/geometry state to be represented by an original capture or a fully qualified input/action sequence.
