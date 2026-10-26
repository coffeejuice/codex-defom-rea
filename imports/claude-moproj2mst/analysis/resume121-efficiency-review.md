# CP119–121 efficiency review

CP119–120 grouped builds took 38–42 s with few compile actions. CP121 rose to 193.375 s because its focused preflight and full build both rebuilt broad target sets. Test launchers stayed near 30 s through CP120 and reached 32.219 s at CP121. No result approaches a 10× improvement, and no token measurements are available.

| Checkpoint | Configure | Focused preflight | Full build | Grouped build total | CTest | Launcher | Registered / clean passes |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP116 final | 2.078 s | 4.172 s | 29.500 s | 41.569 s | 13.391 s | 29.672 s | 306 / 299 |
| CP117 | 2.219 s | 1.859 s | 22.828 s | 32.989 s | 13.500 s | 30.312 s | 306 / 299 |
| CP118 | 2.172 s | 32.031 s | 123.375 s | 163.710 s | 13.250 s | 30.625 s | 307 / 300 |
| CP119 | 2.218 s | 5.469 s | 28.860 s | 42.485 s | 13.406 s | 30.031 s | 307 / 300 |
| CP120 | 2.110 s | 2.734 s | 27.579 s | 38.432 s | 13.485 s | 29.782 s | 307 / 300 |
| CP121 | 2.344 s | 57.140 s | 126.735 s | 193.375 s | 13.953 s | 32.219 s | 308 / 301 |

The CP119–121 Ninja logs show that CP119 had 10 focused-preflight compiles plus four links and 12 full-build compiles plus 284 links. CP120 had four preflight compiles plus two links and eight full-build compiles plus 286 links. CP121 had 156 preflight compiles plus four links and 293 full-build compiles plus 295 links. In CP121 preflight, 153 of 156 C++ compilations are `m2m_core`; its full build compiles 224 test translation units and 69 source translation units. CP118 is similar at 157 preflight compiles and 295 full-build compiles (226 tests, 69 source files), with 294 full-build links.

CP121 therefore repeats CP118’s broad-rebuild pattern: source fingerprint changed and file count rose from 3,457 to 3,459 while the recorded CMake cache hash stayed the same. The Ninja action counts are almost identical to CP118, but CP121 preflight elapsed 57.140 s versus 32.031 s, and its full-build phase took 126.735 s versus 123.375 s. These logs do not contain dependency explanations, per-action times, or host CPU/I/O telemetry; they cannot establish whether the long preflight reflects a particular shared header, source timestamps, or contention. CP119/120 are useful lower-action observations but are different source snapshots, not controlled timing repeats.

## Launcher and fixture integrity

| Checkpoint | Fixture SHA before / after | Source/binary binding before / after | CTest | Launcher |
|---|---:|---:|---:|---:|
| CP116 | 5.734 / 5.937 s | 1.797 / 1.797 s | 13.391 s | 29.672 s |
| CP117 | 5.750 / 5.844 s | 1.844 / 2.343 s | 13.500 s | 30.312 s |
| CP118 | 5.985 / 6.219 s | 1.859 / 2.281 s | 13.250 s | 30.625 s |
| CP119 | 6.125 / 5.796 s | 1.829 / 1.844 s | 13.406 s | 30.031 s |
| CP120 | 5.781 / 5.812 s | 1.828 / 1.828 s | 13.485 s | 29.782 s |
| CP121 | 6.656 / 6.016 s | 2.281 / 2.266 s | 13.953 s | 32.219 s |

All six runs use the same four-worker threaded SHA-256 fixture hashing, parent-cache path resolution, and no persistent cache. Each before/after manifest covers 16,728 files and 1,867,669,827 bytes. Every run reports stable source/binary binding and zero fixture changes. The source file counts progress 3,456, 3,456, 3,457, 3,457, 3,457, and 3,459; source hashes differ by checkpoint as expected.

CP119–121 each retain six logged-gap tests, one logged-difference test, zero failures, skips, or incomplete outputs, and no changed diagnostics. Clean pass counts rise with registered tests (300/307, 300/307, 301/308). Validation remains `objective_complete: false`. CTest time is about 13–14 s; the small increase at CP121 is not a major suite-time change. Hash and binding phases make up 15.3–17.2 s of the launcher, and CTest about 13–14 s. Token usage is recorded as unavailable.

## Exact next experiment

The most useful no-fidelity-loss performance experiment is a three-pair, alternating four-worker versus eight-worker fixture-hash benchmark at a safe frozen boundary, using identical fixture roots and complete before/after manifests. Fixture hashing is the largest repeatable launcher phase outside CTest; changing only worker count leaves the accepted test set, fixture content, source/binary bindings, and assertions intact. Record wall and process CPU time, preserve each ordered manifest, and report cache-state limits. Adopt a different worker count only if all manifest hashes match and the repeated median improvement is material. Do not run this during CP122 source edits/build. Keep the existing one-megabyte read chunks; the earlier CP80 benchmark found four-megabyte chunks slower. The 10× test-time target is not attained.
