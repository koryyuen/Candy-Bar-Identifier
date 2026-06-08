#include "image_provider.h"
#ifndef ARDUINO_EXCLUDE_CODE
#include "Arduino.h"
#include <TinyMLShield.h>

TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, int8_t* image_data) {

  static bool g_is_camera_initialized = false;
  static byte data[176 * 144 * 2];  // static so it's not on the stack

  if (!g_is_camera_initialized) {
    if (!Camera.begin(QCIF, RGB565, 5, OV7675)) {
      Serial.println("Camera init failed!");
      return kTfLiteError;
    }
    Serial.println("Camera init OK");
    g_is_camera_initialized = true;
  }

  Camera.readFrame(data);

  int min_x = (176 - 96) / 2;
  int min_y = (144 - 96) / 2;
  int index = 0;

  for (int y = min_y; y < min_y + 96; y++) {
    for (int x = min_x; x < min_x + 96; x++) {
      int pixel_index = (y * 176 + x) * 2;
      uint16_t pixel = (data[pixel_index + 1] << 8) | data[pixel_index];
      int8_t r = (int8_t)(((pixel >> 11) & 0x1F) * 255 / 31) - 128;
      int8_t g = (int8_t)(((pixel >> 5)  & 0x3F) * 255 / 63) - 128;
      int8_t b = (int8_t)((pixel & 0x1F) * 255 / 31) - 128;
      image_data[index++] = r;
      image_data[index++] = g;
      image_data[index++] = b;
    }
  }

  return kTfLiteOk;

}

#endif