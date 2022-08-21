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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "bounce.h"

typedef enum {
  BUTTON_RES_ALPHA = 0,
  BUTTON_RES_BETA,
  BUTTON_RES_MAX
} buttonIdx_t;

typedef enum {
  JUSTPRESSED = 0x01,
  ISSHORTPRESSED = 0x02,
  WASSHORTPRESSED = 0x04,
  ISLONGPRESSED = 0x08,
  WASLONGPRESSED = 0x10,
  NOTPRESSED = 0x20
} buttonBfStatus_t;

typedef struct {
  debouncer_t debouncer;
  uint32_t longPressInterval;
}buttonBase_t;

buttonBase_t *ButtonBase_Init(buttonBase_t *pThis, const _HalDio_t *pHal, 
  uint16_t debounceInterval, uint32_t longPressInterval);
buttonBase_t *ButtonBase_GetStaticButtonFromIndex(buttonIdx_t idx);
void ButtonBase_setDebounceInterval(buttonBase_t *pThis, uint16_t debounceInterval);
buttonBfStatus_t ButtonBase_Update(buttonBase_t *pThis);

#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H__ */
