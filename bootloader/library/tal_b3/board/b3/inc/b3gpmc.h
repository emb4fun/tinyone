/**************************************************************************
*  Copyright (c) 2015-2023 by Michael Fischer (www.emb4fun.de)
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
**************************************************************************/
#if !defined(__B3GPMC_H__)
#define __B3GPMC_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>

/**************************************************************************
*  All Structures and Common Constants
**************************************************************************/

#define GPMC_CS0_START_ADDRESS   0x10000000 

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/
void GPMCInit (void);
void GPMCResetSet (void);
void GPMCResetClear (void);
void GPMCConfigureIRQ (void (*IrqHandler)(void));

#endif /* !__B3GPMC_H__ */

/*** EOF ***/
