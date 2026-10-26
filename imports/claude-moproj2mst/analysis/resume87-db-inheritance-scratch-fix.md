# CP87: isolate DB-inheritance scratch across executable runs

The four `db_inheritance` failures came from fixture setup, not DB behavior. `Rig::copyDb` uses `std::filesystem::copy_file` without overwrite semantics. Its destination directories had been formed from PID plus a per-process serial, so a later CTest executable run with the same PID-scoped scratch root could reuse an old `Inherited.DB` or `selected.DB` and fail before assertions.

Changed only `source/apps/moproj2mst/tests/mdt/test_db_inheritance.cpp`. Each Rig now obtains a candidate directory using a thread-safe monotonic serial and creates it with the single-directory `create_directory` operation. Existing leftovers are skipped rather than reused; unexpected filesystem errors fail setup with a path-qualified exception. The existing PID-scoped parent remains under `M2M_TEST_SCRATCH`, and the fixture data is still copied into an otherwise empty per-Rig directory. This also avoids deleting prior test scratch or touching fixture sources.

Preflight: the file retains its Testkit `main`, uses only supported `REQUIRE`/`CHECK` macros, and adds the standard `<atomic>` include for the serial. No production/evidence files, build, CTest, GUI, or mutable REA state were changed. Root owns focused rebuild and rerun; no passing-test claim is made here.
