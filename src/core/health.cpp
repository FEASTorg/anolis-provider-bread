#include "core/health.hpp"

#include <chrono>
#include <string>

#include <google/protobuf/timestamp.pb.h>

namespace anolis_provider_bread::health {
namespace {

google::protobuf::Timestamp to_timestamp(const std::chrono::system_clock::time_point &time_point) {
    using namespace std::chrono;
    const auto seconds = time_point_cast<std::chrono::seconds>(time_point);
    const auto nanos = duration_cast<nanoseconds>(time_point - seconds);

    google::protobuf::Timestamp timestamp;
    timestamp.set_seconds(seconds.time_since_epoch().count());
    timestamp.set_nanos(static_cast<int>(nanos.count()));
    return timestamp;
}

} // namespace

ProviderHealth make_provider_health(const runtime::RuntimeState &state) {
    ProviderHealth provider;
    provider.set_state(state.ready ? ProviderHealth::STATE_OK : ProviderHealth::STATE_DEGRADED);
    provider.set_message(state.startup_message);
    provider.mutable_metrics()->insert({"device_count", std::to_string(state.devices.size())});
    provider.mutable_metrics()->insert({"ready", state.ready ? "true" : "false"});
    provider.mutable_metrics()->insert({"discovery_mode", to_string(state.config.discovery_mode)});
    provider.mutable_metrics()->insert({"inventory_mode", "config_seeded"});
    provider.mutable_metrics()->insert({"bus_path", state.config.bus_path});
    return provider;
}

std::vector<DeviceHealth> make_device_health(const runtime::RuntimeState &state) {
    std::vector<DeviceHealth> devices;
    devices.reserve(state.devices.size());

    for(const auto &device : state.devices) {
        DeviceHealth health;
        health.set_device_id(device.descriptor.device_id());
        health.set_state(state.ready ? DeviceHealth::STATE_OK : DeviceHealth::STATE_UNREACHABLE);
        health.set_message(state.ready
                               ? "Phase 1 config-seeded stub device"
                               : "Provider not ready");
        *health.mutable_last_seen() = to_timestamp(state.started_at);
        health.mutable_metrics()->insert({"address", device.descriptor.address()});
        health.mutable_metrics()->insert({"type_id", device.descriptor.type_id()});
        health.mutable_metrics()->insert({"inventory", "config_seeded"});
        devices.push_back(health);
    }

    return devices;
}

void populate_wait_ready(const runtime::RuntimeState &state, WaitReadyResponse &response) {
    response.mutable_diagnostics()->insert({"provider_version", ANOLIS_PROVIDER_BREAD_VERSION});
    response.mutable_diagnostics()->insert({"device_count", std::to_string(state.devices.size())});
    response.mutable_diagnostics()->insert({"ready", state.ready ? "true" : "false"});
    response.mutable_diagnostics()->insert({"inventory_mode", "config_seeded"});
    response.mutable_diagnostics()->insert({"discovery_mode", to_string(state.config.discovery_mode)});
    response.mutable_diagnostics()->insert({"bus_path", state.config.bus_path});
    response.mutable_diagnostics()->insert({"query_delay_us", std::to_string(state.config.query_delay_us)});
    response.mutable_diagnostics()->insert({"startup_message", state.startup_message});
}

} // namespace anolis_provider_bread::health
