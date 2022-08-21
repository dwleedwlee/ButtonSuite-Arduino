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

#include "bounce.h"

inline static void setStateFlag(debouncer_t *pThis, const uint8_t flag) {
  pThis->state |= flag;
}

inline static void unsetStateFlag(debouncer_t *pThis, const uint8_t flag) {
  pThis->state &= ~flag;
}

inline static void toggleStateFlag(debouncer_t *pThis, const uint8_t flag) {
  pThis->state ^= flag;
}

inline static void changeState(debouncer_t *pThis) {
  toggleStateFlag(pThis, DEBOUNCED_STATE);
  setStateFlag(pThis, CHANGED_STATE);
  pThis->durationOfPreviousState = HAL_Ticks() - pThis->stateChangeLastTime;
  pThis->stateChangeLastTime = HAL_Ticks();
}

inline static bool getStateFlag(debouncer_t *pThis, const uint8_t flag) {
  return ((pThis->state & flag) != 0);
}

inline static bool readCurrentState(debouncer_t *pThis) {
  return HalDio_DigitalRead(pThis->pHal);
}

debouncer_t *BOUNCE_Init(debouncer_t *pThis, const _HalDio_t *pHal) {
  pThis->pHal = pHal;
  
  pThis->previousMillis = 0;
  pThis->intervalMillis = 5;
  pThis->state = 0;
  return pThis;
}

void BOUNCE_interval(debouncer_t *pThis, uint16_t intervalMillis) {
  pThis->intervalMillis = intervalMillis;
}

void BOUNCE_begin(debouncer_t *pThis) {
  pThis->state = 0;
  if(readCurrentState(pThis)) {
    setStateFlag(pThis, DEBOUNCED_STATE | UNSTABLE_STATE);
  }
  pThis->previousMillis = 0;
}

bool BOUNCE_update(debouncer_t *pThis) {
  unsetStateFlag(pThis, CHANGED_STATE);
  
#ifdef BOUNCE_LOCK_OUT
  if((HAL_Ticks() - pThis->previousMillis) >= pThis->intervalMillis) {
    bool currentState = readCurrentState(pThis);
    if(currentState != getStateFlag(pThis, DEBOUNCED_STATE)) {
      pThis->previousMillis = HAL_Ticks();
      changeState(pThis);
    }
  }
#elif defined BOUNCE_WITH_PROMPT_DETECTION
  bool readState = readCurrentState(pThis);
  
  if(readState != getStateFlag(pThis, DEBOUNCE_STATE)) {
    if(HAL_Ticks() - pThis->previousMillis >= pThis->intervalMillis) {
      changeState(pThis);
    }
  }
  
  if(readState != getStateFlag(pThis, UNSTABLE_STATE)) {
    toggleStateFlag(pThis, UNSTABLE_STATE);
    pThis->previousMillis = HAL_Ticks();
  }
#else
  bool currentState = readCurrentState(pThis);

  if(currentState != getStateFlag(pThis, UNSTABLE_STATE)) {
    pThis->previousMillis = HAL_Ticks();
    toggleStateFlag(pThis, UNSTABLE_STATE);
  } else {
    if(HAL_Ticks() - pThis->previousMillis >= pThis->intervalMillis) {
      if(currentState != getStateFlag(pThis, DEBOUNCED_STATE)) {
        pThis->previousMillis = HAL_Ticks();
        
        changeState(pThis);
      }
    }
  }
#endif

  return BOUNCE_changed(pThis);
}

bool BOUNCE_read(debouncer_t *pThis) {
  return getStateFlag(pThis, DEBOUNCED_STATE);
}

bool BOUNCE_fell(debouncer_t *pThis) {
  return !getStateFlag(pThis, DEBOUNCED_STATE) && getStateFlag(pThis, CHANGED_STATE);
}

bool BOUNCE_rose(debouncer_t *pThis) {
  return getStateFlag(pThis, DEBOUNCED_STATE) && getStateFlag(pThis, CHANGED_STATE);
}

bool BOUNCE_changed(debouncer_t *pThis) {
  return getStateFlag(pThis, CHANGED_STATE);
}

uint32_t BOUNCE_currentDuration(debouncer_t *pThis) {
  return (HAL_Ticks() - pThis->stateChangeLastTime);
}

uint32_t BOUNCE_previousDuration(debouncer_t *pThis) {
  return pThis->durationOfPreviousState;
}
