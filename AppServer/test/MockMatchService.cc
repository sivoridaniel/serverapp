/*
 * MockMatchService.h
 *
 *  Created on: 1 de mayo de 2016
 *      Author: agustin
 */

#include "gmock/gmock.h"
#include "../src/service/impl/MatchService.h"
#include "../src/service/impl/RemoteSharedService.h"
#include "../src/dao/MatchDao.h"
#include "../src/dao/ChatDao.h"
#include "../src/dao/SearchStatsDao.h"
#include "../src/service/impl/ChatService.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class MockMatchDao : public MatchDao{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2( put, void(string id, Entity* e) );

};

class MockChatDao : public ChatDao{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2( put, void(string id, Entity* e) );

};

class MockSearchStatsDao : public SearchStatsDao{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};

ACTION(ThrowEntityNotFoundException){
	throw EntityNotFoundException();
}

ACTION(ThrowIllegalStateException){
	throw IllegalStateException();
}

ACTION(ThrowEntityExistsException){
	throw EntityExistsException();
}

class MockChatService : public IChatService{
private:
	MockChatDao* chatDao;
public:
	MockChatService(MockChatDao* chatDao){
		this->chatDao = chatDao;
	}

	~MockChatService(){
		delete chatDao;
	}

	MOCK_METHOD2(createChat, void(string idUser1,  string idUser2) );
	MOCK_METHOD2(getAllMessages, vector<Message*>(string idUser1,  string idUser2) );
	MOCK_METHOD2(getNewMessages, vector<Message*>(string idUser1,  string idUser2) );
	MOCK_METHOD3(addNewMessage, void(string idUser1,  string idUser2,  string message) );
	MOCK_METHOD3(updateLastMessageSeen, void(string idUser1,  string idUser2, int messageIndex) );
	MOCK_METHOD2(getChat, Chat*(string idUser1,  string idUser2) );

};

class MockSharedService : public IRemote{
public:

	MOCK_CONST_METHOD1(MockFunctionGetUser, UserProfile*(string id) );
	virtual UserProfile* getUser(string id){
		if (id.compare("alinari")==0){
			UserProfile* user = new UserProfile("alinari", "1");
			return user;
		}
		else{
			UserProfile* user = new UserProfile("jferrio", "0");
			return user;
		}
	}

	MOCK_METHOD1(createInterest, void (Interest* interest));
	MOCK_METHOD0(getInterests, list<Interest*>(void));
	MOCK_METHOD0(getUsers, list<UserProfile*>(void));
	MOCK_METHOD1(createUser, void (UserProfile* userProfile));
	MOCK_METHOD1(deleteUser, void (string id));
	MOCK_METHOD1(updateUser, void (UserProfile* userProfile));
	MOCK_METHOD1(getPhoto, string(string id));

};

/**
 * Test para probar el camino feliz cuando se confirma un usuario
 */
