#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "config/provider_config.hpp"
#include "devices/common/stub_inventory.hpp"

namespace anolis_provider_bread::runtime {

struct RuntimeState {
    ProviderConfig config;
    std::vector<inventory::StubDevice> devices;
    bool ready = false;
    std::string startup_message;
    std::chrono::system_clock::time_point started_at;
};

void reset();
void initialize(const ProviderConfig &config);
RuntimeState snapshot();

} // namespace anolis_provider_bread::runtime
