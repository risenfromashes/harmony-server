#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct PollRemove {
  static std::optional<PollRemove>
  from_json(simdjson::ondemand::document &doc) {
    PollRemove ret;
    auto user_id = doc["user_id"];
    auto poll_id = doc["poll_id"];
    // check error
    if (check_error(user_id, poll_id)) {
      std::cerr << "PollRemove json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, poll_id)) {
      std::cerr << "PollRemove json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.poll_id = poll_id;
    return ret;
  }

  std::string_view user_id;
  std::string_view poll_id;
};