TEST(MatchServiceConfirmTest,confirmingUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->addNewMatch("alinari");
	EXPECT_CALL(*mockMatchDao, get("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	EXPECT_CALL(*mockChatService, createChat("psivori", "alinari"));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	EXPECT_NO_THROW({matchService->confirmUser("psivori","alinari");});

	delete matchService;
}

/**
 * Test confirmacion falla mismo nombre usuario
 */
TEST(MatchServiceConfirmBadUserTest,confirmingBadUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();

	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->confirmUser("psivori","psivori");
		FAIL();
	}
	catch(IllegalStateException& e){
		//OK
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test confirmacion falla el usuario esta en la lista de aceptados
 */
TEST(MatchServiceConfirmUserIsInAcceptedListTest,confirmingUserIsInAcceptedList){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->acceptUser("alinari");
	EXPECT_CALL(*mockMatchDao, get("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->confirmUser("psivori","alinari");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test confirmacion falla el usuario esta en la lista de rechazados
 */
TEST(MatchServiceConfirmUserIsInRejectedListTest,confirmingUserIsInRejectedList){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->rejectUser("alinari");
	EXPECT_CALL(*mockMatchDao, get("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->confirmUser("psivori","alinari");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test confirmacion falla el usuario no esta en la lista de matcheados
 */
TEST(MatchServiceConfirmUserIsNotMatchedTest,confirmingUserIsNotMatched){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	EXPECT_CALL(*mockMatchDao, get("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->confirmUser("psivori","alinari");
		FAIL();
	}
	catch(IllegalStateException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test para probar el camino feliz cuando se acepta un usuario que NO lo tiene en su lista de aceptados
 */
TEST(MatchServiceAccept1Test, acceptingUser1){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	EXPECT_CALL(*mockMatchDao, get(_))
	.WillOnce(Return(matchPsivori))
	.WillOnce(Return(matchAlinari));

	SearchStats* stats = new SearchStats();
	UserStat* userStat = new UserStat("alinari",0,0,"");
	stats->addUserStat(userStat);

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(1).WillRepeatedly(Return(stats));
	EXPECT_CALL(*mockMatchDao, put("psivori", matchPsivori)).Times(1).WillOnce(Return());
	EXPECT_CALL(*mockChatService, getChat("psivori","alinari")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	bool res;
	EXPECT_NO_THROW({res = matchService->addToYesList("psivori","alinari");});
	ASSERT_TRUE(!res);
	delete matchService;
}

/**
 * Test para probar el camino feliz cuando se acepta un usuario que SI lo tiene en su lista de aceptados
 */
TEST(MatchServiceAccept2Test, acceptingUser2){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();

	matchAlinari->acceptUser("psivori");
	SearchStats* stats = new SearchStats();
	UserStat* userStat = new UserStat("alinari",0,0,"");
	stats->addUserStat(userStat);

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(1).WillRepeatedly(Return(stats));
	EXPECT_CALL(*mockMatchDao, get(_))
	.WillOnce(Return(matchPsivori))
	.WillOnce(Return(matchAlinari));
	EXPECT_CALL(*mockMatchDao, put("psivori", matchPsivori)).Times(1).WillOnce(Return());
	EXPECT_CALL(*mockMatchDao, put("alinari", matchAlinari)).Times(1).WillOnce(Return());
	EXPECT_CALL(*mockChatService, getChat("psivori","alinari")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	bool res;
	EXPECT_NO_THROW({res = matchService->addToYesList("psivori","alinari");});
	ASSERT_TRUE(res);
	delete matchService;
}

/**
 * Test confirmacion falla el usuario es el mismo
 */
TEST(MatchServiceAcceptBadUserTest,acceptingBadUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToYesList("psivori","psivori");
		FAIL();
	}
	catch(IllegalStateException& e){
		//OK
	}
	delete matchService;
}

/**
 * Test confirmacion falla el usuario ya esta en la lista de rechazados
 */
TEST(MatchServiceAcceptUserIsInRejectedListTest,confirmingUserIsInRejectedList){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	matchPsivori->rejectUser("alinari");
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockMatchDao, get("alinari")).WillOnce(Return(matchAlinari));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToYesList("psivori","alinari");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test confirmacion falla el usuario ya esta en la lista de matcheados
 */
TEST(MatchServiceAcceptUserIsMatchedTest,confirmingUserIsMatched){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchJFerrio = new Match();
	matchPsivori->addNewMatch("jferrio");
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockMatchDao, get("jferrio")).WillOnce(Return(matchJFerrio));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToYesList("psivori","jferrio");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test confirmacion falla el chat existe
 */
TEST(MatchServiceAcceptChatExistsTest,confirmingUserChatExists){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	Chat* chat = new Chat();
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockMatchDao, get("alinari")).WillOnce(Return(matchAlinari));
	EXPECT_CALL(*mockChatService, getChat("psivori","alinari")).Times(1).WillOnce(Return(chat));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToYesList("psivori","alinari");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test para probar el camino feliz cuando se rechaza un usuario
 */
TEST(MatchServiceRejectTest, rejectingUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	EXPECT_CALL(*mockMatchDao, get("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	EXPECT_CALL(*mockMatchDao, put("psivori", match)).Times(1).WillOnce(Return());
	EXPECT_CALL(*mockChatService, getChat("psivori","alinari")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	EXPECT_NO_THROW({matchService->addToNoList("psivori","alinari");});
	delete matchService;
}

/**
 * Test rechazo falla el usuario es el mismo
 */
TEST(MatchServiceRejectBadUserTest,rejectingBadUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToNoList("psivori","psivori");
		FAIL();
	}
	catch(IllegalStateException& e){
		//OK
	}
	delete matchService;
}

/**
 * Test rechazo falla el usuario ya esta en la lista de aceptados
 */
TEST(MatchServiceRejectUserIsInAceptedListTest,rejectingUserIsInAceptedList){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->acceptUser("alinari");
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToNoList("psivori","alinari");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test rechazo  el usuario ya esta en la lista de rechazados, no hace nada
 */
TEST(MatchServiceRejectUserIsInRejectedListTest,rejectingUserIsInRejectedList){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->rejectUser("alinari");
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	matchService->addToNoList("psivori","alinari");

	delete matchService;
}

/**
 * Test rechazar falla el usuario ya esta en la lista de matcheados
 */
TEST(MatchServiceRejectUserIsMatchedTest,rejectingUserIsMatched){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->addNewMatch("jferrio");
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToNoList("psivori","jferrio");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

/**
 * Test rechazo falla el chat existe
 */
TEST(MatchServiceRejectChatExistsTest,rejectingUserChatExists){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Chat* chat = new Chat();
	Match* matchPsivori = new Match();
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockChatService, getChat("psivori","alinari")).Times(1).WillOnce(Return(chat));

	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->addToNoList("psivori","alinari");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete matchService;
}

TEST(MatchServiceTest, getNewMatches){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->addNewMatch("alinari");
	matchPsivori->addNewMatch("jferrio");
	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	list<UserProfile*> users = matchService->getNewMatches("psivori");
	ASSERT_TRUE(users.size()==2);
	for (list<UserProfile*>::iterator it=users.begin(); it!=users.end(); ++it){
		UserProfile* user = *it;
		delete user;
	}
	delete matchService;
}


TEST(MatchServiceGetChatsTest, getChatsTest){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->addChat("alinari");

	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(Return(matchPsivori));

	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	list<UserProfile*> chats = matchService->getChats("psivori");
	ASSERT_TRUE(chats.size()==1);
	for (list<UserProfile*>::iterator it=chats.begin(); it!=chats.end(); ++it){
		UserProfile* chat = *it;
		delete chat;
	}
	delete matchService;
}

TEST(MatchServiceGetChatsNotFoundTest, getChatsNotFoundTest){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();

	EXPECT_CALL(*mockMatchDao, get("psivori")).WillOnce(ThrowEntityNotFoundException());

	MatchService* matchService = new MatchService(mockMatchDao, mockSearchStatsDao, mockChatService, mockSharedService);
	try{
		matchService->getChats("psivori");
		FAIL();
	}
	catch(EntityNotFoundException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
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
