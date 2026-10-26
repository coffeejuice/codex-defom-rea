# Main menu 9 "Convert the 2D DB to 3D keyword file"

Decompile: `deform-rea-work/out/p89/menu9.c` (`FUN_14000afd0`, depth 2); assembly checked in
`out/p89/d/dis9.txt`. Related keyword: [CNVT3D.md](CNVT3D.md).

## 1. Summary

Main menu line (`bm_main.c`): `"   9 - Convert the 2D DB to 3D keyword file\n"`; `case '9'`
calls `FUN_14000afd0(main+0x188)`.

**The item does nothing but ask a question.** `FUN_14000afd0` loops: print the question, read
an answer, return on `N`/`n`/`e`/`E`, and repeat on anything else — including `Y`/`y`, for which
there is no call at all (verified in the assembly: the "Y"/"y" compares at `0x14000b07c` /
`0x14000b099` only set the loop flag; the function contains no other calls than the prompt
`FUN_14000af30`, the reader `FUN_140007d30`, string copies and `free`). It never reaches
`FUN_1401467f0` (CNVT3D), writes no file and launches nothing. The only way to run the 2D→3D
conversion in batch mode is the `CNVT3D` keyword.

## 2. Prompts and answers

Per iteration:

1. `FUN_14000af30`: `std::cout << '\n'` (via `FUN_1400078b0(cout, 10)`), stream-state check,
   `cout << "Convert 2D to 3D? (y/n) " << '\n'` and flush (`FUN_140007150`). The question is
   followed by a newline.
2. `FUN_140007d30(menu, out, 1, 0, 0)` (param_3 = 1 → range from the arguments):
   prompt `" Enter Selection (0-0, R, E, /): "`, getline (256), validation `FUN_140006420`
   (see [FILE_INPUT_MENU.md](FILE_INPUT_MENU.md) §2): `e`, `E`, `r`, `R`, `/`, any
   non-number and `0` are accepted; a non-zero number is rejected with `"\n"`,
   `"??? Invalid Input\n"`, `"\n"` and the selection prompt is repeated.
3. The accepted answer, trimmed of blanks and `'?'` (`FUN_1401b1060`), is compared **exactly**
   (2 bytes including the NUL):

| Answer | Result |
|---|---|
| `Y`, `y` | flag 1 → loop again (no action) |
| `N`, `n`, `e`, `E` | return to the main menu |
| anything else (`/`, `R`, `yes`, empty, `0`…) | flag -1 → loop again |

Stdin example (enter item 9, leave it, exit DEF_PRE): `\n9\nN\nE\nY\n`.

## 3. Relation to CNVT3D

None in code. The menu text suggests the same function as `CNVT3D` (which writes a 3D KEY file,
see CNVT3D.md), but the interactive path was left as a stub in v11.0.2.

## 4. Helper functions

| Address | Role |
|---|---|
| `FUN_14000afd0` | menu-9 loop |
| `FUN_14000af30` | prints `"Convert 2D to 3D? (y/n) "` |
| `FUN_140007d30`, `FUN_140006420` | selection prompt / validation |
| `FUN_1400058b0` | `std::string` copy |

## 5. Open questions

* Whether a line consisting only of blanks is passed through unchanged by the trim (it is, by
  reading `FUN_1401b1060`; `" "` then falls into the "anything else" row).
