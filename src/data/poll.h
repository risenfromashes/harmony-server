#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct Poll {
  static std::optional<Poll> from_json(simdjson::ondemand::document &doc) {
    Poll ret;
    auto user_id = doc["user_id"];
    auto title = doc["title"];
    auto group_id = doc["group_id"];
    auto options = doc["options"];
    // check error
    if (check_error(user_id, title, group_id, options)) {
      std::cerr << "Poll json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id)) {
      std::cerr << "Poll json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.title = title;
    ret.group_id = group_id;
    ret.options = options;
    return ret;
  }

  std::string_view user_id;
  std::string_view title;
  std::string_view group_id;
  std::string_view options;
};
