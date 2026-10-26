# Command keywords: shared mechanics (Phases 8–9)

Facts used by every spec in this folder. Addresses are DEF_PRE_64.exe v11.0.2 (image base
0x140000000). Decompiles: `deform-rea-work/out/p89/*.c` (Ghidra copy `ghidra_p89*`, scripts in
`deform-rea-work/scripts_p89`).

## Dispatch

The KEY interpreter `FUN_1401b6520` reads a keyword name with `fscanf("%39s")`, looks it up in
the registry and calls the group-6 handler **`FUN_140378e00(model, FILE*, id)`**. The handler
reads the rest of the keyword line (and any following lines) from the same `FILE*`, so a
command's "argument grammar" is: the remainder of the keyword line, then N more lines.
Return 0 = ok, 2 = "Reading Error" (the reader prints it), any other non-zero likewise.
Every case ends with `return iVar4 == 0 ? 0 : 2`.

| Registry id | Keyword | Switch case | Handler (after argument parsing) |
|---:|---|---|---|
| 11 | EXECMD | `case 0xb` | inline, `FUN_140223790` family |
| 15 | GENINT | `case 0xf` | `FUN_1401ac020(model, file)` |
| 16 | IMPLUB | `case 0x10` | `FUN_1401ac310(model, file, ids, flag)` |
| 17 | IMPEQP | `case 0x11` | `FUN_1401ac680(model, file, id)` |
| 21 | ARMCTL | `case 0x15` | inline (writes `DEF_ARM_CTL.DAT`) |
| 287 | CNVT3D | `0x11f+0` | `FUN_1401467f0(cvt, model+0x1748, model, ids, n)` |
| 291 | REMESH | `0x11f+4` | `FUN_1401a9a10(model, &obj)` |
| 292 | BRDEXT | `0x11f+5` | `FUN_1401a9ac0(model, &obj)` |
| 293 | DEFAMG | `0x11f+6` | `FUN_1401a9be0(model, &obj)` |
| 294 | DEFINT | `0x11f+7` | `FUN_1401aa000(model, &obj)` |
| 296 | DEFFRZ | `0x11f+9` | `FUN_1401aa370(model, &a, &b, &d)` |
| 297 | MORP | `0x11f+10` | `FUN_1401aa750(model, n, doubles, strings)` |
| 298 | GFREAD | `0x11f+0xb` | `FUN_1401aa9a0(model, &obj, filename)` |

## Argument readers (all take the KEY `FILE*`)

All numeric readers `fgets` one line (1024 bytes), replace every `,` with a space
(`FUN_140323970(line, ',', ' ')`) and `sscanf` it; they return 1 only on EOF or when `sscanf`
returns -1 (empty line), otherwise 0 even if fewer fields matched (unmatched fields stay 0).

| Function | Reads | Format |
|---|---|---|
| `FUN_140413de0(f, int*, skipBlank)` | rest of line | `%i` (so `0x10` and `010` are hex/octal) ; when `skipBlank` and the line is just `\n`, reads the next line |
| `FUN_140413ee0(f, int*, int*)` | rest of line | `%i %i` |
| `FUN_140413fb0(f, int*, int*, int*, int* nread)` | rest of line | `%i %i %i` |
| `FUN_140415f00(f, int*, int*, double*)` | rest of line | `%i %i %Flf` |
| `FUN_1404154c0(f, int*, double*)` | rest of line | int + double (used by GENSUB/DSTROKE; not decompiled here) |
| `FUN_140414990(f, double*)` | rest of line | `%Flf` |
| `FUN_140413900(f, int* n, max)` | rest of line | `strtok(" ,\n\t\r")` into a global token table (max 128); `FUN_140413a50(i)` returns token i (or `""`) |
| `FUN_140413ab0(f, buf, n)` | **two** lines: discards the rest of the keyword line, then returns the next line, truncated to n-1 chars, trailing spaces/CR/LF removed |
| `FUN_140413c60(f, buf, n)` | one line, trailing spaces/CR/LF removed |
| `fgets` + `FUN_140323c50` | one line, trailing spaces/CR/LF removed (leading spaces kept) |

`FUN_1401d6ea0(model, id)` finds object `id` in the object list at `model+0x1380`
(`FUN_14017aaf0`); 0 → the case returns 2. `FUN_14019ccc0(model, id)` (GFREAD) finds or
**creates** objects up to `id` (new ones get type 7).
`FUN_14047b934` is `fopen`. `FUN_1401b85d0(name)` deletes a file; `FUN_1401b8560(name)` tests
whether a file can be opened for reading.

## Launching external programs: `FUN_140223790(mode, cmdline, dirKind)`

