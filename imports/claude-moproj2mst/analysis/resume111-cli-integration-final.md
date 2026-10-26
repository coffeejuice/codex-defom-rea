# CP111 isolated Save integration

The opt-in `--isolated-save` CLI path now runs one Open/Save conversion in an
owned child process. It binds a versioned, bounded UTF-16 job to the
supervisor's random nonce before launch. A normal child publishes Started and
Returned; the CLI returns the adapter code. At the source-qualified remesh
failed-open boundary, the child publishes Started and Boundary, exits with the
fixed candidate status `0xC0000417`, and the supervisor verifies process
identity, protocol, status and absence of an intervention. It then creates,
flushes and closes an exclusive terminal receipt before requesting the same
status from its own CLI process. The receipt reports a requested parent relay,
not an observed parent exit. Unknown/Custom handler profiles remain guarded.

The CLI rejects a receipt inside the project tree before child creation, as
well as exact/equivalent project or selected-output aliases. A receipt
collision fails closed without replacing the existing file. Product scratch
is an exclusive owned leaf; uncertain child stop retains it for diagnosis.
The default CLI path and the six-export DLL ABI remain unchanged.

Source SHA-256 at the final CP111 build:

| File | SHA-256 |
|---|---|
| `src/cli/isolated_save_job.cpp` | `2eaf5b06156721deda592251f11a8b7ca47268442836547658af6ceb8f8a5ea5` |
| `src/cli/isolated_save_worker.cpp` | `19d3191d1039772fea9e75e49a5042be5781e076cba3714a4ab5bfa47dfb182f` |
| `src/cli/isolated_save_supervisor.cpp` | `26d24adeab0e28663d667a0eb70014ecc5cb9ad5c8459b89fa1c40e84ce3efeb` |
| `src/engine/terminal_harness_supervisor.cpp` | `bc8a00d8593f12f116b25e98a35ef96d1342f206db046cb207ea26cdff058ce6` |
| `tests/g0/test_cli_isolated_save.cpp` | `7c02af0d03fd7ebf578b7f7fe65f072f69f51d519c20881e609c00a23b9b0e2e` |
| `tests/g0/test_cli_isolated_supervisor.cpp` | `ac90ec2bf19fbc2175f34178b08ce5e9b76caec82b4e4898946fa33c772a84ba` |

The final grouped build receipt is
`analysis/resume111-cli-repair4-build.json`, with source tree SHA-256
`f30274ec9e90a9a33c54a1b5e3b3ccb7d416ef22a5bb1d608ac17c1f3087f3b8`.
Five focused tests passed. The full source-bound run registered 298 tests:
291 clean, six with logged gaps, one diagnostic comparison with differences,
and zero failures. Historical comparison remains 93 SAME/five DIFF; fixtures
were unchanged. The six-export check and three ABI tests passed. The strict
B109 raw-output comparison also passed separately in this build.

This route proves candidate process ownership and candidate file effects on
the constructed B133 overlay. It does not prove that the original followed
this same-run terminal path, reproduce the native CRT context-capture and
unwind prelude, or implement native Close. The retained source fixture has 64
files; the disposable overlay adds two remesh seeds. The public C ABI remains
guarded for this terminal branch, and a normal Returned receipt is deferred.
Original GUI comparison remains gated by unsupported app-control release.
