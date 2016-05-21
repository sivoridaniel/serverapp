/*
 * MockMatchService.h
 *
 *  Created on: 1 de mayo de 2016
 *      Author: agustin
 */

#include "gmock/gmock.h"
#include "../src/service/MatchService.h"
#include "../src/service/RemoteSharedService.h"
#include "../src/dao/MatchDao.h"
#include "../src/dao/ChatDao.h"
#include "../src/service/ChatService.h"
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
	MOCK_CONST_METHOD2( MockFunctionPut, void(string id, Entity* e) );
	virtual void put(string id, Entity * e) const throw(InvalidEntityException)
	{
			MockFunctionPut(id,e);
	}
};

class MockChatDao : public ChatDao{
public:

	MOCK_CONST_METHOD1(MockFunctionGet, Entity*(string id) );
	virtual Entity* get(string id) const throw(EntityNotFoundException)
	{
		if (id.compare("alinari-psivori")==0){
			throw EntityNotFoundException();
		}
	    return MockFunctionGet(id);
	}
	MOCK_CONST_METHOD2( MockFunctionPut, void(string id, Entity* e) );
	virtual void put(string id, Entity * e) const throw(InvalidEntityException)
	{
			MockFunctionPut(id,e);
	}
};

class MockChatService : public ChatService{
public:
	MockChatService(ChatDao* chatDao):ChatService(chatDao){}

	MOCK_CONST_METHOD2(MockFunctionCreateChat, void(string idUser1,  string idUser2) );
	virtual void createChat(string idUser1, string idUser2) const
	{
	    MockFunctionCreateChat(idUser1, idUser2);
	}

};

class MockSharedService : public RemoteSharedService{
public:

	MOCK_CONST_METHOD1(MockFunctionGetUser, UserProfile*(string id) );
	UserProfile* getUser(string id){
		return MockFunctionGetUser(id);
	}

};

TEST(MatchServiceTest,confirmingUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->addNewMatch("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
	EXPECT_NO_THROW({matchService->confirmUser("psivori","alinari");});

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
