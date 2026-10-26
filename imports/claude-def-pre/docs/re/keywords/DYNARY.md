# DYNARY (id 266, group 2, case 0x7a)

Per-object list of generic "dynamic arrays" (`DYNARY_DATA` records in a `DynAryLinkedList` at
`obj+0xba8`; RTTI names). Each record is a `count × ncomp` array of doubles or ints; the
header's subtype selects double table (1), int table (2) or sparse double array with a default (3).

## KEY grammar (read)
Header = rest of the keyword line, split by the tokenizer `FUN_140413900` (max 10 tokens,
separators space , \n \t \r); integer tokens use `strtol(...,10)` (decimal only), the default uses
`atof`; missing tokens are 0:

    DYNARY <objNo:%i> <idx> <kind> <subtype> <count> <ncomp> [<nrec>] [<default>]

* `kind` outside 1..1 is forced to 1 (any value → 1).
* `nrec` = token 5 if the line has ≥ 6 tokens, else `count`.
* `default` = token 6 if the line has ≥ 7 tokens, else 0.0.

Body (only for the subtype / ncomp combinations below; everything else reads no body lines):

| subtype | storage | ncomp | body: `count` lines, line `i` (index token read and ignored) |
|---|---|---|---|
| 1 | double | 2 | `<i:%i> <v1:%Flf> <v2:%Flf>` (`FUN_140415680`) |
| 1 | double | 3 | `<i> <v1> <v2> <v3>` (`FUN_140415880`) |
| 2 | int | 3 | `<i:%i> <v1:%i> <v2:%i> <v3:%i>` (`FUN_140414100`) |
| 2 | int | 4 | `<i>` + 4 × `%i` (`FUN_140414200`, no blank skip) |
| 2 | int | 8 | `<i>` + 8 × `%i` (`FUN_140414600`, no blank skip) |
| 3 | double, sparse | any | `nrec` records `<index:%i>` + `ncomp` × `%Flf`, free format over lines (`fscanf`), then one `fgets` (`FUN_1404199a0`; none when `nrec ≤ 0`) |

Line-based bodies (subtypes 1, 2): one `fgets` line per record, commas = spaces, values stored
sequentially in record order (the index is not used). Subtype 3: records are stored at
`(index-1)*ncomp` when `1 ≤ index ≤ count`, others are read and dropped (see
`group2-helpers.md`, per-node readers).

## Model storage
List: `SfLinkedList` layout (`+8` head, `+0x10` tail, `+0x18` count). Factory (vtable slot 1,
`FUN_14025ba40`) = `new` 0x40 bytes + `FUN_14025afe0`.

`DYNARY_DATA` (0x40 bytes):

| offset | type | field (init) |
|---|---|---|
| +0x00 | ptr | vtable |
| +0x08 | byte | active flag (1) |
| +0x0c | int | kind (0) |
| +0x10 | ptr | next record |
| +0x18 | int | subtype (0) |
| +0x1c | byte | isInt (0) |
| +0x20 | int | storage class (0) |
| +0x24 | int | count (0) |
| +0x28 | int | ncomp (0) |
| +0x30 | ptr | double vector (handle; data begin at `*(+0x30)+8`, end at `+0x10`) |
| +0x38 | ptr | int vector (same layout) |

Reader: `rec = FUN_14025a990(list, idx)` (append default records until `count ≥ idx`, return the
`idx`-th; `idx ≤ 1` → head, NULL if empty), dynamic cast to `DYNARY_DATA`; NULL → nothing stored,
return 0 (body lines stay in the stream). Then `FUN_14025b220(rec, kind=1, subtype, count, ncomp)`:
`+0x0c, +0x18, +0x24, +0x28` set; for kind 1: subtype 1 → isInt 0, class 1; 2 → isInt 1,
class 2; 3 → isInt 0, class 3; other subtypes leave `+0x1c/+0x20` unchanged (0/0 for a new
record). Then the active vector (int if isInt, else double) is resized to `count × ncomp` when its
size differs (new elements 0, existing kept; `FUN_14025b4d0`).
Body dispatch: class 1 needs isInt 0, class 2 needs isInt 1, class 3 needs isInt 0. Class 3
first fills every element with `default` (`FUN_14025b580`), then reads the records.

