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

class MockSearchCandidatesService : public ISearchCandidatesService{
public:
	MOCK_METHOD1(getCandidates, list<UserProfile*>(string));
};

ACTION(ThrowSearchDailyLimitExcededException){
	throw SearchDailyLimitExcededException();
}

ACTION(ThrowEntityNotFoundException){
	throw EntityNotFoundException();
}


/**
 * Test para probar el camino feliz cuando se invoca a /candidates
 */
TEST(SearchCtrlTest,searchCtrlTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();


	string url = "/candidates";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();


	MockSearchCandidatesService* mockSearchService = new MockSearchCandidatesService();
	SearchCandidatesController* searchCtrl = new SearchCandidatesController(mockSearchService);
	list<UserProfile*> candidates;
	UserProfile* user1 = new UserProfile("alinari", "1234");
	UserProfile* user2 = new UserProfile("psivori", "4321");
	Interest* interest = new Interest("Bands","Divididos");
	user1->addInterest(interest);
	candidates.push_back(user1);
	candidates.push_back(user2);

	EXPECT_CALL(*mockSearchService, getCandidates("10")).Times(1).WillOnce(Return(candidates));

	string code;
	EXPECT_NO_THROW({code = searchCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==searchCtrl->STATUS_OK);
	delete searchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /candidates sin id usuario
 */
TEST(SearchCtrlInvalidQueryStringTest,searchCtrlInvalidQueryStringTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();


	string url = "/candidates";
	string method = "GET";
	string queryString = "";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();


	MockSearchCandidatesService* mockSearchService = new MockSearchCandidatesService();
	SearchCandidatesController* searchCtrl = new SearchCandidatesController(mockSearchService);

	string code;
	EXPECT_NO_THROW({code = searchCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==searchCtrl->STATUS_NOK);
	delete searchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /candidates y devuelve error por limite diario superado
 */
TEST(SearchCtrlDailyLimitTest,searchCtrlDailyLimitTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();


	string url = "/candidates";
	string method = "GET";
	string queryString = "id=10";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();


	MockSearchCandidatesService* mockSearchService = new MockSearchCandidatesService();
	SearchCandidatesController* searchCtrl = new SearchCandidatesController(mockSearchService);

	EXPECT_CALL(*mockSearchService, getCandidates("10")).Times(1).WillRepeatedly(ThrowSearchDailyLimitExcededException());

	string code;
	EXPECT_NO_THROW({code = searchCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==searchCtrl->STATUS_NOK);
	delete searchCtrl;
	delete nc;
	delete hm;

}

/**
 * Test para probar cuando se invoca a /candidates y devuelve error por usuario no encontrado
 */
TEST(SearchCtrlUserNotFoundTest,searchCtrlUserNotFoundTest){

	struct mg_connection *nc = new mg_connection();
	struct http_message  *hm = new http_message();


	string url = "/candidates";
	string method = "GET";
	string queryString = "id=11";
	hm->uri.p = url.c_str();
	hm->uri.len = url.length();
	hm->method.p = method.c_str();
	hm->method.len = method.length();
	hm->query_string.p = queryString.c_str();
	hm->query_string.len = queryString.length();


	MockSearchCandidatesService* mockSearchService = new MockSearchCandidatesService();
	SearchCandidatesController* searchCtrl = new SearchCandidatesController(mockSearchService);

	EXPECT_CALL(*mockSearchService, getCandidates("11")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	string code;
	EXPECT_NO_THROW({code = searchCtrl->connect(nc, hm, true);});
	ASSERT_TRUE(code==searchCtrl->STATUS_NOT_FOUND);
	delete searchCtrl;
	delete nc;
	delete hm;

}



/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockChatCtrlTest
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
