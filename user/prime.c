#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0)
    {
        exit(0);
    }
    else
    {
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}
