# Build target audit for the 150 converter CTests

A converter-only build can prepare all 150 registered top-level CTests with 142
explicit targets. This avoids unrelated runtime test and database utility products,
but retains nearly all shared compilation. This is a static dependency audit;
no build, test, fixture read/write, or application execution occurred.

## Runtime coverage

| Registered tests | Required runtime products |
| --- | --- |
| 139 tests discovered from `tests/*/test_*.cpp` | Their matching `m2m_<packet>_<topic>.exe`; ABI tests also import `moproj2mst.dll` |
| `m2m.g0.cli_missing_project` | `moproj2mst.exe` (build target `moproj2mst_cli`) |
| `m2m.release.sample_cpp` | `m2m_sample_cpp.exe` and adjacent `moproj2mst.dll` |
| `m2m.release.sample_csharp` | `m2m_sample_csharp.exe`, adjacent `moproj2mst.dll`, installed .NET Framework runtime |
| Seven Python registrations | Installed Python; the release sample loads the explicit DLL path |
| `m2m.release.exports` | Installed CMake and MSVC `dumpbin.exe`; reads `moproj2mst.dll` |

The seven Python registrations are `m2m.g0.runner_self_test`,
`m2m.g0.corpus_lines_self`, `m2m.p08.corpus_lines`, `m2m.p08.keyword_table`,
`m2m.p09.corpus_lines`, `m2m.support.paths`, and `m2m.release.sample_python`.
The runner self-test spawns the same Python script in dummy-CLI mode via
`sys.executable`; it does not require a separate compiled converter process.
The keyword-table test reads the original executable as a reference data file;
it does not execute it. These classifications come from source and registration
inspection; fixtures themselves were not opened.

Two subprocess relationships need explicit attention when building one test:

* `m2m.release.unchanged` invokes `M2M_CLI` through `CreateProcessW`.
  Building its C++ target alone does not ensure a current CLI. Include
  `--target moproj2mst_cli` when preparing that test.
* The current optimized `m2m.s.save_folder` spawns its own executable resolved by
  `GetModuleFileNameW` for workers. It needs no separate worker or sibling CLI.

The seven Python invocations use the configured Python executable at
`C:/Program Files/Python314/python.exe`. The export check uses
`C:/Program Files/CMake/bin/cmake.exe` and the configured MSVC
`14.44.35207/bin/Hostx64/x64/dumpbin.exe`. These are host prerequisites, not
products this target selection builds. Windows system DLLs and the C# runtime
remain host prerequisites; this audit does not independently enumerate their
transitive OS imports.

## Safe minimal explicit set and closure

The 142 named targets below are all directly invoked compiled test products:
139 C++ tests, the CLI and two samples. Their existing CMake dependency graph
already brings in `moproj2mst.dll`. Adding explicit `moproj2mst` is harmless but
redundant for this complete set; it is necessary when preparing only Python or
export-check subsets. `m2m_core` and the shared static libraries also arrive
transitively: `defrt`, `deformdb_core`, `defmodel`, `defkey`, `defdbio`, `defcmd`,
and `def_pre_core`. Testkit is header/interface support. The graph links the
converter against `deformdb_core`, not the separate `deformdb.dll`.

Static traversal of the current generated Ninja graph found 1,052 non-phony
edges in `all`, versus 1,004 in this set's closure. The difference is 24 C++
compilation edges and 24 links: 17 `defrt_test_*` executables, database
`dbinfo`, `dbraw`, `dbmanager`, `test_codec`, `test_writer`, `capi_smoke`, and
`deformdb.dll`. The cold receipt reports 1,053 commands because its Ninja log also
contains the glob-verification command, outside the parsed `all` dependency
closure. This 48-edge reduction is not a measured elapsed-time improvement.

Do not omit static dependency libraries, ABI tests or release samples merely to
reduce work. Do not build `m2m_tier2`/`m2m_tier3` as a preparation step: those
custom targets execute corpus tests. This list prepares the 150 `m2m` tests,
not any additional tests registered under dependency subdirectories. Recompute
it from registrations whenever tests/CMake change; do not treat the list as an
eternal allowlist. Validate with a serial build and full required suite before
adopting it as the default.

