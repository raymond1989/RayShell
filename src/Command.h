#ifndef COMMAND_H
#define COMMAND_H

#define BUFFER_SIZE 100

struct OneCmd {
  char argv[BUFFER_SIZE][BUFFER_SIZE];
  unsigned int argc;

  OneCmd(): argc(0) { argv[0][0] = '\0'; }
};

struct CmdInfo {
  OneCmd cmds[BUFFER_SIZE];
  unsigned int num_pipe;
  char file_in[BUFFER_SIZE];
  char file_out[BUFFER_SIZE];

  CmdInfo(): num_pipe(0) 
  { 
    file_in[0] = '\0';
    file_out[0] = '\0';
  }
};

#endif
