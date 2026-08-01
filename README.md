# Autonomous Rover

A 3D-printed rover built from scratch — Raspberry Pi 4 brain, ESP32 motor control — designed to go from full manual control to a semi-autonomous "follow me" mode.

## Team
- **Josh** — Hardware: chassis design/printing, wiring, assembly, battery pack
- **Franklin** — Software: RPi setup, ESP32 firmware, vision, navigation
- **Bruno** — Website: project site and build documentation

## Project Goals
1. **Manual control** — drive the rover remotely from a browser, with a safety watchdog that stops it if the connection drops.
2. **Assisted control + follow mode** — the rover helps avoid obstacles while a human still steers, and can track and follow a person on its own for a set period of time.

## Specs
| | |
|---|---|
| Size | 775mm (L) × 445mm (W) × 375mm (H) |
| Compute | Raspberry Pi 4 + ESP32 |
| Drive | 4x JGA-37 DC gear motors, BTS7960 driver |
| Power | Custom 3S6P 18650 battery pack |
| Sensing | Camera + VL53L1X time-of-flight sensor |
| Budget | $500 |

## Status
Built at Penn State Hazleton. See `ROADMAP.md` for the full technical timeline and milestones.

## Repo Layout
```
/firmware    - ESP32 motor control code
/navigation  - vision, obstacle detection, follow-mode logic
/ui          - browser control interface
/docs        - wiring diagrams, build notes
```
