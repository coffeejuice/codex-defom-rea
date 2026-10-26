# Owned Python child Frida exception-handler no-crash preflight

This converter-only preflight creates a harmless Python sleeper in a suspended state, attaches directly with Frida, installs `Process.setExceptionHandler` with a callback that returns `false`, records the callback registration readiness, and resumes the sleeper. Cleanup asks the owning helper to terminate through its retained process handle, waits for process exit and both handles to close, then requires strict PID absence in independent Windows and Frida process views. It does not generate an exception or crash. A pass establishes only that the callback registration and owned-child cleanup path work; it does not test callback invocation, access-violation payloads, or converter exception behavior. Native execution is guarded off pending root review.

```json
{
  "schema": "owned-python-exception-handler-no-crash-preflight-v1",
  "startedUtc": "2026-10-26T05:29:20.582Z",
  "host": "Sim2",
  "target": {
    "python": "C:\\Program Files\\Python312\\python.exe",
    "cwd": "C:\\PROBLEM",
    "childCode": "import time; time.sleep(60)"
  },
  "guiInput": false,
  "guiControlAcquired": false,
  "interceptorHooks": 0,
  "maxSeconds": 20,
  "nativeExecutionEnabled": true,
  "stages": [
    {
      "name": "frida-local-device",
      "utc": "2026-10-26T05:29:21.158Z"
    },
    {
      "name": "helper-started",
      "utc": "2026-10-26T05:29:21.166Z"
    },
    {
      "name": "owned-harmless-python-child-created-suspended",
      "utc": "2026-10-26T05:29:21.241Z"
    },
    {
      "name": "frida-attached-before-resume",
      "utc": "2026-10-26T05:29:21.370Z"
    },
    {
      "name": "handler-installed-before-resume",
      "utc": "2026-10-26T05:29:21.371Z"
    },
    {
      "name": "resumed-harmless-sleeper",
      "utc": "2026-10-26T05:29:21.404Z"
    },
    {
      "name": "exact-held-handle-termination-requested",
      "utc": "2026-10-26T05:29:21.404Z"
    }
  ],
  "failures": [],
  "helperRawReceipts": [
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
        "creation_filetime": 134374661612364350,
        "cwd": "c:\\problem",
        "host": "Sim2",
        "launcher_pid": 35280,
        "pid": 40696,
        "primary_tid": 30012,
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
  "scriptMessages": [
    {
      "type": "send",
      "payload": {
        "kind": "handler-ready",
        "pid": 40696,
        "arch": "x64",
        "platform": "windows",
        "registrationCallCompleted": true
      }
    }
  ],
  "pythonSha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a",
  "helperSha256": "8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d",
  "fridaVersion": "17.23.2",
  "fridaPackagePath": "C:\\Users\\alext\\AppData\\Roaming\\npm\\node_modules\\rea-agents\\node_modules\\frida\\package.json",
  "fridaPackageSha256": "7efb59d2050b7b6304e1617f463c4eb7b2f7cb7e3e80e677976825a76ee31e8d",
  "fridaDeviceId": "local",
  "helperArgv": [
    "C:\\Program Files\\Python312\\python.exe",
    "-I",
    "-S",
    "C:\\Users\\alext\\Projects\\codex-deform-rea\\imports\\claude-moproj2mst\\admin\\owned_suspended_process.py",
    "--exe",
    "C:\\Program Files\\Python312\\python.exe",
    "--cwd",
    "C:\\PROBLEM",
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
  "createdReceipt": {
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
      "creation_filetime": 134374661612364350,
      "cwd": "c:\\problem",
      "host": "Sim2",
      "launcher_pid": 35280,
      "pid": 40696,
      "primary_tid": 30012,
      "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
    },
    "state": "suspended",
    "wait_result": 258
  },
  "ownedIdentity": {
    "argv": [
      "c:\\program files\\python312\\python.exe",
      "-I",
      "-S",
      "-c",
      "import time; time.sleep(60)"
    ],
    "canonical_image": "c:\\program files\\python312\\python.exe",
    "creation_filetime": 134374661612364350,
    "cwd": "c:\\problem",
    "host": "Sim2",
    "launcher_pid": 35280,
    "pid": 40696,
    "primary_tid": 30012,
    "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
  },
  "handlerReady": {
    "kind": "handler-ready",
    "pid": 40696,
    "arch": "x64",
    "platform": "windows",
    "registrationCallCompleted": true
  },
  "resumedReceipt": {
    "event": "resumed",
    "exit_code": 259,
    "exit_verified": false,
    "previous_suspend_count": 1,
    "state": "running",
    "wait_result": 258
  },
  "endpointReceipt": {
    "event": "endpoint",
    "reason": "terminate_command"
  },
  "cleanupReceipt": {
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
  "helperExit": {
    "code": 0,
    "signal": null
  },
  "helperStderr": "",
  "helperStdoutClosed": true,
  "scriptUnloadSettled": true,
  "sessionDetached": true,
  "windowsAfter": {
    "pid": 40696,
    "present": false,
    "processes": []
  },
  "windowsAbsent": true,
  "fridaAfter": {
    "deviceId": "local",
    "matches": []
  },
  "fridaAbsent": true,
  "cleanupVerified": true,
  "processAbsent": true,
  "registrationVerified": true,
  "noCrashCoverage": true,
  "resumeVerified": true,
  "terminationPathVerified": true,
  "captureComplete": true,
  "finishedUtc": "2026-10-26T05:29:21.782Z",
  "outcome": "PASS"
}
```
