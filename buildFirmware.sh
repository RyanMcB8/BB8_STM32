#!/usr/bin/env bash

# stop on error
set -e 

# Setting the parameters
BUILD_DIR="build/BB8"
TARGET="BB8"
ELF_PATH="$BUILD_DIR/$TARGET.elf"

# Building from the CMkaeLists.txt file.
echo "Building BB8 project..."
cd ./Firmware
cmake -B "$BUILD_DIR" -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake -DCMAKE_C_FLAGS="-Wno-unused-function -Wno-unused-variable"
cmake --build "$BUILD_DIR" --target "$TARGET" 

# Searching to see if the microcontroller is connected.
echo "Build complete!   "
echo "Checking for ST-Link..."

#   Linux build
if [[ "$(uname)" == "Linux" ]]; then
    if lsusb | grep -qi "STMicroelectronics ST-LINK"; then
        echo "ST-Link detected"

        # Flashing the ST device. 
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

#   Windows build
elif [[ "$OS" == "Windows_NT" ]]; then
    if powershell.exe -Command "Get-PnpDevice -FriendlyName '*ST-Link*'"; then
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
else 
    echo "Unknown or uncompatible operating system detected."
    exit 1
fi