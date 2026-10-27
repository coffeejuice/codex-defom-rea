# CP394 exact CP334 pipe-flag marker

CP393 recorded `cp334_h0_pipeflags_other`, proving a nonzero flag outside 0/1/4/5 but not its value. CP334 now emits the exact unsigned `DWORD` decimal value before the unchanged zero-flags validation. CP349 accepts canonical decimal values from 0 through `0xFFFFFFFF` for handle ordinals 0 and 1. It rejects leading zeroes, signs, values above the DWORD maximum and malformed rows; the 2048-byte and 40-record trace bounds are unchanged. Legacy `_other` remains accepted for the CP393 raw trace.

Injected tests cover a large nonzero DWORD, retention of both owned handles on rejection, a valid rejected trace and malformed numeric markers. Independent read-only review found no change to the `Hold` or cleanup path. `analysis/cp394-exact-pipe-flags-tests.log` records 629 passing CP3xx tests from both import roots. No native child, WORKER, original application or GUI ran during this integration.

CP395 next freezes the source and bytecode, obtains a one-occurrence gate, and runs at most one new monitored inert diagnostic. Diagnose the exact value before modifying CP334's validation rule. The seventh CP393 occurrence must not be repeated. Public C++ converter candidate CP301 is unchanged.
