#pragma once

#include "check_error.h"
#include <iostream>
#include <simdjson.h>

struct Post {
  static std::optional<Post> from_json(simdjson::ondemand::document &doc) {
    Post ret;
    auto poster_id = doc["poster_id"];
    auto group_id = doc["group_id"];
    auto subject_id = doc["subject_id"];
    auto parent_post_id = doc["parent_post_id"];
    auto type = doc["type"];
    auto content = doc["content"];
    // check error
    if (check_error(poster_id, group_id, subject_id, parent_post_id, type,
                    content)) {
      std::cerr << "Post json parsing error" << std::endl;
      return std::nullopt;
    }
    // check null
    if (check_null(poster_id, group_id, subject_id)) {
      std::cerr << "Post json null error" << std::endl;
      return std::nullopt;
    }

    ret.poster_id = poster_id;
    ret.group_id = group_id;
    ret.subject_id = subject_id;
    ret.parent_post_id = parent_post_id;
    ret.type = type;
    ret.content = content;
    return ret;
  }

  std::string_view poster_id;
  std::string_view group_id;
  std::string_view subject_id;
  std::string_view parent_post_id;
  std::string_view type;
  std::string_view content;
};
