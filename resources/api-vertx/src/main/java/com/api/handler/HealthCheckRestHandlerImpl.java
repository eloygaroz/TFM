package com.api.handler;


import io.netty.handler.codec.http.HttpResponseStatus;
import io.vertx.core.json.Json;
import io.vertx.rxjava3.core.http.HttpServerResponse;
import io.vertx.rxjava3.ext.web.Router;
import io.vertx.rxjava3.ext.web.RoutingContext;
import java.util.Map;

public class HealthCheckRestHandlerImpl implements HealthCheckRestHandler {


  @Override
  public void addHandlersTo(Router router) {
    // handlers added in verticle initialization
  }

  @Override
  public void check(RoutingContext context) {
    HttpServerResponse response = context.response();
    if (!response.ended() && !response.closed()) {
      response.setStatusCode(HttpResponseStatus.OK.code()).putHeader("Content-Type", "application/json")
          .end(Json.encodePrettily(Map.of("message","ok")));
    }
  }
}
