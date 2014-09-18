#include <readline/readline.h> 
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include "Parser.h"

int main()
{
  char *input;
  char prompt[100];
  while (1) {
    snprintf(prompt, sizeof(prompt), "%s:%s $ ", getenv("USER"), getcwd(NULL, 1024));
    input = readline(prompt);
    Parser p(input);
    p.run();
    p.printCmdInfo();
    free(input);
  }
}
