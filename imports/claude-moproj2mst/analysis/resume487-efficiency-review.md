# CP485–487 safe-boundary efficiency review

CP485's full build took 0.656 seconds because its focused test had already built the only changed target. CP486's full build took 32.734 seconds and CP487's 44.485 seconds; their production parser/header changes rebuilt broader dependent targets. The bound full launchers took 19.781, 20.484 and 20.578 seconds. Different compile workloads prevent a controlled speed claim. Token-use measurement was unavailable, so no token-saving claim is made.

The two exact malformed SW2SP leaves are faithful but would be inefficient as a long sequence of isolated tokens. CP488 therefore tests whether the same PE-bound getter branches support a finite class with explicit exclusions, then one focused and one full bound validation. Keep the original static/runtime distinction, source freeze, independent review, logged gaps and refusal inspection intact.
