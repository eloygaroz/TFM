FROM maven:3.8.6-openjdk-11 AS maven

WORKDIR /app
COPY . .

RUN mvn -s src/main/resources/settings.xml clean package
RUN ls /app/target

FROM hirokimatsumoto/alpine-openjdk-11 AS profiler

WORKDIR /app
COPY --from=maven /app/target/api-vertx-1.0.0.jar /app/app.jar
COPY --from=maven /app/src/main/resources/config.yaml /app/config.yaml

FROM adoptopenjdk/openjdk11:x86_64-alpine-jdk-11.0.22_7-slim

WORKDIR /app
EXPOSE 8080

ENV SERVICE_NAME api-vertx
ENV SERVICE_VERSION 1.0.0

COPY --from=profiler /app/app.jar ./app.jar
COPY --from=profiler /app/config.yaml /app/config.yaml

ENV VERTX_CONFIG_PATH /app/config.yaml

ENTRYPOINT [ "sh", "-c", "java -jar /app/app.jar"]