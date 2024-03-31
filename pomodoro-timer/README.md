# Pomodoro Timer 

Pomodoro technique si a time management method which breaks work into time intervals separated by short breaks. In this case you have the freedom to choose work duration from 15 to 90 minutes.

## Preview

| Setting the timer | End of the timer |
| ----------------- | ---------------- |
| [![preview gif](/pomodoro-timer/preview1.gif)](https://www.youtube.com/watch?v=Tud4qeWpTGw) | [![preview gif](/pomodoro-timer/preview2.gif)](https://www.youtube.com/watch?v=Tud4qeWpTGw) |

## Components

- Arduino Nano
- TM1637 4-digit 7-segment display
- SH1106 I2C OLED Display 128x64
- Buttons (3x)
- 10k Rezistors (3x)

## Diagram

![diagram](/pomodoro-timer/diagram.svg)

## Usage

When powering the Arduino the default focus time is set to 40 minutes. You can use the '+' and '-' button to adjust the time, the minumum is 15 minutes and maximum 90 minutes. After pressing 'Start' button the timer starts with a progress bar on the OLED screen. If you want to pause the timer, press the start button again. If the timer runs out, the built-in LED on Arduino board starts blinking and digital display starts showing text 'done'.  Press the start button again to repeat the cycle.

## Code

### Used libraries

- [TM1637Display](https://github.com/avishorp/TM1637)
- [u8g2](https://github.com/olikraus/u8g2)

