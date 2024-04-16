package com.api.commons;

import java.util.function.Function;

public class Utils {

  private Utils() {
  }

  public static String getEnv(String variable) {
    return ((Function<String, String>) System::getenv).apply(variable);
  }
}
