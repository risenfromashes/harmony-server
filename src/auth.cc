#include "auth.h"

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
      response->set_header("set-cookie", std::move(sid));
      response->send_json(LoginData{.success = true, .id = user_id}.to_json());
      co_return;
    }
  }
  // response.s
  response->set_status("400");
  response->send_json(
      LoginData{.success = false,
                .reason = res.is_error() ? res.error_message()
                                         : "Username or password incorrect"}
          .to_json());
}
