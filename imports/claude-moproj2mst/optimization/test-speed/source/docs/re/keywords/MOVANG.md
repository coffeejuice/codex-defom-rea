# MOVANG (id 152, group 2, case 0x08)

Obsolete keyword: it is parsed but its value is thrown away, and nothing writes it. Its name
(movement angle) matches what MOVCTL now carries itself (the 2D movement angle on the MOVCTL
line).

## KEY grammar (read)

2D (`FUN_1400015e0(model)` true):

    MOVANG <objNo:%i> <value:%Flf>

`FUN_140414990` reads the rest of the line into a stack local (`dStack_1128`) that is not stored
anywhere afterwards.

3D (any non-2D model): **nothing is read after the object number**, case status 0. The rest of
the line stays in the stream; the KEY loop reads its next token as a keyword, e.g.
`MOVANG 1 -90.0` → `WARNING: Keyword -90.0 not found. Skip <rest>` (stdout), then continues.

## Model storage

None (the object is still created by the object-number lookup `FUN_14019ccc0`).

## Read errors and validation

2D: blank/EOF rest of line → **2**; otherwise 0. 3D: 0.

## KEY writer

None. The registry handler2 is the no-op `0x1402f75d0`, and a scan of every call to the keyword
name function `FUN_1402f7720` in the binary (the immediate passed in ECX before each of the 518
calls; only `FUN_1402f5090` passes a variable id, and it prints a name only when that id is 299) found no
call with id 152 (0x98). So MOVANG is never written.

## Unit conversion

None.

## Helpers

`FUN_14038b8a0` case 8, `FUN_140414990`.

## Corpus check

No corpus file uses MOVANG.

## Open questions

None.
