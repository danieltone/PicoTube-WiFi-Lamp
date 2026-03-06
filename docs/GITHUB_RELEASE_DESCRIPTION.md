# PicoTube-WiFi-Lamp v1.0.0

Pico 2W Wi-Fi lamp firmware with a built-in web UI for manual RGB control and animated presets.

## Highlights

- Wi-Fi AP mode (`picoled`)
- Browser control at `http://192.168.4.1`
- Manual color + brightness control
- Presets:
  - Rainbow Flow
  - Theater Chase
  - Breathing White
  - Classic Vacuum Tube Amber
  - Candy Alternating
  - Ocean Alternating
- Works with 2x2 WS2812 boards (4 LEDs by default)

## Quick start

1. Put Pico in BOOTSEL mode.
2. Drag `firmware.uf2` to the `RPI-RP2` drive.
3. Connect phone/computer to Wi-Fi `picoled`.
4. Open `http://192.168.4.1`.

## Wiring summary

- `VBUS (5V)` -> LED `VCC`
- `GND` -> LED `GND`
- `GP6` (pin 9) -> LED `DIN` (with inline 330-470Ω data resistor)

See `README.md` and `picodiagram.png` for full details.
