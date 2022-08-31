#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct Subject {
  static std::optional<Subject> from_json(simdjson::ondemand::document &doc) {
    Subject ret;
    auto user_id = doc["user_id"];
    auto group_id = doc["group_id"];
    auto name = doc["name"];
    // check error
    if (check_error(user_id, group_id, name)) {
      std::cerr << "Subject json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id)) {
      std::cerr << "Subject json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.group_id = group_id;
    ret.name = name;
    return ret;
  }

  std::string_view user_id;
  std::string_view group_id;
  std::string_view name;
};
