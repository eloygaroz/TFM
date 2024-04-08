package com.api.config;

import io.vertx.core.json.JsonObject;

import java.util.Optional;

public class SharedConfig implements HttpServerConfig {

    public static final String DEFAULT_HOST = "127.0.0.1";
    public static final int DEFAULT_PORT = 8080;

    public JsonObject config;

    private static SharedConfig instance;

    public static SharedConfig getSuperInstance() {
        if (instance == null) {
            instance = new SharedConfig();
        }
        return instance;
    }

    public void init(JsonObject configInit) {
        config = configInit;
    }

    public Optional<JsonObject> getConfig() {
        return Optional.of(config);
    }

    @Override
    public Optional<String> getHost() {
        return Optional.ofNullable(config.getJsonObject("server").getString("host"));
    }

    @Override
    public Optional<Integer> getPort() {
        return Optional.ofNullable(config.getJsonObject("server").getInteger("port"));
    }

    public Object getValue(String pattern) {
        var conf = config;
        final String[] accessors = pattern.split("\\.");
        final int numAccessors = accessors.length;
        final int lastAccessorIndex = numAccessors - 1;
        int i = 0;
        while (i < lastAccessorIndex && conf != null) {
            conf = conf.getJsonObject(accessors[i]);
            i++;
        }

        return (conf == null ? null : conf.getValue(accessors[lastAccessorIndex]));
    }
} 
