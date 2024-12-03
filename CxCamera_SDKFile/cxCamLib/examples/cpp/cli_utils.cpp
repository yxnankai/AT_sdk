/**
@file cli_utils.cpp
@package CxCamLib library
@brief utilities for command line tools, e.g. Linux (POSIX) implementation of _kbhit() and _getch().
@copyright (c) 2020, Automation Technology GmbH.
@version 1.0, initial version
*/
#include "cli_utils.h"

#ifdef __linux__
/**
 Linux (POSIX) implementation of _kbhit() and _getch().
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#define NO_STROPTS // Throws errors while building
#ifndef NO_STROPTS
#include <stropts.h>
#endif

/** _kbhit returns nonzero, if a keystroke is waiting in the buffer
*/
int _kbhit()
{
    termios prev;
    tcgetattr(STDIN_FILENO, &prev);
    termios term2 = prev;
    term2.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &term2);
    int byteswaiting;
    ioctl(STDIN_FILENO, FIONREAD, &byteswaiting);
    tcsetattr(STDIN_FILENO, TCSANOW, &prev);
    return byteswaiting;
}

/** reads from keypress, doesn't echo
*/
int _getch()
{
    struct termios prev, newt;
    int            ch;
    tcgetattr(STDIN_FILENO, &prev);
    newt = prev;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &prev);
    return ch;
}
#endif
