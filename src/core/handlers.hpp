#pragma once

#include "protocol.pb.h"

namespace anolis_provider_bread::handlers {

void handle_hello(const anolis::deviceprovider::v1::HelloRequest &request,
                  anolis::deviceprovider::v1::Response &response);
void handle_wait_ready(const anolis::deviceprovider::v1::WaitReadyRequest &request,
                       anolis::deviceprovider::v1::Response &response);
void handle_list_devices(const anolis::deviceprovider::v1::ListDevicesRequest &request,
                         anolis::deviceprovider::v1::Response &response);
void handle_describe_device(const anolis::deviceprovider::v1::DescribeDeviceRequest &request,
                            anolis::deviceprovider::v1::Response &response);
void handle_read_signals(const anolis::deviceprovider::v1::ReadSignalsRequest &request,
                         anolis::deviceprovider::v1::Response &response);
void handle_call(const anolis::deviceprovider::v1::CallRequest &request,
                 anolis::deviceprovider::v1::Response &response);
void handle_get_health(const anolis::deviceprovider::v1::GetHealthRequest &request,
                       anolis::deviceprovider::v1::Response &response);
void handle_unimplemented(anolis::deviceprovider::v1::Response &response,
                          const std::string &message);

} // namespace anolis_provider_bread::handlers
