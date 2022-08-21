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

#include "button.h"

static buttonBase_t tableButton[BUTTON_RES_MAX];

buttonBase_t *ButtonBase_Init(buttonBase_t *pThis, const _HalDio_t *pHal, 
  uint16_t debounceInterval, uint32_t longPressInterval) {
  pThis->longPressInterval = longPressInterval;
  BOUNCE_interval(BOUNCE_Init(&(pThis->debouncer), pHal), debounceInterval);
  return pThis;
}

buttonBase_t *ButtonBase_GetStaticButtonFromIndex(buttonIdx_t idx) {
  return &(tableButton[idx]);
}

void ButtonBase_setDebounceInterval(buttonBase_t *pThis, uint16_t debounceInterval) {
  BOUNCE_interval(&(pThis->debouncer), debounceInterval);
}

buttonBfStatus_t ButtonBase_Update(buttonBase_t *pThis) {
  BOUNCE_update(&(pThis->debouncer));
  
  if(BOUNCE_fell(&(pThis->debouncer))) {
    return JUSTPRESSED;
  }
  
  if(!BOUNCE_read(&(pThis->debouncer))) {
    if(BOUNCE_currentDuration(&(pThis->debouncer)) < pThis->longPressInterval) {
      return ISSHORTPRESSED;
    } else {
      return ISLONGPRESSED;
    }
  }
  
  if(BOUNCE_rose(&(pThis->debouncer))) {
    if(BOUNCE_previousDuration(&(pThis->debouncer)) < pThis->longPressInterval) {
      return WASSHORTPRESSED;
    } else {
      return WASLONGPRESSED;
    }
  }
  
  return NOTPRESSED;
}
