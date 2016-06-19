/*
 * MockABMUserCtrl.cc
 *
 *  Created on: Jun 19, 2016
 *      Author: agustin
 */

#include "gmock/gmock.h"
#include "../src/controller/AbmUserController.h"
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


ACTION(ThrowEntityNotFoundException){
	throw EntityNotFoundException();
}

ACTION(ThrowRemoteException){
	throw RemoteException();
}

/*
 * /user/newuser
 * /user/updateuser
 * /user/photo
 * /interests
 */

/**
 * Test para probar el camino feliz cuando se invoca a /user/newuser
 */
TEST(AbmUserNewUserCtrlTest,abmUserNewUserCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/newuser";
	string method = "POST";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();


	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, createNewUser(_)).Times(1);

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_OK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /user/newuser y da error remoto
 */
TEST(AbmCtrlNewUserRemoteErrorTest,abmCtrlNewUserRemoteErrorTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/newuser";
	string method = "POST";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, createNewUser(_)).Times(1).WillOnce(ThrowRemoteException());

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}


/**
 * Test para probar cuando se invoca a/user/newuser y el json es invalido
 */
TEST(AbmCtrlNewUserInvalidJsonTest,authCtrlNewUserInvalidJsonTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/newuser";
	string method = "POST";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}


/**
 * Test para probar el camino feliz cuando se invoca a /user/updateuser
 */
TEST(AbmUserUpdateUserCtrlTest,abmUserUpdateUserCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/updateuser";
	string method = "PUT";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();


	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, modifyUser(_)).Times(1);

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_OK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /user/updateuser y da error remoto
 */
TEST(AuthCtrlUpdateUserRemoteErrorTest,authCtrlUpdateUserRemoteErrorTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/updateuser";
	string method = "PUT";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, modifyUser(_)).Times(1).WillOnce(ThrowRemoteException());

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}


/**
 * Test para probar cuando se invoca a /user/updateuser y da usuario no encontrado
 */
TEST(AuthCtrlUpdateUserNotFoundTest,authCtrlUpdateUserNotFoundTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/updateuser";
	string method = "PUT";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", \"password\":\"1234\"} }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, modifyUser(_)).Times(1).WillOnce(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOT_FOUND);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a/user/updateuser y el json es invalido
 */
TEST(AbmCtrlUpdateUserInvalidJsonTest,abmCtrlUpdateUserInvalidJsonTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/updateuser";
	string method = "PUT";
	string json = "{\"user\":{\"name\":\"Agustin\" , \"email\":\"alinari@gmail.com\", }";

	hm->body.p=json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar el camino feliz cuando se invoca a /user/photo
 */
TEST(AbmUserGetPhotoCtrlTest,abmUserGetPhotoCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/photo";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, getPhoto("10")).Times(1).WillRepeatedly(Return("photobase64"));

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_OK);
	delete abmCtrl;
	delete nc;
	delete hm;

}


/**
 * Test para probar cuando se invoca a /user/photo con los parametros invalidos
 */
TEST(AbmUserGetPhotoInvalidParamsCtrlTest,abmUserGetPhotoInvalidParamsCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/photo";
	string method = "GET";
	string queryString = "";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /user/photo y da error remoto
 */
TEST(AbmUserGetPhotoRemoteErrorCtrlTest,abmUserGetPhotoRemoteErrorCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/photo";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, getPhoto("10")).Times(1).WillRepeatedly(ThrowRemoteException());

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /user/photo y da usuario inexistente
 */
TEST(AbmUserGetPhotoNotFoundCtrlTest,abmUserGetPhotoNotFoundCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/user/photo";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, getPhoto("10")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOT_FOUND);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar el camino feliz cuando se invoca a /interests
 */
TEST(AbmUserGetInterestsTest,abmUserGetInterestsTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/interests";
	string method = "GET";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	list<Interest*> interests;
	Interest* interest1 = new Interest("Bands", "The Beatles");
	Interest* interest2 = new Interest("Cars", "Porsche");

	interests.push_back(interest1);
	interests.push_back(interest2);

	EXPECT_CALL(*mockAbmService, getInterests()).Times(1).WillRepeatedly(Return(interests));

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_OK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /interests y da error remoto
 */
TEST(AbmUserGetInterestsRemoteErrorTest,abmUserGetInterestsRemoteErrorTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();

	string url = "/interests";
	string method = "GET";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockAbmUserService* mockAbmService = new MockAbmUserService();
	AbmUserController* abmCtrl = new AbmUserController(mockAbmService);

	EXPECT_CALL(*mockAbmService, getInterests()).Times(1).WillRepeatedly(ThrowRemoteException());

	string code;
	EXPECT_NO_THROW({code = abmCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==abmCtrl->STATUS_NOK);
	delete abmCtrl;
	delete nc;
	delete hm;

}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockAbmUserCtrlTest
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


