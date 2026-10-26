# Owned Python child Frida exception-handler preflight

Converter-only runtime preflight using a newly created, owned Python child. The child is created suspended, attached directly with Frida 17.23.2, and the exception handler is installed before one resume. Its fixed `ctypes.CFUNCTYPE(None)(0)()` call triggers an access violation. The callback records the first access violation and returns `false`, leaving natural exception handling in control. Passing cleanup requires natural child exit, exact helper handle closure, and absence from both Windows and Frida process views. No original converter, DEFORM application, GUI input, or existing PID is targeted.

The raw helper JSONL receipts and the bounded result are retained below. A passing result validates this isolated Frida callback mechanism only; it does not establish converter exception behavior.

```json
{
  "schema": "owned-python-exception-handler-preflight-v1",
  "startedUtc": "2026-10-26T05:11:43.211Z",
  "host": "Sim2",
  "target": {
    "python": "C:\\Program Files\\Python312\\python.exe",
    "cwd": "C:\\PROBLEM",
    "childCode": "import ctypes; ctypes.CFUNCTYPE(None)(0)()"
  },
  "guiInput": false,
  "guiControlAcquired": false,
  "interceptorHooks": 0,
  "maxSeconds": 20,
  "stages": [],
  "failures": [
    {
      "stage": "preflight",
      "name": "Error",
      "message": "EPERM: operation not permitted, open 'C:\\Users\\alext\\AppData\\Roaming\\npm\\node_modules\\rea-agents\\node_modules\\frida\\package.json'"
    }
  ],
  "helperRawReceipts": [],
  "scriptMessages": [],
  "pythonSha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a",
  "helperSha256": "8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d",
  "helperStderr": "",
  "helperStdoutClosed": false,
  "scriptUnloadSettled": true,
  "sessionDetached": true,
  "windowsAbsent": false,
  "fridaAbsent": false,
  "cleanupVerified": false,
  "processAbsent": false,
  "exceptionRecordQualified": false,
  "captureComplete": false,
  "finishedUtc": "2026-10-26T05:11:43.216Z",
  "outcome": "INCOMPLETE"
}
```

## Execution gate

The runner stopped before launching the helper because the sandbox denied reading the installed Frida package manifest (`EPERM`). A request to run this exact bounded command with elevated permissions was rejected by automatic review: it classified the intentional child crash as lacking user authorization and instructed that the rejection not be bypassed. No helper or target child was launched. Consequently, this attempt did not verify Windows process absence, Frida process absence, exception payloads, natural exit, or retained-handle cleanup; the corresponding fields remain false or empty above.
