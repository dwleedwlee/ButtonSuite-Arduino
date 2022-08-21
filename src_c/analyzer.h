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

#ifndef __ANALYZER_H__
#define __ANALYZER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "button.h"

typedef enum {
  BUTTON_ANALYZER_APP = 0,
  BUTTON_ANALYZER_MAX
}buttonAnalyzerIdx_t;

typedef struct {
  buttonBase_t *base;
  buttonBfStatus_t analyzedStat;
  uint8_t sustainCount;
  uint8_t sustainVal;
}buttonAnalyzer_t;

buttonAnalyzer_t *ButtonAnalyzer_Init(buttonAnalyzer_t *pThis, buttonBase_t *base, const uint8_t sustainCount);
buttonAnalyzer_t *ButtonAnalyzer_GetStaticFromIndex(buttonAnalyzerIdx_t idx);
void ButtonAnalyzer_Update(buttonAnalyzer_t *pThis);
bool ButtonAnalyzer_PushedUpdate(buttonAnalyzer_t *pThis);

#ifdef __cplusplus
}
#endif

#endif /* __ANALYZER_H__ */
