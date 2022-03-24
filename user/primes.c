#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process(int fd_read)
{
    int prime;
    if (read(fd_read, &prime, sizeof(int)) == 0)
    {
        close(fd_read);
        exit(0);
    }
    fprintf(1, "prime %d\n", prime);
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0)
    {
        close(p[1]);
        process(p[0]);
        exit(0);
    }
    else
    {
        close(p[0]);
        int n;
        while (read(fd_read, &n, sizeof(n)))
        {
            if (n % prime)
            {
                write(p[1], &n, 4);
            }
        }
        close(p[1]);
        wait(0);
        close(fd_read);
        exit(0);
    }
}

int main(int argc, char const *argv[])
{
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0)
    {
        close(p[1]);
        process(p[0]);
        exit(0);
    }
    else
    {
        close(p[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}
