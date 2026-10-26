# USRDEF

Up to ten user-defined text lines.

* registry id 87 (`0x57`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x41 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → n (failure → 2). For i < min(n,10): `fgets(1024)` (EOF → 2), strip trailing space/CR/LF (`FUN_140323c50`), `strncpy(M+0xdd8+0x51·i, line, 0x50)` (no terminator added when the line has 80+ characters; the 81st byte keeps its previous value).

## Model fields

`M+0xdd8 + 0x51·i`, i = 0..9 (`S+0x688…`).

## Default

ten empty strings.

## KEY writer

Section *User Defined Variables*: k = 1 + index of the last non-empty slot; if k > 0 `"%-6s %7i\n"` k, then slots 0..k-1 each `"%s\n"` (empty slots in between are written as empty lines).

## Helpers

`FUN_140413de0`, `FUN_140323c50`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 56 occurrences in 56 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Forging/RIVET_EN.KEY`.
