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
  // Have the debouncer update.  This does most of the work of handling the button state.
  // The debouncer determines the current state, if the state changed since the last time
  // "update" was called, what the time between state changes was, and so on.
  BOUNCE_update(&(pThis->debouncer));
  
  // Catch transitions from HIGH to LOW.  This 
  if(BOUNCE_fell(&(pThis->debouncer))) {
    return JUSTPRESSED;
  }

  // Look to see if the button is currently pressed.
  // The "Bounce.read()" function returns true if the pin is reading high.  In
  // our case, the button is pressed if the pin is low.  So the button is pressed
  // if "read" returns false.  
  if(!BOUNCE_read(&(pThis->debouncer))) {
    if(BOUNCE_currentDuration(&(pThis->debouncer)) < pThis->longPressInterval) {
      return ISSHORTPRESSED;
    } else {
      return ISLONGPRESSED;
    }
  }

  // Catch transitions from LOW to HIGH.  This is the button release.  If the
  // button is released, we then check to see if the press duration makes is a
  // short or long press.  
  if(BOUNCE_rose(&(pThis->debouncer))) {
    if(BOUNCE_previousDuration(&(pThis->debouncer)) < pThis->longPressInterval) {
      return WASSHORTPRESSED;
    } else {
      return WASLONGPRESSED;
    }
  }
  
  // Button is still in unpushed state and did not just get released.  In
  // this case, the button is considered idle (nothing happened).
  return NOTPRESSED;
}
