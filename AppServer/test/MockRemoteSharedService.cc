/*
 * MockRemoteSharedService.cc
 *
 *  Created on: Jun 12, 2016
 *      Author: agustin
 */
#include "gmock/gmock.h"
#include "../src/service/impl/RemoteSharedService.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class MockRestClient: public IRestClient
{
public:

	MOCK_METHOD2(put, RestResponse*(string, string));
	MOCK_METHOD1(del, RestResponse*(string));
	MOCK_METHOD2(post, RestResponse*(string, string));
	MOCK_METHOD1(get, RestResponse*(string));

};

static string crearJsonUser()
{
	UserProfile* userProfile = new UserProfile("alinari", "123");
	Interest* interes = new Interest("cars", "Ferrari");
	userProfile->setAge("33");
	userProfile->setEmail("alinari@gmail.com");
	userProfile->setId("1");
	userProfile->setName("Agustin");
	userProfile->setSex("M");
	userProfile->setPhotoProfile("-");
	userProfile->setAlias("alinari");
	userProfile->addInterest(interes);
	string json = userProfile->toSharedJson();

	delete userProfile;
	return json;
}

static string createJsonInterests()
{
	Json::Value root;
	Json::FastWriter writer;

	root["interests"][0]["category"] = "Bandas";
	root["interests"][0]["value"] = "Divididos";
	root["interests"][1]["category"] = "Bandas";
	root["interests"][1]["value"] = "The Beatles";
	string json = writer.write(root);
	return json;
}

static string createJsonPhoto()
{
	Json::Value root;
	Json::FastWriter writer;

	root["photo"] = "photobase64";
	string json = writer.write(root);
	return json;
}

static string createJsonUsers()
{
	Json::Value root;
	Json::FastWriter writer;

	root["users"][0]["user"]["name"] = "Agustin";
	root["users"][0]["user"]["age"] = "33";
	root["users"][0]["user"]["sex"] = "M";
	root["users"][0]["user"]["alias"] = "alinari";
	root["users"][0]["user"]["email"] = "alinari@gmail.com";
	root["users"][0]["user"]["photo"] = "/users/138/photo";
	root["users"][0]["user"]["location"]["latitude"] = "0.0";
	root["users"][0]["user"]["location"]["longitude"] = "0.0";
	root["users"][0]["user"]["id"] = "138";
	root["users"][0]["user"]["interests"][0]["category"] = "Bandas";
	root["users"][0]["user"]["interests"][0]["value"] = "Divididos";

	root["users"][1]["user"]["name"] = "Juan";
	root["users"][1]["user"]["age"] = "25";
	root["users"][1]["user"]["sex"] = "M";
	root["users"][1]["user"]["alias"] = "jperez";
	root["users"][1]["user"]["email"] = "jperez@gmail.com";
	root["users"][1]["user"]["photo"] = "/users/198/photo";
	root["users"][1]["user"]["location"]["latitude"] = "0.0";
	root["users"][1]["user"]["location"]["longitude"] = "0.0";
	root["users"][1]["user"]["id"] = "198";
	root["users"][1]["user"]["interests"][0]["category"] = "Bandas";
	root["users"][1]["user"]["interests"][0]["value"] = "The Beatles";

	string json = writer.write(root);

	return json;
}

static string createJsonGeneric(string success, string data)
{
	Json::Value root;
	Json::FastWriter writer;

	root["success"] = success;
	root["data"] = data;
	string json = writer.write(root);
	return json;
}

/**
 * Test para probar el camino feliz cuando se llama al get user
 */
TEST(SharedServiceGetUserTest,getUserTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = crearJsonUser();
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users/1")).Times(1).WillOnce(Return(res));

	UserProfile* user;
	EXPECT_NO_THROW(
	{
		user = sharedService->getUser("1")
		;
	}
);
    	ASSERT_TRUE(user->getName() == "Agustin");
	delete user;
	delete sharedService;
}

/**
 * Test para probar get User cuando el usuario no existe
 */
TEST(SharedServiceGetUser404Test,getUser404Test)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "usuario inexistente");
	RestResponse* res = new RestResponse(404, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users/2")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->getUser("2");
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		ASSERT_TRUE(true);
	} catch (RemoteException& e)
	{
		FAIL();
	}

	delete sharedService;
}

/**
 * Test para probar get User cuando devuelve error remoto
 */
