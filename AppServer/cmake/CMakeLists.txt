cmake_minimum_required (VERSION 3.2.2)
project (AppServer C CXX)

if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -DROCKSDB_PLATFORM_POSIX -DROCKSDB_LIB_IO_POSIX  -DOS_LINUX -fno-builtin-memcmp -DROCKSDB_FALLOCATE_PRESENT -DROCKSDB_MALLOC_USABLE_SIZE -march=native -pthread -lrt -lz -lsnappy -lbz2")
endif()

file(GLOB CONTROLLER_SRC ../src/controller/*.cpp)
file(GLOB UTILS_SRC ../src/utils/*.cpp)
file(GLOB WEBSERVER_SRC ../src/webserver/mongoose.c)
file(GLOB UTILS_SRC ../src/utils/*.cpp)
file(GLOB JSON_SRC ../src/json/*.cpp)
file(GLOB DAO_SRC ../src/dao/*.cpp)
file(GLOB MODEL_SRC ../src/model/*.cpp)
file(GLOB SERVICE_SRC ../src/service/*.cpp)
file(GLOB EXCEPTION_SRC ../src/exception/*.cpp)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(PkgConfig)

include_directories(../src)
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${BZIP_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})    

add_library(zlib SHARED IMPORTED)
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libz.so)

add_library(bzip2 SHARED IMPORTED)
set_target_properties(bzip2 PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libbz2.so)

add_library(rocksdb STATIC IMPORTED)
set_target_properties(rocksdb PROPERTIES IMPORTED_LOCATION "/usr/local/lib/librocksdb.a" INTERFACE_LINK_LIBRARIES "/usr/lib/x86_64-linux-gnu/libbz2.so;/usr/lib/x86_64-linux-gnu/libz.so;/usr/lib/libsnappy.so")

add_library(log4cplus SHARED IMPORTED)
set_target_properties(log4cplus PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblog4cplus-1.2.so.5.1.4)

add_library(jansson SHARED IMPORTED)
set_target_properties(jansson PROPERTIES IMPORTED_LOCATION /usr/local/lib/libjansson.so)

add_library(jwt SHARED IMPORTED)
set_target_properties(jwt PROPERTIES IMPORTED_LOCATION /usr/local/lib/libjwt.so)



add_library(webserver ${WEBSERVER_SRC})
add_library(controller ${CONTROLLER_SRC})
add_library(utils ${UTILS_SRC})
add_library(dao ${DAO_SRC})
add_library(json ${JSON_SRC} ../src/jsoncpp.cpp)
add_library(model ${MODEL_SRC})
add_library(service ${SERVICE_SRC})
target_link_libraries(dao rocksdb)
target_link_libraries(service dao)
target_link_libraries(model json)

# add the executable
add_executable(AppServer ../src/restful_server.cpp)
target_compile_features(AppServer PRIVATE cxx_range_for)
target_link_libraries(AppServer ${CMAKE_THREAD_LIBS_INIT} rt zlib snappy bzip2 rocksdb log4cplus jansson jwt webserver controller dao service utils  json model)




