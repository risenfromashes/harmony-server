#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct GroupUpdate {
  static std::optional<GroupUpdate>
  from_json(simdjson::ondemand::document &doc) {
    GroupUpdate ret;
    auto user_id = doc["user_id"];
    auto group_id = doc["group_id"];
    auto name = doc["name"];
    auto intro = doc["intro"];
    auto photo_id = doc["photo_id"];
    // check error
    if (check_error(user_id, group_id, name, intro, photo_id)) {
      std::cerr << "GroupUpdate json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id)) {
      std::cerr << "GroupUpdate json null error" << std::endl;
      return std::nullopt;
    }

    if (check_null(photo_id)) {
      ret.photo_id = "0";
    } else {
      ret.photo_id = photo_id;
    }

    ret.user_id = user_id;
    ret.group_id = group_id;
    ret.name = name;
    ret.intro = intro;
    return ret;
  }

  std::string_view user_id;
  std::string_view group_id;
  std::string_view name;
  std::string_view intro;
  std::string_view photo_id;
};
