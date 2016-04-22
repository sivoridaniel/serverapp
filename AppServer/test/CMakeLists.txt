cmake_minimum_required(VERSION 3.2.2)
project (test)

set(GTEST_DIR ../../googletest)

add_subdirectory(${GTEST_DIR} /googlemock)

include_directories(SYSTEM ${GTEST_DIR}/googletest/include ${GTEST_DIR}/googlemock/include)

add_executable(authenticationctrltest MockAuthenticationController.cpp)
target_link_libraries(authenticationctrltest gmock gmock_main jwt)
