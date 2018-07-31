#include <iostream>
#include <wiringPi.h>
#include <cstring>
#include <csignal>
#include <unistd.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GPIO Pins
int d0_pin = 11;
int d1_pin = 11;
int d2_pin = 11;
int d3_pin = 11;
int d4_pin = 11;
int d5_pin = 11;
int d6_pin = 11;
int d7_pin = 11;
int enable_pin = 11;
int cs_1_pin = 11;
int dc_pin = 11;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Screen init
U8G2_T6963_128X64_F_8080 screen_1(U8G2_R0, d0_pin, d1_pin, d2_pin, d3_pin, d4_pin, d5_pin, d6_pin, d7_pin, enable_pin, cs_1_pin, dc_pin);

static volatile bool complete = false;
static int forced_exit_attempts = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions
void exitHandler(int s) {
    if (forced_exit_attempts >= 2) {
        cout << "Forcing shutdown" << endl;
        u8g2_ClearDisplay(&u8g2);
        exit(1);
    }
    complete = true;
    forced_exit_attempts++;
}

void drawU8G2Logo(u8g2 *u8g2) {
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFontMode(u8g2, 1);

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb16_mf);
    u8g2_DrawStr(u8g2, 0, 22, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb19_mn);
    u8g2_DrawStr(u8g2, 14, 8, "8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb16_mf);
    u8g2_DrawStr(u8g2, 36, 22, "g");
    u8g2_DrawStr(u8g2, 48, 22, "\xb2");

    u8g2_DrawHLine(u8g2, 2, 25, 34);
    u8g2_DrawHLine(u8g2, 3, 26, 34);
    u8g2_DrawVLine(u8g2, 32, 22, 12);
    u8g2_DrawVLine(u8g2, 33, 23, 12);
    u8g2_SendBuffer(u8g2);
}

void drawBannerText(u8g2 *u8g2, uint32_t durationSecs) {

    uint8_t x = 255;
    uint32_t prevMillis = 0;

    //Set Font
    u8g2_SetFont(u8g2, u8g2_font_7x13B_mf);
    uint32_t elapsed = 0;

    while (elapsed != durationSecs) {
        if (complete)
            break;

        uint32_t curMillis = millis();

        //Count the number of elapsed seconds
        if ((curMillis - prevMillis) >= 1000) {
            prevMillis = curMillis;
            elapsed++;
        }

        if (x <= 0)
            x = 255;
        u8g2_ClearBuffer(u8g2);
        string text = "UG82 running on Raspberry Pi!";
        u8g2_DrawStr(u8g2, x, 32, text.c_str());
        u8g2_SendBuffer(u8g2);
        x-=5;
        delay(500);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup
/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup() {
  screen_1.begin();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main
int loop() {
    struct sigaction sigIntHandler{};
    sigIntHandler.sa_handler = exitHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, nullptr);

    //Initialize Wiring Pi
    wiringPiSetup();

    cout << "Running display demo...Press Ctrl+C to exit" << endl;

    //Run display loop
    while (!complete) {
        ///Draw U8G2 Logo
        drawU8G2Logo(&screen_1);
        delay(5000);

        //Run Banner Animation
        drawBannerText(&screen_1, 20);
    }

    cout << "Exiting demo" << endl;
    u8g2_ClearDisplay(&screen_1);
}
