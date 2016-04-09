/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "webserver/mongoose.h"
#include <iostream>
#include <assert.h>
#include "utils/DbHelper.h"
#include "rocksdb/db.h"
#include "dao/UserDao.h"
#include "service/AuthenticationService.h"
#include "controller/FactoryController.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>

using namespace log4cplus;

static const char *s_http_port = "3000";
static struct mg_serve_http_opts s_http_server_opts;
static FactoryController* factController;

//static void handle_sum_call(struct mg_connection *nc, struct http_message *hm) {
//  char n1[100], n2[100];
//  double result;
//
//  /* Get form variables */
//  mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
//  mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));
//
//  /* Send headers */
//  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//
//  /* Compute the result and send it back as a JSON object */
//  result = strtod(n1, NULL) + strtod(n2, NULL);
//  mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
//  mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
//}
//
//static void handle_new_user(struct mg_connection *nc, struct http_message *hm){
//	char name[100], password[100], firstName[100], lastName[100], email[100];
//
//	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
//
//	/* Get form variables */
//	mg_get_http_var(&hm->body, "name", name, sizeof(name));
//	mg_get_http_var(&hm->body, "password", password, sizeof(password));
//	mg_get_http_var(&hm->body, "lastName", lastName, sizeof(lastName));
//	mg_get_http_var(&hm->body, "firstName", firstName, sizeof(firstName));
//	mg_get_http_var(&hm->body, "email", email, sizeof(email));
//
//	/* Send headers */
//	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//
//	/* Create new user in database */
//	std::string sname(name);
//	std::string spassword(password);
//	std::string slastName(lastName);
//	std::string sfirstName(firstName);
//	std::string semail(email);
//	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Guardando en db usuario " << sname));
//	std::string result = authService->createNewUser(sname,spassword, semail);
//
//	/* Send result back as a JSON object */
//	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
//	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
//}
//
//static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
//  struct http_message *hm = (struct http_message *) ev_data;
//  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
//
//  switch (ev) {
//    case MG_EV_HTTP_REQUEST:
//      /* Handle RESTful call */
//      if (mg_vcmp(&hm->uri, "/api/v1/sum") == 0)
//      {
//        handle_sum_call(nc, hm);
//      }
//      else if (mg_vcmp(&hm->uri, "/newuser") == 0)
//      {
//    	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Event Handler: POST /newuser"));
//    	handle_new_user(nc, hm);
//      }
//      else if (mg_vcmp(&hm->uri, "/printcontent") == 0)
//      {
//        char buf[100] = {0};
//        memcpy(buf, hm->body.p,
//               sizeof(buf) - 1 < hm->body.len? sizeof(buf) - 1 : hm->body.len);
//        printf("%s\n", buf);
//      }
//      else
//      {
//        mg_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */
//      }
//      break;
//    default:
//      break;
//  }
//}
//

// Define an event handler function
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));

  FactoryController* fController = FactoryController::getInstance();

  switch (ev) {
    case MG_EV_HTTP_REQUEST: //Data arrives on http request
      // This event handler implements simple TCP echo server
      LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Event Handler, call to controller"));
      fController->connect(nc,hm);
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;
  char *cp;
#ifdef MG_ENABLE_SSL
  const char *ssl_cert = NULL;
#endif

  initialize();
  BasicConfigurator config;
  config.configure();
  factController = new FactoryController();

  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Mongoose webserver 6.3"));
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando Base de Datos RocksDB 4.4"));
  string dbpath = "/tmp/testdb";
  DbHelper::initDatabase(dbpath);


  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("RocksDB 4.4 iniciada en " << dbpath));
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando HTTP server"));
  mg_mgr_init(&mgr, NULL);

  /* Process command line options to customize HTTP server */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
    } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
      s_http_server_opts.document_root = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      s_http_port = argv[++i];
    } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
      s_http_server_opts.auth_domain = argv[++i];
#ifdef MG_ENABLE_JAVASCRIPT
    } else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc) {
      const char *init_file = argv[++i];
      mg_enable_javascript(&mgr, v7_create(), init_file);
#endif
    } else if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
      s_http_server_opts.global_auth_file = argv[++i];
    } else if (strcmp(argv[i], "-A") == 0 && i + 1 < argc) {
      s_http_server_opts.per_directory_auth_file = argv[++i];
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      s_http_server_opts.url_rewrites = argv[++i];
#ifndef MG_DISABLE_CGI
    } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      s_http_server_opts.cgi_interpreter = argv[++i];
#endif
#ifdef MG_ENABLE_SSL
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      ssl_cert = argv[++i];
#endif
    } else {
      LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Opción desconocida " << argv[i]));
      exit(1);
    }
  }

  /* Set HTTP server options */
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error iniciando servidor en puerto " << s_http_port));
    exit(1);
  }

#ifdef MG_ENABLE_SSL
  if (ssl_cert != NULL) {
    const char *err_str = mg_set_ssl(nc, ssl_cert, NULL);
    if (err_str != NULL) {
      LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error cargando certificado SSL: " << err_str));
      exit(1);
    }
  }
#endif

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";
  s_http_server_opts.enable_directory_listing = "yes";

  /* Use current binary directory as document root */
  if (argc > 0 && ((cp = strrchr(argv[0], '/')) != NULL ||
      (cp = strrchr(argv[0], '/')) != NULL)) {
    *cp = '\0';
    s_http_server_opts.document_root = argv[0];
  }
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Inicializando RESTful server en puerto " << s_http_port));
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  DbHelper::closeDatabase();
  delete factController;
  return 0;
}



//int main(int argc, char *argv[]) {
//	 struct mg_mgr mgr;
//	 struct mg_connection *nc; //Lo que devuelve la función bind()
//	 Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
//
//	  mg_mgr_init(&mgr, NULL);  // Initialize event manager object
//
//	  // Note that many connections can be added to a single event manager
//	  // Connections can be created at any point, e.g. in event handler function
//	  nc=mg_bind(&mgr, s_http_port, ev_handler);  // Create listening connection and add it to the event manager
//
//	  if (nc == NULL) {
//	      LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error iniciando servidor en puerto " << s_http_port));
//	      exit(1);
//	  }
//
//
//	  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Mongoose webserver 6.3"));
//	  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Inicializando RESTful server en puerto " << s_http_port));
//	  for (;;) {  // Start infinite event loop
//	    mg_mgr_poll(&mgr, 1000);
//	  }
//
//	  mg_mgr_free(&mgr);
//	  return 0;
//}




