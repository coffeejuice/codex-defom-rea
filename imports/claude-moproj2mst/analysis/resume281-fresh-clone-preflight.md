# CP281 fresh clone preflight

Status: **PASS**. This is static fixture preparation only. No application process, GUI control, Frida, or REA was used.

- Created UTC: `2026-10-27T00:09:05.286633+00:00`
- Destination: `C:\PROBLEM\m2m281a`
- Creation command: `python -c "shutil.copytree(source, target)" (Python shutil.copytree; destination must not exist)`
- Source count: 33/27; clone count: 33/27
- Source before equals clone after: `True`
- Source before equals source after: `True`
- Gate exit: `0`; gate status: **HOLD**
- Full manifests and gate receipt: `resume281-fresh-clone-preflight.json`

The gate reports `readyForIndependentReview: false` and HOLD. It does not authorize launching the original.
