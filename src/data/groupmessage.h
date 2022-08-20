#pragma once

#include "check_error.h"

#include <iostream>
#include <simdjson.h>

struct GroupMessage {
  static std::optional<GroupMessage>
  from_json(simdjson::ondemand::document &doc) {
    GroupMessage ret;
    auto user_id = doc["user_id"];
    auto group_id = doc["group_id"];
    auto subject_id = doc["subject_id"];
    auto content = doc["content"];

    if (check_error(user_id, group_id, subject_id, content)) {
      std::cerr << "GroupMessage Parsing error" << std::endl;
      return std::nullopt;
    }

    if (check_null(user_id, group_id, subject_id)) {
      std::cerr << "Null check error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.group_id = group_id;
    ret.subject_id = subject_id;
    ret.content = content;
    return ret;
  }

  std::string_view user_id;
  std::string_view group_id;
  std::string_view subject_id;
  std::string_view content;
};
