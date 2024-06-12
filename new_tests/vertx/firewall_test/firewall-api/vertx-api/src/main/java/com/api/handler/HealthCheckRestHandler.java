package com.api.handler;


import io.vertx.rxjava3.ext.web.RoutingContext;

public interface HealthCheckRestHandler extends DefaultRestHandler {

  String RESOURCE_NAME = "/health";

  void check(RoutingContext routingContext);
}
