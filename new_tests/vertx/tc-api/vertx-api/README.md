# api-vertx

[![Alt text](https://img.shields.io/badge/vert.x-4.5.7-purple.svg)](https://vertx.io)

This template could be a started point in order to developer an Async API over JVM

## Index

- [Stack info](#stack-info)
- [How to run](#how-to-run)
  - [How to run as Docker](#how-to-run-as-docker)
- [Service Info](#service-info)
  - [Defaults endpoint](#defaults-endpoint)
- [Help](#help)

## Stack info

-   Java
-   Maven
-   Docker

## PR format
<#type>(<#module>): <#JIRA_ID> <#message>
- example:
fix(dummy-api): #AUT-30 consume data from different selector

Where Type must be one of the following values:
- feature
- hotfix (bug fix with the environment)
- fix (bug fix)
- docs (documentation)
- refactor
- test (when adding missing tests)
- chore

## How to run

### How to run as Docker

``` bash
docker run -rm -d -p 8080:8080 -v $(pwd)/src/main/resources:/tmp -e VERTX_CONFIG_PATH=/app/config.json --name my-microservice my-microservice
```

## Service Info
### Defaults endpoint

``` bash
curl -v http://localhost:8080/health
```

## Help

* [Vert.x Documentation](https://vertx.io/docs/)
* [Vert.x Stack Overflow](https://stackoverflow.com/questions/tagged/vert.x?sort=newest&pageSize=15)
* [Vert.x User Group](https://groups.google.com/forum/?fromgroups#!forum/vertx)
* [Vert.x Gitter](https://gitter.im/eclipse-vertx/vertx-users)
