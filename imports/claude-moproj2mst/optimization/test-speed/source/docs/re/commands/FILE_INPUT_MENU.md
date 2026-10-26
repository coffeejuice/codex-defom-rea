# Main menu 2 "File Input": KEYWORD Input Menu and DEF_CVT3 imports

Decompiles: `deform-rea-work/out/p89/fileinp.c` (`FUN_140014ba0`, `FUN_14000b170`, depth 2),
`out/bm_main.c` (main menu), `out/p89/menu9.c` (selection reader `FUN_140007d30`),
assembly `out/p89/d/dis9.txt`. Launcher: [common.md](common.md).

## 1. Summary

| | |
|---|---|
| Entry | main menu `"   2 - File Input\n"` → `case '2'` in the main loop (`bm_main.c`): sets menu fields `+0x178 = main+0x190` (model, `_DB_STEP_DATA`), `+0x180 = main+0x3598` (DB object) and calls `FUN_140014ba0(menu)` |
| File menu | `FUN_140014ba0(m)`; `m+8` = model, `m+0x10` = DB object |
| Display + selection | `FUN_14000bac0(m, out)` |
| Option 1 | inline in `FUN_140014ba0` (KEY file → `FUN_1401b5f00`) |
| Option 2 | `FUN_14000b170(m)` (DB step → `FUN_1401cc2a0`) |
| Options 3..8 | `system`-launch of `DEF_CVT3 KEY <fmt>` (`FUN_1402240d0`, dirKind 0), then read `SCRATCH.KEY` |

No option of this menu reaches the in-process DXF/IGES readers; those are reached only from the
`GFREAD` keyword and from geometry primitives / CNVT3D ([DXF_IGES_READERS.md](DXF_IGES_READERS.md)).

## 2. Line reading

All answers come from `std::cin` (`PTR_DAT_14063a030`):

* `FUN_140009c50(m, out)` (free-text answers): `fflush(stdout)`, getline up to 256 chars
  (`FUN_140007460`, delimiter `'\n'`), then `FUN_1401b1060` trims leading and trailing
  `\t \n \v \f \r ' '` **and `'?'`** (a string made only of these characters is left unchanged),
  then: first char `' '` → `""`; ends with `"\n"` (`FUN_1401b0cc0`) → last char removed.
* `FUN_140007d30(m, out, 0, 0)` (menu selection): loop: `printf(" Enter Selection (%d-%d, R,
  E, /): ", lo, hi)` with `lo = m[0]`, `hi = m[1]` (ints at the start of the menu object; values
  not visible here), `fflush`, getline (256). The raw line is validated by `FUN_140006420`:
  accepted when it is `"e"`/`"E"`/`"r"`/`"R"`/`"/"` (exact), when `atoi(line) == 0` (any
  non-number, including the empty line), or when `lo <= atoi(line) <= hi`. Accepted → returned
  after the trim of `FUN_1401b1060`. Rejected → if the answer string is still empty (not
  possible after a non-empty read) `"\n Enter:  %d - %d  to Select or Modify\n"`,
  `"         E to Exit\n"`, `"         / to Exit to Main Menu\n"`,
  `"         R to Redisplay the Menu\n\n"`; else `"\n"`, `"??? Invalid Input\n"`, `"\n"`; and
  the prompt is repeated (menu not redisplayed).
* `"\n Hit <CR> to continue ..."` (`FUN_140009e90`): `fflush`, reads and discards one line.
* Yes/no answers use `FUN_1401b0940(ans, "n", 0)`: true when the first character of the answer,
  lower-cased, is `n` (case-insensitive compare of `strlen("n") = 1` chars).

## 3. Menu text (`FUN_14000bac0`, verbatim, to stdout)

```
"\n\n"
" INPUT PREP. -\n"
"-----------------------------------------------------\n"
" [%s]\n\n"                                   model+0x340 (problem name)
" ***  KEYWORD Input Menu  *** \n\n"
"     1 - Input KEYWORD File\n"
"     2 - Input DEFORM Data Base\n"
"     3 - Input IGES File\n"
"     4 - Input ANSYS/PREP7 File\n"
"     5 - Input STL File\n"
"     6 - Input PATRAN Neutral File\n"
"     7 - Input IDEAS Universal File\n"
"     8 - Input UGII Neutral File\n\n"
```
then the selection prompt of §2.

## 4. Dispatch (`FUN_140014ba0`)

The menu is redisplayed on every pass. Only the **first character** of the accepted answer is
used (`strncpy(c, ans, 1)`):

