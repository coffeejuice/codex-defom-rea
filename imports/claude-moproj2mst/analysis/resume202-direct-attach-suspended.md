# Direct Frida attach to an owned CREATE_SUSPENDED child

One attempt against an isolated Python sleep child created by the existing helper. The helper retains the original process/thread handles and enforces its 20 second watchdog. No GUI or existing process was targeted. Cleanup uses helper terminate/EOF and its exact held handle; no PID-based kill is used.

This tests attachment before helper ResumeThread; it does not establish attachment to the reference application or REA guardian integration. A success would support this helper as a candidate ownership path, subject to integration checks.

```json
{
  "startedUtc": "2026-10-26T04:09:29.686Z",
  "moduleUrl": "file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js",
  "exe": "C:\\Program Files\\Python312\\python.exe",
  "cwd": "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\resume202-frida-selftest",
  "maxSeconds": 20,
  "helperPath": "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin\\owned_suspended_process.py",
  "receipts": [
    {
      "event": "created",
      "exit_code": 259,
      "exit_verified": false,
      "gui_control_acquired": false,
      "identity": {
        "argv": [
          "c:\\program files\\python312\\python.exe",
          "-I",
          "-S",
          "-c",
          "import time; time.sleep(60)"
        ],
        "canonical_image": "c:\\program files\\python312\\python.exe",
        "creation_filetime": 134374613703545070,
        "cwd": "c:\\users\\alext\\projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\resume202-frida-selftest",
        "host": "Sim2",
        "launcher_pid": 32052,
        "pid": 10408,
        "primary_tid": 7124,
        "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
      },
      "state": "suspended",
      "wait_result": 258
    },
    {
      "event": "resumed",
      "exit_code": 259,
      "exit_verified": false,
      "previous_suspend_count": 1,
      "state": "running",
      "wait_result": 258
    },
    {
      "event": "endpoint",
      "reason": "terminate_command"
    },
    {
      "event": "cleanup",
      "exit_code": 57857,
      "exit_verified": true,
      "handles_closed": true,
      "state": "exited",
      "termination_attempts": [
        {
          "api_succeeded": true,
          "exit_code": 57857,
          "exit_verified": true,
          "requested_exit_code": 57857,
          "source": "final_cleanup",
          "wait_result": 0,
          "winerror": null
        }
      ],
      "termination_requested": true,
      "wait_result": 0
    }
  ],
  "stages": [
    {
      "name": "local-device",
      "utc": "2026-10-26T04:09:30.269Z"
    },
    {
      "name": "validated-owned-create-suspended",
      "utc": "2026-10-26T04:09:30.359Z"
    },
    {
      "name": "attached",
      "utc": "2026-10-26T04:09:30.473Z"
    },
    {
      "name": "script-loaded",
      "utc": "2026-10-26T04:09:30.475Z"
    },
    {
      "name": "script-ready",
      "utc": "2026-10-26T04:09:30.475Z"
    },
    {
      "name": "helper-resumed",
      "utc": "2026-10-26T04:09:30.475Z"
    }
  ],
  "failures": [],
  "readiness": {
    "kind": "ready",
    "pid": 10408
  },
  "pythonSha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a",
  "helperSha256": "305d1036dc65ede44485aed49714f07290d65aaadbf1dbac11b3867a8e0c31e0",
  "fridaVersion": "17.23.2",
  "helperArgv": [
    "-I",
    "-S",
    "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin\\owned_suspended_process.py",
    "--exe",
    "C:\\Program Files\\Python312\\python.exe",
    "--cwd",
    "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\resume202-frida-selftest",
    "--sha256",
    "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a",
    "--max-seconds",
    "20",
    "--",
    "-I",
    "-S",
    "-c",
    "import time; time.sleep(60)"
  ],
  "created": {
    "event": "created",
    "exit_code": 259,
    "exit_verified": false,
    "gui_control_acquired": false,
    "identity": {
      "argv": [
        "c:\\program files\\python312\\python.exe",
        "-I",
        "-S",
        "-c",
        "import time; time.sleep(60)"
      ],
      "canonical_image": "c:\\program files\\python312\\python.exe",
      "creation_filetime": 134374613703545070,
      "cwd": "c:\\users\\alext\\projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\resume202-frida-selftest",
      "host": "Sim2",
      "launcher_pid": 32052,
      "pid": 10408,
      "primary_tid": 7124,
      "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
    },
    "state": "suspended",
    "wait_result": 258
  },
  "resumed": {
    "event": "resumed",
    "exit_code": 259,
    "exit_verified": false,
    "previous_suspend_count": 1,
    "state": "running",
    "wait_result": 258
  },
  "helperExit": {
    "code": 0,
    "signal": null
  },
  "cleanup": {
    "event": "cleanup",
    "exit_code": 57857,
    "exit_verified": true,
    "handles_closed": true,
    "state": "exited",
    "termination_attempts": [
      {
        "api_succeeded": true,
        "exit_code": 57857,
        "exit_verified": true,
        "requested_exit_code": 57857,
        "source": "final_cleanup",
        "wait_result": 0,
        "winerror": null
      }
    ],
    "termination_requested": true,
    "wait_result": 0
  },
  "localProcessAfter": null,
  "fridaProcessAfter": null,
  "finishedUtc": "2026-10-26T04:09:30.793Z",
  "outcome": "PASS"
}
```
