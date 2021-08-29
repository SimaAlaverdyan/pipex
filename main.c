#include "pipex.h"

int main(int argc, char **argv)
{
    char *path;

    if (argc != 5)
        ft_exit("argument error, use ./pipex file1 cmd1 cmd2 file2");
    if ((path = get_env("PATH", environ)) == NULL)
        ft_exit("Path error");

    pipex(argv, path);
    return (0);
}