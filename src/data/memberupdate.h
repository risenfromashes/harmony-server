#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct MemberUpdate {
  static std::optional<MemberUpdate>
  from_json(simdjson::ondemand::document &doc) {
    MemberUpdate ret;
    auto user_id = doc["user_id"];
    auto member_id = doc["member_id"];
    auto group_id = doc["group_id"];
    auto add = doc["add"];
    // check error
    if (check_error(user_id, group_id, member_id, add)) {
      std::cerr << "MemberUpdate json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id, member_id)) {
      std::cerr << "GroupUpdate json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.member_id = member_id;
    ret.group_id = group_id;
    ret.add = add;
    return ret;
  }

  std::string_view user_id;
  std::string_view member_id;
  std::string_view group_id;
  std::string_view add;
};
