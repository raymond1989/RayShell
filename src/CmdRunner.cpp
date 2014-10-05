#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "CmdRunner.h"
#include "Command.h"

void CmdRunner::run()
{
  int ret_code;
  if (_cmdinfo->num_pipe == 0) {
    if (isBuildin()) {
      exit(0);
    }
    if (fork() == 0) {
      if (strcmp(_cmdinfo->file_in, "") != 0) {
        int fin = open(_cmdinfo->file_in, O_RDONLY);
        if (fin < 0) {
          perror("cannot open file in\n");
          exit(1);
        }
        if (dup2(fin, 0) != 0) {
          perror("cannot dup file in\n");
          exit(1);
        }
        close(fin);
      }

      if (strcmp(_cmdinfo->file_out, "") != 0) {
        int fout = open(_cmdinfo->file_out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (fout < 0) {
          perror("cannot open file out\n");
          exit(1);
        }
        if (dup2(fout, 1) != 1) {
          perror("cannot dup file out\n");
          exit(1);
        }
        close(fout);
      }

      execvp(_cmdinfo->cmds[0].argv[0], (char**)(_cmdinfo->cmds[0].argv));
      perror("fail on execvp\n");
      exit(1);
    } else {
      wait(&ret_code);
#ifdef _DEBUG_
      printf("ret code %d\n", ret_code);
#endif
    }
  } else {
    int pid = fork();
    if (pid < 0) {
      perror("fork error");
      exit(1);
    } else if (pid == 0) {
      execOneCmd(_cmdinfo->num_pipe, _cmdinfo->cmds[_cmdinfo->num_pipe].argv);
    } else {
      int ret;
      wait(&ret);
    }
  }
}

void CmdRunner::execOneCmd(int cmd_id, char ** const argv, int fd_out)
{
  int fd_pair[2];
  if (cmd_id == 0) { // first cmd
  // redirect stdin to file if infile exists
      if (strcmp(_cmdinfo->file_in, "") != 0) {
        int fin = open(_cmdinfo->file_in, O_RDONLY);
        if (fin < 0) {
          perror("cannot open file in\n");
          exit(1);
        }
        if (dup2(fin, 0) != 0) {
          perror("cannot dup file in\n");
          exit(1);
        }
        close(fin);
      }

  // redirect stdout to pipe write end
      if (dup2(fd_out, 1) != 1) {
        perror("dup2 error");
        exit(1);
      }
      close(fd_out);

      execvp(argv[0], argv);
      perror("fail on execvp\n");
      exit(1);

  } else if (cmd_id == this->_cmdinfo->num_pipe) { // last cmd
  // redirect stdout to file if outfile exists
    if (strcmp(_cmdinfo->file_out, "") != 0) {
        int fout = open(_cmdinfo->file_out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (fout < 0) {
          perror("cannot open file out\n");
          exit(1);
        }
        if (dup2(fout, 1) != 1) {
          perror("cannot dup file out\n");
          exit(1);
        }
        close(fout);
    }
  // creat pipe
    pipe(fd_pair);
    int pid = fork();
    if (pid < 0) {
      perror("fork error\n");
      exit(1);
    } else if (pid == 0) {
      close(fd_pair[0]);
      execOneCmd(cmd_id - 1, (char**)_cmdinfo->cmds[cmd_id -1].argv, fd_pair[1]);
      //close(fd_pair[1]);
    } else {
 // redirect input to pipe read end
      close(fd_pair[1]);
      if (dup2(fd_pair[0], 0) != 0) {
        perror("dup2 error");
        exit(1);
      }
      close(fd_pair[0]);
      int ret;
      waitpid(pid, &ret, 0);
      execvp(argv[0], argv);
      perror("fail on execvp\n");
      exit(1);
    }

  } else { // cmds in the middle
  // redirect stdout to pipe write end
      if (dup2(fd_out, 1) != 1) {
        perror("dup2 error");
        exit(1);
      }
      close(fd_out);

  // creat pipe
    pipe(fd_pair);
    int pid = fork();
    if (pid < 0) {
      perror("fork error\n");
      exit(1);
    } else if (pid == 0) {
      close(fd_pair[0]);
      execOneCmd(cmd_id - 1, (char**)_cmdinfo->cmds[cmd_id -1].argv, fd_pair[1]);
      //close(fd_pair[1]);
    } else {
 // redirect input to pipe read end
      close(fd_pair[1]);
      if (dup2(fd_pair[0], 0) != 0) {
        perror("dup2 error");
        exit(1);
      }
      close(fd_pair[0]);
      int ret;
      waitpid(pid, &ret, 0);
      execvp(argv[0], argv);
      perror("fail on execvp\n");
      exit(1);
    }

  }
}

bool CmdRunner::isBuildin()
{
  bool ret = false;
  size_t i;
  for (i = 0; i < g_num_buildins; i++) {
    if (strcmp(g_buildins[i], _cmdinfo->cmds[0].argv[0]) == 0) {
      ret = true;
      g_buildin_table[i]();
    }
  }
  return ret;
}
