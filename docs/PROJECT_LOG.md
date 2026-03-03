# Project Log (What We Did So Far)

This file records the full build path for this beginner project.

## 1) Initial hardware plan

- Started with ESP32 + 1x8 RGB board discussion.
- Mapped wiring to LED board `IN` side and provided first test sketch.

## 2) Platform switch to Arduino Nano

- Switched target from ESP32 to Arduino Nano (ATmega328P).
- Updated wiring and sketch for Nano data pin `D6`.

## 3) Project created in this workspace

- Added Arduino IDE sketch:
  - `nano_rgb_test/nano_rgb_test.ino`
- Added PlatformIO project scaffold:
  - `platformio.ini`
  - `src/main.cpp`

## 4) Upload/debug sequence completed

- `arduino-cli` binary in this environment failed with `Exec format error`.
- Used PlatformIO (`pio`) instead.
- Detected Nano serial device with CH340 USB-UART.
- Encountered common Nano upload sync errors (`stk500_getsync`).
- Tried new bootloader profile and old bootloader profile.
- Successful upload achieved using old bootloader profile (`nanoatmega328`).
- Serial port changed over time (`/dev/ttyUSB0` -> `/dev/ttyUSB1`) and was handled accordingly.

## 5) Firmware improvements

- Expanded from simple color cycle to multiple beginner-friendly patterns:
  - Solid colors
  - Color wipe
  - Theater chase
  - Rainbow cycle
  - Breathing white
- Added easy controls:
  - `BRIGHTNESS`
  - `SPEED_MS`

## 6) Documentation pass

- Rewrote `README.md` for absolute beginners.
- Added image reference (`easydoesit.png`) and mapping of labels 1/2/3.
- Added detailed parts list and troubleshooting section.
- Added GitHub publishing checklist in `docs/GITHUB_PUBLISH_CHECKLIST.md`.

## Current known-good upload command

```bash
pio run -e nanoatmega328 -t upload --upload-port /dev/ttyUSB1
```

> Note: your serial port may be `/dev/ttyUSB0` or another `ttyUSBx`.
