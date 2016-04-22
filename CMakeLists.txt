cmake_minimum_required(VERSION 3.5)

set(binary_dir /googletest/googlemock/gtest)
set(source_dir /googletest/googletest)
# We need thread upport
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest/googlemock
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest/googlemock/gtest
    # Disable install step
    INSTALL_COMMAND ""
)
# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set gtest properties
ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")

# Download and install GoogleMock
ExternalProject_Add(
    gmock
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest/googlemock/
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest/googlemock/
    # Disable install step
    INSTALL_COMMAND ""
)

set(binary_dir /googletest/googlemock)
set(source_dir /googletest/googlemock)
# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock)

# Set gmock properties
ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")
