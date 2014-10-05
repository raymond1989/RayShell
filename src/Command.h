#ifndef COMMAND_H
#define COMMAND_H

#define BUFFER_SIZE 100

struct OneCmd {
  char *argv[BUFFER_SIZE];
  unsigned int argc;

  OneCmd();
  ~OneCmd();
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

extern const size_t g_num_buildins;
extern const char *g_buildins[]; 
typedef void (*Functor)();
extern const Functor g_buildin_table[];

#endif