```text
m2m_g0_abi
m2m_g0_abi_open_failure
m2m_g0_abi_original_settings
m2m_g0_core
m2m_mdt_boolean_editor
m2m_mdt_close_effects
m2m_mdt_cogging_io
m2m_mdt_cogging_save
m2m_mdt_conversion_session
m2m_mdt_db5_keepmesh
m2m_mdt_db5_sync
m2m_mdt_db_inheritance
m2m_mdt_db_load
m2m_mdt_display_selection
m2m_mdt_doe_activation
m2m_mdt_doe_default_session
m2m_mdt_doe_model_manager_binding
m2m_mdt_doe_output_data
m2m_mdt_doe_property_table
m2m_mdt_doe_sample_io
m2m_mdt_doe_session
m2m_mdt_doe_session_notifications
m2m_mdt_doe_slice
m2m_mdt_doe_snapshot_write
m2m_mdt_doe_statevar
m2m_mdt_doe_statevar_apply
m2m_mdt_doe_stepdata
m2m_mdt_doe_study
m2m_mdt_doe_task_factory
m2m_mdt_doe_task_lists
m2m_mdt_doe_task_runtime
m2m_mdt_doe_view_stepdata
m2m_mdt_editor_base
m2m_mdt_editor_grid
m2m_mdt_editor_setup
m2m_mdt_engine_l3
m2m_mdt_engine_loader
m2m_mdt_engine_probe
m2m_mdt_file_device
m2m_mdt_gui_mesh_defaults
m2m_mdt_l5_loader
m2m_mdt_l6_slots
m2m_mdt_l7_load
m2m_mdt_lifecycle_events
m2m_mdt_loader_l2
m2m_mdt_mdt_write
m2m_mdt_model_manager
m2m_mdt_multiblow_get
m2m_mdt_multiblow_load
m2m_mdt_multiblow_save
m2m_mdt_picking_model
m2m_mdt_picking_owner
m2m_mdt_save_l2
m2m_mdt_save_l6
m2m_mdt_shapeoptim_props
m2m_mdt_simcontrol300
m2m_mdt_simcontrol300_io
m2m_mdt_simcontrol300_lifecycle
m2m_mdt_simcontrol300_mo
m2m_mdt_simcontrol300_native_io
m2m_mdt_simcontrol300_owner
m2m_mdt_simcontrol300_remesh
m2m_mdt_simcontrol300_snapshot
m2m_mdt_view_manager
m2m_mdt_viewer_box
m2m_mdt_viewer_display_options
m2m_mdt_viewer_property_table
m2m_mdt_viewer_slice
m2m_mdt_viewer_stepdata_write
m2m_mdt_widget_events
m2m_moproj_deflate
m2m_moproj_dom_save
m2m_moproj_moproj_lists
m2m_moproj_moproj_ops
m2m_moproj_moproj_write
m2m_moproj_props_l2
m2m_p01_accessors
m2m_p01_blob
m2m_p01_htexpress
m2m_p01_longpath
m2m_p01_walk
m2m_p01_xml
m2m_p02r_registry
m2m_p03_connector_contract
m2m_p03_flowops
m2m_p03_order
m2m_p03_subproject
m2m_p04_corpus_numbers
m2m_p04_device_stream
m2m_p04_numbers
m2m_p04_strings
m2m_p04_text_device
m2m_p05_paths
m2m_p06_headers
m2m_p06_mdt
m2m_p06_token
m2m_p06_wizards
m2m_p07_corpus
m2m_p07_unit
m2m_p08_emit_a
m2m_p08_props_keys
m2m_p09_emit_b
m2m_p09_keywords
m2m_p10_base_order
m2m_p11_accessors
m2m_p11_heattransfer
m2m_p11_xcheck
m2m_p12_objects
m2m_p12_p12_writers
m2m_p13_p13_mdwz
m2m_p13_p13_writers
m2m_p14_multiblow_data
m2m_p14_multiblow_writer
m2m_p14_simcount
m2m_p15_apply_indexed
m2m_p15_classinit
m2m_p15_express
m2m_p15_heattreat_tree
m2m_p15_tree
m2m_p16_htwz
m2m_p16_items
m2m_p16_replace
m2m_p16_report
m2m_p16_setters
m2m_p16_trees
m2m_p16_writers
m2m_p17_classinit
m2m_p17_classprops
m2m_p17_paths
m2m_p17_units
m2m_release_unchanged
m2m_s_gui_startup_unit
m2m_s_native_directories
m2m_s_postload_views
m2m_s_save_answer
m2m_s_save_current
m2m_s_save_dispatch
m2m_s_save_folder
m2m_s_save_io
m2m_sample_cpp
m2m_sample_csharp
moproj2mst_cli
```

## Evidence identity

Generated files were read without invoking CMake or Ninja:

* `build/test-speed-candidate/CTestTestfile.cmake`: SHA-256 `04e4b61c3d986fa05c2962ac71c9a96a21fddbea8083591ecaa5d718a307f1d2`
* `build/test-speed-candidate/build.ninja`: SHA-256 `a867d710e4f2d320010edf328a60de31e4301f20a77fc95b307f6dd2d15f33e4`

Source inspection covered the converter CMake registration loop and release
fragment, tests and testkit process/loading call sites, sample wrappers, and the
runner self-test dispatch. Source edits in other optimization lanes can change
worker internals; this report records the inspected self-spawn behavior, without
claiming runtime validation of those edits.
