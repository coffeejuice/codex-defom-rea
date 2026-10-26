# Direct Frida owned-child smoke test

One direct Node/Frida attempt against a newly spawned isolated Python sleep process. No GUI application or existing user process was attached.

This diagnoses the direct binding path only; it does not prove that REA wrapper attachment or a reference-application capture works. Explicit cwd is established only if the injected GetCurrentDirectoryW result matches the requested directory.

```json
{
  "startedUtc": "2026-10-26T04:04:39.394Z",
  "moduleUrl": "file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js",
  "target": "C:\\Program Files\\Python312\\python.exe",
  "argv": [
    "C:\\Program Files\\Python312\\python.exe",
    "-I",
    "-S",
    "-c",
    "import time; time.sleep(60)"
  ],
  "cwd": "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\resume202-frida-selftest",
  "stages": [
    {
      "name": "local-device",
      "utc": "2026-10-26T04:04:39.976Z"
    },
    {
      "name": "spawned-paused",
      "utc": "2026-10-26T04:04:39.982Z",
      "pid": 26488
    },
    {
      "name": "attached",
      "utc": "2026-10-26T04:04:40.091Z"
    },
    {
      "name": "script-created",
      "utc": "2026-10-26T04:04:40.092Z"
    },
    {
      "name": "script-loaded",
      "utc": "2026-10-26T04:04:40.093Z"
    },
    {
      "name": "script-ready",
      "utc": "2026-10-26T04:04:40.093Z"
    },
    {
      "name": "resumed",
      "utc": "2026-10-26T04:04:40.093Z"
    },
    {
      "name": "owned-child-killed",
      "utc": "2026-10-26T04:04:40.345Z"
    }
  ],
  "failures": [],
  "pid": 26488,
  "readiness": {
    "kind": "ready",
    "pid": 26488,
    "cwd": "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\resume202-frida-selftest",
    "cwdLength": 99
  },
  "fridaPackageVersion": "17.23.2",
  "localProcessAfter": null,
  "fridaProcessAfter": null,
  "finishedUtc": "2026-10-26T04:04:40.779Z",
  "outcome": "PASS"
}
```
