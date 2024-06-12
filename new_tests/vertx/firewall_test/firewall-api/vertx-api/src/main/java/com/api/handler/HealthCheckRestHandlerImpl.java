package com.api.handler;


import io.netty.handler.codec.http.HttpResponseStatus;
import io.vertx.core.impl.logging.Logger;
import io.vertx.core.impl.logging.LoggerFactory;
import io.vertx.core.json.Json;
import io.vertx.rxjava3.core.http.HttpServerResponse;
import io.vertx.rxjava3.ext.web.Router;
import io.vertx.rxjava3.ext.web.RoutingContext;
import java.util.Map;

public class HealthCheckRestHandlerImpl implements HealthCheckRestHandler {

  private static final Logger LOGGER = LoggerFactory.getLogger(HealthCheckRestHandlerImpl.class.getName());

  @Override
  public void addHandlersTo(Router router) {
    // handlers added in verticle initialization
  }

  @Override
  public void check(RoutingContext context) {
    HttpServerResponse response = context.response();
    LOGGER.info(String.format("Inbound request: %s", context.request().path()));
    if (!response.ended() && !response.closed()) {
      var body = Map.of("message","ok");
      LOGGER.info(String.format("Response: %s", body));
      response.setStatusCode(HttpResponseStatus.OK.code()).putHeader("Content-Type", "application/json")
          .end(Json.encodePrettily(body));
    }
  }
}
