SOURCE_DIR := ./src
ASSETS_DIR := ./assets
LIB_SPI := ./lib/spi
LIB_CAN := ./lib/can

LIBRARIES := $(LIB_SPI) $(LIB_CAN)

# Set this flag to "yes" (no quotes) to use ISP (SPI); otherwise JTAG is used
PROGRAM_WITH_JTAG :=no

# Feel free to ignore anything below this line
PROGRAMMER := atmelice
ifeq ($(PROGRAM_WITH_JTAG),yes)
	PROGRAMMER := atmelice_isp
endif

BUILD_DIR := build
TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU)

OBJECT_FILES := $(wildcard $(BUILD_DIR)/*.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY = all flash fuse clean erase

$(SOURCE_DIR) $(LIBRARIES) $(ASSETS_DIR):
	$(MAKE) --directory=$@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

all: $(SOURCE_DIR) $(LIBRARIES) $(ASSETS_DIR)

flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

fuse:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U efuse:w:0xff:m
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U hfuse:w:0x19:m
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U lfuse:w:0xfd:m

clean:
	rm -rf $(BUILD_DIR)

erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e