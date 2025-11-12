# ACC Firmware Unit Testing

This directory contains host-side unit test assets for the accumulator cooling controller firmware.

* `cmock_example/` – a self-contained Unity/CMock harness that demonstrates how to isolate HAL dependencies using generated mocks.
* `test_script.c` – legacy hardware smoke-test harness produced by STM32CubeMX.
* `unit_tests.c` – placeholder for on-target unit tests (kept for backwards compatibility).

See the `cmock_example` folder for instructions on how to build and extend the mock-based tests on a development machine.
