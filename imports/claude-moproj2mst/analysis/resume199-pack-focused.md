# CP199 PACK fixture-enabled focused check

The source-stable CP199 integrated candidate has source SHA-256
`9c2ab8a0f82ce299c03b89da8daf0f5a895acc1a74c2b719078564321e51885f`
over 3,554 files (`analysis/resume199-integrated-build.json`). Its
`m2m_mdt_mdt_write.exe` SHA-256 was
`836cbad59f0805114566eccf3a9b3223edbf8f4edd4688674cc0c373e47c57d1`.

With `M2M_INSTALL_FIXTURE` set to the installation directory
`C:\Program Files\SFTC\DEFORM\v11.0.2`, the focused
`m2m.mdt.mdt_write` CTest passed all six internal cases, including
`header_text_from_pack_dat`; its optional-fixture skip event was absent.
The read-only `PACK\pack.dat` fixture was 11 bytes and its SHA-256 before
and after was
`f82967bf8117671aad02e8621549adbb6caf1e5265c55818ec806d9d6b250c10`.
The CTest exit was 0; `build/candidate/Testing/Temporary/LastTest.log` had
SHA-256 `fd01b50761c71700a46b29cafdcd381734451d551498715010071bb12421fe9c`
after this focused run.

This validates the optional PACK header assertion for this candidate only.
The full CP199 run intentionally did not set that environment variable and
retains its logged-gap classification; its frozen receipt must not be
rewritten. This check did not launch or modify the installed application.
