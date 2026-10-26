# CP73 SW2SP comma delimiter implementation

## Change

The one-integer `SW2SP.DAT` reader now recognizes the evidence-qualified byte prefix `1,234` and returns 1 while advancing only past the leading `1`. The comma remains at the input cursor, matching the original `QTextStream` unget behavior. The special case is enabled only when the caller requests exactly one value and the path is exactly `SW2SP.DAT`; it does not alter `SPRING.INI`, other sidecars, or multi-value reads.

The branch is deliberately exact. It does not interpret the comma as a thousands separator or add generic grouping support. A focused adjacent-suffix case (`1,234x`) verifies that the comma and following bytes do not affect the first successful extraction. The test also retains non-SW2SP path and two-value request guards.

## Evidence

`analysis/resume73-sw2sp-next.md` documents the source-qualified QtCore control flow and retained binary SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. `QTextStreamPrivate` uses the C locale and its default zero number-options word. In the integer getter, the C-locale option comparison succeeds, selecting the unget path before the conditional group-separator comparison. The comma therefore remains unread, and the already accumulated decimal digit(s) make this a successful extraction. The original Extra3D sidecar reader requests one SW2SP integer and ignores later stream status, so bytes after the comma are not consumed by this request.

This is static binary/control-flow qualification; it is not an original-process runtime comparison. No general locale profile, arbitrary comma token, or follow-on extraction behavior is claimed.

## Static preflight

- Reviewed the one-value path gate and confirmed it is supplied only for `values.size() == 1 && path.u16() == u"SW2SP.DAT"`.
- Reviewed the branch cursor update: `++cursor` consumes only the initial `1`, leaving the comma unread.
- Reviewed the focused test for exact CRLF input, adjacent suffix, other-path rejection, and multi-value rejection.
- `git diff --check` completed without diagnostics. No build or test executable was run; root owns integration validation.

## Frozen file hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `5E903AAFA1E4D18EA30FF162A1862D00638D3C9BF0EF8E90459F3FE68DACB6D8`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `E07AC49D48CD4253958CB57AF2E77E9CAABC7B41658E6443F2827802C2A5AED0`
