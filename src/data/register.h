#pragma once

#include <iostream>
#include <simdjson.h>

struct Register {
  static std::optional<Register> from_json(simdjson::ondemand::document &doc) {
    Register ret;
    auto user_name = doc["user_name"];
    auto password = doc["password"];
    auto first_name = doc["first_name"];
    auto middle_name = doc["middle_name"];
    auto last_name = doc["last_name"];
    // check error

    if (user_name.error() || password.error() || first_name.error() ||
        middle_name.error() || last_name.error()) {
      std::cerr << "json parsing error" << std::endl;
      return std::nullopt;
    }
    // check types
    if (!user_name.is_scalar() || !password.is_scalar() ||
        !first_name.is_scalar() || !middle_name.is_scalar() ||
        !last_name.is_scalar()) {
      std::cerr << "json type error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (user_name.is_null() || password.is_null()) {
      std::cerr << "json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_name = user_name;
    ret.password = password;
    ret.first_name = first_name;
    ret.middle_name = middle_name;
    ret.last_name = last_name;
    return ret;
  }

  std::string_view user_name;
  std::string_view password;
  std::string_view first_name;
  std::string_view middle_name;
  std::string_view last_name;
};
