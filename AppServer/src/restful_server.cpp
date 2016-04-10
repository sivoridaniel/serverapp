/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "webserver/mongoose.h"
#include <iostream>
#include "utils/DbHelper.h"
#include "controller/FactoryController.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>

using namespace log4cplus;

static const char *s_http_port = "3000";
static struct mg_serve_http_opts s_http_server_opts;

// Define an event handler function
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));

  FactoryController* fController = FactoryController::getInstance();

  switch (ev) {
    case MG_EV_HTTP_REQUEST: //Data arrives on http request
      // This event handler implements simple TCP echo server
      LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Event Handler, call to controller"));
      fController->connect(nc,hm, s_http_server_opts);
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
  return 0;
}



