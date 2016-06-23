/*
 * MockSearchCandidatesService.cc
 *
 *  Created on: Jun 22, 2016
 *      Author: agustin
 */

#include "gmock/gmock.h"
#include "../src/service/impl/MatchService.h"
#include "../src/service/impl/RemoteSharedService.h"
#include "../src/dao/SearchStatsDao.h"
#include "../src/service/impl/SearchCandidatesService.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class MockSharedService: public IRemote
{
public:

	MOCK_METHOD1(getUser, UserProfile*(string id) );
	MOCK_METHOD1(createInterest, void (Interest* interest));
	MOCK_METHOD0(getInterests, list<Interest*>(void));
	MOCK_METHOD0(getUsers, list<UserProfile*>(void));
	MOCK_METHOD1(createUser, void (UserProfile* userProfile));
	MOCK_METHOD1(deleteUser, void (string id));
	MOCK_METHOD1(updateUser, void (UserProfile* userProfile));
	MOCK_METHOD1(getPhoto, string(string id));

};

class MockMatchService: public IMatchService
{
public:
	MOCK_METHOD2(addToYesList, bool(string, string));
	MOCK_METHOD2(addToNoList, void(string, string));
	MOCK_METHOD1(getNewMatches, list<UserProfile*>(string));
	MOCK_METHOD1(getChats, list<UserProfile*>(string));
	MOCK_METHOD2(confirmUser, void(string, string));
	MOCK_METHOD2(isACandidate, bool(string, string));
};


class MockSearchStatsDao: public SearchStatsDao
{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};



ACTION(ThrowEntityNotFoundException){
throw EntityNotFoundException();
}

ACTION(ThrowEntityExistsException){
throw EntityExistsException();
}

ACTION(ThrowInvalidEntityException){
throw InvalidEntityException();
}

ACTION(ThrowRemoteException){
throw RemoteException();
}


/**
 * Test para probar  filtros de busqueda mismo usuario
 */
TEST(SearchServiceFilterSameUserTest,searchServiceFilterSameUserTest)
{
	MockMatchService* mockMatchService = new MockMatchService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockSharedService* mockSharedService = new MockSharedService();

	SearchStats* stats1 = new SearchStats();
	UserStat* userStat1 = new UserStat("psivori", 0,0, "");
	stats1->addUserStat(userStat1);

	SearchStats* stats2 = new SearchStats();
	UserStat* userStat2 = new UserStat("psivori", 0,0, "");
	stats2->addUserStat(userStat2);

	list<UserProfile*> users;
	UserProfile* user1 = new UserProfile("psivori", "123");
	user1->setId("psivori");
	users.push_back(user1);

	UserProfile* user2 = new UserProfile("psivori", "123");
	user2->setId("psivori");

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(AtLeast(1)).WillOnce(Return(stats1)).WillOnce(Return(stats2));
	EXPECT_CALL(*mockSharedService, getUsers()).Times(AtLeast(1)).WillOnce(Return(users));
	EXPECT_CALL(*mockSharedService, getUser("psivori")).Times(AtLeast(1)).WillOnce(Return(user2));

	SearchCandidatesService* searchService = new SearchCandidatesService(mockSearchStatsDao, mockMatchService, mockSharedService);
	list<UserProfile*> candidates;
	EXPECT_NO_THROW(
	{
		candidates = searchService->getCandidates("psivori");
		;
	}
	);

	ASSERT_TRUE(candidates.size()==0);

	delete searchService;
}


/**
 * Test para probar  filtros de busqueda para usuarios matcheados
 */
TEST(SearchServiceFilterMatchedTest,searchServiceFilterMatchedTest)
{
	MockMatchService* mockMatchService = new MockMatchService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockSharedService* mockSharedService = new MockSharedService();

	SearchStats* stats1 = new SearchStats();
	UserStat* userStat1 = new UserStat("psivori", 0,0, "");
	stats1->addUserStat(userStat1);

	SearchStats* stats2 = new SearchStats();
	UserStat* userStat2 = new UserStat("psivori", 0,0, "");
	stats2->addUserStat(userStat2);

	list<UserProfile*> users;
	UserProfile* user1 = new UserProfile("alinari", "123");
	user1->setId("alinari");
	users.push_back(user1);

	UserProfile* user2 = new UserProfile("psivori", "123");
	user2->setId("psivori");

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(AtLeast(1)).WillOnce(Return(stats1)).WillOnce(Return(stats2));
	EXPECT_CALL(*mockSharedService, getUsers()).Times(AtLeast(1)).WillOnce(Return(users));
	EXPECT_CALL(*mockSharedService, getUser("psivori")).Times(AtLeast(1)).WillOnce(Return(user2));
	EXPECT_CALL(*mockMatchService, isACandidate("psivori","alinari")).Times(AtLeast(1)).WillRepeatedly(Return(false));

	SearchCandidatesService* searchService = new SearchCandidatesService(mockSearchStatsDao, mockMatchService, mockSharedService);
	list<UserProfile*> candidates;
	EXPECT_NO_THROW(
	{
		candidates = searchService->getCandidates("psivori");
		;
	}
	);

	ASSERT_TRUE(candidates.size()==0);

	delete searchService;
}

