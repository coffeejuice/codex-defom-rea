# CP202 harmless owned-process Frida attach preflight

This preflight launched only a suspended Python 3.12 child through the
converter-owned helper. It did not launch DEFORM, acquire GUI control, attach
to a user process, or modify installed files. The child command was
`-I -S -c "import time; time.sleep(60)"` with explicit converter scratch cwd;
the verified Python image SHA-256 was
`4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a`.

The helper reported host `Sim2`, PID `33156`, creation FILETIME
`134374606486452407`, primary TID `32528`, and suspended state. The REA
`start_frida_session({mode:"attach",pid:33156,device_id:"local"})` call
returned `isError=true`, code `capability_unavailable`, category
`unsupported_provider`, message “This analysis is unavailable for the
current target. Choose another analysis or target.” No session ID was
issued; no script was loaded and the child was never resumed. This is the
REA tool's projected error category. A later read-only inspection found that
the manager can wrap several backend exceptions in this generic category;
the actual Windows/Frida attach cause was not retained in this compact call
output. It does not prove the original cannot be instrumented. The same
failed action was not retried.

The helper's 20-second watchdog then emitted `endpoint: deadline` and a
cleanup receipt: exact held-handle termination source `watchdog_deadline`,
requested and observed exit code `57857` (`0xE201`), API success, wait result
0, `exit_verified=true`, `handles_closed=true`. Independent PowerShell
`Get-Process -Id 33156` found no process; REA
`list_frida_processes({device_id:"local"})` returned no entry for PID 33156
and `cleanup_error=null`. The helper exited 0. These observations qualify
this one owned child only; no accepted application Close is inferred.

Before this launch, a read-only host alignment check found exact Codex PIDs
22324 and 31472 in both local PowerShell and REA's local process inventory.
Afterward `list_frida_devices({})` listed local, socket and barebone devices.
Inventory availability does not establish attach capability. The passive
original Save-route probe remains blocked on a diagnosed attach/instrument
method; the suspended-launch cwd and cleanup lane is independently usable.
