# Resume 205 Python AV callback semantics

## Finding

The child exited naturally with code `1` because the probe deliberately executed a null function pointer and Frida's exception callback returned `false`. That return value means “not handled”; it does not suppress the access violation or resume the faulting instruction. The OS/Python process therefore terminated through its normal unhandled-fault path. The receipt confirms a real AV at address/RIP `0x0`, with an execute access, and a stack through `libffi-8.dll`, `_ctypes.pyd`, and Python. It does not establish exactly which Python/Windows termination layer mapped the AV to exit code `1`; that mapping is not captured in the report. The checker expected raw status `0xC0000005`, so it marked cleanup unverified despite confirming natural exit, closed handles, no termination request, and absence from Windows and Frida process listings.

This is a checker expectation mismatch, not evidence that the callback swallowed the exception or that cleanup used forced termination. The payload explicitly records `disposition: return_false_continue_natural_exception`, while cleanup records exit code `1`, `termination_requested: false`, and an empty termination-attempt list.

## Does the callback payload support the original probe?

It supports an initial AV diagnosis: exception type/address, faulting RIP, access kind/address, general registers, and an accurate backtrace are all present. This capture is useful to check whether a converter AV occurs in or below a particular native call. It does not by itself prove the converter's own exception-handler behavior: the callback is installed in a separate Python child, and returning false intentionally delegates final handling to the process. The payload also omits a numeric Windows exception code/flags and thread ID; the observed type/address and stack are the available evidence.

## Minimal next test

Do not change the probe into a handler that returns true at RIP `0x0`: that would retry the invalid instruction and can produce a repeated fault. Make the preflight's natural-exit acceptance classify exit code `1` separately from `0xC0000005`, retaining the existing no-termination, handle-closure, and dual process-absence requirements. If the original probe requires demonstrating continuation after the handler, use a controlled owned child whose handler changes the saved instruction pointer to a known, valid continuation and returns true; record that continuation marker and exit receipt. That is a distinct test from this observation-only AV capture.

No process was run for this static analysis; no code was edited.