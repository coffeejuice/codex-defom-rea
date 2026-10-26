# CP89 B98 GUI-free candidate comparison receipt

## Run identity

Ran only the frozen CP88 candidate CLI against a disposable copy of the retained B98 **after-state** folder. No original executable or GUI was launched. The command exited 0 with empty stdout/stderr:

```text
C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\candidate\bin\moproj2mst.exe C:\PROBLEM\b98r\run\Gear_blank.moproj C:\PROBLEM\b98r\run\candidate.MST --strict --codepage 1252 --write-save
```

The recorded cwd is `C:\PROBLEM\b98r\run`. All generated project/output paths checked are shorter than 100 characters: project 37, KEY 34, raw MST 33, path-qualified MST 48.

The CLI is bound to CP88 build source-tree SHA-256 `eacfe403abee631861befe6f9f35b274c9be1928af0e52f95e6e1e671c54cfef`. Candidate `moproj2mst.exe` is 4,126,720 bytes, SHA-256 `eef23401ece560ca6405c5ae23cb315cea88a1b14dc907f97c4c7e689a4be373`; adjacent `moproj2mst.dll` is SHA-256 `eed0f155561e8c95c46a77a046fcd3deb2ef23ac1770b767c05c3d76ab8401cd`; `deformdb.dll` is SHA-256 `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`. The EXE hash matches `analysis/resume88-integrated-order-repair-build.json` and its test execution binding.

The 31-file copied input manifest exactly matched the historical source fixture before conversion. The original fixture remained unchanged after conversion. Receipts and disposable raw data are under `C:\PROBLEM\b98r\`:

- `input-manifest.json` — copied input file list, sizes and SHA-256 values.
- `candidate-run.json`, `stdout.txt`, `stderr.txt` — argv/cwd/status and captured streams.
- `comparison-receipt.json` — binary/source binding, before/after manifests, path line, counts and hashes.
- `run\candidate.MST` — untouched raw candidate output.
- `run\candidate.path-qualified.MST` — separate output with exactly one candidate path value replaced by the historical path.

## Raw MST result

Historical original `Gear_blank.MST`: 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. Candidate raw `candidate.MST`: 2,710 bytes, SHA-256 `ce805350a189f07ae935c1fa8e9813df87ca63d203e814cea9a054ae5ded609f`.

Both have seven `KFREAD    \t1` markers and exactly one operation path. At line 30 (path value begins at offset 1,338), the original is:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

The raw candidate value is:

```text
C:/PROBLEM/b98r/run/Gear_blank.KEY
```

Raw bytes are unequal; candidate length is 46 bytes shorter, exactly the difference between those path values. Replacing only the single candidate path with the single historical path in a separate byte vector produces the full historical MST: 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`, byte-for-byte equal. The raw file was not modified.

## Side-file manifest qualification

The copied fixture contains 31 files. `--write-save` added the raw candidate MST; producing the separate path-qualified file added a second MST. It also rewrote the copied `Gear_blank.moproj`: input 11,229 bytes / SHA-256 `dddc325a6d6eb34c1625a904eb1bc055a6403c450c312bbe1703bb26af5f9da5`; after run 11,067 bytes / SHA-256 `d2c21248071fc6f314a83951040affd4919cb2ed65afbabdbd657d6bd0493592`. No other copied input file changed or disappeared.

This path check began from the retained **after-state** B98 folder to isolate the Diestress MST field. It establishes candidate MST payload equality after the one exact path substitution. The `.moproj` rewrite is a separate observed side effect and was not compared to a fresh original Save; therefore this is not full-folder parity. To assess the full historical B98 folder result independently, the next candidate-only comparison would need to start from `98_B_DS3F_before` and compare every generated output against `98_B_DS3F`, retaining the same short-path/raw-first rules.

## Scope

The protected original `DEF_GUI_MO_64.exe` was not executed or modified. This is fresh candidate output compared to a retained original MST file, not fresh-original execution, not raw-byte parity, and not proof of general KFREAD behavior. No source or build was changed.
