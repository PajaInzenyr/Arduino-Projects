#include <Arduino.h>
#include <U8g2lib.h>
#include <TM1637TinyDisplay.h>

#define BTN1 5
#define BTN2 6
#define BTN3 7

#define CLK 2
#define DIO 3

#define SDA A4
#define SCL A5

#define WIDTH 64
#define HEIGHT 128

U8G2_SH1106_128X64_NONAME_1_HW_I2C oled(U8G2_R3); // rotation is set to vertical layout
TM1637TinyDisplay display(CLK, DIO);

int8_t minutes = 2;
int8_t seconds = 0;

uint16_t sessionLenght = 0;
uint16_t tempTime = 0;

enum programStates { mainScreen, timerRunning, timerPaused, doneScreen };
enum programStates state = mainScreen;

uint8_t stopWatch[] = { 0, 0, 0, 0 };

uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
  SEG_C | SEG_E | SEG_G, // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, // E
  0,
  0,
  0,
};

void setup() {
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  oled.begin();
  oled.setContrast(160);

  display.begin();
  display.setBrightness(2);
}

void clockWork() {
  if (seconds >= 60) {
    seconds -= 60;
    minutes++;
  }

  if (seconds < 0) {
    seconds += 60;
    minutes--;
  }
}

void btnAction() {
  if (digitalRead(BTN1) == HIGH && minutes < 90 && state == mainScreen) {
    minutes += 5;
    delay(100);
  }

  if (digitalRead(BTN2) == HIGH && minutes > 15 && state == mainScreen) {
    minutes -= 5;
    delay(100);
  }

  if (digitalRead(BTN3) == HIGH) {
    if (state == mainScreen)
      sessionLenght = minutes * 60 + seconds;

    if (state == mainScreen || state == timerPaused) {
      state = timerRunning;
    } else if (state == timerRunning) {
      state = timerPaused;
    }

    if (state == doneScreen) {
      state = mainScreen;
      minutes = 40;
      seconds = 0;
    }
      
    oled.clear();
    tempTime = millis()/1000;
    delay(100);
  }
}

void timer() {
  if (state == timerRunning && millis()/1000 > tempTime) {
    seconds--;
    tempTime = millis()/1000;
  }

  if (seconds == 0 && minutes == 0) {
    state = doneScreen;
  }
}

void oledDraw() {
  oled.firstPage();
  do {
    if (state == mainScreen) {
      oled.setFont(u8g2_font_profont12_tf);
      oled.drawStr(8, 17, "Set time");
      oled.setFont(u8g2_font_profont17_mn);
      oled.drawStr(2, 114, "+");
      oled.drawStr(2, 130, "-");
      oled.setFont(u8g2_font_profont11_tf);
      oled.drawStr(34, 128, "Start");
    }

    if (state == timerRunning) {
      oled.setFont(u8g2_font_profont12_tf);
      oled.drawStr(3, 17, "STUDY TIME");
      oled.drawFrame(0, 32, WIDTH, 10);
      uint8_t boxWidth = map(seconds + minutes * 60, 0, sessionLenght, 0, 60);
      oled.drawBox(2, 34, boxWidth, 6);
    }

    if (state == timerPaused) {
      oled.setFont(u8g2_font_profont12_tf);
      oled.drawStr(16, 17, "Timer");
      oled.drawStr(14, 29, "paused");
    }

    if (state == doneScreen) {
      oled.setFont(u8g2_font_profont17_tf);
      oled.drawStr(12, 17, "DONE!");
      oled.setFont(u8g2_font_profont12_tf);
      oled.drawStr(21, 36, "Take");
      oled.drawStr(12, 48, "a break");
    }
  } while (oled.nextPage());
}

void tm1637() {
  stopWatch[0] = display.encodeDigit(floor(minutes/10));
  stopWatch[1] = display.encodeDigit(minutes%10) + 128;
  stopWatch[2] = display.encodeDigit(floor(seconds/10));
  stopWatch[3] = display.encodeDigit(seconds%10);

  if (state == mainScreen || state == timerRunning || state == timerPaused)
    display.setSegments(stopWatch);

  if (state == doneScreen) { // scrolling done text
    display.setSegments(done);

    uint8_t temp = done[6];
    for (int i = 6; i > 0; i--) {
      done[i] = done[i - 1];
    }
    done[0] = temp;

    display.setSegments(done);
    delay(400);
  } 
    
}

void blinkLED() {
  if (state == doneScreen) {
    if (millis()%1000 >= 500)
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
  
  tm1637();
  timer();
  clockWork();
  btnAction();
  oledDraw();
  blinkLED();

  delay(100);
}
