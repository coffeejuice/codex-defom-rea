# OBJPHS (id 204, group 2, no case: not parsed)

OBJPHS is a registry entry with no reader and no writer. The binary still has a debug string
`MOBJPHS = %c, %d\n` in `FUN_14043e0d0`, but no KEY code uses the keyword.

## KEY grammar (read)
Not parsed. The interpreter finds `OBJPHS` in the registry (group 2) and calls
`FUN_14038b8a0(model, f, 204)`:
1. `fscanf("%i")` reads the object number (EOF → 2);
2. `FUN_14019ccc0` gets or creates the object. **A side effect is that object `objNo`, and
   placeholder objects 1..objNo-1, are created if missing**; `objNo < 1` → 2;
3. `switch(204 - 0x90 = 0x3c)` has no `case 0x3c`, so the `default:` branch returns **2**.

So the result is always 2, "ERROR: Keyword OBJPHS Reading Error." on stdout, and the KEY loop
continues. The rest of the keyword line and any data lines are **not consumed**: the interpreter
reads their tokens as the next keywords.

Confirmed in `p_main.c`: the cases go 0x3b (ANGMVY), then 0x3d (VOLFC); the `default:` at line
1118 returns 2.

## Model storage
None, apart from the object creation side effect above.

## Read errors and validation
Always 2, as above.

## KEY writer
None. No function looks up the name of id 204 (`FUN_1402f7720(0xcc)`): `writer_index.txt` has no
entry and `group2-writer-map.md` lists it as never named. Registry handler2 is the no-op
`0x1402f75d0`. A string search for `OBJPHS` finds only the registry name (not referenced from code)
and the debug format `MOBJPHS = %c, %d\n` (`FUN_14043e0d0`, a model dump, not KEY output).

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` (default case).
* `FUN_14019ccc0`: object get-or-create.

## Corpus check
No corpus file uses OBJPHS.

## Open questions
None for the port. A byte-identical port must keep the "object created, then return 2" behaviour,
and must leave the rest of the line unconsumed.
