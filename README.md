<div align="center">

# 🚦 Four-Way Traffic Light System

### ATmega16 · Proteus · Real Hardware

[![AVR](https://img.shields.io/badge/MCU-ATmega16-blue?style=flat-square&logo=atmel)](https://www.microchip.com)
[![Clock](https://img.shields.io/badge/Clock-1MHz_RC-lightgrey?style=flat-square)](https://www.microchip.com)
[![Proteus](https://img.shields.io/badge/Simulation-Proteus-orange?style=flat-square)](https://www.labcenter.com)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](LICENSE)
[![Stars](https://img.shields.io/github/stars/Shahzadhpr/traffic-light?style=flat-square&color=yellow)](https://github.com/Shahzadhpr/Traffic-Light)

A complete **4-way traffic light controller** with countdown timers, built around the **ATmega16** microcontroller.  
The project includes a full Proteus simulation and is ready for real hardware implementation.

</div>

&nbsp;

## 📸 Simulation 
<!--& Hardware-->
https://github.com/user-attachments/assets/06f43b33-ec95-45bb-ada3-f20ce3b44d66

&nbsp;

## 📋 Project Overview

This system controls a 4-way intersection with North-South (N-S) and West-East (W-E) traffic lights. Each direction has a **7-segment countdown display** showing the remaining time. The controller cycles through 4 states automatically:

| State | N-S Light | W-E Light | Duration |
|:------|:---------:|:---------:|:--------:|
| `NS_GREEN` | 🟢 Green | 🔴 Red | 30 sec |
| `NS_YELLOW` | 🟡 Yellow | 🔴 Red | 5 sec |
| `WE_GREEN` | 🔴 Red | 🟢 Green | 30 sec |
| `WE_YELLOW` | 🔴 Red | 🟡 Yellow | 5 sec |

&nbsp;

## ⚙️ Technical Specifications

| Parameter | Value |
|:----------|:------|
| Microcontroller | ATmega16 |
| Clock | 1 MHz internal RC |
| Timer0 | Display multiplexing (prescaler /8) |
| Timer2 | 1-second timing in asynchronous mode |
|Crystal| 32.768 kHz |
| Display | 2× 7SEG-MPX2-CA (Common Anode) |
| LED Driver | ULN2803 |

&nbsp;

## 🗂️ Port Mapping

| Port | Function |
|:-----|:---------|
| `PORTA` (PA0–PA5) | N-W traffic LEDs |
| `PORTB` (PB0–PB5) | S-E traffic LEDs |
| `PORTD` (PD0–PD7) | 7-segment data (inverted, Common Anode) |
| `PORTC` (PD0–PD3) | 7-segment digit selects 

&nbsp;

## 🔌 Hardware Wiring Notes

### Recommended Component Values

| Component | Value |
|:----------|:-----:|
| LED current-limiting resistors | 220 Ω |
| BC337 base resistors | 1 kΩ |
| VCC bypass capacitor | 100 nF (near MCU) |
| Timer2 crystal | 32.768 kHz |

&nbsp;

## 🧠 How the Code Works

### State Machine

The firmware uses a 4-state finite state machine. Each state transition is triggered when the countdown timer reaches zero:

```
NS_GREEN (30s) → NS_YELLOW (5s) → WE_GREEN (30s) → WE_YELLOW (5s) → [repeat]
```

### Display Multiplexing (Timer0 ISR)

Timer0 overflow cycles through 4 digits in round-robin order:
- Digit 0 → N-S tens place
- Digit 1 → N-S ones place
- Digit 2 → W-E tens place
- Digit 3 → W-E ones place

Segments are driven **inverted** (`~seg[value]`) because the display is **Common Anode**.

### 1-Second Timing (Timer2 ISR)

Timer2 runs asynchronously from a **32.768 kHz crystal** for accurate 1-second ticks. It sets a `sec_flag` bit; the main loop reads and clears it to decrement the countdown.

### 7-Segment Encoding (Common Anode)

```c
flash unsigned char seg[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};
```

> Output to PORTD = `~seg[digit]` (inverted for Common Anode)

&nbsp;

## 🗃️ Repository Structure

```
├── 📁 Code
│   ├── 📁 Debug
│   │   ├── 📁 Exe
│   │   │   ├── 📄 traffic_light_project.a
│   │   │   ├── 📄 traffic_light_project.hex
│   │   │   ├── 📄 traffic_light_project.o
│   │   │   └── 📄 traffic_light_project.rom
│   │   ├── 📁 List
│   │   │   ├── 📄 traffic_light_project.asm
│   │   │   └── 📄 traffic_light_project.lst
│   │   ├── 📄 traffic_light_project.cof
│   │   ├── 📄 traffic_light_project.obj
│   │   ├── 📄 traffic_light_project.str1
│   │   ├── 📄 traffic_light_project.sym1
│   │   ├── 📄 traffic_light_project.sym2
│   │   └── 📄 traffic_light_project.sym3
│   ├── 📄 traffic_light_project.atsln
│   ├── 📄 traffic_light_project.c
│   ├── 📄 traffic_light_project.cbi
│   ├── 📄 traffic_light_project.cci
│   ├── 📄 traffic_light_project.cof
│   ├── 📄 traffic_light_project.cproj
│   ├── 📄 traffic_light_project.cwp
│   ├── 📄 traffic_light_project.c~
│   ├── 📄 traffic_light_project.fct
│   ├── 📄 traffic_light_project.prj
│   └── 📄 traffic_light_project.txt
├── 📁 Simulation
│   ├── 📁 Project Backups
│   │   ├── 📄 Traffic-Light [20260606, 22-37-48].pdsprj
│   │   ├── 📄 Traffic-Light [20260608, 17-35-47].pdsprj
│   │   ├── 📄 Traffic-Light [20260608, 18-34-27].pdsprj
│   │   ├── 📄 Traffic-Light [20260608, 18-43-10].pdsprj
│   │   ├── 📄 Traffic-Light [20260608, 18-46-18].pdsprj
│   │   ├── 📄 Traffic-Light [20260608, 18-49-40].pdsprj
│   │   ├── 📄 Traffic-Light [20260609, 15-21-44].pdsprj
│   │   ├── 📄 Traffic-Light [20260609, 15-27-50].pdsprj
│   │   ├── 📄 Traffic-Light [Autosaved].pdsprj
│   │   ├── 📄 Traffic-Light [Autosaved].pdsprj.DESKTOP-HQR2G01.Shahzad.Hpr.workspace
│   │   └── 📄 test [20260608, 17-27-01].pdsprj
│   ├── 📄 Traffic-Light.pdsprj
│   └── 📄 Traffic-Light.pdsprj.DESKTOP-HQR2G01.Shahzad.Hpr.workspace
├── 📄 LICENSE
└── 📝 README.md
```

&nbsp;

## 🚀 Getting Started

### Simulation (Proteus)

1. Open `Simulation/Traffic-Light.pdsprj` in **Proteus ISIS**
2. The `.hex` file is already linked — press **Play** to run

### Flashing to Hardware

1. Open `code/traffic_light_project.atsln` in **AVR Studio** (or compatible IDE)
2. Build the project → generates a fresh `.hex`
3. Flash `Debug/Exe/traffic_light_project.hex` to ATmega16 using:
   - USBasp / AVRISP programmer
   - Command: `avrdude -c usbasp -p m16 -U flash:w:traffic_light_project.hex`
4. Set fuse bits: **internal 1 MHz RC clock** (default fuses are fine)
5. For accurate timing, solder a **32.768 kHz crystal** to TOSC1/TOSC2 pins

### Tools Used

| Tool | Purpose | Download |
|:-----|:--------|:--------:|
| **AVR Studio** | Compilation | [⬇ Download](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) |
| **Proteus** | Simulation | [⬇ Download](https://www.labcenter.com/downloads/) |

&nbsp;

## 👥 Team

| Name | Role |
|:-----|:-----|
| **Shahzad Hassanpour** | Proteus Simulation & Circuit Design |
| **Nazanin Ghobadi** | Algorithm Design & Programming |
| **Mohammad Barati** | Hardware Implementation |

&nbsp;

## 📬 Contact

Feel free to reach out to any of the team members:

### Shahzad Hassanpour

[![LinkedIn](https://img.shields.io/badge/LinkedIn-Shahzad%20Hassanpour-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/hassanpourshahzad/)
[![GitHub](https://img.shields.io/badge/GitHub-Shahzadhpr-orange?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Shahzadhpr)

### Nazanin Ghobadi

[![GitHub](https://img.shields.io/badge/GitHub-nazaninghobadi-8B5CF6?style=for-the-badge&logo=github&logoColor=white)](https://github.com/nazaninghobadi)
[![Email](https://img.shields.io/badge/Email-Nazligh8383%40gmail.com-EA4335?style=for-the-badge&logo=gmail&logoColor=white)](mailto:Nazligh8383@gmail.com)

### Mohammad Barati

[![Telegram](https://img.shields.io/badge/Telegram-%40Mohamdtkd-26A5E4?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/Mohamdtkd)

&nbsp;

## ⭐ Support This Project

If you found this project helpful or learned something from it, please consider giving it a **⭐ star** on GitHub!

Stars help others discover the project and motivate us to keep improving it. Thank you! 🙏

&nbsp;

## 📄 License

This project is open-source and available under the MIT License.

&nbsp;

<div align="center">

*Made with ❤️ by Team **Jahat** — 2026*

</div>
