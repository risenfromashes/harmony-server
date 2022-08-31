
#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct EventRemove {
  static std::optional<EventRemove>
  from_json(simdjson::ondemand::document &doc) {
    EventRemove ret;
    auto user_id = doc["user_id"];
    auto event_id = doc["event_id"];
    // check error
    if (check_error(user_id, event_id)) {
      std::cerr << "Event json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, event_id)) {
      std::cerr << "Event json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.event_id = event_id;
    return ret;
  }

  std::string_view user_id;
  std::string_view event_id;
};
