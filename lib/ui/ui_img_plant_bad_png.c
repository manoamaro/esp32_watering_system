// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 9.1.0
// Project name: watering_system

#include "ui.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

// IMAGE DATA: assets/plant_bad.png
const LV_ATTRIBUTE_MEM_ALIGN uint8_t ui_img_plant_bad_png_data[] = {
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xD6,0xC5,0xB2,0x9C,0x78,0xD6,0xDA,0xDE,0xFA,0xE6,0xDA,0xE6,0xDE,0xFF,0x50,0x94,0x86,0x41,0xA6,0x41,0xAF,0x8B,0xBE,0xFF,0xB8,0xDE,0x98,0xD6,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xD6,0xC5,0xB2,0xA4,0x78,0xD6,0xBA,0xDE,0x5B,0xF7,0xFF,0xFF,0x95,0xBD,0x28,0x5A,0x37,0xCE,0xDE,0xFF,0x49,0x62,0x10,0x8C,0xFF,0xFF,0xD9,0xE6,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x38,0xCE,0xF3,0xA4,0x7D,0xEF,0xDE,0xFF,0x16,0xCE,0x78,0xD6,0x6D,0x83,0x28,0x5A,0x5C,0xF7,0xD6,0xBD,0xC6,0x41,
    0xA2,0x28,0xCF,0x83,0x1B,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x89,0x62,0xC6,0x49,0x0C,0x6B,0x76,0xAD,0xBA,0xDE,0x28,0x62,0x85,0x41,0x4D,0x73,0x59,0xCE,0x83,0x51,0x62,0x8A,0x65,0x82,0x80,0x50,0xCB,0x6A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x42,0x49,0x85,0xB3,0x04,0x9B,0xE1,0x48,0x52,0x94,0x79,0xDE,0x20,0x18,0x91,0x9C,0x92,0x94,0xC2,0xAA,0x07,0xBC,0x23,0x41,0xE7,0xD3,0x02,0x49,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x45,0x72,0x28,0xC4,0x86,0x82,
    0xA4,0x92,0xA0,0x48,0xF8,0xBD,0x6D,0x83,0x4C,0x73,0x75,0xB5,0xC0,0x50,0xC3,0x59,0xE2,0x30,0xE1,0x50,0x84,0x51,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x26,0x62,0x86,0xBB,0x01,0x18,0x83,0x51,0xE3,0xAA,0x8B,0x62,0x8E,0x7B,0x44,0x39,0x5B,0xEF,0x87,0x39,0xE0,0x50,0xC6,0xAA,0x00,0x30,0xD3,0x94,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0x8B,0x80,0x48,0xE6,0xA2,0xE4,0x71,0xE0,0x40,0xB2,0xA4,0x8E,0x7B,0xC2,0x28,0x1A,0xE7,0x7C,0xEF,0x84,0x49,0x00,0x10,0x10,0x84,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1C,0xEF,0x89,0x62,0xC2,0x79,0x61,0x69,0x00,0x00,0x96,0xBD,0x10,0x94,0xE7,0x51,0x4C,0x7B,0x3B,0xEF,0x51,0x9C,0x21,0x10,0x39,0xCE,0xF8,0xCD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1B,0xEF,0x1B,0xEF,0x30,0x8C,0xEF,0x83,0x75,0xB5,0x89,0x6A,0xE7,0x49,0x03,0x31,0x0F,0x9C,0x7D,0xF7,0x54,0xC5,0x2C,0x83,0x16,0xD6,0x53,0xBD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x99,0xDE,0xB9,0xE6,0xFA,0xEE,0x99,0xDE,0xDE,0xFF,0x79,0xD6,0xA2,0x28,0x65,0x39,0xDA,0xE6,0x7C,0xF7,0xB9,0xE6,
    0xFA,0xEE,0xBD,0xFF,0x7C,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x99,0xD6,0x5C,0xEF,0x9D,0xF7,0x9C,0xEF,0xBD,0xF7,0xFF,0xFF,0x6E,0x83,0x4D,0x73,0xFE,0xF7,0x9C,0xF7,0xDA,0xDE,0xDD,0xF7,0xBD,0xF7,0xBA,0xDE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3B,0xEF,0xFB,0xE6,0x72,0x8C,0x71,0x8C,0x71,0x94,0xF3,0x9C,0xA7,0x39,0x8A,0x5A,0x96,0xB5,0x30,0x8C,0xAF,0x7B,0x30,0x84,0x76,0xAD,0x5D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB,0xE6,0x89,0x62,0x41,0x59,
    0xE3,0x71,0xC3,0x71,0xC3,0x69,0x65,0x7A,0x44,0x72,0x82,0x61,0xA3,0x69,0xE3,0x71,0xC2,0x71,0x60,0x38,0x72,0x9C,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x14,0xA5,0xC0,0x48,0x29,0xFD,0x4B,0xF5,0x0A,0xED,0x0A,0xED,0x2B,0xF5,0x2B,0xF5,0x0A,0xED,0x0A,0xED,0x4B,0xF5,0x2A,0xFD,0x40,0x81,0x86,0x49,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDA,0xDE,0x07,0x5A,0xA2,0x59,0x24,0x6A,0xE4,0x61,0xE4,0x61,0xE4,0x61,0xE4,0x69,0xE4,0x69,0xC4,0x61,0x24,0x6A,0x04,0x72,0x80,0x38,0xD3,0xA4,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7C,0xF7,0x79,0xCE,0x04,0x31,0x62,0x69,0xC4,0x71,0x83,0x61,0x63,0x61,0x83,0x61,0x83,0x61,0xA3,0x69,0xA3,0x71,0x00,0x20,0x2D,0x6B,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBA,0xDE,0x7D,0xEF,0xCA,0x72,0x25,0xB3,0x49,0xD4,0xE8,0xCB,0xE8,0xCB,0xE8,0xCB,0xE8,0xCB,0x49,0xD4,0x86,0xCB,0xA0,0x50,0x72,0x94,0x3C,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBA,0xDE,0x9D,0xF7,0xEB,0x6A,0x63,0x82,0x49,0xD4,0xE8,0xC3,0xE7,0xBB,0xE8,0xC3,0xE8,0xC3,0x28,0xCC,0xE5,0xAA,
    0xC1,0x40,0xD3,0x9C,0x9A,0xDE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xB9,0xDE,0xDE,0xF7,0x8E,0x7B,0xE2,0x71,0x68,0xD4,0xC7,0xBB,0xC7,0xBB,0xC7,0xBB,0xC7,0xBB,0xE8,0xC3,0x22,0x92,0x43,0x49,0x59,0xC6,0x1C,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x99,0xD6,0x9D,0xF7,0x51,0x8C,0xC2,0x69,0x69,0xDC,0xC7,0xBB,0xC7,0xBB,0xC7,0xBB,0xE8,0xC3,0xC7,0xC3,0x60,0x79,0xE7,0x59,0x1D,0xE7,0xDB,0xDE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA,0xE6,0x9C,0xF7,0x35,0xA5,
    0xC4,0x61,0x88,0xDC,0xE7,0xC3,0xE7,0xC3,0xE7,0xC3,0x49,0xCC,0xA7,0xC3,0xC0,0x60,0xAB,0x6A,0xBB,0xDE,0x38,0xCE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x5B,0xF7,0x7C,0xF7,0x1C,0xDF,0x48,0x62,0xC3,0x9A,0xC7,0xC3,0xC7,0xBB,0xC7,0xBB,0x86,0xBB,0x83,0xAA,0x60,0x40,0xB3,0x9C,0xB5,0xC5,0x13,0xB5,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xD9,0xE6,0xCA,0x72,0xAA,0x6A,0xC7,0x51,0xE3,0x38,0xA5,0x59,0x84,0x51,0xA4,0x59,0x43,0x51,0x43,0x49,0x03,0x39,0x69,0x62,0xC6,0x49,0x30,0x94,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    
    //alpha channel data:
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xBA,0xB2,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4B,0xC1,0x1B,0x00,0xAB,0x6A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x21,0x6A,0x72,0x00,0x2D,0xBC,
    0xF6,0x79,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA3,0xAF,0x81,0x2E,0x04,0xA8,0xBE,0x3C,0x03,0xFF,0xFF,0xFF,0xFF,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x63,0x14,0xFF,0x2D,0x35,0xFF,0xFF,0xFF,0xFF,0xFA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEB,0xFF,0xFF,
    0xFF,0xFF,0x12,0x5B,0x54,0x2C,0xFF,0xFF,0xFF,0xFF,0xE5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD9,0xFF,0xFF,0xFF,0xFF,0x77,0x43,0x69,0x00,0xC2,0xFF,0xFF,0xFF,0x4F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6A,0xFF,0xFF,0xFF,0xFF,0x48,0x53,0x74,0x00,0x04,0xD8,0xFF,0x85,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xA5,0xFF,0xFF,0xFF,0x27,0x2C,0x9C,0x63,0x00,0x6B,0xE3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x39,0x43,0x37,0xA0,0xA3,0xF2,0x72,0x00,0x42,0x77,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0xFD,0xBF,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x4D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x4B,0x49,0x47,0x9D,0x9E,0x49,0x4F,0x4F,0x4C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xBB,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x5A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xCA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x5C,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xE1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x85,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB5,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x99,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x49,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xA2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x63,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x94,0x98,0xB5,0xDA,0xD6,0xE0,0xE2,0xDE,0xD6,0xD3,0x92,0xA0,0x53,0x00,0x00,0x00,0x00,0x00,
    };
const lv_image_dsc_t ui_img_plant_bad_png = {
   .header.w = 24,
   .header.h = 24,
   .data_size = sizeof(ui_img_plant_bad_png_data),
   .header.cf = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
   .header.magic = LV_IMAGE_HEADER_MAGIC,
   .data = ui_img_plant_bad_png_data};

