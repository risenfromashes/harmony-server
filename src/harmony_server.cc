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
  server.post("/add-poll", add_poll);
  server.post("/update-poll", update_poll);
  server.post("/remove-poll", remove_poll);
  server.post("/change-vote", change_vote);
  server.post("/update-group", update_group_info);
  server.post("/update-member", update_member);
  server.post("/add-subject", add_subject);
  server.post("/remove-subject", remove_subject);
  server.post("/remove-group", remove_group);
  server.post("/add-event", add_event);
  server.post("/update-event", update_event);
  server.post("/remove-event", remove_event);

  server.get("/users/{user_id}", get_users);
  server.get("/user-info/{user_id}", get_user_info);
  server.get("/groups/{user_id}", get_groups);
  server.get("/posts/{user_id}/{group_id}/{subject_id}/{parent_id}", get_posts);
  server.get("/members/{user_id}/{group_id}", get_members);
  server.get("/feed", get_feed);
  server.get("/polls/{user_id}/{group_id}", get_polls);
  server.get("/get-events/{user_id}/{group_id}", get_events);
  server.get("/group-messages/{user_id}/{group_id}/{subject_id}",
             get_group_messages);
  server.get("/events/{user_id}", create_event_source);

  server.listen();
}
