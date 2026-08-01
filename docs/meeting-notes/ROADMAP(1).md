# Autonomous Rover — Technical Roadmap

**Team:** Josh (hardware/CAD) · Franklin (software/firmware) · Bruno (website)
**Location:** Penn State Hazleton
**Budget:** $500
**Platform:** Raspberry Pi 4 + ESP32
**Size:** 775mm (L) × 445mm (W) × 375mm (H)

---

## Components

| Category | Part |
|---|---|
| Compute | Raspberry Pi 4, ESP32, MicroSD, camera |
| Drive | 4x JGA-37 DC gear motors, BTS7960 motor driver, 4x wheels |
| Power | Custom 3S6P 18650 battery pack (self-built), 5V BEC (isolated from motor rail) |
| Sensing | Camera (vision, follow mode), VL53L1X ToF distance sensor (backup) |
| Chassis | 3D-printed frame, fasteners, cable management |

---

## Phase 1 — Manual Control
**Deadline: Aug 31, 2026** (Start: Jun 20, 2026)

Rover drives fully under human control via browser UI. Watchdog kills all motors if the connection drops.

| Weeks | Focus |
|---|---|
| 1–2 | Environment setup, order parts, CAD v1, bench-test one motor |
| 3–4 | Print/fit chassis, RPi ↔ ESP32 comms over UART, command protocol |
| 5–6 | Full assembly (all 4 motors + BTS7960), camera streaming, first drive attempt |
| 7–9 | Fix issues from first drive, joystick UI, watchdog (<500ms), PWM ramping, mounts/cable management |
| 10 | Buffer — stabilize, 10-minute continuous drive test, no failures |

**Definition of Done**
- [ ] Drives forward/backward/left/right reliably
- [ ] Browser joystick UI works from a phone on rover WiFi
- [ ] Watchdog stops all motors within 500ms of signal loss
- [ ] 10-minute continuous manual session, no failures
- [ ] Power system stable — no brownouts

---

## Phase 2 — Assisted Control + Follow Mode
**Deadline: Sep 11, 2026**

Half-manual/half-assisted control: human steers, rover assists with obstacle avoidance. Plus a follow mode where the rover tracks and follows a person for a set period.

| Days | Focus |
|---|---|
| 1–4 | VL53L1X + camera obstacle detection wired into the drive loop; slow/stop/steer-assist while human steers |
| 5–8 | Camera-based person tracking; rover follows at a set distance for a defined period |
| 9–11 | Tuning, manual override reliability, demo rehearsal |

**Definition of Done**
- [ ] Assisted mode: rover slows/stops/steers around obstacles while human retains control
- [ ] Follow mode: rover tracks and follows a person for a set duration
- [ ] Manual override interrupts either mode instantly, every time
- [ ] Mode switching is instant and reliable

---

## Risks

| Risk | Mitigation |
|---|---|
| Battery pack (3S6P self-built) inconsistency | Test voltage/current under load before relying on it for drive testing |
| BTS7960 + motor noise corrupting serial comms | Keep signal wires away from motor wires, isolate 5V BEC from motor rail |
| VL53L1X range/accuracy limits outdoors | Validate range in actual test environment; camera is primary, ToF is backup |
| Follow mode tracking loses the person | Define a clear recovery behavior (stop and wait) if tracking is lost |
| Compressed schedule (10.5 weeks to Phase 1, 11 days to Phase 2) | Phase 1 is the priority — don't start Phase 2 work early if Phase 1 isn't solid |

---

*Website (Bruno) develops in parallel and launches alongside the Phase 2 demo.*
