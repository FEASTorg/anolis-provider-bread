#include "devices/common/stub_inventory.hpp"

namespace anolis_provider_bread::inventory {
namespace {

using ArgSpec = anolis::deviceprovider::v1::ArgSpec;
using CapabilitySet = anolis::deviceprovider::v1::CapabilitySet;
using FunctionPolicy = anolis::deviceprovider::v1::FunctionPolicy;
using FunctionSpec = anolis::deviceprovider::v1::FunctionSpec;
using SignalSpec = anolis::deviceprovider::v1::SignalSpec;

constexpr auto VT_BOOL = anolis::deviceprovider::v1::VALUE_TYPE_BOOL;
constexpr auto VT_INT64 = anolis::deviceprovider::v1::VALUE_TYPE_INT64;
constexpr auto VT_UINT64 = anolis::deviceprovider::v1::VALUE_TYPE_UINT64;
constexpr auto VT_DOUBLE = anolis::deviceprovider::v1::VALUE_TYPE_DOUBLE;
constexpr auto VT_STRING = anolis::deviceprovider::v1::VALUE_TYPE_STRING;

constexpr auto CAT_CONFIG = anolis::deviceprovider::v1::FunctionPolicy_Category_CATEGORY_CONFIG;
constexpr auto CAT_ACTUATE = anolis::deviceprovider::v1::FunctionPolicy_Category_CATEGORY_ACTUATE;

void add_arg(FunctionSpec &function,
             const std::string &name,
             anolis::deviceprovider::v1::ValueType type,
             const std::string &description,
             bool required,
             const std::string &unit = "") {
    ArgSpec *arg = function.add_args();
    arg->set_name(name);
    arg->set_type(type);
    arg->set_description(description);
    arg->set_required(required);
    if(!unit.empty()) {
        arg->set_unit(unit);
    }
}

FunctionSpec *add_function(CapabilitySet &caps,
                           uint32_t function_id,
                           const std::string &name,
                           const std::string &description,
                           anolis::deviceprovider::v1::FunctionPolicy_Category category) {
    FunctionSpec *function = caps.add_functions();
    function->set_function_id(function_id);
    function->set_name(name);
    function->set_description(description);
    function->mutable_policy()->set_category(category);
    function->mutable_policy()->set_is_idempotent(false);
    return function;
}

void add_signal(CapabilitySet &caps,
                const std::string &signal_id,
                const std::string &description,
                anolis::deviceprovider::v1::ValueType type,
                const std::string &unit = "") {
    SignalSpec *signal = caps.add_signals();
    signal->set_signal_id(signal_id);
    signal->set_name(signal_id);
    signal->set_description(description);
    signal->set_value_type(type);
    signal->set_poll_hint_hz(1.0);
    signal->set_stale_after_ms(5000);
    if(!unit.empty()) {
        signal->set_unit(unit);
    }
}

CapabilitySet build_rlht_capabilities() {
    CapabilitySet caps;

    auto *set_mode = add_function(caps, 1, "set_mode", "Set RLHT control mode.", CAT_CONFIG);
    add_arg(*set_mode, "mode", VT_STRING, "One of closed_loop or open_loop.", true);

    auto *set_setpoints = add_function(caps, 2, "set_setpoints", "Set RLHT channel temperature setpoints.", CAT_CONFIG);
    add_arg(*set_setpoints, "setpoint1_c", VT_DOUBLE, "Channel 1 target temperature.", true, "C");
    add_arg(*set_setpoints, "setpoint2_c", VT_DOUBLE, "Channel 2 target temperature.", true, "C");

    auto *set_pid = add_function(caps, 3, "set_pid_x10", "Set RLHT PID gains encoded x10.", CAT_CONFIG);
    add_arg(*set_pid, "kp1_x10", VT_UINT64, "Channel 1 proportional gain x10.", true);
    add_arg(*set_pid, "ki1_x10", VT_UINT64, "Channel 1 integral gain x10.", true);
    add_arg(*set_pid, "kd1_x10", VT_UINT64, "Channel 1 derivative gain x10.", true);
    add_arg(*set_pid, "kp2_x10", VT_UINT64, "Channel 2 proportional gain x10.", true);
    add_arg(*set_pid, "ki2_x10", VT_UINT64, "Channel 2 integral gain x10.", true);
    add_arg(*set_pid, "kd2_x10", VT_UINT64, "Channel 2 derivative gain x10.", true);

    auto *set_periods = add_function(caps, 4, "set_periods_ms", "Set RLHT relay periods.", CAT_CONFIG);
    add_arg(*set_periods, "period1_ms", VT_UINT64, "Relay 1 period.", true, "ms");
    add_arg(*set_periods, "period2_ms", VT_UINT64, "Relay 2 period.", true, "ms");

    auto *set_tc_select = add_function(caps, 5, "set_tc_select", "Select RLHT thermocouple inputs.", CAT_CONFIG);
    add_arg(*set_tc_select, "tc1_index", VT_UINT64, "Channel 1 thermocouple index.", true);
    add_arg(*set_tc_select, "tc2_index", VT_UINT64, "Channel 2 thermocouple index.", true);

    auto *set_open_duty = add_function(caps, 6, "set_open_duty_pct", "Set RLHT open-loop duty percentages.", CAT_ACTUATE);
    add_arg(*set_open_duty, "duty1_pct", VT_UINT64, "Channel 1 duty cycle percentage.", true, "%");
    add_arg(*set_open_duty, "duty2_pct", VT_UINT64, "Channel 2 duty cycle percentage.", true, "%");

    add_signal(caps, "mode", "Current RLHT control mode.", VT_STRING);
    add_signal(caps, "t1_c", "Channel 1 measured temperature.", VT_DOUBLE, "C");
    add_signal(caps, "t2_c", "Channel 2 measured temperature.", VT_DOUBLE, "C");
    add_signal(caps, "setpoint1_c", "Channel 1 target temperature.", VT_DOUBLE, "C");
    add_signal(caps, "setpoint2_c", "Channel 2 target temperature.", VT_DOUBLE, "C");
    add_signal(caps, "period1_ms", "Relay 1 control period.", VT_UINT64, "ms");
    add_signal(caps, "period2_ms", "Relay 2 control period.", VT_UINT64, "ms");
    add_signal(caps, "relay1_on", "Relay 1 state derived from RLHT flags.", VT_BOOL);
    add_signal(caps, "relay2_on", "Relay 2 state derived from RLHT flags.", VT_BOOL);
    add_signal(caps, "estop", "Emergency stop state derived from RLHT flags.", VT_BOOL);

    return caps;
}

CapabilitySet build_dcmt_capabilities() {
    CapabilitySet caps;

    auto *set_open_loop = add_function(caps, 1, "set_open_loop", "Set DCMT open-loop PWM values.", CAT_ACTUATE);
    add_arg(*set_open_loop, "motor1_pwm", VT_INT64, "Motor 1 PWM command.", true);
    add_arg(*set_open_loop, "motor2_pwm", VT_INT64, "Motor 2 PWM command.", true);

    auto *set_brake = add_function(caps, 2, "set_brake", "Set DCMT brake outputs.", CAT_ACTUATE);
    add_arg(*set_brake, "motor1_brake", VT_BOOL, "Motor 1 brake state.", true);
    add_arg(*set_brake, "motor2_brake", VT_BOOL, "Motor 2 brake state.", true);

    auto *set_mode = add_function(caps, 3, "set_mode", "Set DCMT control mode.", CAT_CONFIG);
    add_arg(*set_mode, "mode", VT_STRING, "One of open_loop, closed_position, or closed_speed.", true);

    auto *set_setpoint = add_function(caps, 4, "set_setpoint", "Set DCMT control targets.", CAT_CONFIG);
    add_arg(*set_setpoint, "motor1_target", VT_INT64, "Motor 1 target value.", true);
    add_arg(*set_setpoint, "motor2_target", VT_INT64, "Motor 2 target value.", true);

    auto *set_pid = add_function(caps, 5, "set_pid_x10", "Set DCMT PID gains encoded x10.", CAT_CONFIG);
    add_arg(*set_pid, "kp1_x10", VT_UINT64, "Motor 1 proportional gain x10.", true);
    add_arg(*set_pid, "ki1_x10", VT_UINT64, "Motor 1 integral gain x10.", true);
    add_arg(*set_pid, "kd1_x10", VT_UINT64, "Motor 1 derivative gain x10.", true);
    add_arg(*set_pid, "kp2_x10", VT_UINT64, "Motor 2 proportional gain x10.", true);
    add_arg(*set_pid, "ki2_x10", VT_UINT64, "Motor 2 integral gain x10.", true);
    add_arg(*set_pid, "kd2_x10", VT_UINT64, "Motor 2 derivative gain x10.", true);

    add_signal(caps, "mode", "Current DCMT control mode.", VT_STRING);
    add_signal(caps, "motor1_target", "Motor 1 target value.", VT_INT64);
    add_signal(caps, "motor2_target", "Motor 2 target value.", VT_INT64);
    add_signal(caps, "motor1_value", "Motor 1 measured value.", VT_INT64);
    add_signal(caps, "motor2_value", "Motor 2 measured value.", VT_INT64);
    add_signal(caps, "motor1_brake", "Motor 1 brake state.", VT_BOOL);
    add_signal(caps, "motor2_brake", "Motor 2 brake state.", VT_BOOL);
    add_signal(caps, "estop", "Emergency stop state.", VT_BOOL);

    return caps;
}

StubDevice build_device(const ProviderConfig &config, const DeviceSpec &spec) {
    StubDevice device;
    device.descriptor.set_device_id(spec.id);
    device.descriptor.set_provider_name(config.provider_name);
    device.descriptor.set_label(spec.label);
    device.descriptor.set_address(format_i2c_address(spec.address));
    device.descriptor.mutable_tags()->insert({"family", "bread"});
    device.descriptor.mutable_tags()->insert({"inventory", "config_seeded"});

    switch(spec.type) {
    case DeviceType::Rlht:
        device.descriptor.set_type_id("bread.rlht");
        device.descriptor.set_type_version("1");
        device.descriptor.mutable_tags()->insert({"contract", "rlht"});
        device.capabilities = build_rlht_capabilities();
        break;
    case DeviceType::Dcmt:
        device.descriptor.set_type_id("bread.dcmt");
        device.descriptor.set_type_version("1");
        device.descriptor.mutable_tags()->insert({"contract", "dcmt"});
        device.capabilities = build_dcmt_capabilities();
        break;
    }

    return device;
}

} // namespace

std::vector<StubDevice> build_seed_inventory(const ProviderConfig &config) {
    std::vector<StubDevice> devices;
    devices.reserve(config.devices.size());
    for(const DeviceSpec &spec : config.devices) {
        devices.push_back(build_device(config, spec));
    }
    return devices;
}

const StubDevice *find_device(const std::vector<StubDevice> &devices, const std::string &device_id) {
    for(const StubDevice &device : devices) {
        if(device.descriptor.device_id() == device_id) {
            return &device;
        }
    }
    return nullptr;
}

bool signal_exists(const StubDevice &device, const std::string &signal_id) {
    for(const auto &signal : device.capabilities.signals()) {
        if(signal.signal_id() == signal_id) {
            return true;
        }
    }
    return false;
}

bool function_exists(const StubDevice &device, uint32_t function_id, const std::string &function_name) {
    for(const auto &function : device.capabilities.functions()) {
        const bool id_match = function_id != 0 && function.function_id() == function_id;
        const bool name_match = !function_name.empty() && function.name() == function_name;
        if(id_match || name_match) {
            return true;
        }
    }
    return false;
}

} // namespace anolis_provider_bread::inventory
