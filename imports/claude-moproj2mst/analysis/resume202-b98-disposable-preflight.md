# CP202 B98 disposable preflight (read only)

## Scope and result

This report checks the B98 fixture, the two converter-owned preparation documents, current installed image hashes, and the proposed short disposable root. No original executable was launched; no GUI or app-control session was acquired; no REA attach, copy, source/build change, or parent DMO/Claude change was made. `python tools/recovery_status.py` reports the broader recovery blocked on the unavailable GUI release lifecycle; the two B98 plan documents below show a separate Frida attach capability blocker.

The two requested documents are under the converter subtree:

- `imports/claude-moproj2mst/analysis/resume201-passive-oracle-probe-plan.md`
- `imports/claude-moproj2mst/analysis/resume202-frida-attach-selftest.md`

The 201 plan's candidate is the installed original `DEF_GUI_MO_64.exe` with arguments `-m open -f <project>`. It requires a disposable copy of the full fixture, an explicit disposable working directory, complete prehook identity checks/readiness, bounded observation, and verified exact-process termination and absence. It prohibits treating termination as accepted Close evidence. The 202 self-test found that REA `start_frida_session(mode=attach,pid=...)` returned `capability_unavailable` for a harmless suspended Python child, without resuming it. That qualifies the attempted provider method only; it does not rule out all instrumentation methods. The reported same-host held-handle cleanup lane was qualified for that Python child only.

## Fixture identity and path review

