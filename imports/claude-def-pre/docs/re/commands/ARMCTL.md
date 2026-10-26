# ARMCTL — write DEF_ARM_CTL.DAT (three integers)

Shared mechanics: [common.md](common.md). Code: inline in `FUN_140378e00` case 0x15
(`out/bm_cmd.c`; assembly 0x14037e0be–0x14037e1f4 checked); helpers in `out/p89/armctl.c`.

## 1. Summary

| | |
|---|---|
| Registry id | 21 |
| Dispatcher | `FUN_140378e00` case 0x15, inline |
| External program | none launched |

Deletes `DEF_ARM_CTL.DAT`, then writes three integers to it, one per line. The model is not used.
`DEF_ARM_CTL.DAT` is referenced nowhere else in DEF_PRE (StrRefs: only this case), so it is a
control file for the automatic remesh / multi-operation driver (`DEF_ARM`, `DEF_ARM_CTL.com` in
`2D\` and `3D\`) — inferred consumer.

## 2. Argument grammar

```
ARMCTL <v1> <v2> <v3>      rest of the keyword line
```

`FUN_140413900(f, &n, 3)`: `fgets(1024)` into a global buffer, `strtok` with `" ,\n\t\r"`, at most
3 tokens (the token table slots 0..2 are cleared first). EOF → returns 1 → case returns 2.
Each value = `atol(FUN_140413a50(k))` (`FUN_14047b6c0` = `strtol(s, NULL, 10)`); a missing token
gives `""` → 0; text → 0; `"12abc"` → 12. Decimal only (no hex/octal, unlike `%i`).

Example: `ARMCTL 1 0 5` (or `ARMCTL 1,0,5`).

## 3. Model fields used

None.

## 4. File written

1. `remove("DEF_ARM_CTL.DAT")` — the call the decompiler shows as `FID_conflict__wrmdir` is
   CRT `0x14047e29c`, which calls `DeleteFileA` (so `remove`/`_unlink`, not `_rmdir`).
   Result ignored.
2. `FUN_1401b1880(h, "DEF_ARM_CTL.DAT")` (h = {name, FILE*=0}); `FUN_1401b18f0(h, "w")` =
   `fopen(name, "w")` (text mode). Failure → nothing written, no message.
3. An `ofstream`-like `SfOStream` is attached to the `FILE*` (`FUN_1401b1bd0` → `FUN_1401b1f30`
   → `FUN_1401b3b10` filebuf on FILE, mode out).
4. Three times: `os << v_k` (`FUN_140159340`, `ostream::operator<<(int)` via `num_put`; decimal,
   no padding, `-` for negatives) then `std::endl` (`FUN_140007a50`: put `'\n'` + flush).
5. `fclose` (`FUN_1401b1940`), stream destroyed (`FUN_140158be0`), handle dtor (no-op).

Content (CRLF line ends on disk, text mode):

```
<v1>
<v2>
<v3>
```

## 5–6. Launch / read-back

None.

## 7. Messages / returns

No messages. Case returns 2 only on EOF of the keyword line; otherwise 0 (also when the file
cannot be created).

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_140413900` / `FUN_140413a50` | tokenise line / get token (`""` when absent) |
| `FUN_14047b6c0` | `atol` |
| `FID_conflict__wrmdir` `0x14047e29c` | `remove` (DeleteFileA) |
| `FUN_1401b1880`, `FUN_1401b18f0`, `FUN_1401b1940`, `FUN_1401b18b0` | FILE handle: init, fopen, fclose, dtor |
| `FUN_1401b1bd0`, `FUN_140158be0` | `SfOStream` over FILE: ctor / dtor |
| `FUN_140159340` | `operator<<(int)` |
| `FUN_140002830` + `FUN_140007a50` | apply manipulator `std::endl` |

## 9. Open questions

* Meaning of the three integers (DEF_ARM side).
