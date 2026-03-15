#include "config/provider_config.hpp"
#include "devices/common/inventory.hpp"

#include <gtest/gtest.h>

namespace anolis_provider_bread {
namespace {

TEST(StubInventoryTest, BuildsSeedInventoryForConfiguredDevices) {
    ProviderConfig config;
    config.provider_name = "bread-lab";
    config.bus_path = "/dev/i2c-1";
    config.devices = {
        DeviceSpec{"rlht0", DeviceType::Rlht, "Left Heater", 0x08},
        DeviceSpec{"dcmt0", DeviceType::Dcmt, "Conveyor Drive", 0x09},
    };

    const auto inventory_devices = inventory::build_seed_inventory(config);
    ASSERT_EQ(inventory_devices.size(), 2U);

    EXPECT_EQ(inventory_devices[0].descriptor.device_id(), "rlht0");
    EXPECT_EQ(inventory_devices[0].descriptor.type_id(), "bread.rlht");
    EXPECT_EQ(inventory_devices[0].descriptor.address(), "0x08");
    EXPECT_EQ(inventory_devices[1].descriptor.type_id(), "bread.dcmt");

    EXPECT_EQ(inventory_devices[0].capabilities.functions_size(), 6);
    EXPECT_EQ(inventory_devices[1].capabilities.functions_size(), 5);
    EXPECT_TRUE(inventory::signal_exists(inventory_devices[0], "t1_c"));
    EXPECT_TRUE(inventory::signal_exists(inventory_devices[1], "motor1_value"));
    EXPECT_TRUE(inventory::function_exists(inventory_devices[0], 1, ""));
    EXPECT_TRUE(inventory::function_exists(inventory_devices[1], 0, "set_mode"));
}

} // namespace
} // namespace anolis_provider_bread
