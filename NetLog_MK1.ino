/*
  NetLog MK1 - The ESP32 WiFi logger
  by Tigersprojects

  Designed for an ESP32-S3 with a GPS module.

  Remember to set "USB CDC On Boot" to "Enabled"
  and "USB Mode" to "USB-OTG" under the Tools menu.

  You can usually set "Upload Mode" to
  "USB-OTG CDC (TinyUSB)" to avoid booting
  the device into DFU mode.

  Format the flash with FAT12 or FAT16
  for NetLog to work properly.
*/

#include <WiFi.h>

#include <Wire.h>
#include <TinyGPS++.h>
#include <Adafruit_NeoPixel.h>

#include "SPI.h"
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"
#include "Adafruit_TinyUSB.h"

// For flashTransport definition
#include "flash_config.h"

#include "config.h"

// Define stuff
String cur_ssid, csv_str;

char cur_bssid[18],
     temp_bssid[18],
     newline_bssid[19],
     temp_char;

bool exists = false,
     gps_new_data = false,
     flash_busy = false;

HardwareSerial neogps(1);
Adafruit_SPIFlash flash(&flashTransport);

FatVolume fatfs;
FatFile bssid_file;

TinyGPSPlus gps;
Adafruit_USBD_MSC usb_msc;


// Checks if a file exists
bool fileExists(const char *path) {
  FatFile file = fatfs.open(path);

  bool exists = file;
  if (exists) file.close();

  return exists;
}


// Prints to a file
void fileAppend(const char *path, const char *message) {
  File file = fatfs.open(path, FILE_WRITE);

  if (!file) return;

  file.print(message);
  file.close();
}


// Returns a CSV-safe string
String CSVSafeString(String input) {
  String output = "";

  // Double all quotes
  for (int c = 0; c < input.length(); c++) {
    output += ((input.charAt(c) == '"') ? "\"\"" : String(input.charAt(c)));
  }

  // Append and prepend quotes to the output
  output = "\"" + output + "\"";
  return output;
}


// Setup
void setup() {

  // Init NeoPixel
  rgb.begin();

  rgb.setPixelColor(0, rgb.Color(255, 255, 0));
  rgb.setBrightness(neopixel_brightness);

  rgb.show();

  // Init WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(1000);

  // Init storage
  flash.begin();

  // Configure usb_msc
  usb_msc.setID(netlog_vid, netlog_name, netlog_ver);
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  usb_msc.setCapacity(flash.size() / 512, 512);

  usb_msc.setUnitReady(true);
  usb_msc.begin();

  // Reattach if already mounted
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  // Init flash FS
  while (!fatfs.begin(&flash)) {
    rgb.setPixelColor(0, rgb.Color(255, 0, 0));
    rgb.show();

    delay(1000);
  }

  // Create necessary files if they don't exist
  if (!fileExists(data_filename)) {
    flash_busy = true;
    fileAppend(data_filename, "\"ssid\",\"bssid\",\"rssi\",\"auth\",\"coord\",\"speed\"\n");
    flash_busy = false;
  }

  if (!fileExists(bssid_filename)) {
    flash_busy = true;
    fileAppend(bssid_filename, "");
    flash_busy = false;
  }

  // Connect to GPS
  neogps.begin(GPS_BAUD, SERIAL_8N1, GPS_RXD2, GPS_TXD2);
  delay(2000);

  // Startup successful (with cool fade effect)
  for (int b = 0; b < neopixel_brightness; b++) {
    rgb.setBrightness(neopixel_brightness - b);
    rgb.show();

    delay(5);
  }

  rgb.setPixelColor(0, rgb.Color(0, 0, 0));
  rgb.setBrightness(neopixel_brightness);
  rgb.show();

  delay(500);
}


