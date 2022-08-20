#pragma once

#include <iostream>
#include <simdjson.h>

#include "check_error.h"

struct Register {
  static std::optional<Register> from_json(simdjson::ondemand::document &doc) {
    Register ret;
    auto user_name = doc["user_name"];
    auto password = doc["password"];
    auto first_name = doc["first_name"];
    auto middle_name = doc["middle_name"];
    auto last_name = doc["last_name"];

    check_error(user_name, password, first_name, middle_name, last_name);
    check_null(user_name, password);

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
