SOURCE_FILES := $(wildcard *.c)

BUILD_DIR := build
TARGET_CPU := atmega162

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU)

OBJECT_FILES := $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

.PHONY: all
all: $(OBJECT_FILES)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(SUBDIRS) $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out

	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex