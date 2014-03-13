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
*   File: delay.c
*   Description: Implements delay-related functions
*-----------------------------------------------------------------------------*/

#include "delay.h"

int dflag=0;
void __ISR(_TIMER_1_VECTOR, IPL2) T1Handle(){
    mT1ClearIntFlag();
    dflag=1;
}


void DelayMs(WORD delay)
{
    while( delay-- )
    {
        OpenTimer1(T1_ON | T1_PS_1_256, 128);
        ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2 | T1_INT_SUB_PRIOR_2);
        EnableIntT1;
	while(!dflag);
   	dflag=0;
    }
    DisableIntT1;
}
