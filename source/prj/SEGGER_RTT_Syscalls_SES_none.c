/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
Purpose : Implementation of low-level functions for I/O with the
          SEGGER Runtime Library.
          
Note    : This file is based on an SEGGER example from here:
          https://wiki.segger.com/Embedded_Studio_Library_IO#None         
*/

/*********************************************************************
*
*       #include section
*
**********************************************************************
*/

#if defined(__SES_VERSION) && (__SES_VERSION >= 60000)

#include "__SEGGER_RTL_Int.h"
#include "stdint.h"
#include "stdio.h"
#include "time.h"

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*       Local types
*
**********************************************************************
*/

struct __SEGGER_RTL_FILE_impl    // NOTE: Provides implementation for FILE
{
   int stub; // only needed so impl has size != 0.
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static FILE __SEGGER_RTL_stdin_file  = { 0 };  // stdin reads from UART
static FILE __SEGGER_RTL_stdout_file = { 0 };  // stdout writes to UART
static FILE __SEGGER_RTL_stderr_file = { 0 };  // stderr writes to UART

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

FILE *stdin  = &__SEGGER_RTL_stdin_file;  // NOTE: Provide implementation of stdin for RTL.
FILE *stdout = &__SEGGER_RTL_stdout_file; // NOTE: Provide implementation of stdout for RTL.
FILE *stderr = &__SEGGER_RTL_stderr_file; // NOTE: Provide implementation of stderr for RTL.

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       __SEGGER_RTL_X_file_stat()
*
*  Function description
*    Get file status.
*
*  Parameters
*    stream - Pointer to file.
*
*  Additional information
*    Low-overhead test to determine if stream is valid.  If stream
*    is a valid pointer and the stream is open, this function must
*    succeed.  If stream is a valid pointer and the stream is closed,
*    this function must fail.
*
*    The implementation may optionally determine whether stream is
*    a valid pointer: this may not always be possible and is not
*    required, but may assist debugging when clients provide wild
*    pointers.
*
*  Return value
*    <  0 - Failure, stream is not a valid file.
*    >= 0 - Success, stream is a valid file.
*/
int __SEGGER_RTL_X_file_stat(FILE *stream) 
{
   (void)stream;
   
   return(-1);
}

/*********************************************************************
*
*       __SEGGER_RTL_X_file_bufsize()
*
*  Function description
*    Get stream buffer size.
*
*  Parameters
*    stream - Pointer to file.
*
*  Additional information
*    Returns the number of characters to use for buffered I/O on
*    the file stream.  The I/O buffer is allocated on the stack
*    for the duration of the I/O call, therefore this value should
*    not be set arbitrarily large.
*
*    For unbuffered I/O, return 1.
*
*  Return value
*    Nonzero number of characters to use for buffered I/O; for
*    unbuffered I/O, return 1.
*/
int __SEGGER_RTL_X_file_bufsize(FILE *stream) 
{
   (void)stream;
   
   return(-1);
}

/*********************************************************************
*
*       __SEGGER_RTL_X_file_read()
*
*  Function description
*    Read data from file.
*
*  Parameters
*    stream - Pointer to file to read from.
*    s      - Pointer to object that receives the input.
*    len    - Number of characters to read from file.
*
*  Return value
*    >= 0 - Success, amount of data read.
*    <  0 - Failure.
*
*  Additional information
*    Reading from any stream other than stdin results in an error.
*/
int __SEGGER_RTL_X_file_read(FILE *stream, char *s, unsigned len) 
{
   (void)stream;
   (void)len;
   
   return(-1);
}

/*********************************************************************
*
*       __SEGGER_RTL_X_file_write()
*
*  Function description
*    Write data to file.
*
*  Parameters
*    stream - Pointer to file to write to.
*    s      - Pointer to object to write to file.
*    len    - Number of characters to write to the file.
*
*  Return value
*    >= 0 - Success.
*    <  0 - Failure.
*
*  Additional information
*    this version is NOT reentrant!
*    stdout and stderr are directed to UART;
*    writing to any stream other than stdout or stderr results in an error
*/
int __SEGGER_RTL_X_file_write(FILE *stream, const char *s, unsigned len) 
{
   (void)stream;
   (void)s;
   (void)len;
   
   return(-1);
}

/*********************************************************************
*
*       __SEGGER_RTL_X_file_unget()
*
*  Function description
*    Push character back to stream.
*
*  Parameters
*    stream - Pointer to file to push back to.
*    c      - Character to push back.
*
*  Return value
*    >= 0 - Success.
*    <  0 - Failure.
*
*  Additional information
*    Push-back is only supported for standard input, and
*    only a single-character pushback buffer is implemented.
*/
int __SEGGER_RTL_X_file_unget(FILE *stream, int c) 
{
   (void)stream;
   (void)c;
   
   return(-1);
}

/********************************************************************/

/*
 * __SEGGER_RTL_X_get_time_of_day()
 *
 * This function was still missing. 
 *
 */

uint32_t OS_UnixtimeGet (void); 
 
int __SEGGER_RTL_X_get_time_of_day (struct timeval *__tp)
{
   __tp->tv_sec  = OS_UnixtimeGet();
   __tp->tv_usec = 0;
   
   return(0);
}

#endif // defined(__SES_VERSION) && (__SES_VERSION >= 60000)

/*************************** End of file ****************************/