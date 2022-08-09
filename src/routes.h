#pragma once

#include "auth.h"

#include "harmony/http2.h"

#include "data/login.h"

hm::Task<> login_user(hm::HttpRequest *req, hm::HttpResponse *res);