TEST(SharedServiceGetUserErrorTest,getUserErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users/3")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->getUser("3");
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al create interest
 */
TEST(SharedServiceCreateInterestTest,createInterestTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);

	Interest* interest = new Interest("cars", "Toyota");
	RestResponse* res = new RestResponse(200, interest->toJson());

	EXPECT_CALL(*mockRestClient, post("mock.com/interests", _)).Times(1).WillOnce(Return(res));

	EXPECT_NO_THROW(
	{
		sharedService->createInterest(interest)
		;
	}
	);
	delete interest;
	delete sharedService;
}

/**
 * Test para probar createInterest cuando el interes ya se encuentra registrado en el shared
 */
TEST(SharedServiceCreateInterest400Test,createInterest400Test)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	Interest* interest = new Interest("cars", "Toyota");
	string json = createJsonGeneric("false", "interes ya existente");
	RestResponse* res = new RestResponse(400, json);

	EXPECT_CALL(*mockRestClient, post("mock.com/interests", _)).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->createInterest(interest);
		FAIL();
	} catch (EntityExistsException& e)
	{
		ASSERT_TRUE(true);
	} catch (RemoteException& e)
	{
		FAIL();
	}
	delete interest;
	delete sharedService;
}

/**
 * Test para probar createInterest cuando devuelve error remoto
 */
TEST(SharedServiceCreateInterestErrorTest,createInterestErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	Interest* interest = new Interest("cars", "Toyota");
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, post("mock.com/interests", _)).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->createInterest(interest);
		FAIL();
	} catch (EntityExistsException& e)
	{
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete interest;
	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al getInterests
 */
TEST(SharedServiceGetInterestsTest,getInterestsTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonInterests();
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/interests")).Times(1).WillOnce(Return(res));

	list<Interest*> listInterests;
	EXPECT_NO_THROW(
	{
		listInterests = sharedService->getInterests()
		;
	}
);

		ASSERT_TRUE(listInterests.size() == 2);
	for (list<Interest*>::iterator it = listInterests.begin(); it != listInterests.end(); ++it)
	{
		Interest* interest = *it;
		delete interest;
	}
	delete sharedService;
}

/**
 * Test para probar getInterests cuando devuelve error remoto
 */
TEST(SharedServiceGetInterestsErrorTest,createGetInterestsErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/interests")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->getInterests();
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al getUsers
 */
TEST(SharedServiceGetUsersTest,getUsersTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonUsers();
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users")).Times(1).WillOnce(Return(res));

	list<UserProfile*> users;
	EXPECT_NO_THROW(
	{
		users = sharedService->getUsers()
		;
	}
);

		ASSERT_TRUE(users.size() == 2);
	for (list<UserProfile*>::iterator it = users.begin(); it != users.end(); ++it)
	{
		UserProfile* user = *it;
		delete user;
	}
	delete sharedService;
}

/**
 * Test para probar getUsers cuando devuelve error remoto
 */
TEST(SharedServiceGetUsersErrorTest,createGetUsersErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->getUsers();
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al getPhoto
 */
TEST(SharedServiceGetPhotoTest,getPhotoTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonPhoto();
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users/1/photo")).Times(1).WillOnce(Return(res));

	string photo;
	EXPECT_NO_THROW(
	{
		photo = sharedService->getPhoto("1")
		;
	}
);
    	ASSERT_TRUE(photo == "photobase64");
	delete sharedService;
}

/**
 * Test para probar getPhoto cuando el usuario o foto no existe
 */
TEST(SharedServiceGetPhoto404Test,getPhoto404Test)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "usuario inexistente");
	RestResponse* res = new RestResponse(404, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users/2/photo")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->getPhoto("2");
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		ASSERT_TRUE(true);
	} catch (RemoteException& e)
	{
		FAIL();
	}

	delete sharedService;
}

/**
 * Test para probar getPhoto cuando devuelve error remoto
 */
TEST(SharedServiceGetPhotoErrorTest,getPhotoErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, get("mock.com/users/3/photo")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->getPhoto("3");
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al createUser
 */
TEST(SharedServiceCreateUserTest,createUserTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);

	UserProfile* user = new UserProfile("alinari", "1234");
	user->setId("1");
	string json = crearJsonUser();
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, post("mock.com/users/1", _)).Times(1).WillOnce(Return(res));

	EXPECT_NO_THROW(
	{
		sharedService->createUser(user)
		;
	}
	);
	delete user;
	delete sharedService;
}

