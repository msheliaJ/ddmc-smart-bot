# CSE-Project-3 (Raspberry Pi Motor Control)

This repository contains weekly technical deliverables for **EECE 72030** focused on PWM-based motor control using a Raspberry Pi and the L293D H‑Bridge. Each week lives on its **own branch** to keep history clean and demos reproducible. The `main` branch stays as a stable landing page.

---

## Branch Plan (Weeks 1–5)

- **week1_setup**
  - Raspberry Pi setup notes (SSH, VNC, WiringPi), sample `hello_world` and `LED_blink`.
  - Slides: PWM basics (frequency, period, duty cycle, slew rate).
  - Deliverables: Assignment 1.

- **week2_pwm_led**
  - C program implementing PWM for LED control using `SIGALRM` @ 1 ms tick, keys: `a/s/d/w/q` and exit key.
  - Prints “Invalid input, refer to controls menu” for unsupported keys.
  - Deliverables: Assignment 2.

- **pwm_single_motor**  *(Assignment 3 name requirement)*
  - Oscilloscope screenshots validating 75% and 90% duty cycle.
  - Code updated for 100 ms period; direction control with safe LOW-before-change rule.
  - Deliverables: Assignment 3.

- **pwm_dual_motor**  *(Assignment 4 name requirement)*
  - Two-motor integration with FWD/BCK/LFT/RGT/STOP and two speed presets (slow & fast).
  - Clean pin mapping comments (WiringPi ↔ BCM GPIO ↔ physical pin).
  - Deliverables: Assignment 4.

- **phaseI_demo**  *(Assignment 5 name requirement)*
  - Final demo branch. Robust input parsing, safe shutdown key, README with install, wiring, BOM, compile/run steps.
  - Presentation (investor-style) and project report (PDF).
  - Deliverables: Assignment 5.

---

## Local Workflow



1) **Clone once**
```bash
git clone git@github.com:msheliaJ/CSE-Project-3.git
cd CSE-Project-3
git pull
```

2) **Create branches (one-time)**
```bash
git checkout -b week1_setup && git push -u origin week1_setup
git checkout main

git checkout -b week2_pwm_led && git push -u origin week2_pwm_led
git checkout main

git checkout -b pwm_single_motor && git push -u origin pwm_single_motor
git checkout main

git checkout -b pwm_dual_motor && git push -u origin pwm_dual_motor
git checkout main

git checkout -b phaseI_demo && git push -u origin phaseI_demo
git checkout main
```

3) **Add weekly deliverables to each branch**  

```bash
git checkout phaseI_demo

mkdir -p Week05_PhaseI_Demo

#   EECE72030_Assignment_05.pdf
#   presentation.pptx
#   report.pdf
git add Week05_PhaseI_Demo
git commit -m "Week 5: Phase I demo – code, report, slides, wiring & README"
git push
```


- `Week01_Setup` (Assignment 1)
- `Week02_PWM_LED` (Assignment 2)
- `Week03_PWM_SingleMotor` (Assignment 3)
- `Week04_PWM_DualMotor` (Assignment 4)



## Repository Structure (per branch)

```
<branch-root>/
├─ Week0X_<Label>/
│  ├─ code/                 # .c files 
│  ├─ docs/                 # wiring diagrams, oscilloscope images, notes
│  ├─ presentation/         # slides (PPTX/PDF)
│  ├─ report/               # PDF report (where applicable)
│  └─ README.md             # branch-specific instructions (compile/run)
└─ .gitignore
```

---

## Build & Run (typical)

```bash

gcc -Wall -O2 -lwiringPi -o motor motor.c
sudo ./motor
```

**Tip:** Always set both H-Bridge inputs LOW before changing direction. Add a dedicated **exit key** to stop motors and quit gracefully.

---

## Pin Mapping Template (copy into your code)

```c
// WiringPi ↔ BCM ↔ Physical pin
// e.g. IN1: wiringPi 0  -> BCM 17 -> pin 11
//      IN2: wiringPi 2  -> BCM 27 -> pin 13
//      ENA: wiringPi 1  -> BCM 18 -> pin 12  (PWM capable)
```

---

## Issues & PRs

Open an **Issue** for questions or bugs. Use **Pull Requests** to merge weekly work into a future “release” if needed. Keep commits small and messages clear.
