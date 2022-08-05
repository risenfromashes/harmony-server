#include "harmony/http2.h"

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
  int threads = std::thread::hardware_concurrency();
  double timeout = 0.0;
#ifndef NDEBUG
  const char *port = "5000";
#else
  const char *port = "443";
#endif

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
  server.connect_database("postgresql:///testdb1");

  server.get("/", [](auto *req, auto *res) { res->send_file("/index.html"); });

  server.get("/users",
             [](hm::HttpRequest *req, hm::HttpResponse *res) -> hm::Task<> {
               auto db = res->get_db_connection();
               auto users = co_await db.query("SELECT * FROM users;");
               if (users.is_error()) {
                 res->set_status("404");
                 res->send_html("<html> Failed to select users; " +
                                std::string(users.error_message()) + "</html>");
                 co_return;
               }
               res->send_json(users.to_json());
             });

  server.listen(timeout);
}
