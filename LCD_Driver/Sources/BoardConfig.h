/*HEADER******************************************************************************************
*
* Copyright 2013 Freescale Semiconductor, Inc.
*
* Freescale Confidential Proprietary. Licensed under the Freescale AOA Protocol Software License. 
* See the FREESCALE_AOA_Protocol_LICENSE file distributed with this work for more details. You may 
* not use this file except in compliance with the License.
*
**************************************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES, 
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                                         
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Defines & Macros Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

#define CPU_CLOCK	(8000000UL)

#define BUS_CLOCK	(8000000UL)
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Typedef Section                                          
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                Function-like Macros Section                                   
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Extern Constants Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Extern Variables Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                Function Prototypes Section                                    
///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

void BoardConfig_vfnInit(void);

#if defined(__cplusplus)
}
#endif // __cplusplus


#endif /* SYSTEMCONFIG_H_ */
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
