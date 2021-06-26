/*
	The MIT License (MIT)
	
	Copyright (c) 2019 Lance A. Endres
	
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

#include "ResettableButton.h"

// Constructors.
Resettable::Resettable(int pin) :
	ButtonBase(pin),
	_resetOnLongPress(true)
{
}

Resettable::Resettable(int pin, int debounceInterval) :
	ButtonBase(pin, debounceInterval),
	_resetOnLongPress(true)
{
}

// Destructor.
Resettable::~Resettable()
{
}

void Resettable::setResetOnLongPress(bool enabled)
{
	_resetOnLongPress = enabled;
}

void Resettable::setLongPressInterval(int longPressInterval)
{
	// The "_longPressInterval" member is in the "ButtonBase" class.
  _longPressInterval = longPressInterval;
}