cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    MassHunterLibToQuant
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    MassHunterLibToQuant_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    MassHunterLibToQuant_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(MassHunterLibToQuant_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(MassHunterLibToQuant_exe source/main.cpp)
add_executable(MassHunterLibToQuant::exe ALIAS MassHunterLibToQuant_exe)

set_property(TARGET MassHunterLibToQuant_exe PROPERTY OUTPUT_NAME MassHunterLibToQuant)

target_compile_features(MassHunterLibToQuant_exe PRIVATE cxx_std_20)

target_link_libraries(MassHunterLibToQuant_exe PRIVATE MassHunterLibToQuant_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT MassHunterLibToQuant_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of MassHunterLibToQuant"
  )
endif()

include(cmake/dev-mode.cmake)
