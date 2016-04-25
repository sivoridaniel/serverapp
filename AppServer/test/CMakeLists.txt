cmake_minimum_required(VERSION 3.2.2)
project (test)

set(UTILS_DIR ../src/utils)
set(EXCEPTION_DIR ../src/exception)

set(UTILS_FILES_H ${UTILS_DIR}/JwToken.h)
set(UTILS_FILES_CPP ${UTILS_DIR}/JwToken.cpp)

set(EXCEPTION_FILES ${EXCEPTION_DIR}/*.h)

set(GTEST_DIR ../../googletest)

add_subdirectory(${GTEST_DIR} /googlemock)

include_directories(SYSTEM ${GTEST_DIR}/googletest/include ${GTEST_DIR}/googlemock/include) 

add_executable(authenticationctrltest MockAuthenticationController.cpp ${UTILS_FILES_CPP} ${UTILS_FILES_H})
target_link_libraries(authenticationctrltest gmock gmock_main jwt log4cplus)
