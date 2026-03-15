# Devices

`src/devices/` contains the BREAD-facing adapter and inventory code.

Current structure:

- `common/` for shared inventory, compatibility, and contract-driven helpers
- `rlht/` reserved for RLHT-specific adapter code
- `dcmt/` reserved for DCMT-specific adapter code

Phase 3 adds the provider-owned inventory model on top of `bread-crumbs-contracts` constants and compatibility helpers. Full RLHT/DCMT adapters still belong to Phase 4.