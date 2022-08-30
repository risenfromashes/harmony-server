#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct VoteChange {
  static std::optional<VoteChange>
  from_json(simdjson::ondemand::document &doc) {
    VoteChange ret;
    auto user_id = doc["user_id"];
    auto poll_id = doc["poll_id"];
    auto option_id = doc["option_id"];
    auto add = doc["add"];
    // check error
    if (check_error(user_id, poll_id, option_id, add)) {
      std::cerr << "VoteChange json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, poll_id, option_id, add)) {
      std::cerr << "VoteChange json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.poll_id = poll_id;
    ret.option_id = option_id;
    ret.add = add;
    return ret;
  }

  std::string_view user_id;
  std::string_view poll_id;
  std::string_view option_id;
  std::string_view add;
};
