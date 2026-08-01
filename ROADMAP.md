# Autonomous Rover Roadmap
**Josh & Franklin — Penn State Hazleton**  
**Timeline:** May 26 – August 16, 2026 (12 Weeks)  
**Budget:** $100–200 | **Platform:** Raspberry Pi 4 + ESP32

---

## Overview

A 3D-printed planetary rover capable of navigating rough terrain, built on a Raspberry Pi 4 brain with ESP32 motor control. The project progresses through three stages: manual control, assisted navigation, and full autonomy. Hard deadline is August 16, 2026.

**Josh** — Hardware lead: chassis CAD, 3D printing, wiring, assembly  
**Franklin** — Software lead: RPi setup, ESP32 firmware, vision, navigation

---

## Development Stages

### Stage 1 — Manual Control (Target: End of Week 7)
Rover drives under human control from a browser UI. Motor control is smooth, serial link is solid, watchdog stops everything if connection drops.

### Stage 2 — Assisted Navigation (Target: End of Week 9)
Camera is live in the UI. Rover detects obstacles and responds — slowing, stopping, or steering. Human still controls direction.

### Stage 3 — Full Autonomy (Target: Week 11, Stretch Goal)
Rover drives itself through a defined course. Closed loop of sensing, deciding, and acting without human input. Manual override always works immediately.

---

## Weekly Timeline

### Week 1 — May 26–Jun 1 | SETUP
**Phase:** Environment Setup + Research + Parts

