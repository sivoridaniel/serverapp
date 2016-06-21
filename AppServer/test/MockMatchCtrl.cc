/*
 * MockMatchCtrl.cc
 *
 *  Created on: Jun 18, 2016
 *      Author: agustin
 */
#include "gmock/gmock.h"
#include "../src/controller/MatchController.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

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

ACTION(ThrowEntityNotFoundException){
throw EntityNotFoundException();
}

ACTION(ThrowEntityExistsException){
throw EntityExistsException();
}

/**
 * Test para probar el camino feliz cuando se invoca a /match
 */
TEST(MatchCtrlGetMatchesTest,matchCtrlGetMatchesTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	list<UserProfile*> matches;
	UserProfile* user1 = new UserProfile("alinari", "1234");
	UserProfile* user2 = new UserProfile("psivori", "4321");
	Interest* interest = new Interest("Bands", "Divididos");
	user1->addInterest(interest);
	matches.push_back(user1);
	matches.push_back(user2);

	EXPECT_CALL(*mockMatchService, getNewMatches("10")).Times(1).WillOnce(Return(matches));

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_OK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match con los parametros incorrectos
 */
TEST(MatchCtrlGetMatchesInvalidQueryStringTest, matchCtrlGetMatchesInvalidQueryStringTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match";
	string method = "GET";
	string queryString = "";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match y devuelve error por usuario no encontrado
 */
TEST(MatchCtrlGetMatchesUserNotFoundTest,matchCtrlGetMatchesUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, getNewMatches("10")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOT_FOUND);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/yes y no hay match
 */
TEST(MatchCtrlYesTest,matchCtrlYesTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/yes";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToYesList("10","11")).Times(1).WillRepeatedly(Return(false));

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_OK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/yes y hay match
 */
TEST(MatchCtrlYesMatchedTest,matchCtrlYesMatchedTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/yes";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToYesList("10","11")).Times(1).WillRepeatedly(Return(true));

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_OK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/yes y devuelve error por json malformado
 */
TEST(MatchCtrlYesInvalidJsonTest,matchCtrlYesInvalidJsonTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/yes";
	string method = "POST";
	string json = "{\"idFrom\":\"10\"  \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/yes y devuelve error por usuario no encontrado
 */
TEST(MatchCtrlYesUserNotFoundTest,matchCtrlYesUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/yes";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToYesList("10","11")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOT_FOUND);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/yes y ya habia puesto que no
 */
TEST(MatchCtrlYesIllegalTest,matchCtrlYesIllegalTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/yes";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToYesList("10","11")).Times(1).WillRepeatedly(ThrowEntityExistsException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/no
 */
TEST(MatchCtrlNoTest,matchCtrlNoTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/no";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToNoList("10","11")).Times(1);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_OK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/no y devuelve error por json malformado
 */
TEST(MatchCtrlNoInvalidJsonTest,matchCtrlNoInvalidJsonTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/no";
	string method = "POST";
	string json = "{\"idFrom\":\"10\"  }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/no y devuelve error por usuario no encontrado
 */
TEST(MatchCtrlNoUserNotFoundTest,matchCtrlNoUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/no";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToNoList("10","11")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOT_FOUND);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/no y ya habia puesto que si
 */
TEST(MatchCtrlNoIllegalTest,matchCtrlNoIllegalTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/no";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, addToNoList("10","11")).Times(1).WillRepeatedly(ThrowEntityExistsException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/confirm
 */
TEST(MatchCtrlConfirmTest,matchCtrlConfirmTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/confirm";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, confirmUser("10","11")).Times(1);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_OK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/confirm y devuelve error por json malformado
 */
TEST(MatchCtrlConfirmInvalidJsonTest,matchCtrlConfirmInvalidJsonTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/confirm";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\": }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/confirm y devuelve error por usuario no encontrado
 */
TEST(MatchCtrlConfirmUserNotFoundTest,matchCtrlConfirmUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/confirm";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, confirmUser("10","11")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOT_FOUND);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /match/confirm y es inconsistente
 */
TEST(MatchCtrlConfirmIllegalTest,matchCtrlConfirmIllegalTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match/confirm";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\" }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, confirmUser("10","11")).Times(1).WillRepeatedly(ThrowEntityExistsException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar el camino feliz cuando se invoca a /chats
 */
TEST(MatchCtrlGetChatsTest,matchCtrlGetChatsTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chats";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	list<UserProfile*> chats;
	UserProfile* user1 = new UserProfile("alinari", "1234");
	UserProfile* user2 = new UserProfile("psivori", "4321");
	Interest* interest = new Interest("Bands", "Divididos");
	user1->addInterest(interest);
	chats.push_back(user1);
	chats.push_back(user2);

	EXPECT_CALL(*mockMatchService, getChats("10")).Times(1).WillOnce(Return(chats));

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_OK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chats y devuelve error parametros invalidos
 */
TEST(MatchCtrlGetChatsInvalidQueryStringTest, matchCtrlGetChatsInvalidQueryStringTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/match";
	string method = "GET";
	string queryString = "";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOK);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chats y devuelve error por usuario no encontrado
 */
TEST(MatchCtrlGetChatsUserNotFoundTest,matchCtrlGetChatsUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chats";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockMatchService* mockMatchService = new MockMatchService();
	MatchController* matchCtrl = new MatchController(mockMatchService);

	EXPECT_CALL(*mockMatchService, getChats("10")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = matchCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == matchCtrl->STATUS_NOT_FOUND);
	delete matchCtrl;
	delete nc;
	delete hm;

}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockMatchCtrlTest
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

