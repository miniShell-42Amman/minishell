#include <unistd.h>

int main()
{
    int pid = fork();
    char *envs[2];
    envs[0] = "lol";
    envs[1] = NULL;
    char *args[2];
    args[0] = "./minishell";
    args[1] = NULL;
    if (!pid)
    {
        execve("./minishell", args, envs);
    }
    wait(0);
}