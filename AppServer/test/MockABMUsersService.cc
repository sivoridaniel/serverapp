#include <gmock/gmock.h>
#include <gmock/gmock-actions.h>
#include <gmock/gmock-cardinalities.h>
#include <gmock/gmock-generated-actions.h>
#include <gmock/gmock-generated-function-mockers.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/tchar.h>
//#include <ctime>
#include <list>
#include <sstream>
#include <string>

#include "../src/dao/MatchDao.h"
#include "../src/dao/UserDao.h"
#include "../src/dao/SearchStatsDao.h"
#include "../src/exception/EntityExistsException.h"
#include "../src/exception/EntityNotFoundException.h"
#include "../src/exception/InvalidEntityException.h"
#include "../src/exception/JsonParseException.h"
#include "../src/exception/RemoteException.h"
#include "../src/model/Interest.h"
#include "../src/model/UserProfile.h"
#include "../src/service/api/IRemote.h"
#include "../src/service/impl/AbmUserService.h"
//#include "../src/service/impl/RemoteSharedService.h"

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class MockMatchDao: public MatchDao
{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};

class MockUserDao: public UserDao
{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};

class MockSearchStatsDao: public SearchStatsDao
{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};

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

ACTION(ThrowJsonParseException){
throw JsonParseException();
}

/**
 * Test camino feliz para probar createUser
 */
