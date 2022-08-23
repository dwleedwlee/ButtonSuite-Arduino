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

#ifndef __BOUNCE_H__
#define __BOUNCE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "hal_dio.h"

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT

// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

typedef enum {
  DEBOUNCED_STATE = 0x01,	// Final returned calculated debounced state
  UNSTABLE_STATE = 0x02,	// Actual last state value behind the scene
  CHANGED_STATE = 0x04,		// The DEBOUNCED_STATE has changed since last update()
}bounceBfState_t;

typedef struct {
  const _HalDio_t *pHal;
  
  uint32_t previousMillis;
  uint16_t intervalMillis;
  uint8_t state;
  uint32_t stateChangeLastTime;
  uint32_t durationOfPreviousState;
}debouncer_t;

debouncer_t *BOUNCE_Init(debouncer_t *pThis, const _HalDio_t *pHal);
/**
@brief  Sets the debounce interval in milliseconds.		
@param    interval_millis
		The interval time in milliseconds. 
 */
void BOUNCE_interval(debouncer_t *pThis, uint16_t intervalMillis);
void BOUNCE_begin(debouncer_t *pThis);
/*!
@brief   Updates the pin's state. 
Because Bounce does not use interrupts, you have to "update" the object before reading its value and it has to be done as often as possible (that means to include it in your loop()). Only call update() once per loop().
@return True if the pin changed state.
*/
bool BOUNCE_update(debouncer_t *pThis);
/**
 @brief Returns the pin's state (HIGH or LOW).
 @return HIGH or LOW.
 */
bool BOUNCE_read(debouncer_t *pThis);
/**
@brief Returns true if pin signal transitions from high to low.
*/
bool BOUNCE_fell(debouncer_t *pThis);
/**
@brief Returns true if pin signal transitions from low to high.
*/
bool BOUNCE_rose(debouncer_t *pThis);
/**
 @brief Returns true if the state changed on last update.
 @return True if the state changed on last update. Otherwise, returns false.
*/
bool BOUNCE_changed(debouncer_t *pThis);
/**
@brief Returns the duration in milliseconds of the current state. 
Is reset to 0 once the pin rises ( rose() ) or falls ( fell() ).
@return The duration in milliseconds (unsigned long) of the current state.
*/
uint32_t BOUNCE_currentDuration(debouncer_t *pThis);
/**
 @brief Returns the duration in milliseconds of the previous state.
 Takes the values of duration() once the pin changes state.
 @return The duration in milliseconds (unsigned long) of the previous state. 
 */
uint32_t BOUNCE_previousDuration(debouncer_t *pThis);

#ifdef __cplusplus
}
#endif

#endif /* __BOUNCE_H__ */
