#pragma once

#include <iostream>
#include <simdjson.h>

bool check_error(auto &&val, auto &&...rest) {
  if (val.error()) {
    std::cerr << val.error() << std::endl;
    return true;
  }

  if constexpr (sizeof...(rest) > 0) {
    return check_error(std::forward<decltype(rest)>(rest)...);
  }
  return false;
}

bool check_null(auto &&val, auto &&...rest) {
  if (val.is_null()) {
    return true;
  }

  if constexpr (sizeof...(rest) > 0) {
    return check_null(std::forward<decltype(rest)>(rest)...);
  }
  return false;
}
