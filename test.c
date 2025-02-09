#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main (int ac, char **av)
{
    if (ac == 1)
    {
        printf("Usage: %s <command>\n", av[0]);
        return (1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (1);
    }
    if (pid == 0)
    {   
        int ret = ac / 2;
        int *array = malloc(sizeof(int) * ret);
        int i = 0;
        while (i < ret)
        {
            // int num1 = atoi(av[i + 1]);
            // int num2 = atoi(av[i + 2]);
            // printf("%d + %d = %d\n", num1, num2, num1 + num2);
            array[i] = atoi(av[i + 1]);
            i++;
        }

        int number = 0;
        while (i > 0)
        {
            int num1 = array[i - 1];
            int num2 = array[i - 2];
            number += num1 + num2;
            // printf("%d + %d = %d\n", num1, num2, num1 + num2);
            i -= 2;
        }
        printf("Result: %d prossec one\n", number);
        free(array);
    }
    else
    {
        waitpid(pid, NULL, 0);
        int *array2 = malloc(sizeof(int) * (ac / 2));
        if (array2 == NULL)
        {
            perror("malloc");
            return (1);
        }
        int i = 0;
        while (i < ac / 2)
        {
            array2[i] = atoi(av[i + ac / 2 + 1]);
            i++;
        }
        i = ac / 2;
        int number2 = 0;
        while (i > 0)
        {
            int num1 = array2[i - 1];
            int num2 = array2[i - 2];
            number2 += num1 + num2;
            i -= 2;
        }
        printf("Result: %d prossec two\n", number2);
        free(array2);
    }
        // int res = ac / 2;
        // int i = res;
        // while (ac > res)
        // {
        //     int num1 = atoi(av[i - 1]);
        //     int num2 = atoi(av[i - 2]);
        //     printf("%d + %d = %d\n", num1, num2, num1 - num2);
        //     i--;
        // }
    return (0);
}