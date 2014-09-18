#include <iostream>
#include <cstring>

#include "Parser.h"

using namespace std;

Parser::Parser(const char *str): cmd_info(new CmdInfo), cmd_line(new char[strlen(str) + 1]) 
{ 
  strcpy(cmd_line, str); 
  cout << "Parser constr: " << cmd_line << endl;
}



Parser::~Parser() 
{
  delete cmd_info;
  delete [] cmd_line;
}

void Parser::run()
{
  const char *iter = cmd_line;
  goToNonBlank(iter);

  cout << "begin parse cmd line" << endl;

  while (*iter != '\0') {
    unsigned int &argc = cmd_info->cmds[cmd_info->num_pipe].argc;
    //char** argv = (char**)cmd_info->cmds[cmd_info->num_pipe].argv;

    if (*iter == '|') {
      cmd_info->cmds[cmd_info->num_pipe].argv[argc][0] = '\0';
      ++cmd_info->num_pipe;
      ++iter;
      goToNonBlank(iter);
    } else if (*iter == '<' || *iter == '>') {
      if (*iter++ == '<') {
        getWord(cmd_info->file_in, iter);
      } else {
        getWord(cmd_info->file_out, iter);
      }
    } else {
      getWord(cmd_info->cmds[cmd_info->num_pipe].argv[argc], iter);
      ++argc;
    }
  }

  unsigned int &argc = cmd_info->cmds[cmd_info->num_pipe].argc;
  //char **argv = (char**)cmd_info->cmds[cmd_info->num_pipe].argv;
  cmd_info->cmds[cmd_info->num_pipe].argv[argc][0] = '\0';
}

CmdInfo *Parser::getCmdInfo()
{
  return cmd_info;
}

void Parser::printCmdInfo()
{
  unsigned int i, j;
  for (i = 0; i < cmd_info->num_pipe + 1; i++) {
    cout << "cmd " << i << ": ";
    for (j = 0; j < cmd_info->cmds[i].argc; j++) {
      cout << "  " << cmd_info->cmds[i].argv[j] << endl; 
    }
  }
  cout << "infile:  " << cmd_info->file_in << endl;
  cout << "outfile: " << cmd_info->file_out << endl;
}

void Parser::goToNonBlank(const char * &iter)
{
  while(*iter != '\0' && *iter == ' ') { ++iter; }
  cout << "find nonblank: " << *iter << endl;
}

void Parser::getWord(char *dst, const char * &iter)
{
  cout << "begin get a word" << endl;
  char *const org = dst;
  goToNonBlank(iter);
  while(*iter != '\0' && *iter != ' ') { *dst++ = *iter++; }
  *dst = '\0';
  cout << "get word: " << dst << endl;
  goToNonBlank(iter);
}