Wrappers: `FUN_1402240d0` (dirKind 0), `FUN_140224100` (2 = "2D\"), `FUN_140224130`
(3 = "3D\"), `FUN_140224160` (4 = try "..\" too). The first argument is 1 in every caller found
so far (system path).

1. `base = getenv("DEFORM_PC_PATH")`, with a `\` appended if missing (empty if unset).
   **DEF_PRE always overwrites this variable at start-up**: `FUN_1401b8e10` (called from `main`)
   `_putenv`s `HOME`, `USER`, `DEFORM_DIR`, `DEFORM2_DIR`, `DEFORM3_DIR`, `DEFORM23_DIR`,
   `DEFORM_USER`, `DEF_HOME_DIR` and `DEFORM_PC_PATH` = the folder of the running executable
   (`GetModuleFileNameA`, cut at the last `\`). So the programs come from the folder DEF_PRE
   itself lives in: `<install>\DEF_PRE_64.exe` runs `<install>D\...`; a copy in `2D\` runs
   `2D\..D\...`. The port must take this folder from configuration (the DEFORM install), not
   from its own location, and set the same nine variables for the child programs.
   `dir = base`; if `base` ends in `2D\`/`3D\`/`2d\`/`3d\` (char before the last `\` is `d`/`D`
   and the one before is `2`/`3`), dirKind 2 appends `..\2D\`, dirKind 3 `..\3D\`; otherwise
   dirKind 2 appends `2D\`, 3 appends `3D\`.
2. The command line is copied, trailing spaces/CR/LF removed, and split by
   `FUN_140222e20(line, " ,\t\n", argv, 21)` (quotes `'`/`"` group; at most 21 words; commas
   separate words too).
3. If `argv[0]` is not absolute (length < 2, or first char not `\` and second not `:`), the
   program is searched as `dir+argv0`, `dir+argv0+".EXE"`, `dir+argv0+".COM"` (each tested with
   `fopen(...,"r")`); dirKind 4 with a `2D\`/`3D\` base additionally tries `dir+"..\"+argv0`
   unchanged. If none opens, `argv[0] = base + argv0` (no extension).
   Note: when a file is found the **un-suffixed** `dir+argv0` is used as `argv[0]` (the `.EXE`/
   `.COM` probes only decide that it exists; `cmd.exe` then resolves the extension).
4. Mode 1: `system("\"" + "\"" + argv0 + "\"" + (" " + argv[i])... + "\"")`, i.e.
   `cmd /c ""C:\...\DEF_AMG" arg arg"`. `system` waits; its return value is ignored.
   Redirections such as `< DEF_AMG.INI > DEF_AMG.MSG` are ordinary words, so `cmd` performs them.
5. Other modes (not used by the command keywords): `<`/`>` words are applied with `freopen` on
   stdin/stdout, `CreateProcessA(argv0, "\"argv0\" args", ..., NORMAL_PRIORITY_CLASS (0x20),
   cwd NULL)`, **no wait**, stdin/stdout reopened on `CON` afterwards.

The working directory is always DEF_PRE's own (no `chdir`); every input and output file named
below is relative to it.

For the port: launching through `cmd /c` with the same quoting keeps redirection and extension
resolution identical. The port must reproduce the search order exactly because `DEFORM_PC_PATH`
decides which build (2D or 3D folder) runs.

## Notes added from the command specs

* The dispatcher ignores the return value of the REMESH/BRDEXT/DEFAMG/DEFINT/DEFFRZ handlers:
  those cases return 0 unless an argument read fails or the object is missing.
* Model fields used by several commands: `model+0x340` PROBID (char[0x400]), `model+0x744`
  DB step, `model+0x1748` DB file name (set by DBREAD).
* KEY reader `FUN_1401b5f00(model, file, flag, mode)` stores `mode` in `model+0x1740` while
  reading: bit 1 skip DBREAD, 2 skip MESHNO, 4 skip NSTART, 8 keep PROBID. With `flag` set,
  `FUN_1401d7f10` runs before the read and `FUN_1401d7f40` after it.
* Several cases never store their reader's status (EXECMD `%i`, IMPLUB `%i %i %i`, IMPEQP `%i`,
  GENINT's `FUN_140413ab0`, MORP's `%i %i` line): missing fields become 0 and only EOF on a
  following `fgets` returns 2.
* CRT helpers: `FUN_140484f90` = `strcpy`, `FUN_140484ef0` = `strcat`, `FUN_14047b6c0` = `atol`,
  `0x14047e29c` = `remove` (Ghidra shows it as `FID_conflict__wrmdir`).
* `FUN_140224130` (dirKind 3) is also called from GFREAD (`FUN_14024c290` PDA2KEY3,
  `FUN_14024d8b0` STB2KEY3). `FUN_14019ccc0` (find-or-create object) is used by GFREAD and IMPEQP.
* `FUN_1401b85d0(name)` tests the file with `fopen` and then calls `remove`. `FUN_14047903c()+0x30`
  is `stdout`. `FUN_1400b9130(name, text)` writes a small marker file: the mesher and
  interpolation launchers write `DEFORM_UIRUNREMESH.DAT` ("2D"/"3D") before a launch and delete
  it afterwards.
* The external programs are run with DEF_PRE's environment after `FUN_1401b8e10`, so `HOME`,
  `USER`, `DEFORM_DIR` ... all point at DEF_PRE's folder in the child too.
* `ACTOUT` (registry 286) is handled before the switch (`param_3 == 0x11e`): one ignored int on
  the keyword line, then the next line is stored at `model+0x140`; CNVT3D writes its 3D KEY there.
* `FUN_14047b6c0` is decimal `strtol`-style `atol`. `FUN_1401b0940(s, p, 0)` is a
  case-insensitive prefix compare; a non-zero third argument makes it exact.
* dirKind 0 adds no subfolder, so `DEF_CVT3` (only `3D\DEF_CVT3.com` in the install) is not found
  when DEF_PRE_64.exe runs from the install root; `cmd` then reports it as not recognised.
