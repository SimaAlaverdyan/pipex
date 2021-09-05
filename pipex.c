#include "pipex.h"
#include <string.h>
#include <signal.h>

void	ft_exit(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(0);
}
void    freematrix(char **matrix)
{
    int        i;

    i = -1;
    while ((matrix)[++i])
        free((matrix)[i]);
    free(matrix);
}
void    ft_execute(char *path, char **cmd)
{
    char		**tab;
	char		*tmp1;
	char		*tmp2;
	int			i;

	i = 0;
	tab = ft_split(path, ':');

	while (tab[i])
	{
		if (cmd[0][0] != '/')
        {
			tmp1 = ft_strjoin(tab[i], "/");
            tmp2 = ft_strjoin(tmp1, cmd[0]);
        }
		execve(tmp2, cmd, environ);
		free(tmp1);
		free(tmp2);
		++i;
	}
	freematrix(tab);
	ft_exit("Error: Invalid command");
}

char	*get_env(char *key, char **env)
{
	int		i;
	size_t	key_len;

	i = 0;
	key = ft_strjoin(key, "=");
	key_len = ft_strlen(key);
	while (env[i] != '\0')
	{
		if (ft_strncmp(env[i], key, key_len) == 0)
		{
			free(key);
			return (env[i] + key_len);
		}
		i++;
	}
	free(key);
	return (NULL);
}

void    childpipe(int fd[2], int outfile, char *cmd2, char *path)
{
    char    **args;

    close(fd[1]);
    args = ft_split(cmd2, ' ');
	
	if (dup2(fd[0], 0) == -1)
		exit(0);
	if (dup2(outfile, 1) == -1)
		exit(0);
    if (args[0][0] == '/') {
        if (execve(args[0], args, environ))
            ft_exit("Error: Invalid command");
    }
    else
        ft_execute(path, args);
	freematrix(args);
    close(outfile);
    close(fd[0]);
}

void    parentfpipe(int fd[2], int infile, char *cmd1, char *path)
{
    char	**args;

	close(fd[0]);
	args = ft_split(cmd1, ' ');
	if (dup2(infile, 0) == -1)
		ft_exit("Failed to connect infile with STDIN");
	if (dup2(fd[1], 1) == -1)
		ft_exit("Failed to connect STDOUT with pipe's WRITE END");
	if (args[0][0] == '/')
	{
		if (execve(args[0], args, environ) == -1)
			ft_exit("Error: Invalid command");
	}
	else
        ft_execute(path, args);

	freematrix(args);	
	close(infile);
	close(fd[1]);
	wait(NULL);
}

void    pipex(char **argv, char *path)
{
    pid_t   getpid; // fork()
    int     infile; //for reading
    int     outfile; //for writing
    int     fd[2]; //pipe argument

    infile = open(argv[1], O_RDONLY);
    if (infile == -1)
        ft_exit("Failed to open infile");
    outfile = open(argv[4], O_WRONLY);
    if (outfile == -1)
        ft_exit("Failed to open outfile");
    if (pipe(fd) == -1)
        ft_exit("Failed to pipe");

	if(!strcmp(argv[3], "") || !strcmp(argv[2], ""))
		ft_exit("empty command");

    getpid = fork();
    if (getpid == -1) 
        ft_exit("Failed to fork");
    else if (getpid == 0)               //child process
        childpipe(fd, outfile, argv[3], path);
    else                                //parent process
        parentfpipe(fd, infile, argv[2], path);
}

