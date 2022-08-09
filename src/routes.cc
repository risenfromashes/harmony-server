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