## Read errors and validation
* objNo EOF / < 1 → 2.
* Tokenizer EOF → 2 (empty line is not an error: all header values 0, nothing read).
* Subtype 1/2 body line: helper returns 1 (EOF or empty line) → 2 immediately.
* Subtype 3: `FUN_1404199a0` EOF → 1 → 2.
* No other validation: negative `count`/`ncomp` are not checked (a negative product makes the
  resize call misbehave; corpus never exercises it). Index tokens are never checked.

## KEY writer
`FUN_1402a41c0(model, f, obj, objNo)`, called from `FUN_14029e250` right after the OPSTOP writer
`FUN_1402a4020` (same conditions: normal objects only, after the handler chain succeeded); its
return value is ignored. `f`/`obj` NULL → 3. `objNo == 0` → `obj+8`.

For `i = 1 .. list count`: `rec` = `i`-th record cast to `DYNARY_DATA`, skip NULL. With
`kind = +0x0c`, `sub = +0x18`, `count = +0x24`, `ncomp = +0x28`:

* class 1 and isInt 0: `"%-6s %7i %7i %7i %7i %7i %7i\n"` objNo, i, kind, sub, count, ncomp; then
  per row `r` (1-based): ncomp 2 → `" %7i %20.10E %20.10E\n"`, ncomp 3 →
  `" %7i %20.10E %20.10E %20.10E\n"`, other ncomp → **return 3** (after the header; with
  `count == 0` no error).
* class 2 and isInt 1: same header; rows ncomp 3 → `" %7i %7i %7i %7i\n"`, 4 → `" %7i"`×5 `"\n"`,
  8 → `" %7i"`×9 `"\n"`; other ncomp → return 3.
* class 3 and isInt 0: compute (`FUN_1402f5360`, mode 1, reference 0.0):
  * uniform = every one of the `count×ncomp` values equals value[0];
  * uniform → `default = value[0]`, `nrec = 0`;
  * else → `default = 0.0`, `nrec` = number of rows with at least one value != 0.0.

  Header `"%-6s %7i %7i %7i %7i %7i %7i %7i %20.10E\n"` objNo, i, kind, sub, count, ncomp, nrec,
  default. If not uniform: for every row with a value != 0.0: `" %7i"` (row number) then the
  `ncomp` values through `FUN_1402f2e80(f, ncomp, row, 5, 0)` (`" %20.10E"` each, after every 5th
  value `"\n"` + 8 spaces, final `"\n"`; `_common.md` §3).
* any other combination (e.g. a gap record with class 0) → **return 3**: the rest of the list is
  not written.

Round-trip: lossless for the supported combinations (the reader forces kind 1; the writer prints
`+0x0c`, which is 1 for every record read from a KEY file). Gap records (idx skipped in the file)
abort the writer at the first gap. Edge: `count×ncomp == 0` for class 3 makes the writer read
`value[0]`/`defaults[0]` out of range (undefined output for the default field).

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x7a — reader; `FUN_140413900`/`FUN_140413a50`/`FUN_14047b6c0`/`FUN_14048754c`
  — tokenizer, token, `strtol`, `atof`.
* `FUN_140415680`, `FUN_140415880`, `FUN_140414100`, `FUN_140414200`, `FUN_140414600`,
  `FUN_1404199a0` — body readers (`group2-helpers.md`).
* `FUN_14025a990` / `FUN_14025a7d0` / `FUN_14025a9e0` — list get-or-create / append / n-th (shared
  with OPSTOP); `FUN_14025bb50` — n-th + cast; `FUN_1402b73e0` — count of `obj+0xba8`.
* `FUN_14025b1d0` — cast to `DYNARY_DATA`; `FUN_14025afe0` / `FUN_14025b0d0` — constructor/init;
  `FUN_14025b220` — set header; `FUN_14025b4d0` — resize; `FUN_14025b410` — element count;
  `FUN_14025b580` — fill doubles; `FUN_14025b470` / `FUN_14025b4a0` — double / int data pointers;
  `FUN_14025b310/350/370/3b0/3f0/960` — get kind/subtype/count/ncomp/class/isInt.
* `FUN_1402a41c0` — writer; `FUN_1402f5360` — uniform/default/nrec analysis; `FUN_1402f2e80` — value
  line writer.

## Corpus check
No corpus file uses DYNARY.

## Open questions
* What the arrays hold (consumer of `obj+0xba8`) and the meaning of kind/subtype beyond the
  storage class.
* The behaviour for negative `count`/`ncomp` (resize with a negative size) would need the vector
  resize code (`FUN_14019a740`, `FUN_140159b60`).
