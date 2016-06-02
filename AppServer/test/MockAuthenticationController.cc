/*
 * MockAuthenticationController.h
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */

#include "gmock/gmock.h"
#include "../src/utils/JwToken.h"
#include "../src/service/impl/AuthenticationService.h"
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

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};

class MockSharedService : public IRemote{
public:

	MOCK_METHOD1(getUser, UserProfile*(string id) );
	MOCK_METHOD1(createInterest, void (Interest* interest));
	MOCK_METHOD0(getInterests, list<Interest*>(void));
	MOCK_METHOD0(getUsers, list<UserProfile*>(void));
	MOCK_METHOD1(createUser, void (UserProfile* userProfile));
	MOCK_METHOD1(deleteUser, void (string id));
	MOCK_METHOD1(updateUser, void (UserProfile* userProfile));

};

TEST(JwTokenTest, generatingToken){

	  JwToken* jwToken=new JwToken();
	  string token = "";

	  EXPECT_NO_THROW({token=jwToken->generarToken("sivori.daniel@gmail.com");});

	  cout<<"*****************************************************************"<<endl;
	  cout<<"TOKEN GENERADO: "<<token<<endl;
	  cout<<"*****************************************************************"<<endl;

	  delete jwToken;
}

TEST(AuthorizationTokenTest, isValidToken){

	JwToken* jwToken=new JwToken();
	string token="";
	bool isValidToken = false;

	EXPECT_NO_THROW({token = jwToken->generarToken("17");});

	if(token.size()!=0)
	   isValidToken=jwToken->isTokenValid(token);

	ASSERT_TRUE(isValidToken);

	delete jwToken;

}

TEST(AuthenticationServiceTest,login){
	MockUserDao* mockUserDao = new MockUserDao();
	MockSharedService* mockShared = new MockSharedService();
	UserProfile* userProfile = new UserProfile("sivori.daniel@gmail.com","password");
	UserProfile* userProfileShared = new UserProfile("sivori.daniel@gmail.com","password");
	userProfile->setId("1");
	EXPECT_CALL(*mockUserDao, get("sivori.daniel@gmail.com")).Times(AtLeast(1)).WillOnce(Return(userProfile));
	EXPECT_CALL(*mockShared, getUser("1")).Times(AtLeast(1)).WillOnce(Return(userProfileShared));
	AuthenticationService* authenticationService = new AuthenticationService(mockUserDao,mockShared);
	EXPECT_NO_THROW({authenticationService->getUserLogin("sivori.daniel@gmail.com","password");});
	ASSERT_TRUE(userProfileShared->getPassword() == "password");
	delete userProfileShared;
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
