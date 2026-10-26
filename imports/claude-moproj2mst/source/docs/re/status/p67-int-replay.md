# P6-7 integration replay (branch claude/p67-3-int)

Replay of the cached oracle runs through the port, one case per process
(`python -I tools/int/run_int.py`, test `port/defcmd/tests/test_int_oracle.cpp`, runner
`cmd_oracle_runner.hpp` with per-case input dirs and clock pinning from p67-gate 67ef43e).

Result 2026-10-21 (p67-7 2168662 + p67-6 + p67-2 + p67-3 + SRCNT4 + p67-gate 67ef43e):
**92 / 92 PASS** — 15 OBJPOS drop cases (modes 7, 8, 9), 5 INICTC, 72 GENCTC (2D, 3D, s3d input
files, two-masters interference): every group 2 record of every object and every selected stdout
line equal to the original.

Known limit: in the original, the SRCNT4 records (" Interference detected, KOBJ=", " Info:
Checking Invisible definition") can follow an unflushed menu prompt on the same stdout line. The
replay compares them from where they start (`Options::normalize`); the exact interleaving of prompt
and record is left to the def_pre end-to-end run at the P6-7 gate.
