#include "pipex.h"

void	ft_exit(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(0);
}

void    pipex(int argc, char **argv)
{
    printf("%d\n", argc);
    printf("%s\n", argv[1]);

    pid_t   getpid; // fork()
    int     infile; //for reading
    int     outfile; //for writing
    int     fd[2]; //pipe argument

    infile = open(argv[1], O_RDONLY);
    if (infile == -1)
        ft_exit("Failed to open infile");
    outfile = open(argv[argc - 1], O_WRONLY);
    if (outfile == -1)
        ft_exit("Failed to open outfile");
    if (pipe(fd) == -1) {
        ft_exit("Failed to pipe");
    }

    getpid = fork();

    if (getpid == -1) 
        ft_exit("Failed to fork");
    else if (getpid == 0) //child process
        childpipe();
    else //parent process
        parentfpipe();
    
    
}