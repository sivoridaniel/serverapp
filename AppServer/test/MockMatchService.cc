/*
 * MockMatchService.h
 *
 *  Created on: 1 de mayo de 2016
 *      Author: agustin
 */

#include "gmock/gmock.h"
#include "../src/service/MatchService.h"
#include "../src/dao/MatchDao.h"
#include "../src/dao/ChatDao.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;

class MockMatchDao : public MatchDao{
public:

	MOCK_CONST_METHOD1(MockFunctionGet, Entity*(string id) );
	virtual Entity* get(string id) const throw(EntityNotFoundException)
	{
	    return MockFunctionGet(id);
	}
	MOCK_CONST_METHOD1( MockFunctionPut, void(Entity* e) );
	virtual void put(Entity * e) const throw(InvalidEntityException)
	{
			MockFunctionPut(e);
	}
};

class MockChatDao : public ChatDao{
public:

	MOCK_CONST_METHOD1(MockFunctionGet, Entity*(string id) );
	virtual Entity* get(string id) const throw(EntityNotFoundException)
	{
	    return MockFunctionGet(id);
	}
	MOCK_CONST_METHOD1( MockFunctionPut, void(Entity* e) );
	virtual void put(Entity * e) const throw(InvalidEntityException)
	{
			MockFunctionPut(e);
	}
};

TEST(MatchServiceTest,confirmingUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	Match* match = new Match();
	match->addNewMatch("alinari");
	Chat* chat = new Chat();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatDao);
	EXPECT_NO_THROW({matchService->confirmUser("psivori","alinari");});

	delete match;
	delete chat;
	delete matchService;
}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockMatchServiceTest
 */
int main(int argc, char* argv[]){
	::testing::InitGoogleMock(&argc, argv);

	initialize();
	BasicConfigurator config;
	config.configure();

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando Tests"));

	return RUN_ALL_TESTS();
}
