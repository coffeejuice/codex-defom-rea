# CP77 Open selected-path test receipt

- **Test source:** `source/apps/moproj2mst/tests/p01/test_walk.cpp`
- **SHA-256:** `0DD38C746957C022B4BFDE05B952E9D85A73B26EEC07CC9728B8F90FAC8B6AC7`
- **Registered CTest target:** `m2m.p01.walk` (executable target `m2m_p01_walk`, per `source/apps/moproj2mst/CMakeLists.txt` test naming rules).
- **Source qualification:** Native `0x14001ca30` constructs the project from `changeSuffix(selected, ".DB")` before calling `0x14008a9b0`; native Open accepts `.moproj`, `.DB`, and `.KEY`, cleans/directly retains MOPROJ or normalizes DB/KEY to MOPROJ, then updates the DB path before parsing. Candidate `moproj/read.cpp::openProject` follows that path prefix but deliberately refuses DB/KEY after its DB-open prefix. Tests assert normalization and that refusal, not DB/KEY Open support or dialog defaults.
- **Write scope:** CP77 changed only `tests/p01/test_walk.cpp`; no production source, CMake, or build files were changed in this increment.
- **Validation:** No build or tests were run, by instruction. GUI and REA state were not accessed.