Current fixture directory: `imports/claude-moproj2mst/evidence/gui_mo/oracle_b/98_B_DS3F_before`. It has 27 files. `Gear_blank.moproj` is 10,932 bytes, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`, matching the retained identity in the plan. The project XML references its assets by relative paths (`Objects/...`, `Materials/...`, `Equipments/...`) and names the primary database `Gear_blank.DB`. A byte-level scan of all fixture files found no embedded drive-absolute or UNC path references. This scan does not establish that the application itself or its libraries never write to user settings, registry, or other external locations.

Per-file content hashes and projected path lengths under `C:\PROBLEM\p202a`:

| Relative file | Bytes | SHA-256 | Full path chars |
|---|---:|---|---:|
| `Gear_blank.moproj` | 10932 | `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd` | 34 |
| `Gear_blank.MST.oracleA` | 3341 | `0cd9baf2d3ebfffd93e173d8082aceeb65d2b20acd24b099a2895a99637bba96` | 39 |
| `Equipments\Equipment00001.KEY` | 824 | `5ca3d185d7124fc473045971bdaf1d49c1a5a25a390850dcd5375211c2c1cfe7` | 46 |
| `Equipments\Equipment00002.KEY` | 824 | `2350acf340a1a8c08901ce3a16289be392bc6a6461d03f9ec7420103391de479` | 46 |
| `Equipments\Equipment00003.KEY` | 824 | `c0b881dab5a57ca1007eb05ba4d40077a8ea036d3e83b5db4a9dbf5d60fdbaaa` | 46 |
| `Equipments\Equipment00004.KEY` | 666 | `17ff93d94c561d2e395e841dd94abe47b92873e6cb6cd6e6a03f90f30b909a6a` | 46 |
| `Equipments\Equipment00005.KEY` | 824 | `ae3b86be5d6fe40bc7eb177af201ac9f4829c7f849b7f5d3f27e35b477088274` | 46 |
| `Equipments\Equipment00006.KEY` | 824 | `3d22ecf079e61469cc6a73e8869855913a2c1aacd12330d9e13fbe11ba7a02db` | 46 |
| `Materials\Material00001.KEY` | 4439 | `712868b9d5be55aa43896d98db61a0dead82fe81aabef0ebf56a6bc1270bb256` | 44 |
| `Materials\Material00002.KEY` | 4439 | `bbd41da7b3f43c5d8c1c5d055cb24f0169c66f273cff7975e73da452ac1a6135` | 44 |
| `Objects\Object00001.KEY` | 524074 | `8f369e2d895a5dcd6e70203ce38b4dab2063f2f455970b4e637cadb7d5d0dadb` | 40 |
| `Objects\Object00002.KEY` | 40760 | `e937cf42cf2a64758b197c35481563ee0fc4802d6e48f151fffaa4df19d06ad5` | 40 |
| `Objects\Object00003.KEY` | 40763 | `0df6d724ac8c3ea629c80e817877365c71ec4835fe589468a2eb20b382582efb` | 40 |
| `Objects\Object00005.KEY` | 1095631 | `a06e811aa9d8aa03df739c4e6feed92bb2d8437f0b86ef9fdf07f7e59a16dc79` | 40 |
| `Objects\Object00006.KEY` | 982594 | `d185e9b0464ecf4435adff15327c59f15c4fdeb9fd7505dabe19a3a99c980cf3` | 40 |
| `Operations\Task00001\InterObject.KEY` | 631 | `f2931ede73e8debfb8c78d7ffc0a7af1051769f1b0911f95cd96e7008077ae82` | 53 |
| `Operations\Task00001\MeshSettings00001.KEY` | 624 | `2f570d8fcead57b2c6649032f49f73b6b08a2ff6c3f606e618c18b7a8d22ef7f` | 59 |
| `Operations\Task00001\ObjProperties00001.KEY` | 169 | `021caff357de2d72e5043483a36c35ef5848233258ba55530c0fde9499a0b713` | 60 |
| `Operations\Task00001\ObjProperties00002.KEY` | 169 | `26e9d62de1e0faeb32262d6ef2897a50530452f3246089bdcedb2c5cf6ceac2e` | 60 |
| `Operations\Task00001\ObjProperties00003.KEY` | 169 | `4e19975afc35c73512b5a1906044799959d5cb7113a1e104039d5b026f2360e8` | 60 |
| `Operations\Task00001\SimCtrl.KEY` | 2232 | `50f8f88cf7ec26cd0d46b4c9816185b2abd24b7b9ed606884c613f700a58865c` | 49 |
| `Operations\Task00002\InterObject.KEY` | 631 | `6e56721bf95738edfcc07c29cf3a07ad931b3b3fe05a306525446a47d4716d37` | 53 |
| `Operations\Task00002\MeshSettings00003.KEY` | 624 | `674425cc9086977b471ad3dc494394be1f00c242f83618402838ec9188952142` | 59 |
| `Operations\Task00002\ObjProperties00001.KEY` | 169 | `021caff357de2d72e5043483a36c35ef5848233258ba55530c0fde9499a0b713` | 60 |
| `Operations\Task00002\ObjProperties00002.KEY` | 169 | `26e9d62de1e0faeb32262d6ef2897a50530452f3246089bdcedb2c5cf6ceac2e` | 60 |
| `Operations\Task00002\ObjProperties00003.KEY` | 169 | `4e19975afc35c73512b5a1906044799959d5cb7113a1e104039d5b026f2360e8` | 60 |
| `Operations\Task00002\SimCtrl.KEY` | 2136 | `7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c` | 49 |

The longest current projected path is 60 characters, below the strict 100-character limit. The existing `C:\PROBLEM\p202a` path does not exist. The check was read-only; no directory was created. This length assessment covers current fixture names only, not filenames generated by DEFORM.

## Current installed original image hashes

Read-only hashes at the plan's expected locations:

| File | Bytes | SHA-256 | Comparison |
|---|---:|---|---|
| `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe` | 67,611,136 | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` | Matches retained plan digest |
| `C:\Program Files\SFTC\DEFORM\v11.0.2\QtGui4_64.dll` | 10,859,008 | `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` | Matches retained plan digest |
| `C:\Program Files\SFTC\DEFORM\v11.0.2\QtCore4_64.dll` | 3,250,688 | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` | Matches retained plan digest |

These are current on-disk file hashes. They do not certify the bytes of modules loaded in a future process.

## What a no-instrumentation CLI run can establish

If independently authorized and properly isolated, a CLI invocation may establish that the named executable was started with the recorded arguments and disposable cwd, whether the process returned/exited within the chosen timeout, its exit status, and observed filesystem differences within the disposable directory. It may provide limited evidence that the application accepted or attempted the Open request if its own output or expected project artifacts show this.

It cannot establish the dock visibility/event/signal/slot chain or the Save-route call. A changed file does not by itself attribute a successful Save; unchanged files do not prove no Save occurred. Process exit or termination does not establish accepted Close. A CLI run without instrumentation cannot show that startup was dialog-free or distinguish an unobserved modal from a stalled or unattended UI state. Directory snapshots cannot capture registry, user settings, or writes outside the disposable root. These constraints are consistent with the 201 plan: the full Open -> Save -> Close oracle remains unproven.

## Preflight decision

The fixture content and current installed-image identities are suitable for a future controlled disposable-root attempt; the fixture paths are short and contain no discovered absolute external references. However, the planned passive Frida route remains blocked by the 202 self-test's provider attach failure. No launch is performed by this preflight. Any later run must first satisfy the 201 plan's cwd, instrumentation/readiness, and exact-owned-process cleanup gates and preserve a pre/post source-fixture manifest.

