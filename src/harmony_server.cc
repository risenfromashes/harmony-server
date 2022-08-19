#include "harmony/http2.h"

#include "routes.h"

int main(int argc, char **argv) {
  hm::Server server(hm::Server::load_config("../config.json"));

  server.get("/", [](auto *req, auto *res) { res->send_file("/index.html"); });

  server.get("/assets/*", [](auto *req, auto *res) { res->send_file(); });

  server.post("/login", login_user);
  server.post("/register", register_user);
  server.post("/post", add_post);
  server.post("/group-message", add_post);

  server.get("/groups/{user_id}", get_groups);
  server.get("/posts/{user_id}", get_posts);
  server.get("/feed", get_feed);
  server.get("/group-messages/{user_id}/{group_id}/{subject_id}",
             get_group_messages);

  server.get("/events/{user_id}", create_event_source);

  server.listen();
}
