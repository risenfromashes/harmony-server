#include "routes.h"

#include "data/error.h"
#include "data/groupmessage.h"
#include "data/post.h"
#include "data/register.h"
#include "data/user.h"

#include <cstdio>

hm::Task<> login_user(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto name = body["user_name"];
  auto password = body["password"];
  std::cout << "user: " << name << std::endl;
  std::cout << "password: " << password << std::endl;
  if (name.error() || password.error()) {
    std::cout << "error" << std::endl;
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
  auto reg = Register::from_json(body);
  if (reg) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("register_user", reg->user_name,
                                         reg->password, reg->first_name,
                                         reg->middle_name, reg->last_name);
    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json("{}");
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }
  } else {
    res->set_status("400");
    res->send_json(Error{.reason = "Invalid data"});
  }
}

hm::Task<> get_groups(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("get_groups", uid.value());

    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json(std::move(rt));
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }

  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> get_posts(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("get_posts", uid.value());

    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json(rt.to_json());
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }

  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> get_group_messages(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  auto gid = req->get_param("group_id");
  auto sid = req->get_param("subject_id");

  if (!uid || !gid || !sid) {
    res->set_status("400");
    res->send_json(Error{.reason = "Invalid request"});
    co_return;
  }

  if (co_await is_authenticated(req, res, uid.value())) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("get_group_messages", uid.value(),
                                         gid.value(), sid.value());

    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json(std::move(rt));
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }

  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> add_post(hm::HttpRequest *req, hm::HttpResponse *res) {
  // TODO: MAJOR BUG, WHAT IF BODY IS ALREADY RECEIVED?
  auto body = co_await req->json();
  auto post = Post::from_json(body);

  if (post && co_await is_authenticated(req, res, post->user_id)) {
    std::cout << "Got text: " << post->text << std::endl;
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("add_post", post->user_id,
                                         post->group_id, post->text);

    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json("{}");
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }

  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> add_group_message(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto msg = GroupMessage::from_json(body);

  if (!msg) {
    res->set_status("400");
    res->send_json(Error{.reason = "Invalid request"});
    co_return;
  }

  if (co_await is_authenticated(req, res, msg->user_id)) {
    std::cout << "Got text: " << msg->content << std::endl;
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("add_group_message", msg->user_id,
                                         msg->group_id, msg->subject_id,
                                         msg->content);
    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json(std::move(rt));
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }
  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> add_event(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto post = Post::from_json(body);

  if (post && co_await is_authenticated(req, res, post->user_id)) {
    std::cout << "Got text: " << post->text << std::endl;
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("add_post", post->user_id,
                                         post->group_id, post->text);

    if (!rt.is_error()) {
      res->set_status("200");
      res->send_json("{}");
    } else {
      res->set_status("400");
      res->send_json(Error{.reason = rt.error_message()});
    }

  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> create_event_source(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {
    auto ev = hm::EventSource::create(req, res);
    auto db = res->get_db_connection();
    auto channels = co_await db.query_params(
        "SELECT * FROM register_user_events($1::int4)", uid.value());
    for (std::string_view channel : channels) {
      ev.suscribe(channel);
    }
  }
}
