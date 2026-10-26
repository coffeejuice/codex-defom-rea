# MATAXI (id 254, group 2, case 0x6e)

Per-element material axis data: a double array stored as state variable 0x2e of the
element-data block `obj+0x9e0`, at `obj+0xa40`. The component count is `model+0x16b0`. The
reader sets it to 1 (2D) or 9 (3D, which fits a 3×3 orientation matrix) and then replaces it
with the count given in the file.

## KEY grammar (read)

* `model+0x16b0 = 9` if 3D (`FUN_1400015f0`), else 1.
* `dst = FUN_140184150(obj+0x9e0, &false)`: `FUN_1401836c0(block, nelem = block+0xe8,
  model+0x15f8, block+0xec, 0x2e)` allocates variable 0x2e; it returns `block+0x60`
  (= `obj+0xa40`).
* `FUN_140416c90(f, dst, n = obj+0x884, &model+0x16b0, mode 2)`:

        MATAXI <obj> <count:%i> <k:%i> [<d1:%Flf> … <d10:%Flf>]     rest of the keyword line (sscanf, no comma replacement)
        count records (fscanf):  <elem:%i> <v1:%Flf> … <vk:%Flf>
        no trailing fgets

* The mode-2 check `model+0x16b0 > 10` → 1 happens before the header is read, with the value
  just set (1 or 9), so it never fires.
* **`model+0x16b0 = k`** (the file's value) as soon as the header is parsed.
* Defaults: `nd` = conversions − 2. If `nd == k`, every element gets the default vector;
  else if `nd > 0`, every component of every element gets d1.
* Records: `FUN_140419b90(f, dst, count, n, m = k, k)`. Each record has k values; it is stored
  at `[(elem-1)*k + j]` if `1 ≤ elem ≤ n`. With `k == 0` it falls back to `FUN_1404199a0`
  with m = 0 (index only) **plus** a final `fgets`.
* The record loop has no trailing `fgets`, so the rest of the last record line stays in the
  stream. The KEY interpreter's next `%39s` simply skips the newline.

## Model storage

* `double* obj+0xa40`, laid out element-major with `model+0x16b0` values per element.
* `model+0x16b0` is model-wide: the last MATAXI read sets it for all objects.
* The array is allocated with the component count in effect when `FUN_140184150` runs (1 or 9).
  A file with `k` larger than that writes past the allocation; the code does not check this.

## Read errors and validation

* Header EOF or `sscanf < 1` → **2**.
* Record EOF → **2**; non-numeric tokens are not detected.
* `k > 10` is **not** rejected after the header (the `> 10` test ran before k was known).

## KEY writer

Group2-writer-map.md step 55: `FUN_1402a4a80(model, f, obj, 0x2e, N, 0)` (code 0x1d is an
alias), both dimensions. The registry handler `FUN_1402b6780` (also code 0x2e) is not called
by `FUN_14029e250`. The writer is
`FUN_1402f5c10(f, "MATAXI", N, obj+0xa40, n = obj+0x884, m = model+0x16b0, default 0.0, fmt 2)`:

* Nothing if n, m or the array is 0/NULL.
* Uniformity (fmt 2) means every element's m-vector equals element 0's vector.
  * uniform: defaults = element 0's vector, count 0;
  * otherwise: defaults = m × 0.0, count c = elements with any component ≠ 0.0.
* Header: `"%-6s %7i %7i %7i"` → MATAXI, N, count, m, then m × `" %20.10E"` defaults, then `"\n"`.
* Records (non-uniform only): `" %7i"` elem, then m × `" %20.10E"` through `FUN_1402f2e80`
  (5 per line, `"\n"` + 8 spaces continuation), then `"\n"`.

This round-trips: `nd == m == k`, so the default vector is applied per component and the
records override it.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140184150`, `FUN_140182980`, `FUN_1401836c0`, `FUN_140329e60` | allocate variable 0x2e of the element-data block |
| `FUN_140416c90` (mode 2), `FUN_140419b90` | reader |
| `FUN_1402a4a80` (code 0x2e), `FUN_1402f5c10` (fmt 2), `FUN_1402f5360`, `FUN_1402f2e80` | writer |
| `FUN_1402b6780` | registry handler, unused by the object writer |

## Corpus check

* `2D\EN\plane stress\Ela_PlaneStress_EN.KEY`: `MATAXI       1       0       1  0.0000000E+000`
  parses (uniform, k = 1).
* `2D\EN\MultipleOperations\MO2_Boolean_Oprations\Operations\Task00004\DEF_BOOLEAN_OUT.KEY`:
  `MATAXI       1     642       1    0.0000000000E+000` / `       1    4.0351727646E+000` parses.
* All 515 corpus headers have k = 1, including the 3D files, e.g. `MATAXI 2 0 1 0.0E+00`.

## Open questions

* What the 9-component 3D form contains; the corpus never uses it.
