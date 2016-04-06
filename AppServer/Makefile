CC=cc
CXX=g++
PLATFORM=OS_LINUX
PLATFORM_LDFLAGS= -lpthread -lrt -lz -lsnappy -lbz2
VALGRIND_VER=
PLATFORM_CCFLAGS= -DROCKSDB_PLATFORM_POSIX -DROCKSDB_LIB_IO_POSIX  -DOS_LINUX -fno-builtin-memcmp -DROCKSDB_FALLOCATE_PRESENT -DROCKSDB_MALLOC_USABLE_SIZE -march=native 
PLATFORM_CXXFLAGS=-std=c++11  -DROCKSDB_PLATFORM_POSIX -DROCKSDB_LIB_IO_POSIX  -DOS_LINUX -fno-builtin-memcmp -DROCKSDB_FALLOCATE_PRESENT -DROCKSDB_MALLOC_USABLE_SIZE -march=native 
EXEC_LDFLAGS=
PROG = app_server
SOURCE_DIR= src
BUILD_DIR= build/$(PROG)
SOURCES = $(SOURCE_DIR)/*.cpp $(SOURCE_DIR)/webserver/*.c $(SOURCE_DIR)/model/*.cpp $(SOURCE_DIR)/dao/*.cpp
CFLAGS = -W -Wall  $(CFLAGS_EXTRA) 
LIBS = -lrocksdb -llog4cplus


ifeq ($(SSL), openssl)
	CFLAGS += -DMG_ENABLE_SSL -lssl -lcrypto -lcrypto
endif


all: $(BUILD_DIR)

$(BUILD_DIR): $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) -DDEBUG -g -o $@ $(CFLAGS) $(LIBS) $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(BUILD_DIR)

