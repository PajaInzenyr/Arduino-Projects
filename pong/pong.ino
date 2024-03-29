#include <Arduino.h>
#include <U8g2lib.h>

#define CS 8
#define DC 9
#define RES 10
#define MOSI 11
#define CLK 13
#define inputX_1 A4
#define inputX_2 A5
#define WIDTH 128
#define HEIGHT 64

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled(U8G2_R0, CS, DC, RES);

int zeroX_1, zeroX_2;

int16_t xpos_ball = WIDTH/2; 
int16_t ypos_ball = HEIGHT/2; 
int16_t xvel_ball = 1;
int16_t yvel_ball = 1;

const int8_t xpos_box1 = 0;
int8_t ypos_box1 = 24;
const int8_t xpos_box2 = 125;
int8_t ypos_box2 = 24;

const int8_t box_width = 3;
const int8_t box_height = 16;

int8_t yvel_box1 = 0;
int8_t yvel_box2 = 0;

uint8_t score1 = 0;
uint8_t score2 = 0;

uint8_t bounceCounter = 0;

void setup() {
  zeroX_1 = analogRead(inputX_1);
  zeroX_2 = analogRead(inputX_2);

  randomSeed(analogRead(0));

  oled.begin();
  oled.setContrast(180);

  oled.firstPage(); // INTRO SCREEN
  do {
    oled.setFont(u8g2_font_helvB12_tf);
    oled.drawStr(16, 36, "GET READY");
  } while(oled.nextPage());

  delay(3000);
}

void joystickAction(int16_t* actX_1, int16_t* actX_2) {
  *actX_1 = analogRead(inputX_1) - zeroX_1;
  *actX_2 = analogRead(inputX_2) - zeroX_2;

  if (*actX_1 > 0) 
    *actX_1 = map(*actX_1, 0, 1023 - zeroX_1, 0, 100);
  else
    *actX_1 = map(*actX_1, 0, -zeroX_1, 0, -100);
  
  if (*actX_2 > 0)
    *actX_2 = map(*actX_2, 0, 1023-zeroX_2, 0, 100);
  else
    *actX_2 = map(*actX_2, 0, -zeroX_2, 0, -100);
}

void ballMovement(int16_t* xpos_ball, int16_t* ypos_ball, int16_t* xvel_ball, int16_t* yvel_ball) {
  *xpos_ball += *xvel_ball;
  *ypos_ball += *yvel_ball;
}

void wallBounce(int16_t* xpos_ball, int16_t* ypos_ball, int16_t* xvel_ball, int16_t* yvel_ball) {
  if (*xpos_ball >= WIDTH - 3 || *xpos_ball <= 2) // left and right WALL bounce - WIN/LOSS 
  {
    if (*xpos_ball <= 2)
    {
      score2++;
      *xvel_ball /= *xvel_ball;
    }
      
    if (*xpos_ball >= WIDTH - 3)
    {
      score1++;
      *xvel_ball /= -*xvel_ball;
    }

    printScore();

    if (score1 == 9 || score2 == 9)
    {
      score1 = 0;
      score2 = 0;
    }

    *xpos_ball = random(48, 80);
    *ypos_ball = random(16, 48);
  }
  
  if (*ypos_ball >= 61 || *ypos_ball <= 2) // UP and DOWN bounce
    *yvel_ball = -*yvel_ball;
}

void printScore() {
  oled.firstPage(); // print score 
  do {
    oled.setFont(u8g2_font_VCR_OSD_mn);
    oled.setCursor(50, 32);
    oled.print(score1);
    oled.drawStr(63, 32, ":");
    oled.setCursor(74, 32);
    oled.print(score2);
    oled.setFont(u8g2_font_helvB12_tf);
    if (score1 == 9)
      oled.drawStr(0, 48, "<-- Player WON");
    if (score2 == 9)
      oled.drawStr(0, 48, "Player WON -->");
  } while(oled.nextPage());

  delay(2000);
}

void boxBounce(int16_t* xvel_ball, int16_t* yvel_ball, uint8_t* bounceCounter) {
  // LEFT box 
  if (xpos_ball <= xpos_box1 + box_width + 2 && (ypos_ball >= ypos_box1 - 2 && ypos_ball <= ypos_box1 + box_height + 2))
  {
    *xvel_ball = -*xvel_ball;
    ++*bounceCounter;
    if (*bounceCounter >= 5)
    {
      *bounceCounter = 0;
      ++*xvel_ball;
    }
  }

  // RIGHT box
  if (xpos_ball >= xpos_box2 - 3 && (ypos_ball >= ypos_box2 - 2 && ypos_ball <= ypos_box2 + box_height + 2))
  {
    *xvel_ball = -*xvel_ball;
    ++*bounceCounter;
    if (*bounceCounter >= 5)
    {
      *bounceCounter = 0;
      --*xvel_ball;
    }
  }

}

void boxMovement(int8_t* ypos_box1, int8_t* ypos_box2, int8_t actX_1, int8_t actX_2) {
  // box speed based on input
  
  if (actX_1 >= 85)
    yvel_box1 = 3;
  if (actX_1 >= 50 && actX_1 < 85)
    yvel_box1 = 2;
  if (actX_1 >= 20 && actX_1 < 50)
    yvel_box1 = 1;
  if (actX_1 < 20 && actX_1 > -20)
    yvel_box1 = 0;
  if (actX_1 < -20 && actX_1 >= -50)
    yvel_box1 = -1;
  if (actX_1 < -50 && actX_1 >= -85)
    yvel_box1 = -2;
  if (actX_1 <= -85)
    yvel_box1 = -3;

  if (actX_2 >= 85)
    yvel_box2 = 3;
  if (actX_2 >= 50 && actX_2 < 85)
    yvel_box2 = 2;
  if (actX_2 >= 20 && actX_2 < 50)
    yvel_box2 = 1;
  if (actX_2 < 20 && actX_2 > -20)
    yvel_box2 = 0;
  if (actX_2 < -20 && actX_2 >= -50)
    yvel_box2 = -1;
  if (actX_2 < -50 && actX_2 >= -85)
    yvel_box2 = -2;
  if (actX_2 <= -85)
    yvel_box2 = -3;

  // box movement
  *ypos_box1 += yvel_box1;
  *ypos_box2 += yvel_box2;

  if (*ypos_box1 < 0)
    *ypos_box1 = 0;
  if (*ypos_box1 + box_height > 63)
    *ypos_box1 = 63 - box_height;
  
  if (*ypos_box2 < 0)
    *ypos_box2 = 0;
  if (*ypos_box2 + box_height > 63)
    *ypos_box2 = 63 - box_height;
}

void drawGame() {
  oled.firstPage();
  do {
    oled.drawDisc(xpos_ball, ypos_ball, 2);
    oled.drawBox(xpos_box1, ypos_box1, box_width, box_height);
    oled.drawBox(xpos_box2, ypos_box2, box_width, box_height);
  } while (oled.nextPage());
}

void loop() {
  int16_t actX_1, actX_2;
  joystickAction(&actX_1, &actX_2);

  boxMovement(&ypos_box1, &ypos_box2, actX_1, actX_2);
  ballMovement(&xpos_ball, &ypos_ball, &xvel_ball, &yvel_ball);
  wallBounce(&xpos_ball, &ypos_ball, &xvel_ball, &yvel_ball);
  boxBounce(&xvel_ball, &yvel_ball, &bounceCounter);
  drawGame();

  delay(16);
}
