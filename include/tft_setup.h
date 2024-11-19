#pragma once

#include <Arduino.h>

#define USER_SETUP_LOADED 1    
#define ST7789_DRIVER 1
#define TFT_WIDTH 240
#define TFT_HEIGHT 240
#define TFT_CS SS
#define TFT_DC 33 // DC Pin
#define TFT_RST 26 // RST Pin
#define TFT_MOSI MOSI
#define TFT_SCLK SCK
#define TFT_MISO MISO
#define TFT_BL -1
#define TOUCH_CS -1
#define LOAD_GLCD 0
#define LOAD_FONT2 0
#define LOAD_FONT4 0
#define LOAD_FONT6 0
#define LOAD_FONT7 0
#define LOAD_FONT8 0
#define LOAD_GFXFF 0
#define SMOOTH_FONT 0
#define SPI_FREQUENCY 27000000