/**
 * Test para probar  filtros de busqueda por distancia
 */
TEST(SearchServiceFilterDistanceTest,searchServiceFilterDistanceTest)
{
	MockMatchService* mockMatchService = new MockMatchService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockSharedService* mockSharedService = new MockSharedService();

	SearchStats* stats1 = new SearchStats();
	UserStat* userStat1 = new UserStat("psivori", 0,0, "");
	stats1->addUserStat(userStat1);

	SearchStats* stats2 = new SearchStats();
	UserStat* userStat2 = new UserStat("psivori", 0,0, "");
	stats2->addUserStat(userStat2);

	list<UserProfile*> users;
	UserProfile* user1 = new UserProfile("alinari", "123");
	Location* location1 = new Location("-34.608056","-58.370278");
	user1->setId("alinari");
	user1->setLocation(location1);
	users.push_back(user1);

	UserProfile* user2 = new UserProfile("psivori", "123");
	Location* location2 = new Location("-34.607611","-58.44575");
	user2->setLocation(location2);
	user2->setId("psivori");

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(AtLeast(1)).WillOnce(Return(stats1)).WillOnce(Return(stats2));
	EXPECT_CALL(*mockSharedService, getUsers()).Times(AtLeast(1)).WillOnce(Return(users));
	EXPECT_CALL(*mockSharedService, getUser("psivori")).Times(AtLeast(1)).WillOnce(Return(user2));
	EXPECT_CALL(*mockMatchService, isACandidate("psivori","alinari")).Times(AtLeast(1)).WillRepeatedly(Return(true));

	SearchCandidatesService* searchService = new SearchCandidatesService(mockSearchStatsDao, mockMatchService, mockSharedService);
	list<UserProfile*> candidates;
	EXPECT_NO_THROW(
	{
		candidates = searchService->getCandidates("psivori");
		;
	}
	);

	ASSERT_TRUE(candidates.size()==0);

	delete searchService;
}


/**
 * Test para probar  filtros de busqueda por interes
 */
TEST(SearchServiceFilterInterestTest,searchServiceFilterInterestTest)
{
	MockMatchService* mockMatchService = new MockMatchService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	MockSharedService* mockSharedService = new MockSharedService();

	SearchStats* stats1 = new SearchStats();
	UserStat* userStat1 = new UserStat("psivori", 0,0, "");
	stats1->addUserStat(userStat1);

	SearchStats* stats2 = new SearchStats();
	UserStat* userStat2 = new UserStat("psivori", 0,0, "");
	stats2->addUserStat(userStat2);

	SearchStats* stats3 = new SearchStats();
	UserStat* userStat3 = new UserStat("alinari", 1,0, "");
	stats3->addUserStat(userStat3);

	list<UserProfile*> users;
	UserProfile* user1 = new UserProfile("alinari", "123");
	Location* location1 = new Location("-34.608056","-58.370278");
	Interest* interest1 = new Interest("Bands", "The Beatles");
	user1->setId("alinari");
	user1->setLocation(location1);
	user1->addInterest(interest1);
	users.push_back(user1);

	UserProfile* user2 = new UserProfile("psivori", "123");
	Location* location2 = new Location("-34.609652","-58.3925806");
	Interest* interest2 = new Interest("Bands", "The Beatles");
	user2->setLocation(location2);
	user2->setId("psivori");
	user2->addInterest(interest2);

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(AtLeast(1)).WillOnce(Return(stats1)).WillOnce(Return(stats2)).WillOnce(Return(stats3));
	EXPECT_CALL(*mockSharedService, getUsers()).Times(AtLeast(1)).WillOnce(Return(users));
	EXPECT_CALL(*mockSharedService, getUser("psivori")).Times(AtLeast(1)).WillOnce(Return(user2));
	EXPECT_CALL(*mockMatchService, isACandidate("psivori","alinari")).Times(AtLeast(1)).WillRepeatedly(Return(true));

	SearchCandidatesService* searchService = new SearchCandidatesService(mockSearchStatsDao, mockMatchService, mockSharedService);
	list<UserProfile*> candidates;
	EXPECT_NO_THROW(
	{
		candidates = searchService->getCandidates("psivori");
		;
	}
	);
	ASSERT_TRUE(candidates.size()==1);
	delete user1;

	delete searchService;
}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockSearchServiceTest
 */
int main(int argc, char* argv[])
{
	::testing::InitGoogleMock(&argc, argv);

	initialize();
	BasicConfigurator config;
	config.configure();

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando Tests"));

	return RUN_ALL_TESTS();
}


