SOURCE_FILES := uart.c

ifeq ($(N),1)
	BUILD_DIR := ../../build_node1
else
	BUILD_DIR := ../../build_node2
endif

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU)

OBJECT_FILES := $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(OBJECT_FILES)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
