#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char **argv, char **env)
{
    char *args[] = {"echo", "-n -e","loay loay loay loat", NULL};
    char *path = "/bin/echo";
    int pid = fork();
    if(pid == 0)
    {
        execve(path, args, env);
        perror("execve");
        printf("NO RUN\n");
    }
    else
    {
        printf("PARENT\n");
        waitpid(pid, NULL, 0);
    }
    return 0;
}