/**
 * Test para probar createUser cuando el user ya se encuentra registrado en el shared
 */
TEST(SharedServiceCreateUser400Test,createUser400Test)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	UserProfile* user = new UserProfile("alinari", "1234");
	user->setId("1");
	string json = createJsonGeneric("false", "usuario ya existente");
	RestResponse* res = new RestResponse(400, json);

	EXPECT_CALL(*mockRestClient, post("mock.com/users/1", _)).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->createUser(user);
		FAIL();
	} catch (EntityExistsException& e)
	{
		ASSERT_TRUE(true);
	} catch (RemoteException& e)
	{
		FAIL();
	}
	delete user;
	delete sharedService;
}

/**
 * Test para probar createUser cuando devuelve error remoto
 */
TEST(SharedServiceCreateUserErrorTest,createUserErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	UserProfile* user = new UserProfile("alinari", "1234");
	user->setId("1");
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, post("mock.com/users/1", _)).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->createUser(user);
		FAIL();
	} catch (EntityExistsException& e)
	{
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete user;
	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al updateUser
 */
TEST(SharedServiceUpdateUserTest,updateUserTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);

	UserProfile* user = new UserProfile("alinari", "1234");
	user->setId("1");
	string json = crearJsonUser();
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, put("mock.com/users/1", _)).Times(1).WillOnce(Return(res));

	EXPECT_NO_THROW(
	{
		sharedService->updateUser(user)
		;
	}
	);
	delete user;
	delete sharedService;
}

/**
 * Test para probar updateUser cuando el user no se encuentra registrado en el shared
 */
TEST(SharedServiceUpdateUser404Test,updateUser404Test)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	UserProfile* user = new UserProfile("alinari", "1234");
	user->setId("1");
	string json = createJsonGeneric("false", "usuario inexistente");
	RestResponse* res = new RestResponse(404, json);

	EXPECT_CALL(*mockRestClient, put("mock.com/users/1", _)).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->updateUser(user);
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		ASSERT_TRUE(true);
	} catch (RemoteException& e)
	{
		FAIL();
	}
	delete user;
	delete sharedService;
}

/**
 * Test para probar updateUser cuando devuelve error remoto
 */
TEST(SharedServiceUpdateUserErrorTest,updateUserErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	UserProfile* user = new UserProfile("alinari", "1234");
	user->setId("1");
	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, put("mock.com/users/1", _)).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->updateUser(user);
		FAIL();
	} catch (EntityExistsException& e)
	{
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete user;
	delete sharedService;
}

/**
 * Test para probar el camino feliz cuando se llama al deleteUser
 */
TEST(SharedServiceDeleteUserTest,deleteUserTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);

	string json = createJsonGeneric("true", "eliminado");
	RestResponse* res = new RestResponse(200, json);

	EXPECT_CALL(*mockRestClient, del("mock.com/users/1")).Times(1).WillOnce(Return(res));

	EXPECT_NO_THROW(
	{
		sharedService->deleteUser("1")
		;
	}
	);
	delete sharedService;
}

/**
 * Test para probar deleteUser cuando el user no se encuentra registrado en el shared
 */
TEST(SharedServiceDeleteUser404Test,deleteUser404Test)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);
	string json = createJsonGeneric("false", "usuario inexistente");
	RestResponse* res = new RestResponse(404, json);

	EXPECT_CALL(*mockRestClient, del("mock.com/users/1")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->deleteUser("1");
		FAIL();
	} catch (EntityNotFoundException& e)
	{
		ASSERT_TRUE(true);
	} catch (RemoteException& e)
	{
		FAIL();
	}
	delete sharedService;
}

/**
 * Test para probar deleteUser cuando devuelve error remoto
 */
TEST(SharedServiceDeleteUserErrorTest,deleteUserErrorTest)
{
	MockRestClient* mockRestClient = new MockRestClient();
	RemoteSharedService* sharedService = new RemoteSharedService("mock.com", mockRestClient);

	string json = createJsonGeneric("false", "error");
	RestResponse* res = new RestResponse(500, json);

	EXPECT_CALL(*mockRestClient, del("mock.com/users/1")).Times(1).WillOnce(Return(res));

	try
	{
		sharedService->deleteUser("1");
		FAIL();
	} catch (EntityExistsException& e)
	{
		FAIL();
	} catch (RemoteException& e)
	{
		ASSERT_TRUE(true);
	}

	delete sharedService;
}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockSharedServiceTest
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

