package com.api.config;


public class Config extends SharedConfig {

    private static Config instance;

    private Config() {
        super();

        var conf = Config.getSuperInstance();
        if (conf.getConfig().isPresent()) {
            this.config = conf.getConfig().get();
        }
    }

    public static Config getInstance() {
        if (instance == null) {
            instance = new Config();
        }
        return instance;
    }

    public String getOpenApiFilePath() {
        String openApiFilePath = (String) getValue("openapi.filePath");
        if (openApiFilePath == null || openApiFilePath.isEmpty()) {
            throw new RuntimeException("The OpenAPI definition file path is empty or is null");
        }
        return openApiFilePath;
    }

    public String getSomePropertyFromConfigFilegetSomePropertyFromConfigFile() {
        return getValue("api.example.host").toString();
    }

}