TEST(AbmUserServiceCreateUser,createUserTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");

	EXPECT_CALL(*mockSearchStatsDao, get("stats")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());
	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillRepeatedly(ThrowEntityNotFoundException());
	EXPECT_CALL(*mockSharedService, createUser(userProfile)).Times(1).WillRepeatedly(Return());
	EXPECT_CALL(*mockUserDao, put("juan@gmail.com",userProfile)).Times(1).WillRepeatedly(Return());
	EXPECT_CALL(*mockMatchDao, put("1",_)).Times(1).WillRepeatedly(Return());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	EXPECT_NO_THROW(
	{
		abmService->createNewUser(userProfile)
		;
	}
	);

	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso de que la entidad sea invalida
 */
TEST(AbmUserServiceCreateUserInvalid, createUserInvalidTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillRepeatedly(ThrowEntityNotFoundException());
	EXPECT_CALL(*mockSharedService, createUser(userProfile)).Times(1).WillRepeatedly(Return());
	EXPECT_CALL(*mockUserDao, put("juan@gmail.com",userProfile)).Times(1).WillRepeatedly(ThrowInvalidEntityException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->createNewUser(userProfile);
		FAIL();
	} catch (InvalidEntityException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso de que el usuario exista
 */
TEST(AbmUserServiceCreateUserExists, createUserExistsTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillRepeatedly(ThrowEntityExistsException());

	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->createNewUser(userProfile);
		FAIL();
	} catch (EntityExistsException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso de que el shared tire error
 */
TEST(AbmUserServiceCreateUserRemoteError, createUserRemoteErrorTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillRepeatedly(ThrowEntityNotFoundException());
	EXPECT_CALL(*mockSharedService, createUser(userProfile)).Times(1).WillRepeatedly(ThrowRemoteException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->createNewUser(userProfile);
		FAIL();
	} catch (RemoteException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete userProfile;
	delete abmService;
}

/**
 * Test camino feliz para probar modifyUser
 */
TEST(AbmUserServiceModifyUser,modifyUserTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");
	UserProfile* userProfileDb = new UserProfile("juan", "123");
	userProfileDb->setEmail("juan@gmail.com");
	userProfileDb->setId("1");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillOnce(Return(userProfileDb));
	EXPECT_CALL(*mockSharedService, updateUser(userProfile)).Times(1).WillRepeatedly(Return());
	EXPECT_CALL(*mockUserDao, put("juan@gmail.com",userProfile)).Times(1).WillRepeatedly(Return());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	EXPECT_NO_THROW(
	{
		abmService->modifyUser(userProfile)
		;
	}
	);

	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso de que la entidad sea invalida
 */
TEST(AbmUserServiceModifyUserInvalid, modifyUserInvalidTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");
	UserProfile* userProfileDb = new UserProfile("juan", "123");
	userProfileDb->setEmail("juan@gmail.com");
	userProfileDb->setId("1");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillOnce(Return(userProfileDb));
	EXPECT_CALL(*mockSharedService, updateUser(userProfile)).Times(1).WillRepeatedly(Return());
	EXPECT_CALL(*mockUserDao, put("juan@gmail.com",userProfile)).Times(1).WillRepeatedly(ThrowInvalidEntityException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->modifyUser(userProfile);
		FAIL();
	} catch (InvalidEntityException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete userProfileDb;
	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso de que el usuario no exista
 */
TEST(AbmUserServiceModifyUserExists, modifyUserExistsTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillRepeatedly(ThrowEntityNotFoundException());

	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->modifyUser(userProfile);
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso de que el shared tire error
 */
TEST(AbmUserServiceModifyUserRemoteError, modifyUserRemoteErrorTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* userProfile = new UserProfile("juan", "123");
	userProfile->setId("1");
	userProfile->setEmail("juan@gmail.com");
	UserProfile* userProfileDb = new UserProfile("juan", "123");
	userProfileDb->setEmail("juan@gmail.com");
	userProfileDb->setId("1");

	EXPECT_CALL(*mockUserDao, get("juan@gmail.com")).Times(AtLeast(1)).WillOnce(Return(userProfileDb));
	EXPECT_CALL(*mockSharedService, updateUser(userProfile)).Times(1).WillRepeatedly(ThrowRemoteException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->modifyUser(userProfile);
		FAIL();
	} catch (RemoteException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete userProfile;
	delete abmService;
}

/**
 * Test para probar el caso feliz de get intereses
 */
TEST(AbmUserServiceGetInterestsTest, getInterestsTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	list<Interest*> interests;
	list<Interest*> response;
	Interest* interest1 = new Interest("cars", "Fiat");
	Interest* interest2 = new Interest("movies", "Matrix");
	interests.push_back(interest1);
	interests.push_back(interest2);
	EXPECT_CALL(*mockSharedService, getInterests()).Times(1).WillRepeatedly(Return(interests));
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	EXPECT_NO_THROW(
	{
		response = abmService->getInterests()
		;
	}
);
		ASSERT_TRUE(response.size() == 2);
	delete interest1;
	delete interest2;
	delete abmService;
}

/**
 * Test para probar el caso de que el shared tire error
 */
TEST(AbmUserServiceGetInterestsRemoteError, getInterestsRemoteErrorTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	EXPECT_CALL(*mockSharedService, getInterests()).Times(1).WillRepeatedly(ThrowRemoteException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->getInterests();
		FAIL();
	} catch (RemoteException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete abmService;
}

/**
 * Test para probar el caso de que el json de respuesta sea invalido
 */
TEST(AbmUserServiceGetInterestsInvalidJson, getInterestsInvalidJsonTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	EXPECT_CALL(*mockSharedService, getInterests()).Times(1).WillRepeatedly(ThrowJsonParseException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	try
	{
		abmService->getInterests();
		FAIL();
	} catch (JsonParseException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}

	delete abmService;
}

TEST(AbmUserGetPhotoTest, abmUserGetPhotoTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	EXPECT_CALL(*mockSharedService, getPhoto("1")).Times(1).WillRepeatedly(Return("photo"));
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	string photo = "";
	EXPECT_NO_THROW(
	{
		photo = abmService->getPhoto("1")
		;
	}
);
		ASSERT_TRUE(photo == "photo");

	delete abmService;
}

TEST(AbmUserGetPhotoRemoteErrorTest, abmUserGetPhotoRemoteErrorTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	EXPECT_CALL(*mockSharedService, getPhoto("1")).Times(1).WillRepeatedly(ThrowRemoteException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	string photo = "";
	try
	{
		abmService->getPhoto("1");
		FAIL();
	} catch (RemoteException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}
	delete abmService;
}

TEST(AbmUserGetPhotoNotFoundTest, abmUserGetPhotoNotFoundTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	EXPECT_CALL(*mockSharedService, getPhoto("1")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	string photo = "";
	try
	{
		abmService->getPhoto("1");
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}
	delete abmService;
}

TEST(AbmUserUpdateTokenTest, abmUserUpdateTokenTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();
	UserProfile* user = new UserProfile("Agustin", "1234");
	user->setEmail("alinari@gmail.com");
	UserProfile* userAux = new UserProfile("Agustin", "1234");
	userAux->setEmail("alinari@gmail.com");
	EXPECT_CALL(*mockUserDao, get("alinari@gmail.com")).Times(1).WillRepeatedly(Return(userAux));
	EXPECT_CALL(*mockUserDao, put("alinari@gmail.com", user));
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);

	EXPECT_NO_THROW(
	{
		abmService->updateToken(user)
		;
	}
	);
	delete user;
	delete abmService;
}

TEST(AbmUserUpdateTokenInvalidUserErrorTest, abmUserUpdateTokenInvalidUserErrorTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* user = new UserProfile("Agustin", "1234");
	user->setEmail("alinari@gmail.com");

	EXPECT_CALL(*mockUserDao, get("alinari@gmail.com")).Times(1).WillRepeatedly(ThrowInvalidEntityException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	try
	{
		abmService->updateToken(user);
		FAIL();
	} catch (InvalidEntityException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}
	delete abmService;
	delete user;
}

TEST(AbmUserUpdateTokenNotFoundTest, abmUserUpdateTokenNotFoundTest)
{
	MockUserDao* mockUserDao = new MockUserDao();
	MockMatchDao* mockMatchDao = new MockMatchDao();
	MockSharedService* mockSharedService = new MockSharedService();
	MockSearchStatsDao* mockSearchStatsDao = new MockSearchStatsDao();

	UserProfile* user = new UserProfile("Agustin", "1234");
	user->setEmail("alinari@gmail.com");

	EXPECT_CALL(*mockUserDao, get("alinari@gmail.com")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());
	AbmUserService* abmService = new AbmUserService(mockUserDao, mockMatchDao, mockSearchStatsDao, mockSharedService);
	try
	{
		abmService->updateToken(user);
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		//OK;
	} catch (exception& e)
	{
		FAIL();
	}
	delete abmService;
	delete user;
}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockMatchServiceTest
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

