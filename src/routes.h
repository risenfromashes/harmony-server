#pragma once

#include "auth.h"

#include "harmony/http2.h"

#include "data/login.h"

hm::Task<> login_user(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> get_user_info(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> get_feed(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> register_user(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> update_user_info(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> get_groups(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> get_posts(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> get_group_messages(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> add_post(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> add_group_message(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> add_event(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> create_event_source(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> add_image(hm::HttpRequest *req, hm::HttpResponse *res);

hm::Task<> send_image(hm::HttpRequest *req, hm::HttpResponse *res);
