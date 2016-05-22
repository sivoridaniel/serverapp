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

static bool quit = false;


// Define an event handler function
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {

  if (ev == MG_EV_HTTP_REQUEST) {

      FactoryController* fController = FactoryController::getInstance();

      struct http_message *hm = (struct http_message *) ev_data;
      fController->connect(nc,hm, s_http_server_opts);
      if (mg_vcmp(&hm->uri, "/quit") == 0) {
    	  quit = true;
      }
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  initialize();
  PropertyConfigurator::doConfigure("log4cpp.properties");

  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));

  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Mongoose webserver 6.3"));
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando Base de Datos RocksDB 4.4"));
  string dbpath = "/tmp/testdb";
  DbHelper::initDatabase(dbpath);


  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("RocksDB 4.4 iniciada en " << dbpath));
  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando HTTP server"));
  mg_mgr_init(&mgr, NULL);


  /* Set HTTP server options */
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error iniciando servidor en puerto " << s_http_port));
    exit(1);
  }

  mg_set_protocol_http_websocket(nc);

 // mg_enable_multithreading(nc);

  LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Inicializando RESTful server en puerto " << s_http_port));
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
    if (quit){
    	break;
    }
  }
  mg_mgr_free(&mgr);
  FactoryController* fController = FactoryController::getInstance();
  delete fController;
  DbHelper::closeDatabase();
  return 0;
}



