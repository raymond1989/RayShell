RayShell
========

a simple shell

Parser: 
    transform command line into CmdInfo structure
CmdRunner: 
    use backward recursive structure to represent a pipe-connected command;
    use dup2, pipe, fork
