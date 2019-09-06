/*
 * Copyright 2019, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   wiltoncall_systemd.cpp
 * Author: alex
 *
 * Created on September 6, 2019, 5:41 PM
 */

#include <functional>
#include <memory>
#include <vector>

#include <sys/types.h>
#include <systemd/sd-daemon.h>

#include "staticlib/io.hpp"
#include "staticlib/json.hpp"
#include "staticlib/support.hpp"
#include "staticlib/utils.hpp"

#include "wilton/support/buffer.hpp"
#include "wilton/support/exception.hpp"
#include "wilton/support/logging.hpp"
#include "wilton/support/registrar.hpp"

namespace wilton {
namespace systemd {

namespace { //anonymous

const std::string logger = std::string("wilton.systemd");

} // namespace

support::buffer notify(sl::io::span<const char> data) {
    // json parse
    auto json = sl::json::load(data);
    auto rstate = std::ref(sl::utils::empty_string());
    for (const sl::json::field& fi : json.as_object()) {
        auto& name = fi.name();
        if ("state" == name) {
            rstate = fi.as_string_nonempty_or_throw(name);
        } else {
            throw support::exception(TRACEMSG("Unknown data field: [" + name + "]"));
        }
    }
    if (rstate.get().empty()) throw support::exception(TRACEMSG(
            "Required parameter 'state' not specified"));
    const std::string& state = rstate.get();
    auto pid = sl::utils::current_process_pid();

    // call
    wilton::support::log_debug(logger, std::string("Is due to notify systemd,") +
            " message: [" + state + "]");
    auto err = sd_pid_notify(static_cast<pid_t>(pid), 0, state.c_str());
    if (err <= 0) {
        throw support::exception(TRACEMSG("Error notifying systemd," +
                " message: [" + state + "],"
                " error code: [" + sl::support::to_string(err) + "]"));
    }
    wilton::support::log_debug(logger, "Notification sent successfully");
    return support::make_null_buffer();
}

} // namespace
}

extern "C" char* wilton_module_init() {
    try {
        wilton::support::register_wiltoncall("systemd_notify", wilton::systemd::notify);
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }

}
