#include "core/runtime_state.hpp"

#include <mutex>
#include <utility>

namespace anolis_provider_bread::runtime {
namespace {

std::mutex g_mutex;
RuntimeState g_state;

} // namespace

void reset() {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_state = RuntimeState{};
}

void initialize(const ProviderConfig &config) {
    RuntimeState state;
    state.config = config;
    state.devices = inventory::build_seed_inventory(config);
    state.ready = true;
    state.startup_message = "Phase 1 provider shell ready with config-seeded inventory";
    state.started_at = std::chrono::system_clock::now();

    std::lock_guard<std::mutex> lock(g_mutex);
    g_state = std::move(state);
}

RuntimeState snapshot() {
    std::lock_guard<std::mutex> lock(g_mutex);
    return g_state;
}

} // namespace anolis_provider_bread::runtime
