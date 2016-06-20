/*
 * MockChatCtrl.cc
 *
 *  Created on: Jun 18, 2016
 *      Author: agustin
 */
#include "gmock/gmock.h"
#include "../src/controller/ChatController.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class MockChatService: public IChatService
{
public:
	MOCK_METHOD2(getAllMessages, vector<Message*>(string, string));
	MOCK_METHOD2(getNewMessages, vector<Message*>(string, string));
	MOCK_METHOD3(addNewMessage, void(string, string, string));
	MOCK_METHOD3(updateLastMessageSeen, void(string, string, int));
	MOCK_METHOD2(createChat, void(string, string));
	MOCK_METHOD2(getChat, Chat*(string, string));
};

ACTION(ThrowEntityNotFoundException){
throw EntityNotFoundException();
}

/**
 * Test para probar el camino feliz cuando se invoca a /chat
 */
TEST(ChatCtrlGetAllMessagesTest,chatCtrlGetAllMessagesTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat";
	string method = "GET";
	string queryString = "idFrom=10&idTo=11";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	vector<Message*> messages;
	Message* message1 = new Message(1, "10", "hola");
	Message* message2 = new Message(2, "11", "chau");
	messages.push_back(message1);
	messages.push_back(message2);
	EXPECT_CALL(*mockChatService, getAllMessages("10","11")).Times(1).WillOnce(Return(messages));

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_OK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat con los parametros incorrectos
 */
TEST(ChatCtrlGetAllMessagesInvalidQueryStringTest,chatCtrlGetAllMessagesInvalidQueryStringTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat";
	string method = "GET";
	string queryString = "idTo=2";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat y devuelve error por usuario no encontrado
 */
TEST(ChatCtrlGetAllMessagesUserNotFoundTest,chatCtrlGetAllMessagesUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat";
	string method = "GET";
	string queryString = "idFrom=10&idTo=11";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	EXPECT_CALL(*mockChatService, getAllMessages("10","11")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOT_FOUND);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar el camino feliz cuando se invoca a /chat/new
 */
TEST(ChatCtrlGetNewMessagesTest,chatCtrlGetNewMessagesTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/new";
	string method = "GET";
	string queryString = "idFrom=10&idTo=11";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	vector<Message*> messages;
	Message* message1 = new Message(1, "10", "hola");
	Message* message2 = new Message(2, "11", "chau");
	messages.push_back(message1);
	messages.push_back(message2);
	EXPECT_CALL(*mockChatService, getNewMessages("10","11")).Times(1).WillOnce(Return(messages));

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_OK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/new con los parametros incorrectos
 */
TEST(ChatCtrlGetNewMessagesInvalidQueryStringTest,chatCtrlGetNewMessagesInvalidQueryStringTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/new";
	string method = "GET";
	string queryString = "idTo=2";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/new y devuelve error por usuario no encontrado
 */
TEST(ChatCtrlGetNewMessagesUserNotFoundTest,chatCtrlGetNewMessagesUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/new";
	string method = "GET";
	string queryString = "idFrom=10&idTo=11";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	EXPECT_CALL(*mockChatService, getNewMessages("10","11")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOT_FOUND);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/message
 */
TEST(ChatCtrlAddNewMessagesTest,chatCtrlAddNewMessagesTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/message";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\", \"message\":\"hola\"}";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	EXPECT_CALL(*mockChatService, addNewMessage("10","11","hola")).Times(1);

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_OK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/message y devuelve error por json malformado
 */
TEST(ChatCtrlAddNewMessagesInvalidJsonTest,chatCtrlAddNewMessagesInvalidJsonTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/message";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\", }";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/message y devuelve error por usuario no encontrado
 */
TEST(ChatCtrlAddNewMessagesUserNotFoundTest,chatCtrlAddNewMessagesUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/message";
	string method = "POST";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\", \"message\":\"hola\"}";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	EXPECT_CALL(*mockChatService, addNewMessage("10","11", "hola")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOT_FOUND);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/last
 */
TEST(ChatCtrlUpdateLastMessageTest,chatCtrlUpdateLastMessagesTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/last";
	string method = "PUT";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\", \"idMessage\":\"1\"}";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	EXPECT_CALL(*mockChatService, updateLastMessageSeen("10","11",1)).Times(1);

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_OK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/last y devuelve error por json malformado
 */
TEST(ChatCtrlUpdateLastInvalidJsonTest,chatCtrlUpdateLastInvalidJsonTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/last";
	string method = "PUT";
	string json = "{\"idFrom\":\"10\" ,  \"idMessage\":\"1\"}";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOK);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /chat/last y devuelve error por usuario no encontrado
 */
TEST(ChatCtrlUpdateLastUserNotFoundTest,chatCtrlUpdateLastUserNotFoundTest)
{

	struct mg_connection *nc = new mg_connection();
	struct http_message *hm = new http_message();

	string url = "/chat/last";
	string method = "PUT";
	string json = "{\"idFrom\":\"10\" , \"idTo\":\"11\", \"idMessage\":\"1\"}";

	hm->body.p = json.c_str();
	hm->body.len = json.length();
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();

	MockChatService* mockChatService = new MockChatService();
	ChatController* chatCtrl = new ChatController(mockChatService);

	EXPECT_CALL(*mockChatService, updateLastMessageSeen("10","11", 1)).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW(
	{
		code = chatCtrl->connect(nc, hm, true)
		;
	}
);
		ASSERT_TRUE(code == chatCtrl->STATUS_NOT_FOUND);
	delete chatCtrl;
	delete nc;
	delete hm;

}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockChatCtrlTest
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
