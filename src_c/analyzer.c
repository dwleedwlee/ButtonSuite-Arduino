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

#include "analyzer.h"

static buttonAnalyzer_t tableButtonAnalyzer[BUTTON_ANALYZER_MAX];

buttonAnalyzer_t *ButtonAnalyzer_Init(buttonAnalyzer_t *pThis, buttonBase_t *base, const uint8_t sustainCount) {
  pThis->base = base;
  pThis->sustainCount = sustainCount;
  pThis->analyzedStat = NOTPRESSED;
  pThis->sustainVal = 1;
  
  return pThis;
}

buttonAnalyzer_t *ButtonAnalyzer_GetStaticFromIndex(buttonAnalyzerIdx_t idx) {
  return &(tableButtonAnalyzer[idx]);
}

void ButtonAnalyzer_Update(buttonAnalyzer_t *pThis) {
  buttonBfStatus_t curStat = ButtonBase_Update(pThis->base);
  
  if(pThis->analyzedStat == WASSHORTPRESSED) {
    if(pThis->sustainVal <= 1) {
      pThis->analyzedStat = NOTPRESSED;
    } else {
      pThis->sustainVal--;
    }
  } else if (pThis->analyzedStat == ISLONGPRESSED) {
    if(pThis->sustainVal <= 1) {
      if(curStat != ISLONGPRESSED) {
        pThis->analyzedStat = NOTPRESSED;
      }
    } else {
      pThis->sustainVal--;
    }
  } else {
    if((curStat == WASSHORTPRESSED) || (curStat == ISLONGPRESSED)) {
      pThis->analyzedStat = curStat;
      pThis->sustainVal = pThis->sustainCount;
    }
  }
}

bool ButtonAnalyzer_PushedUpdate(buttonAnalyzer_t *pThis) {
  buttonBfStatus_t curStat = ButtonBase_Update(pThis->base);
  
  if((curStat == WASSHORTPRESSED) || (curStat == WASLONGPRESSED)) {
    pThis->analyzedStat = curStat;
    return true;
  } else {
    pThis->analyzedStat = NOTPRESSED;
    return false;
  }
}
