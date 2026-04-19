#!/usr/bin/env bash

# stop on error
set -e 

# Setting the parameters
BUILD_DIR="Firmware/build/BB8"
TARGET="BB8"
ELF_PATH="$BUILD_DIR/$TARGET.elf"

# Building from the CMkaeLists.txt file.
echo "Building BB8 project..."
cmake --build "$BUILD_DIR" --target "$TARGET"

# Searching to see if the microcontroller is connected.
echo "Build complete!   "
echo "Checking for ST-Link..."

if lsusb | grep -qi "STMicroelectronics ST-LINK"; then
    echo "ST-Link detected"

    # ---- Step 3: Flash ----
    if [ -f "$ELF_PATH" ]; then
        echo "Flashing firmware..."

        openocd \
          -f interface/stlink.cfg \
          -f target/stm32wbx.cfg \
          -c "program $ELF_PATH verify reset exit"

        echo "Flash complete"
    else
        echo "ELF file not found at: $ELF_PATH"
        exit 1
    fi

else
    echo "No ST-Link detected."
    echo "Please connect an ST-Link and try again."
    exit 1
fi