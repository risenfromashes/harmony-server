#include "routes.h"

#include "auth.h"
#include "data/error.h"
#include "data/group.h"
#include "data/groupmessage.h"
#include "data/post.h"
#include "data/register.h"
#include "data/user.h"
#include "data/userupdate.h"

#include <cstdio>
#include <fstream>

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

hm::Task<> get_user_info(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("get_user_info", uid.value());

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

hm::Task<> update_user_info(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto update = UserUpdate::from_json(body);
  if (update && co_await is_authenticated(req, res, update->user_id)) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared(
        "update_user_info", update->user_id, update->user_name,
        update->password, update->first_name, update->middle_name,
        update->last_name, update->dp_id);

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

hm::Task<> get_users(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_prepared("get_users");

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
  auto gid = req->get_param("group_id");
  auto sid = req->get_param("subject_id");
  auto pid = req->get_param("parent_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {

    auto db = res->get_db_connection();
    auto rt = co_await db.query_params(
        "SELECT get_posts($1::INT, $2::INT, $3::INT, $4::INT);", uid.value(),
        gid.value(), sid.value(), pid.value());

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
  auto body = co_await req->json();
  auto post = Post::from_json(body);

  if (post && co_await is_authenticated(req, res, post->poster_id)) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_params(
        "SELECT add_post($1::INT, $2::INT, $3::INT, $4::INT,"
        "$5::VARCHAR, $6::TEXT);",
        post->poster_id, post->group_id, post->subject_id, post->parent_post_id,
        post->type, post->content);

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
    auto rt = co_await db.query_params(
        "SELECT add_group_message($1::int, $2::int, $3::int, $4::text)",
        msg->user_id, msg->group_id, msg->subject_id, msg->content);
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

hm::Task<> add_group(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto body = co_await req->json();
  auto g = Group::from_json(body);

  if (!g) {
    res->set_status("400");
    res->send_json(Error{.reason = "Invalid request"});
    co_return;
  }

  if (co_await is_authenticated(req, res, g->creator_id)) {
    auto db = res->get_db_connection();
    auto rt = co_await db.query_params(
        "SELECT create_group($1::int, $2::varchar, $3::text, $4::int[])",
        g->creator_id, g->name, g->intro, g->members);

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
  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> add_image(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto uid = req->get_param("user_id");
  if (uid && co_await is_authenticated(req, res, uid.value())) {
    auto uuid = hm::util::generate_uuid();

    auto type = req->get_header("content-type");
    if (!type) {
      res->set_status("400");
      res->send_json("{}");
    }

    auto ext = hm::util::get_extension(type.value());
    auto file_name = uuids::to_string(uuid) + "." + std::string(ext);
    std::ofstream out("../uploads/" + file_name);
    for (;;) {
      auto data = co_await req->data();
      if (data.empty()) {
        break;
      }
      out.write(data.data(), data.length());
    }
    out.close();

    auto db = res->get_db_connection();
    auto rt = co_await db.query_params(
        "SELECT add_picture($1::int4, $2::text, $3::varchar);", uid.value(),
        "/image/" + file_name, type.value());

    res->set_status("200");
    res->send_json(std::move(rt));
  } else {
    res->set_status("401");
    res->send_json("{}");
  }
}

hm::Task<> send_image(hm::HttpRequest *req, hm::HttpResponse *res) {
  auto name = req->get_param("name");
  if (name) {
    std::string link = "../uploads/" + std::string(name.value());
    res->send_file(link, false, false, false);
  } else {
    res->set_status("400");
    res->send_json("{}");
  }
  co_return;
}
