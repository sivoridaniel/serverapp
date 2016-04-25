/*
 * MockAuthenticationController.h
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */

#include "gmock/gmock.h"
#include "../src/utils/JwToken.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;

//class MockAuthenticationController :

//class MockAuthenticationController : public IAuthenticationController {
//public:
//	MOCK_METHOD2(login,string(string username,string password));
//};


//TEST(AuthenticatateTest, isRegisterUser) {
//  MockAuthenticationController mockAuthCtrl;
//  EXPECT_CALL(mockAuthCtrl, login("psivori","1234")).Times(AtLeast(1)).WillOnce(Return(NULL));
//  EXPECT_TRUE(mockAuthCtrl.login("psivori","1234"));
//}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./authenticationctrltest
 */
TEST(JwTokenTest, generatingToken){

	  JwToken* jwToken=new JwToken();
	  string token = "";

	  EXPECT_NO_THROW({token=jwToken->generarToken("psivori");});

	  cout<<token<<endl;

	  delete jwToken;
}

TEST(AuthorizationTokenTest, isValidToken){

	JwToken* jwToken=new JwToken();
	string token="";
	bool isValidToken = false;

	EXPECT_NO_THROW({token = jwToken->generarToken("psivori");});

	if(token.size()!=0)
	   isValidToken=jwToken->isTokenValid(token);

	delete jwToken;

}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);

	initialize();
	BasicConfigurator config;
	config.configure();

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando Tests"));

	return RUN_ALL_TESTS();
}
