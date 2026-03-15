#pragma once

#include <cstdint>
#include <string>

#include "config/provider_config.hpp"

namespace anolis_provider_bread::inventory {

enum class ProbeStatus {
    Supported,
    UnsupportedType,
    VersionReadFailed,
    IncompatibleCrumbsVersion,
    IncompatibleModuleMajor,
    IncompatibleModuleMinor,
    TypeMismatch,
};

struct ModuleVersion {
    uint16_t crumbs_version = 0;
    uint8_t module_major = 0;
    uint8_t module_minor = 0;
    uint8_t module_patch = 0;
};

bool try_parse_bread_type(uint8_t type_id, DeviceType &out);
uint8_t bread_type_id(DeviceType type);
std::string bread_contract_name(DeviceType type);
std::string provider_type_id(DeviceType type);
ProbeStatus evaluate_version_compatibility(uint8_t type_id,
                                           const ModuleVersion &version,
                                           std::string *detail = nullptr);
std::string to_string(ProbeStatus status);

} // namespace anolis_provider_bread::inventory