# CP102 heat-route test link repair

The first grouped preflight (`analysis/resume102-integrated-grouped-build.json`)
failed only while linking the new `m2m_mdt_editor_setup_heat_route` test:
LNK2019/LNK1120 reported a missing `main`. Its focused-preflight log is
`build/logs/resume102-integrated-grouped-focused-preflight-02.log`.

Root added `int main() { return testkit::run_all(); }` at the end of
`source/apps/moproj2mst/tests/mdt/test_editor_setup_heat_route.cpp`. The
repaired test source SHA-256 is
`7D26E0E191AB83604EA403E45BEB55049054CDA8A365F60606AE85675531B8E1`.
No production source or behavioral assertion changed. The repaired grouped
build is `analysis/resume102-integrated-grouped-repair1-build.json`; its
focused preflight, full build, focused CTest and full suite passed. Preserve
the first failed receipt as a separate validation attempt.
