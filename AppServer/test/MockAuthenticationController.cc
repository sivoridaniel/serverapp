/*
 * MockAuthenticationController.h
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */

#include "gmock/gmock.h"
#include "../src/utils/JwToken.h"
#include "../src/service/AuthenticationService.h"
#include "../src/dao/UserDao.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;

class MockUserDao : public UserDao{
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

class MockSharedService : public RemoteSharedService{
public:

	MOCK_CONST_METHOD1(MockFunctionGetUser, UserProfile*(string id) );
	UserProfile* getUser(string id){
		return MockFunctionGetUser(id);
	}

};

TEST(JwTokenTest, generatingToken){

	  JwToken* jwToken=new JwToken();
	  string token = "";

	  EXPECT_NO_THROW({token=jwToken->generarToken("psivori");});

	  cout<<"*****************************************************************"<<endl;
	  cout<<"TOKEN GENERADO: "<<token<<endl;
	  cout<<"*****************************************************************"<<endl;

	  delete jwToken;
}

TEST(AuthorizationTokenTest, isValidToken){

	JwToken* jwToken=new JwToken();
	string token="";
	bool isValidToken = false;

	EXPECT_NO_THROW({token = jwToken->generarToken("psivori");});

	if(token.size()!=0)
	   isValidToken=jwToken->isTokenValid(token);

	ASSERT_TRUE(isValidToken);

	delete jwToken;

}

TEST(AuthenticationServiceTest,login){
	MockUserDao* mockUserDao = new MockUserDao();
	MockSharedService* mockShared = new MockSharedService();
	UserProfile* userProfile = new UserProfile("psivori","password");
	EXPECT_CALL(*mockUserDao, MockFunctionGet("psivori")).Times(AtLeast(1)).WillOnce(Return(userProfile));
	AuthenticationService* authenticationService = new AuthenticationService(mockUserDao,mockShared);
	EXPECT_NO_THROW({authenticationService->getUserLogin("psivori","password");});

	delete userProfile;
	delete authenticationService;
}

/*
 * Correr con valgrind: valgrind --leak-check=full -v ./authenticationctrltest
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
