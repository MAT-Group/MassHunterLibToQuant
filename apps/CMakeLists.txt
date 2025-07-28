

add_executable(LibraryToQuantMethod_exe LibraryToQuant.cpp)
add_executable(LibraryToQuantMethod::exe ALIAS LibraryToQuantMethod_exe)

set_property(TARGET LibraryToQuantMethod_exe PROPERTY OUTPUT_NAME LibraryToQuantMethod)

target_compile_features(LibraryToQuantMethod_exe PRIVATE cxx_std_20)

target_link_libraries(LibraryToQuantMethod_exe PRIVATE MassHunterLibToQuant_lib)

# ---- Library to CSV of ions ----

add_executable(LibraryToCSV_exe LibraryToCSV.cpp)
add_executable(LibraryToCSV::exe ALIAS LibraryToCSV_exe)

set_property(TARGET LibraryToCSV_exe PROPERTY OUTPUT_NAME LibraryToCSV)

target_compile_features(LibraryToCSV_exe PRIVATE cxx_std_20)

target_link_libraries(LibraryToCSV_exe PRIVATE MassHunterLibToQuant_lib)

# ---- Library to Score ----

add_executable(LibraryToScore_exe LibraryToScore.cpp)
add_executable(LibraryToScore::exe ALIAS LibraryToScore_exe)

set_property(TARGET LibraryToScore_exe PROPERTY OUTPUT_NAME LibraryToScore)

target_compile_features(LibraryToScore_exe PRIVATE cxx_std_20)

target_link_libraries(LibraryToScore_exe PRIVATE MassHunterLibToQuant_lib)
