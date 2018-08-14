#include <iostream>
#include <wiringPi.h>
#include <cstring>
#include <csignal>
#include <unistd.h>

#include "U8g2lib.h"
#include <U8x8lib.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GPIO Pins
int d0_pin = 11;        //data pins: 240X128 pin 11-18
int d1_pin = 13;
int d2_pin = 15;
int d3_pin = 29;
int d4_pin = 31;
int d5_pin = 33;
int d6_pin = 35;
int d7_pin = 37;
int wr_pin = 36;     //write: 240X128 pin 5 (active low)
int cs_1_pin = 38;      //chip select: 240X128 pin 7 (active low)
int dc_pin = 40;        //Command/data: 240S128 pin 8

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Screen init
U8G2_T6963_240X128_F_8080 screen_1(U8G2_R0, d0_pin, d1_pin, d2_pin, d3_pin, d4_pin, d5_pin, d6_pin, d7_pin, wr_pin, cs_1_pin, dc_pin);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Drawing Functions
void drawPage1(u8g2 *u8g2) {
    u8g2_ClearBuffer(u8g2);
    u8g2.SetFont(u8g2_font_ncenB24_mf);
    u8g2.drawStr(0, 40, "FORD");
    u8g2_SendBuffer(u8g2);
}

void drawPage2(u8g2 *u8g2) {
    u8g2_ClearBuffer(u8g2);
    u8g2.SetFont(u8g2_font_ncenB24_mf);
    u8g2.drawStr(0, 40, "BRONCO");
    u8g2_SendBuffer(u8g2);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup
/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup() {
  screen_1.begin();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main
int main() {
    //Initialize Wiring Pi
    wiringPiSetup();

    //Run display loop
    while (!complete) {
        //Run Banner Animation
        drawPage1(&screen_1);
        delay(5000);
        drawPage2(&screen_1);
        delay(5000);
    }

    u8g2_ClearDisplay(&screen_1);
}
