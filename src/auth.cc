#include "auth.h"
#include "data/user.h"

hm::AwaitableTask<> authenticate_user(hm::HttpResponse *response,
                                      std::string_view name,
                                      std::string_view password) {
  auto db = response->get_db_connection();
  // first check if the credentials are correct
  auto res = co_await db.query_prepared("login_user", name, password);

  if (!res.is_error() && res.num_rows()) {
    assert(res["user_id"].has_value());
    auto user_id = res["user_id"].value();

    auto sid = uuids::to_string(hm::util::generate_uuid());
    auto res = co_await db.query_prepared("add_session", sid, user_id);
    if (!res.is_error()) {
      // successfully loginned user
      response->set_status("200");
      auto sid_str = "sid=" + sid;
      response->set_header("set-cookie", sid_str);
      response->send_json(Login{.success = true, .id = user_id}.to_json());
      co_return;
    } else {
      response->set_status("400");
      response->send_json(
          Login{.success = false, .reason = res.error_message()}.to_json());
    }
  }
  // response.s
  response->set_status("400");
  response->send_json(Login{.success = false,
                            .reason = res.is_error()
                                          ? res.error_message()
                                          : "Username or password incorrect"}
                          .to_json());
}

hm::AwaitableTask<bool> is_authenticated(hm::HttpResponse *response,
                                         std::string_view sid,
                                         std::string_view user_id) {
  std::cout << "sending query" << std::endl;
  auto db = response->get_db_connection();
  auto res = co_await db.query_prepared("check_session", sid, user_id);
  co_return res.exists();
}

hm::AwaitableTask<bool> is_authenticated(hm::HttpRequest *req,
                                         hm::HttpResponse *res,
                                         std::string_view user_id) {
  auto sid = req->get_cookie("sid");

  if (sid.has_value()) {
    bool authenticated = co_await is_authenticated(res, sid.value(), user_id);
    co_return authenticated;
  }

  co_return false;
}

hm::AwaitableTask<bool> is_authenticated(hm::HttpRequest *req,
                                         hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto uid = User::from_json(body);
  auto sid = req->get_cookie("sid");

  if (uid.has_value() && sid.has_value()) {
    bool authenticated =
        co_await is_authenticated(res, sid.value(), uid.value().user_id);
    co_return authenticated;
  }

  co_return false;
}
