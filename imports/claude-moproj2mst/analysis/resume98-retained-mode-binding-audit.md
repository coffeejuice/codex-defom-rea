# CP98 retained-artifact mode binding and next-comparison audit

## Finding

No retained artifact independently binds A55's original process to an effective `MoInputMode`, and none of B120–B122 is a complete before-folder Open→Save case. Therefore no new mode-equivalent before/after comparison can be selected from the retained material alone. No app or candidate was run for this audit.

## A55 evidence check

The retained `_tools/rs.py` describes one fresh original process, File→Open, File→Save, File→Close, answer “No” to the close-time save question, then exit. `rs_log.tsv` records A55 saved (3,541-byte MST), one close dialog answered No, exit 0, and one dialog. It records no PID, INI path/hash, parsed mode, mode-change event, active editor class, or CG-toggle callback. The A55 output directory contains `opened.png`, the `.moproj`, MST and `.MST.corpus`; its before folder contains the pristine 28-file copy. Neither tree contains the app settings file or a process-settings receipt.

The only retained `DEFORM-MO.ini` backup located under Oracle-B is `_tools/settings_backup_l5s/DEFORM-MO.ini`, SHA-256 `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`, containing `MoInputMode=1`. It is a generic L5 settings backup (alongside `DEFORM.ini`), dated 2026-10-22; nothing binds that file to the A55 process or proves it was the path that process read. `opened.png` shows a pane/state but cannot distinguish the internal mode or prove the checked-signal callback ran. Thus mode 1 is a hypothesis, not an independent original binding. The exact missing artifact is the A55 process's effective `[Setting] MoInputMode` plus the resolved INI path and content hash at launch (or an equivalently direct retained process trace).

Even if that setting were recovered as nonzero, the current candidate public nonzero-mode path is not a complete paired comparison: retained source qualification says it stops before editor300 setup/iteration fill, where the conditional CG signal path is reached. If recovered as mode 0, the static editor300 callback hypothesis would not explain the two bytes, and a mode-0 candidate run still would not establish why the historical original output differs. No byte normalization is valid for the A55 `SimCtrl.KEY` row.

## DOE cases

The retained DOE before folders start from a project with no Type 2 study. The after states include UI-created study/output state; B121 additionally adds the scalar TLOC variable and `temp.KWD`; B122 adds a Workpiece-Geometry variable and three GEO payloads. The diff summaries and screenshots document those changes, not a candidate CLI action recipe for creating them. Current public conversion still selects subproject 0, while the retained post-action DOE XML requires the separate owned-study/activation and ordered child handling. CP95's validation still records B120/B121/B122 as the same historical DIFF rows (7/8/10 missing paths respectively). Running the CLI from those before folders would omit known GUI-created effects, so it cannot serve as the complete before/after comparison requested here. A mode setting would not repair that action/input mismatch.

## B98 reconciliation

CP93's conclusion that no remaining historical row is ready remains correct. CP95's later candidate-only B98 run strengthens only the already-bounded B98 artifact comparison: frozen CP95, receipt `C:\PROBLEM\b98r\r95-49479e3d\receipt.json`, 29 comparable paths per tree, one raw `Gear_blank.MST` path difference, and byte-identical MST after replacing exactly the candidate KEY path. It did not execute the original or establish the historical GUI action sequence, so it does not bind A55's settings or make the DOE cases comparable.

**Disposition:** preserve the historical DIFF rows. The next smallest independent A55 evidence would be a retained process-specific settings receipt if one exists in an unindexed capture source; none appears in the checked A55 folders, `rs_log.tsv`, generic settings backup, or screenshot metadata. Without that evidence, a new original trial would be required, but this report does not authorize or schedule GUI control.

No source, build, fixture, settings, or REA state was modified. No original executable or GUI was launched.
