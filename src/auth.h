#pragma once

#include <harmony/http2.h>
#include <string_view>

#include "data/login.h"

hm::AwaitableTask<> authenticate_user(hm::HttpResponse *response,
                                      std::string_view name,
                                      std::string_view password);

hm::AwaitableTask<bool> is_authenticated(hm::HttpResponse *response,
                                         std::string_view sid,
                                         std::string_view user_id);
