# CP68 efficiency review: CP66–CP68

## Configure, preflight, and full-build receipts

| Checkpoint | Focused targets | Preflight configure/build/total | Full configure/build/total | Sum of both build receipts |
|---|---:|---:|---:|---:|
| CP66 | 3 | 10.875 / 5.656 / 19.397 s | 10.875 / 21.578 / 35.302 s | 54.699 s |
| CP67 | 4 | 11.000 / 5.094 / 19.118 s | 10.953 / 22.422 / 36.255 s | 55.373 s |
| CP68 | 5 | 10.937 / 19.813 / 33.721 s | 10.968 / 58.344 / 72.294 s | 106.015 s |

All six build receipts passed, reported stable source during each operation, and paired each checkpoint’s focused preflight with its full build on the same source fingerprint: CP66 `866af960…e6cc`, CP67 `9a3ccf5b…2852`, and CP68 `5fd4d38d…b314`. The CP68 verified source archive binds that same fingerprint and 3,292 files.

The notable regression is CP68’s build work: its full build took 58.344 s versus 21.578 s in CP66 and 22.422 s in CP67. The logs show 89 C++ object compile tasks in CP68’s full build, compared with 8 and 7 respectively; link tasks were similar (189 versus 188 and 187). This indicates materially more recompilation, but the receipts do not isolate which source/header changes caused it. CP68’s five-target preflight also needed 19.813 s of build work, versus roughly 5 s in CP66/67. Together, CP68’s two build receipts total 106.015 s, about 50.6 s more build-phase work than CP67; configure time remained nearly flat at 21.905 s total.

## Full launcher and test bindings

| Checkpoint | Cases | Launcher | CTest | Fixture SHA passes | Source/binary binding passes | Stable/no changes |
|---|---:|---:|---:|---:|---:|---|
| CP66 | 212 | 29.781 s | 12.203 s | 13.015 s | 3.516 s | yes |
| CP67 | 212 | 30.187 s | 12.641 s | 12.906 s | 3.547 s | yes |
| CP68 | 214 | 32.078 s | 14.110 s | 13.282 s | 3.608 s | yes |

CP68 completed with exit 0, no skipped or failed tests, no fixture changes, stable source binding, and equal before/after SHA-256 manifests for all 211 captured CTest/CLI binaries. Six logged gaps and one logged difference remain. Its launcher took 1.891 s longer than CP67 with two more tests; CTest rose 1.469 s, while fixture and binding hashes together rose 0.437 s. This small sample does not establish a performance trend.

The new admin source preflight was measured separately on CP68’s five focused test files using three end-to-end CLI trials: 0.0684, 0.0696, and 0.0709 s (median **0.0696 s**); all returned success for all five paths. This is below 0.3% of the test launcher and negligible against build time. It is not a timing phase captured in CP68’s original runner receipt, so it is reported as a separate rerun, not retroactively included in CP68’s measured total.

## Next practical optimization

The recorded receipts show a repeated configure cost of about 10.9 s for both the focused preflight and full-build invocation. For future checkpoints, a single stable-snapshot build workflow could configure once, run the selected focused target group, then run the full build in the same configured build tree. Keep one source fingerprint around both build commands and reject the receipt if it changes; hash all final artifacts only after the full build. This preserves the focused Release gate and full build while removing one redundant configure phase, with a measured opportunity of roughly 11 s per checkpoint. Keep all CTest cases, fixture hash passes, source/binary bindings, and folder coverage unchanged.

This optimization targets measured overhead, not the CP68 recompile increase. The cause of the additional 81 C++ compile/link tasks is not established by these receipts; do not suppress or narrow the full build to hide it. Token usage was unavailable in the receipts.

Evidence: `analysis/resume66-focused-preflight-build.json`, `analysis/resume66-integrated-full-build.json`, `analysis/resume66-integrated-full-tests.json`; corresponding CP67 and CP68 receipts; `build/logs/resume66-integrated-full-build.log`, `build/logs/resume67-integrated-full-build.log`, `build/logs/resume68-integrated-full-build.log`; CP68 freeze receipt `build/wave3-checkpoint68-verified/receipt.json`.
