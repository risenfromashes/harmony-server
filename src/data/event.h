#pragma once

#include <iostream>
#include <simdjson.h>

struct Event {
  static std::optional<Event> from_json(simdjson::ondemand::document &doc) {
    Event ret;
    auto event_name = doc["event_name"];
    auto group_id = doc["group_id"];
    // check error
    // auto text = doc["text"];

    // if (user_id.error() || group_id.error() || text.error()) {
    //   std::cerr << "json parsing error" << std::endl;
    //   std::cout << user_id.error() << std::endl;
    //   std::cout << group_id.error() << std::endl;
    //   std::cout << text.error() << std::endl;
    //   return std::nullopt;
    // }
    // // check types
    // if (!user_id.is_scalar() || !group_id.is_scalar() || !text.is_scalar()) {
    //   std::cerr << "json type error" << std::endl;
    //   return std::nullopt;
    // }
    // // check null
    // if (user_id.is_null() || group_id.is_null()) {
    //   std::cerr << "json null error" << std::endl;
    //   return std::nullopt;
    // }

    // ret.user_id = user_id;
    // ret.group_id = group_id;
    // ret.text = text;
    return ret;
  }

  // std::string_view user_id;
  // std::string_view group_id;
  // std::string_view text;
};
