#ifndef PARSER_H
#define PARSER_H

#include "Command.h"

class Parser {
  public:
    Parser(const char*);
    ~Parser();

    void run();
    CmdInfo *getCmdInfo();
    void printCmdInfo();

  private:
    CmdInfo *cmd_info;
    char    *cmd_line;
    //enum State {CMD, REDIR, PIPE}
    void goToNonBlank(const char* &);
    void getWord(char *, const char* &);

    //noncopyable
    Parser(const Parser&);
    Parser& operator=(const Parser&);
};

#endif
