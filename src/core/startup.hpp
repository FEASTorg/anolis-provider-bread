#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "config/provider_config.hpp"
#include "crumbs/session.hpp"
#include "devices/common/inventory.hpp"

namespace anolis_provider_bread::startup {

struct DiscoveryResult {
    std::vector<inventory::InventoryDevice> devices;
    std::vector<inventory::ProbeRecord> unsupported_probes;
    std::vector<std::string> missing_expected_ids;
    std::string inventory_mode;
};

// Probe a single I2C address: version query → compat check → caps query.
// Returns a ProbeRecord whose status indicates whether the device is usable.
// On any I/O failure the record carries a VersionReadFailed status; it never
// throws.
inventory::ProbeRecord probe_device(crumbs::Session &session, uint8_t address);

// Run the full discovery sequence (scan or manual) and build inventory.
// Throws std::runtime_error if the bus scan itself fails (scan mode only).
DiscoveryResult run_discovery(crumbs::Session &session, const ProviderConfig &config);

} // namespace anolis_provider_bread::startup
