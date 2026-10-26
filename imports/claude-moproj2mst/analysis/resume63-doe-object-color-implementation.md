# CP63 isolated object-color table value consumer freeze

The new helper recovers cdeab0's table-value algorithm using an explicitly supplied TextFile, TokenReader, initial table bytes and trailing output destinations. It does not bind the native process-global palette, construct/register a DB child, call fopen/fclose, resolve a filename or release public B127/DBLIST/Report guards.

Only three new source/test files and this implementation report were written for this increment. Existing datafiles token/stream implementation, manager, model, Session, dbSetPath, public reader and CMake files were not edited. Source is frozen for root's build/test/archive.

## API and native mapping

ObjectColorTableValues contains six tables of100 four-byte RGBA colors. Its initial bytes belong to the caller; no native palette defaults are fabricated. Native table bases relative to process owner1447c4860 are760,8f0,a80,c10,da0,f30. The API is:

```cpp
bool readIsolatedObjectColorTableValues(datafiles::TextFile* openedStream,
    datafiles::TokenReader& reader, ObjectColorTableValues& values,
    std::int32_t& firstTrailingValue, std::int32_t& secondTrailingValue);
```

A null openedStream selects the independently qualified fopen-failure branch: returnfalse without touching reader, tables or trailing destinations. It is a declared isolated input, not a helper filesystem observation. A nonnull stream is borrowed after a separately qualified successful rt open. The caller supplies the initial reader mode and state. The helper uses the existing TokenReader::Int and kNewLine/0 rules, with their existing port qualifications.

The helper performs exact100 attempts. RowID and R/G/B/A int destinations initialize once to0 and persist across attempts. A valid ID1..100 selects rowID-1. Only first-read token count13 selects four-table RGB; all other return values select six-table RGBA for valid IDs. Invalid IDs skip payload reads. First four tables read RGB, store RGB immediately, then optionally read/store alpha. Last two RGBA tables read all four components before any store. Components narrow directly to unsigned bytes; no clamp or color validation is added. Duplicate IDs overwrite reached entries in encounter order.

The function neither breaks nor resets destinations on EOF. Existing Int reads at EOF preserve the supplied destination, so prior rowID/components can be reused, including RGBA selection after return-2. After100 attempts it reads trailing1 with kNewLine, then trailing2 with0, and returnstrue. It does not reject an empty/truncated opened stream or roll back earlier stores. Caller-owned trailing destinations retain prior values on unsuccessful Int reads.

The seam stops before native fclose: the borrowed TextFile stays caller-owned, and true describes this isolated opened-stream value path. Original cdeab0 closes its FILE and then returnstrue; no native file-handle/close effect is claimed here. Native filename local8bit conversion and same-process global token/palette identity remain production dependencies.

## Nine focused Testkit cases

1. Observed open-failure input preserves table bytes, both outputs and an already-partly-consumed reader position.
2.100 complete RGB rows preserve all alpha bytes and last two tables; trailing reads consume exactly two tokens and leave a third available.
3.100 complete RGBA rows map all six tables and wrap negative/out-of-byte-range component values rather than clamp.
4. Duplicate valid IDs overwrite; invalid0/101/-1 rows skip payload work and keep unrelated entries intact.
5. A short RGB file reaches EOF with a prior valid ID and reuses its last RGB plus initial local alpha0 into all six tables, preserving output destinations at EOF.
6. Opened empty stream returnstrue and leaves destinations unchanged.
7. An explicit adapter stop before the first table's alpha preserves already stored RGB.
8. An adapter stop before the fifth table's alpha preserves that entire previous fifth color while retaining first four table writes.
9. An adapter stop on the second trailing read preserves the completed first trailing destination and exact100 earlier row attempts.

AdapterStop fixtures verify source-order partial writes through the borrowed stream seam; they do not claim that a host exception reproduces native CRT error behavior. Actual EOF preservation is tested separately. All fixtures use supplied memory/line-reader state and perform no filesystem operations.

## Static verification and root integration

All 4 quoted includes resolve; nine unique TEST_CASE names, Testkit main and supported CHECK assertions are present; no C assert is used. New files have final newlines and no trailing whitespace. These are static checks only. No compiler, build, CTest or executable tests ran in this lane.

Suggested root target: m2m_doe_object_color_table_values; CTest name: m2m.doe.object_color_table_values. Add src/doe/object_color_table_values.cpp to the same core linkage as the other internal DOE helpers and link the new test to that core. Root owns compilation, focused execution and grouped validation.

Next production dependency: actual process palette owner and prior table/TokenReader state, qualified rt open/local8bit filename and close ownership, and native constructor/path-setter staging before manager ID/append/connections. Existing-child DBLIST lookup can bypass those absent-child effects only when its actual resolved-path/manager result is qualified; no such B127 result is guessed here. Public and absent-child guards remain unchanged.

Freeze UTC: 2026-10-24T21:47:05.011962+00:00.

| New source/test | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/object_color_table_values.hpp` | `f47927b461bff8ab46f0cd95884a74a871882891b9e7b8d5b5a0e61d6c74b46c` |
| `source/apps/moproj2mst/src/doe/object_color_table_values.cpp` | `8b1fe49f8588639b97e0f31fc12eab7867909d573b34b2cfb1ed4cb6102db4cf` |
| `source/apps/moproj2mst/tests/doe/test_object_color_table_values.cpp` | `a9000501d8d8d558dbc5ce816355bff05f6ba345ab37db4bd8b6a7909d5888db` |

| Source qualification/current dependency | SHA-256 |
| --- | --- |
| `analysis/resume63-dblist-constructor.md` | `01633372f41b235f59e64c44057390cc15cf02963a67babcc58f4574bd596da0` |
| `analysis/resume63-dblist-palette-reader.asm` | `24a9b2a6eb17187f63d3b06cc16bea0c7ad363e56ab7f43066e371664ea39aa2` |
| `analysis/resume63-dblist-constructor-receipt.json` | `ad25c211bad7620d5f60fe6f0fa8d4043da5de1f98ce5687b42a8d647a2824b7` |
| `analysis/wave2-doe-model-manager-path-palette-reader-decompile.json` | `20c6a01ef361346567e260d707d87735fd2d0c665f3e3f2b81c073639f774f23` |
| `analysis/wave2-doe-model-manager-path-palette-reader-receipt.json` | `e63128e15739ce1156916ad98b1b89b02ea1b951db076d23113faa0987259872` |
| `source/apps/moproj2mst/src/datafiles/token.cpp` | `3c56020a595e356e3a6c51a177c6470c88ae63622d85a6256ef4dc298e5bde9e` |
| `source/apps/moproj2mst/src/datafiles/textfile.cpp` | `48e57cfa86788277f620778f1137794da4c0c100d68b95999a9fb69b1ababd85` |
| `source/apps/moproj2mst/include/m2m/datafiles.hpp` | `8bbb05a24dd045212b846f0e5a8049d7395db543bf774413cc9d6cc97804cbbc` |
