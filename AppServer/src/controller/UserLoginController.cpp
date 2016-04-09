/*
 * UserProfileController.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "UserLoginController.h"

UserLoginController::UserLoginController() {

	userDao = new UserDao();
	authService = new AuthenticationService(userDao);
	//mg_vcmp(&hm->uri, "/newuser") == 0
}

string UserLoginController::connect(struct mg_connection *nc, struct http_message *hm){

	if(mg_vcmp(&hm->uri, "/user/newuser") == 0){
		return event_handler_new_user(nc,hm);
	}else if(mg_vcmp(&hm->uri, "/user/getuser") == 0){
		return event_handler_get_user(nc,hm);
	}

	return "{}"; //Por default devuelve un JSON vacío.
}

string UserLoginController::event_handler_new_user(struct mg_connection *nc, struct http_message *hm){
	char name[100], password[100], firstName[100], lastName[100], email[100];

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("UserLoginController"));

	/* Get form variables */
	mg_get_http_var(&hm->body, "name", name, sizeof(name));
	mg_get_http_var(&hm->body, "password", password, sizeof(password));
	mg_get_http_var(&hm->body, "lastName", lastName, sizeof(lastName));
	mg_get_http_var(&hm->body, "firstName", firstName, sizeof(firstName));
	mg_get_http_var(&hm->body, "email", email, sizeof(email));

	/* Send headers */
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Create new user in database */
	string sname(name);
	string spassword(password);
	string slastName(lastName);
	string sfirstName(firstName);
	string semail(email);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Guardando en db usuario " << sname));
	string result = authService->createNewUser(sname,spassword, semail);

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	return result;
}

string UserLoginController::event_handler_get_user(struct mg_connection *nc, struct http_message *hm){
	return "{}";
}

UserLoginController::~UserLoginController() {
	delete authService;
	delete userDao;
}

