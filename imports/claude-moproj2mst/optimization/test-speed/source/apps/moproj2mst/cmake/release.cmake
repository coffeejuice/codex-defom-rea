# Release packaging and checks for moproj2mst.dll (owner: P18 front half; included from the
# end of CMakeLists.txt).
#
#   version resource      cmake/version.rc.in -> moproj2mst.dll and moproj2mst.exe
#   m2m.release.exports   dumpbin: exactly the five C ABI exports, x64, no CRT DLL (static /MT),
#                         file version = project version
#   m2m.release.sample_*  the caller samples in samples/ (C++, C# P/Invoke, Python ctypes) load
#                         the DLL and convert a missing project (M2M_E_OPEN)
#
# Label: m2m-release (ctest -L m2m-release).

if(NOT MSVC)
    return()
endif()

# ---- version resource -----------------------------------------------------------------------------
enable_language(RC)
set(M2M_ABI_VERSION 1)   # M2M_API_VERSION in include/moproj2mst.h
foreach(kind dll exe)
    if(kind STREQUAL "dll")
        set(M2M_RC_FILENAME "moproj2mst.dll")
        set(M2M_RC_FILETYPE "VFT_DLL")
        set(M2M_RC_DESCRIPTION "moproj2mst: DEFORM .moproj to .MST converter (C ABI ${M2M_ABI_VERSION})")
    else()
        set(M2M_RC_FILENAME "moproj2mst.exe")
        set(M2M_RC_FILETYPE "VFT_APP")
        set(M2M_RC_DESCRIPTION "moproj2mst: DEFORM .moproj to .MST converter, command line")
    endif()
    configure_file("${CMAKE_CURRENT_LIST_DIR}/version.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/m2m_version_${kind}.rc" @ONLY)
endforeach()
target_sources(moproj2mst PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/m2m_version_dll.rc")
target_sources(moproj2mst_cli PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/m2m_version_exe.rc")

# ---- exports / dependencies / version check -------------------------------------------------------
get_filename_component(m2m_tools_dir "${CMAKE_LINKER}" DIRECTORY)
find_program(M2M_DUMPBIN dumpbin HINTS "${m2m_tools_dir}")
if(M2M_DUMPBIN)
    add_test(NAME m2m.release.exports
             COMMAND "${CMAKE_COMMAND}" -DDLL=$<TARGET_FILE:moproj2mst> -DDUMPBIN=${M2M_DUMPBIN}
                     -DVERSION=${PROJECT_VERSION}.${M2M_ABI_VERSION}
                     -P "${CMAKE_CURRENT_LIST_DIR}/check_exports.cmake")
    set_tests_properties(m2m.release.exports PROPERTIES LABELS "m2m;m2m-release")
endif()

# ---- caller samples ---------------------------------------------------------------------------------
set(m2m_samples "${CMAKE_CURRENT_SOURCE_DIR}/samples")
set(m2m_missing "${CMAKE_BINARY_DIR}/no such folder/missing.moproj")
# Every sample prints "api 1", "unit <m2m_set_gui_unit(0) result>" and "result <code>" and exits
# with the result code.
set(m2m_sample_pass "api 1.*unit 0.*result 2")

add_executable(m2m_sample_cpp "${m2m_samples}/cpp/convert.cpp")
target_include_directories(m2m_sample_cpp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(m2m_sample_cpp PRIVATE moproj2mst)
target_compile_options(m2m_sample_cpp PRIVATE /W4 /utf-8)
target_compile_definitions(m2m_sample_cpp PRIVATE UNICODE _UNICODE)
add_test(NAME m2m.release.sample_cpp COMMAND m2m_sample_cpp "${m2m_missing}")

if(Python3_FOUND)
    add_test(NAME m2m.release.sample_python
             COMMAND "${Python3_EXECUTABLE}" -I "${m2m_samples}/python/moproj2mst.py"
                     --dll $<TARGET_FILE:moproj2mst> "${m2m_missing}")
endif()

# C#: Roslyn csc from the Build Tools, .NET Framework 4.x, x64 (the DLL is x64 only).
find_program(M2M_CSC csc
    HINTS "${m2m_tools_dir}/../../../../../../MSBuild/Current/Bin/Roslyn"
          "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/MSBuild/Current/Bin/Roslyn"
          "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn"
    NO_DEFAULT_PATH)
if(M2M_CSC)
    set(m2m_cs_exe "${CMAKE_BINARY_DIR}/bin/m2m_sample_csharp.exe")
    add_custom_command(OUTPUT "${m2m_cs_exe}"
        COMMAND "${M2M_CSC}" -nologo -platform:x64 -optimize -out:${m2m_cs_exe}
                "${m2m_samples}/csharp/Moproj2Mst.cs" "${m2m_samples}/csharp/Sample.cs"
        DEPENDS "${m2m_samples}/csharp/Moproj2Mst.cs" "${m2m_samples}/csharp/Sample.cs"
        VERBATIM)
    add_custom_target(m2m_sample_csharp ALL DEPENDS "${m2m_cs_exe}" moproj2mst)
    # The sample finds moproj2mst.dll next to itself (bin/).
    add_test(NAME m2m.release.sample_csharp COMMAND "${m2m_cs_exe}" "${m2m_missing}")
endif()

foreach(t sample_cpp sample_python sample_csharp)
    if(TEST m2m.release.${t})
        set_tests_properties(m2m.release.${t} PROPERTIES LABELS "m2m;m2m-release"
                             PASS_REGULAR_EXPRESSION "${m2m_sample_pass}")
    endif()
endforeach()
