#ifndef CONFIG_H_
#define CONFIG_H_

// Lolin S3 Pro //
Adafruit_NeoPixel rgb(1, 38, NEO_RGBW + NEO_KHZ800);

// NeoPixel brightness from 0 - 255 //
const uint8_t neopixel_brightness = 100;

const char *data_filename = "NetLog_data.csv",
           *bssid_filename = "NetLog_bssids.txt",

           *netlog_ver = "1.14",
           *netlog_vid = "NetLog",
           *netlog_name = "NetLog MK1";

// For the GY-NEO6MV2 (and many other GPS modules) //
#define GPS_BAUD 9600

// Lolin S3 Pro //
#define GPS_RXD2 10
#define GPS_TXD2 9
#endif
