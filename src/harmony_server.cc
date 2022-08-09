#include "harmony/http2.h"

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "routes.h"

int main(int argc, char **argv) {
  int threads = std::thread::hardware_concurrency();
  double timeout = 0.0;
  // #ifndef NDEBUG
  const char *port = "5000";
  // #else
  //   const char *port = "443";
  // #endif

  for (int i = 1; i < argc; i++) {
    std::string_view arg = argv[i];
    if (arg == "--threads" && (i + 1) < argc) {
      threads = std::atoi(argv[i + 1]);
    }
    if (arg == "--timeout" && (i + 1) < argc) {
      timeout = std::atoi(argv[i + 1]);
    }

    if (arg == "--port" && (i + 1) < argc) {
      port = argv[i + 1];
    }
  }

  std::cout << "Server starting with " << threads << " threads and " << timeout
            << " seconds timeout on port " << port << std::endl;

  hm::Server server({.num_threads = threads,
                     .port = port,
                     .dhparam_file = "../harmony-http/certs/dhparam.pem",
                     .cert_file = "../certs/cert.pem",
                     .key_file = "../certs/key.pem"});

  server.serve_static_files("../harmony-web/dist/");
  server.connect_database("postgresql:///testdb2");
  server.set_query_location("../harmony-data/prepared/");

  server.get("/", [](auto *req, auto *res) { res->send_file("/index.html"); });

  server.post("/login", login_user);
  server.get("/feed", get_feed);

  server.listen(timeout);
}
