#include "routes.h"

#include "data/error.h"
#include "data/register.h"

hm::Task<> login_user(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto name = body["user_name"];
  auto password = body["password"];
  if (name.error() || password.error()) {
    res->set_status("400");
    res->send_json(
        Login{.success = false, .reason = "Invalid Request"}.to_json());
    co_return;
  }
  co_await authenticate_user(res, name.value(), password.value());
}

hm::Task<> get_feed(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto user_id = body["user_id"];
  auto sid = req->get_cookie("sid");

  if (!user_id.error() && !user_id.is_scalar() && // user_id can't be integer
      sid.has_value()) {
    bool authenticated = co_await is_authenticated(res, sid.value(), user_id);
    if (authenticated) {
      res->set_status("200");
      res->send_json(Login{.success = true, .id = user_id}.to_json());
      co_return;
    }
  }
  res->set_status("403");
  res->send_json(R"({"sucess":false, "id":-1})");
}

hm::Task<> register_user(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto reg_ = Register::from_json(body);
  if (reg_.has_value()) {
    auto reg = reg_.value();
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("register_user", reg.user_name,
                                         reg.password, reg.first_name,
                                         reg.middle_name, reg.last_name);
    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json("{}");
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()}.to_json());
    }
  } else {
    res->set_status("400");
    res->send_json(Error{.reason = "Invalid data"}.to_json());
  }
}
