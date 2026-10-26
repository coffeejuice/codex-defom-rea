# BCCANG (id 164, group 2, case 0x14)

Registered keyword (registry flag 8, handler1/2/3 all no-op stubs) whose parser case is empty: the
reader accepts the keyword and its object number and stores nothing. Nothing in the program writes
it. The name suggests an angular (rotated-axis) BC code, but no code path gives it a meaning.

## KEY grammar (read)

```
BCCANG <objNo:%i>
```

Only the object number is consumed (`fscanf("%i")` in the common prologue). Case 0x14 then returns
the initial status 0 without reading anything else. **The rest of the line is left in the stream**,
so whatever follows the object number is read by the KEY interpreter as the next keyword token(s)
(`_common.md` section 1):

* nothing else on the line → fine (the newline is skipped by the next `fscanf("%39s")`);
* e.g. `BCCANG 1 0 0` → the token `0` is looked up as a keyword, not found, the rest of the line is
  read with `fgets` and `WARNING: Keyword 0 not found. Skip  0` (with the line's own `\n`) is
  printed to stdout; reading continues;
* any following data lines are likewise treated as keyword lines (each unknown first token gives
  one warning and skips its line; a token that happens to be a keyword name would be executed).

## Model storage

None. Side effect of the prologue only: `FUN_14019ccc0(model, objNo)` creates the object (and
placeholder objects up to it) if it does not exist yet.

## Read errors and validation

* object number EOF → **2**; `objNo < 1` → **2** (group2-helpers.md);
* otherwise **0**. No messages from the case itself.

## KEY writer

None. Checked:

* handler2 is the no-op `0x1402f75d0`; `FUN_14029e250` and its sub-writers never name id 164
  (group2-writer-map.md list);
* all 518 references to the name function `FUN_1402f7720` in the binary were scanned for the
  argument loaded into `ECX`: none passes `0xa4`; the only call with a variable argument,
  `FUN_1402f5090`, is used for id 299 only;
* the string `"BCCANG"` (at `0x140544060`) is referenced only by the keyword table entry at
  `0x140546688` (record 184 of the table at `0x140544400`), used for name lookup.

So a KEY file read and written back loses every `BCCANG` line. A port should accept the keyword,
consume the object number, and leave the rest of the line to the main loop.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x14 (p_main.c lines 1385–1387): empty case.
* `FUN_14019ccc0`: get-or-create object.
* `FUN_1401b6520`: KEY interpreter (unknown-token handling for the leftover text).

## Corpus check

No corpus file uses BCCANG (`corpus_kw.tsv`: 0 files).

## Open questions

* None for the KEY path. Whether a DB/binary path stores a BCCANG value was not examined
  (out of scope for KEY output).
