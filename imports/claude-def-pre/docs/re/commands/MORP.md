# MORP — write a morphing control file and run DEF_MORP2 / DEF_MORP3

Shared mechanics (dispatch, readers, launcher `FUN_140223790`): see [common.md](common.md).
Decompiles: `deform-rea-work/out/p89/morp.c`, dispatcher `out/bm_cmd.c` (`FUN_140378e00`).

## 1. Summary

| | |
|---|---|
| Registry id | 297 |
| Dispatcher | `FUN_140378e00`, `param_3 >= 0x11f`, `switch (param_3 - 0x11f)` case 10 |
| Handler | `FUN_1401aa750(model, m, &doubles, &strings)` |
| External program | 2D: `DEF_MORP2.EXE` (no arguments, dirKind 2); 3D: `DEF_MORP3.EXE morphing DEF_MORP3.INI` (dirKind 3) |
| Read-back | none |

The command copies its arguments into `DEF_MORP2.INI` (2D) or `DEF_MORP3.INI` (3D) and starts the
morphing program. Nothing in the model is read or changed; DEF_PRE does not read any output.
2D/3D is the **model** dimension: `FUN_1400015f0(model)` = `model+0x1c == 3`.

## 2. Argument grammar

```
MORP <n> <m>            rest of keyword line, FUN_140413ee0: "%i %i" (',' -> ' ')
<d_1>                   n lines, FUN_140414990: "%Flf" (one double per line)
...
<d_n>
<s_1>                   4n+2 lines, FUN_140413c60: whole line, '\n' cut, trailing ' ', '\r', '\n' removed,
...                     truncated to 1023 chars; leading spaces kept
<s_4n+2>
```

* `n` = number of doubles; `m` = an integer passed through to the INI file (its meaning belongs to
  DEF_MORP; not interpreted by DEF_PRE).
* The status of the `"%i %i"` reader is stored but never tested: on a short/empty line the missing
  values are 0 (n = 0 still reads 2 string lines).
* A double line or a string line at EOF → the case returns 2 immediately (lists destroyed, no file
  written). A double line that does not parse leaves 0.0 (the reader zeroes it first) and is not an
  error unless the line is empty (`sscanf` = -1 → 2).
* Doubles are pushed into a list (`FUN_140382510` ctor, `FUN_1403825a0` push_back), strings into a
  list of `std::string` (`FUN_14032f7d0` ctor, `FUN_14032f880` push_back); order = input order
  (inferred from push_back/index walk).
* Handler return value is ignored; the case returns the status of the last reader (0 normally).

KEY example (n = 1 → 6 string lines):

```
MORP 1 3
0.5
line1
line2
line3
line4
line5
line6
```

## 3. Model fields used

Only `model+0x1c` (dimension). No object lookup.

## 4. Files written

Name: 3D `DEF_MORP3.INI`, 2D `DEF_MORP2.INI`; `fopen(name, "wt")` (text mode → CRLF on disk).
If `fopen` fails the handler returns 0 and nothing is launched (no message).

| Line(s) | Format | Value |
|---|---|---|
| 1 | `"%d\n"` | m (second integer of the keyword line) |
| 2 | `"%d\n"` | list size = n |
| 3 .. n+2 | `"%lf\n"` | each double (so 6 decimals, e.g. `0.500000`) |
| next 4n+2 | `"%s\n"` | each string line verbatim (after trimming) |

The file is closed before the launch and never deleted by DEF_PRE.

## 5. Launch

`sprintf(buf, ...)` then:

* 2D: `"DEF_MORP2.EXE"` → `FUN_140224100(1, buf)` (dirKind 2, "2D\").
* 3D: `"DEF_MORP3.EXE morphing DEF_MORP3.INI"` → `FUN_140224130(1, buf)` (dirKind 3, "3D\").

Mode 1 → `system("\"\"<dir>DEF_MORP2.EXE\"\"")` / `system("\"\"<dir>DEF_MORP3.EXE\" morphing DEF_MORP3.INI\"")`
(see common.md for the program search). Synchronous; exit code ignored; no success check.
The 2D program gets no arguments, so it must find `DEF_MORP2.INI` itself (inferred: fixed name in
the current directory). The child inherits DEF_PRE's stdin/stdout.

Install check: `v11.0.2\2D\DEF_MORP2.EXE`, `v11.0.2\3D\DEF_MORP3.exe` exist.

## 6. Read-back

None.

## 7. Messages and error paths

No messages. Returns: case → 2 on EOF while reading doubles/strings; otherwise 0.
`FUN_1401aa750` returns 1 (launched) or 0 (INI not opened); ignored.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401aa750` | handler: write INI, launch |
| `FUN_1400015f0` | model is 3D (`+0x1c == 3`) |
| `FUN_1401b0790` / `FUN_1401b0760` / `FUN_1401b0810` | double list: size (`*list+0x10`), element i (walk i links from head, value at node+0x10) |
| `FUN_1401b0740` / `FUN_1401b0710` / `FUN_1401b07b0` | string list: size, element i |
| `FUN_140003010` | `std::string::c_str` (SSO: inline when capacity ≤ 15) |
| `FUN_140224100` / `FUN_140224130` | launcher wrappers dirKind 2 / 3 |

## 9. Open questions

* Meaning of m, of the doubles and of the 4n+2 strings (DEF_MORP input format) — not visible in
  DEF_PRE.
* Whether DEF_MORP2 reads stdin (it inherits DEF_PRE's stdin; in a scripted run it could consume
  the menu script).
