#pragma once

#include <harmony/http2.h>
#include <string_view>

struct Login {
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
    auto ret = writer.string();
    std::cout << "=======" << std::endl;
    std::cout << ret << std::endl;
    std::cout << "=======" << std::endl;
    return ret;
  }

  operator std::string() { return to_json(); }
};
