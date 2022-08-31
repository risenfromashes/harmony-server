#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct GroupRemove {
  static std::optional<GroupRemove>
  from_json(simdjson::ondemand::document &doc) {
    GroupRemove ret;
    auto user_id = doc["user_id"];
    auto group_id = doc["group_id"];
    // check error
    if (check_error(user_id, group_id)) {
      std::cerr << "GroupDelete json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id)) {
      std::cerr << "GroupDelete json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.group_id = group_id;
    return ret;
  }

  std::string_view user_id;
  std::string_view group_id;
};
