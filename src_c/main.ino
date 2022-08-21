
#include <jled.h>
#include "analyzer.h"
#include "button.h"
#include "hal_dio.h"

// breathe LED for 5 times, LED is connected to pin 9 (PWM capable) gpio
auto led = JLed(9).Breathe(2000).Repeat(5).DelayAfter(2000);

void setup() {
  Serial.begin(9600);
  Serial.println("Button Test Program");
  ButtonBase_Init(ButtonBase_GetStaticButtonFromIndex(BUTTON_RES_ALPHA), HalDio_GetStaticFromIndex(HAL_DIO_PIN_BOUNCE_APP), 50, 2000);
  ButtonAnalyzer_Init(ButtonAnalyzer_GetStaticFromIndex(BUTTON_ANALYZER_APP), ButtonBase_GetStaticButtonFromIndex(BUTTON_RES_ALPHA), 3);
}

void loop() {
  static bool state = false;
  if(true == ButtonAnalyzer_PushedUpdate(ButtonAnalyzer_GetStaticFromIndex(BUTTON_ANALYZER_APP))) {
    Serial.println("Button Pressed");
    led.Reset();
    state = true;
  }
  if(true == state) {
    led.Update();
  }
}
