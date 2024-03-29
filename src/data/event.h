#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct Event {
  static std::optional<Event> from_json(simdjson::ondemand::document &doc) {
    Event ret;
    auto user_id = doc["user_id"];
    auto group_id = doc["group_id"];
    auto event_title = doc["event_title"];
    auto event_description = doc["event_description"];
    auto event_time = doc["event_time"];
    // check error
    if (check_error(user_id, event_title, group_id, event_description,
                    event_time)) {
      std::cerr << "Event json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id)) {
      std::cerr << "Event json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.group_id = group_id;
    ret.event_title = event_title;
    ret.event_description = event_description;
    ret.event_time = event_time;
    return ret;
  }

  std::string_view user_id;
  std::string_view group_id;
  std::string_view event_title;
  std::string_view event_description;
  std::string_view event_time;
};
