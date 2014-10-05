#include <stdlib.h>
#include <string.h>

#include "Command.h"

OneCmd::OneCmd(): argc(0) 
{ 
  memset(argv, 0, BUFFER_SIZE * sizeof(char*));
}

OneCmd::~OneCmd()
{
  int i;
  for (i = 0; i < argc; i++) {
    delete [] argv[i];
  }
}

void exit() { exit(0); }
const char *g_buildins[] = { "exit" };
const size_t g_num_buildins = sizeof(g_buildins) / sizeof(char*);
const Functor g_buildin_table[] = { exit };

