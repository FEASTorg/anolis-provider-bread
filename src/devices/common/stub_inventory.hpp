#pragma once

#include <string>
#include <vector>

#include "config/provider_config.hpp"
#include "protocol.pb.h"

namespace anolis_provider_bread::inventory {

using Device = anolis::deviceprovider::v1::Device;
using CapabilitySet = anolis::deviceprovider::v1::CapabilitySet;

struct StubDevice {
    Device descriptor;
    CapabilitySet capabilities;
};

std::vector<StubDevice> build_seed_inventory(const ProviderConfig &config);
const StubDevice *find_device(const std::vector<StubDevice> &devices, const std::string &device_id);
bool signal_exists(const StubDevice &device, const std::string &signal_id);
bool function_exists(const StubDevice &device, uint32_t function_id, const std::string &function_name);

} // namespace anolis_provider_bread::inventory
