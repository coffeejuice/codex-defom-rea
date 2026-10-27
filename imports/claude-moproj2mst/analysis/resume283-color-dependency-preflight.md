# CP283 original color helper dependency preflight

## Result

**HOLD for execution; static dependency preflight passed.** The installed `DEF_GUI_MO_64.exe` hash matches the CP279/CP282 target. Its direct PE imports include `QtGui4_64.dll` and `QtCore4_64.dll`. The installed root QtGui DLL exports the three exact decorated `QColor` accessor names referenced by CP282. This establishes only static dependency availability. It does not establish which module path or version the loader used at runtime.

## Bound installed files

The installed executable is `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, size 67,611,136 bytes, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Windows file metadata exposes no file or product version, consistent with the retained CP279 qualification.

The exact root-level dependency paths are:

| DLL | Size | File version | SHA-256 |
| --- | ---: | --- | --- |
| `C:\Program Files\SFTC\DEFORM\v11.0.2\QtGui4_64.dll` | 10,859,008 | 4.8.4.0 | `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` |
| `C:\Program Files\SFTC\DEFORM\v11.0.2\QtCore4_64.dll` | 3,250,688 | 4.8.4.0 | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |

## Direct PE imports and export check

Parsed the 64-bit PE import directory of the exact installed executable. Its direct import DLL names are `QtCore4_64.dll`, `QtGui4_64.dll`, `QtXml4_64.dll`, `OLEAUT32.dll`, `QtNetwork4_64.dll`, `WS2_32.dll`, `QtCommercialChart_64.dll`, `QtOpenGL4_64.dll`, `QtSvg4_64.dll`, `QtSql4_64.dll`, `OPENGL32.dll`, `GLU32.dll`, `KERNEL32.dll`, `USER32.dll`, `COMDLG32.dll`, `ADVAPI32.dll`, and `ole32.dll`.

The CP282 color callsites bind these QtGui import names: `?redF@QColor@@QEBANXZ`, `?greenF@QColor@@QEBANXZ`, and `?blueF@QColor@@QEBANXZ`. Direct export-directory parsing of the installed root `QtGui4_64.dll` found all three names. The installed root `QtCore4_64.dll` is also present as a direct dependency; these three color accessors are QtGui exports, not QtCore exports.

There are separate 32-bit Qt DLLs under the `2D` and `3D` subdirectories. They are not the names imported by this x64 executable and were not substituted for the root x64 DLLs. Static import membership and file presence do not determine Windows loader search resolution or prove an actual loaded path/version.

## Scope and disposition

This was read-only. No application process, GUI, REA database, build, installed file, project, or source was opened or changed. Runtime loaded-module identity remains **unknown**. The original-run gate remains closed under CP279 and CP282; this receipt does not authorize execution.
