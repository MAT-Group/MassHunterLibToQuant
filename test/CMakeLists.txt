# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(MassHunterLibToQuantTests LANGUAGES CXX)

# ---- Tests ----

add_executable(MassHunterLibToQuant_test  "source/Library.cpp")
target_link_libraries(MassHunterLibToQuant_test PRIVATE MassHunterLibToQuant_lib)
target_compile_features(MassHunterLibToQuant_test PRIVATE cxx_std_20)

add_test(NAME MassHunterLibToQuant_test COMMAND MassHunterLibToQuant_test)

# ---- End-of-file commands ----

add_folders(Test)