| First char | Action |
|---|---|
| `1` | KEY file (§5), then back to the menu |
| `2` | `FUN_14000b170` (§6), then back to the menu |
| `3` | `DEF_CVT3 KEY IGE` (§7) |
| `4` | `DEF_CVT3 KEY ANS` |
| `5` | `DEF_CVT3 KEY STL` |
| `6` | `DEF_CVT3 KEY PDA` |
| `7` | `DEF_CVT3 KEY STB` |
| `8` | `DEF_CVT3 KEY UG2` |
| `/` | return to the main menu |
| `0`, `E`, `e` | return to the main menu |
| anything else (`R`, `r`, empty, `9`…`99` only if in range, letters) | redisplay the menu |

So `"12"` (if within range) acts as `1`, `"Exit"` as `E`, `"end"` as `e`.

## 5. Option 1 — Input KEYWORD File

1. Default name = `strcpy(model+0x340) + ".KEY"`; if `FUN_1401b8560(default)` (can open for
   read) is false the default becomes `"?"`.
2. `printf("\n Keyword File = [ %s ] ? ", default)`; answer by `FUN_140009c50`; a non-empty
   answer replaces the name.
3. File exists:
   * unless the "stop" flag (below) is set: `"\n *** INFO: Reading Keyword File\n"`,
     `FUN_1401b5f00(model, name, &reset=1, &flags=0)` (the model is reset by `FUN_1401d7f10`,
     the KEY is read by `FUN_1401b6520`, then `FUN_1401d7f40`; open failure prints
     `"ERROR: Keyword file %s open error!\n"`), `"\n *** INFO: Keyword File Read\n"`,
     `"\n Hit <CR> to continue ..."` + one line;
   * back to the menu.
4. File missing: `"\n ??? Can not find: %s\n"`, `"\n Re-enter File Name? [Y] "`, answer:
   starts with `n`/`N` → set the stop flag and go back to the menu; otherwise go to step 1.

Quirk: the stop flag (`bVar1`) is never cleared inside one `FUN_140014ba0` call, so after one
"N" every later option-1 selection that finds its file **silently does not read it** (and a
missing file then returns to the menu after the re-enter question whatever the answer).
Leaving the file menu (`E`, `/`) and re-entering resets it.

## 6. Option 2 — Input DEFORM Data Base (`FUN_14000b170`)

1. Default = `model+0x340`, with `".DB"` appended unless `FUN_1401b0cc0(name, ".DB")`
   (ends with `.DB`, inferred); missing file → `"?"`.
2. `printf("\n DEFORM Database = [ %s ] ? ", default)`; non-empty answer replaces it.
3. File cannot be opened for read: `"\n     ERROR  : Unable to Open Database: %s\n"`,
   `"\n Try Another Database [Y]? "`; answer starting with `n`/`N` → return to the file menu;
   anything else → step 1.
4. `FUN_1401dc3f0(db, name, 0)` (open DB) non-zero → `"\n     ERROR in reading %s\n"`, return
   (no wait).
5. Header (stdout):
   ```
   "\nINPUT PREP. - DATABASE INPUT -\n"
   "-------------------------------------\n"
   " [ %s ] \n"                              model+0x340
   "\n ***  DEFORM Database Input  ***\n\n"
   "      Database Name:           %s\n"     db+0x650
   "      Simulation Title:        %s\n\n"   db+0xa50
   "      DEFORM System Version:   %.2f\n"   (float)db+0xabc / 1000.0f
   "      DEFORM System Release:   %s\n"     first 8 chars of db+0xaa1
   "      Initial Run Date:        %s\n"     first 8 chars of db+0xaa9
   "      Current Simulation Date: %s\n"     first 8 chars of db+0xab1
   "      No. of Result Sets:           %d\n" db+0xacc
   "\n\n      Step Numbers:\n"
   ```
   then for each directory entry `e` (count `db+0xac8`, 0x28-byte entries at `*(db+0xad8)`)
   with `e+0x24 != 0`: `"  %5d"` `e+4` (step number), and `"\n"` after every 10th printed
   number. The **last printed** step is the default (0 if none).
6. `printf("\n\n Step Number = %d ?", default)`; answer by `FUN_140009c50`; empty → `"%d"` of
   the default; `step = atoi(answer)` (`FUN_1401b0870`).
7. `"\n    INFO   : READING STEP %d FROM %s\n"` (step, db+0x650),
   `FUN_1401cc2a0(model, db, step, 1)` (result ignored; step semantics as `DBREAD`, see
   `dbmap/stepdriver.md`), `"    INFO   : STEP   %d READ\n"`, `"\n Hit <CR> to continue ..."`
   + one line; return to the file menu.

