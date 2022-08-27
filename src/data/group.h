#pragma once

#include <iostream>
#include <simdjson.h>

#include "check_error.h"

struct Group {
  static std::optional<Group> from_json(simdjson::ondemand::document &doc) {
    Group ret;
    auto creator_id = doc["creator_id"];
    auto name = doc["name"];
    auto intro = doc["intro"];
    auto members = doc["members"];
    // check error

    if (check_error(creator_id, name, intro, members)) {
      std::cerr << "Group Parsing error" << std::endl;
      return std::nullopt;
    }

    if (check_null(creator_id, name)) {
      std::cerr << "Null check error" << std::endl;
      return std::nullopt;
    }

    ret.creator_id = creator_id;
    ret.name = name;
    ret.intro = intro;
    ret.members = members;
    return ret;
  }

  std::string_view creator_id;
  std::string_view name;
  std::string_view intro;
  std::string_view members;
};
