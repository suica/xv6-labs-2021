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
        int received;
        read(p[0], &received, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], &received, 1);
        close(p[1]);
        exit(0);
    }
    else if (pid > 0)
    {
        int content = 233;
        write(p[1], &content, 1);
        int response;
        read(p[0], &response, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p[0]);
        exit(0);
    }
    else
    {
        fprintf(2, "fork error\n");
        exit(1);
    }
}
