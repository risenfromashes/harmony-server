#include "harmony/http2.h"

#include "routes.h"

int main(int argc, char **argv) {
  hm::Server server(hm::Server::load_config("../config.json"));

  server.get("/", [](auto *req, auto *res) { res->send_file("/index.html"); });

  server.get("/assets/*", [](auto *req, auto *res) { res->send_file(); });

  server.get("/image/{name}", send_image);
  server.post("/login", login_user);
  server.post("/register", register_user);
  server.post("/update-info", update_user_info);
  server.post("/post", add_post);
  server.post("/group-message", add_group_message);
  server.post("/upload-image/{user_id}", add_image);
  server.post("/add-group", add_group);

  server.get("/users/{user_id}", get_users);
  server.get("/user-info/{user_id}", get_user_info);
  server.get("/groups/{user_id}", get_groups);
  server.get("/posts/{user_id}/{group_id}/{subject_id}/{parent_id}", get_posts);
  server.get("/feed", get_feed);
  server.get("/group-messages/{user_id}/{group_id}/{subject_id}",
             get_group_messages);

  server.get("/events/{user_id}", create_event_source);

  server.listen();
}
