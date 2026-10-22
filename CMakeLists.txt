cmake_minimum_required(VERSION 3.24)
project(deform_loader VERSION 0.1.0 LANGUAGES CXX)
add_library(deform_loader src/database.cpp src/packed_array.cpp src/object_summary.cpp)
target_include_directories(deform_loader PUBLIC include)
target_compile_features(deform_loader PUBLIC cxx_std_20)
if(MSVC)
  target_compile_options(deform_loader PRIVATE /W4 /permissive-)
else()
  target_compile_options(deform_loader PRIVATE -Wall -Wextra -Wpedantic)
endif()
add_executable(deform_db tools/deform_db.cpp)
target_link_libraries(deform_db PRIVATE deform_loader)
add_executable(deform_dump tools/dump_step.cpp)
target_link_libraries(deform_dump PRIVATE deform_loader)
add_executable(deform_objects tools/object_summary.cpp)
target_link_libraries(deform_objects PRIVATE deform_loader)
enable_testing()
add_executable(packed_array_test tests/packed_array_test.cpp)
target_link_libraries(packed_array_test PRIVATE deform_loader)
add_test(NAME packed_array COMMAND packed_array_test)
add_executable(database_test tests/database_test.cpp)
target_link_libraries(database_test PRIVATE deform_loader)
add_test(NAME database COMMAND database_test)
add_executable(object_summary_test tests/object_summary_test.cpp)
target_link_libraries(object_summary_test PRIVATE deform_loader)
add_test(NAME object_summary COMMAND object_summary_test)
add_library(deform_recovered_state src/recovered/state_variable.cpp src/recovered/mesh_boundary.cpp src/recovered/hexa_boundary.cpp src/recovered/planar_boundary.cpp src/recovered/mesh_storage.cpp src/recovered/keyword_reader.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/object_model.cpp src/recovered/keyword_import.cpp src/recovered/geometry_storage.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/component_layout.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/geometry_segments.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/custom_fields.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/property_table.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/furnace_model.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/relation_properties.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/fstres_properties.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/grndat_properties.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/frcmod_properties.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/hardening_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/creep_properties.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/retained_material_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/object_direct_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/remesh_scalar_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/object_rotation_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/object_integer_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/object_output_range_import.cpp)
target_sources(deform_recovered_state PRIVATE src/recovered/unit_system.cpp)
target_include_directories(deform_recovered_state PUBLIC src)
target_compile_features(deform_recovered_state PUBLIC cxx_std_20)
if(MSVC)
  target_compile_options(deform_recovered_state PRIVATE /W4 /permissive-)
else()
  target_compile_options(deform_recovered_state PRIVATE -Wall -Wextra -Wpedantic)
