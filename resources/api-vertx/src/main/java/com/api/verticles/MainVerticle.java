package com.api.verticles;

import com.api.config.Config;
import com.api.handler.HealthCheckRestHandlerImpl;
import io.vertx.core.impl.logging.Logger;
import io.vertx.core.impl.logging.LoggerFactory;
import io.vertx.rxjava3.core.AbstractVerticle;
import io.vertx.rxjava3.ext.web.openapi.RouterBuilder;

import static com.api.config.SharedConfig.DEFAULT_HOST;
import static com.api.config.SharedConfig.DEFAULT_PORT;


public class MainVerticle extends AbstractVerticle {

    private static final Logger LOGGER = LoggerFactory.getLogger(MainVerticle.class.getName());

    private static final String HEALTH_OPERATION = "health";

    @Override
    public void start() {
        var config = Config.getInstance();

        final var httpHost = config.getHost().orElse(DEFAULT_HOST);
        final var httpPort = config.getPort().orElse(DEFAULT_PORT);

        var openApiFilePath = config.getOpenApiFilePath();

        RouterBuilder
                .rxCreate(vertx, openApiFilePath)
                .doOnError(throwable -> {
                    LOGGER.error("🔥 Error creating router: {}", throwable);
                    System.exit(1);
                })
                .map(routerBuilder -> {
                    routerBuilder.operation(HEALTH_OPERATION).handler(new HealthCheckRestHandlerImpl()::check);
                    LOGGER.debug("✅ Routing Done");
                    return routerBuilder.createRouter();
                })
                .subscribe(
                        router -> vertx.createHttpServer().requestHandler(router).rxListen(httpPort, httpHost).subscribe(
                                httpServer -> LOGGER.info(String.format("HTTP server started on http://%s:%s", httpHost, httpPort))
                        )
                );

        RouterBuilder
                .rxCreate(vertx, openApiFilePath)
                .doOnError(throwable -> {
                    LOGGER.error("🔥 Error creating router: {}", throwable);
                    System.exit(1);
                })
                .map(routerBuilder -> {
                    routerBuilder.operation(HEALTH_OPERATION).handler(new HealthCheckRestHandlerImpl()::check);
                    LOGGER.debug("✅ Routing Done");
                    return routerBuilder.createRouter();
                })
                .subscribe(
                        router -> vertx.createHttpServer().requestHandler(router).rxListen(httpPort + 1, httpHost).subscribe(
                                httpServer -> LOGGER.info(String.format("HTTP server started on http://%s:%s", httpHost, httpPort + 1))
                        )
                );
    }

}
