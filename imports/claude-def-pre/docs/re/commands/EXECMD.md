# EXECMD — run a command line

Shared mechanics: [common.md](common.md) (launcher `FUN_140223790`). Code: inline in
`FUN_140378e00` case 0xb (`out/bm_cmd.c`); launcher decompile `out/p89/execmd.c`.

## 1. Summary

| | |
|---|---|
| Registry id | 11 |
| Dispatcher | `FUN_140378e00` case 0xb, inline |
| External program | whatever the line names |

## 2. Argument grammar

```
EXECMD <mode>       rest of keyword line, FUN_140413de0(f, &mode, 0): "%i" (mode preset 0)
<command line>      fgets(1024), not trimmed
```

* The `%i` reader status is not stored: empty/garbage → mode 0.
* Command line at EOF → case returns 2. Otherwise 0.
* `FUN_140484f90(cmd, line)` is CRT `strcpy` (byte copy to NUL, verified in assembly), so `cmd`
  keeps the trailing `'\n'` from `fgets` (and any leading/trailing spaces).

## 3. Dispatch

| mode | Call | Effect |
|---|---|---|
| 1 | `system(cmd)` | `cmd.exe /c <line>\n` exactly as written (no program search, no quoting); the trailing newline ends the command (inferred harmless) |
| 2 | `FUN_140224100(1, cmd)` | `FUN_140223790(1, cmd, 2)`: program searched in `%DEFORM_PC_PATH%` + `2D\` (or `..\2D\`) |
| 3 | `FUN_140224130(1, cmd)` | dirKind 3, `3D\` |
| 4 | `FUN_140224160(1, cmd)` | dirKind 4: base dir, plus `..\` retry when the base ends in `2D\`/`3D\` |
| other (0, 5, …) | `FUN_1402240d0(1, cmd)` | dirKind 0: base dir only |

Modes ≠ 1 go through common.md "Launching external programs": trailing spaces/CR/LF removed, split
on `" ,\t\n"` (quotes group; max 21 words — extra words are dropped; commas become word breaks,
i.e. spaces in the rebuilt line), first word searched (`<dir>w`, `<dir>w.EXE`, `<dir>w.COM`;
absolute when it starts with `\` or has `:` second), then
`system("\"\"" + argv0 + "\"" + " " + w1 + … + "\"")`. Synchronous, return value ignored. No
output file is checked. Working directory = DEF_PRE's.

Not found and not absolute → `argv0 = base + word` with no extension (so with `DEFORM_PC_PATH`
unset, a plain `cmd` stays `cmd` and is found on `PATH` by cmd.exe; with it set, `C:\…\cmd` fails).

## 4–6. Files / read-back

None.

## 7. Messages

None from DEF_PRE (the child's output goes to the inherited stdout).

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_140413de0` | `%i` line reader |
| `FUN_140484f90` | `strcpy` |
| `FUN_1402240d0` / `FUN_140224100` / `FUN_140224130` / `FUN_140224160` | launcher, dirKind 0 / 2 / 3 / 4 |
| `FUN_140223790` | launcher (common.md) |

## 9. Open questions

None for DEF_PRE's side.
