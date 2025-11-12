# CMock Unit Test Template

This folder demonstrates how to unit test firmware modules on a host PC using Unity and a lightweight CMock runtime. The example tests the hysteresis helper used by the fan controller.

## Directory Layout

```
cmock_example/
├── cmock/            # Minimal runtime that coordinates mock verification
├── include/          # Public headers for modules under test
├── mocks/            # Example mock generated in the style of CMock
├── src/              # Firmware module under test
├── tests/            # Unity test cases and runner
└── unity/            # Unity test framework (trimmed for firmware workflows)
```

## Building the Example

The project ships with a portable `Makefile`. Invoke the following commands from the `cmock_example` directory:

```bash
make       # Builds the test runner
make run   # Builds and executes the unit tests
make clean # Removes the build artefacts
```

The build produces a standalone executable (`test_runner`) that runs on the host machine. Adjust `CC` and `CFLAGS` inside the `Makefile` if you require a different toolchain.

## Adapting the Template

1. Copy the files under `src/` and `include/` to model the module you want to test.
2. Run the official CMock generator (bundled with Ceedling) to produce the mocks for your dependencies, or duplicate `mocks/mock_temperature_sensor.*` and adjust the expected functions.
3. Extend the tests under `tests/` and update the `Makefile` so the new sources are compiled into the runner.

The simplified runtime in `cmock/` registers each mock's verification and destruction hooks so expectations are automatically checked during `tearDown()`. This mirrors the control flow used by the full CMock toolchain and makes migrating to the upstream generator straightforward.
