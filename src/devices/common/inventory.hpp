#pragma once

#include <string>
#include <vector>

#include "config/provider_config.hpp"
#include "devices/common/bread_compatibility.hpp"
#include "protocol.pb.h"

namespace anolis_provider_bread::inventory {

using Device = anolis::deviceprovider::v1::Device;
using CapabilitySet = anolis::deviceprovider::v1::CapabilitySet;

enum class InventorySource {
    ConfigSeeded,
    Manual,
    Discovered,
};

enum class CapabilitySource {
    Seeded,
    Queried,
    BaselineFallback,
};

struct CapabilityProfile {
    uint8_t schema = 0;
    uint8_t level = 0;
    uint32_t flags = 0;
    CapabilitySource source = CapabilitySource::Seeded;
};

struct ProbeRecord {
    int address = 0;
    uint8_t type_id = 0;
    ProbeStatus status = ProbeStatus::UnsupportedType;
    ModuleVersion version;
    CapabilityProfile capability_profile;
    std::string detail;
};

struct InventoryDevice {
    Device descriptor;
    CapabilitySet capabilities;
    DeviceType type = DeviceType::Rlht;
    int address = 0;
    InventorySource source = InventorySource::ConfigSeeded;
    bool expected = false;
    ModuleVersion version;
    CapabilityProfile capability_profile;
};

struct InventoryBuildResult {
    std::vector<InventoryDevice> supported_devices;
    std::vector<ProbeRecord> unsupported_probes;
    std::vector<std::string> missing_expected_ids;
};

std::vector<ProbeRecord> build_seed_probes(const ProviderConfig &config);
InventoryBuildResult build_inventory_from_probes(const ProviderConfig &config,
                                                 const std::vector<ProbeRecord> &probes,
                                                 InventorySource source);
std::vector<InventoryDevice> build_seed_inventory(const ProviderConfig &config);
CapabilityProfile make_baseline_capability_profile(DeviceType type);
CapabilityProfile make_seeded_capability_profile(DeviceType type);
std::string to_string(InventorySource source);
std::string to_string(CapabilitySource source);
const InventoryDevice *find_device(const std::vector<InventoryDevice> &devices, const std::string &device_id);
bool signal_exists(const InventoryDevice &device, const std::string &signal_id);
bool function_exists(const InventoryDevice &device, uint32_t function_id, const std::string &function_name);

} // namespace anolis_provider_bread::inventory