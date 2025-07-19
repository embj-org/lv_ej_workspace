# LVGL EJ Workspace

EJ Workspace for [LVGL](https://github.com/lvgl/lvgl) performance benchmarking and testing. This workspace uses the [EJ framework](https://embj-org.github.io/ej/) to run automated performance benchmarks on real embedded hardware as part of LVGL's continuous integration pipeline.

## Overview

This workspace contains everything needed to build, deploy, and run LVGL performance tests across multiple embedded platforms. It leverages EJ's distributed testing architecture to automatically measure LVGL performance on real hardware and report results back to the CI system.

### Key Components

- **`ejlv_builder/`** - Custom EJ builder application that handles building and running LVGL benchmarks for different target platforms
- **`config.toml`** - EJ configuration defining available boards and their build/run configurations
- **Board-specific directories** - Contains platform-specific build configurations, source code, and scripts
- **`lvgl/`** - LVGL library submodule used for benchmarking
- **`service/`** - Systemd service configuration for running the EJ builder as a daemon

## Usage

### Prerequisites

- Rust toolchain (latest stable)
- Target hardware properly configured and accessible
- SSH access to target boards (where applicable)
- Platform-specific build tools (CMake, ESP-IDF, etc.)

### Setup

1. Clone this repository with submodules:

```bash
git clone --recursive <repository-url>
```

2. Install `ejb` and `ejlv_builder`:

```bash
cargo install ejb
cargo install --path ejlv_builder
```

3. Setup EJB

Modify the paths to match your local setup. You will have to modify the paths inside `config.toml` to point to your local LVGL repository and the specific board results files.

4. Configure the EJ builder by editing `service/ejb.env`:

```bash
EJB_ID=your-builder-id
EJB_TOKEN=your-auth-token
EJB_CONFIG_PATH=/path/to/config.toml
EJB_SERVER=https://ci.lvgl.io
```

Modify the `WorkingDirectory` and `ExecStart` paths in `service/ejb.service` to point to your local paths.

We recommend using the `logs` directory as the working directory for EJB.

4. Install and start the EJ builder service:

```bash
sudo cp service/ejb.service /etc/systemd/system/
sudo cp service/ejb.env /etc/ejb/
sudo systemctl enable ejb
sudo systemctl start ejb
```

You can check the status and logs of the service with:

```bash
sudo systemctl status ejb
journalctl -u ejb -f
```

### Adding New Platforms

To add support for a new board or configuration:

1. Add the board configuration to `config.toml`
2. Create the platform-specific build directory and files
3. Implement build and run logic in `ejlv_builder/src/main.rs`
4. Test the configuration locally before deploying

## Contributing

This workspace is part of the LVGL ecosystem. For questions or contributions:

- **LVGL Issues**: [LVGL GitHub Repository](https://github.com/lvgl/lvgl)
- **EJ Framework**: [EJ Documentation](https://embj-org.github.io/ej/)

## License

This project follows the same licensing as LVGL. See individual components for specific license information.
