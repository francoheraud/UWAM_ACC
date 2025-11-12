CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude -Isrc -Imocks -Iunity -Icmock
LDFLAGS ?=
BUILD_DIR := build
TARGET := test_runner

SRC := \
    tests/test_fan_control.c \
    src/fan_control.c \
    mocks/mock_temperature_sensor.c \
    unity/unity.c \
    cmock/cmock.c

.PHONY: all clean run

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
