# CP124 focused PACK header check against frozen CP123 source

## Result

The previously skipped optional PACK header assertion now passes. I built the single `m2m_mdt_mdt_write` test target from the verified CP123 source archive in a new disposable build directory, supplied the installed PACK path by `M2M_INSTALL_FIXTURE`, and invoked only that test executable with the retained read-only corpus root and isolated scratch paths. The target reported **0 of 6 cases failed**; `header_text_from_pack_dat` passed along with the five sibling cases.

The raw test output contains `refused (active cogging operation):` with no names after the colon. The test itself asserts the refusal list is empty and that 27 MDTs compare with zero Beta cases. Therefore the prior gap event `refused (active cogging operation): ` was an unconditional empty diagnostic line, not an actual refused project. The other logged event, `M2M_INSTALL_FIXTURE unset`, is closed by this run. This confirms the `mdt_write` row can be removed from the current logged-gap set only after root updates the suite receipt; it does not close the other five gap suites.

## Source, executable, and fixture binding

| Item | Binding |
|---|---|
| Frozen CP123 source archive | `build/wave3-checkpoint123-verified/source.zip`, SHA-256 `74c2b6f54ebf62827dc400c1e0649abfe46c6a478f6c0af3d5a07d554c3e5817` |
| Frozen source tree | The retained archive receipt binds 3,462 files to SHA-256 `81e35c00599eb03f0351cb452f502eb442fa48208d91cdcbf28fd40ac41c68c5`; rehydrated tree fingerprint matched before and after the run. |
| Exact test source in archive | `apps/moproj2mst/tests/mdt/test_mdt_write.cpp`, SHA-256 `c7928359396d0b24aecde5605ef67291d55b8bc521a65de89050dfd5af9c4abd` |
| Rebuilt focused test executable | `build/cp124-pack-focused/build/bin/m2m_mdt_mdt_write.exe`, SHA-256 `86b57e89e9b2dc193e06113fa706f55446d13e2d56da81a14f84e9c016bdea21` |
| PACK fixture read by the test | `C:\Program Files\SFTC\DEFORM\v11.0.2\PACK\pack.dat`, 11 bytes, SHA-256 `f82967bf8117671aad02e8621549adbb6caf1e5265c55818ec806d9d6b250c10` before and after the run |
| Corpus input | Existing `evidence/gui_mo/s1/corpus`, used read-only; test copies are directed to `build/cp124-pack-focused/build/direct-scratch`. |

The retained CP123 verified package did not include the test executable. The mutable `build/candidate/bin/m2m_mdt_mdt_write.exe` had changed from its CP123 recorded hash `2c6deabb987c080107eda3dec7d9ccc765a38b3e1c78cdea5a8076bea5940b7f` to `dd81bcdbb7afc2618fa60c62558023c87f4832930e26148c96aa0ef9db5e864d`; I rejected it before execution. Instead, the test was rebuilt from the CP123 archive. Its new hash is recorded above; it is source-bound to CP123 but is not byte-identical to the unretained CP123 test binary.

## Exact execution and retained raw evidence

1. Rehashed `source.zip` using its retained receipt framing/order and extracted it under `build/cp124-pack-focused/source`; its 3,462-file source fingerprint matched CP123 exactly.
2. Configured and built only the test target in the disposable build directory using MSVC 19.44.35228.0 / Ninja:

   ```text
   cmake -S <workspace>\build\cp124-pack-focused\source\apps\moproj2mst -B <workspace>\build\cp124-pack-focused\build -G Ninja -DCMAKE_MAKE_PROGRAM=C:\Tools\Ninja\ninja.exe -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=<MSVC 14.44.35207 cl.exe> -DM2M_TEST_SCRATCH=<workspace>\build\cp124-pack-focused\build\t
   cmake --build <workspace>\build\cp124-pack-focused\build --parallel 4 --target m2m_mdt_mdt_write
   ```

3. Ran the resulting test executable directly with `M2M_INSTALL_FIXTURE=C:\Program Files\SFTC\DEFORM\v11.0.2`, `M2M_CORPUS` bound to the retained corpus, and `DEF_PORT_REPO`, `M2M_TEST_SCRATCH`, `TEMP`, `TMP`, and `TMPDIR` pointed at the archived source and disposable scratch. Exit code was 0. Direct invocation was needed because the extracted source package intentionally omits `evidence/` and its generated CTest environment otherwise pointed at a nonexistent temporary corpus path.

Raw direct output is retained at `build/cp124-pack-focused/direct.log` (SHA-256 `3800c509350c4a1b20e17050cf5422a7da278364339cc7ee028e511d3c4408cd`). Configure/build transcript is `build/cp124-pack-focused/run.log` (SHA-256 `f590bfa1c5a5f2632e5e6d27052d6e54aba0cc7d22c37daa2c9d5e409ddf8fa7`). The initial CTest-only attempt is separately retained as `build/cp124-pack-focused/ctest.log` (SHA-256 `80f209fecdcfb98167e5c076b47af946181657c04e1129ed952786020e3fb4d8`) and `junit.xml`; it failed three corpus-dependent sibling cases because the archive has no `evidence/` directory, while `header_text_from_pack_dat` passed. Do not report that setup attempt as a PACK assertion failure. An initial configure attempt outside the Visual Studio developer environment also failed to find a compiler; only the disposable build directory was reset before the successful configure.

## Qualification

No source tree, retained corpus, installed PACK file, original executable, GUI, or REA database was modified or launched. This is a candidate test result bound to the exact CP123 source archive and a verified installed fixture; it does not establish original runtime parity. The empty active-cogging event is a reporting/classification issue in the test log event scanner, not evidence of a native refusal path. Root should use this receipt when refreshing the suite-level gap summary.