// Loop
void loop() {

  // Check if there is available data from the GPS module
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        gps_new_data = true;
      }
    }
  }

  // Start scanning networks if the GPS module has a connection
  if (gps.location.isValid() && gps.satellites.value() != 0) {

    // Make sure the NeoPixel is off
    rgb.setPixelColor(0, rgb.Color(0, 0, 0));
    rgb.show();

    // Scan networks
    unsigned long networks_found = WiFi.scanNetworks();

    // Search for new networks
    for (int i = 0; i < networks_found; i++) {

      // Get SSID and stop at newline char ('\n')
      // (Yes, you can put newlines in SSIDs)    
      cur_ssid = WiFi.SSID(i).substring(0, WiFi.SSID(i).indexOf('\n'));

      // Get BSSID
      memset(cur_bssid, '\0', 18);
      WiFi.BSSIDstr(i).toCharArray(cur_bssid, 18);

      // Open BSSID file and make sure the BSSID doesn't already exist
      bssid_file = fatfs.open(bssid_filename);
      exists = false;

      while (bssid_file.available()) {

        // Check first character for maximum speed
        temp_char = bssid_file.read();

        if (temp_char == cur_bssid[0]) {

          // Read entire string if the characters match
          memset(temp_bssid, '\0', 18);

          while (temp_char != '\n') {
            strncat(temp_bssid, &temp_char, 1);
            temp_char = bssid_file.read();
          }

          if (strcmp(temp_bssid, cur_bssid) == 0) {
            exists = true;
            break;
          }

        } else {

          // Go to next newline if the characters don't match
          while (bssid_file.read() != '\n');
        }
      }

      bssid_file.close();


      // Save network info
      if (!exists) {

        // Flash NeoPixel to signal new network
        rgb.setPixelColor(0, rgb.Color(255, 255, 255));
        rgb.show();

        delay(100);

        rgb.setPixelColor(0, rgb.Color(0, 0, 0));
        rgb.show();

        delay(100);

        // Add SSID, BSSID, and RSSI to CSV file
        csv_str = CSVSafeString(cur_ssid) + "," +
                  CSVSafeString(cur_bssid) + "," +
                  CSVSafeString(String(WiFi.RSSI(i)) + " dBm") + ",";

        // Append auth to CSV file
        switch (WiFi.encryptionType(i)) {
          case WIFI_AUTH_OPEN:
            csv_str += CSVSafeString("Open"); break;

          case WIFI_AUTH_WEP:
            csv_str += CSVSafeString("WEP"); break;

          case WIFI_AUTH_WPA_PSK:
            csv_str += CSVSafeString("WPA PSK"); break;

          case WIFI_AUTH_WPA2_PSK:
            csv_str += CSVSafeString("WPA2 PSK"); break;

          case WIFI_AUTH_WPA_WPA2_PSK:
            csv_str += CSVSafeString("WPA WPA2 PSK"); break;

          case WIFI_AUTH_WPA2_ENTERPRISE:
            csv_str += CSVSafeString("WPA2 Enterprise"); break;
          
          case WIFI_AUTH_WPA3_PSK:
            csv_str += CSVSafeString("WPA3 PSK"); break;

          case WIFI_AUTH_WPA2_WPA3_PSK:
            csv_str += CSVSafeString("WPA2 WPA3 PSK"); break;

          case WIFI_AUTH_WAPI_PSK:
            csv_str += CSVSafeString("WAPI PSK"); break;

          default:
            csv_str += CSVSafeString(String("Unknown Auth (" +
                      String(WiFi.encryptionType(i)) + ")"));
            break;
        
        } csv_str += ",";

        // Save NetLog position and speed (skipping altitude)
        csv_str += CSVSafeString(String(
          ((gps.location.lat() >= 0) ? "+" : "") + String(gps.location.lat(), 6) + " " +
          ((gps.location.lng() >= 0) ? "+" : "") + String(gps.location.lng(), 6)
        )) + ",";

        csv_str += CSVSafeString(String(gps.speed.kmph(), 1) + " km/h") + "\n";

        // Create BSSID with a newline
        memset(newline_bssid, '\0', 19);
        strncpy(newline_bssid, cur_bssid, 18);
        strncat(newline_bssid, "\n", 1);

        // Append data to files
        flash_busy = true;

        if (!fileExists(data_filename))
          fileAppend(data_filename, "\"ssid\",\"bssid\",\"rssi\",\"auth\",\"coord\",\"speed\"\n");

        fileAppend(data_filename, csv_str.c_str());
        fileAppend(bssid_filename, newline_bssid);

        flash_busy = false;
      }
    }

  } else {
    if (!gps_new_data) {

      // No connection to GPS module
      rgb.setPixelColor(0, rgb.Color(255, 0, 0));
      rgb.show();

      delay(500);

      rgb.setPixelColor(0, rgb.Color(0, 0, 0));
      rgb.show();

    } else {

      // Searching for satellites
      rgb.setPixelColor(0, rgb.Color(0, 255, 0));
      rgb.show();

      delay(500);

      rgb.setPixelColor(0, rgb.Color(0, 0, 0));
      rgb.show();
    }

    delay(500);
  }
}


// Callback for read command
int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize) {
  while (flash_busy);

  int32_t block = flash.readBlocks(lba, (uint8_t*)buffer, bufsize / 512) ? bufsize : -1;

  return block;
}


// Callback for write command
int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize) {
  while (flash_busy);

  int32_t block = flash.writeBlocks(lba, buffer, bufsize / 512) ? bufsize : -1;

  return block;
}


// Callback for flush command
void msc_flush_cb(void) {
  while (flash_busy);

  flash.syncBlocks();
  fatfs.cacheClear();
}