## 7. Options 3–8 — DEF_CVT3 imports

| Option | `sprintf` text (verbatim) | Format (menu) | Implemented by DEF_CVT3 (strings of `3D\DEF_CVT3.com`, inferred) |
|---|---|---|---|
| 3 | `DEF_CVT3 KEY IGE` | IGES | no (`"***  NOT IMPLEMENTED  ***"`) |
| 4 | `DEF_CVT3 KEY ANS` | ANSYS/PREP7 | no |
| 5 | `DEF_CVT3 KEY STL` | STL | yes → `DEF_STL.EXE` |
| 6 | `DEF_CVT3 KEY PDA` | PATRAN neutral | yes → `PDA2KEY.EXE` |
| 7 | `DEF_CVT3 KEY STB` | IDEAS universal | yes → `STB2KEY.EXE` |
| 8 | `DEF_CVT3 KEY UG2` | UGII neutral | no |

Launch: `FUN_1402240d0(1, text)` → `FUN_140223790(1, text, 0)`: `argv = {"DEF_CVT3", "KEY",
"<fmt>"}`, program searched as `%DEFORM_PC_PATH%\DEF_CVT3[.EXE|.COM]` (dirKind 0: no `2D\`/`3D\`
appended), then `system("\"\"<dir>DEF_CVT3\" KEY <fmt>\"")`; waits; return value ignored.
In the v11.0.2 install `DEF_CVT3.com` exists only in `3D\` (none in the root, none in `2D\`), so
the launch only finds the program when `DEFORM_PC_PATH` points to the `3D\` folder; otherwise
`cmd` reports that the program is not found and nothing else happens.

DEF_CVT3 runs in the same console and **inherits DEF_PRE's stdin**: its own prompts (input
file name etc.; not part of DEF_PRE) read from the same stream. Because DEF_PRE reads stdin
through the CRT/iostream buffer, a piped script may already be buffered inside DEF_PRE, so the
child can see EOF (inferred; must be checked by running).

Read-back: `if (FUN_1401b8560("SCRATCH.KEY"))` → `FUN_1401b5f00(model, "SCRATCH.KEY", &1, &0)`
(model reset, KEY read, no INFO messages). `SCRATCH.KEY` is **not deleted**. Then always
`"\n Hit <CR> to continue ..."` + one line, back to the menu. No other files are written or read
by DEF_PRE for these options; the converter's input file is whatever it asks for.

## 8. Stdin scripts

Main menu prompt and exit (`bm_main.c`): `E` → `"\nAre you sure [N]?"`, `y` ends.
Typical scripts (each line is one answer):

| Goal | stdin |
|---|---|
| read `A.KEY` | `\n2\n1\nA.KEY\n\nE\nE\nY\n` (the `\n` after the file name is the "Hit <CR>") |
| read step 5 of `A.DB` | `\n2\n2\nA.DB\n5\n\nE\nE\nY\n` |
| STL via DEF_CVT3 | `\n2\n5\n<DEF_CVT3 answers>\n\nE\nE\nY\n` |

## 9. Helper functions

| Address | Role |
|---|---|
| `FUN_140014ba0` | file menu loop |
| `FUN_14000bac0` | menu text + `FUN_140007d30` |
| `FUN_140007d30`, `FUN_140006420` | selection prompt / validation |
| `FUN_140009c50`, `FUN_1401b1060` | read + trim an answer |
| `FUN_140009e90` | "Hit <CR> to continue" |
| `FUN_1401b0940` | prefix compare (param 3 = 0: case-insensitive) |
| `FUN_1401b0cc0` | ends-with (inferred) |
| `FUN_1401b5f00` | read a KEY file into the model |
| `FUN_14000b170` | DB input |
| `FUN_1401dc3f0`, `FUN_1401cc2a0` | open DB, load step |
| `FUN_1402240d0` | launcher, dirKind 0 |

## 10. Open questions

* `lo`/`hi` of the selection range (ints at the start of the menu object, set by the main menu
  constructor) are not decompiled here; with `hi = 8` an answer `"9"` is rejected with
  `"??? Invalid Input"`.
* DEF_CVT3/DEF_STL/PDA2KEY/STB2KEY prompts and the location of `SCRATCH.KEY` they write are
  outside DEF_PRE; the table in §7 rests on strings of `DEF_CVT3.com` only.
* The step default when `db+0xac8 == 0`: `"%d"` of 0.
