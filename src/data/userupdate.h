#pragma once

#include <iostream>
#include <optional>
#include <simdjson.h>

#include "check_error.h"

struct UserUpdate {
  static std::optional<UserUpdate>
  from_json(simdjson::ondemand::document &doc) {
    UserUpdate ret;
    auto user_id = doc["id"];
    auto user_name = doc["user_name"];
    auto password = doc["password"];
    auto first_name = doc["first_name"];
    auto middle_name = doc["middle_name"];
    auto last_name = doc["last_name"];
    auto dp_id = doc["dp_id"];

    if (check_error(user_id, user_name, password, first_name, middle_name,
                    last_name, dp_id)) {
      std::cerr << "UserUpdate parsing error" << std::endl;
      return std::nullopt;
    }

    if (check_null(user_id, user_name, password)) {
      std::cerr << "UserUpdate NULL value error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.user_name = user_name;
    ret.password = password;
    ret.first_name = first_name;
    ret.middle_name = middle_name;
    ret.last_name = last_name;
    ret.dp_id = dp_id;
    return ret;
  }

  std::string_view user_id;
  std::string_view user_name;
  std::string_view password;
  std::string_view first_name;
  std::string_view middle_name;
  std::string_view last_name;
  std::string_view dp_id;
};
