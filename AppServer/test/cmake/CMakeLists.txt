########################################################################
# CMake build script for Google Test.
#
# To run the tests for Google Test itself on Linux, use 'make test' or
# ctest.  You can select which tests to run using 'ctest -R regex'.
# For more options, run 'ctest --help'.

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

# When other libraries are using a shared version of runtime libraries,
# Google Test also has to use one.
option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  OFF)

option(gtest_build_tests "Build all of gtest's own tests." OFF)

option(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)

option(
  gtest_hide_internal_symbols
  "Build gtest with internal symbols hidden in shared libraries."
  OFF)

# Defines pre_project_set_up_hermetic_build() and set_up_hermetic_build().
include(cmake/hermetic_build.cmake OPTIONAL)

if (COMMAND pre_project_set_up_hermetic_build)
  pre_project_set_up_hermetic_build()
endif()

########################################################################
#
# Project-wide settings

# Name of the project.
#
# CMake files in this project can refer to the root source directory
# as ${gtest_SOURCE_DIR} and to the root binary directory as
# ${gtest_BINARY_DIR}.
# Language "C" is required for find_package(Threads).
project(gtest CXX C)
cmake_minimum_required(VERSION 2.6.2)

if (COMMAND set_up_hermetic_build)
  set_up_hermetic_build()
endif()

if (gtest_hide_internal_symbols)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()

# Define helper functions and macros used by Google Test.
include(internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Where Google Test's .h files can be found.
include_directories(
  ${gtest_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR})

# Where Google Test's libraries can be found.
link_directories(${gtest_BINARY_DIR}/src)

# Summary of tuple support for Microsoft Visual Studio:
# Compiler    version(MS)  version(cmake)  Support
# ----------  -----------  --------------  -----------------------------
# <= VS 2010  <= 10        <= 1600         Use Google Tests's own tuple.
# VS 2012     11           1700            std::tr1::tuple + _VARIADIC_MAX=10
# VS 2013     12           1800            std::tr1::tuple
if (MSVC AND MSVC_VERSION EQUAL 1700)
  add_definitions(/D _VARIADIC_MAX=10)
endif()

########################################################################
#
# Defines the gtest & gtest_main libraries.  User tests should link
# with one of them.

# Google Test libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that gtest can be compiled by a user
# aggressive about warnings.
cxx_library(gtest "${cxx_strict}" ../include/src/gtest-all.cc)
cxx_library(gtest_main "${cxx_strict}" ../include/src/gtest_main.cc)
target_link_libraries(gtest_main gtest)

# If the CMake version supports it, attach header directory information
# to the targets for when we are part of a parent build (ie being pulled
# in via add_subdirectory() rather than being a standalone build).
if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
  target_include_directories(gtest      INTERFACE "${gtest_SOURCE_DIR}/include")
  target_include_directories(gtest_main INTERFACE "${gtest_SOURCE_DIR}/include")
endif()

########################################################################
#
# Install rules
install(TARGETS gtest gtest_main
  DESTINATION lib)
install(DIRECTORY ${gtest_SOURCE_DIR}/include/gtest
  DESTINATION include)

######################################################################
#
# The tests are not built by default.  To build them, set the
# gtest_build_tests option to ON.  You can do it by running ccmake
# or specifying the -Dgtest_build_tests=ON flag when running cmake.

if (gtest_build_tests)
  # This must be set in the root directory for the tests to be run by
  # 'make test' or ctest.
  enable_testing()

  ############################################################
  # C++ tests built with standard compiler flags.
  cxx_test(AuthenticationControllerTest gtest_main ../../src/controller/AuthenticationController.cpp)
# cxx_executable(AuthenticationControllerTest ../ gtest_main ../../src/controller/AuthenticationController.cpp)
#  cxx_test(gtest_pred_impl_unittest gtest_main)
#  cxx_test(gtest_premature_exit_test gtest
#    test/gtest_premature_exit_test.cc)
#  cxx_test(gtest-printers_test gtest_main)
#  cxx_test(gtest_prod_test gtest_main
#    test/production.cc)
#  cxx_test(gtest_repeat_test gtest)
#  cxx_test(gtest_sole_header_test gtest_main)
#  cxx_test(gtest_stress_test gtest)
#  cxx_test(gtest-test-part_test gtest_main)
#  cxx_test(gtest_throw_on_failure_ex_test gtest)
#  cxx_test(gtest-typed-test_test gtest_main
#    test/gtest-typed-test2_test.cc)
#  cxx_test(gtest_unittest gtest_main)
#  cxx_test(gtest-unittest-api_test gtest)

  ############################################################
  # C++ tests built with non-standard compiler flags.

  # MSVC 7.1 does not support STL with exceptions disabled.
 # if (NOT MSVC OR MSVC_VERSION GREATER 1310)
 #   cxx_library(gtest_no_exception "${cxx_no_exception}"
 #     src/gtest-all.cc)
 #   cxx_library(gtest_main_no_exception "${cxx_no_exception}"
 #     src/gtest-all.cc src/gtest_main.cc)
 # endif()
 # cxx_library(gtest_main_no_rtti "${cxx_no_rtti}"
 #   src/gtest-all.cc src/gtest_main.cc)

 # cxx_test_with_flags(gtest-death-test_ex_nocatch_test
 #   "${cxx_exception} -DGTEST_ENABLE_CATCH_EXCEPTIONS_=0"
 #   gtest test/gtest-death-test_ex_test.cc)
 # cxx_test_with_flags(gtest-death-test_ex_catch_test
 #   "${cxx_exception} -DGTEST_ENABLE_CATCH_EXCEPTIONS_=1"
 #   gtest test/gtest-death-test_ex_test.cc)

 # cxx_test_with_flags(gtest_no_rtti_unittest "${cxx_no_rtti}"
 #   gtest_main_no_rtti test/gtest_unittest.cc)

 # cxx_shared_library(gtest_dll "${cxx_default}"
 #   src/gtest-all.cc src/gtest_main.cc)

 # cxx_executable_with_flags(gtest_dll_test_ "${cxx_default}"
 #   gtest_dll test/gtest_all_test.cc)
 # set_target_properties(gtest_dll_test_
 #                       PROPERTIES
 #                       COMPILE_DEFINITIONS "GTEST_LINKED_AS_SHARED_LIBRARY=1")

 # if (NOT MSVC OR MSVC_VERSION LESS 1600)  # 1600 is Visual Studio 2010.
    # Visual Studio 2010, 2012, and 2013 define symbols in std::tr1 that
    # conflict with our own definitions. Therefore using our own tuple does not
    # work on those compilers.
 #   cxx_library(gtest_main_use_own_tuple "${cxx_use_own_tuple}"
 #     ../gtest-all.cc src/gtest_main.cc)

 #   cxx_test_with_flags(gtest-tuple_test "${cxx_use_own_tuple}"
 #     gtest_main_use_own_tuple test/gtest-tuple_test.cc)

 #   cxx_test_with_flags(gtest_use_own_tuple_test "${cxx_use_own_tuple}"
 #     gtest_main_use_own_tuple
 #     test/gtest-param-test_test.cc test/gtest-param-test2_test.cc)
 # endif()
  
endif()
