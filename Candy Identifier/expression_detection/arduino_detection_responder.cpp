/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "detection_responder.h"
#include "model_settings.h"
#include "Arduino.h"

// Flash the blue LED after each inference
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        int8_t* scores, int num_classes) {
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    is_initialized = true;
  }

  // Flash blue between captures
  digitalWrite(LEDB, LOW);
  delay(100);
  digitalWrite(LEDB, HIGH);

  // Find the highest scoring class
  int best_index = 0;
  int8_t best_score = scores[0];
  for (int i = 1; i < num_classes; i++) {
    if (scores[i] > best_score) {
      best_score = scores[i];
      best_index = i;
    }
  }
  Serial.print("Candy: ");
  Serial.println(kCategoryLabels[best_index]);
  Serial.print("Score: ");
  Serial.println(best_score);
  TF_LITE_REPORT_ERROR(error_reporter, "Candy: %s (score: %d)",
                       kCategoryLabels[best_index], best_score);
}

#endif  // ARDUINO_EXCLUDE_CODE
