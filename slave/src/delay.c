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
 *----------------------------------------------------------------------------*/


#include "delay.h"

int dflag=0;
void __ISR(_CORE_TIMER_VECTOR, IPL2) CTHandle(){
	mCTClearIntFlag();
	dflag=1;
}


void DelayMs(WORD delay)
{
    unsigned int int_status;
    while( delay-- )
    {
        OpenCoreTimer(GetSystemClock() / 2000);
        mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));
		while(!dflag);
   		dflag=0;
    }
	mDisableIntCoreTimer();
}
