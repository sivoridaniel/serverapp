/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"
#include <iostream>
#include <assert.h>
#include "rocksdb/db.h"
#include "UserDao.h"
#include "User.h"
//#include <log4cplus/logger.h>
//#include <log4cplus/loggingmacros.h>
//#include <log4cplus/configurator.h>
//#include <iomanip>

//using namespace log4cplus;

static const char *s_http_port = "3000";
static struct mg_serve_http_opts s_http_server_opts;
static UserDao* userDao;

static void handle_sum_call(struct mg_connection *nc, struct http_message *hm) {
  char n1[100], n2[100];
  double result;

  /* Get form variables */
  mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
  mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));
 
  /* Send headers */
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  /* Compute the result and send it back as a JSON object */
  result = strtod(n1, NULL) + strtod(n2, NULL);
  mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
  mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
}

static void handle_new_user(struct mg_connection *nc, struct http_message *hm){
	char name[100], password[100], firstName[100], lastName[100], email[100];

	/* Get form variables */
	mg_get_http_var(&hm->body, "name", name, sizeof(name));
	mg_get_http_var(&hm->body, "password", password, sizeof(password));
	mg_get_http_var(&hm->body, "lastName", lastName, sizeof(lastName));
	mg_get_http_var(&hm->body, "firstName", firstName, sizeof(firstName));
	mg_get_http_var(&hm->body, "email", email, sizeof(email));

	/* Send headers */
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Create new user in database */
	std::string sname(name);
	std::string spassword(password);
	std::string slastName(lastName);
	std::string sfirstName(firstName);
	std::string semail(email);
	User* user = new User(name,password);
	user->setFirstName(sfirstName);
	user->setLastName(slastName);
	user->setPassword(spassword);
	user->setEmail(semail);
	userDao->putUser(user);
    delete user;

    /* Read user from database */
    User* us = userDao->getUser(sname);
    std::string result = us->getName();
    delete us;

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      /* Handle RESTful call */
      if (mg_vcmp(&hm->uri, "/api/v1/sum") == 0)
      {
        handle_sum_call(nc, hm);
      }
      else if (mg_vcmp(&hm->uri, "/newuser") == 0)
      {
    	handle_new_user(nc, hm);
      }
      else if (mg_vcmp(&hm->uri, "/printcontent") == 0)
      {
        char buf[100] = {0};
        memcpy(buf, hm->body.p,
               sizeof(buf) - 1 < hm->body.len? sizeof(buf) - 1 : hm->body.len);
        printf("%s\n", buf);
      }
      else
      {
        mg_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */
      }
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

 /* initialize();
  BasicConfigurator config;
  config.configure();

  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
  LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Hello, World!"));
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Inicializando Base de Datos"));*/
  rocksdb::DB* db;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status =
  rocksdb::DB::Open(options, "/tmp/testdb", &db);
  assert(status.ok());

  userDao = new UserDao(db);

  //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Base de Datos creada"));
  //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Inicializando Webserver"));
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
      //LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error desconocido"));
      fprintf(stderr, "Unknown option: [%s]\n", argv[i]);
      exit(1);
    }
  }

  /* Set HTTP server options */
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    //LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error iniciando servidor en puerto " << s_http_port));
    fprintf(stderr, "Error starting server on port %s\n", s_http_port);
    exit(1);
  }

#ifdef MG_ENABLE_SSL
  if (ssl_cert != NULL) {
    const char *err_str = mg_set_ssl(nc, ssl_cert, NULL);
    if (err_str != NULL) {
      LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error cargando certificado SSL: " << err_str));
      fprintf(stderr, "Error loading SSL cert: %s\n", err_str);
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
  //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Inicializando RESTful server en puerto " << s_http_port));
  printf("Starting RESTful server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  delete userDao;
  delete db;

  return 0;
}
