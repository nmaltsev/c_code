#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define HELP_CMD_MESSAGE "Enter ? for help"
#define HELP_MESSAGE "\tFormat: cmd l_type start length [whence(optional)]\n" \
    "\t'cmd' --- 'g' (F_GETLK), 's' (F_SETLK), or 'w' (F_SETLKW)\n" \
    "\t'l_type' --- 'r' (F_RDLCK), 'w' (F_WRLCK), or 'u' (F_UNLCK)\n" \
    "\t'start' --- lock starting offset\n" \
    "\t'length' --- lock starting offset\n" \
    "\t'whence' --- 's' (SEEK_SET, default), 'c' (SEEK_CUR), or 'e' (SEEK_END)"
    
