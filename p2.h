#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "getword.h"
#include "CHK.h"



#define MAXITEM 100
#define MAXCHARS 26000

int parse();

void signalHandler();

void executePipe();

