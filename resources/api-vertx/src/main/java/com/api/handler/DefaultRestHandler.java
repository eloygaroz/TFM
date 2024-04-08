package com.api.handler;


import io.vertx.core.Handler;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.impl.logging.Logger;
import io.vertx.core.impl.logging.LoggerFactory;
import io.vertx.rxjava3.core.http.HttpServerResponse;
import io.vertx.rxjava3.ext.web.Router;
import io.vertx.rxjava3.ext.web.RoutingContext;
import io.vertx.rxjava3.ext.web.handler.LoggerHandler;
import io.vertx.rxjava3.ext.web.handler.ResponseTimeHandler;

public interface DefaultRestHandler {

  Logger LOGGER = LoggerFactory.getLogger(DefaultRestHandler.class);

  void addHandlersTo(Router router);

  default void makeResponse(final RoutingContext context, final int httpCode, final String body,
      final String contentType) {
    HttpServerResponse response = context.response();

    if (!response.ended() && !response.closed()) {
      response = response.setStatusCode(httpCode).putHeader("Content-Type", contentType);
      response.endHandler(
              event -> LOGGER.info(String.format("Request: %s, response: %s", context.request(), context.response())))
          .end(body);
    }
  }

  default void addHandlerTo(Router router, HttpMethod verb, String path, Handler<RoutingContext> handler) {
    router.route(verb, path)
        .handler(LoggerHandler.create())
        .handler(ResponseTimeHandler.create())
        .handler(handler);

    LOGGER.info(String.format("%s: %s handler created", verb.name(), path));
  }

  default void addGetHandlerTo(Router router, String path, Handler<RoutingContext> handler) {
    addHandlerTo(router, HttpMethod.GET, path, handler);
  }

}

