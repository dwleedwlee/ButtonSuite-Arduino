/*
  The MIT License (MIT)
  
  Copyright (c) 2022 Dongwook Lee (elight.lee@gmail.com)
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Arduino.h>
#include "hal_dio.h"

static uint32_t digitalReadAdapter(uint16_t pin, bool *level) {
  *level = digitalRead(pin);
  return 0;
}

static const _HalDio_t tableHalDio[HAL_DIO_PIN_MAX] = {
  {digitalReadAdapter, 12},
};

uint32_t HAL_Ticks(void) {
  return millis();
}

const _HalDio_t *const HalDio_Init(halDioPin_t idx) {
  const _HalDio_t *hal = &(tableHalDio[idx]);
  pinMode(hal->pin, INPUT_PULLUP);
  return hal;
}

const _HalDio_t *const HalDio_GetStaticFromIndex(halDioPin_t idx) {
  return &(tableHalDio[idx]);
}

bool HalDio_DigitalRead(const _HalDio_t *hal) {
  bool level;
  
  hal->dioRead(hal->pin, &level);
#ifdef DIO_LOW_ACTIVE
    return level ? false : true;
#else
    return level ? true : false;
#endif
}
