#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "config/provider_config.hpp"
#include "devices/common/stub_inventory.hpp"

// Forward declaration — callers that need the full Session type must include
// crumbs/session.hpp themselves.
namespace anolis_provider_bread::crumbs {
class Session;
} // namespace anolis_provider_bread::crumbs

namespace anolis_provider_bread::runtime {

struct RuntimeState {
    ProviderConfig config;
    std::vector<inventory::StubDevice> devices;
    bool ready = false;
    std::string startup_message;
    std::chrono::system_clock::time_point started_at;
    // Discovery diagnostics
    std::string inventory_mode;
    int unsupported_probe_count = 0;
    std::vector<std::string> missing_expected_ids;
};

void reset();
void initialize(const ProviderConfig &config);
RuntimeState snapshot();

// Returns the live CRUMBS session, or nullptr when running without hardware.
// The pointer is valid for the lifetime of the provider process after
// initialize() returns successfully.
crumbs::Session *session();

} // namespace anolis_provider_bread::runtime
