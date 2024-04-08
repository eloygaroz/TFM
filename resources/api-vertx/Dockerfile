# first stage: Package microservice
FROM maven:3.6.0-jdk-11 AS maven

WORKDIR /app
COPY . .

RUN mvn -s settings.xml clean package

# Second stage: setup your microservice
FROM hirokimatsumoto/alpine-openjdk-11 AS profiler

WORKDIR /app

COPY --from=maven /app/target/*-with-dependencies-shade.jar /app/app.jar
COPY --from=maven /app/src/main/resources/config.yaml /app/config.yaml

# Second stage: setup your microservice
FROM adoptopenjdk/openjdk11:jdk-11.0.2.9-alpine-slim

WORKDIR /app
EXPOSE 8080

ENV SERVICE_NAME api-vertx
ENV SERVICE_VERSION 1.0.0

COPY --from=profiler /app/app.jar ./app.jar

ENTRYPOINT [ "sh", "-c", "java -jar /app/app.jar"]