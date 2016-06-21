cmake_minimum_required(VERSION 3.2.2)
project(test)

find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

set(DIR_APP ../..)

set(OUTPUT_NAME coverageReport)

set(GTEST_DIR ../../googletest)

set(CMAKE_CURRENT_SOURCE_DIR ../cmake)

#include_directories(SYSTEM ${GTEST_DIR}/googletest/include ${GTEST_DIR}/googlemock/include) 

enable_testing()

add_subdirectory(${GTEST_DIR} /googlemock)

include_directories(SYSTEM ${GTEST_DIR}/googletest/include ${GTEST_DIR}/googlemock/include)

set(UNITTEST_SRC ../test/MockAuthenticationService.cc)
set(SRC_FILES  ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_SRC})

set(UNITTEST_MATCH_SRC ../test/MockMatchService.cc)
set(SRC_FILES_MATCH ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_MATCH_SRC})

set(UNITTEST_CHAT_SRC ../test/MockChatService.cc)
set(SRC_FILES_CHAT  ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_CHAT_SRC})

set(UNITTEST_ABMUSER_SRC ../test/MockABMUsersService.cc)
set(SRC_FILES_ABMUSER  ${UTILS_SRC} ${DAO_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_ABMUSER_SRC})

set(UNITTEST_SHARED_SRC ../test/MockRemoteSharedService.cc)
set(SRC_FILES_SHARED  ${UTILS_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${UNITTEST_SHARED_SRC})

set(SEARCH_CTRL_SRC ../test/MockSearchCandidatesCtrl.cc)
set(SRC_SEARCH_CTRL  ${UTILS_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${CONTROLLER_SRC} ${SEARCH_CTRL_SRC})

set(CHAT_CTRL_SRC ../test/MockChatCtrl.cc)
set(SRC_CHAT_CTRL  ${UTILS_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${CONTROLLER_SRC} ${CHAT_CTRL_SRC})

set(MATCH_CTRL_SRC ../test/MockMatchCtrl.cc)
set(SRC_MATCH_CTRL  ${UTILS_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${CONTROLLER_SRC} ${MATCH_CTRL_SRC})

set(AUTH_CTRL_SRC ../test/MockAuthenticationCtrl.cc)
set(SRC_AUTH_CTRL  ${UTILS_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${CONTROLLER_SRC} ${AUTH_CTRL_SRC})

set(ABM_CTRL_SRC ../test/MockABMUserCtrl.cc)
set(SRC_ABM_CTRL  ${UTILS_SRC} ${JSON_SRC} ${MODEL_SRC} ${SERVICE_SRC} ${CONTROLLER_SRC} ${ABM_CTRL_SRC})

add_executable(mockMatchServiceTest ${SRC_FILES_MATCH}) 
add_executable(mockAuthenticationServiceTest ${SRC_FILES})
add_executable(mockChatServiceTest ${SRC_FILES_CHAT}) 
add_executable(mockABMUsersServiceTest ${SRC_FILES_ABMUSER}) 
add_executable(mockSharedServiceTest ${SRC_FILES_SHARED})
add_executable(mockSearchCtrlTest ${SRC_SEARCH_CTRL})
add_executable(mockChatCtrlTest ${SRC_CHAT_CTRL})
add_executable(mockMatchCtrlTest ${SRC_MATCH_CTRL})
add_executable(mockAuthCtrlTest ${SRC_AUTH_CTRL})
add_executable(mockAbmUserCtrlTest ${SRC_ABM_CTRL})

target_link_libraries(mockAuthenticationServiceTest utils dao model service  gmock gmock_main  log4cplus jansson jwt json  ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockMatchServiceTest utils dao model service  gmock gmock_main  log4cplus json  ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockChatServiceTest utils dao model service  gmock gmock_main  log4cplus json  ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockABMUsersServiceTest utils dao model service  gmock gmock_main  log4cplus jansson jwt json  ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockSharedServiceTest utils model service  gmock gmock_main  log4cplus json ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockSearchCtrlTest utils model service controller webserver gmock gmock_main  log4cplus json ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockChatCtrlTest utils model service controller webserver gmock gmock_main  log4cplus json ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockMatchCtrlTest utils model service controller webserver gmock gmock_main  log4cplus json ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockAuthCtrlTest utils model service controller webserver gmock gmock_main  log4cplus json ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})
target_link_libraries(mockAbmUserCtrlTest utils model service controller webserver gmock gmock_main  log4cplus json ${LibXML2_LIBRARIES} ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES} ${GCOV_LINK_FLAGS})

add_test(NAME mockAuthenticationServiceTest COMMAND mockAuthenticationServiceTest)
add_test(NAME mockMatchServiceTest COMMAND mockMatchServiceTest)
add_test(NAME mockChatServiceTest COMMAND mockChatServiceTest)
add_test(NAME mockABMUsersServiceTest COMMAND mockABMUsersServiceTest)
add_test(NAME mockSharedServiceTest COMMAND mockSharedServiceTest)
add_test(NAME mockSearchCtrlTest COMMAND mockSearchCtrlTest)
add_test(NAME mockChatCtrlTest COMMAND mockChatCtrlTest)
add_test(NAME mockMatchCtrlTest COMMAND mockMatchCtrlTest)
add_test(NAME mockAuthCtrlTest COMMAND mockAuthCtrlTest)
add_test(NAME mockAbmUserCtrlTest COMMAND mockAbmUserCtrlTest)

add_custom_target(cov COMMAND ${LCOV_PATH} --directory . --zerocounters
COMMAND mockAuthenticationServiceTest
COMMAND mockMatchServiceTest
COMMAND mockChatServiceTest
COMMAND mockABMUsersServiceTest
COMMAND mockSharedServiceTest
COMMAND mockSearchCtrlTest
COMMAND mockChatCtrlTest
COMMAND mockMatchCtrlTest
COMMAND mockAuthCtrlTest
COMMAND mockAbmUserCtrlTest
# Capture lcov counters
COMMAND ${LCOV_PATH} --directory ${DIR_APP} --capture --output-file ${OUTPUT_NAME}.info
 # Remove googletest coverage results
COMMAND ${LCOV_PATH} --remove ${OUTPUT_NAME}.info '/usr/*' 'googletest*' 'googlemock*' 'src/json*' --output-file ${OUTPUT_NAME}.info
COMMAND ${GENHTML_PATH} -o ${OUTPUT_NAME} ${OUTPUT_NAME}.info --legend -s
COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_NAME}.info
WORKING_DIRECTORY ${DIR_APP}/AppServer/test COMMENT "Generating code coverage results.")


