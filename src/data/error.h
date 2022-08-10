#pragma once

#include <harmony/http2.h>
#include <string_view>

struct Error {
  std::string_view reason;

  std::string to_json() {
    hm::json::Writer writer;
    {
      hm::json::Object root = writer.root();
      root["reason"] = reason;
    }
    return writer.string();
  }

  operator std::string() { return to_json(); }
};
