/*-----------------------------------------------------------------------------*
 *
 *   Copyright 2013 Tom Cahill
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 *   File: delay.h
 *
 *   Description: Defines delay-related functions
 *----------------------------------------------------------------------------*/


#ifndef DELAY_H
#define DELAY_H

#include <plib.h>

#define SYS_FREQ (80000000)
#define GetSystemClock()       (SYS_FREQ)

void DelayMs(WORD);

#endif