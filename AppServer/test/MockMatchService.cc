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

	MOCK_CONST_METHOD1(createInterest, void (Interest* interest));
	void createInterest(Interest* interest){
		createInterest(interest);
	}
	MOCK_CONST_METHOD0(getInterests, list<Interest*>(void));
	list<Interest*> getInterests(){
		return getInterests();
	}
	MOCK_CONST_METHOD0(getUsers, list<UserProfile*>(void));
	list<UserProfile*> getUsers(){
		return getUsers();
	}

	MOCK_CONST_METHOD1(createUser, void (UserProfile* userProfile));
	void createUser(UserProfile* userProfile){
		createUser(userProfile);
	}
	MOCK_CONST_METHOD1(deleteUser, void (string id));
	void deleteUser(string id){
		deleteUser(id);
	}
	MOCK_CONST_METHOD1(updateUser, void (UserProfile* userProfile));
	void updateUser(UserProfile* userProfile){
		updateUser(userProfile);
	}

};

/**
 * Test para probar el camino feliz cuando se confirma un usuario
 */
TEST(MatchServiceConfirmTest,confirmingUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->addNewMatch("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	EXPECT_CALL(*mockChatDao, MockFunctionPut("alinari-psivori",_));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
	EXPECT_NO_THROW({matchService->confirmUser("psivori","alinari");});

	delete matchService;
}

/**
 * Test confirmacion falla mismo nombre usuario
 */
TEST(MatchServiceConfirmBadUserTest,confirmingBadUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();

	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->acceptUser("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	match->rejectUser("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet(_))
	.WillOnce(Return(matchPsivori))
	.WillOnce(Return(matchAlinari));
	EXPECT_CALL(*mockMatchDao, MockFunctionPut("psivori", matchPsivori)).Times(1).WillOnce(Return());
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet(_))
	.WillOnce(Return(matchPsivori))
	.WillOnce(Return(matchAlinari));
	EXPECT_CALL(*mockMatchDao, MockFunctionPut("psivori", matchPsivori)).Times(1).WillOnce(Return());
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
	bool res;
	EXPECT_NO_THROW({res = matchService->addToYesList("psivori","alinari");});
	ASSERT_TRUE(!res);
	delete matchService;
}

/**
 * Test confirmacion falla el usuario es el mismo
 */
TEST(MatchServiceAcceptBadUserTest,acceptingBadUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	matchPsivori->rejectUser("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("alinari")).WillOnce(Return(matchAlinari));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchJFerrio = new Match();
	Chat* chat = new Chat();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("jferrio")).WillOnce(Return(matchJFerrio));
	EXPECT_CALL(*mockChatDao, MockFunctionGet("jferrio-psivori")).WillOnce(Return(chat));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Match* matchAlinari = new Match();
	matchPsivori->addNewMatch("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("alinari")).WillOnce(Return(matchAlinari));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* match = new Match();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(match));
	EXPECT_CALL(*mockMatchDao, MockFunctionPut("psivori", match)).Times(1).WillOnce(Return());
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
	EXPECT_NO_THROW({matchService->addToNoList("psivori","alinari");});
	delete matchService;
}

/**
 * Test rechazo falla el usuario es el mismo
 */
TEST(MatchServiceRejectBadUserTest,rejectingBadUser){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->acceptUser("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->rejectUser("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
	matchService->addToNoList("psivori","alinari");

	delete matchService;
}

/**
 * Test rechazar falla el usuario ya esta en la lista de matcheados
 */
TEST(MatchServiceRejectUserIsMatchedTest,rejectingUserIsMatched){
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockChatDao* mockChatDao = new MockChatDao();
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	Chat* chat = new Chat();
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	EXPECT_CALL(*mockChatDao, MockFunctionGet("jferrio-psivori")).WillOnce(Return(chat));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->addNewMatch("alinari");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
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
	MockChatService* mockChatService = new MockChatService(mockChatDao);
	MockSharedService* mockSharedService = new MockSharedService();
	Match* matchPsivori = new Match();
	matchPsivori->addNewMatch("alinari");
	matchPsivori->addNewMatch("jferrio");
	EXPECT_CALL(*mockMatchDao, MockFunctionGet("psivori")).WillOnce(Return(matchPsivori));
	MatchService* matchService = new MatchService(mockMatchDao, mockChatService, mockSharedService);
	list<UserProfile*> users = matchService->getNewMatches("psivori");
	ASSERT_TRUE(users.size()==2);
	for (list<UserProfile*>::iterator it=users.begin(); it!=users.end(); ++it){
		UserProfile* user = *it;
		delete user;
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
