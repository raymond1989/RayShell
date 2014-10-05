#ifndef CMDRUNNER_H
#define CMDRUNNER_H

class CmdInfo;

class CmdRunner {
  public:
    CmdRunner(CmdInfo *cmdinfo): _cmdinfo(cmdinfo) {}
    void run();
  private:
    void execOneCmd(int, char ** const argv, int fd_out = 1); 
    bool isBuildin();
    CmdInfo *_cmdinfo; 
};

#endif
