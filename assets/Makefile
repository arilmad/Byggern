SOURCE_FILES := bitmap.c fonts.c
BINS := $(SOURCE_FILES:%.c=%)

BUILD_DIR := ../build
TARGET_CPU := armega162

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU)

OBJECT_FILES := $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)


#.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

.PHONY: all
all: $(BINS)

$(BUILD_DIR):
	@echo "Make dir"
	mkdir $(BUILD_DIR)
	
$(BUILD_DIR)/%: %.o
	@echo "Checking.."
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $<