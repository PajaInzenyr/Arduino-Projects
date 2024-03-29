# Pong 

One of the earliest arcade video games released in 1972. Pong resembles two players as rectangles on each side of the screen, with the goal of hitting a ball back and forth between them. 

This project is making use of limited hardware and software capabilities of an Arduino. Using only relatively simple math and basic compoments. 

## Preview

[![preview gif](/pong/preview.gif)](http://www.youtube.com/watch?v=TSi0fp79siA)

## Components

- Arduino Uno
- SH1106 SPI OLED Display 128x64
- Joystick (2x)
- Prototyping shield + breadboard (optional)

## Diagram

![diagram](/pong/diagram.svg)

## Usage

When the ball hits your wall, your oponent gains a point. 
The score resets once the score of a player exceeds 9.
The ball speeds up after every 5 successful bounces.

## Code

### Used libraries

- [u8g2](https://github.com/olikraus/u8g2)

---

> [!NOTE]
> - SPI OLED is not required but was chosen for better speed and smoother operation.
>   - You can use any other OLED supported by u8g2 library. Just change the display initialization and define pins.
> - Code deserves a revision and it's not perfect but totally functional in my test cases
