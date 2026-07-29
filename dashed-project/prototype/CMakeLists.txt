cmake_minimum_required(VERSION 3.21)
project(Dashed VERSION 0.1.0 LANGUAGES CXX)

option(DASHED_BUILD_TESTS "Build Dashed tests" ON)
option(DASHED_BUILD_BENCHMARKS "Build Dashed microbenchmarks" ON)
option(DASHED_ENABLE_SANITIZERS "Enable ASan and UBSan on supported compilers" OFF)

add_library(dashed_core
  src/branching.cpp
  src/domain.cpp
  src/literal_slice.cpp
  src/propagation.cpp
  src/sweep.cpp
  src/value_set.cpp
)
add_library(Dashed::core ALIAS dashed_core)
set_target_properties(dashed_core PROPERTIES EXPORT_NAME core)

target_include_directories(dashed_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(dashed_core PUBLIC cxx_std_17)
target_compile_options(dashed_core PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Wconversion;-Wshadow>
  $<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->
)

if(DASHED_ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(dashed_core PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(dashed_core PUBLIC -fsanitize=address,undefined)
endif()

add_executable(dashed_example examples/basic.cpp)
target_link_libraries(dashed_example PRIVATE Dashed::core)

if(DASHED_BUILD_TESTS)
  enable_testing()
  add_executable(dashed_core_tests tests/test_core.cpp)
  target_link_libraries(dashed_core_tests PRIVATE Dashed::core)
  add_test(NAME dashed_core_tests COMMAND dashed_core_tests)

  add_executable(dashed_property_tests tests/test_properties.cpp)
  target_link_libraries(dashed_property_tests PRIVATE Dashed::core)
  add_test(NAME dashed_property_tests COMMAND dashed_property_tests)

  add_executable(
    dashed_sweep_tests
    tests/test_sweep.cpp
  )
  target_link_libraries(
    dashed_sweep_tests
    PRIVATE Dashed::core
  )
  add_test(
    NAME dashed_sweep_tests
    COMMAND dashed_sweep_tests
  )

  find_package(Python3 COMPONENTS Interpreter QUIET)
  if(Python3_Interpreter_FOUND)
    add_test(
      NAME dashed_vis_validation
      COMMAND ${Python3_EXECUTABLE}
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/validate_vis.py
              ${CMAKE_CURRENT_SOURCE_DIR}/integration/gecode/gecode/list/var-imp/list.vis
    )
  endif()
endif()

if(DASHED_BUILD_BENCHMARKS)
  add_executable(dashed_fixed_sequence_bench bench/fixed_sequence_bench.cpp)
  target_link_libraries(dashed_fixed_sequence_bench PRIVATE Dashed::core)
  add_executable(dashed_old_style_compaction_bench
    bench/old_style_compaction_bench.cpp)
  target_link_libraries(dashed_old_style_compaction_bench PRIVATE Dashed::core)
endif()


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS dashed_core
  EXPORT DashedTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT DashedTargets
  FILE DashedTargets.cmake
  NAMESPACE Dashed::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dashed
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/DashedConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DashedConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/DashedConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dashed
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/DashedConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/DashedConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dashed
)
