#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include "libft/libft.h"
#include <stdio.h>
#include <fcntl.h>

extern char     **environ;

void        pipex(char **argv, char *path);
void        ft_exit(char *str);
void        childpipe(int fd[2], int outfile, char *cmd2, char *path);
void        parentfpipe(int fd[2], int infile, char *cmd1, char *path);
char	    *get_env(char *key, char **env);
#endif