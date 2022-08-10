#pragma once

#include <iostream>
#include <simdjson.h>

struct User {
  static std::optional<User> from_json(simdjson::ondemand::document &doc) {
    User ret;
    auto user_id = doc["user_id"];
    // check error

    if (user_id.error()) {
      std::cerr << "json parsing error" << std::endl;
      return std::nullopt;
    }
    // check types
    if (!user_id.is_scalar()) {
      std::cerr << "json type error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (!user_id.is_null()) {
      std::cerr << "json null error" << std::endl;
      return std::nullopt;
    }
    ret.user_id = user_id;
    return ret;
  }

  std::string_view user_id;
};
