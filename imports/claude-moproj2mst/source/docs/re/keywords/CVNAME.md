# CVNAME

Named collection of strings (one name + n entries) per id.

* registry id 90 (`0x5a`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x44 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` → id, n (failure → 2). Next line: name (`FUN_140413c60`, 64 bytes, EOF → 2), stored with `FUN_14032f350(coll, id, name)` (collection at `M+0x33b8`, entry created by `FUN_14032f240` if missing), then `FUN_14032f1a0(coll, id)` clears its list. Then n lines (64 bytes each, EOF → 2) appended with `FUN_14032f5f0`.

## Model fields

Collection object at `M+0x33b8` (`FUN_14019a4e0`), entries keyed by id with a name and a string list.

## Default

empty.

## KEY writer

Last records of the group: for each entry (`FUN_14032f1e0` count, `FUN_14032f210(i)`): `"%-6s %7i %7i\n"` (id, list size), the name line, then each list string with `"%s\n"`.

## Helpers

`FUN_140413ee0`, `FUN_140413c60`, `FUN_14032f350`, `FUN_14032f1a0`, `FUN_14032f5f0`, `FUN_14032f1e0`, `FUN_14032f210`, `FUN_14032ef00`, `FUN_14032efa0`, `FUN_14032ef50`, `FUN_14032f070`. Shared mechanics: [_common.md](_common.md).

## Corpus

No corpus KEY file contains this keyword; grammar is from code only.
