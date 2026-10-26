# CP206 B98 converter candidate comparison

**CP212 qualification:** The original CLI side of the proposed comparison
passed a literal `-f` path to a four-hex-digit-per-UTF-16 decoder. Its
malformed 34-character argument is outside the decoder's valid domain.
The converter's B98 Save receipt remains valid for the candidate, but the
original crash is not a valid B98 Open comparator. See
`analysis/resume212-cli-decoder-independent.md`.

The frozen CP201 reconstructed converter was tested on a fresh complete copy of the B98 Gear project, without running or controlling the original application. The source was `evidence/gui_mo/oracle_b/98_B_DS3F_before`; all 27 copied files in `C:\PROBLEM\b98cp201\project` were checked by relative path, length and SHA-256 before execution (`resume206-b98-candidate-before.json`). The copied `Gear_blank.moproj` SHA-256 was `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`, matching the original crash fixture.

The exact frozen executable `build/wave3-checkpoint201-verified/moproj2mst.exe` had SHA-256 `0b531be96af428c53031700ee62cfd193cea73f879563499878e8dc367831fa3`. It ran the public command-line conversion with an explicit output path, `--write-save`, the installed DEFORM directory, and `--isolated-save`. The reconstructed process returned `0` in the recorded 09:09:15.268–09:09:15.332 UTC interval (`resume206-b98-candidate-process.json`). The isolated supervisor's source returns the worker's code directly on a normal worker return and writes its special receipt only for a qualified terminal case; no receipt file is expected or present on this normal path. There was no captured stderr/stdout text (the PowerShell pipeline produced no console file).

Afterward, `C:\PROBLEM\b98cp201\result\Gear_blank.MST` existed with 2,718 bytes, SHA-256 `94ea8a00d2f860def960ca3f6c110a72d045d35229ac691cfa81d5cb50ef3b35`. The copied project contained 29 files: `Gear_blank.moproj` changed, and `Gear_blank.KEY` plus `Operations\Task00001\DieStress.Wzd` were added. No preexisting project file was removed (`resume206-b98-candidate-after.json`). The evidence source, CP205 original disposable fixture and frozen build were not modified by this comparison.

The original v11.0.2 `DEF_GUI_MO_64.exe -m open -f` B98 run is a **different command profile**: it crashed during `postOpen`'s direct `2signalRefreshMainView()` callback, before Open returned, with QtCore read/RCX `0x58` and natural exit `0xC0000005`; its 27 project files stayed unchanged (`resume205-original-crash-capture-qualification.md`). The reconstructed `--write-save` success is therefore a substantial compatibility discrepancy candidate in the shared Open stage, but these two command profiles are not a complete paired Open→Save→Close acceptance comparison. No original Save or Close occurred. Resolve the producer/field transition and public m2m input-domain correspondence before selecting a process-level failure contract. Preserve the original bug if it is confirmed in the supported lifecycle; do not silently normalize it to a successful Save.
