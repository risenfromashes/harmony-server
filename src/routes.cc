#include "routes.h"

hm::Task<> login_user(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto name = body["user_name"];
  auto password = body["password"];
  if (name.error() || password.error()) {
    res->set_status("400");
    res->send_json(
        LoginData{.success = false, .reason = "Invalid Request"}.to_json());
    co_return;
  }
  co_await authenticate_user(res, name.value(), password.value());
}

hm::Task<> get_feed(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto user_id = body["user_id"];
  auto sid = req->get_cookie("sid");
  if (!user_id.error() && sid.has_value()) {
    int64_t uid = user_id.get_int64();
    std::cout << "calling is_authenticated" << std::endl;
    std::cout << uid << std::endl;
    bool authenticated = co_await is_authenticated(res, sid.value(), uid);
    if (authenticated) {
      std::cout << "is authenticated" << std::endl;
      res->set_status("200");
      res->send_json(R"({"sucess":true, "id":8})");
      co_return;
    }
  }
  std::cout << "is not authenticated" << std::endl;
  res->set_status("403");
  res->send_json(R"({"sucess":false, "id":-1})");
}
