//Header for TUI (Terminal User Interface) functions, 2024

#ifndef TUICONTROLLER_HPP
#define TUICONTROLLER_HPP

#include <stdlib.h> 

struct TerminalSize{
    int rows;
    int columns;
};

#ifndef __unix__
#include <windows.h>

TerminalSize GetTerminalSize(){
    TerminalSize output;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns,rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    output.rows = rows;
    output.columns = columns;

    return output;
}

void cleanTerminal()
{
    system("cls");
}

#endif

#ifndef __WIN32
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>



TerminalSize GetTerminalSize()
{
    TerminalSize output;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    output.rows = w.ws_row;
    output.columns = w.ws_col;
    return output;
}

void CleanTerminal()
{
    system("clear");
}


#endif


#endif