SOURCE_DIR := ./src
ASSETS_DIR := ./assets
LIB_SPI := ./lib/spi

LIBRARIES := $(LIB_SPI)

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

.PHONY = all flash fuse clean erase

$(SOURCE_DIR) $(LIBRARIES) $(ASSETS_DIR):
	$(MAKE) --directory=$@

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