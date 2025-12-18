# Ambient Light–Adaptive Motor Control (PSoC 4100S)

## Overview

This project is a **bare-metal embedded control application** developed on the **Infineon PSoC 4100S microcontroller**.  
The system adapts motor speed based on ambient light intensity sensed using an LDR, with motor direction controlled via a GPIO interrupt.

The primary goal of this project was to **design and implement custom peripheral drivers from scratch**, without using any HAL or vendor abstraction libraries, relying only on the **PSoC 4100S datasheets and Technical Reference Manuals (TRM)**.

---

## Key Features

- Bare-metal, **register-level programming**
- Custom peripheral drivers (no HAL, no libraries)
- ADC-based ambient light sensing (LDR)
- PWM-based motor speed control
- GPIO interrupt for motor direction control
- Speed ramping and clamping for stable control
- Clean firmware layering: **drivers → middleware → application**

---

## System Behavior

- Ambient light is sensed using an **LDR connected to the ADC**
- Raw ADC values are processed and scaled in firmware
- Scaled values are mapped to **PWM duty cycle** to control motor speed
- Motor speed adaptively changes with light intensity
- An onboard button triggers a **GPIO interrupt** to toggle motor direction

> For demonstration, LED/PWM behavior may be used as a proxy for motor speed when external hardware is unavailable.

---

## Firmware Architecture

The project follows a layered firmware design for clarity and scalability:

```

.
├── drivers/
│   ├── gpio/
│   ├── timer/
│   ├── pwm/
│   ├── adc/
│   └── clock/
│
├── middleware/
│   ├── motor/
│   ├── ldr/
│   └── button/
│
├── app/
│   └── main.c
│
└── README.md

````

### Drivers
Low-level, register-level implementations that directly control hardware peripherals.

### Middleware
Encapsulates control logic (motor control, sensor processing, button handling) independent of hardware details.

### Application
Implements system-level behavior by connecting sensor input to actuator output.

---

## Hardware Used

- Infineon **PSoC 4100S** development board
- LDR (Light Dependent Resistor)
- DC motor or LED (for PWM visualization)
- Onboard push button
- USB power/debug interface

---

## Build & Run

1. Clone the repository:

   ```bash
   git clone https://github.com/nidish40/PSOC_ambient_motor_control.git
   ```

2. Open the project in an ARM embedded toolchain or IDE
   (e.g., ModusToolbox, VS Code with ARM GCC, or a compatible IDE).

3. Build the project and flash the firmware to the **PSoC 4100S** development board.

4. Power the board and observe the system behavior.

---

## Future Extensions

* UART-based logging and diagnostics
* SWD / ITM-based debug output
* Improved ADC calibration using lookup tables (LUTs)
* Closed-loop motor speed feedback
* Configurable control parameters via a communication interface
* RTOS-based task scheduling and task separation
* Support for additional sensors or control modes

---
