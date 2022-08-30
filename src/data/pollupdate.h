#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct PollUpdate {
  static std::optional<PollUpdate>
  from_json(simdjson::ondemand::document &doc) {
    PollUpdate ret;
    auto user_id = doc["user_id"];
    auto poll_id = doc["poll_id"];
    auto title = doc["title"];
    auto options = doc["options"];
    // check error
    if (check_error(user_id, poll_id, title, options)) {
      std::cerr << "PollUpdate json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, poll_id, title)) {
      std::cerr << "PollUpdate json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.poll_id = poll_id;
    ret.title = title;
    ret.options = options;
    return ret;
  }

  std::string_view user_id;
  std::string_view poll_id;
  std::string_view title;
  std::string_view options;
};
