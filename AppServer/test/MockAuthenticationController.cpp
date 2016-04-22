/*
 * MockAuthenticationController.h
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */

#include "gmock/gmock.h"
#include "../src/controller/IAuthenticationController.h"
#include "jwt.h"
#include <string>
#include <ctime>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;

class MockAuthenticationController : public IAuthenticationController {
public:
	MOCK_METHOD2(isRegisterUser,bool(string username,string password));
};


TEST(AuthenticatateTest, isRegisterUser) {
  MockAuthenticationController mockAuthCtrl;
  EXPECT_CALL(mockAuthCtrl, isRegisterUser("psivori","1234")).Times(AtLeast(1)).WillOnce(Return(true));
  EXPECT_TRUE(mockAuthCtrl.isRegisterUser("psivori","1234"));
}

TEST(AuthorizationTokenTest, generatingToken){
	  jwt *jwt = NULL;
	  unsigned char key256[32] = "012345678901234567890123456789X"; //Key Data 32 bytes for algorithm
	  int ret = 0;
	  char *out;
	  time_t timestamp = time(NULL);
	  stringstream timestamp_aux_str;
	  timestamp_aux_str << timestamp;
	  const char* timestamp_str = timestamp_aux_str.str().c_str();

	  ret = jwt_new(&jwt);
	  EXPECT_TRUE(ret == 0); //Si es exitoso es 0 return val.

	  ret = jwt_set_alg(jwt, JWT_ALG_HS256, key256, sizeof(key256)); //Header: typ: jwt, alg: HS256
	  EXPECT_TRUE(ret == 0);										 //Si es exitoso es 0 return val.

	  ret = jwt_add_grant(jwt, "iss", "index2.html/match/newmatches"); //Payload -> "iss": "uri+controller"
	  EXPECT_TRUE(ret == 0);										   //Si es exitoso es 0 return val.

	  ret = jwt_add_grant(jwt, "username", "psivori"); //Payload -> "username":"xxxx"
	  EXPECT_TRUE(ret == 0);					       //Si es exitoso es 0 return val.

	  ret = jwt_add_grant(jwt, "timestamp", timestamp_str); //Payload -> "timestamp":"long int"
	  EXPECT_TRUE(ret == 0);					       //Si es exitoso es 0 return val.

	  out = jwt_encode_str(jwt);
	  EXPECT_TRUE(out != NULL); //Si es exitoso debe devolver distinto de NULL.

	  cout<<timestamp_str<<endl;
	  cout<<out<<endl;

	  free(out);
	  jwt_free(jwt);
}

TEST(AuthorizationTokenTest, isValidToken){
	  jwt *jwt = NULL;
	  unsigned char key256[32] = "012345678901234567890123456789X"; //Key Data 32 bytes for algorithm
	  int ret = 0;

	  time_t timestamp = time(NULL);
	  time_t timestampToken = 1461215628;
	  stringstream timestamp_aux_str;
	  timestamp_aux_str << timestampToken;
	  string timestamp_str = timestamp_aux_str.str().c_str();

	  const char token[] = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9." //HEADER
			  	  	  	   "eyJpc3MiOiJpbmRleDIuaHRtbC9tYXRjaC9uZXdtYXRjaGVzIiwidGltZXN0YW1wI" //PAYLOAD
			               "joiMTQ2MTIxNDE3NCIsInVzZXJuYW1lIjoicHNpdm9yaSJ9." //PAYLOAD
			               "0-fhNKapRuAlblVhxoePAblsxPlutBLXm-3-q84RfcA"; //Signature

	  ret = jwt_decode(&jwt, token, key256, sizeof(key256));
	  EXPECT_TRUE(ret == 0); //Si es exitoso es 0 return val.
	  EXPECT_TRUE(timestamp_str.compare(jwt_get_grant(jwt,"timestamp"))==0); //Si es el timestamp esperado
	  cout<<(timestamp-timestampToken)/100<<endl; //Me da los minutos de diferencia. Supongo renovar token cada 1 minuto.

	  jwt_free(jwt);
}

int main(int argc, char* argv[]){
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
