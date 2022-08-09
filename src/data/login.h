#pragma once

#include <harmony/http2.h>
#include <string_view>

struct LoginData {
  bool success;
  std::string_view reason;
  std::string_view id;

  std::string to_json() {
    hm::json::Writer writer;
    {
      hm::json::Object root = writer.root();
      root["success"] = success;
      root["reason"] = reason;
      root["id"] = id;
    }
    return writer.string();
  }
};
