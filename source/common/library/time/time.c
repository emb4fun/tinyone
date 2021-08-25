/**************************************************************************
*  Copyright (c) 2019 by Michael Fischer (www.emb4fun.de).
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
*
***************************************************************************
*  History:
*
*  22.09.2019  mifi  First Version.
**************************************************************************/
#define __TIME_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <time.h>
#include "time64.h"

/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  Time64Init                                                           */
/*                                                                       */
/*  this is a linker guard only.                                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void Time64Init (void)
{
   /* Do nothing here */
} /* Time64Init */

/*************************************************************************/
/*  gmtime_r                                                             */
/*                                                                       */
/*  Convert a time value to a structure.                                 */
/*                                                                       */
/*  In    : __tp, __result                                               */
/*  Out   : __result                                                     */
/*  Return: __result                                                     */
/*************************************************************************/
struct tm *gmtime_r (const time_t *__tp, struct tm *__result)
{   
   Time64_T _time = (uint32_t)*__tp;

   gmtime64_r(&_time, __result);

   return(__result);
} /* gmtime_r */

/*************************************************************************/
/*  localtime_r                                                          */
/*                                                                       */
/*  Convert a time value to a structure, GMT only.                       */
/*                                                                       */
/*  In    : __tp, __result                                               */
/*  Out   : __result                                                     */
/*  Return: __result                                                     */
/*************************************************************************/
struct tm *localtime_r (const time_t *__tp, struct tm *__result)
{
   Time64_T _time = (uint32_t)*__tp;

   gmtime64_r(&_time, __result);

   return(__result);
} /* localtime_r */

/*** EOF ***/
