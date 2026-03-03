# Detailed Parts List

This list is tailored for the exact beginner build in this project.

## Core electronics

| Qty | Item | Suggested spec | Typical role |
|---:|---|---|---|
| 1 | Arduino Nano (ATmega328P) | 5V/16MHz, CH340 USB or FTDI USB | Runs LED control code |
| 1 | 1x8 RGB LED board | WS2812B / NeoPixel compatible, with `IN` and `OUT` | Addressable LEDs |
| 1 | USB cable | Data-capable cable matching your Nano connector | Power + programming |
| 3-4 | Jumper wires | Reliable dupont/jumper leads | Connect `5V`, `GND`, `D6` |

## Protection/stability parts

| Qty | Item | Suggested value | Placement |
|---:|---|---|---|
| 1 | Resistor | `330Ω` to `470Ω`, 1/4W | In series on `D6` -> LED `IN` |
| 1 | Electrolytic capacitor | `1000µF`, at least `6.3V` | Across LED `VCC` and `GND` |

## Useful support tools

| Qty | Item | Why it helps |
|---:|---|---|
| 1 | Breadboard | Easier beginner wiring and changes |
| 1 | Multimeter | Check power/ground and debug wiring |
| 1 | Small parts kit | Spare wires/resistors for mistakes |

## Software/tools used

- Arduino IDE (beginner upload flow)
- PlatformIO (`pio`) for terminal-based build/upload
- Adafruit NeoPixel library

## Power/current note (beginner-safe)

An 8-LED board is small enough for USB-powered demos at low brightness. This project intentionally keeps brightness low (`BRIGHTNESS = 40`) to reduce current draw and heat.
