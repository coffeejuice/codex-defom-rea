# Owned Python child Frida exception-handler preflight

Converter-only runtime preflight using a newly created, owned Python child. The child is created suspended, attached directly with Frida 17.23.2, and the exception handler is installed before one resume. Its fixed `ctypes.CFUNCTYPE(None)(0)()` call triggers an access violation. The callback records the first access violation and returns `false`, leaving natural exception handling in control. Passing cleanup requires natural child exit, exact helper handle closure, and absence from both Windows and Frida process views. No original converter, DEFORM application, GUI input, or existing PID is targeted.

The raw helper JSONL receipts and the bounded result are retained below. A passing result validates this isolated Frida callback mechanism only; it does not establish converter exception behavior.

```json
{
  "schema": "owned-python-exception-handler-preflight-v1",
  "startedUtc": "2026-10-26T08:51:11.981Z",
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
  "stages": [
    {
      "name": "frida-local-device",
      "utc": "2026-10-26T08:51:12.553Z"
    },
    {
      "name": "helper-started",
      "utc": "2026-10-26T08:51:12.562Z"
    },
    {
      "name": "owned-python-child-created-suspended",
      "utc": "2026-10-26T08:51:12.638Z"
    },
    {
      "name": "frida-attached-before-resume",
      "utc": "2026-10-26T08:51:12.759Z"
    },
    {
      "name": "handler-installed-before-resume",
      "utc": "2026-10-26T08:51:12.761Z"
    },
    {
      "name": "resumed-once",
      "utc": "2026-10-26T08:51:12.786Z"
    },
    {
      "name": "exception-observed-or-error",
      "utc": "2026-10-26T08:51:12.815Z"
    },
    {
      "name": "helper-closed-after-natural-exit-window",
      "utc": "2026-10-26T08:51:12.870Z"
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
          "import ctypes; ctypes.CFUNCTYPE(None)(0)()"
        ],
        "canonical_image": "c:\\program files\\python312\\python.exe",
        "creation_filetime": 134374782726332080,
        "cwd": "c:\\problem",
        "host": "Sim2",
        "launcher_pid": 24648,
        "pid": 30744,
        "primary_tid": 27428,
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
      "reason": "target_exited"
    },
    {
      "event": "cleanup",
      "exit_code": 1,
      "exit_verified": true,
      "handles_closed": true,
      "state": "exited",
      "termination_attempts": [],
      "termination_requested": false,
      "wait_result": 0
    }
  ],
  "scriptMessages": [
    {
      "type": "send",
      "payload": {
        "kind": "handler-ready",
        "pid": 30744,
        "arch": "x64",
        "platform": "windows",
        "registrationCallCompleted": true
      }
    },
    {
      "type": "send",
      "payload": {
        "kind": "first-access-violation",
        "pid": 30744,
        "exceptionType": "access-violation",
        "exceptionAddress": {
          "address": "0x0",
          "symbol": "0",
          "module": null
        },
        "rip": {
          "address": "0x0",
          "symbol": "0",
          "module": null
        },
        "faultingInstruction": {
          "address": "0x0",
          "symbol": "0",
          "module": null
        },
        "registers": {
          "rip": "0x0",
          "rsp": "0x24061ef428",
          "rbp": "0x24061ef450",
          "rax": "0x7ffb543c4493",
          "rbx": "0x20",
          "rcx": "0x1d9044645e0",
          "rdx": "0x7ffb555e8bc9",
          "rsi": "0x0",
          "rdi": "0x0",
          "r8": "0x1d9044645e8",
          "r9": "0xffffffffffffff0",
          "r10": "0x0",
          "r11": "0x24061ef430",
          "r12": "0x24061ef690",
          "r13": "0x24061ef690",
          "r14": "0x24061ef690",
          "r15": "0x24061ef600"
        },
        "accurateBacktrace": [
          {
            "address": "0x7ffb543c42c2",
            "symbol": "0x7ffb543c42c2 python.exe!ffi_call",
            "module": {
              "name": "libffi-8.dll",
              "path": "c:\\program files\\python312\\DLLs\\libffi-8.dll",
              "base": "0x7ffb543c0000",
              "size": 45056,
              "offset": "0x42c2"
            }
          },
          {
            "address": "0x7ffb00d73e56",
            "symbol": "0x7ffb00d73e56",
            "module": {
              "name": "_ctypes.pyd",
              "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
              "base": "0x7ffb00d70000",
              "size": 131072,
              "offset": "0x3e56"
            }
          },
          {
            "address": "0x7ffb00d72c1b",
            "symbol": "0x7ffb00d72c1b",
            "module": {
              "name": "_ctypes.pyd",
              "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
              "base": "0x7ffb00d70000",
              "size": 131072,
              "offset": "0x2c1b"
            }
          },
          {
            "address": "0x7ffb00d727f8",
            "symbol": "0x7ffb00d727f8",
            "module": {
              "name": "_ctypes.pyd",
              "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
              "base": "0x7ffb00d70000",
              "size": 131072,
              "offset": "0x27f8"
            }
          },
          {
            "address": "0x7ffb55522018",
            "symbol": "0x7ffb55522018 python.exe!PyObject_Vectorcall",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x82018"
            }
          },
          {
            "address": "0x7ffb555219c5",
            "symbol": "0x7ffb555219c5 python.exe!PyObject_Vectorcall",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x819c5"
            }
          },
          {
            "address": "0x7ffb55522ea5",
            "symbol": "0x7ffb55522ea5 python.exe!PyEval_EvalFrameDefault",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x82ea5"
            }
          },
          {
            "address": "0x7ffb554d81e4",
            "symbol": "0x7ffb554d81e4 python.exe!PyObject_ClearManagedDict",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x381e4"
            }
          },
          {
            "address": "0x7ffb554d6b66",
            "symbol": "0x7ffb554d6b66 python.exe!PyEval_EvalCode",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x36b66"
            }
          },
          {
            "address": "0x7ffb5558e838",
            "symbol": "0x7ffb5558e838 python.exe!Py_SourceAsString",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0xee838"
            }
          },
          {
            "address": "0x7ffb5558ea70",
            "symbol": "0x7ffb5558ea70 python.exe!PyModule_ClearDict",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0xeea70"
            }
          },
          {
            "address": "0x7ffb5558ebdf",
            "symbol": "0x7ffb5558ebdf python.exe!PyRun_StringFlags",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0xeebdf"
            }
          },
          {
            "address": "0x7ffb55628871",
            "symbol": "0x7ffb55628871 python.exe!PyRun_SimpleStringFlags",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x188871"
            }
          },
          {
            "address": "0x7ffb556287ff",
            "symbol": "0x7ffb556287ff python.exe!PyThread_tss_set",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x1887ff"
            }
          },
          {
            "address": "0x7ffb554c8ee2",
            "symbol": "0x7ffb554c8ee2 python.exe!PyMem_GetAllocator",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x28ee2"
            }
          },
          {
            "address": "0x7ffb554c8700",
            "symbol": "0x7ffb554c8700 python.exe!Py_RunMain",
            "module": {
              "name": "python312.dll",
              "path": "c:\\program files\\python312\\python312.dll",
              "base": "0x7ffb554a0000",
              "size": 6991872,
              "offset": "0x28700"
            }
          }
        ],
        "memoryOperation": {
          "operation": "execute",
          "address": "0x0"
        },
        "disposition": "return_false_continue_natural_exception"
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
    "import ctypes; ctypes.CFUNCTYPE(None)(0)()"
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
        "import ctypes; ctypes.CFUNCTYPE(None)(0)()"
      ],
      "canonical_image": "c:\\program files\\python312\\python.exe",
      "creation_filetime": 134374782726332080,
      "cwd": "c:\\problem",
      "host": "Sim2",
      "launcher_pid": 24648,
      "pid": 30744,
      "primary_tid": 27428,
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
      "import ctypes; ctypes.CFUNCTYPE(None)(0)()"
    ],
    "canonical_image": "c:\\program files\\python312\\python.exe",
    "creation_filetime": 134374782726332080,
    "cwd": "c:\\problem",
    "host": "Sim2",
    "launcher_pid": 24648,
    "pid": 30744,
    "primary_tid": 27428,
    "sha256": "4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a"
  },
  "handlerReady": {
    "kind": "handler-ready",
    "pid": 30744,
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
  "exceptionEvent": {
    "kind": "first-access-violation",
    "pid": 30744,
    "exceptionType": "access-violation",
    "exceptionAddress": {
      "address": "0x0",
      "symbol": "0",
      "module": null
    },
    "rip": {
      "address": "0x0",
      "symbol": "0",
      "module": null
    },
    "faultingInstruction": {
      "address": "0x0",
      "symbol": "0",
      "module": null
    },
    "registers": {
      "rip": "0x0",
      "rsp": "0x24061ef428",
      "rbp": "0x24061ef450",
      "rax": "0x7ffb543c4493",
      "rbx": "0x20",
      "rcx": "0x1d9044645e0",
      "rdx": "0x7ffb555e8bc9",
      "rsi": "0x0",
      "rdi": "0x0",
      "r8": "0x1d9044645e8",
      "r9": "0xffffffffffffff0",
      "r10": "0x0",
      "r11": "0x24061ef430",
      "r12": "0x24061ef690",
      "r13": "0x24061ef690",
      "r14": "0x24061ef690",
      "r15": "0x24061ef600"
    },
    "accurateBacktrace": [
      {
        "address": "0x7ffb543c42c2",
        "symbol": "0x7ffb543c42c2 python.exe!ffi_call",
        "module": {
          "name": "libffi-8.dll",
          "path": "c:\\program files\\python312\\DLLs\\libffi-8.dll",
          "base": "0x7ffb543c0000",
          "size": 45056,
          "offset": "0x42c2"
        }
      },
      {
        "address": "0x7ffb00d73e56",
        "symbol": "0x7ffb00d73e56",
        "module": {
          "name": "_ctypes.pyd",
          "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
          "base": "0x7ffb00d70000",
          "size": 131072,
          "offset": "0x3e56"
        }
      },
      {
        "address": "0x7ffb00d72c1b",
        "symbol": "0x7ffb00d72c1b",
        "module": {
          "name": "_ctypes.pyd",
          "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
          "base": "0x7ffb00d70000",
          "size": 131072,
          "offset": "0x2c1b"
        }
      },
      {
        "address": "0x7ffb00d727f8",
        "symbol": "0x7ffb00d727f8",
        "module": {
          "name": "_ctypes.pyd",
          "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
          "base": "0x7ffb00d70000",
          "size": 131072,
          "offset": "0x27f8"
        }
      },
      {
        "address": "0x7ffb55522018",
        "symbol": "0x7ffb55522018 python.exe!PyObject_Vectorcall",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x82018"
        }
      },
      {
        "address": "0x7ffb555219c5",
        "symbol": "0x7ffb555219c5 python.exe!PyObject_Vectorcall",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x819c5"
        }
      },
      {
        "address": "0x7ffb55522ea5",
        "symbol": "0x7ffb55522ea5 python.exe!PyEval_EvalFrameDefault",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x82ea5"
        }
      },
      {
        "address": "0x7ffb554d81e4",
        "symbol": "0x7ffb554d81e4 python.exe!PyObject_ClearManagedDict",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x381e4"
        }
      },
      {
        "address": "0x7ffb554d6b66",
        "symbol": "0x7ffb554d6b66 python.exe!PyEval_EvalCode",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x36b66"
        }
      },
      {
        "address": "0x7ffb5558e838",
        "symbol": "0x7ffb5558e838 python.exe!Py_SourceAsString",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0xee838"
        }
      },
      {
        "address": "0x7ffb5558ea70",
        "symbol": "0x7ffb5558ea70 python.exe!PyModule_ClearDict",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0xeea70"
        }
      },
      {
        "address": "0x7ffb5558ebdf",
        "symbol": "0x7ffb5558ebdf python.exe!PyRun_StringFlags",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0xeebdf"
        }
      },
      {
        "address": "0x7ffb55628871",
        "symbol": "0x7ffb55628871 python.exe!PyRun_SimpleStringFlags",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x188871"
        }
      },
      {
        "address": "0x7ffb556287ff",
        "symbol": "0x7ffb556287ff python.exe!PyThread_tss_set",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x1887ff"
        }
      },
      {
        "address": "0x7ffb554c8ee2",
        "symbol": "0x7ffb554c8ee2 python.exe!PyMem_GetAllocator",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x28ee2"
        }
      },
      {
        "address": "0x7ffb554c8700",
        "symbol": "0x7ffb554c8700 python.exe!Py_RunMain",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x28700"
        }
      }
    ],
    "memoryOperation": {
      "operation": "execute",
      "address": "0x0"
    },
    "disposition": "return_false_continue_natural_exception"
  },
  "observation": {
    "kind": "first-access-violation",
    "pid": 30744,
    "exceptionType": "access-violation",
    "exceptionAddress": {
      "address": "0x0",
      "symbol": "0",
      "module": null
    },
    "rip": {
      "address": "0x0",
      "symbol": "0",
      "module": null
    },
    "faultingInstruction": {
      "address": "0x0",
      "symbol": "0",
      "module": null
    },
    "registers": {
      "rip": "0x0",
      "rsp": "0x24061ef428",
      "rbp": "0x24061ef450",
      "rax": "0x7ffb543c4493",
      "rbx": "0x20",
      "rcx": "0x1d9044645e0",
      "rdx": "0x7ffb555e8bc9",
      "rsi": "0x0",
      "rdi": "0x0",
      "r8": "0x1d9044645e8",
      "r9": "0xffffffffffffff0",
      "r10": "0x0",
      "r11": "0x24061ef430",
      "r12": "0x24061ef690",
      "r13": "0x24061ef690",
      "r14": "0x24061ef690",
      "r15": "0x24061ef600"
    },
    "accurateBacktrace": [
      {
        "address": "0x7ffb543c42c2",
        "symbol": "0x7ffb543c42c2 python.exe!ffi_call",
        "module": {
          "name": "libffi-8.dll",
          "path": "c:\\program files\\python312\\DLLs\\libffi-8.dll",
          "base": "0x7ffb543c0000",
          "size": 45056,
          "offset": "0x42c2"
        }
      },
      {
        "address": "0x7ffb00d73e56",
        "symbol": "0x7ffb00d73e56",
        "module": {
          "name": "_ctypes.pyd",
          "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
          "base": "0x7ffb00d70000",
          "size": 131072,
          "offset": "0x3e56"
        }
      },
      {
        "address": "0x7ffb00d72c1b",
        "symbol": "0x7ffb00d72c1b",
        "module": {
          "name": "_ctypes.pyd",
          "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
          "base": "0x7ffb00d70000",
          "size": 131072,
          "offset": "0x2c1b"
        }
      },
      {
        "address": "0x7ffb00d727f8",
        "symbol": "0x7ffb00d727f8",
        "module": {
          "name": "_ctypes.pyd",
          "path": "c:\\program files\\python312\\DLLs\\_ctypes.pyd",
          "base": "0x7ffb00d70000",
          "size": 131072,
          "offset": "0x27f8"
        }
      },
      {
        "address": "0x7ffb55522018",
        "symbol": "0x7ffb55522018 python.exe!PyObject_Vectorcall",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x82018"
        }
      },
      {
        "address": "0x7ffb555219c5",
        "symbol": "0x7ffb555219c5 python.exe!PyObject_Vectorcall",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x819c5"
        }
      },
      {
        "address": "0x7ffb55522ea5",
        "symbol": "0x7ffb55522ea5 python.exe!PyEval_EvalFrameDefault",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x82ea5"
        }
      },
      {
        "address": "0x7ffb554d81e4",
        "symbol": "0x7ffb554d81e4 python.exe!PyObject_ClearManagedDict",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x381e4"
        }
      },
      {
        "address": "0x7ffb554d6b66",
        "symbol": "0x7ffb554d6b66 python.exe!PyEval_EvalCode",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x36b66"
        }
      },
      {
        "address": "0x7ffb5558e838",
        "symbol": "0x7ffb5558e838 python.exe!Py_SourceAsString",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0xee838"
        }
      },
      {
        "address": "0x7ffb5558ea70",
        "symbol": "0x7ffb5558ea70 python.exe!PyModule_ClearDict",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0xeea70"
        }
      },
      {
        "address": "0x7ffb5558ebdf",
        "symbol": "0x7ffb5558ebdf python.exe!PyRun_StringFlags",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0xeebdf"
        }
      },
      {
        "address": "0x7ffb55628871",
        "symbol": "0x7ffb55628871 python.exe!PyRun_SimpleStringFlags",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x188871"
        }
      },
      {
        "address": "0x7ffb556287ff",
        "symbol": "0x7ffb556287ff python.exe!PyThread_tss_set",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x1887ff"
        }
      },
      {
        "address": "0x7ffb554c8ee2",
        "symbol": "0x7ffb554c8ee2 python.exe!PyMem_GetAllocator",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x28ee2"
        }
      },
      {
        "address": "0x7ffb554c8700",
        "symbol": "0x7ffb554c8700 python.exe!Py_RunMain",
        "module": {
          "name": "python312.dll",
          "path": "c:\\program files\\python312\\python312.dll",
          "base": "0x7ffb554a0000",
          "size": 6991872,
          "offset": "0x28700"
        }
      }
    ],
    "memoryOperation": {
      "operation": "execute",
      "address": "0x0"
    },
    "disposition": "return_false_continue_natural_exception"
  },
  "cleanupReceipt": {
    "event": "cleanup",
    "exit_code": 1,
    "exit_verified": true,
    "handles_closed": true,
    "state": "exited",
    "termination_attempts": [],
    "termination_requested": false,
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
    "pid": 30744,
    "present": false,
    "processes": [],
    "ownedCreationFiletime": "134374782726332080",
    "pidReuseObserved": false,
    "ownedInstancePresent": false
  },
  "windowsAbsent": true,
  "fridaAfter": {
    "deviceId": "local",
    "matches": []
  },
  "fridaAbsent": true,
  "naturalExitVerified": false,
  "cleanupByTermination": false,
  "cleanupVerified": false,
  "processAbsent": true,
  "captureComplete": false,
  "finishedUtc": "2026-10-26T08:51:13.154Z",
  "outcome": "INCOMPLETE"
}
```
