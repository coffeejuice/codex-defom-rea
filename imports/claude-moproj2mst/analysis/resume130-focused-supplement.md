# CP130 focused CTest supplement

`analysis/wave3-checkpoint130-validation.json` accidentally recorded an empty
`focused` list because the freeze CLI expects the final test-name component and
was given full dotted CTest names. Its full 316-test source-bound result and
immutable candidate freeze remain valid. The focused tests were run before the
full build and repeated after freeze; all four passed with zero failures.

The repeated CTest log is `build/logs/resume130-focused-ctest.log`, SHA-256
`a81e918f663609c45a7626c66cf58fa1f8fd4f59cab77d5bccb38bffe28ba3d8`.
It covers `m2m.doe.type3_optimizer_scalar_prefix`,
`m2m.mdt.simcontrol300_meshing_selection`,
`m2m.mdt.simcontrol300_spring_reader` and
`m2m.model.feature_crt_runtime`. No source or frozen artifact was modified
for this supplement. Future freezes must pass final components to `--focused`.
