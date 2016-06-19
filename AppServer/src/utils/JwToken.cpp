/*
 * JwToken.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */

#include "JwToken.h"

void JwToken::evaluateOperation(int intcod,jwt*jwt,char* pcharcod,string msgError,Logger logger)throw (TokenException){

	if(intcod!=0 || pcharcod==NULL){
		LOG4CPLUS_ERROR(logger,LOG4CPLUS_TEXT(msgError) << " " << intcod);
		jwt_free(jwt);
		throw TokenException();
	}
}

string JwToken::generarToken(string email)throw (TokenException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("JwToken"));
	jwt *jwt=NULL;
	unsigned char key256[32] = "012345678901234567890123456789X";//(unsigned char*)KEY_DATA(); //Key Data 32 bytes for algorithm
	string seconds_expire = "1800";
	char *out;
	string ret_token="";
	time_t timestamp = time(NULL);
	string stimestamp = to_string(timestamp);

	int codint = jwt_new(&jwt);

	evaluateOperation(codint,jwt,(char *)"",MSG_ERROR_NEW_JWT(),logger);
	evaluateOperation(jwt_set_alg(jwt, JWT_ALG_HS256,key256, sizeof(key256)),jwt,(char *)"",
			                MSG_ERROR_HEADER(),logger); //Header: typ: jwt, alg: HS256
	LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("EMAIL PARA EL PAYLOAD: "<<email<<" "<<jwt));
	evaluateOperation(jwt_add_grant(jwt, "email", email.c_str()),jwt,(char *)"",
			                MSG_ERROR_PAYLOAD(),logger); //Payload -> "username":"xxxx"
	LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("TIMESTAMP: "<<stimestamp<<" "<<jwt));
	evaluateOperation(jwt_add_grant(jwt, "timestamp", stimestamp.c_str()),jwt,(char *)"",
			                MSG_ERROR_PAYLOAD(),logger); //Payload -> "timestamp":"long int"
	LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("EXPIRE: "<<seconds_expire<<" "<<jwt));
	evaluateOperation(jwt_add_grant(jwt, "expire", seconds_expire.c_str()),jwt,(char *)"",
							MSG_ERROR_PAYLOAD(),logger); //Payload -> "expire": "cantidad de segundos en que expira el token"
	LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("TOKEN GENERADO " <<jwt));
	out = jwt_encode_str(jwt);

	evaluateOperation(0,jwt,out,MSG_ERROR_TOKEN(),logger);

	if(out != NULL)
	ret_token=out;

	free(out);
	jwt_free(jwt);

	return ret_token;
}

bool JwToken::isTokenValid(string token)throw (TokenException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("JwToken"));
	jwt *jwt = NULL;
	unsigned char key256[32] = "012345678901234567890123456789X";//(unsigned char*)KEY_DATA(); //Key Data 32 bytes for algorithm
	string seconds_expire_str = "1800";
	time_t timestamp = time(NULL);

	evaluateOperation(jwt_decode(&jwt, token.c_str(), key256, sizeof(key256)),jwt,(char *)"",
	                  MSG_ERROR_DECODE_TOKEN(),logger);

	long int timestampold = atol(jwt_get_grant(jwt,"timestamp"));
	int seconds_expire = atoi(seconds_expire_str.c_str());
	bool isNotExpired=((timestamp - timestampold)<=seconds_expire); //Si es el timestamp no supero la cantidad de segundos para expirar

	jwt_free(jwt);

	return isNotExpired;
}

string JwToken::getEmail(string token)throw (TokenException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("JwToken"));
	jwt *jwt = NULL;
	unsigned char key256[32] = "012345678901234567890123456789X";//(unsigned char*)KEY_DATA(); //Key Data 32 bytes for algorithm

	evaluateOperation(jwt_decode(&jwt, token.c_str(), key256, sizeof(key256)),jwt,(char *)"",
					  MSG_ERROR_DECODE_EMAIL(),logger);

	string email = jwt_get_grant(jwt,"email");

	jwt_free(jwt);

	return email;
}
