# Core

`src/core/` holds provider lifecycle, ADPP transport, and runtime orchestration code.

Phase 1 now includes:

- framed stdio transport for ADPP
- config-backed runtime state
- request handlers for Hello, WaitReady, ListDevices, DescribeDevice, and GetHealth
- unimplemented placeholders for ReadSignals and Call until hardware work begins
