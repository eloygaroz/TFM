package com.api;

import com.api.commons.Utils;
import com.api.config.SharedConfig;
import com.api.verticles.MainVerticle;
import io.netty.channel.DefaultChannelId;
import io.reactivex.rxjava3.core.Single;
import io.vertx.config.ConfigRetrieverOptions;
import io.vertx.config.ConfigStoreOptions;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.DeploymentOptions;
import io.vertx.core.json.JsonObject;
import io.vertx.rxjava3.config.ConfigRetriever;
import io.vertx.rxjava3.core.Vertx;

public class Main {


    public static void main(String[] args) {

        DefaultChannelId.newInstance();

        var deploymentOptions = new DeploymentOptions();
        var vertx = Vertx.vertx();
        getConfig(vertx).subscribe(
                config -> {
                    SharedConfig.getSuperInstance().init(config);
                    Class<? extends AbstractVerticle> verticle = MainVerticle.class;
                    vertx.rxDeployVerticle(verticle.getName(), deploymentOptions).subscribe();
                }
        );
    }

    private static Single<JsonObject> getConfig(Vertx vertx) {
        if (Utils.getEnv("VERTX_CONFIG_PATH") == null) {
            return Single.error(new IllegalStateException("Missing VERTX_CONFIG_PATH environment variable"));
        }
        return ConfigRetriever.create(vertx, generateConfigRetrieverOptions()).rxGetConfig();
    }

    private static ConfigRetrieverOptions generateConfigRetrieverOptions() {
         return new ConfigRetrieverOptions().addStore(new ConfigStoreOptions()
                .setType("file")
                .setFormat("yaml")
                .setConfig(new JsonObject().put("path", Utils.getEnv("VERTX_CONFIG_PATH"))));
    }

}
