# Software I²C Master-Slave Implementation in C (AVR / ATmega32)

This repository contains a complete software-based I²C protocol implementation in C for AVR microcontrollers, specifically ATmega32. Unlike typical examples found online, this project includes **both master and slave logic**, built entirely from scratch using GPIO-level control — no hardware I²C modules involved.

## 🔧 Features

- Full I²C protocol emulation:
  - Start and stop condition handling
  - Bit-level transmission with precise timing
  - ACK/NACK signaling
  - Byte-level send/receive routines
- Master and Slave roles implemented in pure C
- GPIO-based bit-banging for SDA and SCL
- Proteus simulation setup for validation
- Designed for educational clarity and low-level protocol understanding


## 🧠 Why This Project Matters

Most online resources provide only partial I²C implementations — often just the master side or using hardware modules. This project fills a gap by offering a **complete software-driven solution**, ideal for learning, simulation, and low-level experimentation.

## 🛠️ Tools & Setup

- **Microcontroller**: ATmega32
- **Simulation**: Proteus (schematic included)
- **Language**: C (compiled with avr-gcc)
- **Clock Speed**: 8 MHz (recommended for timing accuracy)



