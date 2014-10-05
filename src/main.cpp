#include <readline/readline.h> 
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "Parser.h"
#include "CmdRunner.h"

int main()
{
  char *input;
  char prompt[100];
  while (1) {
    snprintf(prompt, sizeof(prompt), "%s:%s $ ", getenv("USER"), getcwd(NULL, 1024));
    input = readline(prompt);
    Parser p(input);
    p.run();
#ifdef _DEBUG_
    p.printCmdInfo();
#endif
    CmdRunner app(p.getCmdInfo());
    app.run();
    free(input);
  }
}
