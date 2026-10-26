# CP202 synthetic visibility fixture lifecycle repair

Outcome: **PASS**. Raw JSON receipt: `resume202-visibility-fixture-lifecycle-2026-10-26T04-42-53.880Z.json`.

The run uses only a newly created, converter-owned suspended Python child. It does not start the original application or acquire GUI control. Cleanup terminates the child through its retained process handle first; only then does the runner call Frida 17.23.2 Session.isDetached(). When the session is already detached, script cleanup is recorded as not applicable after target exit; no unload call is made or represented as successful. Independent CIM and Frida same-PID absence checks follow. Timeouts remain uncertain.

Historical attempts remain preserved in the original preflight report and attempt JSON. This report makes no claim about the original executable, Qt behavior, GUI behavior, or original interception.

```json
{
  "startedUtc": "2026-10-26T04:42:53.880Z",
  "moduleUrl": "file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js",
  "exe": "C:\\Program Files\\Python312\\python.exe",
  "cwd": "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin",
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
        "creation_filetime": 134374633745221660,
        "cwd": "c:\\users\\alext\\projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin",
        "host": "Sim2",
        "launcher_pid": 40888,
        "pid": 24700,
        "primary_tid": 1816,
        "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
      },
      "state": "suspended",
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
      "utc": "2026-10-26T04:42:54.443Z"
    },
    {
      "name": "validated-owned-create-suspended",
      "utc": "2026-10-26T04:42:54.527Z"
    },
    {
      "name": "attached",
      "utc": "2026-10-26T04:42:54.643Z"
    },
    {
      "name": "script-loaded",
      "utc": "2026-10-26T04:42:54.645Z"
    },
    {
      "name": "script-ready",
      "utc": "2026-10-26T04:42:54.645Z"
    }
  ],
  "failures": [],
  "readiness": {
    "kind": "fixture-result",
    "passed": true,
    "pid": 24700,
    "arch": "x64",
    "fridaRuntimeVersion": "17.23.2",
    "scriptRuntime": "QJS",
    "code": "0x26e5cdc0000",
    "site": "0x26e5cdc0040",
    "siteOffset": 64,
    "main": "0x26e5e9505c0",
    "dock": "0x26e5cdbf3c0",
    "vtable": "0x26e5cdbfa90",
    "candidateHex": "488b842450020000488b8020040000488b8c2450020000488b8920040000488b000fb6542474",
    "before64": "488b842450020000488b8020040000488b8c2450020000488b8920040000488b000fb6542474498900498948084189501089d04881c478020000c30000000000",
    "patched64": "e9c303f4ff0f1f00488b8020040000488b8c2450020000488b8920040000488b000fb6542474498900498948084189501089d04881c478020000c30000000000",
    "restored64": "488b842450020000488b8020040000488b8c2450020000488b8920040000488b000fb6542474498900498948084189501089d04881c478020000c30000000000",
    "changedByteOffsets": [
      0,
      1,
      2,
      3,
      4,
      5,
      6
    ],
    "changedSpanBytes": 7,
    "observations": [
      {
        "sequence": 1,
        "tid": 2208,
        "rsp": "0x6e8eb6e450",
        "main": "0x26e5e9505c0",
        "dock": "0x26e5cdbf3c0",
        "requestedRaw": 0,
        "match": true
      },
      {
        "sequence": 2,
        "tid": 2208,
        "rsp": "0x6e8eb6e450",
        "main": "0x26e5e9505c0",
        "dock": "0x26e5cdbf3c0",
        "requestedRaw": 1,
        "match": true
      }
    ],
    "runs": [
      {
        "phase": "baseline",
        "requested": 0,
        "returned": 0,
        "rax": "0x26e5cdbfa90",
        "rcx": "0x26e5cdbf3c0",
        "edx": 0
      },
      {
        "phase": "baseline",
        "requested": 1,
        "returned": 1,
        "rax": "0x26e5cdbfa90",
        "rcx": "0x26e5cdbf3c0",
        "edx": 1
      },
      {
        "phase": "hooked",
        "requested": 0,
        "returned": 0,
        "rax": "0x26e5cdbfa90",
        "rcx": "0x26e5cdbf3c0",
        "edx": 0
      },
      {
        "phase": "hooked",
        "requested": 1,
        "returned": 1,
        "rax": "0x26e5cdbfa90",
        "rcx": "0x26e5cdbf3c0",
        "edx": 1
      },
      {
        "phase": "restored",
        "requested": 0,
        "returned": 0,
        "rax": "0x26e5cdbfa90",
        "rcx": "0x26e5cdbf3c0",
        "edx": 0
      },
      {
        "phase": "restored",
        "requested": 1,
        "returned": 1,
        "rax": "0x26e5cdbfa90",
        "rcx": "0x26e5cdbf3c0",
        "edx": 1
      }
    ],
    "listenerDetached": true
  },
  "pythonSha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a",
  "helperSha256": "305d1036dc65ede44485aed49714f07290d65aaadbf1dbac11b3867a8e0c31e0",
  "agentSha256": "86fbc278d6015b04db91204600e939729eea7497e52d4b0d61c3a903ea67a034",
  "runnerSha256": "1dea29ea9d12e9024e9095f6f53d1c978eb74547bc8a7894cbd67fb28e4bf40c",
  "fridaVersion": "17.23.2",
  "helperArgv": [
    "-I",
    "-S",
    "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin\\owned_suspended_process.py",
    "--exe",
    "C:\\Program Files\\Python312\\python.exe",
    "--cwd",
    "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin",
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
      "creation_filetime": 134374633745221660,
      "cwd": "c:\\users\\alext\\projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin",
      "host": "Sim2",
      "launcher_pid": 40888,
      "pid": 24700,
      "primary_tid": 1816,
      "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
    },
    "state": "suspended",
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
  "localProcessAfter": {
    "schema": "owned-pid-presence-v1",
    "querySucceeded": true,
    "pid": 24700,
    "present": false,
    "processes": []
  },
  "fridaProcessAfter": null,
  "finishedUtc": "2026-10-26T04:42:54.917Z",
  "teardown": {
    "methodShape": {
      "isDetached": "function",
      "afterChildTermination": true
    },
    "attempts": [
      {
        "operation": "script.cleanup",
        "settled": true,
        "notApplicable": true,
        "disposition": "not_applicable_session_already_detached_after_target_exit"
      }
    ],
    "sessionDetachedAtEnd": true,
    "scriptUnloadSettled": false,
    "scriptCleanupDisposition": "not_applicable_session_already_detached_after_target_exit"
  },
  "outcome": "PASS",
  "evidencePath": "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\analysis\\resume202-visibility-fixture-lifecycle-2026-10-26T04-42-53.880Z.json"
}
```
