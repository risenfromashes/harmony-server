
#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct SubjectRemove {
  static std::optional<SubjectRemove>
  from_json(simdjson::ondemand::document &doc) {
    SubjectRemove ret;
    auto user_id = doc["user_id"];
    auto subject_id = doc["subject_id"];
    auto group_id = doc["group_id"];
    // check error
    if (check_error(user_id, group_id, subject_id)) {
      std::cerr << "SubjectRemove json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(user_id, group_id)) {
      std::cerr << "SubjectRemove json null error" << std::endl;
      return std::nullopt;
    }

    ret.user_id = user_id;
    ret.subject_id = subject_id;
    ret.group_id = group_id;
    return ret;
  }

  std::string_view user_id;
  std::string_view subject_id;
  std::string_view group_id;
};
