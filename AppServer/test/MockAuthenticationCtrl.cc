/*
 * MockAuthenticationCtrl.cc
 *
 *  Created on: Jun 19, 2016
 *      Author: agustin
 */

#include "gmock/gmock.h"
#include "../src/controller/AuthenticationController.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class MockAbmUserService : public IAbmUserService{
public:
	MOCK_METHOD1(createNewUser, string(UserProfile*));
	MOCK_METHOD1(modifyUser, void(UserProfile*));
	MOCK_METHOD1(updateToken, void(UserProfile*));
	MOCK_METHOD1(getPhoto, string(string));
	MOCK_METHOD0(getInterests, list<Interest*>());
};

class MockAuthService : public IAuthenticationService{
public:
	MOCK_METHOD2(getUserLogin, UserProfile*(string,string));
};


ACTION(ThrowEntityNotFoundException){
	throw EntityNotFoundException();
}

ACTION(ThrowIncorrectPasswordException){
	throw IncorrectPasswordException();
}


/**
 * Test para probar el camino feliz cuando se invoca a /login_user
 */
TEST(AuthCtrlTest,authCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/login_user";
	string method = "GET";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();


	MockAbmUserService* mockAbmService = new MockAbmUserService();
	MockAuthService* mockAuthService = new MockAuthService();
	AuthenticationController* authCtrl = new AuthenticationController(mockAuthService,mockAbmService);

	UserProfile* user = new UserProfile("alinari", "1234");
	user->setEmail("alinari@gmail.com");

	EXPECT_CALL(*mockAuthService, getUserLogin("alinari@gmail.com","1234")).Times(1).WillOnce(Return(user));
	EXPECT_CALL(*mockAbmService, updateToken(user)).Times(1);

	string code;
	EXPECT_NO_THROW({code = authCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==authCtrl->STATUS_OK);
	delete authCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /login_user y no se encuentra el usuario
 */
TEST(AuthCtrlUserNotFoundTest,authCtrlUserNotFoundTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/login_user";
	string method = "GET";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();


	MockAbmUserService* mockAbmService = new MockAbmUserService();
	MockAuthService* mockAuthService = new MockAuthService();
	AuthenticationController* authCtrl = new AuthenticationController(mockAuthService,mockAbmService);

	UserProfile* user = NULL;

	EXPECT_CALL(*mockAuthService, getUserLogin("alinari@gmail.com","1234")).Times(1).WillOnce(Return(user));

	string code;
	EXPECT_NO_THROW({code = authCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==authCtrl->STATUS_NOK);
	delete authCtrl;
	delete nc;
	delete hm;

}


/**
 * Test para probar cuando se invoca a /login_user y el password es incorrecto
 */
TEST(AuthCtrlInvalidPasswordTest,authCtrlInvalidPasswordTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/login_user";
	string method = "GET";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1235\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();


	MockAbmUserService* mockAbmService = new MockAbmUserService();
	MockAuthService* mockAuthService = new MockAuthService();
	AuthenticationController* authCtrl = new AuthenticationController(mockAuthService,mockAbmService);

	EXPECT_CALL(*mockAuthService, getUserLogin("alinari@gmail.com","1235")).Times(1).WillOnce(ThrowIncorrectPasswordException());

	string code;
	EXPECT_NO_THROW({code = authCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==authCtrl->STATUS_NOK);
	delete authCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /login_user y devuelve error 404
 */
TEST(AuthCtrlNotFoundTest,authCtrlNotFoundTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/login_user";
	string method = "GET";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1235\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();


	MockAbmUserService* mockAbmService = new MockAbmUserService();
	MockAuthService* mockAuthService = new MockAuthService();
	AuthenticationController* authCtrl = new AuthenticationController(mockAuthService,mockAbmService);

	EXPECT_CALL(*mockAuthService, getUserLogin("alinari@gmail.com","1235")).Times(1).WillOnce(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW({code = authCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==authCtrl->STATUS_NOT_FOUND);
	delete authCtrl;
	delete nc;
	delete hm;

}


/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockAuthenticationCtrlTest
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


