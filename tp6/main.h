#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


#define HELP_MESSAGE "\tWelcome to the Shell program\n" \
    "\tEnter \"help\" to view this manual\n" \
    "\tEnter \"exit\" to exit the shell\n" \
    "\t\n"
    
