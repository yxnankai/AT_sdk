/*! Utility functions for command line tools, like Linux (POSIX) implementation of _kbhit() and _getch()
@file cli_utils.h
*/
/*************************************************************************************
THIS SOFTWARE IS PROVIDED BY AUTOMATION TECHNOLOGY GMBH "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL AUTOMATION TECHNOLOGY GMBH BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

@copyright (c) 2020, Automation Technology GmbH.
@version 1.0, initial version
*************************************************************************************/
#ifndef cli_utils_H_INCLUDED
#define cli_utils_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_MSC_VER)
#include <conio.h>
#else
#include <unistd.h>
int _getch();
int _kbhit();
#endif

#ifdef __cplusplus
}
#endif

#endif /* cli_utils_H_INCLUDED */