| Josh | Franklin |
|------|----------|
| Inventory all EE components on hand | Flash RPi 4, set up SSH |
| Research chassis designs (bolbot #1583399 + others) | Set up Python venv, install pyserial, OpenCV, numpy |
| Decide drive type: differential vs 4-wheel steering | Install PlatformIO, flash blink to ESP32 |
| Start rough chassis sketch in Fusion 360 | Create GitHub repo: /firmware /navigation /ui /docs |
| Identify and order missing parts immediately | Research UART vs I2C vs SPI — read, don't implement |

**Milestone:** Both environments verified working. Parts order placed. Drive type decided. GitHub repo live.

> ⚠ Order parts immediately. Shipping takes 5–10 days. Don't wait for a perfect list.

---

### Week 2 — Jun 2–8 | MECH
**Phase:** CAD v1 + Motor Bench Testing (Isolated)

| Josh | Franklin |
|------|----------|
| Full chassis CAD draft: frame, motor positions, wheel mounts | Wire one motor + L298N to ESP32 on the bench (no RPi) |
| Print one small test piece to check scale and fit | Write ESP32 firmware to spin motor via serial commands |
| Fix CAD if test piece is wrong before printing full parts | Open Serial Monitor, manually type commands, watch motor |
| Rough placement of RPi, ESP32, battery on frame | Experiment with PWM values — understand speed control |

**Milestone:** CAD v1 draft complete. One motor spins reliably under ESP32 control.

> ℹ Franklin has NO integration goals this week. Bench-only motor work.

---

### Week 3 — Jun 9–15 | MECH
**Phase:** First Chassis Prints + RPi-ESP32 Comms

| Josh | Franklin |
|------|----------|
| Print primary chassis components: frame sections, motor brackets | Scale from 1 motor to all 4 motors wired to ESP32 |
| Test fit each part immediately as it comes off the printer | Test each motor independently by name (FL, FR, RL, RR) |
| Write down every fitment issue and dimension problem | Write Python script on RPi to open UART and send strings |
| Reprint critical parts that don't fit before moving forward | Get basic ping/pong working: RPi sends, ESP32 replies |
| Check: do motors seat in mounts? Do wheels clear frame? | Verify baud rate stability at multiple speeds |

**Milestone:** First chassis parts printed and test-fit. RPi and ESP32 exchanging messages over UART.

> ⚠ First prints WILL have problems. Holes too tight, walls too thin, motor mount flex. Iterate — don't stop.

---

### Week 4 — Jun 16–22 | ITER
**Phase:** Mechanical Iteration + Protocol Design

| Josh | Franklin |
|------|----------|
| Redesign every component that failed fitment in Week 3 | Design full command protocol on paper first |
| Reprint and re-test each revised part before moving on | Implement protocol: simple JSON or fixed strings like `M:FL:255:FWD\n` |
| Mock-mount motors with tape/clamps and check geometry | Add error handling: ESP32 should not crash on garbage input |
| Identify structural weak points: where does the frame flex? | Test reliability: 1000 commands in a loop, count acknowledgments |
| Begin thinking about suspension concept | Build motor abstraction: `forward()`, `backward()`, `stop()` |

**Milestone:** Revised chassis parts reprinted and fitting. Command protocol defined, documented, passing reliability test.

> ℹ Franklin: do not start the web UI yet. Solid protocol is more important than a joystick page.

---

### Week 5 — Jun 23–29 | ITER
**Phase:** First Assembly + Camera Experiments

| Josh | Franklin |
|------|----------|
| Assemble core chassis with motors actually bolted in | Connect camera to RPi, get live stream running |
| Wire all 4 motors to driver board (bench power only, no LiPo yet) | Stream over network (MJPEG works fine) |
| Check for binding or misalignment in motor mounting | Experiment with OpenCV: resize, grayscale, blur |
| Route wires through frame (this takes longer than expected) | Try simple color or edge detection on a real scene |
| Identify where battery, RPi, and ESP32 physically go | Benchmark: how many fps can RPi 4 process with CV ops? |

**Milestone:** Chassis physically assembled with all motors mounted. Camera streaming live frames at usable fps.

> ⚠ If assembly takes the full week, skip electronics mounts for now — use velcro/tape temporarily. Keep moving.

---

### Week 6 — Jun 30–Jul 6 | ITER
**Phase:** First Drive Attempt — Expect Problems

| Josh | Franklin |
|------|----------|
| Temporarily mount RPi and ESP32 onto chassis | Connect drive code to ESP32 for first time in real scenario |
| Power test: measure voltage at RPi and ESP32 with everything connected | Send forward/backward/turn commands, observe what rover does |
| First drive attempt on smooth floor | Debug: wrong motor directions, timing issues, serial dropout |
| Observe: do all 4 wheels spin correctly? Does it track straight? | Add watchdog to ESP32: stop all motors if no command in 500ms |
| Document every mechanical problem found | Write down every software bug — fix dangerous ones now, log rest |

**Milestone:** Rover moves under software control at least once. Full failure log documented for hardware and software.

> ⚠ Do not try to make it drive perfectly. The goal is to move it and find out what's broken.  
> ⚠ Motor noise can corrupt UART. If you see garbage serial data, add decoupling caps and use a separate BEC for logic.

---

### Week 7 — Jul 7–13 | STAGE 1
**Phase:** Fix Everything From Week 6

| Josh | Franklin |
|------|----------|
| Work through mechanical failure log one item at a time | Fix every critical software bug from Week 6 |
| Reprint parts that cracked, flexed, or didn't align | Add PWM ramping: no hard starts or stops |
| Fix motor direction issues if mechanically caused | Build simple browser-based joystick UI (basic, just functional) |
| Add strain relief to all wire connections | Test UI from a phone on same WiFi, verify response |
| Verify battery mounting is secure and balanced | Manual override: single button that stops all motors immediately |
| Torque-check all fasteners | Run rover continuously for 10 minutes, log failures |

**Milestone:** ✅ STAGE 1 STABLE — Rover drives cleanly in all directions. 10-minute continuous manual session with no failures. Watchdog verified.

> ⚠ If Stage 1 is not stable by end of this week, take another week. Building Stage 2 on a shaky Stage 1 guarantees worse problems later.

---

### Week 8 — Jul 14–20 | STAGE 1
**Phase:** Hardware Polish + Vision Pipeline Start

| Josh | Franklin |
|------|----------|
| Design and print real mounts for RPi, ESP32, battery | Mount camera on rover, get stream working while rover moves |
| Run all wires properly through frame, add cable management | Assess image quality on the move — adjust mount if too shaky |
| Make rover serviceable without full disassembly | Build OpenCV pipeline: capture, resize, preprocess (just plumbing) |
| Test on actual rough surface: carpet, gravel, outdoor pavement | Experiment with blob detection on actual rover footage |
| Find and fix new mechanical problems on rough terrain | Benchmark detection speed on RPi 4 — know your performance budget |

**Milestone:** Rover has proper electronics mounts. Camera streams at usable quality while moving. Detection pipeline runs on real footage.

> ℹ Do NOT connect vision to motor control yet. Pure pipeline experimentation only.

---

### Week 9 — Jul 21–27 | STAGE 2
**Phase:** Vision-to-Motor Integration + Terrain Testing

| Josh | Franklin |
|------|----------|
| Sustained terrain testing: hills, gravel, carpet, outdoor surfaces | Connect detection pipeline output to motor commands for first time |
| Look for: wheel slip, frame stress, motor temperature under load | Start simple: obstacle in center frame → stop. Get that working first. |
| Reprint or reinforce structural weak points found | Tune detection threshold — too sensitive = false stops, too loose = crashes |
| Document battery voltage curve under load | Add steering response: obstacle left → turn right, obstacle right → turn left |
| Help set up repeatable test course with clear obstacles | Log detection accuracy honestly: how often right vs wrong |

**Milestone:** Rover slows or stops based on camera input. Basic steering response to obstacles. Accuracy documented honestly.

> ⚠ Lighting changes everything. Test indoors, outdoors, and in dim light.  
> ℹ If vision is unreliable: consider HC-SR04 ultrasonic as a backup. Cheap, simple, always works.

---

### Week 10 — Jul 28–Aug 3 | BUFFER
**Phase:** Integration Buffer — Fix, Tune, Harden

| Josh | Franklin |
|------|----------|
| Fix any deferred mechanical issues — do it now | Stabilize all outstanding software bugs from Weeks 6–9 |
| Cable management final pass — every wire secured and labeled | Improve obstacle detection reliability from real-world data |
| Finalize and photograph wiring diagram | Mode switching (manual / assisted) instant and clean |
| Structural stress test: hardest terrain repeatedly | Write test procedures for each mode |
| Check every fastener, test every connector | Comment all code — if you can't explain it, you don't understand it yet |

**Milestone:** All Stage 1 + Stage 2 behaviors working reliably. No known critical bugs. Code and hardware documented.

> ℹ If you're ahead of schedule: don't start Stage 3. Use the time to make Stages 1 and 2 better.

---

### Week 11 — Aug 4–10 | STAGE 3
**Phase:** Autonomy Attempt + Reliability Runs

| Josh | Franklin |
|------|----------|
| Build defined test course: specific obstacles, repeatable layout | Build autonomous drive loop: scan → classify → decide → act → repeat |
| Stay nearby during autonomous runs in case of mechanical failure | Start conservative: slow speed, frequent stops, cautious turns |
| Fix mechanical issues that surface during autonomous operation | Run course 10 times, log success/failure and reason for each |
| Document what terrain handles well vs what it struggles with | Fix most common failure modes, retest |
| Take video of every test run — this is demo material | Manual override must interrupt autonomous mode instantly |

**Milestone:** Rover completes test course autonomously at least 5/10 attempts. Manual override verified. Failure modes documented.

> ⚠ Do not aim for 100% reliability this week. Aiming for perfection will leave you with no demo at all.

---

### Week 12 — Aug 11–16 | FINAL
**Phase:** Code Freeze + Demo Prep

| Josh | Franklin |
|------|----------|
| Final mechanical inspection: tighten everything | Code freeze — no new features, no refactors |
| Make rover presentable: sand edges, tidy wiring | Only fix bugs that would cause a demo failure |
| Prepare transport solution so it survives moving | Finalize README: someone else should be able to run it from docs |
| Rehearse demo: manual → assisted → autonomous | Commit wiring diagram, architecture diagram, test results to repo |
| Photograph rover for documentation | Rehearse demo on your own: know startup sequence cold |

**Milestone:** ✅ DONE — Demo-ready rover. All code committed. All docs complete.

> ⚠ Code freeze means code freeze. Breaking a working demo by fixing "one more thing" is the most common final-week disaster.

---

## Work Breakdown Structure

### Josh — Hardware
| Task | Dependency | Week |
|------|-----------|------|
| Chassis concept research, drive type decision | None | 1 |
| Fusion 360 CAD v1 — frame geometry | Drive type decided | 2 |
| Test print: single bracket to validate scale | CAD v1 | 2 |
| Print primary chassis components | CAD validated | 3 |
| Fitment testing on all printed parts | Parts printed | 3 |
| CAD v2: fix all fitment issues | Fitment results | 4 |
| Reprint revised components | CAD v2 | 4 |
| Suspension design (rigid, compliant, or damper) | Frame geometry locked | 4–5 |
| Full chassis assembly with motors mounted | All parts fit | 5 |
| Structural reinforcement from first drive findings | Week 6 test results | 7 |
| Electronics enclosures and mounts | Board positions decided | 8 |
| Cable management and strain relief | Mounts installed | 8–10 |
| Camera mount design and installation | Camera position decided | 8 |
| Terrain hardening — reinforce weak points | Terrain test results | 9–10 |

### Franklin — Software
| Task | Dependency | Week |
|------|-----------|------|
| RPi OS + Python environment setup | RPi in hand | 1 |
| ESP32 blink test, PlatformIO setup | ESP32 in hand | 1 |
| Single motor: PWM + direction via serial | ESP32 working | 2 |
| Scale to 4 motors: named control (FL/FR/RL/RR) | 1-motor working | 3 |
| RPi UART setup + ping/pong with ESP32 | Both boards wired | 3 |
| Command protocol design + implementation | Ping works | 4 |
| 1000-command reliability test | Protocol implemented | 4 |
| Motor abstraction: forward/backward/stop functions | 4-motor working | 4 |
| WiFi AP on RPi + Flask web server | RPi OS setup | 5–6 |
| Joystick UI, mobile-friendly | Web server up | 6–7 |
| PWM ramping — smooth start/stop | Motor abstraction | 7 |
| Watchdog: stop motors on comms loss >500ms | Protocol stable | 7 |
| Camera stream on RPi over network | Camera connected | 5 |
| OpenCV pipeline: capture, resize, preprocess | Camera streaming | 5–6 |
| RPi 4 fps benchmark with CV ops running | Pipeline exists | 5–6 |
| Obstacle detection implementation | Pipeline working | 8 |
| Vision-to-motor: first connection | Detection working | 9 |
| Obstacle avoidance tuning | V2M connected | 9–10 |
| Autonomous drive loop | Avoidance stable | 11 |
| 10-run autonomous reliability test | Drive loop | 11 |

---

## Risks & Mitigation

| Risk | Level | Area | Mitigation |
|------|-------|------|-----------|
| Chassis redesign kills 1-2 weeks | High | Mechanical | Expected — planned into Weeks 3-4. If on 3rd major redesign after Week 5, simplify the geometry. |
| 3S LiPo brownouts RPi under motor load | High | Electrical | Mandatory: dedicated 5V BEC for RPi. Never share rail with motors. Test explicitly in Week 6. |
| Serial comms corrupt under motor noise | High | Electrical | Keep signal wires away from motor wires. Add 100µF caps near ESP32 power. Test with motors at full speed. |
| Parts arrive late | High | Logistics | Order Week 1, immediately. Mock with LEDs and cardboard if needed. Franklin can test with simulated signals. |
| 3D prints fail or don't fit repeatedly | High | Mechanical | Print small test pieces first. PETG or PLA+ at 40%+ infill, 3+ perimeters. Iteration is the process. |
| RPi 4 too slow for vision + drive loop | Med | Software | Benchmark Week 5. Fix: smaller resolution (320x240), frame skipping, avoid ML models. Target 5–15fps min. |
| Vision unreliable in varied lighting | Med | Software | Test in actual demo environment. If unreliable, add HC-SR04 ultrasonic as backup. |
| Stage 3 doesn't reach demo quality | Med | Schedule | Stage 1 and 2 are the priority. Autonomy is the stretch goal, not the foundation. |
| Integration problems discovered late | Med | Integration | Week 6 first drive + Week 10 buffer exist to catch this. Do not skip the messy Week 6 attempt. |
| WiFi drops, rover doesn't stop | Med | Software | Watchdog on ESP32 is mandatory. Test drop scenario deliberately — pull WiFi, verify stop within 500ms. |

---

## Definition of Done

### Stage 1: Manual Control
- [ ] Drives forward, backward, left, right — every time
- [ ] Browser joystick UI works on a phone connected to rover WiFi
- [ ] Command-to-response latency feels responsive (<150ms)
- [ ] Watchdog stops all motors within 500ms of connection loss
- [ ] 10-minute continuous manual drive with no failures
- [ ] Power system stable: no brownouts, no overheating

### Stage 2: Assisted Navigation
- [ ] Camera stream visible in UI while rover is moving
- [ ] Rover stops or slows when obstacle is directly in its path
- [ ] Rover steers around detected obstacles in assisted mode
- [ ] Mode switching (manual → assisted) is instant and reliable
- [ ] 5-minute assisted mode session with no software crashes
- [ ] Detection accuracy documented — know what it handles and what it doesn't

### Stage 3: Full Autonomy *(Stretch Goal)*
- [ ] Rover completes a defined test course without human input
- [ ] Course completion rate ≥ 5/10 attempts, documented
- [ ] Manual override interrupts autonomous mode immediately, every time
- [ ] Rover handles a stuck or blocked state without human reset
- [ ] 10+ autonomous run attempts logged with outcomes

### Hardware & Build Quality
- [ ] No loose wires, unsecured connectors, or parts held with tape
- [ ] All electronics have proper mounts
- [ ] Battery life documented — you know how long a charge lasts
- [ ] Wiring diagram complete and committed to repo
- [ ] Rover survives transport without anything coming loose

### Documentation & Repository
- [ ] README explains hardware setup, wiring, and how to run each mode
- [ ] All code commented — someone else can read and understand it
- [ ] Wiring diagram in the repo
- [ ] Project doc updated with final decisions and lessons learned
- [ ] Demo video recorded showing what the rover can do

---

*Hard deadline: August 16, 2026. Stage 1 and 2 are the deliverables. Stage 3 is the goal.*
