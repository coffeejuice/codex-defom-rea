# CP302 exit-only fixture

`exit_only.c` is a fixed, synthetic x64 console child whose `main` returns zero.
It has no GUI, hook, network, or wait behavior. It must never be substituted for
an original DEFORM executable.

From a Windows x64 MSVC Developer PowerShell with `VCToolsInstallDir` set, run
`python admin/build_cp302_fixture.py` from the converter root. The build script
accepts no caller-selected source, executable, or hash. It pins the source SHA-256,
uses separate MSVC compile and link invocations with `/MT /MACHINE:X64
/SUBSYSTEM:CONSOLE`, writes `C:\PROBLEM\s302\fixture.obj` and
`C:\PROBLEM\s302\fixture.exe`, and records compiler, linker, commands, source,
and inspected PE identity in `analysis/cp302-fixture-build.json`. It never starts
the binary. Its PE gate requires AMD64, console subsystem, and imports limited
to `KERNEL32.DLL`, and refuses TLS and delay-import directories; a toolchain
that adds other imports fails closed. The builder refuses existing output,
object, and receipt paths. The receipt records the exact bytes read by the PE
inspector. Tool files or output files can still be replaced between inspection
and later use; verify the receipt and binary immediately before use.

Run `python admin/test_cp302_fixture_source.py` for the static source contract.
Review the receipt and binary before any separate synthetic child experiment.
The reviewed binary is also preserved here as `fixture.exe` (108,032 bytes,
SHA-256 `66eadac3f5dd6ee0d0b856a53760dc9270ecfd170808c62dff01b99a583f6a28`).
It was copied byte-for-byte from the one built at `C:\PROBLEM\s302\fixture.exe`;
neither copy was executed during CP302 preparation. A later test must still bind
its actual source/clone image and cleanup to a fresh receipt.
