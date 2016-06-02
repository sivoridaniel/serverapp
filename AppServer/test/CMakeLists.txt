cmake_minimum_required(VERSION 3.2.2)
project(test)

find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

set(DIR_APP ../..)

set(OUTPUT_NAME coverageReport)

set(GTEST_DIR ../../googletest)

set(CMAKE_CURRENT_SOURCE_DIR ../cmake)

include_directories(SYSTEM ${GTEST_DIR}/googletest/include ${GTEST_DIR}/googlemock/include) 

enable_testing()

add_subdirectory(${GTEST_DIR} /googlemock)

include_directories(SYSTEM ${GTEST_DIR}/googletest/include ${GTEST_DIR}/googlemock/include) 

set(UNITTEST_SRC ../test/MockAuthenticationController.cc)
set(SRC_FILES ${WEBSERVER_SRC} ${CONTROLLER_SRC} ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_SRC})

set(UNITTEST_MATCH_SRC ../test/MockMatchService.cc)
set(SRC_FILES_MATCH ${WEBSERVER_SRC} ${CONTROLLER_SRC} ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_MATCH_SRC})

set(UNITTEST_CHAT_SRC ../test/MockChatService.cc)
set(SRC_FILES_CHAT ${WEBSERVER_SRC} ${CONTROLLER_SRC} ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_CHAT_SRC})

set(UNITTEST_ABMUSER_SRC ../test/MockABMUsersService.cc)
set(SRC_FILES_ABMUSER ${WEBSERVER_SRC} ${CONTROLLER_SRC} ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_ABMUSER_SRC})

add_executable(mockMatchServiceTest ${SRC_FILES_MATCH}) 
add_executable(mockAuthenticationCtrlTest ${SRC_FILES})
add_executable(mockChatServiceTest ${SRC_FILES_CHAT}) 
add_executable(mockABMUsersServiceTest ${SRC_FILES_ABMUSER}) 

target_link_libraries(mockAuthenticationCtrlTest utils dao model service controller gmock gmock_main rocksdb log4cplus jansson jwt json webserver ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockMatchServiceTest utils dao model service controller gmock gmock_main rocksdb log4cplus jansson jwt json webserver ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockChatServiceTest utils dao model service controller gmock gmock_main rocksdb log4cplus jansson jwt json webserver ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockABMUsersServiceTest utils dao model service controller gmock gmock_main rocksdb log4cplus jansson jwt json webserver ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})

add_test(NAME mockAuthenticationCtrlTest COMMAND mockAuthenticationCtrlTest)
add_test(NAME mockMatchServiceTest COMMAND mockMatchServiceTest)
add_test(NAME mockChatServiceTest COMMAND mockChatServiceTest)
add_test(NAME mockABMUsersServiceTest COMMAND mockABMUsersServiceTest)

add_custom_target(cov COMMAND ${LCOV_PATH} --directory . --zerocounters
COMMAND mockAuthenticationCtrlTest
COMMAND mockMatchServiceTest
COMMAND mockChatServiceTest
COMMAND mockABMUsersServiceTest
# Capture lcov counters
COMMAND ${LCOV_PATH} --directory ${DIR_APP} --capture --output-file ${OUTPUT_NAME}.info
 # Remove googletest coverage results
COMMAND ${LCOV_PATH} --remove ${OUTPUT_NAME}.info '/usr/*' 'gtest*' 'include/*' --output-file ${OUTPUT_NAME}.info.cleaned
COMMAND ${GENHTML_PATH} -o ${OUTPUT_NAME} ${OUTPUT_NAME}.info.cleaned --legend -s
COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_NAME}.info ${OUTPUT_NAME}.info.cleaned
WORKING_DIRECTORY ${DIR_APP}/AppServer/test COMMENT "Generating code coverage results.")