endif()
add_executable(state_variable_replay tests/state_variable_replay.cpp)
target_link_libraries(state_variable_replay PRIVATE deform_recovered_state)
add_executable(mesh_boundary_replay tests/mesh_boundary_replay.cpp)
target_link_libraries(mesh_boundary_replay PRIVATE deform_recovered_state)
add_executable(planar_boundary_replay tests/planar_boundary_replay.cpp)
target_link_libraries(planar_boundary_replay PRIVATE deform_recovered_state)
add_executable(mesh_storage_replay tests/mesh_storage_replay.cpp)
target_link_libraries(mesh_storage_replay PRIVATE deform_recovered_state)
add_executable(mesh_refresh_replay tests/mesh_refresh_replay.cpp)
target_link_libraries(mesh_refresh_replay PRIVATE deform_recovered_state)
add_executable(keyword_reader_replay tests/keyword_reader_replay.cpp)
target_link_libraries(keyword_reader_replay PRIVATE deform_recovered_state)
add_executable(keyword_import_replay tests/keyword_import_replay.cpp)
target_link_libraries(keyword_import_replay PRIVATE deform_recovered_state)
add_executable(metadata_import_replay tests/metadata_import_replay.cpp)
target_link_libraries(metadata_import_replay PRIVATE deform_recovered_state)
add_executable(component_layout_replay tests/component_layout_replay.cpp)
target_link_libraries(component_layout_replay PRIVATE deform_recovered_state)
add_executable(unit_system_test tests/unit_system_test.cpp)
target_link_libraries(unit_system_test PRIVATE deform_recovered_state)
add_test(NAME unit_owned_state_contract COMMAND unit_system_test)
add_executable(grain_layout_test tests/grain_layout_test.cpp)
target_link_libraries(grain_layout_test PRIVATE deform_recovered_state)
add_test(NAME grain_fallback_source_contract COMMAND grain_layout_test)
find_package(Python3 COMPONENTS Interpreter QUIET)
if(Python3_Interpreter_FOUND)
  foreach(prsnam_cohort IN ITEMS training evaluation)
    add_test(NAME "prsnam_${prsnam_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_prsnam.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/prsnam-${prsnam_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/prsnam-${prsnam_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/prsnam-runtime-${prsnam_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/prsnam-${prsnam_cohort}-regression.json")
    set_tests_properties("prsnam_${prsnam_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(output_range_cohort IN ITEMS training evaluation)
    add_test(NAME "output_range_${output_range_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_output_range.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/output-range-${output_range_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/output-range-${output_range_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/output-range-runtime-${output_range_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/output-range-${output_range_cohort}-regression.json")
    set_tests_properties("output_range_${output_range_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(object_integer_cohort IN ITEMS training evaluation)
    add_test(NAME "object_integers_${object_integer_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_object_integers.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/object-integers-${object_integer_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/object-integers-${object_integer_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/object-integers-runtime-${object_integer_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/object-integers-${object_integer_cohort}-regression.json")
    set_tests_properties("object_integers_${object_integer_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(object_rotation_cohort IN ITEMS training evaluation)
    add_test(NAME "object_rotation_${object_rotation_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_object_rotation.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/object-rotation-${object_rotation_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/object-rotation-${object_rotation_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/object-rotation-runtime-${object_rotation_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/object-rotation-${object_rotation_cohort}-regression.json")
    set_tests_properties("object_rotation_${object_rotation_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(remesh_scalar_cohort IN ITEMS training evaluation)
    add_test(NAME "remesh_scalars_${remesh_scalar_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_remesh_scalars.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/remesh-scalars-${remesh_scalar_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/remesh-scalars-${remesh_scalar_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/remesh-scalars-runtime-${remesh_scalar_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/remesh-scalars-${remesh_scalar_cohort}-regression.json")
    set_tests_properties("remesh_scalars_${remesh_scalar_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(object_direct_cohort IN ITEMS training evaluation)
    add_test(NAME "object_direct_${object_direct_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_object_direct.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/object-direct-${object_direct_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/object-direct-${object_direct_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/object-direct-runtime-${object_direct_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/object-direct-${object_direct_cohort}-regression.json")
    set_tests_properties("object_direct_${object_direct_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(retained_cohort IN ITEMS training evaluation)
    add_test(NAME "retained_material_${retained_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_retained_material.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/retained-material-${retained_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/retained-material-${retained_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/retained-material-runtime-${retained_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/retained-material-${retained_cohort}-regression.json")
    set_tests_properties("retained_material_${retained_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(creep_cohort IN ITEMS training evaluation)
    add_test(NAME "creep_${creep_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_creep.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/creep-${creep_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/creep-${creep_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/creep-runtime-${creep_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/creep-${creep_cohort}-regression.json")
    set_tests_properties("creep_${creep_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(hardening_cohort IN ITEMS training evaluation)
    add_test(NAME "hardening_${hardening_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_hardening.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/hardening-${hardening_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/hardening-${hardening_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/hardening-runtime-${hardening_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/hardening-${hardening_cohort}-regression.json")
    set_tests_properties("hardening_${hardening_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(frcmod_cohort IN ITEMS training evaluation)
    add_test(NAME "frcmod_${frcmod_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_frcmod.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/frcmod-${frcmod_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/frcmod-${frcmod_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/frcmod-runtime-${frcmod_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/frcmod-${frcmod_cohort}-regression.json")
    set_tests_properties("frcmod_${frcmod_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(material_curves_cohort IN ITEMS training evaluation)
    add_test(NAME "material_curves_${material_curves_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_curves.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-curves-${material_curves_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-curves-${material_curves_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-curves-runtime-${material_curves_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/material-curves-${material_curves_cohort}-regression.json")
    set_tests_properties("material_curves_${material_curves_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(material_scalars_cohort IN ITEMS training evaluation)
    add_test(NAME "material_scalars_${material_scalars_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_scalars.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-scalars-${material_scalars_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-scalars-${material_scalars_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-scalars-runtime-${material_scalars_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/material-scalars-${material_scalars_cohort}-regression.json")
    set_tests_properties("material_scalars_${material_scalars_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(root_names_cohort IN ITEMS training evaluation)
    add_test(NAME "root_names_${root_names_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_root_names.py"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/root-names-${root_names_cohort}-inputs.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/root-names-${root_names_cohort}"
        --evidence "${CMAKE_CURRENT_SOURCE_DIR}/analysis/root-names-runtime-${root_names_cohort}.json"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/root-names-${root_names_cohort}-regression.json")
    set_tests_properties("root_names_${root_names_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  add_test(NAME environment_prototype_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_environment_prototype.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --report "${CMAKE_CURRENT_BINARY_DIR}/environment-prototype-regression.json")
  set_tests_properties(environment_prototype_reference_replay PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  foreach(environment_cohort IN ITEMS training-a training-b evaluation-a evaluation-b)
    add_test(NAME "environment_${environment_cohort}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_environment.py"
        --cohort "${environment_cohort}"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --report "${CMAKE_CURRENT_BINARY_DIR}/environment-${environment_cohort}-regression.json")
    set_tests_properties("environment_${environment_cohort}_reference_replay" PROPERTIES
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endforeach()
  foreach(root_fields_batch IN ITEMS training evaluation)
    set(root_fields_plan "root-fields-fixture-plan.json")
    if(root_fields_batch STREQUAL "evaluation")
      set(root_fields_plan "root-fields-evaluation-inputs.json")
    endif()
    add_test(NAME "root_fields_${root_fields_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_root_fields.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/root-fields-runtime-${root_fields_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/root-fields-${root_fields_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${root_fields_plan}")
  endforeach()
  foreach(root_tables_batch IN ITEMS training evaluation)
    set(root_tables_plan "root-tables-fixture-plan.json")
    if(root_tables_batch STREQUAL "evaluation")
      set(root_tables_plan "root-tables-evaluation-inputs.json")
    endif()
    add_test(NAME "root_tables_${root_tables_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_root_tables.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/root-tables-runtime-${root_tables_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/root-tables-${root_tables_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${root_tables_plan}")
  endforeach()
  foreach(iteration_batch IN ITEMS training evaluation)
    set(iteration_plan "iteration-controls-fixture-plan.json")
    if(iteration_batch STREQUAL "evaluation")
      set(iteration_plan "iteration-controls-evaluation-inputs.json")
    endif()
    add_test(NAME "iteration_controls_${iteration_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_iteration_controls.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/iteration-controls-runtime-${iteration_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/iteration-controls-${iteration_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${iteration_plan}")
  endforeach()
  foreach(dimensional_batch IN ITEMS training evaluation)
    set(dimensional_plan "dimensional-controls-fixture-plan.json")
    if(dimensional_batch STREQUAL "evaluation")
      set(dimensional_plan "dimensional-controls-evaluation-inputs.json")
    endif()
    add_test(NAME "dimensional_controls_${dimensional_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_dimensional_controls.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dimensional-controls-runtime-${dimensional_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/dimensional-controls-${dimensional_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${dimensional_plan}")
  endforeach()
  foreach(time_step_batch IN ITEMS training evaluation)
    set(time_step_plan "time-step-import-fixture-plan.json")
    if(time_step_batch STREQUAL "evaluation")
      set(time_step_plan "time-step-import-evaluation-inputs.json")
    endif()
    add_test(NAME "time_step_import_${time_step_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_time_step_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-step-import-runtime-${time_step_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/time-step-import-${time_step_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${time_step_plan}")
  endforeach()
  foreach(stopping_vector_batch IN ITEMS training evaluation)
    set(stopping_vector_plan "stopping-vectors-fixture-plan.json")
    if(stopping_vector_batch STREQUAL "evaluation")
      set(stopping_vector_plan "stopping-vectors-evaluation-inputs.json")
    endif()
    add_test(NAME "stopping_vectors_${stopping_vector_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_stopping_vectors.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/stopping-vectors-runtime-${stopping_vector_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/stopping-vectors-${stopping_vector_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${stopping_vector_plan}")
  endforeach()
  foreach(stopping_batch IN ITEMS training evaluation)
    set(stopping_plan "stopping-scalars-fixture-plan.json")
    set(stopping_run "stopping-scalars-training-v2")
    set(stopping_capture "stopping-scalars-runtime-training-v2.json")
    if(stopping_batch STREQUAL "evaluation")
      set(stopping_plan "stopping-scalars-evaluation-inputs.json")
      set(stopping_run "stopping-scalars-evaluation")
      set(stopping_capture "stopping-scalars-runtime-evaluation.json")
    endif()
    add_test(NAME "stopping_scalars_${stopping_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_stopping_scalars.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${stopping_capture}"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/${stopping_run}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${stopping_plan}")
  endforeach()
  foreach(stpinc_batch IN ITEMS training evaluation)
    set(stpinc_plan "stpinc-fixture-plan.json")
    if(stpinc_batch STREQUAL "evaluation")
      set(stpinc_plan "stpinc-evaluation-inputs.json")
    endif()
    add_test(NAME "stpinc_${stpinc_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_stpinc_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/stpinc-runtime-${stpinc_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/stpinc-${stpinc_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${stpinc_plan}")
  endforeach()
  add_test(NAME elmnod_unknown_control_source_guard
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/check_elmnod_unknown_controls.py"
      --driver "$<TARGET_FILE:keyword_import_replay>")
  foreach(elmnod_batch IN ITEMS training evaluation)
    set(elmnod_plan "elmnod-fixture-plan.json")
    if(elmnod_batch STREQUAL "evaluation")
      set(elmnod_plan "elmnod-evaluation-inputs.json")
    endif()
    add_test(NAME "elmnod_${elmnod_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_elmnod_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/elmnod-runtime-${elmnod_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/elmnod-${elmnod_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${elmnod_plan}")
  endforeach()
  foreach(htmthd_batch IN ITEMS training evaluation)
    set(htmthd_plan "htmthd-fixture-plan.json")
    if(htmthd_batch STREQUAL "evaluation")
      set(htmthd_plan "htmthd-evaluation-inputs.json")
    endif()
    add_test(NAME "htmthd_${htmthd_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_htmthd_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/htmthd-runtime-${htmthd_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/htmthd-${htmthd_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${htmthd_plan}")
  endforeach()
  foreach(trans_batch IN ITEMS positive evaluation positive-v2 evaluation-v2 selector-transitions)
    string(REPLACE "-" "_" trans_suffix "${trans_batch}")
    if(trans_batch STREQUAL "positive" OR trans_batch STREQUAL "evaluation")
      string(PREPEND trans_suffix "initial_error_")
    endif()
    add_test(NAME "trans_${trans_suffix}_cpp_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_trans_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/trans-runtime-${trans_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/trans-${trans_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/trans-${trans_batch}-inputs.json")
  endforeach()
  add_test(NAME trans_original_source_audit
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/audit_trans_discovery.py"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/trans-runtime-discovery.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/trans-discovery/run.json"
      --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/trans-discovery-inputs.json")
  foreach(tnow_batch IN ITEMS training evaluation)
    set(tnow_plan "tnow-fixture-plan.json")
    if(tnow_batch STREQUAL "evaluation")
      set(tnow_plan "tnow-evaluation-inputs.json")
    endif()
    add_test(NAME "tnow_${tnow_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_tnow_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/tnow-runtime-${tnow_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/tnow-${tnow_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${tnow_plan}")
  endforeach()
  add_test(NAME unit_discovery_cpp_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_unit_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-runtime-discovery.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/unit-discovery-v2/run.json"
      --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-discovery-inputs.json")
  add_test(NAME unit_evaluation_cpp_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_unit_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-runtime-evaluation.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/unit-evaluation/run.json"
      --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-evaluation-inputs.json")
  add_test(NAME unit_original_source_audit
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/audit_unit_discovery.py"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-runtime-discovery.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/unit-discovery-v2/run.json"
      --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-discovery-inputs.json"
      --static "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-static-evidence.json"
      --data "${CMAKE_CURRENT_SOURCE_DIR}/analysis/unit-static-data.json")
  foreach(meshno_batch IN ITEMS training evaluation)
    set(meshno_plan "meshno-fixture-plan.json")
    if(meshno_batch STREQUAL "evaluation")
      set(meshno_plan "meshno-evaluation-inputs.json")
    endif()
    add_test(NAME "meshno_${meshno_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_meshno_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/meshno-runtime-${meshno_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/meshno-${meshno_batch}-v2/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${meshno_plan}")
  endforeach()
  foreach(stype_batch IN ITEMS training evaluation)
    set(stype_plan "stype-fixture-plan.json")
    if(stype_batch STREQUAL "evaluation")
      set(stype_plan "stype-evaluation-inputs.json")
    endif()
    add_test(NAME "stype_${stype_batch}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_stype_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/stype-runtime-${stype_batch}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/stype-${stype_batch}/run.json"
        --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/${stype_plan}")
  endforeach()
  add_test(NAME smode_training_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_smode_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/smode-runtime-training.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/smode-training/run.json"
      --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/smode-fixture-plan.json")
  add_test(NAME smode_evaluation_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_smode_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/smode-runtime-evaluation.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/smode-evaluation/run.json"
      --plan "${CMAKE_CURRENT_SOURCE_DIR}/analysis/smode-evaluation-inputs.json")
  add_test(NAME grndat_static_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_grndat_static.py"
      --driver "$<TARGET_FILE:keyword_import_replay>")
  foreach(grndat_index RANGE 0 21)
    if(grndat_index LESS 10)
      set(grndat_suffix "0${grndat_index}")
    else()
      set(grndat_suffix "${grndat_index}")
    endif()
    add_test(NAME "grndat_native_${grndat_suffix}_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_grndat_native.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/grndat-runtime-batch-${grndat_suffix}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/grndat-batch-${grndat_suffix}/run.json"
        --batch-id "grndat-batch-${grndat_suffix}")
  endforeach()
  add_test(NAME grndat_native_new_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_grndat_native.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/grndat-runtime-new.json"
      --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/grndat-new-runtime/run.json"
      --batch-id grndat-new-runtime)
  add_test(NAME metadata_import_static_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_metadata_import.py"
      --driver "$<TARGET_FILE:metadata_import_replay>")
  foreach(metadata_suffix training-a training-b held-out)
    if(metadata_suffix STREQUAL "training-a")
      set(metadata_run_dir "metadata-training-a-v2")
    else()
      set(metadata_run_dir "metadata-${metadata_suffix}")
    endif()
    add_test(NAME "metadata_${metadata_suffix}_reference_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_metadata_native.py"
        --driver "$<TARGET_FILE:metadata_import_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/metadata-runtime-${metadata_suffix}.json"
        --run "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/${metadata_run_dir}/run.json")
  endforeach()
  add_test(NAME state_variable_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_state_variables.py"
      --driver "$<TARGET_FILE:state_variable_replay>")
  add_test(NAME mesh_boundary_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_mesh_boundaries.py"
      --driver "$<TARGET_FILE:mesh_boundary_replay>")
  add_test(NAME hexa_boundary_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_hexa_boundaries.py"
      --driver "$<TARGET_FILE:mesh_boundary_replay>")
  add_test(NAME planar_boundary_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_planar_boundaries.py"
      --driver "$<TARGET_FILE:planar_boundary_replay>")
  add_test(NAME mesh_storage_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_mesh_storage.py"
      --driver "$<TARGET_FILE:mesh_storage_replay>")
  add_test(NAME keyword_reader_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_keyword_readers.py"
      --driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME keyword_stream_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_keyword_stream.py"
      --driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME keyword_import_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_keyword_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>")
  add_test(NAME keyword_stream_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_keyword_stream.py"
      --driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/keyword-import-runtime-held-out.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/keyword_import/held_out")
  add_test(NAME keyword_import_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_keyword_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/keyword-import-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/keyword-import-held-out/run.json")
  add_test(NAME component_layout_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_component_layout.py"
      --driver "$<TARGET_FILE:component_layout_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/root-layout-runtime-training.json"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/root-layout-runtime-held-out.json")
  add_test(NAME dimension_import_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_dimension_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>")
  add_test(NAME mesh_refresh_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_mesh_refresh.py"
      --driver "$<TARGET_FILE:mesh_refresh_replay>")
  add_test(NAME configuration_import_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_configuration_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>")
  add_test(NAME import_finalization_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_import_finalization.py"
      --driver "$<TARGET_FILE:keyword_import_replay>")
  add_test(NAME time_controls_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_time_controls.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME time_controls_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_time_controls.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-controls-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/time-controls-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-controls-held-out-cases.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/time_controls/held_out"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-controls-corrected-freeze.json")
  add_test(NAME time_table_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_time_table.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME time_table_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_time_table.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-table-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/time-table-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-table-held-out-cases.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/time_table/held_out"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/time-table-candidate-freeze.json")
  add_test(NAME step_distance_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_step_distance.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME step_distance_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_step_distance.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/step-distance-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/step-distance-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/step-distance-held-out-cases.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/step_distance/held_out"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/step-distance-corrected-freeze.json")
  add_test(NAME geometry_import_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_geometry_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME geometry_import_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_geometry_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/geometry-import-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/geometry-import-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/geometry-import-held-out-cases.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/geometry_import/held_out")
  add_test(NAME geometry_import_lifecycle_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_geometry_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/geometry-import-runtime-lifecycle.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/geometry-import-lifecycle/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/geometry-import-lifecycle-cases.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/geometry_import/lifecycle")
  add_test(NAME geometry_segments_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_geometry_segments.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME geometry_segments_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_geometry_segments.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/geometry-segments-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/geometry-segments-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/geometry-segments-held-out-cases.json"
      --fixtures "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/geometry_segments/held_out")
  add_test(NAME material_import_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME material_import_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-import-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-import-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-import-held-out-cases.json")
  add_test(NAME material_object_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_object_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME material_object_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_object_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-object-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-object-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-object-held-out-cases.json")
  add_test(NAME material_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_reference_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>")
  add_test(NAME material_column_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_column_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-column-input-freeze.json")
  add_test(NAME material_column_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_column_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-column-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-column-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-column-held-out-cases.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-column-held-out-input-freeze.json")
  add_test(NAME mesh_field_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_mesh_field_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-runtime-training-v2.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/mesh-field-training-v2/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-evaluation-cases-v2.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-v2-input-freeze.json")
  add_test(NAME mesh_field_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_mesh_field_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/mesh-field-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-held-out-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/mesh-field-held-out-input-freeze.json")
  add_test(NAME dynamic_field_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_dynamic_field_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-runtime-training.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/dynamic-field-training/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-evaluation-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-input-freeze.json")
  add_test(NAME dynamic_field_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_dynamic_field_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/dynamic-field-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-held-out-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/dynamic-field-held-out-input-freeze.json")
  add_test(NAME custom_field_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_custom_field_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-runtime-training-v2.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/custom-field-training-v2c/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-evaluation-cases-v2.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-v2-input-freeze.json")
  add_test(NAME custom_field_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_custom_field_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-runtime-held-out-v2.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/custom-field-held-out-v2/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-held-out-cases-v2.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/custom-field-held-out-v2-input-freeze.json")
  add_test(NAME material_shape_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_shape_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-runtime-training.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-shape-training/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-evaluation-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-input-freeze-v2.json")
  add_test(NAME material_shape_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_material_shape_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/material-shape-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-held-out-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-candidate-freeze-v2.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/material-shape-held-out-input-freeze.json")
  add_test(NAME relation_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_relation_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-runtime-training.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/relation-training-b/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-evaluation-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-input-freeze.json")
  add_test(NAME relation_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_relation_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/relation-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-held-out-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-held-out-input-freeze.json")
  add_test(NAME property_table_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_property_table_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-runtime-training-v3.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/property-table-training-c/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-evaluation-cases-v3.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-v4-input-freeze.json")
  add_test(NAME property_table_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_property_table_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/property-table-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-held-out-cases-v2.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/property-table-held-out-input-freeze.json")
  add_test(NAME relation_property_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_relation_property_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-runtime-training-v2.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/relation-property-training-b/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-evaluation-cases-v2.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-v2-input-freeze.json")
  add_test(NAME relation_property_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_relation_property_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/relation-property-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-held-out-cases-v2.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/relation-property-held-out-input-freeze.json")
  add_test(NAME nested_material_reference_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_nested_material_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-runtime-training.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/nested-material-training/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-evaluation-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-input-freeze.json")
  add_test(NAME nested_material_held_out_replay
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_nested_material_import.py"
      --driver "$<TARGET_FILE:keyword_import_replay>"
      --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
      --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-runtime-held-out.json"
      --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/nested-material-held-out/run.json"
      --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-held-out-cases.json"
      --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-candidate-freeze.json"
      --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/nested-material-held-out-input-freeze.json")
  foreach(fstres_batch IN ITEMS training-a training-b held-out-a held-out-b)
    string(REPLACE "-" "_" fstres_test_suffix "${fstres_batch}")
    set(fstres_run "fstres-${fstres_batch}")
    if(fstres_batch STREQUAL "training-a" OR fstres_batch STREQUAL "held-out-b")
      string(APPEND fstres_run "-v2")
    endif()
    add_test(NAME "fstres_${fstres_test_suffix}_replay"
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/compare_fstres_import.py"
        --driver "$<TARGET_FILE:keyword_import_replay>"
        --reader-driver "$<TARGET_FILE:keyword_reader_replay>"
        --capture "${CMAKE_CURRENT_SOURCE_DIR}/analysis/fstres-runtime-${fstres_batch}.json"
        --run-record "${CMAKE_CURRENT_SOURCE_DIR}/analysis/oracle/${fstres_run}/run.json"
        --cases "${CMAKE_CURRENT_SOURCE_DIR}/analysis/fstres-${fstres_batch}-cases.json"
        --freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/fstres-candidate-freeze.json"
        --input-freeze "${CMAKE_CURRENT_SOURCE_DIR}/analysis/fstres-${fstres_batch}-input-freeze.json")
  endforeach()
endif()
